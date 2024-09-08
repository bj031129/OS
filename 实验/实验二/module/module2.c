#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>

#define MAX_ARRAY 6 // 定义整数数组的最大长度为6

static int int_var = 0;			  // 定义一个整数变量并初始化为0
static char *str_var = "default"; // 定义一个字符串指针变量并初始化为"default"
static int int_array[MAX_ARRAY];  // 定义一个整数数组

int narr; // 定义一个变量用于存储整数数组的大小

// 使用module_param宏将int_var注册为模块参数，类型为int，访问权限为0644，同时添加描述信息
module_param(int_var, int, 0644);
MODULE_PARM_DESC(int_var, "An integer variable");

// 使用module_param宏将str_var注册为模块参数，类型为charp（字符指针），访问权限为0644，同时添加描述信息
module_param(str_var, charp, 0644);
MODULE_PARM_DESC(str_var, "A string variable");

// 使用module_param_array宏将int_array注册为模块参数，类型为int，访问权限为0644，
// 传入指向narr的指针作为参数大小，并添加描述信息
module_param_array(int_array, int, &narr, 0644);
MODULE_PARM_DESC(int_array, "An integer array");

// 模块加载时调用的初始化函数
static int __init hello_init(void)
{
	int i;
	printk("Module2 is loaded!\n");		 // 打印模块加载信息
	printk("int_var is %d.\n", int_var); // 打印整数参数的值
	printk("str_var is %s.\n", str_var); // 打印字符串参数的值
	for (i = 0; i < narr; i++)
	{ // 遍历整数数组并打印每个元素的值
		printk("int_array[%d] = %d\n", i, int_array[i]);
	}
	return 0; // 返回0表示模块加载成功
}

// 模块卸载时调用的退出函数
static void __exit hello_exit(void)
{
	printk("Module2 is removed!\n"); // 打印模块卸载信息
}

// 注册模块的初始化和退出函数
module_init(hello_init);
module_exit(hello_exit);

// 指定模块的许可证信息为GPL
MODULE_LICENSE("GPL");
