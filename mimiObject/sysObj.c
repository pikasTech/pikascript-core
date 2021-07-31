#include "MimiObj.h"
#include "method.h"
#include "sysObj.h"
#include "dataMemory.h"
#include "dataString.h"
#include "dataStrs.h"

static void *getClassPtr(MimiObj *classObj, char *classPath)
{
    char *ptrPath = classPath;
    return obj_getPtr(classObj, ptrPath);
}

int sysObj_setObjbyClass(MimiObj *self, char *objName, char *classPath)
{
    /* class means subprocess init */
    Args *buffs = New_strBuff();
    MimiObj *classHost = obj_getObj(self, "classLoader", 0);
    void *newFunPtr = getClassPtr(classHost, classPath);

    /* class means subprocess init */
    char *mataObjName = strsAppend(buffs, "[mate]", objName);
    obj_setPtr(self, mataObjName, newFunPtr);
    /* add void process Ptr, no inited */
    args_setObjectWithClass(self->attributeList, objName, classPath, NULL);

    args_deinit(buffs);
    return 0;
}

static int storeClassInfo(MimiObj *self, Args *buffs, char *classPath, void *classPtr)
{
    int res = 0;
    MimiObj *classHost = obj_getObj(self, classPath, 1);
    if (NULL == classHost)
    {
        res = 1;
        goto exit;
    }
    char *className = strsGetLastToken(buffs, classPath, '.');
    char *classStoreName = className;
    obj_setPtr(classHost, classStoreName, classPtr);
    res = 0;
    goto exit;
exit:
    return res;
}

int obj_newObj(MimiObj *self, char *objPath, char *classPath)
{
    MimiObj *classLoader = obj_getObj(self, "classLoader", 0);
    void *NewObjPtr = getClassPtr(classLoader, classPath);
    if (NULL == NewObjPtr)
    {
        return 1;
    }
    sysObj_setObjbyClass(self, objPath, classPath);
    return 0;
}

static void newObjMethod(MimiObj *self, Args *args)
{
    /* get arg */
    char *objPath = args_getStr(args, "objPath");
    char *classPath = args_getStr(args, "classPath");
    int res = obj_newObj(self, objPath, classPath);
    if (1 == res)
    {
        method_sysOut(args, "[error] new: class not found .");
        method_setErrorCode(args, 1);
        return;
    }
}

static void import(MimiObj *self, Args *args)
{
    char *classPath = args_getStr(args, "classPath");
    void *classPtr = args_getPtr(args, "classPtr");
    MimiObj *classObj = obj_getObj(self, "classLoader", 0);
    int res = storeClassInfo(classObj, args, classPath, classPtr);
    if (1 == res)
    {
        args_setInt(args, "errCode", 1);
        method_sysOut(args, "[error] class host not found.");
    }
}

static void type(MimiObj *obj, Args *args)
{
    args_setInt(args, "errCode", 0);
    char *argPath = args_getStr(args, "argPath");
    if (NULL == argPath)
    {
        /* no input obj path, use current obj */
        MimiObj *objHost = obj_getPtr(obj, "context");
        Arg *objArg = obj_getArg(objHost, obj->name);
        if (NULL == objArg)
        {
            method_sysOut(args, "[error] type: arg no found.");
            method_setErrorCode(args, 1);
            return;
        }
        method_sysOut(args, arg_getType(objArg));
        return;
    }
    Arg *arg = obj_getArg(obj, argPath);
    if (NULL == arg)
    {
        method_sysOut(args, "[error] type: arg no found.");
        method_setErrorCode(args, 1);
        return;
    }
    method_sysOut(args, arg_getType(arg));
}

static void del(MimiObj *obj, Args *args)
{
    args_setInt(args, "errCode", 0);
    char *argPath = args_getStr(args, "argPath");
    int res = obj_removeArg(obj, argPath);
    if (1 == res)
    {
        method_sysOut(args, "[error] del: object no found.");
        args_setInt(args, "errCode", 1);
        return;
    }
    if (2 == res)
    {
        method_sysOut(args, "[error] del: arg not match.");
        args_setInt(args, "errCode", 2);
        return;
    }
}

static void set(MimiObj *obj, Args *args)
{
    args_setInt(args, "errCode", 0);
    char *argPath = method_getStr(args, "argPath");
    if (obj_isArgExist(obj, argPath))
    {
        /* update arg */
        char *valStr = args_print(args, "val");
        int res = obj_set(obj, argPath, valStr);
        if (1 == res)
        {
            method_sysOut(args, "[error] set: arg no found.");
            args_setInt(args, "errCode", 1);
            return;
        }
        if (2 == res)
        {
            method_sysOut(args, "[error] set: type not match.");
            args_setInt(args, "errCode", 1);
            return;
        }
        if (3 == res)
        {
            method_sysOut(args, "[error] set: object not found.");
            args_setInt(args, "errCode", 1);
            return;
        }
        return;
    }
    /* new arg */
    Arg *val = args_getArg(args, "val");
    Arg *newArg = arg_copy(val);
    char *argName = strsGetLastToken(args, argPath, '.');
    arg_setName(newArg, argName);
    int res = obj_setArg(obj, argPath, newArg);
    if (res == 1)
    {
        method_sysOut(args, "[error] set: object not found.");
        args_setInt(args, "errCode", 1);
    }
    arg_deinit(newArg);
    return;
}

