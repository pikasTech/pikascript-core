// don't modify the mimiSH_core !!
#include "mimiShell.h"
#include "dataMemory.h"
#include "dataString.h"
#include "MimiObj.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// used to config the cmdMap of sh

// start_i is used to skip severl chars in the frount of CMD
static int32_t strGetArgs_With_Start_i(char *CMD, char **argv, int32_t start_i)
{
	int32_t argc = 0;
	int32_t i = 0;
	//arg_i point32_t to the arg operated now
	int32_t arg_i = 0;
	// if not found ' ', then put chars from CMD to argv_tem
	int32_t char_i = 0;
	for (i = start_i; (i < strGetSize(CMD)); i++)
	{
		if (CMD[i] != ' ')
		{
			argv[arg_i][char_i] = CMD[i];
			char_i++;
		}
		if (CMD[i] == ' ')
		{
			// write '\0' to the end of argv
			argv[arg_i][char_i] = 0;
			arg_i++;
			char_i = 0;
		}
		// write '\0' to the end of last argv
		argv[arg_i][char_i] = 0;
	}
	argc = arg_i + 1;
	return argc;
}

int32_t strGetArgs(char *CMD, char **argv)
{
	strGetArgs_With_Start_i(CMD, argv, 0);
	return 0;
}

// the detector of shell luancher, which can add info befor the strout
static void *_detector_shellLuancher(Shell *self,
									 void *(*fun_d)(Shell *, char *, void *(fun)(Shell *, int, char **)),
									 char *CMD,
									 void *(fun)(Shell *, int32_t argc, char **argv))
{
	DMEM *memOut;
	DMEM *memAdd;
	memAdd = DynMemGet(sizeof(char) * 256);
	char *strAdd = (char *)memAdd->addr;
	strAdd[0] = 0;
	memOut = (DMEM *)fun_d(self, CMD, fun);
	if (NULL != memOut)
	{
		strAppend(strAdd, "myShell@STM32F405 >");
		strAppend(strAdd, CMD);
		strAppend(strAdd, "\r\n");
		strAppend(strAdd, (char *)(memOut->addr));
		DynMemPut(memOut);
	}
	return memAdd;
}

// the luancher of python
static Arg *_runPythonCmd(Shell *self, char *cmd)
{
	MimiObj *root = self->context;
	Arg *str = New_arg(NULL);
	Args *res = obj_runDirect(root, cmd);

	char *sysOut = args_getStr(res, "sysOut");
	if (NULL != sysOut)
	{
		arg_setStr(str, sysOut);
		goto exit;
	}
exit:
	if (NULL != res)
	{
		args_deinit(res);
	}
	return str;
}

Arg *shell_cmd(Shell *self, char *cmd)
{
	return _runPythonCmd(self, cmd);
}

void shell_addMap(Shell *self,
				  char *name,
				  void *(*fun)(Shell *shell,
							   int32_t argc,
							   char **argv))
{
	args_setPtr(self->mapList,
				name, fun);
}

void shell_deinit(Shell *self)
{
	args_deinit(self->mapList);
	DynMemPut(self->mem);
}

static void _shellConfig(Shell *self)
{
	/* override in user code */
	/* you can add maps */
}

void shell_init(Shell *self, Args *initArgs)
{
	/* attribute */
	self->context = self;
	self->mapList = New_args(NULL);

	/* override */
	self->_config = _shellConfig;
	self->_detector = _detector_shellLuancher;
	self->_config(self);
}

Shell *New_shell(Args *args)
{
	DMEM *mem = DynMemGet(sizeof(Shell));
	Shell *self = (void *)(mem->addr);
	self->mem = mem;
	shell_init(self, args);
	return self;
}
