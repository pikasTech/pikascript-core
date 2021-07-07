#include "TestClass.h"
#include "dataMemory.h"

static void deinit(TestClass *self)
{
    DynMemPut(self->mem);
}

static void init(TestClass *self, Args *args)
{
    /* attrivute */
    self->context = self;

    /* operation */
    self->dinit = deinit;

    /* object */

    /* override */
}

TestClass *New_TestClass(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(TestClass));
    TestClass *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}