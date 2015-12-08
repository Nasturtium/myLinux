#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define MY_NAME "kbuf"

static const int major = 800;
static const int minor = 0;
struct cdev *my_cdev;


static char BUF[100];

int chardev_open(struct inode *inode,struct file *file)
{
        printk(KERN_INFO "function OPEN() \n");
        return 0;
}
 
static ssize_t chardev_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = 0;
	int nbytes_user = 1;
	printk(KERN_INFO "function READ() \n");

	nbytes_user = copy_to_user(BUF,buf,lbuf);
	if(nbytes_user != 0)
	{
		printk(KERN_INFO "ERROR copy_to_user()! \n");
		return -EINTR;
	}
	nbytes = lbuf - nbytes_user;
	printk(KERN_INFO "READ nbytes =%d \n",nbytes);

	*ppos =+ nbytes;

        return nbytes;
}
 
static ssize_t chardev_write(struct file *file, const char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = 0;
	int nbytes_user = 1;
	printk(KERN_INFO "function WRITE() \n");

	nbytes_user = copy_from_user(BUF,buf,lbuf);
	printk(KERN_INFO "str38 nbytes_user=%d BUF=%c \n",nbytes_user,BUF[0]);
	
	if(nbytes_user != 0)
	{
		printk(KERN_INFO "ERROR copy_from_user! \n");
		return -EINTR;
	}
	nbytes = lbuf - nbytes_user;
        printk(KERN_INFO "str46 nbytes=%d \n",nbytes);

	*ppos =+ nbytes;

	return nbytes;
        //return -EINTR;
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

	if(register_chrdev_region(first_node,1,"kbuf")<0)
	{
		unregister_chrdev_region(first_node, 1);
		printk(KERN_INFO "ERROR register_chrdev_region ! ! ! \n");
		return -EINTR;
	}

	my_cdev = cdev_alloc();
        cdev_init(my_cdev,&chardev_fops);

        if(cdev_add(my_cdev,first_node,1)<0)
	{
		unregister_chrdev_region(first_node, 1);
		printk(KERN_INFO "ERROR cdev_add ! ! ! \n");
		return -EINTR;
	}

	return 0;
}

static void exit_func(void)
{
	dev_t first_node = MKDEV(major,minor);

	printk(KERN_INFO "exit module ! \n");
	cdev_del(my_cdev);
	unregister_chrdev_region(first_node, 1);
}

module_init(init_func);
module_exit(exit_func);
