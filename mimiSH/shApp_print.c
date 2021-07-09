#include "shApp_print.h"
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

void *app_print(Shell *shell, int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    ((char *)(memOut->addr))[0] = 0;
    MimiProcess *root = shell->context;
    MimiProcess *processNow = root->goToProcess(root, PROCESS_DIR, 1);

    if (NULL == processNow)
    {
        strPrint(memOut->addr, "[error: process no found]\r\n");
        return (void *)memOut;
    }
    char argName[32] = {0};
    getLastTokenBySign(PROCESS_DIR, argName, '.');

    char *printStr = NULL;
    printStr = processNow->argPrint(processNow, argName);
    memcpy(memOut->addr, printStr, 256);
    strPrint(memOut->addr, "\r\n");

    char printName[32] = {0};
    strPrint(printName, "[printBuff]");
    strPrint(printName, argName);
    processNow->attributeList->removeArg(processNow->attributeList,
                                         printName);
    return (void *)memOut;
}
