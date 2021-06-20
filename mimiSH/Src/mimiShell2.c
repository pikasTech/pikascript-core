// don't modify the mimiSH_core !!
#include "mimiShell2.h"
#include "dataMemory.h"
#include "dataLinkWithNode.h"
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
static void *detector_shellLuancher(void *(*fun_d)(char *, char *, void *(fun)(int, char **)), char *CMD, char *StartStr, void *(fun)(int argc, char **argv))
{
	DMEM *memOut;
	DMEM *memAdd;
	memAdd = DynMemGet(sizeof(char) * 256);
	char *strAdd = memAdd->addr;
	strAdd[0] = 0;
	memOut = (DMEM *)fun_d(CMD, StartStr, fun);
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
static void *shellLuancher(char *CMD, char *StartStr, void *(fun)(int argc, char **argv))
{
	char StartStrSize = 0;
	int argc = 0;
	DMEM *memOut;
	StartStrSize = strGetSize(StartStr);
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
	memOut = (DMEM *)fun(argc, argv);
	//free the argv memory
	for (int i = 0; i < 16; i++)
	{
		DynMemPut(mem[i]);
	}
	//get argc and argv
	return memOut;
}

static void *Shell_cmd(shell2_t *self, char *cmd)
{
	char *name;
	mimiShell2_cmdMap_t *cmdMap;
	linkWithNode_t *nodeNow = self->cmdMapHead->next;
	while (NULL != nodeNow)
	{
		cmdMap = (mimiShell2_cmdMap_t *)nodeNow->data;
		name = cmdMap->cmdName;
		if (isStartWith(cmd, name))
		{
			return self->detector(shellLuancher, cmd, name, cmdMap->cmdCallBack);
		}
		nodeNow = nodeNow->next;
	}
	// if the cmd is no found then call the app_cmdNoFoudn function
	return self->detector(shellLuancher, cmd, "", app_cmdNofound);
}

static void Shell_addMap(shell2_t *self, char *name, void *(*fun)(int argc, char **argv))
{
	DMEM *mem;
	mem = DynMemGet(sizeof(mimiShell2_cmdMap_t));
	mimiShell2_cmdMap_t *cmdMap = (mimiShell2_cmdMap_t *)(mem->addr);
	cmdMap->mem = mem;
	cmdMap->cmdName = name;
	cmdMap->cmdCallBack = fun;
	self->cmdMapHead->add(self->cmdMapHead, cmdMap);
}

static int Shell_listMap(shell2_t *self, int isShow)
{
	// cmdMap_t *cmdMap;
	int size;
	size = self->cmdMapHead->size(self->cmdMapHead);

	if (isShow)
	{
		printf("the size of cmdMap is :%d \r\n", size);
	}
	return size;
}

static int Shell_test(shell2_t *self, int isShow)
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

static void dinit(shell2_t *self)
{
	self->cmdMapHead->deinit(self->cmdMapHead);
	DynMemPut(self->mem);
}

static void deinit_cmdMap_data(void *data_noType)
{
	mimiShell2_cmdMap_t *cmdMap = data_noType;
	DynMemPut(cmdMap->mem);
	cmdMap->mem = NULL;
}

static void sh_config(shell2_t *self)
{
}

shell2_t *new_mimiShell2(args_t *initArgs)
{
	/* attribute */
	DMEM *mem = DynMemGet(sizeof(shell2_t));
	shell2_t *sh = (shell2_t *)(mem->addr);
	sh->mem = mem;

	/* operation */
	sh->cmd = Shell_cmd;
	sh->cmdMapHead = New_linkWithNode(NULL);
	// set how to deinit the data of cmdMap Link
	sh->cmdMapHead->port_deinit_data = deinit_cmdMap_data;
	sh->addMap = Shell_addMap;
	sh->listMap = Shell_listMap;
	sh->test = Shell_test;
	sh->config = sh_config;
	sh->dinit = dinit;
	sh->detector = detector_shellLuancher;

	/* override */
	sh->config(sh);
	return sh;
}
