#include "allertInfo.h"
#include "dataMemory.h"

static void deinit(allertInfo_t *self)
{
    DynMemPut(self->mem);
}

static void init(allertInfo_t *self, Args *args)
{
    /* attrivute */
    self->context = self;
    self->allert_temp1 = 28;
    self->allert_temp2 = 28;

    /* operation */
    self->deinit = deinit;

    /* object */

    /* override */
}

allertInfo_t *New_allertInfo(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(allertInfo_t));
    allertInfo_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}