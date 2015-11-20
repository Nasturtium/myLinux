#include <linux/module.h>
#include <linux/kernel.h>

void niitm_export(void);

int init_module(void)
{
    printk(KERN_INFO "init module2!!!!!\n");
    niitm_export();
    return 0;
}
