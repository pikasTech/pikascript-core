#include "dataTest.h"
#include "dataMemory.h"
#include "dataList.h"

static void deinit(dataTest_t *self)
{
    DynMemPut(self->mem);
}

static void init(dataTest_t *self, list_t *args)
{
    /* attrivute */
    self->context = self;
    self->a = 0;
    self->b = 0;

    /* operation */
    self->dinit = deinit;

    /* object */

    /* override */
}

dataTest_t *New_dataTest(list_t *args)
{
    DMEM *mem = DynMemGet(sizeof(dataTest_t));
    dataTest_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}