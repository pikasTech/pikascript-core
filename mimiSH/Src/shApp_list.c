#include "dataMemory.h"
#include "dataString.h"
#include "mimiProcess.h"
#include "shApp_argSet.h"
#include "mimiShell2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define PROCESS_DIR argv[1]

static int listEachHandleProcessArg(arg_t *argEach, args_t *handleArgs)
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

void *app_list(shell2_t *shell, int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    ((char *)(memOut->addr))[0] = 0;
    mimiProcess_t *root = shell->context;
    mimiProcess_t *processNow = NULL;

    if (argc == 1)
    {
        processNow = root;
    }
    else
    {
        processNow = goToProcess(root, PROCESS_DIR, 0);
    }

    if (NULL == processNow)
    {
        strPrint(memOut->addr, "[error: process no found]\r\n");
        return (void *)memOut;
    }

    args_t *handleArgs = New_args(NULL);
    handleArgs->setStr(handleArgs, "stringOut", "");
    args_t *processArgs = processNow->attributeList;
    processArgs->foreach (processArgs, listEachHandleProcessArg, handleArgs);
    memcpy(memOut->addr, handleArgs->getStr(handleArgs, "stringOut"), 256);
    handleArgs->dinit(handleArgs);
    strPrint(memOut->addr, "\r\n");
    return (void *)memOut;
}
