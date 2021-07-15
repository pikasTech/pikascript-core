#include "MimiObj.h"
#include "dataMemory.h"
#include "dataString.h"

static void set(MimiObj *obj, Args *args)
{
    // char buff[64] = {0};
    // char *argDir = args_getStr(args, "argDir");
    // char *argName = strGetLastToken(buff, argDir, '.');
    // Arg *valCopied = arg_copy(args_getArg(args, "val"));
    // arg_setName(valCopied, argName);
    // int err = args_setArg(obj->attributeList, valCopied);
    // if (err == 0)
    // {
    //     return;
    // }
    // if (err == 1)
    // {
    //     printf("[error] not get arg.\r\n");
    //     return;
    // }
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
    char *objDir = args_getStr(args, "objDir");
    if (NULL == objDir)
    {
        args_foreach(self->attributeList, listEachArg, args);
        printf("%s\r\n", args_getStr(args, "stringOut"));
        return;
    }
    MimiObj *obj = obj_getObj(self, objDir, 0);
    if (NULL == obj)
    {
        printf("[error] list: obj '%s' no found.\r\n", objDir);
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
    obj_defineMethod(self, "set(argDir:string, val)", set);
    obj_defineMethod(self, "ls(objDir:string)", list);

    /* object */

    /* override */
}

MimiObj *New_MimiObj_sys(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    init_sys(self, args);
    return self;
}