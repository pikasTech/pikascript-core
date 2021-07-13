#include "shApp_print.h"
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

void *app_print(Shell *shell, int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    ((char *)(memOut->addr))[0] = 0;
    MimiObj *root = shell->context;
    MimiObj *processNow = obj_getObj(root, PROCESS_DIR, 1);

    if (NULL == processNow)
    {
        strAppend(memOut->addr, "[error: process no found]\r\n");
        return (void *)memOut;
    }
    char argName[32] = {0};
    strGetLastToken(argName, PROCESS_DIR, '.');

    char *printStr = NULL;
    printStr = obj_print(processNow, argName);
    memcpy(memOut->addr, printStr, 256);
    strAppend(memOut->addr, "\r\n");

    char printName[32] = {0};
    strAppend(printName, "[printBuff]");
    strAppend(printName, argName);
    args_removeArg(processNow->attributeList,
                                         printName);
    return (void *)memOut;
}
