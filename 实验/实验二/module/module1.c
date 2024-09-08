#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

// KERN_INFO是用于打印一般信息的日志级别
//  模块加载时调用的初始化函数
static int __init hello_init(void)
{
	printk(KERN_INFO "Hi, Module1 is loaded.\n");
	return 0; // 返回0表示模块加载成功
}

// 模块卸载时调用的退出函数
static void __exit hello_exit(void)
{
	printk(KERN_INFO "Hi, Module1 is removed.\n");
}

// 声明初始化和退出函数
module_init(hello_init);
module_exit(hello_exit);

// 模块许可证信息
MODULE_LICENSE("GPL");
