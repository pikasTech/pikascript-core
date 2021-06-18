#ifndef __DATA_LINK_H
#define __DATA_LINK_H

#include "dataArgs.h"
#include "dataMemory.h"
typedef struct dataTest
{
    DMEM *mem;
    int a;
    int b;
} data_t;

typedef struct linkWithNode
{
    void *data;
    DMEM *mem;
    /* attribute */
    struct linkWithNode *prior;
    struct linkWithNode *next;

    /* operation */
    void (*add)(struct linkWithNode *, void *);
    void (*destroy)(struct linkWithNode *);
    int (*size)(struct linkWithNode *);
    void *(*traverse)(struct linkWithNode *, void *(*)(void *));
    void (*deinit)(struct linkWithNode *);
    void (*port_deinit_data)(void *);
    void (*deinit_node)(struct linkWithNode *);
    void (*init)(struct linkWithNode *self, args_t *args);

} linkWithNode_t;

linkWithNode_t *New_linkWithNode(args_t *args);

#endif