#include "MimiObj.h"
#include "method.h"
#include "sysObj.h"
#include "dataMemory.h"
#include "dataString.h"
#include "strArgs.h"

static void *getClassPtr(MimiObj *classObj, Args *args, char *classPath)
{
    char *ptrPath = classPath;
    return obj_getPtr(classObj, ptrPath);
}

int obj_setObjbyClass(MimiObj *sys, char *objName, char *classPath)
{
    /* class means subprocess init */
    Args *buffs = New_strBuff();
    MimiObj *classHost = obj_getObj(sys, "class", 0);
    void *newFunPtr = getClassPtr(classHost, buffs, classPath);

    /* class means subprocess init */
    char prifix[] = "[cls]";
    char *mataObjName = args_getBuff(buffs, 256);
    strAppend(mataObjName, prifix);
    strAppend(mataObjName, objName);
    obj_setPtr(sys, mataObjName, newFunPtr);
    /* add void process Ptr, no inited */
    args_setObjectWithClass(sys->attributeList, objName, classPath, NULL);

    args_deinit(buffs);
    return 0;
}

static int storeClassInfo(MimiObj *self, Args *args, char *classPath, void *classPtr)
{
    int res = 0;
    MimiObj *classHost = obj_getObj(self, classPath, 1);
    if (NULL == classHost)
    {
        res = 1;
        goto exit;
    }
    char *className = strsGetLastToken(args, classPath, '.');
    char *classStoreName = className;
    obj_setPtr(classHost, classStoreName, classPtr);
    res = 0;
    goto exit;
exit:
    return res;
}

static void newObj(MimiObj *self, Args *args)
{
    /* get arg */
    char *objPath = args_getStr(args, "objPath");
    char *classPath = args_getStr(args, "classPath");
    /* operation */
    MimiObj *classObj = obj_getObj(self, "class", 0);
    void *NewObjPtr = getClassPtr(classObj, args, classPath);
    if(NULL == NewObjPtr)
    {
        method_sysOut(args, "[error] new: class not found .");
        method_setErrorCode(args, 1);
        return;
    }
    obj_setObjbyClass(self, objPath, classPath);
}

static void import(MimiObj *self, Args *args)
{
    char *classPath = args_getStr(args, "classPath");
    void *classPtr = args_getPtr(args, "classPtr");
    MimiObj *classObj = obj_getObj(self, "class", 0);
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
    Arg *arg = obj_getArg(obj, argPath);
    if (NULL == arg)
    {
        method_sysOut(args, "[error] arg no found.");
        args_setInt(args, "errCode", 1);
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
    char *argName = strGetLastToken(args_getBuff(args, 256), argPath, '.');
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
    if (NULL == handleArgs)
    {
        /* error: not handleArgs input */
        return 1;
    }

    char *argName = args_getBuff(handleArgs, 256);
    memcpy(argName, arg_getName(argEach), strGetSize(arg_getName(argEach)));
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
    args_setInt(args, "errCode", 0);
    args_setStr(args, "stringOut", "");
    char *objPath = args_getStr(args, "objPath");
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

void obj_import(MimiObj *self, char *className, void *classPtr)
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

void obj_importAndSetObj(MimiObj *sys, char *objName, void *NewObjFun)
{
    obj_import(sys, objName, NewObjFun);
    obj_setObjbyClass(sys, objName, objName);
}

static void init_sys(MimiObj *self, Args *args)
{
    /* attribute */

    /* method */
    obj_defineMethod(self, "print(val:any)", print);
    obj_defineMethod(self, "set(argPath:string, val:any)", set);
    obj_defineMethod(self, "ls(objPath:string)", list);
    obj_defineMethod(self, "del(argPath:string)", del);
    obj_defineMethod(self, "type(argPath:string)", type);
    obj_defineMethod(self, "import(classPath:string,classPtr:pointer)", import);
    obj_defineMethod(self, "new(objPath:string,classPath:string)", newObj);

    /* object */
    obj_setObjWithoutClass(self, "class", New_MimiObj);

    /* override */
}

MimiObj *New_MimiObj_sys(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    init_sys(self, args);
    return self;
}
