#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/io.h>
#include <linux/ioport.h>

MODULE_LICENSE("GPL");

int value;
static void *io_memory;
static unsigned long io_memory_start;
static unsigned long io_memory_size = 0x1000; // 示意使用4KB

static int __init iomem_demo_init(void)
{
    printk(KERN_INFO "Start request mem region!\n");

    // 请求IO内存区域
    if (!request_mem_region(io_memory_start, io_memory_size, "mem"))
    {
        printk(KERN_ALERT "Cannot reserve IO memory region.\n");
        return -EBUSY;
    }

    // 将IO内存区域映射到内核地址空间
    io_memory = ioremap(io_memory_start, io_memory_size);
    if (!io_memory)
    {
        printk(KERN_ALERT "Failed to map IO memory.\n");
        release_mem_region(io_memory_start, io_memory_size);
        return -ENOMEM;
    }

    // 向IO内存写入数据，为0x6666
    writel(0x6666, io_memory);
    printk(KERN_INFO "it's ok for %p.\n", io_memory);

    // 从IO内存读取数据，读取之前写入的数据
    value = readl(io_memory);

    // 解除映射并释放IO内存区域
    iounmap(io_memory);
    release_mem_region(io_memory_start, io_memory_size);

    return 0;
}

static void __exit iomem_demo_exit(void)
{
    printk(KERN_INFO "Exit request_region!\n");
}

module_init(iomem_demo_init);
module_exit(iomem_demo_exit);
