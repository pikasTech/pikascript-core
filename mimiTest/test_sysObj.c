#include "MimiObj.h"
#include "MimiClass.h"
#include "sysObj.h"
#include "mimiCom.h"
#include "dataString.h"

int TEST_sysObj(int isShow)
{
    {
        MimiObj *sys = New_MimiObj_sys(NULL);
        obj_import(sys, "Sys", New_MimiObj_sys);
        obj_setObjbyClass(sys, "sys", "Sys");
        obj_run(sys, "sys.ls()");
        obj_deinit(sys);
    }
    return 0;
}
