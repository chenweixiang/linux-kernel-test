#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/fs.h>
#include <linux/genhd.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chen Weixiang");
MODULE_DESCRIPTION("A Hello Module");

static int __init hello_init(void)
{
	char *uuid;
	dev_t devname = 1;

	pr_debug("Before devname=%d\n", devname);

	/* uuid = "fe67c2d0-9b0f-4fd6-8e97-463ce95a7e0c"; */
	uuid = "4d735370-825b-411f-9167-090146a8dd09";
	devname = blk_lookup_devt(uuid, 0);

	pr_debug("After devname=%d\n", devname);

	return 0;
}

static void __exit hello_exit(void)
{
}

module_init(hello_init);
module_exit(hello_exit);
