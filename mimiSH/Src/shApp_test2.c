#include "shApp_test2.h"
#include "dataMemory.h"
#include "dataString.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
char *shell_base_test2(char *CMD, unsigned short int StartStrSize, char *strOut)
{
	int CMDSize = strGetSize(CMD);
	char argv[256] = {0};

	strPrint(strOut, "test is running...\r\n");
	strPrint(strOut, "intput: ");
	strPrint(strOut, CMD);
	strPrint(strOut, "\r\n");
	for (int i = StartStrSize; i < CMDSize; i++)
	{
		argv[i - StartStrSize] = CMD[i];
	}

	for (int i = 0; i < CMDSize; i++)
	{
		if (argv[i] == 't')
		{
			char string[256];
			sprintf(string, "the i = %d, \r\n", i);
			strPrint(strOut, string);
		}
	}
	if (!strcmp(argv, "open"))
	{
		strPrint(strOut, "opend!\r\n");
	}
	return strPrint(strOut, "Finished\r\n");
}

void *app_test2(int argc, char **argv)
{
	DMEM *memOut = DynMemGet(sizeof(char) * 256);
	char *strOut = (char *)(memOut->addr);
	strOut[0] = 0;
	char *CMD = argv[1];
	// unsigned short int StartStrSize = argv[1][0];
	unsigned short int StartStrSize = 0;
	shell_base_test2(CMD, StartStrSize, strOut);
	return memOut;
}
