#include "shApp_argv.h"
#include "dataMemory.h"
#include "dataString.h"
#include "mimiShell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void *app_argv_main2(Shell2 *shell, int argc, char **argv)
{
	DMEM *memOut = DynMemGet(sizeof(char) * 256);
	char *strOut = (char *)(memOut->addr);
	char *arg;
	strOut[0] = 0;
	for (int i = 0; i < argc; i++)
	{
		arg = argv[i];
		char buff[256];
		sprintf(buff, "argv %d: ", i);
		strPrint(strOut, buff);
		strPrint(strOut, arg);
		strPrint(strOut, "\r\n");
	}
	return (void *)memOut;
}
