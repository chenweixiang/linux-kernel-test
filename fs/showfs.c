#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/fs.h>
#include <linux/kallsyms.h>
#include <linux/string.h>

MODULE_LICENSE("GPL"); 
MODULE_AUTHOR("Chen Weixiang");
MODULE_DESCRIPTION("Check Filesystem Module");

static void mod_main(void)
{
	struct file_system_type *fs;
	char *sym_name = "file_systems";
	unsigned long sym_addr;
	unsigned int sum = 0;

	sym_addr = kallsyms_lookup_name(sym_name);

	fs = (struct file_system_type *)(*(unsigned long *)sym_addr);
	while (fs != NULL)
	{
		printk("fs: %s \t\tfs_flags: 0x%08X\n", fs->name, fs->fs_flags);
		fs = fs->next;
        sum++;
	}
	printk("=== total %d elements in list file_systems ===\n", sum);
}

static int __init mod_init(void)
{
	printk("=== insmod module ===\n");
	mod_main();
	return 0;
}

static void __exit mod_exit(void)
{
	printk("=== rmmod module ===\n\n");
}

module_init(mod_init);
module_exit(mod_exit);

