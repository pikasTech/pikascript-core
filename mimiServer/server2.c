#include "server2.h"
#include "dataList.h"
#include "dataMemory.h"

static void deinit(server2_t *self)
{
    DynMemPut(self->mem);
    self->subServerList->dinit(self->subServerList);
    self->dataList->dinit(self->dataList);
}

static void update(server2_t *self, int systime)
{
    // return if lamp is not enable
    if (0 == self->isEnable)
    {
        return;
    }
}

static void enable(server2_t *self)
{
    self->isEnable = 1;
}

static void disable(server2_t *self)
{
    self->isEnable = 0;
}

static void argHandle_context(server2_t *self,
                              list_t *args,
                              char *argName)
{
    self->context = args->getPointerByName(args, argName);
}

static void argHandle_isEnalbe(server2_t *self,
                               list_t *args,
                               char *argName)
{
    self->isEnable = args->getInt64ByName(args, argName);
}

static void argHandle(server2_t *self,
                      list_t *args,
                      char *argName,
                      void (*handle)(server2_t *self,
                                     list_t *args,
                                     char *argName))
{
    if (-1 == args->getIndexByName(args, argName))
    {
        return;
    }
    handle(self, args, argName);
}

static void init(server2_t *self, list_t *args)
{
    /* attrivute */
    self->context = self;
    self->subServerList = New_Link(NULL);
    self->dataList = New_Link(NULL);
    self->isEnable = 1;

    /* operation */
    self->dinit = deinit;
    self->update = update;
    self->enable = enable;
    self->disable = disable;
    self->argHandle = argHandle;

    /* object */

    /* override */

    /* args */
    if (NULL == args)
    {
        return;
    }
    self->argHandle(self,
                    args,
                    "isEnable",
                    argHandle_isEnalbe);
    self->argHandle(self,
                    args,
                    "context",
                    argHandle_context);
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
