#include "server.h"
#include "dataList.h"
#include "dataMemory.h"

static void deinit(server_t *self)
{
    DynMemPut(self->mem);
    self->subOjbectLinkList->dinit(self->subOjbectLinkList);
    self->subAttributeLinkList->dinit(self->subAttributeLinkList);
}

static void update(server_t *self, int systime)
{
    // return if lamp is not enable
    if (0 == self->isEnable)
    {
        return;
    }
}

static void enable(server_t *self)
{
    self->isEnable = 1;
}

static void disable(server_t *self)
{
    self->isEnable = 0;
}

static void argHandle_context(server_t *self,
                              list_t *args,
                              char *argName)
{
    self->context = args->getPointerByName(args, argName);
}

static void argHandle_isEnalbe(server_t *self,
                               list_t *args,
                               char *argName)
{
    self->isEnable = args->getInt64ByName(args, argName);
}

static void argHandle(server_t *self,
                      list_t *args,
                      char *argName,
                      void (*handle)(server_t *self,
                                     list_t *args,
                                     char *argName))
{
    if (-1 == args->getIndexByName(args, argName))
    {
        return;
    }
    handle(self, args, argName);
}

static void init(server_t *self, list_t *args)
{
    /* attrivute */
    self->context = self;
    self->subOjbectLinkList = New_Link(NULL);
    self->subAttributeLinkList = New_Link(NULL);

    for (int i = 0; i < SERVER_SUB_ATTRIBUTE_LIST_LENGTH; i++)
    {
        self->subAttributeList[i] = NULL;
    }

    self->isEnable = 1;

    for (int i = 0; i < SERVER_SUB_OBJECT_LIST_LENGTH; i++)
    {
        self->subObjectList[i] = NULL;
    }

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

server_t *New_server(list_t *args)
{
    DMEM *mem = DynMemGet(sizeof(server_t));
    server_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}