#include "dataLinkNode.h"
#include "dataArgsConst.h"
#include "dataMemory.h"

static void deinit(linkNode_t *self)
{
    DynMemPut(self->mem);
    self->_contantDinit(self->contant);
}

static long long getId(linkNode_t *self)
{
    return self->id;
}

static int isId(linkNode_t *self, long long id)
{
    if (id == self->getId(self))
    {
        return 1;
    }
    return 0;
}

static void init(linkNode_t *self, argsConst_t *args)
{
    /* attrivute */
    self->priorNode = NULL;
    self->nextNode = NULL;
    self->id = 0;

    /* operation */
    self->dinit = deinit;
    self->getId = getId;
    self->isId = isId;

    /* object */
    self->contant = NULL;

    /* override */
    self->_contantDinit = NULL;
}

linkNode_t *New_linkNode(argsConst_t *args)
{
    DMEM *mem = DynMemGet(sizeof(linkNode_t));
    linkNode_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
