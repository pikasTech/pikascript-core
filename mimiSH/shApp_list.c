#include "dataMemory.h"
#include "dataString.h"
#include "MimiObj.h"
#include "shApp_argSet.h"
#include "mimiShell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define PROCESS_DIR argv[1]

static int listEachProcessArg(Arg *argEach, Args *handleArgs)
{
    if (NULL == handleArgs)
    {
        /* error: not handleArgs input */
        return 1;
    }
    char strBuff[256] = {0};
    char *stringOut = args_getStr(handleArgs, "stringOut");
    if (NULL == stringOut)
    {
        // stringOut no found
        return 1;
    }
    memcpy(strBuff, stringOut, 256);
    strAppend(strBuff, argEach->nameDynMem->addr);
    strAppend(strBuff, " ");
    args_setStr(handleArgs, "stringOut", strBuff);
    return 0;
}

void *app_list(Shell *shell, int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    ((char *)(memOut->addr))[0] = 0;
    MimiObj *root = shell->context;
    MimiObj *processNow = NULL;

    if (argc == 1)
    {
        processNow = root;
    }
    else
    {
        processNow = root->getObj(root, PROCESS_DIR, 0);
    }

    if (NULL == processNow)
    {
        strAppend(memOut->addr, "[error: process no found]\r\n");
        return (void *)memOut;
    }

    Args *handleArgs = New_args(NULL);
    args_setStr(handleArgs, "stringOut", "");
    Args *processArgs = processNow->attributeList;
    args_foreach (processArgs, listEachProcessArg, handleArgs);
    memcpy(memOut->addr, args_getStr(handleArgs, "stringOut"), 256);
    args_deinit(handleArgs);
    strAppend(memOut->addr, "\r\n");
    return (void *)memOut;
}
