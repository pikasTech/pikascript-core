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

    // arg type operations
    void (*setInt)(mimiProcess_t *self, char *name, long long val);
    void (*setPtr)(mimiProcess_t *self, char *name, void *pointer);
    void (*setFloat)(mimiProcess_t *self, char *name, float value);
    void (*setStr)(mimiProcess_t *self, char *name, char *str);

    void *(*getPtr)(mimiProcess_t *self, char *name);
    float (*getFloat)(mimiProcess_t *self, char *name);
    char *(*getStr)(mimiProcess_t *self, char *name);
    long long (*getInt)(mimiProcess_t *self, char *name);

    // arg general operations
    void (*argBind)(mimiProcess_t *self, char *type, char *name, void *pointer);
    char * (*argPinrt)(mimiProcess_t *self, char *name);

    // args operations
    void (*loadAttributeFromArgs)(mimiProcess_t *self, args_t *args, char *name);

    // subObject
    void (*addSubobject)(mimiProcess_t *self, char *subObjectName, void *new_projcetFun);
    void (*addSubProcess)(mimiProcess_t *self, char *subObjectName, void *new_projcetFun);

    // subProcess
    void (*dinitSubProcess)(mimiProcess_t *self, char *subObjectName);

    /* virtual operation */
    void (*_beforDinit)(mimiProcess_t *self);
    void (*_updateHandle)(mimiProcess_t *self);
    void (*_portableInit)(mimiProcess_t *self);

    /* object */
};

mimiProcess_t *New_mimiProcess(args_t *args);
#endif
