#include "MimiObj.h"
#include "dataMemory.h"
#include "strArgs.h"
#include "dataString.h"

int storeClassInfo(MimiObj *self, char *classPath, void *classPtr)
{
    int res = 0;
    Args *buffs = New_strBuff();
    MimiObj *classHost = obj_getObj(self, classPath, 1);
    if (NULL == classHost)
    {
        res = 1;
        goto exit;
    }
    char *className = strsGetLastToken(buffs, classPath, '.');
    char *classStoreName = strAppend(strAppend(args_getBuff(buffs, 128), "[className]"), className);
    // obj_setPtr(classHost, classStoreName, classPtr);
    res = 0;
    goto exit;
exit:
    args_deinit(buffs);
    return res;
}

static void import(MimiObj *self, Args *args)
{
    char *classPath = args_getStr(args, "classPath");
    void *classPtr = args_getPtr(args, "classPtr");
    int res = storeClassInfo(self, classPath, classPtr);
    if (1 == res)
    {
        method_sysOut(args, "[error] class host not found.");
    }
}

static void init_class(MimiObj *self, Args *args)
{
    /* attribute */

    /* operation */
    obj_defineMethod(self, "import(classPath:string,classPtr:pointer)", import);

    /* object */

    /* override */
}

MimiObj *New_MimiObj_class(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    init_class(self, args);
    return self;
}