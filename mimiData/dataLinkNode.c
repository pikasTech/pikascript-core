#include "dataLinkNode.h"
#include "dataMemory.h"

static void deinit(LinkNode *self)
{
    DynMemPut(self->mem);
    self->_contantDinit(self->contant);
}

static long long getId(LinkNode *self)
{
    return self->id;
}

static int isId(LinkNode *self, long long id)
{
    if (id == self->getId(self))
    {
        return 1;
    }
    return 0;
}

static void init(LinkNode *self, void *args)
{
    /* attrivute */
    self->priorNode = NULL;
    self->nextNode = NULL;
    self->id = 0;

    /* operation */
    self->deinit = deinit;
    self->getId = getId;
    self->isId = isId;

    /* object */
    self->contant = NULL;

    /* override */
    self->_contantDinit = NULL;
}

LinkNode *New_linkNode(void *args)
{
    DMEM *mem = DynMemGet(sizeof(LinkNode));
    LinkNode *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
