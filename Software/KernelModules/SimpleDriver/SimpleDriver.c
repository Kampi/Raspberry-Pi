#include <linux/fs.h>
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/device.h>
#include <linux/module.h>
#include <linux/uaccess.h>

#define DEVICENAME 			"Hello"
#define MESSAGE				"Hello World!\n"
#define HELLO_MINOR			0

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Daniel Kampert");
MODULE_DESCRIPTION("'Hello World' virtual device");

static dev_t HelloDevNumber;
static struct cdev* DriverObject;
static struct class* HelloClass;
static struct device* HelloDev;
static int MessageCount = 0;

// Function prototypes
static ssize_t DriverRead(struct file* instanz, char __user* user, size_t count, loff_t* offset);
static int DriverOpen(struct inode* geraete_datei, struct file* instanz);
static int DriverClose(struct inode* geraete_datei, struct file* instanz);

// File operations
static struct file_operations FOPS = {
	.owner = THIS_MODULE,
	.read = DriverRead,
	.open = DriverOpen, 
	.release = DriverClose,
};

static int DriverOpen(struct inode* geraete_datei, struct file* instanz)
{
	dev_info(HelloDev, "'DriverOpen' called...\n");

	MessageCount = 0;

	return 0;
}

static int DriverClose(struct inode* geraete_datei, struct file* instanz)
{
	dev_info(HelloDev, "'DriverClose' called...\n");

	return 0;
}

static ssize_t DriverRead(struct file* instanz, char __user* user, size_t count, loff_t* offset)
{
	unsigned long not_copied, to_copy;

	if(MessageCount == 0)
	{
		to_copy = min(count, strlen(MESSAGE) + 1);
		not_copied = copy_to_user(user, MESSAGE, to_copy);
		*offset += to_copy - not_copied;
		MessageCount++;

		return to_copy - not_copied;
	}

	return 0;
}

// Module init point
static int __init Module_Init(void)
{
	// Register a range of char device numbers
	if (alloc_chrdev_region(&HelloDevNumber, HELLO_MINOR, 1, DEVICENAME) < 0)
	{
		return -EIO;
	}

	// Get a cdev object from the kernel
	DriverObject = cdev_alloc();
	if(DriverObject == NULL)
	{
		goto Jump_Free_DeviceNumber;
	}

	DriverObject->owner = THIS_MODULE;
	DriverObject->ops = &FOPS;

	// Add a char device to the system
	if(cdev_add(DriverObject, HelloDevNumber, 1))
	{
		goto Jump_Free_cdev;
	}

	// Create a struct class structure
	HelloClass = class_create(THIS_MODULE, DEVICENAME);
	if(IS_ERR(HelloClass)) 
	{
		pr_err("No udev support!\n");
		goto Jump_Free_cdev;
	}

	// Creates a device and registers it with sysfs
	HelloDev = device_create(HelloClass, NULL, HelloDevNumber, NULL, "%s", DEVICENAME);
	if(IS_ERR(HelloDev)) 
	{
		pr_err("'device_create' failed!\n");
		goto Jump_Free_class;
	}

	return 0;

	// Jumppoints for different errors during the init process
	// The jump points represent the inverse initialization process
	Jump_Free_class:
		class_destroy(HelloClass);
	Jump_Free_cdev:
		kobject_put(&DriverObject->kobj);
	Jump_Free_DeviceNumber:
		unregister_chrdev_region(HelloDevNumber, 1);
		return -EIO;
}

// Module exit point
static void __exit Module_Exit(void)
{
	// Removes the created device
	device_destroy(HelloClass, HelloDevNumber);

	// Remove the class object 
	class_destroy(HelloClass);

	// Unregister the cdev object
	cdev_del(DriverObject);

	// Release the registered device number
	unregister_chrdev_region(HelloDevNumber, 1);

	return;
}

module_init(Module_Init);
module_exit(Module_Exit);
