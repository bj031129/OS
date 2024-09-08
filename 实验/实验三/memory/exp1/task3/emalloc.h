// emalloc.h

#ifndef EMALLOC_H
#define EMALLOC_H
#define HEAP_SIZE 1024 * 1024 // 堆的大小为1MB，这里作为示例设置

#include <stdlib.h>

typedef struct BlockHeader
{
    size_t size;
    bool is_free;
    struct BlockHeader *next;
} BlockHeader;

typedef struct
{
    BlockHeader *free_blocks;
    char heap[HEAP_SIZE];
} MemoryAllocator;

extern void *emalloc(size_t size);
extern void efree(void *ptr);

#endif
