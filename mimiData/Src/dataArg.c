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

static void setInt64(arg_t *self, long long val)
{
    unsigned long int int64Temp = val;
    unsigned char contantBuff[ARG_CONTANT_LENGTH];
    for (int i = 0; i < 8; i++)
    {
        contantBuff[i] = int64Temp;
        int64Temp = int64Temp >> 8;
    }
    self->setContant(self, contantBuff);
}

static void setFloat(arg_t *self, float val)
{
    unsigned char contantBuff[ARG_CONTANT_LENGTH];
    sprintf(contantBuff, "%f", val);
    self->setContant(self, contantBuff);
}

static void setPointer(arg_t *self, void *pointer)
{
    unsigned long int pointerTemp = (unsigned long int)pointer;
    unsigned char contantBuff[ARG_CONTANT_LENGTH];
    for (int i = 0; i < 8; i++)
    {
        contantBuff[i] = pointerTemp;
        pointerTemp = pointerTemp >> 8;
    }
}
static void setString(arg_t *self, void *string)
{
    self->setContant(self, string);
}

static long long getInt64(arg_t *self)
{
    unsigned long long int64Temp = 0;
    for (int i = 7; i > -1; i--)
    {
        int64Temp = int64Temp << 8;
        int64Temp += ((unsigned char *)(self->contant))[i];
    }
    return int64Temp;
}

static void* getPointer(arg_t *self)
{
    void *pointer = NULL;
    unsigned long int pointerTemp = 0;
    for (int i = 7; i > -1; i--)
    {
        pointerTemp = pointerTemp << 8;
        pointerTemp += ((unsigned char *)(self->contant))[i];
    }
    pointer = (void *)pointerTemp;
    return pointer;
}

static float getFloat(arg_t *self)
{
    float val = 0;
    val = atof(self->contant);
    return val;
}
static char *getString(arg_t *self)
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
    self->getContant = getContant;
    self->setType = setType;

    self->setInt64 = setInt64;
    self->setFloat = setFloat;
    self->setPointer = setPointer;
    self->setString = setString;

    self->getInt64 = getInt64;
    self->getFloat = getFloat;
    self->getPointer = getPointer;
    self->getString = getString;

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