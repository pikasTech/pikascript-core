#include "server2.h"
#include "dataList.h"
#include "dataMemory.h"

static void deinit(server2_t *self)
{
    DynMemPut(self->mem);
    self->subServerList->dinit(self->subServerList);
    self->attributeList->dinit(self->attributeList);
}

static void update(server2_t *self, int systime)
{
    // return if lamp is not enable
    if (0 == self->attributeList->getInt64ByName(self->attributeList,"isEnable"))
    {
        return;
    }
}

static void enable(server2_t *self)
{
    self->attributeList->int64(self->attributeList,"isEnable", 1);
}

static void disable(server2_t *self)
{
    self->attributeList->int64(self->attributeList,"isEnable", 0);
}

static void init(server2_t *self, list_t *args)
{
    /* List */
    self->subServerList = New_Link(NULL);
    self->attributeList = New_list(NULL);

    /* attrivute */
    self->attributeList->int64(self->attributeList,"isEnable", 1);
    self->attributeList->pointer(self->attributeList,"isEnable", self);


    /* operation */
    self->dinit = deinit;
    self->update = update;
    self->enable = enable;
    self->disable = disable;

    /* object */

    /* override */

    /* args */
    if (NULL == args)
    {
        return;
    }
    self->attributeList->pointer(self->attributeList,"context",args->getPointerByName(args, "context"));
    self->attributeList->int64(self->attributeList,"isEnable",args->getInt64ByName(args, "isEnable"));
}

server2_t *New_server2(list_t *args)
{
    DMEM *mem = DynMemGet(sizeof(server2_t));
    server2_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
