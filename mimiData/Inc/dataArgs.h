#ifndef _dataArgs__H
#define _dataArgs__H
#include "dataLink.h"
#include "dataMemory.h"

typedef struct Class_Arglist args_t;
struct Class_Arglist
{
    /* attribute */
    DMEM *mem;
    void *context;
    link_t *argLinkList;
    unsigned char nameBuff[ARG_NAME_LENGTH];

    /* operation */
    void (*dinit)(args_t *self);
    void (*init)(args_t *self, args_t *args);
    int (*size)(args_t *self);

    int (*getIndexByName)(args_t *self, char *name);
    arg_t *(*getArgByIndex)(args_t *self, int index);
    arg_t *(*getArgByName)(args_t *self, char *name);

    int (*setArg)(args_t *self, arg_t *arg);

    int (*copyArg)(args_t *self, char *name, args_t *directList);
    char *(*getTypeByName)(args_t *self, char *name);
    int (*isArgExist)(args_t *self, char *name);

    int (*setStr)(args_t *self, char *name, char *strIn);
    int (*getStr)(args_t *self, char *name, char **strOut);
    int (*setStrWithDefaultName)(args_t *self, char *strIn);
    int (*getStrByIndex)(args_t *self, int index, char **strOut);

    int (*setFloatWithDefaultName)(args_t *self, float argFloat);
    int (*setFlt)(args_t *self, char *name, float argFloat);
    float (*getFloatByIndex)(args_t *self, int index);
    float (*getFlt)(args_t *self, char *name);

    int (*setPoi)(args_t *self, char *name, void *argPointer);
    void *(*getPoi)(args_t *self, char *name);
    void *(*getPointerByIndex)(args_t *self, int index);

    int (*setInt)(args_t *self, char *name, long long int64In);
    long long (*getInt)(args_t *self, char *name);
    long long (*getInt64ByIndex)(args_t *self, int index);

    /* virtual operation */

    /* object */
};

args_t *New_args(args_t *args);
#endif
