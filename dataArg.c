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
    return content_totleSize(self->mem);
}

uint16_t content_sizeOffset(uint8_t *self)
{
    char *type = content_getType(self);
    uint16_t typeSize = strGetSize(type);
    uint16_t typeOffset = content_typeOffset(self);
    return typeOffset + typeSize + 1;
}

uint16_t content_getSize(uint8_t *self)
{
    uint16_t size = 0;
    // add 0x30 to avoid 0
    size += self[content_sizeOffset(self) + 1];
    size = (size << 8);
    size += self[content_sizeOffset(self)];
    return size;
}

uint8_t *content_init(char *name, char *type, uint8_t *content, uint16_t size)
{
    uint16_t nameSize = strGetSize(name);
    uint16_t typeSize = strGetSize(type);
    uint8_t *self = (uint8_t *)pikaMalloc(nameSize + 1 +
                                          typeSize + 1 +
                                          2 +
                                          size);
    uint8_t *nameDir = self;
    uint8_t *typeDir = nameDir + nameSize + 1;
    uint8_t *sizeDir = typeDir + typeSize + 1;
    uint8_t *contentDir = sizeDir + 2;

    memcpy(nameDir, name, nameSize + 1);
    memcpy(typeDir, type, typeSize + 1);
    sizeDir[0] = size;
    sizeDir[1] = size >> 8;
    if (NULL != content)
    {
        memcpy(contentDir, content, size);
    }
    return self;
}

uint16_t content_totleSize(uint8_t *self)
{
    return content_contentOffset(self) + content_getSize(self);
}

void arg_freeContent(Arg *self)
{
    if (NULL != self->mem)
    {
        content_deinit(self->mem);
        self->mem = NULL;
    }
}

char *content_getName(uint8_t *self)
{
    return self;
}

uint8_t *content_deinit(uint8_t *self)
{
    pikaFree(self, content_totleSize(self));
}

uint8_t *content_setContent(uint8_t *self, uint8_t *content, uint16_t size)
{
    char *name = content_getName(self);
    char *type = content_getType(self);
    char *newContent = content_init(name, type, content, size);
    content_deinit(self);
    return newContent;
}

uint8_t *content_setName(uint8_t *self, char *name)
{
    char *type = content_getType(self);
    uint8_t *content = content_getContent(self);
    uint16_t size = content_getSize(self);
    char *newContent = content_init(name, type, content, size);
    content_deinit(self);
    return newContent;
}

uint8_t *content_setType(uint8_t *self, char *type)
{
    char *name = content_getName(self);
    uint8_t *content = content_getContent(self);
    uint16_t size = content_getSize(self);
    char *newContent = content_init(name, type, content, size);
    content_deinit(self);
    return newContent;
}

void arg_newContent(Arg *self, uint32_t size)
{
    self->mem = content_init("", "", NULL, size);
}

void arg_setContent(Arg *self, uint8_t *content, uint32_t size)
{
    arg_newContent(self, size);
    memcpy(arg_getContent(self), content, size);
}

void arg_setName(Arg *self, char *name)
{
    self->mem = content_setName(self->mem, name);
}

void arg_setType(Arg *self, char *type)
{
    self->mem = content_setType(self->mem, type);
}

char *content_getType(uint8_t *self)
{
    return self + content_typeOffset(self);
}

uint16_t content_contentOffset(uint8_t *self)
{
    return content_sizeOffset(self) + 2;
}

uint8_t *content_getContent(uint8_t *content)
{
    return content + content_contentOffset(content);
}

uint8_t *arg_getContent(Arg *self)
{
    return content_getContent(self->mem);
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
    self->mem = NULL;
}

uint16_t content_typeOffset(uint8_t *content)
{
    char *name = content_getName(content);
    uint16_t nameSize = strGetSize(name);
    return nameSize + 1;
}

char *arg_getName(Arg *self)
{
    return content_getName(self);
}

char *arg_getType(Arg *self)
{
    return content_getType(self);
}

uint16_t arg_getContentSize(Arg *self)
{
    return content_getSize(self->mem);
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
    arg_setContent(argCopied, arg_getContent(argToBeCopy), arg_getContentSize(argToBeCopy));
    arg_setName(argCopied, arg_getName(argToBeCopy));
    arg_setType(argCopied, arg_getType(argToBeCopy));
    return argCopied;
}
