#ifndef _arg__H
#define _arg__H
#include "dataMemory.h"

typedef struct Class_arg Arg;
struct Class_arg
{
    /* attribute */
    DMEM *mem;
    void *context;

    DMEM *contantDynMem;
    DMEM *nameDynMem;
    DMEM *typeDynMem;

    /* operation */
    void (*deinit)(Arg *self);
    void (*init)(Arg *self, void *voidPointer);

    void (*setName)(Arg *self, char *name);
    void (*setContant)(Arg *self, char *contant, int size);
    void (*setType)(Arg *self, char *type);

    void (*setInt)(Arg *self, long long val);
    void (*setFloat)(Arg *self, float val);
    void (*setPtr)(Arg *self, void *pointer);
    void (*setStr)(Arg *self, char *string);

    long long (*getInt)(Arg *self);
    float (*getFloat)(Arg *self);
    void *(*getPtr)(Arg *self);
    char *(*getStr)(Arg *self);

    char *(*getContant)(Arg *self);

    /* virtual operation */

    /* object */
};

Arg *New_arg(void *voidPointer);
#endif
