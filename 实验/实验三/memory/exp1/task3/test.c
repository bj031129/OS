#include <stdio.h>
#include <stdbool.h>
#include "emalloc.h"

MemoryAllocator allocator; // 声明内存分配器实例

#define HEAP_SIZE 1024 * 1024 // 堆的大小为1MB，这里作为示例设置

int main()
{
    // 初始化内存分配器
    allocator.free_blocks = (BlockHeader *)allocator.heap;
    allocator.free_blocks->size = HEAP_SIZE - sizeof(BlockHeader);
    allocator.free_blocks->is_free = true;
    allocator.free_blocks->next = NULL;

    bool allocation_ok = true;
    for (int i = 0; i < 50; i++)
    {
        for (int j = 0; j < 50; j++)
        {
            char *ptr = (char *)emalloc(50);
            if (ptr == NULL)
            {
                allocation_ok = false;
                break;
            }
            else
            {
                printf("array[%d][%d] is OK!\n", i, j);
            }
            efree(ptr);
        }
    }

    if (allocation_ok)
    {
        printf("Memory allocation is OK.\n");
    }
    else
    {
        printf("Memory allocation has issues.\n");
    }

    return 0;
}
