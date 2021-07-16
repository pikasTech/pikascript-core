#include "MimiObj.h"
#include "dataMemory.h"
#include "dataString.h"

static void type(MimiObj *obj, Args *args)
{
    char *argPath = args_getStr(args, "argPath");
    Arg *arg = obj_getArg(obj, argPath);
    if (NULL == arg)
    {
        printf("[error] arg '%s' no found.\r\n", argPath);
        return;
    }
    printf("<%s>\r\n", arg_getType(arg));
}

static void del(MimiObj *obj, Args *args)
{
    char *argPath = args_getStr(args, "argPath");
    obj_removeArg(obj, argPath);
}

static void set(MimiObj *obj, Args *args)
{
    char *argPath = args_getStr(args, "argPath");
    if (obj_isArgExist(obj, argPath))
    {
        /* update arg */
        char *valStr = args_print(args, "val");
        obj_set(obj, argPath, valStr);
        return;
    }
    /* new arg */
    Arg *val = args_getArg(args, "val");
    Arg *newArg = arg_copy(val);
    char buff[64] = {0};
    char *argName = strGetLastToken(buff, argPath, '.');
    arg_setName(newArg, argName);
    obj_setArg(obj, argPath, newArg);
    arg_deinit(newArg);
    return;
}

static int listEachArg(Arg *argEach, Args *handleArgs)
{
    char strBuff[2][256] = {0};
    int i = 0;

    if (NULL == handleArgs)
    {
        /* error: not handleArgs input */
        return 1;
    }

    char *argName = strBuff[i++];
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
        printf("%s\r\n", args_getStr(args, "stringOut"));
        return;
    }
    MimiObj *obj = obj_getObj(self, objPath, 0);
    if (NULL == obj)
    {
        /* do not find obj */
        printf("[error] list: object '%s' no found.\r\n", objPath);
        return;
    }
    /* list args */
    args_foreach(obj->attributeList, listEachArg, args);
    printf("%s\r\n", args_getStr(args, "stringOut"));
    return;
}

static void print(MimiObj *obj, Args *args)
{
    char *res = args_print(args, "val");
    if (NULL == res)
    {
        return;
    }
    if (0 == res[0])
    {
        return;
    }
    /* not empty */
    printf("%s\r\n", res);
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
