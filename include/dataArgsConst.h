#ifndef _VM_args__H
#define _VM_args__H
#include "dataArg.h"
#include "dataMemory.h"
typedef struct Class_argsConst argsConst_t;

#define ARGS_LIST_LENGTH 8
struct Class_argsConst
{
    /* attribute */
    DMEM *mem;
    void *context;
    int count;
    arg_t *argList[ARGS_LIST_LENGTH];

    /* operation */
    void (*dinit)(argsConst_t *self);
    void (*init)(argsConst_t *self, argsConst_t *args);

    int (*pushStrWithName)(argsConst_t *self, char *name, char *strIn);
    int (*getStrByName)(argsConst_t *self, char *name, char **strOut);

    int (*getIndexByName)(argsConst_t *self, char *name);

    int (*pushStrWithDefaultName)(argsConst_t *self, char *strIn);
    int (*getStrByIndex)(argsConst_t *self, int index, char **strOut);

    int (*pushFloatWithDefaultName)(argsConst_t *self, float argFloat);
    float (*getFloatByIndex)(argsConst_t *self, int index);

    int (*pushPointerWithName)(argsConst_t *self, char *name, void *argPointer);
    void *(*getPointerByName)(argsConst_t *self, char *name);
    void *(*getPointerByIndex)(argsConst_t *self, int index);

    int (*pushInt64WithName)(argsConst_t *self, char *name, long long int64In);
    long long (*getInt64ByName)(argsConst_t *self, char *name);
    long long (*getInt64ByIndex)(argsConst_t *self, int index);

    /* virtual operation */

    /* object */
};

argsConst_t *New_argsConst(argsConst_t *args);
#endif