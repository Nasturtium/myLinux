#include <linux/module.h>

#define NIITM_NAME "hello"

static int niitm_init(void)
{
	printk(KERN_INFO "Loaded %s \n",NIITM_NAME);
	return 0;
}

static void niitm_exit(void)
{
	printk(KERN_INFO "Unloaed %s \n",NIITM_NAME);
}

module_init(niitm_init);
module_exit(niitm_exit);
