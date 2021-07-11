#ifndef _dataArgs__H
#define _dataArgs__H
#include "dataLink.h"
#include "dataMemory.h"
#include "dataArg.h"

typedef struct Class_Arglist Args;
struct Class_Arglist
{
    /* attribute */
    DMEM *mem;
    void *context;
    Link *argLinkList;
    unsigned char nameBuff[32];

    /* operation */
    void (*deinit)(Args *self);
    void (*init)(Args *self, Args *args);
    int (*getSize)(Args *self);

    Arg *(*getArgByIndex)(Args *self, int index);
    Arg *(*getArg)(Args *self, char *name);
    void (*removeArg)(Args *self, char *name);

    int (*setArg)(Args *self, Arg *arg);

    int (*copyArg)(Args *self, char *name, Args *directList);
    char *(*getType)(Args *self, char *name);
    int (*isArgExist)(Args *self, char *name);

    int (*setStr)(Args *self, char *name, char *strIn);
    int (*setStrWithDefaultName)(Args *self, char *strIn);
    char *(*getStr)(Args *self, char *name);
    char *(*getStrByIndex)(Args *self, int index);

    int (*setFloatWithDefaultName)(Args *self, float argFloat);
    int (*setFloat)(Args *self, char *name, float argFloat);
    float (*getFloatByIndex)(Args *self, int index);
    float (*getFloat)(Args *self, char *name);

    int (*setPtr)(Args *self, char *name, void *argPointer);
    void *(*getPtr)(Args *self, char *name);
    void *(*getPointerByIndex)(Args *self, int index);

    int (*setInt)(Args *self, char *name, long long int64In);
    long long (*getInt)(Args *self, char *name);
    long long (*getIntByIndex)(Args *self, int index);

    void (*bindInt)(Args *self, char *name, int *intPtr);
    void (*bindFloat)(Args *self, char *name, float *floatPtr);
    void (*bindStr)(Args *self, char *name, char **stringPtr);
    /* arg general opeartion */
    void (*bind)(Args *self, char *type, char *name, void *pointer);
    char *(*print)(Args *self, char *name);

    int (*set)(Args *self, char *name, char *valueStr);
    int (*setPtrWithType)(Args *self, char *objectName, char *className, void *objectPtr);
    int (*foreach)(Args *self, int (*eachHandle)(Arg *argEach, Args *handleArgs), Args *handleArgs);

    /* virtual operation */

    /* object */
};

Args *New_args(Args *args);
#endif
