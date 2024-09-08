#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>

#define PROC_NAME "hello_proc" // 定义/proc目录下的文件名为hello

static struct proc_dir_entry *proc_entry;

// 读取/proc/hello文件的回调函数
static ssize_t hello_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    char message[100]; // 存储消息的缓冲区
    int len;

    len = snprintf(message, sizeof(message), "Hello from /proc/%s!\n", PROC_NAME); // 构造消息内容
    if (*pos > 0 || count < len)
        return 0;
    if (copy_to_user(buf, message, len) != 0)
        return -EFAULT;
    *pos = len;
    return len;
}

// 定义文件操作结构体，指定回调函数
static const struct file_operations hello_fops = {
    .owner = THIS_MODULE,
    .read = hello_read,
};

// 模块初始化函数
static int __init hello_init(void)
{
    // 在/proc目录下创建hello文件，权限为只读
    proc_entry = proc_create(PROC_NAME, 0444, NULL, &hello_fops);
    printk(KERN_INFO "/proc/%s created\n", PROC_NAME);
    return 0;
}

// 模块退出函数
static void __exit hello_exit(void)
{
    // 移除/proc/hello文件
    proc_remove(proc_entry);
    printk(KERN_INFO "/proc/%s removed\n", PROC_NAME);
}

// 注册模块的初始化和退出函数
module_init(hello_init);
module_exit(hello_exit);
MODULE_LICENSE("GPL");
