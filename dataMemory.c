#include "dataMemory.h"
#include <stdint.h>
#include <stdlib.h>

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

uint16_t pikaMemNow(void)
{
    return DMEMS.heapUsed;
}

uint16_t pikaMemMax(void)
{
    return DMEMS.heapUsedMax;
}

void pikaMemMaxReset(void)
{
    DMEMS.heapUsedMax = 0;
}