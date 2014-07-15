#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/fs.h>
#include <linux/kallsyms.h>
#include <linux/string.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chen Weixiang");
MODULE_DESCRIPTION("Iterate each buses");

static int __init mod_init(void)
{
	struct file_system_type *fs;
	char *sym_name = "file_systems";
	unsigned long sym_addr;

	pr_debug("=== insmod module ===\n");

	sym_addr = kallsyms_lookup_name(sym_name);

	fs = (struct file_system_type *)(*(unsigned long *)sym_addr);
	while (fs != NULL) {
		pr_debug("fs: %s \t\tfs_flags: %d\n", fs->name, fs->fs_flags);
		fs = fs->next;
	}

	return 0;
}

static void __exit mod_exit(void)
{
	pr_debug("=== rmmod module ===\n\n");
}

module_init(mod_init);
module_exit(mod_exit);
