#include "dataLinkNode.h"
#include "dataMemory.h"

void linkNode_deinit(LinkNode *self)
{
    self->_contentDinit(self->content);
    // DynMemPut(self->mem);
    pikaFree(self, self->memSize);
    self = NULL;
}

void linkNode_init(LinkNode *self, void *args)
{
    /* attribute */
    self->nextNode = NULL;
    self->priorNode = NULL;

    /* object */
    self->content = NULL;

    /* override */
    self->_contentDinit = NULL;
}

LinkNode *New_linkNode(void *args)
{
    LinkNode *self = pikaMalloc(sizeof(LinkNode));
    self->memSize = sizeof(LinkNode);
    linkNode_init(self, args);
    return self;
}
