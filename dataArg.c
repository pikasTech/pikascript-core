#include "dataArg.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "stdlib.h"
#include <stdint.h>

void arg_deinit(Arg *self)
{
    if (NULL != self->nameDynMem)
    {
        DynMemPut(self->nameDynMem);
    }
    if (NULL != self->typeDynMem)
    {
        DynMemPut(self->typeDynMem);
    }

    if (NULL != self->content)
    {
        pikaFree(self->content, self->contentSize);
        self->content = NULL;
        self->contentSize = 0;
    }

    pikaFree(self, self->memSize);
    self = NULL;
}

void arg_newContant(Arg *self, uint32_t size)
{
    self->content = pikaMalloc(size);
    self->contentSize = size;
    for (uint32_t i = 0; i < size; i++)
    {
        self->content[i] = 0;
    }
}

void arg_setContant(Arg *self, uint8_t *contant, uint32_t size)
{
    if (NULL != self->content)
    {
        pikaFree(self->content, self->contentSize);
        self->content = NULL;
        self->contentSize = 0;
    }
    self->content = pikaMalloc(size);
    self->contentSize = size;
    memcpy(self->content, contant, size);
}

void arg_setName(Arg *self, char *name)
{
    uint32_t size = strGetSize(name);
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
    uint32_t size = strGetSize(type);
    if (NULL != self->typeDynMem)
    {
        DynMemPut(self->typeDynMem);
    }
    self->typeDynMem = DynMemGet((size + 1) * sizeof(char));
    memcpy(self->typeDynMem->addr, type, size + 1);
}

uint8_t *arg_getContant(Arg *self)
{
    return self->content;
}

void arg_setInt(Arg *self, int64_t val)
{
    int64_t int64Temp = val;
    uint8_t contantBuff[8];
    for (uint32_t i = 0; i < 8; i++)
    {
        // add 0x30 to void \0
        contantBuff[i] = int64Temp;
        int64Temp = int64Temp >> 8;
    }
    arg_setContant(self, contantBuff, 8);
}

void arg_setFloat(Arg *self, float val)
{
    uint8_t contantBuff[4];
    uint8_t *valPtr = (uint8_t *)&val;
    for (uint32_t i = 0; i < 4; i++)
    {
        // add 0x30 to void \0
        contantBuff[i] = valPtr[i];
    }
    arg_setContant(self, contantBuff, 4);
}

float arg_getFloat(Arg *self)
{
    if (NULL == arg_getContant(self))
    {
        return -999.999;
    }
    float valOut = 0;
    uint8_t *valOutPtr = (uint8_t *)(&valOut);
    uint8_t *valPtr = arg_getContant(self);
    for (uint32_t i = 0; i < 4; i++)
    {
        valOutPtr[i] = valPtr[i];
    }
    return valOut;
}

void arg_setPtr(Arg *self, void *pointer)
{
    uint64_t pointerTemp = (uint64_t)pointer;
    uint8_t contantBuff[8];
    for (uint32_t i = 0; i < 8; i++)
    {
        // aboid \0
        contantBuff[i] = pointerTemp;
        pointerTemp = pointerTemp >> 8;
    }
    arg_setContant(self, contantBuff, 8);
}

void arg_setStr(Arg *self, char *string)
{
    arg_setContant(self, (uint8_t *)string, strGetSize(string) + 1);
}

int64_t arg_getInt(Arg *self)
{
    if (NULL == arg_getContant(self))
    {
        return -999999;
    }
    int64_t int64Temp = 0;
    for (int32_t i = 7; i > -1; i--)
    {
        // add 0x30 to avoid 0
        int64Temp = (int64Temp << 8);
        int64Temp += arg_getContant(self)[i];
    }
    return int64Temp;
}

void *arg_getPtr(Arg *self)
{
    void *pointer = NULL;
    uint64_t pointerTemp = 0;
    if (NULL == arg_getContant(self))
    {
        return NULL;
    }
    uint8_t *contant = arg_getContant(self);
    for (int32_t i = 7; i > -1; i--)
    {
        // avoid \0
        uint8_t val = contant[i];
        pointerTemp = (pointerTemp << 8);
        pointerTemp += val;
    }
    pointer = (void *)pointerTemp;
    return pointer;
}
char *arg_getStr(Arg *self)
{
    return arg_getContant(self);
}
void arg_init(Arg *self, void *voidPointer)
{
    /* attribute */
    self->context = self;
    self->nameDynMem = NULL;
    self->typeDynMem = NULL;

    self->content = NULL;
    self->name = NULL;
    self->type = NULL;

    self->contentSize = 0;

    /* operation */

    /* object */

    /* override */
}

char *arg_getName(Arg *self)
{
    if (self->nameDynMem == NULL)
    {
        return NULL;
    }
    return (char *)self->nameDynMem->addr;
}

char *arg_getType(Arg *self)
{
    if (self->typeDynMem == NULL)
    {
        return NULL;
    }
    return (char *)self->typeDynMem->addr;
}

Arg *New_arg(void *voidPointer)
{
    Arg *self = pikaMalloc(sizeof(Arg));
    self->memSize = sizeof(Arg);
    arg_init(self, voidPointer);
    return self;
}

Arg *arg_copy(Arg *argToBeCopy)
{
    Arg *argCopied = New_arg(NULL);
    arg_setContant(argCopied, arg_getContant(argToBeCopy), argToBeCopy->contentSize);
    arg_setName(argCopied, arg_getName(argToBeCopy));
    arg_setType(argCopied, arg_getType(argToBeCopy));
    return argCopied;
}
