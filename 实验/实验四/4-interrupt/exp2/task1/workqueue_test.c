#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/workqueue.h>
#include <linux/timer.h>
#include <linux/timex.h>
#include <linux/rtc.h>
#include <linux/delay.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("BX");
MODULE_DESCRIPTION("Use workqueue to print the time periodically");

int times = 0;
module_param(times, int, 0644); // 用户向内核传递times参数

static struct workqueue_struct *queue = NULL;
static struct delayed_work work1;
static struct delayed_work work2;
static int i = 0;

// 立即执行任务的处理函数
static void work_handler(struct work_struct *work)
{
    // timex 是一个包含时间相关信息的结构体
    struct timex txc;
    // rtc_time 是一个用来表示人类可读时间的结构体，包含年、月、日、时、分、秒等信息
    struct rtc_time tm;
    // 获取当前时间，并将其存储在 txc.time 中
    do_gettimeofday(&(txc.time));
    // 将 txc.time.tv_sec（即当前时间的秒数）转换为 rtc_time 结构体 tm
    rtc_time_to_tm(txc.time.tv_sec, &tm);

    printk(KERN_INFO "%d:\n", i);
    printk(KERN_INFO "Immediate Work: Current time: %d-%02d-%02d %02d:%02d:%02d\n",
           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
           tm.tm_hour, tm.tm_min, tm.tm_sec);

    i++;
    if (i < times)
    {
        queue_delayed_work(queue, &work1, 5 * HZ);
    }
}

// 延迟任务的处理函数
static void work_handler_delay(struct work_struct *work)
{
    struct timex txc;
    struct rtc_time tm;
    do_gettimeofday(&(txc.time));
    rtc_time_to_tm(txc.time.tv_sec, &tm);

    printk(KERN_INFO "Delayed Work: Current time: %d-%02d-%02d %02d:%02d:%02d\n",
           tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday,
           tm.tm_hour, tm.tm_min, tm.tm_sec);
}

static int __init init_workqueue(void)
{
    printk(KERN_INFO "Initializing Workqueue Module\n");

    // 创建工作队列
    queue = create_workqueue("my_workqueue");
    if (!queue)
    {
        printk(KERN_ERR "Failed to create workqueue\n");
        return -ENOMEM;
    }

    // 初始化工作
    INIT_DELAYED_WORK(&work1, work_handler);
    INIT_DELAYED_WORK(&work2, work_handler_delay);

    // 调度立即执行的工作
    queue_delayed_work(queue, &work1, 0);

    // 调度延迟执行的工作，延迟10秒
    queue_delayed_work(queue, &work2, 10 * HZ);

    return 0;
}

static void __exit exit_workqueue(void)
{
    printk(KERN_INFO "Exiting Workqueue Module\n");

    // 取消工作
    cancel_delayed_work_sync(&work1);
    cancel_delayed_work_sync(&work2);

    // 销毁工作队列
    if (queue)
    {
        destroy_workqueue(queue);
    }
}

module_init(init_workqueue);
module_exit(exit_workqueue);

/*
    执行流程总结

    1.加载模块：
    创建工作队列。
    初始化并调度 work1 立即执行。
    初始化并调度 work2 延迟 10 秒执行。

    2.执行 work1：
    获取并打印当前时间。
    递增计数器 i。
    如果 i 小于 times，则调度 work1 延迟 5 秒再次执行。

    3.执行 work2：
    获取并打印当前时间。

    4.卸载模块：
    取消所有延迟工作。
    销毁工作队列。

    通过这个过程，work1 将每 5 秒执行一次，总共执行 5 次，而 work2 将在模块加载后 10 秒执行一次。
*/
