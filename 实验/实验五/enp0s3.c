// 禁止所有数据包

/*
    struct xdp_md {
        __u32 data; // 指向数据包的起点
        __u32 data_end; // 指向数据包的末尾
        __u32 data_meta; //数据包元数据
        __u32 ingress_ifindex; //rxq->dev->ifindex 网卡的序号，ip link显示的那个
        __u32 rx_queue_index;  //rxq->queue_index 网卡接收队列的序号

        __u32 egress_ifindex;  // txq->dev->ifindex 这个参数旧一些的内核是没有的
    };
*/

#include <linux/bpf.h>
#include <linux/if_ether.h>
#include <linux/ip.h>
#include <linux/in.h>

// 将后面的函数或变量放到特定的段（section）中
// __attribute__((section(NAME), used))是GCC编译器的一个属性
// 表示将函数或变量放到名为NAME的段中，并且标记为已使用，防止编译器优化时将其删除
#define __section(NAME) \
    __attribute__((section(NAME), used))

// 这部分代码定义了一个BPF程序，放在名为"prog"的段中
// 函数drop_all接收一个参数struct xdp_md *ctx
// 这是XDP（eXpress Data Path）程序的上下文，包含了数据包的信息
__section("prog") int drop_all(struct xdp_md *ctx)
{
    // 函数的返回值XDP_DROP表示将数据包丢弃
    return XDP_DROP;
}

// 许可证是GPL
char __license[] __section("license") = "GPL";