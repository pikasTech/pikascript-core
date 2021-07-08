#include "dataArg.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "stdlib.h"

static void deinit(Arg *self)
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

static void setContant(Arg *self, char *contant, int size)
{
    if (NULL != self->contantDynMem)
    {
        DynMemPut(self->contantDynMem);
    }
    self->contantDynMem = DynMemGet((size + 1) * sizeof(char));
    memcpy(self->contantDynMem->addr, contant, size + 1);
}

static void setName(Arg *self, char *name)
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

static void setType(Arg *self, char *type)
{
    int size = strGetSize(type);
    if (NULL != self->typeDynMem)
    {
        DynMemPut(self->typeDynMem);
    }
    self->typeDynMem = DynMemGet((size + 1) * sizeof(char));
    memcpy(self->typeDynMem->addr, type, size + 1);
}

static char *getContant(Arg *self)
{
    // return self->contactConst;
    return self->contantDynMem->addr;
}

static void setInt64(Arg *self, long long val)
{
    unsigned long int int64Temp = val;
    unsigned char contantBuff[256];
    for (int i = 0; i < 8; i++)
    {
        // add 0x30 to void \0
        contantBuff[i] = int64Temp;
        int64Temp = int64Temp >> 8;
    }
    self->setContant(self, (char *)contantBuff, 8);
}

static void setFloat(Arg *self, float val)
{
    char contantBuff[256];
    sprintf((char *)contantBuff, "%f", val);
    self->setContant(self, (char *)contantBuff, strGetSize(contantBuff));
}

static void setPointer(Arg *self, void *pointer)
{
    unsigned long int pointerTemp = (unsigned long int)pointer;
    unsigned char contantBuff[256];
    for (int i = 0; i < 8; i++)
    {
        // aboid \0
        contantBuff[i] = pointerTemp;
        pointerTemp = pointerTemp >> 8;
    }
    self->setContant(self, (char *)contantBuff, 8);
}
static void setString(Arg *self, char *string)
{
    self->setContant(self,  string, strGetSize(string));
}

static long long getInt64(Arg *self)
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

static void *getPointer(Arg *self)
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
static float getFloat(Arg *self)
{
    float val = 0;
    val = atof(self->contantDynMem->addr);
    return val;
}
static char *getString(Arg *self)
{
    return self->contantDynMem->addr;
}

static void init(Arg *self, void *voidPointer)
{
    /* attrivute */
    self->context = self;
    self->contantDynMem = NULL;
    self->nameDynMem = NULL;
    self->typeDynMem = NULL;

    /* operation */
    self->deinit = deinit;

    // low level operation
    self->setName = setName;
    self->setContant = setContant;
    self->setType = setType;
    self->getContant = getContant;

    // high level operation
    self->setInt = setInt64;
    self->setFloat = setFloat;
    self->setPtr = setPointer;
    self->setStr = setString;

    self->getInt = getInt64;
    self->getFloat = getFloat;
    self->getPtr = getPointer;
    self->getStr = getString;

    /* object */

    /* override */
}

Arg *New_arg(void *voidPointer)
{
    DMEM *mem = DynMemGet(sizeof(Arg));
    Arg *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, voidPointer);
    return self;
}
