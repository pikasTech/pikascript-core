#include "dataArg.h"
#include "dataList.h"
#include "dataMemory.h"
#include "dataString.h"

static void deinit(arg_t *self)
{
    DynMemPut(self->mem);
}

static void setContant(arg_t *self, char *contant)
{
    memcpy(self->contant, contant, ARG_CONTANT_LENGTH);
}

static void setName(arg_t *self, char *name)
{
    memcpy(self->name, name, ARG_NAME_LENGTH);
}

static void setType(arg_t *self, char *type)
{
    memcpy(self->type, type, strGetSize(type));
}

static char *getContant(arg_t *self)
{
    return self->contant;
}

static void init(arg_t *self, void *voidPointer)
{
    /* attrivute */
    self->context = self;
    for (int i = 0; i < ARG_CONTANT_LENGTH; i++)
    {
        self->contant[i] = 0;
    }
    for (int i = 0; i < ARG_NAME_LENGTH; i++)
    {
        self->name[i] = 0;
    }
    for (int i = 0; i < ARG_TYPE_LENGTH; i++)
    {
        self->type[i] = 0;
    }

    /* operation */
    self->dinit = deinit;
    self->setName = setName;
    self->setContant = setContant;
    self->get = getContant;
    self->setType = setType;

    /* object */

    /* override */
}

arg_t *New_arg(void *voidPointer)
{
    DMEM *mem = DynMemGet(sizeof(arg_t));
    arg_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, voidPointer);
    return self;
}