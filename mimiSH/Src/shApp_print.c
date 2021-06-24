#include "shApp_print.h"
#include "dataMemory.h"
#include "dataString.h"
#include "mimiProcess.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>

void *app_print(int argc, char **argv)
{
	DMEM *memOut = DynMemGet(sizeof(char) * 256);
    mimiProcess_t *root = (mimiProcess_t *)atoi(argv[argc - 1]);
    for (int i = 1; i <= argc - 2; i++)
    {
    }
	return (void *)memOut;
}
