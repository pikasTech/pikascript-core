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
void obj_deinit(MimiObj *self);
void obj_init(MimiObj *self, Args *args);
void obj_update(MimiObj *self);
void obj_enable(MimiObj *self);
void obj_disable(MimiObj *self);

// arg type operations
void obj_setInt(MimiObj *self, char *name, long long val);
void obj_setPtr(MimiObj *self, char *name, void *pointer);
void obj_setFloat(MimiObj *self, char *name, float value);
void obj_setStr(MimiObj *self, char *name, char *str);

void *obj_getPtr(MimiObj *self, char *name);
float obj_getFloat(MimiObj *self, char *name);
char *obj_getStr(MimiObj *self, char *name);
long long obj_getInt(MimiObj *self, char *name);

// arg general operations
void obj_bind(MimiObj *self, char *type, char *name, void *pointer);
int obj_set(MimiObj *self, char *name, char *valStr);

void obj_bindInt(MimiObj *self, char *name, int *valPtr);
void obj_bindFloat(MimiObj *self, char *name, float *valPtr);
void obj_bindString(MimiObj *self, char *name, char **valPtr);

char *obj_print(MimiObj *self, char *name);

// args operations
void obj_load(MimiObj *self, Args *args, char *name);

// subObject
void obj_addOther(MimiObj *self, char *subObjectName, void *new_projcetFun);
void obj_setObj(MimiObj *self, char *subObjectName, void *new_projcetFun);
MimiObj *obj_getDirectObj(MimiObj *self, char *name);
MimiObj *obj_getObj(MimiObj *self, char *processDirectory, int deepth);

// subProcess
void obj_freeObj(MimiObj *self, char *subObjectName);

/* method */
void obj_setMethod(MimiObj *self,
                      char *declearation,
                      void (*methodPtr)(MimiObj *self, Args *args));

void obj_run(MimiObj *self, char *cmd);
