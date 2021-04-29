#include "dataQueue.h"
#include "dataLink.h"
#include "VM_memory.h"

void push(struct dataQueue *queue, void *data)
{
}

void *pop(struct dataQueue *queue)
{
    int a = 1;
    void *data = &a;
    return data;
}

static void deinit(struct dataQueue *queue)
{
    queue->head->deinit(queue->head);
    DynMemPut(queue->mem);
}
void port_deinit_data_default(void * data)
{

}

queue_t *dataQueue_init()
{
    DMEM *mem = DynMemGet(sizeof(queue_t));
    queue_t *queue = (queue_t *)mem->addr;

    queue->deinit = deinit;
    queue->mem = mem;
    queue->pop = pop;
    queue->push = push;
    queue->head = dataLink_init();
    queue->head->port_deinit_data = port_deinit_data_default;
    return queue;
}