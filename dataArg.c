#include "dataArg.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "stdlib.h"
#include <stdint.h>

void arg_deinit(Arg *self)
{
    arg_freeContent(self);
    pikaFree(self, sizeof(Arg));
    self = NULL;
}

uint16_t arg_getTotleSize(Arg *self)
{
    return strGetSize(arg_getName(self)) +
           strGetSize(arg_getType(self)) +
           2 +
           self->contentSize;
}

void arg_freeContent(Arg *self)
{
    if (NULL != self->content)
    {
        pikaFree(self->content, arg_getTotleSize(self));
    }
}

char *content_getName(uint8_t *content)
{
    return content;
}

void arg_newContent(Arg *self, uint32_t size)
{
    char *oldName = arg_getName(self);
    char *oldType = arg_getType(self);
    uint16_t oldNameSize = strGetSize(oldName);
    uint16_t oldTypeSize = strGetSize(oldType);

    char *content = pikaMalloc(oldTypeSize + oldTypeSize + 2 + size);

    memcpy(content_getName(content), oldName, oldNameSize);
    content[content_typeOffset(content) - 1] = 0;
    memcpy(content_getType(content), oldType, oldTypeSize);
    content[content_contentOffset(content) - 1] = 0;

    arg_freeContent(self);

    self->content = content;
    self->contentSize = size;
    for (uint32_t i = 0; i < size; i++)
    {
        arg_getContent(self)[i] = 0;
    }
}

void arg_setContent(Arg *self, uint8_t *content, uint32_t size)
{
    arg_newContent(self, size);
    memcpy(arg_getContent(self), content, size);
}

void arg_setName(Arg *self, char *name)
{
    char *oldName = arg_getName(self);
    char *oldType = arg_getType(self);
    char *oldContent = arg_getContent(self);

    uint16_t newNameSize = strGetSize(name);
    uint16_t oldNameSize = strGetSize(oldName);
    uint16_t oldTypeSize = strGetSize(oldType);

    char *content = pikaMalloc(newNameSize + oldTypeSize + 2 + self->contentSize);

    memcpy(content_getName(content), name, newNameSize);
    content[content_typeOffset(content) - 1] = 0;
    memcpy(content_getType(content), oldType, oldTypeSize);
    content[content_contentOffset(content) - 1] = 0;
    memcpy(content_getContent(content), oldContent, self->contentSize);

    arg_freeContent(self);
    self->content = content;
}

void arg_setType(Arg *self, char *type)
{
    char *oldName = arg_getName(self);
    char *oldType = arg_getType(self);
    char *oldContent = arg_getContent(self);

    uint16_t newTypeSize = strGetSize(type);
    uint16_t oldNameSize = strGetSize(oldName);
    uint16_t oldTypeSize = strGetSize(oldType);

    char *content = pikaMalloc(oldNameSize + newTypeSize + 2 + self->contentSize);

    memcpy(content_getName(content), oldName, oldNameSize);
    content[content_typeOffset(content) - 1] = 0;
    memcpy(content_getType(content), type, newTypeSize);
    content[content_contentOffset(content) - 1] = 0;
    memcpy(content_getContent(content), oldContent, self->contentSize);

    arg_freeContent(self);
    self->content = content;
}

char *content_getType(uint8_t *content)
{
    return content + content_typeOffset(content);
}

uint16_t content_contentOffset(uint8_t *content)
{
    uint16_t typeOffset = content_typeOffset(content);
    char *type = content_getType(content);
    uint16_t typeSize = strGetSize(type);
    return typeOffset + typeSize + 1;
}

uint8_t *content_getContent(uint8_t *content)
{
    return content + content_contentOffset(content);
}

uint8_t *arg_getContent(Arg *self)
{
    if (NULL == arg_getName(self))
    {
        return NULL;
    }
    return self->content + content_contentOffset(self->content);
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
    self->contentSize = 0;
}

uint16_t content_typeOffset(uint8_t *content)
{
    uint16_t nameSize = strGetSize(content);
    return nameSize + 1;
}

char *arg_getName(Arg *self)
{
    return (char *)self->content;
}

char *arg_getType(Arg *self)
{
    if (NULL == arg_getName(self))
    {
        return NULL;
    }
    return (char *)self->content + content_typeOffset(self->content);
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
