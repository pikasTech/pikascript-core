#ifndef _Process__H
#define _Process__H
#include "dataArgs.h"
#include "dataLink.h"
#include "dataMemory.h"
typedef struct Class_process MimiProcess;


struct Class_process
{
    /* attribute */
    DMEM *mem;

    /* list */
    Args *attributeList;

    /* operation */
    void (*deinit)(MimiProcess *self);
    void (*init)(MimiProcess *self, Args *args);
    void (*update)(MimiProcess *self);
    void (*enable)(MimiProcess *self);
    void (*disable)(MimiProcess *self);

    // arg type operations
    void (*setInt)(MimiProcess *self, char *name, long long val);
    void (*setPtr)(MimiProcess *self, char *name, void *pointer);
    void (*setFloat)(MimiProcess *self, char *name, float value);
    void (*setStr)(MimiProcess *self, char *name, char *str);

    void *(*getPtr)(MimiProcess *self, char *name);
    float (*getFloat)(MimiProcess *self, char *name);
    char *(*getStr)(MimiProcess *self, char *name);
    long long (*getInt)(MimiProcess *self, char *name);

    // arg general operations
    void (*argBind)(MimiProcess *self, char *type, char *name, void *pointer);
    int (*argSet)(MimiProcess *self, char *name, char *valStr);

    void (*argBindInt)(MimiProcess *self, char *name, int *valPtr);
    void (*argBindFloat)(MimiProcess *self, char *name, float *valPtr);
    void (*argBindString)(MimiProcess *self, char *name, char **valPtr);

    char *(*argPrint)(MimiProcess *self, char *name);

    // args operations
    void (*loadAttributeFromArgs)(MimiProcess *self, Args *args, char *name);

    // subObject
    void (*addSubobject)(MimiProcess *self, char *subObjectName, void *new_projcetFun);
    void (*addSubProcess)(MimiProcess *self, char *subObjectName, void *new_projcetFun);
    MimiProcess *(*getSubProcess)(MimiProcess *self, char *name);
    MimiProcess *(*goToProcess)(MimiProcess *self, char *processDirectory, int deepth);

    // subProcess
    void (*dinitSubProcessByName)(MimiProcess *self, char *subObjectName);

    /* virtual operation */
    void (*_beforDinit)(MimiProcess *self);
    void (*_updateHandle)(MimiProcess *self);

    /* event operation */

    void (*subscribe)(MimiProcess *self, char *subscribeVarName, void (*handle)(MimiProcess *self));

    /* object */
};

MimiProcess *New_MimiProcess(Args *args);
#endif
