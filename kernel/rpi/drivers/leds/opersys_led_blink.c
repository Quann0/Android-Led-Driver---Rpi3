#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
/* Meta Information */
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Johannes 4 GNU/Linux");
MODULE_DESCRIPTION("GPIO Example");

/* Variables for device and device class */
static dev_t my_device_nr;
static struct class *my_class;
static struct cdev my_device;

#define DRIVER_NAME "my_misc_device"
#define DRIVER_CLASS "blink_class"

/**
 * @brief Read data out of the buffer
 */
static ssize_t driver_read(struct file *File, char *user_buffer, size_t count, loff_t *offs) {
/*
	char data = *user_buffer;	
	if(data == '1')
		printk("Set high for pin 7");
		gpio_set_value(7,1);
	else if(data == '0') 
		printk("Set low for pin 7");
		gpio_set_value(7,0);
	else printk("Invalid value!\n");
*/	
	printk(KERN_INFO "Read Function Called!\n");
	/* Copy data to user */
	//copy_to_user(user_buffer, data, strlen(&data));
	//printk("Data = %c", data + '0');

	return count;
}

/**
 * @brief Write data to buffer
 */
static ssize_t driver_write(struct file *File, const char __user *user_buffer, size_t count, loff_t *offs) {
      
	int size;
	char data;
	
	size = min(count, sizeof(data));
	copy_from_user(&data, user_buffer, size);
	printk("data from user: %c", data);
        if(data == '1')
	{
                printk("Set high for pin 3");
                gpio_set_value(3,1);
	}       
	 else if(data == '0')
	{
                printk("Set low for pin 3");
                gpio_set_value(3,0);
	}
        else printk("Invalid value!\n");
	
	
	
	
	return size;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_open(struct inode *device_file, struct file *instance) {
	printk("dev_nr - open was called!\n");
	return 0;
}

/**
 * @brief This function is called, when the device file is opened
 */
static int driver_close(struct inode *device_file, struct file *instance) {
	printk("dev_nr - close was called!\n");
	return 0;
}

static struct file_operations fops = {
	.owner = THIS_MODULE,
	.open = driver_open,
	.release = driver_close,
	.read = driver_read,
	.write = driver_write
};

/**
 * @brief This function is called, when the module is loaded into the kernel
 */
static int __init ModuleInit(void) {
	int retval;
	printk("Hello, Kernel!\n");

	/* Allocate a device nr */
	if( alloc_chrdev_region(&my_device_nr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device Nr. could not be allocated!\n");
		return -1;
	}
	printk("read_write - Device Nr. Major: %d, Minor: %d was registered!\n", my_device_nr >> 20, my_device_nr && 0xfffff);

	/* Create device class */
	if((my_class = class_create(THIS_MODULE, DRIVER_CLASS)) == NULL) {
		printk("Device class can not be created!\n");
		goto ClassError;
	}

	/* create device file */

	if(device_create(my_class, NULL, my_device_nr, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto FileError;
	}

	/* Initialize device file */
	cdev_init(&my_device, &fops);

	/* Regisering device to kernel */
	if(cdev_add(&my_device, my_device_nr, 1) == -1) {
		printk("Registering of device to kernel failed!\n");
		goto AddError;
	}
	
	/* GPIO init - PIN 4 */
	if(gpio_request(4, "rpi-gpio-4")) 
	{
		printk(KERN_ERR  "Error: Gpio_request failed!\n");
		goto fail_gpio_init;
	}

	printk(KERN_INFO "Init GPIO 4 successful!\n");
	if(gpio_request(3, "rpi-gpio-3"))
	{
		printk(KERN_ERR "Error: Gpio_request failed!\n");
		goto fail_gpio_init;
	}
	printk("Init GPIO 7 successful!\n");
	/* Set direction for PIN */
	if(gpio_direction_output(3, 0))
	{
		printk(KERN_ERR "Error: Set Direction failed!\n");
		goto fail_set_direction_pin_7;
	}
	printk("Set OUTPUT for pin 7 done!\n");
	if(gpio_direction_input(4))
        {
                printk(KERN_ERR "Error: Set Direction failed!\n");
                goto fail_set_direction_pin_4;
        }
	printk("Set INPUT for pin 4 done!\n");
	gpio_set_value(3,0);
	return 0;
fail_set_direction_pin_4:
	gpio_free(4);
fail_set_direction_pin_7:
	gpio_free(7);
fail_gpio_init:
	
AddError:
	device_destroy(my_class, my_device_nr);
FileError:
	class_destroy(my_class);
ClassError:
	unregister_chrdev_region(my_device_nr, 1);
	return -1;
}

/**
 * @brief This function is called, when the module is removed from the kernel
 */
static void __exit ModuleExit(void) {
	gpio_set_value(3,0);
	gpio_free(4);
	gpio_free(3);
	cdev_del(&my_device);
	device_destroy(my_class, my_device_nr);
	class_destroy(my_class);
	unregister_chrdev_region(my_device_nr, 1);
	printk("Goodbye, Kernel\n");
}

module_init(ModuleInit);
module_exit(ModuleExit);
