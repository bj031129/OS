// 禁止所有的 TCP 数据包

/*
    #define ETH_ALEN 6  //定义了以太网接口的MAC地址的长度为6个字节

    #define ETH_HLAN 14  //定义了以太网帧的头长度为14个字节

    #define ETH_ZLEN 60  //定义了以太网帧的最小长度为 ETH_ZLEN + ETH_FCS_LEN = 64个字节

    #define ETH_DATA_LEN 1500  //定义了以太网帧的最大负载为1500个字节

    #define ETH_FRAME_LEN 1514  //定义了以太网正的最大长度为ETH_DATA_LEN + ETH_FCS_LEN = 1518个字节

    #define ETH_FCS_LEN 4   //定义了以太网帧的CRC值占4个字节

    struct  ethhdr
    {
        unsigned  char  h_dest[ETH_ALEN];  //目的MAC地址

        unsigned  char  h_source[ETH_ALEN];  //源MAC地址

        __u16 h_proto ;  //网络层所使用的协议类型
    }__attribute__((packed))   //用于告诉编译器不要对这个结构体中的缝隙部分进行填充操作；
*/

/*
    struct iphdr {
        #if defined(__LITTLE_ENDIAN_BITFIELD)
            __u8    ihl:4,      //首部长度(4位):首部长度指的是IP层头部包含多少个4字节
                    version:4;  //版本(4位)，目前的协议版本号是4,因此IP有时也称作IPv4
        #elif defined (__BIG_ENDIAN_BITFIELD)
            __u8    version:4,
                    ihl:4;
        #else
        #error "Please fix <asm/byteorder.h>"
        #endif
            __u8    tos;
            __be16 -tot_len; //总长度字段(16位)是指整个IP数据报的长度
            __be16 - id;
            __be16 - frag_off;
            __u8    ttl;
            __u8    protocol;
            __be16 - check;
            __be32 - saddr;
            __be32 - daddr;
    };
*/

// 数据链路层中：| MAC头部 | IP头部 | TCP头部 | 上层数据 | ->数据帧

#include <linux/bpf.h>
#include <linux/in.h>
#include <linux/if_ether.h>
#include <linux/ip.h>

#define __section(NAME) \
    __attribute__((section(NAME), used))

// struct xdp_md *ctx 是XDP程序的上下文参数，包含了数据包的信息
__section("prog") int drop_tcp(struct xdp_md *ctx)
{
    // 获取数据包的起始位置
    void *data = (void *)(long)ctx->data;
    // 获取数据包的结束位置
    void *data_end = (void *)(long)ctx->data_end;

    // 检查以太网头部是否在数据包内
    if (data + sizeof(struct ethhdr) > data_end)
    {
        // XDP_ABORTED 是 XDP 程序返回的一种特殊值，表示程序由于某种错误而终止处理数据包
        return XDP_ABORTED;
    }

    // 定义一个指向IP头部的指针
    struct iphdr *ip;
    // struct ethhdr结构体来表示以太网帧的头部
    // 将指针 ip 移动到以太网头之后的位置，这里是IP头的开始位置
    ip = data + sizeof(struct ethhdr);

    // 检查IP头部是否在数据包内
    if ((void *)(ip + 1) > data_end)
    {
        return XDP_ABORTED;
    }

    // 检查IP头部中的协议字段是否为TCP协议
    if (ip->protocol == IPPROTO_TCP)
    {
        // 如果是TCP数据包，返回 XDP_drop，表示丢弃数据包
        return XDP_DROP;
    }
    // 如果不是TCP数据包，返回 XDP_PASS，表示允许数据包通过
    return XDP_PASS;
}

char _license[] __section("license") = "GPL";