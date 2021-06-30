#ifndef _linkNode__H
#define _linkNode__H
#include "dataMemory.h"
typedef struct Class_linkNode linkNode_t;
struct Class_linkNode
{
    /* attribute */
    DMEM *mem;
    linkNode_t *priorNode;
    linkNode_t *nextNode;
    long long id;

    /* operation */
    void (*dinit)(linkNode_t *self);
    void (*init)(linkNode_t *self, void *args);

    long long (*getId)(linkNode_t *self);
    int (*isId)(linkNode_t *self, long long id);

    /* virtual operation */
    void (*_contantDinit)(void *contant);

    /* object */
    void *contant;
};

linkNode_t *New_linkNode(void *args);
#endif