static int listEachArg(Arg *argEach, Args *handleArgs)
{
    Args *buffs = handleArgs;
    if (NULL == handleArgs)
    {
        /* error: not handleArgs input */
        return 1;
    }

    char *argName = strsCopy(buffs, arg_getName(argEach));
    if (strIsStartWith(argName, "["))
    {
        /* skip */
        return 0;
    }

    char *stringOut = args_getStr(handleArgs, "stringOut");
    if (NULL == stringOut)
    {
        // stringOut no found
        return 1;
    }

    strAppend(stringOut, argName);
    strAppend(stringOut, " ");
    args_setStr(handleArgs, "stringOut", stringOut);
    return 0;
}

static void list(MimiObj *self, Args *args)
{
    char *objPath = args_getStr(args, "objPath");
    args_setInt(args, "errCode", 0);
    args_setStr(args, "stringOut", "");
    if (NULL == objPath)
    {
        /* no input obj path, use current obj */
        args_foreach(self->attributeList, listEachArg, args);
        method_sysOut(args, args_getStr(args, "stringOut"));
        return;
    }
    MimiObj *obj = obj_getObj(self, objPath, 0);
    if (NULL == obj)
    {
        /* do not find obj */
        method_sysOut(args, "[error] list: object no found.");
        method_setErrorCode(args, 1);
        return;
    }
    /* list args */
    args_foreach(obj->attributeList, listEachArg, args);
    method_sysOut(args, args_getStr(args, "stringOut"));
    return;
}

static void print(MimiObj *obj, Args *args)
{
    args_setInt(args, "errCode", 0);
    char *res = args_print(args, "val");
    if (NULL == res)
    {
        method_sysOut(args, "[error] print: can not print val");
        args_setInt(args, "errCode", 1);
        return;
    }
    /* not empty */
    method_sysOut(args, res);
}

int obj_import(MimiObj *self, char *className, void *classPtr)
{
    MimiObj *classLoader = obj_getObj(self, "classLoader", 0);
    Args *buffs = New_args(NULL);
    int res = storeClassInfo(classLoader, buffs, className, classPtr);
    args_deinit(buffs);
    return res;
}

void sysObj_importByCmd(MimiObj *self, char *className, void *classPtr)
{
    Args *buffs = New_strBuff();
    {
        char *cmd = args_getBuff(buffs, 256);
        obj_setPtr(self, className, classPtr);
        sprintf(cmd, "import('%s',%s)", className, className);
        obj_run(self, cmd);
    }
    {
        char *cmd = args_getBuff(buffs, 256);
        sprintf(cmd, "del('%s')", className);
        obj_run(self, cmd);
        args_deinit(buffs);
    }
}

void sysObj_importAndSetObj(MimiObj *sys, char *objName, void *NewObjFun)
{
    obj_import(sys, objName, NewObjFun);
    obj_newObj(sys, objName, objName);
}

int loadExceptMethod(Arg *argEach, Args *handleArgs)
{
    char *argName = arg_getName(argEach);
    if (strIsStartWith(argName, "[methodDec]"))
    {
        /* skip method declearation */
        // return 0;
    }
    if (strIsStartWith(argName, "[methodPtr]"))
    {
        /* skip method pointer */
        // return 0;
    }
    args_copyArg(handleArgs, argEach);
    return 0;
}

MimiObj *obj_loadWithoutMethod(MimiObj *thisClass)
{
    MimiObj *newObj = New_MimiObj(NULL);
    Args *thisClassArgs = thisClass->attributeList;
    Args *newObjArgs = newObj->attributeList;
    args_foreach(thisClassArgs, loadExceptMethod, newObjArgs);
    return newObj;
}

MimiObj *New_MimiObj_sys(Args *args)
{
    /* derive */
    MimiObj *self = New_MimiObj(args);

    /* attribute */

    /* method */
    class_defineMethod(self, "print(val:any)", print);
    class_defineMethod(self, "set(argPath:string, val:any)", set);
    class_defineMethod(self, "ls(objPath:string)", list);
    class_defineMethod(self, "del(argPath:string)", del);
    class_defineMethod(self, "type(argPath:string)", type);
    class_defineMethod(self, "import(classPath:string,classPtr:pointer)", import);
    class_defineMethod(self, "new(objPath:string,classPath:string)", newObjMethod);

    /* object */
    obj_setObjWithoutClass(self, "classLoader", New_MimiObj);
    /* 
        init classLoader now, in order to the 
        find it after inited the self object.
    */
    obj_getObj(self, "classLoader", 0);

    /* override */

    return self;
}
