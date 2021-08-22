#include "dataMemory.h"
#include <stdint.h>
#include <stdlib.h>
/* 
    本文件代码修改自作者： piaolingtear
    来源：http://blog.csdn.net/u011833609/article/details/46834203
*/

DMEM_STATE DMEMS = {0};

void *pikaMalloc(uint32_t size)
{
    DMEMS.heapUsed += size;
    if (DMEMS.heapUsedMax<DMEMS.heapUsed)
    {
        DMEMS.heapUsedMax = DMEMS.heapUsed;
    }
    return malloc(size);
}

void pikaFree(void *mem, uint32_t size)
{
    free(mem);
    mem = NULL;
    DMEMS.heapUsed -= size;
}
