#ifndef _linkNode__H
#define _linkNode__H
#include "dataMemory.h"
typedef struct Class_linkNode LinkNode;
struct Class_linkNode
{
    /* attribute */
    LinkNode *nextNode;
    /* object */
    void *content;
};

void linkNode_deinit(LinkNode *self);
void linkNode_init(LinkNode *self, void *args);

LinkNode *New_linkNode(void *args);
#endif
