#include "MimiObj.h"
#include "dataMemory.h"
#include "dataString.h"

static void setBind(MimiObj *obj, Args *args)
{
    char *argPath = args_getStr(args, "argPath");
    char *valStr = args_print(args, "val");
    obj_set(obj, argPath, valStr);
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
        args_foreach(self->attributeList, listEachArg, args);
        printf("%s\r\n", args_getStr(args, "stringOut"));
        return;
    }
    MimiObj *obj = obj_getObj(self, objPath, 0);
    if (NULL == obj)
    {
        printf("[error] list: obj '%s' no found.\r\n", objPath);
        return;
    }
    args_foreach(obj->attributeList, listEachArg, args);
    printf("%s\r\n", args_getStr(args, "stringOut"));
    return;
}

static void print(MimiObj *obj, Args *args)
{
    char *res = args_print(args, "arg");
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

    /* operation */
    obj_defineMethod(self, "print(arg)", print);
    obj_defineMethod(self, "set(argPath:string, val)", setBind);
    obj_defineMethod(self, "ls(objPath:string)", list);

    /* object */

    /* override */
}

MimiObj *New_MimiObj_sys(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    init_sys(self, args);
    return self;
}