#ifndef _MimiObj_sys__H
#define _MimiObj_sys__H
#include "dataMemory.h"
#include "MimiObj.h"

MimiObj *New_MimiObj_sys(Args *args);
<<<<<<< HEAD
void obj_importByCmd(MimiObj *self, char *className, void *classPtr);
void obj_importAndSetObj(MimiObj *self, char *objName, void * NewObjFun);
int obj_setObjbyClass(MimiObj *self, char *objName, char *classPath);
int obj_newObj(MimiObj *self, char *objPath, char *classPath);
int obj_import(MimiObj *self, char *className, void *classPtr);
=======
void obj_import(MimiObj *self, char *className, void *classPtr);
int obj_setObjbyClass(MimiObj *sys, char *objName, char *classPath);
void obj_importAndSetObj(MimiObj *sys, char *objName, void * NewObjFun);
MimiObj *obj_loadWithoutMethod(MimiObj *thisClass);
>>>>>>> d29517d7e63d0ff1398e0b4cf5f7596aba03a129
#endif
