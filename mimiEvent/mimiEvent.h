#ifndef _MimiObj_Event__H
#define _MimiObj_Event__H
#include "dataMemory.h"
#include "MimiObj.h"

MimiObj *New_MimiObj_Event(Args *args);
void event_follow(MimiObj *self, char *argPath, void *handle);

#endif
