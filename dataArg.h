#ifndef _arg__H
#define _arg__H
#include "dataMemory.h"

typedef struct Class_arg Arg;
struct Class_arg
{
    uint16_t contentSize;
    uint8_t *content;
};

uint16_t content_typeOffset(uint8_t *content);
uint16_t content_contentOffset(uint8_t *content);
char *content_getName(uint8_t *content);
char *content_getType(uint8_t *content);
uint8_t *content_getContent(uint8_t *content);

uint16_t arg_getTotleSize(Arg *self);
void arg_freeContent(Arg *self);

void arg_setName(Arg *self, char *name);
void arg_setContent(Arg *self, uint8_t *content, uint32_t size);
void arg_newContent(Arg *self, uint32_t size);
void arg_setType(Arg *self, char *type);
char *arg_getName(Arg *self);
char *arg_getType(Arg *self);
uint8_t *arg_getContent(Arg *self);

void arg_setInt(Arg *self, int64_t val);
void arg_setFloat(Arg *self, float val);
void arg_setPtr(Arg *self, void *pointer);
void arg_setStr(Arg *self, char *string);

int64_t arg_getInt(Arg *self);
float arg_getFloat(Arg *self);
void *arg_getPtr(Arg *self);
char *arg_getStr(Arg *self);
Arg *arg_copy(Arg *argToBeCopy);

void arg_init(Arg *self, void *voidPointer);
void arg_deinit(Arg *self);

Arg *New_arg(void *voidPointer);
#endif
