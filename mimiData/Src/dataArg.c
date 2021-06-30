#include "dataArg.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "stdlib.h"

static void deinit(arg_t *self)
{
    DynMemPut(self->mem);
    if (NULL != self->contantDynMem)
    {
        DynMemPut(self->contantDynMem);
    }
    if (NULL != self->nameDynMem)
    {
        DynMemPut(self->nameDynMem);
    }
    if (NULL != self->typeDynMem)
    {
        DynMemPut(self->typeDynMem);
    }
}

static void setContant(arg_t *self, char *contant, int size)
{
    if (NULL != self->contantDynMem)
    {
        DynMemPut(self->contantDynMem);
    }
    self->contantDynMem = DynMemGet((size + 1) * sizeof(char));
    memcpy(self->contantDynMem->addr, contant, size + 1);
}

static void setName(arg_t *self, char *name)
{
    int size = strGetSize(name);
    if (NULL != self->nameDynMem)
    {
        DynMemPut(self->nameDynMem);
    }
    self->nameDynMem = DynMemGet((size + 1) * sizeof(char));
    // size + 1 to contain \0
    memcpy(self->nameDynMem->addr, name, size + 1);
}

static void setType(arg_t *self, char *type)
{
    int size = strGetSize(type);
    if (NULL != self->typeDynMem)
    {
        DynMemPut(self->typeDynMem);
    }
    self->typeDynMem = DynMemGet((size + 1) * sizeof(char));
    memcpy(self->typeDynMem->addr, type, size + 1);
}

static char *getContant(arg_t *self)
{
    // return self->contactConst;
    return self->contantDynMem->addr;
}

static void setInt64(arg_t *self, long long val)
{
    unsigned long int int64Temp = val;
    unsigned char contantBuff[ARG_CONTANT_LENGTH];
    for (int i = 0; i < 8; i++)
    {
        // add 0x30 to void \0
        contantBuff[i] = int64Temp;
        int64Temp = int64Temp >> 8;
    }
    self->setContant(self, (char *)contantBuff, 8);
}

static void setFloat(arg_t *self, float val)
{
    char contantBuff[ARG_CONTANT_LENGTH];
    sprintf((char *)contantBuff, "%f", val);
    self->setContant(self, (char *)contantBuff, strGetSize(contantBuff));
}

static void setPointer(arg_t *self, void *pointer)
{
    unsigned long int pointerTemp = (unsigned long int)pointer;
    unsigned char contantBuff[ARG_CONTANT_LENGTH];
    for (int i = 0; i < 8; i++)
    {
        // aboid \0
        contantBuff[i] = pointerTemp;
        pointerTemp = pointerTemp >> 8;
    }
    self->setContant(self, (char *)contantBuff, 8);
}
static void setString(arg_t *self, char *string)
{
    self->setContant(self,  string, strGetSize(string));
}

static long long getInt64(arg_t *self)
{
    unsigned long long int64Temp = 0;
    for (int i = 7; i > -1; i--)
    {
        // add 0x30 to avoid 0
        int64Temp = (int64Temp << 8);
        int64Temp += ((unsigned char *)(self->contantDynMem->addr))[i];
    }
    return int64Temp;
}

static void *getPointer(arg_t *self)
{
    void *pointer = NULL;
    unsigned long int pointerTemp = 0;
    for (int i = 7; i > -1; i--)
    {
        // avoid \0
        pointerTemp = (pointerTemp << 8);
        pointerTemp += ((unsigned char *)(self->contantDynMem->addr))[i];
    }
    pointer = (void *)pointerTemp;
    return pointer;
}
static float getFloat(arg_t *self)
{
    float val = 0;
    val = atof(self->contantDynMem->addr);
    return val;
}
static char *getString(arg_t *self)
{
    return self->contantDynMem->addr;
}

static void init(arg_t *self, void *voidPointer)
{
    /* attrivute */
    self->context = self;
    self->contantDynMem = NULL;
    self->nameDynMem = NULL;
    self->typeDynMem = NULL;

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
