#include "PikaObj.h"
#include "TinyObj.h"
#include "method.h"
#include "SysObj.h"
#include "BaseObj.h"
#include "dataMemory.h"
#include "dataString.h"
#include "dataStrs.h"

static void newObjMethod(PikaObj *self, Args *args)
{
    /* get arg */
    char *objPath = args_getStr(args, "objPath");
    char *classPath = args_getStr(args, "classPath");
    int32_t res = obj_newObj(self, objPath, classPath);
    if (1 == res)
    {
        obj_setSysOut(self, "[error] new: class not found .");
        obj_setErrorCode(self, 1);
        return;
    }
}

static void type(PikaObj *self, Args *args)
{
    obj_setErrorCode(self, 0);
    char *argPath = args_getStr(args, "argPath");
    if (NULL == argPath)
    {
        /* no input obj path, use current obj */
        PikaObj *objHost = obj_getPtr(self, "__context");
        Arg *objArg = obj_getArg(objHost, self->name);
        if (NULL == objArg)
        {
            obj_setSysOut(self, "[error] type: arg no found.");
            obj_setErrorCode(self, 1);
            return;
        }
        obj_setSysOut(self, arg_getType(objArg));
        return;
    }
    Arg *arg = obj_getArg(self, argPath);
    if (NULL == arg)
    {
        obj_setSysOut(self, "[error] type: arg no found.");
        obj_setErrorCode(self, 1);
        return;
    }
    obj_setSysOut(self, arg_getType(arg));
}

static void del(PikaObj *self, Args *args)
{
    obj_setErrorCode(self, 0);
    char *argPath = args_getStr(args, "argPath");
    int32_t res = obj_removeArg(self, argPath);
    if (1 == res)
    {
        obj_setSysOut(self, "[error] del: object no found.");
        obj_setErrorCode(self, 1);
        return;
    }
    if (2 == res)
    {
        obj_setSysOut(self, "[error] del: arg not match.");
        obj_setErrorCode(self, 2);
        return;
    }
}

static void set(PikaObj *self, Args *args)
{
    obj_setErrorCode(self, 0);
    char *argPath = method_getStr(args, "argPath");
    if (obj_isArgExist(self, argPath))
    {
        /* update arg */
        char *valStr = args_print(args, "val");
        int32_t res = obj_set(self, argPath, valStr);
        if (1 == res)
        {
            obj_setSysOut(self, "[error] set: arg no found.");
            obj_setErrorCode(self, 1);
            return;
        }
        if (2 == res)
        {
            obj_setSysOut(self, "[error] set: type not match.");
            obj_setErrorCode(self, 1);
            return;
        }
        if (3 == res)
        {
            obj_setSysOut(self, "[error] set: object not found.");
            obj_setErrorCode(self, 1);
            return;
        }
        return;
    }
    /* new arg */
    Arg *val = args_getArg(args, "val");
    Arg *newArg = arg_copy(val);
    char *argName = strsGetLastToken(args, argPath, '.');
    arg_setName(newArg, argName);
    int32_t res = obj_setArg(self, argPath, newArg);
    if (res == 1)
    {
        obj_setSysOut(self, "[error] set: object not found.");
        obj_setErrorCode(self, 1);
    }
    arg_deinit(newArg);
    newArg = NULL;
    return;
}

static int32_t __foreach_listEachArg(Arg *argEach, Args *handleArgs)
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

    stringOut = strsAppend(buffs, stringOut, argName);
    stringOut = strsAppend(buffs, stringOut, " ");
    args_setStr(handleArgs, "stringOut", stringOut);
    return 0;
}

static void list(PikaObj *self, Args *args)
{
    char *objPath = args_getStr(args, "objPath");
    obj_setErrorCode(self, 0);
    args_setStr(args, "stringOut", "");
    if (NULL == objPath)
    {
        /* no input obj path, use current obj */
        args_foreach(self->attributeList, __foreach_listEachArg, args);
        obj_setSysOut(self, args_getStr(args, "stringOut"));
        return;
    }
    PikaObj *obj = obj_getObj(self, objPath, 0);
    if (NULL == obj)
    {
        /* do not find obj */
        obj_setSysOut(self, "[error] list: object no found.");
        obj_setErrorCode(self, 1);
        return;
    }
    /* list args */
    args_foreach(obj->attributeList, __foreach_listEachArg, args);
    obj_setSysOut(self, args_getStr(args, "stringOut"));
    return;
}

static void print(PikaObj *self, Args *args)
{
    obj_setErrorCode(self, 0);
    char *res = args_print(args, "val");
    if (NULL == res)
    {
        obj_setSysOut(self, "[error] print: can not print32_t val");
        obj_setErrorCode(self, 1);
        return;
    }
    /* not empty */
    obj_setSysOut(self, res);
}

int32_t loadExceptMethod(Arg *argEach, Args *handleArgs)
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

PikaObj *obj_loadWithoutMethod(PikaObj *thisClass)
{
    PikaObj *newObj = New_TinyObj(NULL);
    Args *thisClassArgs = thisClass->attributeList;
    Args *newObjArgs = newObj->attributeList;
    args_foreach(thisClassArgs, loadExceptMethod, newObjArgs);
    return newObj;
}

PikaObj *New_SysObj(Args *args)
{
    /* derive */
    PikaObj *self = New_BaseObj(args);

    /* attribute */

    /* method */
    class_defineMethod(self, "print(val:any)", print);
    class_defineMethod(self, "set(argPath:str, val:any)", set);
    class_defineMethod(self, "ls(objPath:str)", list);
    class_defineMethod(self, "del(argPath:str)", del);
    class_defineMethod(self, "type(argPath:str)", type);
    class_defineMethod(self, "new(objPath:str,classPath:str)", newObjMethod);

    /* override */

    return self;
}
