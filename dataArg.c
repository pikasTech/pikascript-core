#include "dataArg.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "stdlib.h"
#include <stdint.h>

void arg_deinit(Arg *self)
{
    if (NULL != self->content)
    {
        pikaFree(self->content, self->contentSize);
        self->content = NULL;
        self->contentSize = 0;
    }

    if (NULL != self->nameWithType)
    {
        pikaFree(self->nameWithType, strGetSize(arg_getName(self)) + strGetSize(arg_getType(self)) + 2);
    }

    pikaFree(self, sizeof(Arg));
    self = NULL;
}

void arg_newContent(Arg *self, uint32_t size)
{
    self->content = pikaMalloc(size);
    self->contentSize = size;
    for (uint32_t i = 0; i < size; i++)
    {
        self->content[i] = 0;
    }
}

void arg_setContent(Arg *self, uint8_t *content, uint32_t size)
{
    if (NULL != self->content)
    {
        pikaFree(self->content, self->contentSize);
        self->content = NULL;
        self->contentSize = 0;
    }

    arg_newContent(self, size);
    memcpy(self->content, content, size);
}

void arg_setName(Arg *self, char *name)
{
    char *oldName = arg_getName(self);
    char *oldType = arg_getType(self);

    uint16_t newNameSize = strGetSize(name);
    uint16_t oldNameSize = strGetSize(oldName);
    uint16_t oldTypeSize = strGetSize(oldType);

    char *nameWithType = pikaMalloc(newNameSize + oldTypeSize + 2);

    memcpy(nameWithType, name, newNameSize);
    nameWithType[newNameSize] = 0; //add '\0'
    memcpy(nameWithType + newNameSize + 1, oldType, oldTypeSize);
    nameWithType[newNameSize + 1 + oldTypeSize] = 0; //add '\0'

    if (NULL != self->nameWithType)
    {
        pikaFree(self->nameWithType, oldNameSize + oldTypeSize + 2);
    }

    self->nameWithType = nameWithType;
}

void arg_setType(Arg *self, char *type)
{
    char *oldName = arg_getName(self);
    char *oldType = arg_getType(self);

    uint16_t newTypeSize = strGetSize(type);
    uint16_t oldNameSize = strGetSize(oldName);
    uint16_t oldTypeSize = strGetSize(oldType);

    char *nameWithType = pikaMalloc(oldNameSize + newTypeSize + 2);

    memcpy(nameWithType, oldName, oldNameSize);
    nameWithType[oldNameSize] = 0; //add '\0'
    memcpy(nameWithType + oldNameSize + 1, type, newTypeSize);
    nameWithType[oldNameSize + 1 + newTypeSize] = 0; //add '\0'

    if (NULL != self->nameWithType)
    {
        pikaFree(self->nameWithType, oldNameSize + oldTypeSize + 2);
    }

    self->nameWithType = nameWithType;
}

uint8_t *arg_getContent(Arg *self)
{
    return self->content;
}

void arg_setInt(Arg *self, int64_t val)
{
    int64_t int64Temp = val;
    uint8_t contentBuff[8];
    for (uint32_t i = 0; i < 8; i++)
    {
        // add 0x30 to void \0
        contentBuff[i] = int64Temp;
        int64Temp = int64Temp >> 8;
    }
    arg_setContent(self, contentBuff, 8);
}

void arg_setFloat(Arg *self, float val)
{
    uint8_t contentBuff[4];
    uint8_t *valPtr = (uint8_t *)&val;
    for (uint32_t i = 0; i < 4; i++)
    {
        // add 0x30 to void \0
        contentBuff[i] = valPtr[i];
    }
    arg_setContent(self, contentBuff, 4);
}

float arg_getFloat(Arg *self)
{
    if (NULL == arg_getContent(self))
    {
        return -999.999;
    }
    float valOut = 0;
    uint8_t *valOutPtr = (uint8_t *)(&valOut);
    uint8_t *valPtr = arg_getContent(self);
    for (uint32_t i = 0; i < 4; i++)
    {
        valOutPtr[i] = valPtr[i];
    }
    return valOut;
}

void arg_setPtr(Arg *self, void *pointer)
{
    uint64_t pointerTemp = (uint64_t)pointer;
    uint8_t contentBuff[8];
    for (uint32_t i = 0; i < 8; i++)
    {
        // aboid \0
        contentBuff[i] = pointerTemp;
        pointerTemp = pointerTemp >> 8;
    }
    arg_setContent(self, contentBuff, 8);
}

void arg_setStr(Arg *self, char *string)
{
    arg_setContent(self, (uint8_t *)string, strGetSize(string) + 1);
}

int64_t arg_getInt(Arg *self)
{
    if (NULL == arg_getContent(self))
    {
        return -999999;
    }
    int64_t int64Temp = 0;
    for (int32_t i = 7; i > -1; i--)
    {
        // add 0x30 to avoid 0
        int64Temp = (int64Temp << 8);
        int64Temp += arg_getContent(self)[i];
    }
    return int64Temp;
}

void *arg_getPtr(Arg *self)
{
    void *pointer = NULL;
    uint64_t pointerTemp = 0;
    if (NULL == arg_getContent(self))
    {
        return NULL;
    }
    uint8_t *content = arg_getContent(self);
    for (int32_t i = 7; i > -1; i--)
    {
        // avoid \0
        uint8_t val = content[i];
        pointerTemp = (pointerTemp << 8);
        pointerTemp += val;
    }
    pointer = (void *)pointerTemp;
    return pointer;
}
char *arg_getStr(Arg *self)
{
    return (char *)arg_getContent(self);
}
void arg_init(Arg *self, void *voidPointer)
{
    /* attribute */
    self->content = NULL;
    self->nameWithType = NULL;
    self->contentSize = 0;
}

char *arg_getName(Arg *self)
{
    return (char *)self->nameWithType;
}

char *arg_getType(Arg *self)
{
    if (NULL == arg_getName(self))
    {
        return NULL;
    }
    uint16_t nameSize = strGetSize(arg_getName(self));
    return (char *)self->nameWithType + nameSize + 1;
}

Arg *New_arg(void *voidPointer)
{
    Arg *self = pikaMalloc(sizeof(Arg));
    arg_init(self, voidPointer);
    return self;
}

Arg *arg_copy(Arg *argToBeCopy)
{
    Arg *argCopied = New_arg(NULL);
    arg_setContent(argCopied, arg_getContent(argToBeCopy), argToBeCopy->contentSize);
    arg_setName(argCopied, arg_getName(argToBeCopy));
    arg_setType(argCopied, arg_getType(argToBeCopy));
    return argCopied;
}
