#include "shDriver_sendTo.h"
#include "VM_memory.h"
#include "mimiSH_core.h"
#include "mimiStr.h"
#include "mimiVM_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void *sDriver_sendTo_main(int argc, char **argv)
{
	DMEM *memOut = DynMemGet(sizeof(char) * 256);
	char *strOut = (char *)(memOut->addr);
	strOut[0] = 0;
	char strToSend[256] = {0};
	char *com_name = argv[0];
	//collect the argvs tobe send
	for (int i = 1; i < argc; i++)
	{
		if (i > 1)
		{
			strPrint(strToSend, " ");
		}

		strPrint(strToSend, argv[i]);
	}

	strOut = VM_usart_send_by_comName(com_name, strToSend, strOut);
	return memOut;
}
