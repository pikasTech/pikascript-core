
#include "shApp_cmdNoFound.h"
#include "dataMemory.h"
#include "dataString.h"
#include <stdlib.h>
void *app_cmdNofound(Shell *self, int argc, char **argv)
{
    DMEM *memOut = DynMemGet(sizeof(char) * 256);
    char *strOut = (char *)(memOut->addr);
    strOut[0] = 0;
    // if (0 == argv[0][0])
    // {
    // 	return (void *)memOut;
    // }
    strAppend(strOut, "[error] The cmd <");
    strAppend(strOut, argv[0]);
    strAppend(strOut, "> no found!\r\n");
    return (void *)memOut;
}
