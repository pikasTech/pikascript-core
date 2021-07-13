// don't modify the mimiSH_core !!
#include "mimiShell.h"
#include "dataMemory.h"
#include "dataString.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// used to config the cmdMap of sh

// the only application the core need to load
#include "shApp_cmdNoFound.h"

// start_i is used to skip severl chars in the frount of CMD
static int strGetArgs_With_Start_i(char *CMD, char **argv, int start_i)
{
	int argc = 0;
	int i = 0;
	//arg_i point to the arg operated now
	int arg_i = 0;
	// if not found ' ', then put chars from CMD to argv_tem
	int char_i = 0;
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

int mimiShell2_strGetArgs(char *CMD, char **argv)
{
	strGetArgs_With_Start_i(CMD, argv, 0);
	return 0;
}

// the detector of shell luancher, which can add info befor the strout
static void *detector_shellLuancher(Shell *self,
									void *(*fun_d)(Shell *, char *, void *(fun)(Shell *, int, char **)),
									char *CMD,
									void *(fun)(Shell *, int argc, char **argv))
{
	DMEM *memOut;
	DMEM *memAdd;
	memAdd = DynMemGet(sizeof(char) * 256);
	char *strAdd = memAdd->addr;
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

// the luancher of shell
static void *shellLuancher(Shell *self,
						   char *CMD,
						   void *(fun)(Shell *, int argc, char **argv))
{
	char StartStrSize = 0;
	int argc = 0;
	DMEM *memOut;
	// sign in the argv memory
	char *argv[16] = {0};
	DMEM *mem[16] = {0};
	for (int i = 0; i < 16; i++)
	{
		mem[i] = DynMemGet(sizeof(char) * 256);
		argv[i] = (char *)mem[i]->addr;
		argv[i][0] = 0;
	}
	//get argc and argv from CMD, and skip the start string
	// argc = strGetArgs_With_Start_i(CMD, argv, strGetSize(StartStr));
	argc = strGetArgs_With_Start_i(CMD, argv, StartStrSize);

	//call the fun
	memOut = (DMEM *)fun(self, argc, argv);
	//free the argv memory
	for (int i = 0; i < 16; i++)
	{
		DynMemPut(mem[i]);
	}
	//get argc and argv
	return memOut;
}

static int luanchShellWhenNameMatch(Arg *argNow, Args *argsHandle)
{
	char *cmd = args_getStr(argsHandle,
							"cmd");
	Shell *shell = args_getPtr(argsHandle,
							   "shell");

	char *name = argNow->nameDynMem->addr;
	char arg0[32] = {0};
	getFirstToken(arg0, cmd, ' ');
	if (mimiStrEqu(arg0, name))
	{
		args_setPtr(argsHandle,
					"shellOut",
					shell->detector(shell,
									shellLuancher,
									cmd,
									arg_getPtr(argNow)));
		args_setStr(argsHandle,
					"succeed", "succeed");
	}
	return 0;
}

static void *Shell_cmd(Shell *self, char *cmd)
{
	Args *argsHandle = New_args(NULL);
	args_setStr(argsHandle,
				"cmd", cmd);
	args_setPtr(argsHandle,
				"shell", self);
	void *shellOut = NULL;
	args_foreach(self->mapList,
				 luanchShellWhenNameMatch, argsHandle);
	if (args_isArgExist(argsHandle,
						"succeed"))
	{
		// ok
		shellOut = args_getPtr(argsHandle,
							   "shellOut");
		goto exit;
	}

	// if the cmd is no found then call the app_cmdNoFoudn function
	shellOut = self->detector(self, shellLuancher, cmd, app_cmdNofound2);
	goto exit;

exit:
	args_deinit(argsHandle);
	return shellOut;
}

static void Shell_addMap(Shell *self,
						 char *name,
						 void *(*fun)(Shell *shell,
									  int argc,
									  char **argv))
{
	args_setPtr(self->mapList,
				name, fun);
}

static int Shell_listMap(Shell *self, int isShow)
{
	// cmdMap_t *cmdMap;
	int size;
	size = args_getSize(self->mapList);

	if (isShow)
	{
		printf("the size of cmdMap is :%d \r\n", size);
	}
	return size;
}

static int Shell_test(Shell *self, int isShow)
{
	char *outstr = 0;
	int size;
	if (4 != self->listMap(self, isShow))
	{
		return 1;
	}

	outstr = self->cmd(self, "argv fekfjk fiej kl j");
	size = strGetSize(outstr);
	if (isShow)
	{
		printf("%s", outstr);
		printf("the size of outstr is %d\r\n", size);
	}

	if (95 != size)
	{
		return 2;
	}

	return 0;
}

static void deinit(Shell *self)
{
	args_deinit(self->mapList);
	DynMemPut(self->mem);
}

static void _shConfig(Shell *self)
{
	/* override in user code */
	/* you can add maps */
}

static void init(Shell *self, Args *initArgs)
{
	/* attribute */

	/* operation */
	self->context = self;
	self->cmd = Shell_cmd;
	self->mapList = New_args(NULL);
	// set how to deinit the data of cmdMap Link
	self->addMap = Shell_addMap;
	self->listMap = Shell_listMap;
	self->test = Shell_test;
	self->deinit = deinit;
	self->detector = detector_shellLuancher;

	/* override */
	self->config = _shConfig;
	self->config(self);
}

Shell *New_shell(Args *args)
{
	DMEM *mem = DynMemGet(sizeof(Shell));
	Shell *self = mem->addr;
	self->mem = mem;
	self->init = init;
	self->init(self, args);
	return self;
}
