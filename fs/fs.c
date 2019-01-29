#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/fs.h>
#include <linux/kallsyms.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/dcache.h>

/*
 * Build commands:
 *   make clean
 *   make o=fs
 *
 * Insert module and check print-out:
 *   sudo insmod fs.ko fsname=ext4
 *   sudo rmmod fs
 *   cat /var/log/syslog
 */

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chen Weixiang");
MODULE_DESCRIPTION("Check Filesystem Module");

#define PREFIXBUFFLEN 50

static char *fsname = NULL;
module_param(fsname, charp, 0444);
MODULE_PARM_DESC(fsname, " show info of specified file system with name fsname. Default: NULL");

static int inodecnt = 5;
module_param(inodecnt, int, 0444);
MODULE_PARM_DESC(inodecnt, " show first elements in list fs_supers[]->s_inodes. Default: 5");


void print_dentry(struct dentry *pdentry, char *prefix)
{
	if (pdentry == NULL) {
		pr_debug("            %s is NULL\n", prefix);
		return;
	}

	pr_debug("            %s->d_iname: %s\n", prefix, pdentry->d_iname);

	/* d_name */
	if (pdentry->d_name.name != NULL)
		pr_debug("            %s->d_name.name: %s\n", prefix, pdentry->d_name.name);

	/* d_flags */
	pr_debug("            %s->d_flags: %d\n", prefix, pdentry->d_flags);

	/* d_seq */
	pr_debug("            %s->d_seq.sequence: %d\n", prefix, pdentry->d_seq.sequence);

	/* d_hash */

	/* d_time */
	pr_debug("            %s->d_time: %lu\n", prefix, pdentry->d_time);

	/* d_subdirs */
	if (list_empty(&pdentry->d_subdirs)) {
		pr_debug("            %s->d_subdirs is NULL\n", prefix);
	} else {
		int subdircnt = 0;
		int suddiridx = 0;
		struct dentry *dentry_ptr;

		struct list_head *subdirs_pos;
		list_for_each(subdirs_pos, &pdentry->d_subdirs)
			subdircnt++;
		pr_debug("            %s->d_subdirs has %d in total\n", prefix, subdircnt);

		list_for_each(subdirs_pos, &pdentry->d_subdirs) {
			dentry_ptr = list_entry(subdirs_pos, struct dentry, d_u.d_child);
			if (dentry_ptr != NULL && dentry_ptr->d_name.name != NULL)
				pr_debug("                d_subdirs[%d]->d_name.name: %s\n",
					 suddiridx, dentry_ptr->d_name.name);

			suddiridx++;
		}
	}
}


