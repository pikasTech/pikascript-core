#ifndef _link2__H
#define _link2__H
#include "dataLinkNode.h"
#include "dataMemory.h"

typedef struct Class_link Link;
struct Class_link
{
    /* attribute */
    DMEM *mem;
    LinkNode *firstNode;
    int64_t TopId;
    uint32_t *memSize;
    /* virtual operation */

    /* object */
};

typedef void (*contantDeinitFun)(void *contant);

void link_deinit(Link *self);
void link_init(Link *self, void *args);
void link_addNode(Link *self, void *contant, contantDeinitFun _contantDinit);
void link_removeNode(Link *self, void *contant);
LinkNode *link_getNode(Link *self, int64_t id);
int32_t link_getSize(Link *self);

Link *New_link(void *args);
#endif
