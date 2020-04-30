#include <linux/fs.h>
#include <linux/version.h>
#include <linux/module.h>
#include <linux/init.h>
#include <asm/uaccess.h>  
#include <linux/ioctl.h>
#include <asm/io.h>

// GPIO controller 
#define Peripherie_Basis	0x20000000						
#define GPIO_Basis			(Peripherie_Basis + 0x200000)

// Driver informations
#define NAME				"Simple_LED"
#define DRIVER_MAJOR 		240

// Author
MODULE_AUTHOR("Daniel Kampert");
MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple LED Treiber");
MODULE_SUPPORTED_DEVICE("Raspberry Pi");       

static volatile uint32_t* gpioRegs; 
static atomic_t BytesToRead = ATOMIC_INIT(4);
static atomic_t BytesToWrite = ATOMIC_INIT(6);

static int driver_open(struct inode* geraete_datei, struct file* instanz)
{
    // Mask memory region
    gpioRegs = (uint32_t *)ioremap(GPIO_Basis, 0xB4);
	
    // Set the I/O as output
    *(gpioRegs + 0x01) = (0x1 << 0x15);

    printk("Open device...\n");
	
    return 0;
}

static int driver_close(struct inode* geraete_datei, struct file* instanz)
{
    printk("Close device...\n");
    return 0;
}

static ssize_t driver_read(struct file *instanz, char* User, size_t Count, loff_t* offset)
{
    size_t BytesToCopy;
    size_t BytesNotCopied;
    uint32_t Status; 

	// Read the input register and get the state
    Status = *(gpioRegs + 0x0D);
    Status &= (0x01 << 0x11);
    Status >>= 0x11;

    printk("Read status: %i\n", Status);
	
    // Copy data to userspace
    BytesToCopy = min((size_t)atomic_read(&BytesToRead), Count);
    BytesNotCopied = copy_to_user(User, &Status, BytesToCopy);
    *offset += BytesToCopy - BytesNotCopied;

    return BytesToCopy - BytesNotCopied;
}

static ssize_t driver_write(struct file* Instanz, const char* Buffer, size_t Count, loff_t* offset)
{
    size_t BytesToCopy;
    size_t BytesNotCopied;
    char Input_Buffer[&BytesToWrite] = "";
	
    // Copy data from userspace
    BytesToCopy = min((size_t)atomic_read(&BytesToWrite), Count);
    BytesNotCopied = copy_from_user(Input_Buffer, Buffer, BytesToCopy);
    *offset += BytesToCopy - BytesNotCopied;
	
    // Catch wrong inputs and switch I/O
    if(!(strcmp(Input_Buffer, "1")))
    {
        printk("Switch on\n");
		*(gpioRegs + 0x07) = (0x01 << 0x11);
	}
	else if(!(strcmp(Input_Buffer, "0")))
	{
		printk("Switch off\n");
		*(gpioRegs + 0x0A) = (0x01 << 0x11);
	}
	else
	{
		printk("Unknown command!\n");
	}
	
    return BytesToCopy - BytesNotCopied;
}

// File operations
static struct file_operations fops = {                     
    .owner= THIS_MODULE,
    .read= driver_read,
    .write= driver_write,
    .open= driver_open, 
    .release= driver_close,
};

// Initialize the module
static int __init Init(void)
{
    if(register_chrdev(DRIVER_MAJOR, NAME, &fops) == 0x00) 
    {
        printk("Driver loaded...\n");
        return 0;
    }
    else
    {
        printk("Error during initialization!\n");
        return -EIO;
    }
}

// Exit the module
static void __exit Exit(void)
{
    unregister_chrdev(DRIVER_MAJOR, NAME);  
}

module_init(Init);
module_exit(Exit);