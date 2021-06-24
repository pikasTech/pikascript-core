#include "shApp_print.h"
#include "dataMemory.h"
#include "dataString.h"
#include "mimiProcess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define ROOT_PTR argv[argc - 1]
#define PROCESS_DIR argv[1]
#define PRINT_NAME argv[2]

void *app_print(int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    ((char *)(memOut->addr))[0] = 0;
    mimiProcess_t *root = (mimiProcess_t *)atoi(ROOT_PTR);
    mimiProcess_t *processNow = root;
    char *printStr = NULL;

    // sign in the argv memory
    char *processArgv[16] = {0};
    DMEM *processMem[16] = {0};
    for (int i = 0; i < 16; i++)
    {
        processMem[i] = DynMemGet(sizeof(char) * 256);
        processArgv[i] = (char *)processMem[i]->addr;
        processArgv[i][0] = 0;
    }
    int processArgc = devideStringBySign(PROCESS_DIR, processArgv, '.');
    for (int i = 0; i < processArgc; i++)
    {
        processNow = processNow->getPtr(processNow, processArgv[i]);
        if (processNow == NULL)
        {
            strPrint(memOut->addr, "[error: process no found]");
            goto exit;
        }
    }
		printStr = processNow->argPrint(processNow, PRINT_NAME);		
    memcpy(memOut->addr, printStr, ARG_CONTANT_LENGTH);
    goto exit;

exit:
    for (int i = 0; i < 16; i++)
    {
        DynMemPut(processMem[i]);
    }
    strPrint(memOut->addr, "\r\n");
    return (void *)memOut;
}
