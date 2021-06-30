#ifndef _link2__H
#define _link2__H
#include "dataLinkNode.h"
#include "dataMemory.h"

typedef struct Class_link link_t;
struct Class_link
{
    /* attribute */
    DMEM *mem;
    linkNode_t *firstNode;
    long long TopId;

    /* operation */
    void (*dinit)(link_t *self);
    void (*init)(link_t *self, void *args);
    void (*add)(link_t *self, void *contant, void (*_contantDinit)(void *contant));
    linkNode_t *(*findNodeById)(link_t *self, long long id);

    int (*size)(link_t *self);
    /* virtual operation */

    /* object */
};

link_t *New_link(void *args);
#endif
