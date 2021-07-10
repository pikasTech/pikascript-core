#include "shApp_argv.h"
#include "dataMemory.h"
#include "dataString.h"
#include "mimiShell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void *app_argv_main2(Shell *shell, int argc, char **argv)
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
		strAppend(strOut, buff);
		strAppend(strOut, arg);
		strAppend(strOut, "\r\n");
	}
	return (void *)memOut;
}
