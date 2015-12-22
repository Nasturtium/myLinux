#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/errno.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>


//cmd ioctl
#define IOCTL_GET_STATISTIC 0x456

#define MY_NAME "kbuf"

typedef struct
{
	int num_open;		//statistic
	int num_release;
	int num_read;
	int num_write;
	
}STATISTIC_CHARDEV;

static STATISTIC_CHARDEV statistic;             //struct!!!! chardev

static const int major = 800;
static const int minor = 0;
struct cdev *my_cdev;

//struct net_device *dev;


static char BUF[100];


int chardev_open(struct inode *inode,struct file *file)
{
        printk(KERN_INFO "function OPEN() \n");
	statistic.num_open++;
        return 0;
}

int chardev_release(struct inode *inode,struct file *file)
{
        printk(KERN_INFO "function RELEASE() \n");
	statistic.num_release++;
        return 0;
}

static ssize_t chardev_read(struct file *file, char __user *buf, size_t lbuf, loff_t *ppos)
{
	int nbytes = 0;
	int nbytes_user = 1;
	printk(KERN_INFO "function READ() \n");
	statistic.num_read++;

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
	statistic.num_write++;

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

static const struct file_operations chardev_fops = {
							.owner = THIS_MODULE,
							.open = chardev_open,
							.release = chardev_release,
							.read = chardev_read,
							.write = chardev_write,
						};


int mynet_netdev_open(struct net_device *dev)
{

	return 0;
}

int mynet_netdev_stop(struct net_device *dev)
{
 
        return 0;
}

netdev_tx_t mynet_netdev_start_xmit(struct sk_buff *skb, struct net_device *dev)
{

	return 0;
//	return XMIT_DROP;
}


static const struct net_device_ops mynet_netdev_ops = {
							.ndo_open = mynet_netdev_open,
							.ndo_stop = mynet_netdev_stop,
							.ndo_start_xmit = mynet_netdev_start_xmit,
							//.ndo_tx_timeout = mynet_netdev_tx_timeout,
							//.ndo_get_stats = mynet_netdev_get_stats,
						};

struct mynet_device {
			struct net_device *dev;
			int count_tr;
			// ?????
			};


static void mynet_netdev_init(struct net_device *dev)
{
	struct mynet_device *priv = netdev_priv(dev);

	//??????????
	ether_setup(dev);
	//dev->watchdog_timeo = timeout;
	dev->netdev_ops = &mynet_netdev_ops;
	dev->flags |=IFF_NOARP;	

}


static struct net_device *mynet_netdev_create(const char *name)
{
	struct net_device *dev;
	
	dev = alloc_netdev(sizeof(struct mynet_device),name,NET_NAME_UNKNOWN,mynet_netdev_init);
	if(register_netdev(dev))
	{
	      printk(KERN_INFO "ERROR to register\n" ); 
	      free_netdev(dev); 
	      return -1; 
	}
	//printk(KERN_INFO "Loading network module \n)"; 
	return dev;
}

static int init_mynet(void)
{

	struct net_device *dev = mynet_netdev_create("mynet1");	
	return 0;
}

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

	//my init
	init_mynet();

	return 0;
}

static void exit_func(void)//struct net_device *dev)
{
	dev_t first_node = MKDEV(major,minor);
	struct net_device *dev;
	
	printk(KERN_INFO "exit module ! \n");
	cdev_del(my_cdev);
	unregister_chrdev_region(first_node, 1);
	//mynet
	unregister_netdev(dev);
	free_netdev(dev);
}

module_init(init_func);
module_exit(exit_func);
