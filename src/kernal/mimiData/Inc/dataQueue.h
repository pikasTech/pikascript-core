#ifndef __DATA_QUEUE_H
#define __DATA_QUEUE_H
#include "dataMemory.h"
#include "dataLinkWithNode.h"
typedef struct dataQueue
{
    DMEM *mem;
    linkWithNode_t *head;

    void (*push)(struct dataQueue *, void *);
    void *(*pop)(struct dataQueue *);
    void (*deinit)(struct dataQueue *);
} queue_t;

queue_t *dataQueue_init();

#endif