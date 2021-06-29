#include "shApp_argSet.h"
#include "dataMemory.h"
#include "dataString.h"
#include "mimiProcess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

mimiProcess_t *goToProcess(mimiProcess_t *root, char *processDirectory, int deepth)
{
    mimiProcess_t *processNow = root;
    // sign in the argv memory
    char *directoryUnit[16] = {0};
    DMEM *processMem[16] = {0};
    for (int i = 0; i < 16; i++)
    {
        processMem[i] = DynMemGet(sizeof(char) * 256);
        directoryUnit[i] = (char *)processMem[i]->addr;
        directoryUnit[i][0] = 0;
    }
    int processArgc = devideStringBySign(processDirectory, directoryUnit, '.');
    for (int i = 0; i < processArgc - deepth; i++)
    {
        char * type = processNow->attributeList->getTypeByName(processNow->attributeList, directoryUnit[i]);
        if (!mimiStrEqu(type, "_class-process"))
        {
            processNow = NULL;
            goto exit;
        }

        processNow = processNow->getPtr(processNow, directoryUnit[i]);

        if (processNow == NULL)
        {
            goto exit;
        }
    }
    goto exit;
exit:
    for (int i = 0; i < 16; i++)
    {
        DynMemPut(processMem[i]);
    }
    return processNow;
}

#define ROOT_PTR argv[argc - 1]
#define PROCESS_DIR argv[1]
#define SET_VAL argv[2]

void *app_argSet(int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    ((char *)(memOut->addr))[0] = 0;
    mimiProcess_t *root = (mimiProcess_t *)atoi(ROOT_PTR);
    mimiProcess_t *processNow = goToProcess(root, PROCESS_DIR, 1);

    if (NULL == processNow)
    {
        strPrint(memOut->addr, "[error: process no found]\r\n");
        return (void *)memOut;
    }

    char setName[32] = {0};
    getLastUnitBySign(PROCESS_DIR, setName, '.');
    int errCode = processNow->argSet(processNow, setName, SET_VAL);
    if (0 != errCode)
    {
        if (1 == errCode)
        {
            strPrint(memOut->addr, "[error: set faild, arg no found]\r\n");
            return (void *)memOut;
        }
        char printbuff[256] = {0};
        sprintf(printbuff, "[error: set faild with error code: %d]\r\n", errCode);
        strPrint(memOut->addr, printbuff);
        return (void *)memOut;
    }

    strPrint(memOut->addr, "[ok]\r\n");
    return (void *)memOut;
}
