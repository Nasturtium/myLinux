#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/cdev.h>

#define MY_NAME "kbuf"

static const int major = 800;
static const int minor = 0;

struct cdev *my_cdev;

int chardev_open(struct inode *inode,struct file *file)
{
        printk(KERN_INFO "function OPEN() \n");
        return -EINTR;
}
 
static ssize_t chardev_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
         //int my_read = 1;
         printk(KERN_INFO "function READ() \n");
         return -EINTR;
}
 
static ssize_t chardev_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
         printk(KERN_INFO "function WRITE() \n");
         return -EINTR;
}

static const struct file_operations chardev_fops =
						{
						.owner = THIS_MODULE,
						.open = chardev_open,
						.read = chardev_read,
						.write = chardev_write,
						};
static int init_func(void)
{
	dev_t first_node = MKDEV(major,minor);

	printk(KERN_INFO "init module ! ! ! \n");

	register_chrdev_region(first_node,1,"kbuf");
	my_cdev = cdev_alloc();

	register_chrdev(major,MY_NAME,&chardev_fops);
        cdev_init(my_cdev,&chardev_fops);
        cdev_add(my_cdev,first_node,minor);

	return 0;
}

static void exit_func(void)
{
	printk(KERN_INFO "exit module ! \n");
}

module_init(init_func);
module_exit(exit_func);
