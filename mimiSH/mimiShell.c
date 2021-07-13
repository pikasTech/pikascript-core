// don't modify the mimiSH_core !!
#include "mimiShell.h"
#include "dataMemory.h"
#include "dataString.h"
#include "MimiObj.h"
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

int strGetArgs(char *CMD, char **argv)
{
	strGetArgs_With_Start_i(CMD, argv, 0);
	return 0;
}

// the detector of shell luancher, which can add info befor the strout
static void *_detector_shellLuancher(Shell *self,
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
static void *_runShellCmd(Shell *self,
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

// the luancher of python
static void *_runPythonCmd(Shell *self,
						   char *CMD)
{
	MimiObj *root = self->context;
	char buff[8][256] = {0};
	int i = 0;
	char *cmdBuff = buff[i++];
	memcpy(cmdBuff, CMD, strGetSize(CMD));
	popToken(buff[i++], cmdBuff, ' ');
	obj_run(root, cmdBuff);

	DMEM *memOut = DynMemGet(1);
	char *strOut = memOut->addr;
	strOut[0] = 0;
	return memOut;
}

static int luanchShellWhenNameMatch(Arg *argHandle, Args *argsHandle)
{
	char *cmd = args_getStr(argsHandle,
							"cmd");
	Shell *shell = args_getPtr(argsHandle,
							   "shell");

	char *name = argHandle->nameDynMem->addr;
	char arg0[32] = {0};
	getFirstToken(arg0, cmd, ' ');
	void *_runCmd = NULL;
	if (mimiStrEqu(arg0, name))
	{
		_runCmd = _runShellCmd;
	}

	if (NULL == _runCmd)
	{
		/* not match */
		return 1;
	}

	args_setPtr(argsHandle,
				"shellOut",
				shell->_detector(shell,
								 _runCmd,
								 cmd,
								 arg_getPtr(argHandle)));
	args_setStr(argsHandle,
				"succeed", "succeed");
	return 0;
}

void *shell_cmd(Shell *self, char *cmd)
{
	Args *argsHandle = New_args(NULL);
	args_setStr(argsHandle,
				"cmd", cmd);
	args_setPtr(argsHandle,
				"shell", self);
	void *shellOut = NULL;
	char arg0[32] = {0};

	getFirstToken(arg0, cmd, ' ');
	/* match py cmd */
	if (mimiStrEqu(arg0, "py"))
	{
		shellOut = _runPythonCmd(self, cmd);
		goto exit;
	}

	args_foreach(self->mapList, luanchShellWhenNameMatch, argsHandle);
	if (args_isArgExist(argsHandle, "succeed"))
	{
		// ok
		shellOut = args_getPtr(argsHandle, "shellOut");
		goto exit;
	}

	// if the cmd is no found then call the app_cmdNoFoudn function
	shellOut = self->_detector(self, _runShellCmd, cmd, app_cmdNofound);
	goto exit;

exit:
	args_deinit(argsHandle);
	return shellOut;
}

void shell_addMap(Shell *self,
				  char *name,
				  void *(*fun)(Shell *shell,
							   int argc,
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
	Shell *self = mem->addr;
	self->mem = mem;
	shell_init(self, args);
	return self;
}
