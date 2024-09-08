#include <linux/module.h>
#include <linux/proc_fs.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define PROC_DIR_NAME "hello_dir" // 定义/proc目录下的文件夹名为myprocdir
#define PROC_FILE_NAME "hello"    // 定义文件夹中的文件名为myprocfile

static struct proc_dir_entry *proc_dir;  // 指向创建的文件夹的指针
static struct proc_dir_entry *proc_file; // 指向创建的文件的指针
static char *proc_buffer;                // 文件的缓冲区
static size_t proc_buffer_size = 256;    // 缓冲区大小为256字节

// 读取文件的回调函数
static ssize_t proc_file_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
    if (*pos >= proc_buffer_size) // 如果读取位置超过了缓冲区大小，返回0表示读取结束
        return 0;

    if (count > proc_buffer_size - *pos) // 如果要读取的字节数超过了剩余的字节数，调整count为剩余的字节数
        count = proc_buffer_size - *pos;

    if (copy_to_user(buf, proc_buffer + *pos, count)) // 将缓冲区的数据复制到用户空间
        return -EFAULT;

    *pos += count; // 更新读取位置
    return count;  // 返回实际读取的字节数
}

// 写入文件的回调函数
static ssize_t proc_file_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{
    if (*pos + count > proc_buffer_size) // 如果写入位置加上要写入的字节数超过了缓冲区大小，返回错误
        return -ENOMEM;

    if (copy_from_user(proc_buffer + *pos, buf, count)) // 将用户空间的数据复制到缓冲区
        return -EFAULT;

    *pos += count; // 更新写入位置
    return count;  // 返回实际写入的字节数
}

// 定义文件操作结构体，指定读写回调函数
static const struct file_operations proc_file_fops = {
    .owner = THIS_MODULE,
    .read = proc_file_read,
    .write = proc_file_write,
};

// 模块初始化函数
static int __init proc_init(void)
{
    // 分配缓冲区内存
    proc_buffer = kmalloc(proc_buffer_size, GFP_KERNEL);
    if (!proc_buffer)
        return -ENOMEM;

    // 在/proc目录下创建myprocdir文件夹
    proc_dir = proc_mkdir(PROC_DIR_NAME, NULL);
    if (!proc_dir)
    {
        kfree(proc_buffer);
        return -ENOMEM;
    }

    // 在myprocdir文件夹下创建myprocfile文件
    proc_file = proc_create_data(PROC_FILE_NAME, 0666, proc_dir, &proc_file_fops, NULL);
    if (!proc_file)
    {
        remove_proc_entry(PROC_DIR_NAME, NULL);
        kfree(proc_buffer);
        return -ENOMEM;
    }

    printk(KERN_INFO "/proc/%s/%s created\n", PROC_DIR_NAME, PROC_FILE_NAME);
    return 0;
}

// 模块退出函数
static void __exit proc_exit(void)
{
    // 移除myprocfile文件
    if (proc_file)
        remove_proc_entry(PROC_FILE_NAME, proc_dir);

    // 移除myprocdir文件夹
    if (proc_dir)
        remove_proc_entry(PROC_DIR_NAME, NULL);

    // 释放缓冲区内存
    if (proc_buffer)
        kfree(proc_buffer);

    printk(KERN_INFO "/proc/%s/%s removed\n", PROC_DIR_NAME, PROC_FILE_NAME);
}

// 注册模块的初始化和退出函数
module_init(proc_init);
module_exit(proc_exit);
MODULE_LICENSE("GPL");
