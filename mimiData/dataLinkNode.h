#ifndef _linkNode__H
#define _linkNode__H
#include "dataMemory.h"
typedef struct Class_linkNode LinkNode;
struct Class_linkNode
{
    /* attribute */
    DMEM *mem;
    LinkNode *priorNode;
    LinkNode *nextNode;
    long long id;

    /* operation */
    void (*deinit)(LinkNode *self);
    void (*init)(LinkNode *self, void *args);

    long long (*getId)(LinkNode *self);
    int (*isId)(LinkNode *self, long long id);

    /* virtual operation */
    void (*_contantDinit)(void *contant);

    /* object */
    void *contant;
};

LinkNode *New_linkNode(void *args);
#endif
