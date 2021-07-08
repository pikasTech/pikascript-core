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
static void *detector_shellLuancher(Shell2 *self,
									void *(*fun_d)(Shell2 *, char *, void *(fun)(Shell2 *, int, char **)),
									char *CMD,
									void *(fun)(Shell2 *, int argc, char **argv))
{
	DMEM *memOut;
	DMEM *memAdd;
	memAdd = DynMemGet(sizeof(char) * 256);
	char *strAdd = memAdd->addr;
	strAdd[0] = 0;
	memOut = (DMEM *)fun_d(self, CMD, fun);
	if (NULL != memOut)
	{
		strPrint(strAdd, "myShell@STM32F405 >");
		strPrint(strAdd, CMD);
		strPrint(strAdd, "\r\n");
		strPrint(strAdd, (char *)(memOut->addr));
		DynMemPut(memOut);
	}
	return memAdd;
}

// the luancher of shell
static void *shellLuancher(Shell2 *self,
						   char *CMD,
						   void *(fun)(Shell2 *, int argc, char **argv))
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
	char *cmd = argsHandle->getStr(argsHandle, "cmd");
	Shell2 *shell = argsHandle->getPtr(argsHandle, "shell");

	char *name = argNow->nameDynMem->addr;
	if (isStartWith(cmd, name))
	{
		argsHandle->setPtr(argsHandle,
						   "shellOut",
						   shell->detector(shell,
										   shellLuancher,
										   cmd,
										   argNow->getPtr(argNow)));
		argsHandle->setStr(argsHandle,
						   "succeed", "succeed");
	}
	return 0;
}

static void *Shell_cmd(Shell2 *self, char *cmd)
{
	Args *argsHandle = New_args(NULL);
	argsHandle->setStr(argsHandle,
					   "cmd", cmd);
	argsHandle->setPtr(argsHandle,
					   "shell", self);
	void *shellOut = NULL;
	self->mapList->foreach (self->mapList,
							luanchShellWhenNameMatch, argsHandle);
	if (argsHandle->isArgExist(argsHandle,
							   "succeed"))
	{
		// ok
		shellOut = argsHandle->getPtr(argsHandle,
									  "shellOut");
		goto exit;
	}

	// if the cmd is no found then call the app_cmdNoFoudn function
	shellOut = self->detector(self, shellLuancher, cmd, app_cmdNofound2);
	goto exit;

exit:
	argsHandle->deinit(argsHandle);
	return shellOut;
}

static void Shell_addMap(Shell2 *self,
						 char *name,
						 void *(*fun)(Shell2 *shell,
									  int argc,
									  char **argv))
{
	self->mapList->setPtr(self->mapList,
						  name, fun);
}

static int Shell_listMap(Shell2 *self, int isShow)
{
	// cmdMap_t *cmdMap;
	int size;
	size = self->mapList->getSize(self->mapList);

	if (isShow)
	{
		printf("the size of cmdMap is :%d \r\n", size);
	}
	return size;
}

static int Shell_test(Shell2 *self, int isShow)
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

static void deinit(Shell2 *self)
{
	self->mapList->deinit(self->mapList);
	DynMemPut(self->mem);
}

static void _shConfig(Shell2 *self)
{
	/* override in user code */
	/* you can add maps */
}

static void init(Shell2 *self, Args *initArgs)
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

Shell2 *New_shell2(Args *args)
{
	DMEM *mem = DynMemGet(sizeof(Shell2));
	Shell2 *self = mem->addr;
	self->mem = mem;
	self->init = init;
	self->init(self, args);
	return self;
}