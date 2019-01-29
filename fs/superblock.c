#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/fs.h>
#include <linux/kallsyms.h>
#include <linux/string.h>
#include <linux/list.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chen Weixiang");
MODULE_DESCRIPTION("Check Filesystem Module");

static char *fsname = NULL;
module_param(fsname, charp, 0444);
MODULE_PARM_DESC(fsname, " show info of specified file system with name fsname");

static int __init mod_init(void)
{
	char *sym_name = "super_blocks";
	unsigned long sym_addr;

	struct list_head *ptr;
	struct list_head *pos;

	struct super_block *sb;
	int sb_cnt = 0;
	int sb_idx = 0;

	pr_debug("=== insmod module ===\n");

	sym_addr = kallsyms_lookup_name(sym_name);
	ptr = (struct list_head *)(*(unsigned long *)sym_addr);

	if (list_empty(ptr)) {
		pr_debug("*** super_blocks is empty\n");
		return 0;
	}

	list_for_each(pos, ptr)
		sb_cnt++;

	list_for_each(pos, ptr) {
		sb_idx++;
		sb = list_entry(pos, struct super_block, s_list);

		if (sb->s_type != NULL && sb->s_type->name != NULL && fsname != NULL && strcmp(fsname, sb->s_type->name) != 0)
			continue;

		pr_debug("*** (%d/%d) super_blocks ***\n", sb_idx, sb_cnt);

		/* s_dev */
		pr_debug("    super_blocks[%d]->s_dev: MAJOR %d, MINOR %d\n", sb_idx-1, MAJOR(sb->s_dev), MINOR(sb->s_dev));

		/* s_type */
		if (sb->s_type == NULL)
			pr_debug("    super_blocks[%d]->s_type: NULL\n", sb_idx-1);
		else if (sb->s_type->name != NULL)
			pr_debug("    super_blocks[%d]->s_type->name: %s\n", sb_idx-1, sb->s_type->name);

		/* s_blocksize_bits, s_blocksize */
		pr_debug("    super_blocks[%d]->s_blocksize_bits: %u\n", sb_idx-1, sb->s_blocksize_bits);
		pr_debug("    super_blocks[%d]->s_blocksize: %lu\n", sb_idx-1, sb->s_blocksize);
	}

	return 0;
}

static void __exit mod_exit(void)
{
	pr_debug("=== rmmod module ===\n\n");
}

module_init(mod_init);
module_exit(mod_exit);
