#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>

static int __init log_init(void)
{
	pr_warn("The flag is: TG17{i_wrote_a_kernel_module_for_this?!}");

	return 0;
}

static void __exit log_exit(void)
{
}

module_init(log_init);
module_exit(log_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PewZ");
MODULE_DESCRIPTION("Simple log module");
