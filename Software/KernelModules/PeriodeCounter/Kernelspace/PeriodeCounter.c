#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/gpio.h>
#include <linux/time.h>
#include <linux/sched.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h> 
#include <linux/seq_file.h>
#include <linux/interrupt.h>
#include <linux/platform_device.h>

#define GPIO				17			
#define DEVICE 				"PeriodeCounter"
#define DEVICE_IRQ			"GPIO_IRQ"
#define PROC_FILE_NAME      "Resolution"
#define BUFFERSIZE			16

// Undefine the flag, because device tree support
// is not implemented atm.
#ifdef CONFIG_OF
	#undef CONFIG_OF
#endif

static char Resolution[] = "us\n";
static unsigned long High_us;
static unsigned long Low_us;
static unsigned long Diff_us;
static struct cdev* DriverObject;
static struct class* GPIOClass;
static struct device* GPIODev; 
static struct proc_dir_entry* ProcFile;
static struct proc_dir_entry* ProcDir;
static struct timespec High;
static struct timespec Low;
static char Buffer[BUFFERSIZE];
static uint32_t RPI_IRQ_Number;
static uint32_t ProcReadCounter; 
static dev_t GPIODevNumber;

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniel Kampert");
MODULE_DESCRIPTION("Raspberry Pi GPIO Interrupt Driver");

static ssize_t ProcOpen(struct inode* Inode, struct file* File);
static ssize_t ProcRead(struct file* Instanz, char __user* User, size_t ToCopy, loff_t* Offset);
static ssize_t ProcWrite(struct file* Instanz, const char __user* Buffer, size_t ToWrite, loff_t* Offset);

static struct file_operations ProcFOPS = {
	.owner = THIS_MODULE,
	.open = ProcOpen,
	.read = ProcRead,
	.write = ProcWrite,
};

#ifdef CONFIG_OF
	static int Probe(struct platform_device* Device);

	static struct platform_driver PlatformDriver = {
		.driver	= {
			.name = DEVICE,
			.owner = THIS_MODULE,
			.of_match_table = of_match_ptr(MatchID),
		},
		
		.probe = &Probe,
	};

	static const struct of_device_id MatchID[] = {
		{ 
			.compatible = "raspberrypi,3-model-b-plusbrcm,bcm2837"
		},
		{}
	};

	MODULE_DEVICE_TABLE(of, MatchID);
#else
	static int DriverOpen(struct inode* GeraeteDatei, struct file* Instanz);
	static ssize_t DriverRead(struct file* Instanz, char __user* User, size_t ToCopy, loff_t* Offset);
	static int DriverClose(struct inode* GeraeteDatei, struct file* Instanz);

	static struct file_operations FOPS = {
		.owner= THIS_MODULE,
		.open = DriverOpen, 
		.read= DriverRead,
		.release = DriverClose,
	};
#endif

// ISR for the interrupt request
static irqreturn_t GPIO_IRQ(int IRQ, void* Data)
{
	// Get the current time with each interrupt
	High = Low;
	ktime_get_ts(&Low);

	return IRQ_HANDLED;
}

static uint32_t ConfigGPIO(int Pin)
{
	uint32_t Error, IRQ_Number;
	char Name[20];

	snprintf(Name, sizeof(Name), "rpi-gpio-%d", Pin);
	Error = gpio_request(Pin, Name);
	if (Error) 
	{
		printk("'gpio_request' failed %d\n", Error);
		return -1;
	}

	// Configure the gpio as an input
	Error = gpio_direction_input(Pin);
	if (Error) 
	{
		printk("'gpio_direction_input' failed %d\n", Error);
		gpio_free(Pin);
		return -1;
	}

	// Get the interrupt number of the gpio
	IRQ_Number = gpio_to_irq(Pin);
	printk("'gpio_to_irq' returned %d\n", IRQ_Number);
	if (IRQ_Number < 0) 
	{
		printk("'gpio_to_irq' failed %d\n", IRQ_Number);
		gpio_free(Pin);
		return -1;
	}

	// Register the IRQ in the kernel
	Error = request_irq(IRQ_Number, GPIO_IRQ, IRQF_TRIGGER_FALLING, DEVICE_IRQ, DriverObject);
	printk("DriverObject: %p\n", DriverObject);
	if (Error) 
	{
		printk("'request_irq' failed with %d\n", Error);
		gpio_free(Pin);
		return -1;
	}
	printk("GPIO %d successfull configured!\n", Pin);

	return IRQ_Number;
}

#ifdef CONFIG_OF
	int Probe(struct platform_device* Device)
	{
		printk("Driver probe");
		return 0;
	}
