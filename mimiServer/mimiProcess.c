#include "mimiProcess.h"
#include "dataArgs.h"
#include "dataMemory.h"

static void deinit(process_t *self)
{
    DynMemPut(self->mem);
    self->subServerList->dinit(self->subServerList);
    self->attributeList->dinit(self->attributeList);
}

static void update(process_t *self, int systime)
{
    // return if is not enable
    if (0 == self->getInt64(self, "isEnable"))
    {
        return;
    }
}

static void enable(process_t *self)
{
    self->setInt64(self, "isEnable", 1);
}

static void disable(process_t *self)
{
    self->setInt64(self, "isEnable", 0);
}

static void setInt64(process_t *self, char *name, long long val)
{
    self->attributeList->setInt64WithName(self->attributeList, name, val);
}

static void setPointer(process_t *self, char *name, void *pointer)
{
    self->attributeList->setPointerWithName(self->attributeList, name, pointer);
}

static void setFloat(process_t *self, char *name, float value)
{
    self->attributeList->setFloatWithName(self->attributeList, name, value);
}

static void setStr(process_t *self, char *name, char *str)
{
    self->attributeList->setStrWithName(self->attributeList, name, str);
}

static long long getInt64(process_t *self, char *name)
{
    return self->attributeList->getInt64ByName(self->attributeList, name);
}

static void *getPointer(process_t *self, char *name)
{
    return self->attributeList->getPointerByName(self->attributeList, name);
}

static float getFloat(process_t *self, char *name)
{
    return self->attributeList->getFloatByName(self->attributeList, name);
}

void getStr(process_t *self, char *name, char **strOut)
{
    self->attributeList->getStrByName(self->attributeList, name, strOut);
}

static void loadAttributeFromArgs(process_t *self, args_t *args, char *name)
{
    args->copyArg(args, name, self->attributeList);
}

static void init(process_t *self, args_t *args)
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

process_t *New_server2(args_t *args)
{
    DMEM *mem = DynMemGet(sizeof(process_t));
    process_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
