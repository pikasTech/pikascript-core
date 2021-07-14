#include "dataArg.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "stdlib.h"

void arg_deinit(Arg *self)
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

 void arg_setContant(Arg *self, char *contant, int size)
{
    if (NULL != self->contantDynMem)
    {
        DynMemPut(self->contantDynMem);
    }
    self->contantDynMem = DynMemGet((size + 1) * sizeof(char));
    memcpy(self->contantDynMem->addr, contant, size + 1);
}

 void arg_setName(Arg *self, char *name)
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

 void arg_setType(Arg *self, char *type)
{
    int size = strGetSize(type);
    if (NULL != self->typeDynMem)
    {
        DynMemPut(self->typeDynMem);
    }
    self->typeDynMem = DynMemGet((size + 1) * sizeof(char));
    memcpy(self->typeDynMem->addr, type, size + 1);
}

 char *arg_getContant(Arg *self)
{
    // return self->contactConst;
    return self->contantDynMem->addr;
}

 void arg_setInt(Arg *self, long long val)
{
    unsigned long int int64Temp = val;
    unsigned char contantBuff[256];
    for (int i = 0; i < 8; i++)
    {
        // add 0x30 to void \0
        contantBuff[i] = int64Temp;
        int64Temp = int64Temp >> 8;
    }
    arg_setContant(self, (char *)contantBuff, 8);
}

 void arg_setFloat(Arg *self, float val)
{
    char contantBuff[256];
    sprintf((char *)contantBuff, "%f", val);
    arg_setContant(self, (char *)contantBuff, strGetSize(contantBuff));
}

 void arg_setPtr(Arg *self, void *pointer)
{
    unsigned long int pointerTemp = (unsigned long int)pointer;
    unsigned char contantBuff[256];
    for (int i = 0; i < 8; i++)
    {
        // aboid \0
        contantBuff[i] = pointerTemp;
        pointerTemp = pointerTemp >> 8;
    }
    arg_setContant(self, (char *)contantBuff, 8);
}
 void arg_setStr(Arg *self, char *string)
{
    arg_setContant(self,  string, strGetSize(string));
}

 long long arg_getInt(Arg *self)
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

 void *arg_getPtr(Arg *self)
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
 float arg_getFloat(Arg *self)
{
    float val = 0;
    val = atof(self->contantDynMem->addr);
    return val;
}
 char *arg_getStr(Arg *self)
{
    return self->contantDynMem->addr;
}

void arg_init(Arg *self, void *voidPointer)
{
    /* attrivute */
    self->context = self;
    self->contantDynMem = NULL;
    self->nameDynMem = NULL;
    self->typeDynMem = NULL;

    /* operation */

    /* object */

    /* override */
}


char *arg_getName(Arg *self)
{
    return self->nameDynMem->addr;
}

char *arg_getType(Arg *self)
{
    return self->typeDynMem->addr;
}

Arg *New_arg(void *voidPointer)
{
    DMEM *mem = DynMemGet(sizeof(Arg));
    Arg *self = mem->addr;
    self->mem = mem;
    arg_init(self, voidPointer);
    return self;
}
