// emalloc.c

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define HEAP_SIZE 1024 * 1024 // 堆的大小为1MB，这里作为示例设置

typedef struct BlockHeader
{
    size_t size;              // 内存块的大小
    bool is_free;             // 是否空闲
    struct BlockHeader *next; // 指向下一个内存块的指针
} BlockHeader;

typedef struct
{
    BlockHeader *free_blocks; // 指向第一个空闲内存块的指针
    char heap[HEAP_SIZE];     // 堆内存数组
} MemoryAllocator;

MemoryAllocator allocator; // 内存分配器实例

void *emalloc(size_t size)
{
    BlockHeader *block = allocator.free_blocks;  // 从第一个空闲块开始搜索
    BlockHeader **prev = &allocator.free_blocks; // 用于更新链表指针的指针

    // 在空闲块链表中查找一个足够大的内存块
    while (block != NULL)
    {
        if (block->is_free && block->size >= size + sizeof(BlockHeader))
        {
            break;
        }
        prev = &block->next;
        block = block->next;
    }

    if (block == NULL)
    {
        return NULL; // 未找到合适的内存块
    }

    // 如果找到的内存块大于请求的大小，将剩余部分分割成一个新的空闲块
    if (block->size > size + sizeof(BlockHeader))
    {
        BlockHeader *new_block = (BlockHeader *)((char *)block + sizeof(BlockHeader) + size);
        new_block->size = block->size - size - sizeof(BlockHeader);
        new_block->is_free = true;
        new_block->next = block->next;

        block->size = size + sizeof(BlockHeader);
        block->next = new_block;
    }

    block->is_free = false;
    *prev = block->next; // 从空闲块链表中移除该块

    return (char *)block + sizeof(BlockHeader); // 返回分配内存的地址
}

void efree(void *ptr)
{
    if (ptr == NULL)
        return;

    BlockHeader *block = (BlockHeader *)((char *)ptr - sizeof(BlockHeader)); // 获取内存块的头部
    block->is_free = true;

    // 如果下一个内存块也是空闲的，合并它们
    BlockHeader *next_block = (BlockHeader *)((char *)block + block->size);
    if (next_block->is_free)
    {
        block->size += next_block->size;
        block->next = next_block->next;
    }
    else
    {
        block->next = allocator.free_blocks; // 将块插入到空闲块链表头部
        allocator.free_blocks = block;
    }
}
