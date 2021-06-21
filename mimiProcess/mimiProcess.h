#ifndef _Process__H
#define _Process__H
#include "dataArgs.h"
#include "dataLink.h"
#include "dataMemory.h"
typedef struct Class_process mimiProcess_t;

struct Class_process
{
    /* attribute */
    DMEM *mem;

    /* list */
    link_t *subProcessList;
    args_t *attributeList;

    /* operation */
    void (*dinit)(mimiProcess_t *self);
    void (*init)(mimiProcess_t *self, args_t *args);
    void (*update)(mimiProcess_t *self);
    void (*enable)(mimiProcess_t *self);
    void (*disable)(mimiProcess_t *self);

    void (*setInt64)(mimiProcess_t *self, char *name, long long val);
    long long (*getInt64)(mimiProcess_t *self, char *name);

    void (*setPoi)(mimiProcess_t *self, char *name, void *pointer);
    void *(*getPointer)(mimiProcess_t *self, char *name);

    void (*setFloat)(mimiProcess_t *self, char *name, float value);
    float (*getFloat)(mimiProcess_t *self, char *name);

    void (*setStr)(mimiProcess_t *self, char *name, char *str);
    void (*getStr)(mimiProcess_t *self, char *name, char **strOut);

    void (*loadAttributeFromArgs)(mimiProcess_t *self, args_t *args, char *name);

    // subObject

    void (*addSubobject)(mimiProcess_t *self, char *subObjectName, void *new_projcetFun);
    void (*dinitSubProcess)(mimiProcess_t *self, char *subObjectName);

    /* virtual operation */
    void (*_beforDinit)(mimiProcess_t *self);
    void (*_updateHandle)(mimiProcess_t *self);

    /* object */
};

mimiProcess_t *New_mimiProcess(args_t *args);
#endif
