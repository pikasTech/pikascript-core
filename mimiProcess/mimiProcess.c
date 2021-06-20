#include "mimiProcess.h"
#include "dataArgs.h"
#include "dataMemory.h"

static void deinit(mimiProcess_t *self)
{
    DynMemPut(self->mem);
    self->subServerList->dinit(self->subServerList);
    self->attributeList->dinit(self->attributeList);
}

static void update(mimiProcess_t *self)
{
    // return if is not enable
    if (0 == self->getInt64(self, "isEnable"))
    {
        return;
    }
    self->_updateHandle(self);
}
static void _updateHandle(mimiProcess_t *self)
{
    // override the handle function here
}
static void enable(mimiProcess_t *self)
{
    self->setInt64(self, "isEnable", 1);
}

static void disable(mimiProcess_t *self)
{
    self->setInt64(self, "isEnable", 0);
}

static void setInt64(mimiProcess_t *self, char *name, long long val)
{
    self->attributeList->setInt(self->attributeList, name, val);
}

static void setPointer(mimiProcess_t *self, char *name, void *pointer)
{
    self->attributeList->setPoi(self->attributeList, name, pointer);
}

static void setFloat(mimiProcess_t *self, char *name, float value)
{
    self->attributeList->setFlt(self->attributeList, name, value);
}

static void setStr(mimiProcess_t *self, char *name, char *str)
{
    self->attributeList->setStr(self->attributeList, name, str);
}

static long long getInt64(mimiProcess_t *self, char *name)
{
    return self->attributeList->getInt(self->attributeList, name);
}

static void *getPointer(mimiProcess_t *self, char *name)
{
    return self->attributeList->getPoi(self->attributeList, name);
}

static float getFloat(mimiProcess_t *self, char *name)
{
    return self->attributeList->getFlt(self->attributeList, name);
}

void getStr(mimiProcess_t *self, char *name, char **strOut)
{
    self->attributeList->getStr(self->attributeList, name, strOut);
}

static void loadAttributeFromArgs(mimiProcess_t *self, args_t *args, char *name)
{
    args->copyArg(args, name, self->attributeList);
}

static void init(mimiProcess_t *self, args_t *args)
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
    self->setPoi = setPointer;
    self->setFloat = setFloat;
    self->setStr = setStr;

    self->getInt64 = getInt64;
    self->getPointer = getPointer;
    self->getFloat = getFloat;
    self->getStr = getStr;

    self->loadAttributeFromArgs = loadAttributeFromArgs;

    /* attrivute */
    self->setInt64(self, "isEnable", 1);
    self->setPoi(self, "context", self);

    /* override */
    self->_updateHandle = _updateHandle;

    /* args */
    if (NULL == args)
    {
        return;
    }
    self->loadAttributeFromArgs(self, args, "context");
    self->loadAttributeFromArgs(self, args, "isEnable");
}

mimiProcess_t *New_mimiProcess(args_t *args)
{
    DMEM *mem = DynMemGet(sizeof(mimiProcess_t));
    mimiProcess_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
