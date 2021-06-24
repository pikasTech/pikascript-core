#include "shApp_print.h"
#include "dataMemory.h"
#include "dataString.h"
#include "mimiProcess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define ROOT_PTR argc - 1
#define PROCESS_RANGE_START 1
#define PROCESS_RANGE_END argc - 3
#define PRINT_NAME argc - 2

void *app_print(int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    mimiProcess_t *root = (mimiProcess_t *)atoi(argv[ROOT_PTR]);
    mimiProcess_t *processNow = root;
    for (int i = PROCESS_RANGE_START; i <= PROCESS_RANGE_END; i++)
    {
        processNow = processNow->getPtr(processNow, argv[i]);
    }
    char *printStr = processNow->argPrint(processNow, argv[PRINT_NAME]);
    memcpy(memOut->addr, printStr, ARG_CONTANT_LENGTH);
    strPrint(memOut->addr, "\r\n");
    return (void *)memOut;
}
