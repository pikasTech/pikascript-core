#ifndef _dataArgs__H
#define _dataArgs__H
#include "dataLink.h"
#include "dataMemory.h"
#include "dataArg.h"

typedef struct Class_Arglist args_t;
struct Class_Arglist
{
    /* attribute */
    DMEM *mem;
    void *context;
    link_t *argLinkList;
    unsigned char nameBuff[32];

    /* operation */
    void (*dinit)(args_t *self);
    void (*init)(args_t *self, args_t *args);
    int (*size)(args_t *self);

    arg_t *(*getArgByIndex)(args_t *self, int index);
    arg_t *(*getArgByName)(args_t *self, char *name);

    int (*setArg)(args_t *self, arg_t *arg);

    int (*copyArg)(args_t *self, char *name, args_t *directList);
    char *(*getTypeByName)(args_t *self, char *name);
    int (*isArgExist)(args_t *self, char *name);

    int (*setStr)(args_t *self, char *name, char *strIn);
    int (*setStrWithDefaultName)(args_t *self, char *strIn);
    char *(*getStr)(args_t *self, char *name);
    char *(*getStrByIndex)(args_t *self, int index);

    int (*setFloatWithDefaultName)(args_t *self, float argFloat);
    int (*setFloat)(args_t *self, char *name, float argFloat);
    float (*getFloatByIndex)(args_t *self, int index);
    float (*getFloat)(args_t *self, char *name);

    int (*setPoi)(args_t *self, char *name, void *argPointer);
    void *(*getPtr)(args_t *self, char *name);
    void *(*getPointerByIndex)(args_t *self, int index);

    int (*setInt)(args_t *self, char *name, long long int64In);
    long long (*getInt)(args_t *self, char *name);
    long long (*getInt64ByIndex)(args_t *self, int index);

    void (*bindInt)(args_t *self, char *name, int *intPtr);
    void (*bindFloat)(args_t *self, char *name, float *floatPtr);
    void (*bindStr)(args_t *self, char *name, char **stringPtr);
    /* arg general opeartion */
    void (*bind)(args_t *self, char *type, char *name, void *pointer);
    char *(*print)(args_t *self, char *name);

    int (*set)(args_t *self, char *name, char *valueStr);
    int (*setObject)(args_t *self, char *objectName, char *className, void *objectPtr);
    int (*foreach)(args_t *self, int (*eachHandle)(arg_t *argEach, args_t *handleArgs), args_t *handleArgs);

    /* virtual operation */

    /* object */
};

args_t *New_args(args_t *args);
#endif
