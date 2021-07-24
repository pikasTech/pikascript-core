#include "MimiObj.h"
#include "MimiClass.h"
#include "sysObj.h"
#include "mimiCom.h"
#include "dataString.h"

int TEST_sysObj(int isShow)
{
    {
        MimiObj *sys = New_MimiObj_sys(NULL);
        obj_import(sys, "Base", New_MimiObj);
        obj_setObjbyClass(sys, "base", "Base");
        obj_run(sys, "ls()");
        obj_run(sys, "ls('class')");
        obj_run(sys, "print('print Class-Base:')");
        obj_run(sys, "print(class.Class-Base)");
        obj_run(sys, "type('base')");
        obj_run(sys, "ls('base')");
        obj_deinit(sys);
    }
    return 0;
}
