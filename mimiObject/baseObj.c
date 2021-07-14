#include "MimiObj.h"
#include "baseObj.h"
#include "dataMemory.h"
#include "dataString.h"

static void print(MimiObj *obj, Args *args)
{
    char *argName = args_getStr(args, "argName");
    char *res = args_print(obj->attributeList, argName);
    if (0 != res[0])
    {
        /* not empty */
        printf("%s\r\n", res);
    }
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

static void list(MimiObj *obj, Args *args)
{
    args_setStr(args, "stringOut", "");
    args_foreach(obj->attributeList, listEachArg, args);
    printf("%s\r\n", args_getStr(args, "stringOut"));
    return;
}

static void set(MimiObj *obj, Args *args)
{
    char *argName = args_getStr(args, "argName");
    char *argVal = args_getStr(args, "argVal");
    args_set(obj->attributeList, argName, argVal);
}

static void init_baseObj(MimiObj *self, Args *args)
{
    /* attrivute */

    /* operation */
    obj_defineMethod(self, "print(argName:string)", print);
    obj_defineMethod(self, "ls()", list);
    obj_defineMethod(self, "set(argName:string, argVal:string)", set);

    /* object */

    /* override */
}

MimiObj *New_baseObj(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    init_baseObj(self, args);
    return self;
}