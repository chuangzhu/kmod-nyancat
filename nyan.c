#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
// #include "animation.c"

static int nyan_open(struct inode *inode, struct file *file)
{
	pr_info("%s: nyan!\n", __func__);
	return 0;
}

static int nyan_release(struct inode *inode, struct file *file) {
	pr_info("%s: nyan!\n", __func__);
	return 0;
};
static ssize_t nyan_read(struct file *file, char __user *buf, size_t count, loff_t *ppos) {
	// return simple_read_from_buffer(buf, count, ppos, "nyan!\n", 6);

	char *from = "nyan!\n";
	size_t available = strlen(from);
	loff_t pos = (*ppos) % available;
	size_t ret;
	pr_info("%s: nyan count=%ld, pos=%lld\n", __func__, count, pos);

	msleep_interruptible(90);

	if (pos < 0)
		return -EINVAL;
	// if (pos >= available || !count)
	// 	return 0; // EOF
	if (count > available - pos)
		count = available - pos;
	ret = copy_to_user(buf, from + pos, count);
	if (ret == count)
		return -EFAULT;
	count -= ret;
	*ppos = pos + count;
	return count;
};

static struct file_operations nyan_fops = {
	.owner = THIS_MODULE,
	.open = nyan_open,
	.release = nyan_release,
	.read = nyan_read,
};
static struct miscdevice nyan_miscdevice = {
	.minor = MISC_DYNAMIC_MINOR,
	.name = "nyan",
	.fops = &nyan_fops,
	.mode = S_IRUGO,
};

static int __init nyan_init(void)
{
	int ret;
	pr_info("%s: Hello nyan!\n", __func__);
	ret = misc_register(&nyan_miscdevice);
	if (ret < 0)
		pr_err("%s: Failed to setup miscdev nyan!\n", __func__);
	return ret;
}

static void __exit nyan_exit(void)
{
	misc_deregister(&nyan_miscdevice);
	pr_info("%s: Goodbye nyan!\n", __func__);
}

module_init(nyan_init);
module_exit(nyan_exit);
MODULE_LICENSE("MIT");
