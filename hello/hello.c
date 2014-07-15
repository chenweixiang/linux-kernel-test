#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/fs.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chen Weixiang");
MODULE_DESCRIPTION("A Hello Module");

static int __init hello_init(void)
{
	enum zone_type hzi;
	gfp_t gfp_mask;

	pr_debug("=== insmod module ===\n");

	/* __GFP_DMA | __GFP_HIGHMEM | __GFP_DMA32 | __GFP_MOVABLE */
	gfp_mask = __GFP_MOVABLE;
	hzi = gfp_zone(gfp_mask);

	pr_debug("hzi=%d\n", hzi);

	return 0;
}

static void __exit hello_exit(void)
{
	pr_debug("=== rmmod module ===\n\n");
}

module_init(hello_init);
module_exit(hello_exit);
