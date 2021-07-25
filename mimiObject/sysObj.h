#ifndef _MimiObj_sys__H
#define _MimiObj_sys__H
#include "dataMemory.h"
#include "MimiObj.h"

MimiObj *New_MimiObj_sys(Args *args);
void obj_import(MimiObj *self, char *className, void *classPtr);
void obj_importAndSetObj(MimiObj *self, char *objName, void * NewObjFun);
int obj_setObjbyClass(MimiObj *self, char *objName, char *classPath);
int obj_newObj(MimiObj *self, char *objPath, char *classPath);
#endif
