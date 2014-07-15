#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/fs.h>
#include <linux/kallsyms.h>
#include <linux/string.h>

#include "fs_init.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chen Weixiang");
MODULE_DESCRIPTION("Check Filesystem Module");

static int __init mod_init(void)
{
	initcall_t *fn;
	int cnt;

	pr_debug("=== insmod module ===\n");

	cnt = 0;
	for (fn = __early_initcall_end; fn < __initcall_end; fn++)
		pr_debug("%d: %p\n", cnt++, fn);

	return 0;
}

static void __exit mod_exit(void)
{
	pr_debug("=== rmmod module ===\n\n");
}

module_init(mod_init);
module_exit(mod_exit);
