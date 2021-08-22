#ifndef __MEMORY_H__
#define __MEMORY_H__

/* 
    本文件代码修改自作者： piaolingtear
    来源：http://blog.csdn.net/u011833609/article/details/46834203
*/

#include <stdint.h>
#include <stdio.h>
#include <string.h>

typedef struct
{
    uint32_t heapUsed;
    uint32_t heapUsedMax;
} DMEM_STATE;

void pikaFree(void *mem, uint32_t size);
void *pikaMalloc(uint32_t size);
#endif //__MEMORY_H__
