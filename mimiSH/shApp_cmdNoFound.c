
#include "shApp_cmdNoFound.h"
#include "dataMemory.h"
#include "dataString.h"
#include <stdlib.h>
void *app_cmdNofound2(Shell *self, int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    char *strOut = (char *)(memOut->addr);
    strOut[0] = 0;
    // if (0 == argv[0][0])
    // {
    // 	return (void *)memOut;
    // }
    strPrint(strOut, "[error] The cmd <");
    strPrint(strOut, argv[0]);
    strPrint(strOut, "> no found!\r\n");
    return (void *)memOut;
}
