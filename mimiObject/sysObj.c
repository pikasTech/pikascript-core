#include "MimiObj.h"
#include "dataMemory.h"
#include "dataString.h"
#include "strArgs.h"

static void type(MimiObj *obj, Args *args)
{
    char *argPath = args_getStr(args, "argPath");
    Arg *arg = obj_getArg(obj, argPath);
    if (NULL == arg)
    {
        method_sysOut(args, "[error] arg no found.");
        return;
    }
    method_sysOut(args, arg_getType(arg));
}

static void del(MimiObj *obj, Args *args)
{
    char *argPath = args_getStr(args, "argPath");
    int res = obj_removeArg(obj, argPath);
    if (1 == res)
    {
        method_sysOut(args, "[error] del: object no found.");
        return;
    }
    if (2 == res)
    {
        method_sysOut(args, "[error] del: arg not match.");
        return;
    }
}

static void set(MimiObj *obj, Args *args)
{
    char *argPath = args_getStr(args, "argPath");
    if (obj_isArgExist(obj, argPath))
    {
        /* update arg */
        char *valStr = args_print(args, "val");
        int res = obj_set(obj, argPath, valStr);
        if (1 == res)
        {
            method_sysOut(args, "[error] set: arg no found.");
            return;
        }
        if (2 == res)
        {
            method_sysOut(args, "[error] set: type not match.");
            return;
        }
        if (3 == res)
        {
            method_sysOut(args, "[error] set: object not found.");
            return;
        }
        return;
    }
    /* new arg */
    Arg *val = args_getArg(args, "val");
    Arg *newArg = arg_copy(val);
    char *argName = strGetLastToken(args_getBuff(args, 64), argPath, '.');
    arg_setName(newArg, argName);
    int res = obj_setArg(obj, argPath, newArg);
    if (res == 1)
    {
        method_sysOut(args, "[error] set: object not found.");
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
        return;
    }
    /* list args */
    args_foreach(obj->attributeList, listEachArg, args);
    method_sysOut(args, args_getStr(args, "stringOut"));
    return;
}

static void print(MimiObj *obj, Args *args)
{
    char *res = args_print(args, "val");
    if (NULL == res)
    {
        method_sysOut(args, "[error] print: can not print val");
        return;
    }
    /* not empty */
    method_sysOut(args, res);
}

static void init_sys(MimiObj *self, Args *args)
{
    /* attrivute */

    /* method */
    obj_defineMethod(self, "print(val:any)", print);
    obj_defineMethod(self, "set(argPath:string, val:any)", set);
    obj_defineMethod(self, "ls(objPath:string)", list);
    obj_defineMethod(self, "del(argPath:string)", del);
    obj_defineMethod(self, "type(argPath:string)", type);

    /* object */

    /* override */
}

MimiObj *New_MimiObj_sys(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    init_sys(self, args);
    return self;
}
