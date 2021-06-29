#include "dataMemory.h"
#include "dataString.h"
#include "mimiProcess.h"
#include "shApp_argSet.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define ROOT_PTR argv[argc - 1]
#define PROCESS_DIR argv[1]

static int listEachHandleProcessArg(arg_t *argEach, args_t *handleArgs)
{
    if (NULL == handleArgs)
    {
        /* error: not handleArgs input */
        return 1;
    }
    char strBuff[ARG_CONTANT_LENGTH] = {0};
    memcpy(strBuff, handleArgs->getStr(handleArgs, "stringOut"), ARG_CONTANT_LENGTH);
    strPrint(strBuff, argEach->name);
    strPrint(strBuff, " ");
    handleArgs->setStr(handleArgs, "stringOut", strBuff);
}

void *app_list(int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    ((char *)(memOut->addr))[0] = 0;
    mimiProcess_t *root = (mimiProcess_t *)atoi(ROOT_PTR);
    mimiProcess_t *processNow = NULL;

    if (argc == 2)
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
    memcpy(memOut->addr, handleArgs->getStr(handleArgs, "stringOut"), ARG_CONTANT_LENGTH);
    handleArgs->dinit(handleArgs);
    strPrint(memOut->addr, "\r\n");
    return (void *)memOut;
}