#else
	static int DriverOpen(struct inode* GeraeteDatei, struct file* Instanz)
	{
		#ifdef DEBUG
			printk("'DriverOpen' called...\n");
		#endif  // DEBUG

		return 0;
	}

	static ssize_t DriverRead(struct file* Instanz, char __user* User, size_t ToCopy, loff_t* Offset)
	{
		ssize_t NotCopied, BytesToCopy;
		unsigned long Mult;
		unsigned long Div;

		if(strcmp(Resolution, "ms\n") == 0)
		{
			Mult = 1000;
			Div = 1000000;
		}
		else if(strcmp(Resolution, "us\n") == 0)
		{
			Mult = 1000000;
			Div = 1000;
		}
		else
		{
			Mult = 1000000000;
			Div = 1;
		}

		// Convert the time stamp to a time, accourding to the resolution settings
		High_us = (High.tv_sec * Mult) + (High.tv_nsec / Div);
		Low_us = (Low.tv_sec * Mult) + (Low.tv_nsec / Div);

		// Get the time difference
		if(High_us > Low_us)
		{
			Diff_us = High_us - Low_us;
		}
		else
		{
			Diff_us = Low_us - High_us;
		}

		// Copy the data to user space
		snprintf(Buffer, BUFFERSIZE, "%lu\n", Diff_us);
		BytesToCopy = min(ToCopy, sizeof(Buffer));
		if((NotCopied = copy_to_user(User, &Buffer, BytesToCopy)))
		{
			printk("Driver was not able to copy %lu byte\n", (unsigned long)NotCopied);
			return -EFAULT;
		}

		*Offset += BytesToCopy - NotCopied;

		return BytesToCopy - NotCopied;
	}

	static int DriverClose(struct inode* GeraeteDatei, struct file* Instanz)
	{
		#ifdef DEBUG
			printk("'DriverClose' called...\n");
		#endif // DEBUG

		return 0;
	}

	// Module init point
	static int __init Module_Init(void)
	{
		// Get a device number from the kernel
		if(alloc_chrdev_region(&GPIODevNumber, 0, 1, DEVICE) < 0)
		{
			return -EIO;
		}

		// Get a cdev object from the kernel
		DriverObject = cdev_alloc();
		if(DriverObject == NULL)
		{
			goto Jump_Free_DeviceNumber;
		}

		// Declare the owner and the file operations for the cdev object
		DriverObject->owner = THIS_MODULE;
		DriverObject->ops = &FOPS;

		// Register the driver in the system
		if(cdev_add(DriverObject,GPIODevNumber, 1))
		{
			goto Jump_Free_cdev;
		}

		// Create a struct class structure
		GPIOClass = class_create(THIS_MODULE, DEVICE);
		if(IS_ERR(GPIOClass)) 
		{
			pr_err("No udev support!\n");
			goto Jump_Free_cdev;
		}

		// Creates a device and registers it with sysfs
		GPIODev = device_create(GPIOClass, NULL, GPIODevNumber, NULL, "%s", DEVICE);
		if (IS_ERR(GPIODev))
		{
			goto Jump_Free_Class;
		}

		RPI_IRQ_Number = ConfigGPIO(GPIO);
		if (RPI_IRQ_Number < 0) 
		{
			goto Jump_Free_Device;
		}

		// Create proc-entry
		ProcDir = proc_mkdir(DEVICE, NULL);
		if(!ProcDir)
		{
			printk("Can not create proc-directory!");
			return -ENOMEM;	
		}

		ProcFile = proc_create_data(PROC_FILE_NAME, S_IWUGO, ProcDir, &ProcFOPS, NULL);
		if(!ProcFile)
		{
			printk("Can not create proc-file!");
			return -ENOMEM;
		}

		return 0;

		// Jumppoints for different Errorors during the init process
		// The jump points represent the inverse initialization process
		Jump_Free_Device:
			device_destroy(GPIOClass, GPIODevNumber);
		Jump_Free_Class:
			class_destroy(GPIOClass);
		Jump_Free_cdev:
			kobject_put(&DriverObject->kobj);
		Jump_Free_DeviceNumber:
			unregister_chrdev_region(GPIODevNumber, 1);
			return -EIO;
	}

	// Module exit point
	static void __exit Module_Exit( void )
	{
		// Remove proc-entry
		if(ProcFile)
		{
			remove_proc_entry(PROC_FILE_NAME, ProcDir);
		}

		// Remove proc-dir
		if(ProcDir)
		{
			remove_proc_entry(DEVICE, NULL);
		}

		// Removes the created device
		device_destroy(GPIOClass, GPIODevNumber);

		// Remove the class object 
		class_destroy(GPIOClass);

		// Unregister the cdev object
		cdev_del(DriverObject);

		// Release the registered device number
		unregister_chrdev_region(GPIODevNumber, 1);

		// Unregister the IRQ
		free_irq(RPI_IRQ_Number, DriverObject);

		// Mark the used gpio as free
		gpio_free(GPIO);

		return;
	}
#endif

/****************************
 * 							*
 * 		Proc operations		*
 * 							*
 ****************************/
ssize_t ProcOpen(struct inode* Inode, struct file* File) 
{
	ProcReadCounter = 0;

    return 0;
}

ssize_t ProcRead(struct file* Instanz, char __user* User, size_t ToCopy, loff_t* Offset)
{
	ssize_t NotCopied, BytesToCopy;

	if(ProcReadCounter == 0)
	{
		ProcReadCounter++;

		BytesToCopy = min(ToCopy, sizeof(Resolution));
		if((NotCopied = copy_to_user(User, &Resolution, BytesToCopy)))
		{
			printk("Driver was not able to copy %lu byte\n", (unsigned long)NotCopied);
			return -EFAULT;
		}

		*Offset += BytesToCopy - NotCopied;

		return BytesToCopy - NotCopied;
	}

	return 0;
}

ssize_t ProcWrite(struct file* Instanz, const char __user* User, size_t ToWrite, loff_t* Offset)
{
	ssize_t ToCopy, NotCopied;

	ToCopy = min(ToWrite, sizeof(Resolution));

	if(ToCopy == 3)
	{	
		// Copy data from userspace to kernelspace
		NotCopied = copy_from_user(Resolution, User, ToCopy);

		// Save the data if all data are copied
		if(NotCopied == 0)
		{
			printk("Set new resolution to: %s", Resolution);
		}

		return ToCopy - NotCopied;
	}

	return ToCopy;
}

#ifdef CONFIG_OF
	module_platform_driver(PlatformDriver);	
#else
	module_init(Module_Init);
	module_exit(Module_Exit);
#endif