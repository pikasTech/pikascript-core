#ifndef _Process__H
#define _Process__H
#include "dataArgs.h"
#include "dataLink.h"
#include "dataMemory.h"
typedef struct Class_process MimiObj;

struct Class_process
{
    /* attribute */
    DMEM *mem;

    /* list */
    Args *attributeList;
    char name[32];

    /* operation */
    void (*deinit)(MimiObj *self);
    void (*init)(MimiObj *self, Args *args);
    void (*update)(MimiObj *self);
    void (*enable)(MimiObj *self);
    void (*disable)(MimiObj *self);

    // arg type operations
    void (*setInt)(MimiObj *self, char *name, long long val);
    void (*setPtr)(MimiObj *self, char *name, void *pointer);
    void (*setFloat)(MimiObj *self, char *name, float value);
    void (*setStr)(MimiObj *self, char *name, char *str);

    void *(*getPtr)(MimiObj *self, char *name);
    float (*getFloat)(MimiObj *self, char *name);
    char *(*getStr)(MimiObj *self, char *name);
    long long (*getInt)(MimiObj *self, char *name);

    // arg general operations
    void (*bind)(MimiObj *self, char *type, char *name, void *pointer);
    int (*set)(MimiObj *self, char *name, char *valStr);

    void (*bindInt)(MimiObj *self, char *name, int *valPtr);
    void (*bindFloat)(MimiObj *self, char *name, float *valPtr);
    void (*bindString)(MimiObj *self, char *name, char **valPtr);

    char *(*print)(MimiObj *self, char *name);

    // args operations
    void (*load)(MimiObj *self, Args *args, char *name);

    // subObject
    void (*addOther)(MimiObj *self, char *subObjectName, void *new_projcetFun);
    void (*setObj)(MimiObj *self, char *subObjectName, void *new_projcetFun);
    MimiObj *(*getDirectObj)(MimiObj *self, char *name);
    MimiObj *(*getObj)(MimiObj *self, char *processDirectory, int deepth);

    // subProcess
    void (*freeObj)(MimiObj *self, char *subObjectName);

    /* virtual operation */
    void (*_beforDinit)(MimiObj *self);
    void (*_updateHandle)(MimiObj *self);

    /* event operation */
    void (*follow)(MimiObj *self, char *subscribeVarName, void (*handle)(MimiObj *self));
    void (*publish)(MimiObj *self, char *argName);

    /* method */
    void (*setMethod)(MimiObj *self,
                      char *name,
                      void (*method)(MimiObj *self, Args *args));
    
    void (*runMethod)(MimiObj *self, char *methodName, Args *args);
    
};
MimiObj *New_MimiObj(Args *args);
#endif
