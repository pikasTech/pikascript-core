#include "shApp_argSet.h"
#include "dataMemory.h"
#include "dataString.h"
#include "MimiProcess.h"
#include <stdio.h>
#include "mimiShell.h"
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

#define ROOT_PTR argv[argc - 1]
#define PROCESS_DIR argv[1]
#define SET_VAL argv[2]

void *app_argSet(Shell *shell, int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    ((char *)(memOut->addr))[0] = 0;
    MimiProcess *root = shell->context;

    int errCode = root->argSet(root, PROCESS_DIR, SET_VAL);
    if (0 != errCode)
    {
        if (1 == errCode)
        {
            strPrint(memOut->addr, "[error]: set faild, arg no found!\r\n");
            return (void *)memOut;
        }
        if (2 == errCode)
        {
            strPrint(memOut->addr, "[error]: set faild, type invalid!\r\n");
            return (void *)memOut;
        }				
        char printbuff[256] = {0};
        sprintf(printbuff, "[error]: set faild with error code: %d!\r\n", errCode);
        strPrint(memOut->addr, printbuff);
        return (void *)memOut;
    }

    strPrint(memOut->addr, "[ok]\r\n");
    return (void *)memOut;
}
