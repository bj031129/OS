#include <linux/module.h>
#include <asm/io.h>
#include <linux/ioport.h>

MODULE_LICENSE("GPL");

struct resource *myregion; // 用于保存请求到的资源信息的指针

static int __init mem_module_init(void)
{
	printk("Start request region!\n");

	// 请求IO端口区域
	myregion = request_region(22222, 10, "ve");

	if (myregion != NULL)
	{
		printk("it's ok for %lld .", myregion->start);
	}
	else
	{
		printk("Failed to request region!\n");
	}

	return 0;
}

static void __exit mem_module_exit(void)
{
	// 释放IO端口区域
	release_region(22222, 10);
	printk("Exit request_region!\n");
}

module_init(mem_module_init);
module_exit(mem_module_exit);
