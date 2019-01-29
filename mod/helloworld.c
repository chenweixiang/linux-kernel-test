#include <linux/module.h>
#include <linux/init.h>
#include <linux/mm_types.h>
#include <linux/gfp.h>
#include <linux/fs.h>
#include <linux/kallsyms.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/dcache.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Chen Weixiang");
MODULE_LICENSE("GPL");
MODULE_VERSION("v1.0.0");
MODULE_DESCRIPTION("Show the prototype of linux kernel module (LKM)");

static bool showparam;
module_param(showparam, bool, 0644);
MODULE_PARM_DESC(showparam, "show parameters or not. Default: false");

#if 0
static char name[20];
module_param_string(name, "Alex", 20, 0444);
MODULE_PARM_DESC(name, "the name of somebody");
#endif

static int age = 5;
module_param(age, uint, 0644);
MODULE_PARM_DESC(age, "the age of somebody");

#if 0
static int array[20];
module_param_array(array, int, 20, 0444);
MODULE_PARM_DESC(array, "An array of something");
#endif

void show_param(void)
{
	pr_debug("age: %d\n", age);
}
EXPORT_SYMBOL(show_param);

static int __init mod_init(void)
{
    pr_debug("=== Hello World! ===\n");

    if (showparam)
		show_param();

	return 0;
}

static void __exit mod_exit(void)
{
	pr_debug("=== Bye World ===\n");
}

module_init(mod_init);
module_exit(mod_exit);
