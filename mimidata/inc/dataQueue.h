#ifndef __DATA_QUEUE_H
#define __DATA_QUEUE_H
#include "VM_memory.h"
#include "dataLink.h"
typedef struct dataQueue
{
    DMEM *mem;
    dataLink_t *head;

    void (*push)(struct dataQueue *, void *);
    void *(*pop)(struct dataQueue *);
    void (*deinit)(struct dataQueue *);
} queue_t;

queue_t *dataQueue_init();

#endif