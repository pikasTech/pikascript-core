#include "dataMemory.h"
#include "dataString.h"
#include "MimiProcess.h"
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
    char *stringOut = handleArgs->getStr(handleArgs, "stringOut");
    if (NULL == stringOut)
    {
        // stringOut no found
        return 1;
    }
    memcpy(strBuff, stringOut, 256);
    strPrint(strBuff, argEach->nameDynMem->addr);
    strPrint(strBuff, " ");
    handleArgs->setStr(handleArgs, "stringOut", strBuff);
    return 0;
}

void *app_list(Shell *shell, int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    ((char *)(memOut->addr))[0] = 0;
    MimiProcess *root = shell->context;
    MimiProcess *processNow = NULL;

    if (argc == 1)
    {
        processNow = root;
    }
    else
    {
        processNow = root->goToProcess(root, PROCESS_DIR, 0);
    }

    if (NULL == processNow)
    {
        strPrint(memOut->addr, "[error: process no found]\r\n");
        return (void *)memOut;
    }

    Args *handleArgs = New_args(NULL);
    handleArgs->setStr(handleArgs, "stringOut", "");
    Args *processArgs = processNow->attributeList;
    processArgs->foreach (processArgs, listEachProcessArg, handleArgs);
    memcpy(memOut->addr, handleArgs->getStr(handleArgs, "stringOut"), 256);
    handleArgs->deinit(handleArgs);
    strPrint(memOut->addr, "\r\n");
    return (void *)memOut;
}
