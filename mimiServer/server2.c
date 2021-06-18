#include "server2.h"
#include "dataArgs.h"
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
    if (0 == self->attributeList->getInt64ByName(self->attributeList, "isEnable"))
    {
        return;
    }
}

static void enable(server2_t *self)
{
    self->attributeList->setInt64WithName(self->attributeList, "isEnable", 1);
}

static void disable(server2_t *self)
{
    self->attributeList->setInt64WithName(self->attributeList, "isEnable", 0);
}

static void setInt64(server2_t *self, char *name, long long val)
{
    if (!self->attributeList->isArgExist(self->attributeList, name))
    {
        self->attributeList->setInt64WithName(self->attributeList,
                                               name,
                                               val);
        return;
    }
    int index = self->attributeList->getIndexByName(self->attributeList, name);
    arg_t *arg = self->attributeList->getArgByIndex(self->attributeList, index);
}

static void setPointer(server2_t *self, char *name, void *pointer)
{
    self->attributeList->setPointerWithName(self->attributeList,
                                             name,
                                             pointer);
}

static void setFloat(server2_t *self, char *name, float value)
{
    self->attributeList->setFloatWithName(self->attributeList,
                                           name,
                                           value);
}

static void setStr(server2_t *self, char *name, char *str)
{
    self->attributeList->setStrWithName(self->attributeList,
                                         name,
                                         str);
}

static long long getInt64(server2_t *self, char *name)
{
    return self->attributeList->getInt64ByName(self->attributeList,
                                               name);
}

static void *getPointer(server2_t *self, char *name)
{
    return self->attributeList->getPointerByName(self->attributeList,
                                                 name);
}

static float getFloat(server2_t *self, char *name)
{
    return self->attributeList->getFloatByName(self->attributeList,
                                               name);
}

void getStr(server2_t *self, char *name, char **strOut)
{
    self->attributeList->getStrByName(self->attributeList,
                                      name,
                                      strOut);
}

static void loadAttributeFromArgs(server2_t *self, args_t *args, char *name)
{
    args->copyArg(args, name, self->attributeList);
}

static void init(server2_t *self, args_t *args)
{
    /* List */
    self->subServerList = New_link(NULL);
    self->attributeList = New_args(NULL);

    /* operation */
    self->dinit = deinit;
    self->update = update;
    self->enable = enable;
    self->disable = disable;

    self->setInt64 = setInt64;
    self->setPointer = setPointer;
    self->setFloat = setFloat;
    self->setStr = setStr;

    self->getInt64 = getInt64;
    self->getPointer = getPointer;
    self->getFloat = getFloat;
    self->getStr = getStr;

    self->loadAttributeFromArgs = loadAttributeFromArgs;

    /* attrivute */
    self->setInt64(self, "isEnable", 1);
    self->setPointer(self, "context", self);

    /* override */

    /* args */
    if (NULL == args)
    {
        return;
    }
    self->loadAttributeFromArgs(self, args, "context");
    self->loadAttributeFromArgs(self, args, "isEnable");
}

server2_t *New_server2(args_t *args)
{
    DMEM *mem = DynMemGet(sizeof(server2_t));
    server2_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
