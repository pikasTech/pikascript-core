#ifndef _MimiObj_sys__H
#define _MimiObj_sys__H
#include "dataMemory.h"
#include "MimiObj.h"

MimiObj *New_MimiObj_sys(Args *args);
void sysObj_importByCmd(MimiObj *self, char *className, void *classPtr);
void sysObj_importAndSetObj(MimiObj *self, char *objName, void * NewObjFun);
int sysObj_setObjbyClass(MimiObj *self, char *objName, char *classPath);
int sysObj_newObj(MimiObj *self, char *objPath, char *classPath);
int sysObj_import(MimiObj *self, char *className, void *classPtr);
#endif