static int __init mod_init(void)
{
	char *sym_name = "file_systems";
	unsigned long sym_addr;

	struct file_system_type *ptr;
	int fs_cnt = 0;

	char buff[KSYM_SYMBOL_LEN];
	struct hlist_node *pos;
	int sb_cnt = 0;

	char prefixbuff[PREFIXBUFFLEN];

	pr_debug("=== insmod module ===\n");

	sym_addr = kallsyms_lookup_name(sym_name);

	ptr = (struct file_system_type *)(*(unsigned long *)sym_addr);
	while (ptr != NULL) {
		/* only print specific file system info */
		if (fsname != NULL && strcmp(fsname, ptr->name) != 0)
			goto next_loop;

		pr_debug("*** (%d) fs: %s ***\n", fs_cnt, ptr->name);

		/* ptr->fs_flags */
		pr_debug("    fs_flags: %d\n", ptr->fs_flags);

		/* ptr->mount */
		if (ptr->mount != NULL) {
			memset(buff, '\0', KSYM_SYMBOL_LEN);
			sprint_symbol(buff, (unsigned long)ptr->mount);
			buff[KSYM_SYMBOL_LEN-1] = '\0';
			if (buff[0] != '\0')
				pr_debug("    mount(): %s\n", buff);
			else
				pr_debug("    mount(): NULL\n");
		} else {
			pr_debug("    mount(): NULL\n");
		}

		/* ptr->kill_sb */
		if (ptr->kill_sb != NULL) {
			memset(buff, '\0', KSYM_SYMBOL_LEN);
			sprint_symbol(buff, (unsigned long)ptr->kill_sb);
			buff[KSYM_SYMBOL_LEN-1] = '\0';
			if (buff[0] != '\0')
				pr_debug("    kill_sb(): %s\n", buff);
			else
				pr_debug("    kill_sb(): NULL\n");
		} else {
			pr_debug("    kill_sb(): NULL\n");
		}

		/* ptr->owner */
		if (ptr->owner != NULL)
			pr_debug("    owner: %s\n", ptr->owner->name);	
		else
			pr_debug("    owner: NULL -> built-in kernel\n");

		/* ptr->fs_supers */
		if (!hlist_empty(&(ptr->fs_supers))) {
			/* no. of super blocks */
			sb_cnt = 0;
			hlist_for_each(pos, &(ptr->fs_supers))
				sb_cnt++;
			pr_debug("    fs_supers: %d super blocks in total\n", sb_cnt);

			/* super block information */
			sb_cnt = 0;
			hlist_for_each(pos, &(ptr->fs_supers)) {
				int i;
				char uuid[33];
				struct super_block *sb_ptr = hlist_entry(pos, struct super_block, s_instances);

				/* s_dev: use command "blkid" to check printed info */
				pr_debug("        fs_supers[%d]->s_dev: MAJOR %d, MINOR %d\n",
					 sb_cnt, MAJOR(sb_ptr->s_dev), MINOR(sb_ptr->s_dev));

				/* s_magic */
				pr_debug("        fs_supers[%d]->s_magic: %lu\n", sb_cnt, sb_ptr->s_magic);

				/* s_id, s_uuid */
				memset(uuid, '\0', 33);
				for (i = 0; i < 16; i++) {
					sprintf(&uuid[i*2], "%02x", sb_ptr->s_uuid[i]);
				}
				pr_debug("        fs_supers[%d]->s_id: %s\n", sb_cnt, sb_ptr->s_id);
				pr_debug("        fs_supers[%d]->s_uuid: %.*s\n", sb_cnt, 32, uuid);

				/* s_blocksize_bits, s_blocksize */
				pr_debug("        fs_supers[%d]->s_blocksize_bits: %u\n", sb_cnt, sb_ptr->s_blocksize_bits);
				pr_debug("        fs_supers[%d]->s_blocksize: %lu\n", sb_cnt, sb_ptr->s_blocksize);

				/* s_root */
				print_dentry(sb_ptr->s_root, "s_root");

				/* s_inodes */
				if (list_empty(&sb_ptr->s_inodes)) {
					pr_debug("        fs_supers[%d]->s_inodes is NULL\n", sb_cnt);
				} else {
					int inode_cnt = 0;
					int inode_idx = 0;
					struct list_head *inode_pos;
					struct inode *inode_ptr;

					/* count total inode */
					list_for_each(inode_pos, &sb_ptr->s_inodes)
						inode_cnt++;
					pr_debug("        fs_supers[%d]->s_inodes has %d in total\n", sb_cnt, inode_cnt);

					/* loop each inode */
					list_for_each(inode_pos, &sb_ptr->s_inodes) {
						inode_ptr = list_entry(inode_pos, struct inode, i_sb_list);

						/* print first inodecnt inodes only */
						inodecnt = inode_cnt < inodecnt ? inode_cnt : inodecnt;
						if (inode_idx < inodecnt) {
							char *imode_str;
							pr_debug("            s_inodes[%d]:", inode_idx);

							/* i_mode */
							if (S_ISLNK(inode_ptr->i_mode))
								imode_str = "S_IFLNK";
							if (S_ISREG(inode_ptr->i_mode))
								imode_str = "S_IFREG";
							if (S_ISDIR(inode_ptr->i_mode))
								imode_str = "S_IFDIR";
							if (S_ISCHR(inode_ptr->i_mode))
								imode_str = "S_IFCHR";
							if (S_ISBLK(inode_ptr->i_mode))
								imode_str = "S_IFBLK";
							if (S_ISFIFO(inode_ptr->i_mode))
								imode_str = "S_IFFIFO";
							if (S_ISSOCK(inode_ptr->i_mode))
								imode_str = "S_IFSOCK";
							pr_debug("                i_mode: %hu => %s\n", inode_ptr->i_mode & S_IFMT, imode_str);

							/* i_uid, i_gid */
							pr_debug("                i_uid: %d, i_gid: %d\n", inode_ptr->i_uid, inode_ptr->i_gid);

							/* i_flags */
							pr_debug("                i_flags: %u\n", inode_ptr->i_flags);
						}

						inode_idx++;
					}
				}

				/* next loop */
				sb_cnt++;
			}
		} else {
			pr_debug("    fs_supers: empty -> no super block\n");
		}

		/* the elements *_key of struct file_system_type are not checked here! */

		pr_debug("\n");

next_loop:
		/* next element in list file_systems */
		ptr = ptr->next;
		fs_cnt++;
	}

	return 0;
}

static void __exit mod_exit(void)
{
	pr_debug("=== rmmod module ===\n\n");
}

module_init(mod_init);
module_exit(mod_exit);
