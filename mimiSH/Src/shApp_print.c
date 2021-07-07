#include "shApp_print.h"
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

void *app_print(Shell2 *shell, int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    ((char *)(memOut->addr))[0] = 0;
    MimiProcess *root = shell->context;
    MimiProcess *processNow = goToProcess(root, PROCESS_DIR, 1);

    if (NULL == processNow)
    {
        strPrint(memOut->addr, "[error: process no found]\r\n");
        return (void *)memOut;
    }
    char printName[32] = {0};
    getLastUnitBySign(PROCESS_DIR, printName, '.');

    char *printStr = NULL;
    printStr = processNow->argPrint(processNow, printName);
    memcpy(memOut->addr, printStr, 256);
    strPrint(memOut->addr, "\r\n");
    return (void *)memOut;
}
