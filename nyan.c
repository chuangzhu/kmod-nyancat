#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static int __init nyan_init(void)
{
	printk(KERN_INFO "Hello nyan!\n");
	return 0;
}

static void __exit nyan_exit(void)
{
	printk(KERN_INFO "Goodbye nyan!\n");
}

module_init(nyan_init);
module_exit(nyan_exit);
MODULE_LICENSE("MIT");
