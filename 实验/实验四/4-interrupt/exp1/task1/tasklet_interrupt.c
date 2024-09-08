/*
	由于中断会打断内核中进程的正常调度运行，所以要求中断服务程序尽可能的短小精悍；
	但是在实际系统中，当中断到来时，要完成工作往往进行大量的耗时处理。
	因此期望让中断处理程序运行得快，并想让它完成的工作量多，
	这两个目标相互制约，诞生——顶/底半部机制。
*/

/*
	使用者定义自己的struct tasklet_struct，通过调度API，把tasklet加入到就绪的列表中。
	这里有两个不同优先级的链表，tasklet_vec（普通优先级）和 tasklet_hi_vec（高优先级），分别对应不同的调度API。
	这两个链表在系统上，分别注册了自己的软中断。
	所以在软中断中，所有就绪的tasklet会被逐一从链表中取出运行（执行tasklet_struct中的callback或func）。
	另外，每个CPU都各自拥有上述两个链表，但激活tasklet时，只会加入到当前CPU的链表上，所以同一个tasklet不会有并发执行产生。
*/

// struct tasklet_struct
// {
// 	struct tasklet_struct *next; // 用于tasklet单链表
// 	unsigned long state;		 // 状态,用于保存“就绪标志”等
// 	atomic_t count;				 // 引用计数,为0时表示可调度,非0则不可调度
// 	bool use_callback;			 // 选择任务处理函数的类型
// 	union
// 	{
// 		void (*func)(unsigned long data);			// 旧格式的任务处理函数
// 		void (*callback)(struct tasklet_struct *t); // 新格式的处理函数
// 	};
// 	unsigned long data; // 旧格式函数的回传参数
// };

// 初始化结构体的变量，绑定处理函数

// void tasklet_init(struct tasklet_struct *t, void (*func)(unsigned long), unsigned long data)
// {
// 	t->next = NULL;
// 	t->state = 0;
// 	atomicset(&t->count, 0);
// 	t->func = func;
// 	t->use_callback = false;
// 	t->data = data;
// }

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/interrupt.h>

static int irq;							// Interrupt Request,中断请求
static char *devname;					// 注册中断的设备名
static struct tasklet_struct mytasklet; // 声明tasklet结构体
static unsigned int req_ret = 0;		// 用于计数中断调用次数

module_param(irq, int, 0644);		// 用户向内核传递irq参数
module_param(devname, charp, 0644); // 用户向内核传递devname参数

MODULE_LICENSE("GPL");

// 用于存储设备相关的信息
// 在注册中断处理程序时被传递给 request_irq 函数，用于标识设备并传递给中断处理程序
struct myirq
{
	int devid;
};

struct myirq mydev = {1900};

/*tasklet结构体的处理函数*/
// 每调用一次tasklet_schedule，则本函数会执行一次
// 内核会在适当的上下文中执行 mytasklet_handler 函数
// 不会立即执行，所以日志信息中没有
static void mytasklet_handler(unsigned long data)
{
	printk(KERN_INFO "=== tasklet is working...\n");
}

/*中断处理函数*/
// 绝大多数情况下，tasklet都是在中断处理函数中激活就绪的
static irqreturn_t myirq_handler(int irq, void *dev)
{
	req_ret++;
	printk(KERN_INFO "=== req_ret is %u\n", req_ret);
	// 调度API，把tasklet加入到就绪的列表中
	// 所有就绪的tasklet会被逐一从链表中取出运行（执行tasklet_struct中的callback或func）
	tasklet_schedule(&mytasklet);

	// IRQ_NONE: 表示中断处理程序没有处理这个中断。
	// 通常用于共享中断的情况下，表明当前处理程序不是这个中断的处理者。
	// IRQ_HANDLED: 表示中断处理程序成功处理了这个中断。
	return IRQ_HANDLED;
}

static int __init myirq_init(void)
{
	int ret;

	printk(KERN_INFO "=== Module starts...\n");
	printk(KERN_INFO "=== req_ret is %u\n", req_ret);

	// 初始化tasklet,指定处理函数:mytasklet_handler
	tasklet_init(&mytasklet, mytasklet_handler, 0);

	// request_irq 函数向内核注册一个中断处理程序，并指定 IRQ 号和处理程序
	// 每当指定的中断发生时，内核会调用注册的中断处理程序 myirq_handler
	// IRQF_SHARED,表示多个设备共享中断
	ret = request_irq(irq, myirq_handler, IRQF_SHARED, devname, &mydev);
	if (ret)
	{
		printk(KERN_ERR "=== %s request IRQ:%d failed with %d\n", devname, irq, ret);
		return ret;
	}
	printk(KERN_INFO "=== %s request IRQ:%d success...\n", devname, irq);

	// 模拟中断事件发生
	// printk(KERN_INFO "=== Simulating interrupt event...\n");

	return 0;
}

static void __exit myirq_exit(void)
{
	printk(KERN_INFO "=== Module exits...\n");
	// 清除一个处于就绪状态（已schedule，正在等待运行）的tasklet
	tasklet_kill(&mytasklet);
	// 注销中断处理函数时
	free_irq(irq, &mydev);
	printk(KERN_INFO "=== %s request IRQ:%d leaving success...\n", devname, irq);
}

module_init(myirq_init);
module_exit(myirq_exit);

/*
	执行流程总结

	1.加载模块：
	当内核加载该模块时，myirq_init 函数被调用。
	在 myirq_init 函数中：
	初始化了一个 tasklet_struct 结构体 mytasklet，并指定了处理函数为 mytasklet_handler。
	使用 request_irq 函数注册了一个中断处理函数 myirq_handler，并指定了中断号、设备名、以及一个结构体 mydev 作为参数。
	如果注册成功，打印注册成功的信息；如果失败，则打印注册失败的信息。

	2.中断处理：
	当系统中断事件发生时，内核会调用注册的中断处理函数 myirq_handler。
	在 myirq_handler 函数中：
	递增了一个用于计数的变量 req_ret，并打印了其值。
	调用 tasklet_schedule 函数，将 mytasklet 添加到就绪列表中，以便稍后执行。

	3.任务队列执行：
	任务队列 mytasklet 被调度后，内核会在适当的上下文中执行 mytasklet_handler 函数。
	在 mytasklet_handler 函数中，打印了一条消息，表示任务队列正在工作。

	4.卸载模块：
	当内核卸载该模块时，myirq_exit 函数被调用。
	在 myirq_exit 函数中：
	调用 tasklet_kill 函数，清除处于就绪状态的任务队列 mytasklet。
	使用 free_irq 函数注销了中断处理函数，并释放了相应的资源。
	打印模块退出的信息。

	通过以上步骤，该模块实现了中断处理和任务队列的功能，用于处理系统中断事件，并在中断上下文之外执行延迟的工作。
*/