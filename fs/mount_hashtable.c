#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/fs.h>
#include <linux/kallsyms.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/mount.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chen Weixiang");
MODULE_DESCRIPTION("Check Filesystem Module");

static char *fsname = NULL;
module_param(fsname, charp, 0444);
MODULE_PARM_DESC(fsname, " show info of specified file system with name fsname");

static void mod_main()
{
	char *sym_name = "mount_hashtable";
	unsigned long sym_addr;

	struct list_head *ptr;
	struct list_head *pos;

	struct vfsmount *vfsmnt;

	sym_addr = kallsyms_lookup_name(sym_name);
	ptr = (struct list_head *)(*(unsigned long *)sym_addr);

	if (ptr->mnt_sb == NULL) {
		pr_debug("*** mount_hashtable->mnt_sb is NULL\n");
		return;
	} else {

		pr_debug("*** mount_hashtable ***\n");

		/* mnt_sb */
		if (ptr->mnt_sb != NULL &&
		    ptr->mnt_sb->s_type != NULL &&
		    ptr->mnt_sb->s_type->name != NULL) {
			pr_debug("    mnt_sb->s_type->name: %s\n", ptr->mnt_sb->s_type->name);
		} else {
			pr_debug("    mnt_sb->s_type->name: don't know\n");
		}
	}
}

static int __init mod_init(void)
{
	pr_debug("=== insmod module ===\n");

	mod_main();

	return 0;
}

static void __exit mod_exit(void)
{
	pr_debug("=== rmmod module ===\n\n");
}

module_init(mod_init);
module_exit(mod_exit);
