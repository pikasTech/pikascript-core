#ifndef _MimiObj_sys__H
#define _MimiObj_sys__H
#include "dataMemory.h"
#include "MimiObj.h"

MimiObj *New_MimiObj_sys(Args *args);
void obj_import(MimiObj *self, char *className, void *classPtr);
int obj_setObjbyClass(MimiObj *sys, char *objName, char *classPath);
void obj_importAndSetObj(MimiObj *sys, char *objName, void * NewObjFun);
MimiObj *obj_loadWithoutMethod(MimiObj *thisClass);
#endif
