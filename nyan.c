#include <linux/fs.h>
#include <linux/init.h>
#include <linux/delay.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/miscdevice.h>
// #include <linux/string.h>
#include "animation.c"

#define FRAMES 12
#define FRAMESIZE ((FRAME_WIDTH+1) * FRAME_HEIGHT + 1)

static int nyan_open(struct inode *inode, struct file *file)
{
	pr_info("%s: nyan!\n", __func__);
	return 0;
}

static int nyan_release(struct inode *inode, struct file *file)
{
	pr_info("%s: nyan!\n", __func__);
	return 0;
};
static ssize_t nyan_read(struct file *file, char __user *buf, size_t count, loff_t *ppos)
{
	size_t available = FRAMESIZE * FRAMES;
	pr_info("%s: nyan count=%ld, pos=%lld\n", __func__, count, *ppos);
	loff_t pos = *ppos;
	size_t ret;

	if (count < FRAMESIZE)
		return -EINVAL;
	size_t frame = (pos % available) / FRAMESIZE;
	for (int line = 0; line < FRAME_HEIGHT; line++) {
		ret = copy_to_user(buf + line*(FRAME_WIDTH+1), frames[frame][line], FRAME_WIDTH);
		*ppos += FRAME_WIDTH - ret;
		ret = copy_to_user(buf + line*(FRAME_WIDTH+1) + FRAME_WIDTH, "\n", 1);
		*ppos += 1 - ret;
	}
	ret = copy_to_user(buf + FRAMESIZE-1, "\n\n", 1);
	*ppos += 1 - ret;
	msleep_interruptible(90);
	return FRAMESIZE;
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
