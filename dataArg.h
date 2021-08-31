#ifndef _arg__H
#define _arg__H
#include "dataMemory.h"

typedef uint8_t Arg;
struct Class_arg
{
    uint8_t *mem;
};

uint16_t content_typeOffset(uint8_t *content);
uint16_t content_contentOffset(uint8_t *content);
uint16_t content_sizeOffset(uint8_t *self);
uint16_t content_contentOffset(uint8_t *self);

char *content_getName(uint8_t *content);
char *content_getType(uint8_t *content);
uint16_t content_getSize(uint8_t *self);
uint8_t *content_getContent(uint8_t *content);

uint16_t content_totleSize(uint8_t *self);

uint8_t *content_init(char *name, char *type, uint8_t *content, uint16_t size);
uint8_t *content_deinit(uint8_t *self);
uint8_t *content_setName(uint8_t *self, char *name);
uint8_t *content_setType(uint8_t *self, char *type);
uint8_t *content_setContent(uint8_t *self, uint8_t *content, uint16_t size);

uint16_t arg_getTotleSize(Arg *self);
void arg_freeContent(Arg *self);

Arg *arg_setName(Arg *self, char *name);
Arg *arg_setContent(Arg *self, uint8_t *content, uint32_t size);
Arg *arg_newContent(Arg *self, uint32_t size);
Arg *arg_setType(Arg *self, char *type);
char *arg_getName(Arg *self);
char *arg_getType(Arg *self);
uint8_t *arg_getContent(Arg *self);
uint16_t arg_getContentSize(Arg *self);

Arg *arg_setInt(Arg *self, int64_t val);
Arg *arg_setFloat(Arg *self, float val);
Arg *arg_setPtr(Arg *self, void *pointer);
Arg *arg_setStr(Arg *self, char *string);

int64_t arg_getInt(Arg *self);
float arg_getFloat(Arg *self);
void *arg_getPtr(Arg *self);
char *arg_getStr(Arg *self);
Arg *arg_copy(Arg *argToBeCopy);

Arg *arg_init(Arg *self, void *voidPointer);
void arg_deinit(Arg *self);

Arg *New_arg(void *voidPointer);
#endif
