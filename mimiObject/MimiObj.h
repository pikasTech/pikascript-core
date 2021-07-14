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
    char *name;

    /* virtual operation */
    void (*_beforDinit)(MimiObj *self);
    void (*_updateHandle)(MimiObj *self);
};
MimiObj *New_MimiObj(Args *args);
#endif

/* operation */
int obj_deinit(MimiObj *self);
int obj_init(MimiObj *self, Args *args);
int obj_update(MimiObj *self);
int obj_enable(MimiObj *self);
int obj_disable(MimiObj *self);

// arg type operations
int obj_setInt(MimiObj *self, char *name, long long val);
int obj_setPtr(MimiObj *self, char *name, void *pointer);
int obj_setFloat(MimiObj *self, char *name, float value);
int obj_setStr(MimiObj *self, char *name, char *str);

void *obj_getPtr(MimiObj *self, char *name);
float obj_getFloat(MimiObj *self, char *name);
char *obj_getStr(MimiObj *self, char *name);
long long obj_getInt(MimiObj *self, char *name);

// arg general operations
int obj_bind(MimiObj *self, char *type, char *name, void *pointer);
int obj_set(MimiObj *self, char *name, char *valStr);

int obj_bindInt(MimiObj *self, char *name, int *valPtr);
int obj_bindFloat(MimiObj *self, char *name, float *valPtr);
int obj_bindString(MimiObj *self, char *name, char **valPtr);

char *obj_print(MimiObj *self, char *name);

// args operations
int obj_load(MimiObj *self, Args *args, char *name);

// subObject
int obj_addOther(MimiObj *self, char *subObjectName, void *new_projcetFun);
int obj_newObj(MimiObj *self, char *subObjectName, void *new_projcetFun);
MimiObj *obj_getObjDirect(MimiObj *self, char *name);
MimiObj *obj_getObj(MimiObj *self, char *processDirectory, int deepth);

// subProcess
int obj_freeObj(MimiObj *self, char *subObjectName);

/* method */
int obj_defineMethod(MimiObj *self,
                      char *declearation,
                      void (*methodPtr)(MimiObj *self, Args *args));

int obj_run(MimiObj *self, char *cmd);
