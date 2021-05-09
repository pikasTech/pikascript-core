#ifndef __DATA_LINK_H
#define __DATA_LINK_H

#include "VM_memory.h"
typedef struct dataTest
{
    DMEM *mem;
    int a;
    int b;
} data_t;

typedef struct dataLink
{
    void *data;
    DMEM *mem;
    struct dataLink *prior;
    struct dataLink *next;
    void (*add)(struct dataLink *, void *);
    void (*destroy)(struct dataLink *);
    int (*size)(struct dataLink *);
    void *(*traverse)(struct dataLink *, void *(*)(void *));
    void (*deinit)(struct dataLink *);
    void (*port_deinit_data)(void *);
    void (*deinit_node)(struct dataLink *);
} dataLink_t;

dataLink_t *dataLink_init(void);

#endif