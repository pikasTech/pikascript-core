#include "dataArgsConst.h"
#include "dataArg.h"
#include "dataMemory.h"
#include "dataString.h"
#include "string.h"
#include <stdlib.h>
static void deinit(argsConst_t *self)
{
    DynMemPut(self->mem);

    for (int i = 0; i < ARGS_LIST_LENGTH; i++)
    {
        if (NULL != self->argList[i])
        {
            self->argList[i]->dinit(self->argList[i]);
        }
    }
}

static int getIndexByName(argsConst_t *self, char *name)
{
    for (int i = 0; i < ARGS_LIST_LENGTH; i++)
    {
        if (0 == strcmp(name, self->argList[i]->name))
        {
            return i;
        }
    }
    return -1;
}

static int getStrByIndex(argsConst_t *self, int index, char **strOut)
{
    int errCode = 0;
    if (index > ARGS_LIST_LENGTH)
    {
        errCode = 2;
        goto exit;
    }
    if (index + 1 > self->count)
    {
        errCode = 1;
        goto exit;
    }
    *strOut = self->argList[index]->contant;
    goto exit;

exit:
    return errCode;
}

static int pushStrWithDefaultName(argsConst_t *self, char *strIn)
{
    char name[ARG_NAME_LENGTH] = {0};
    sprintf(name, "arg%d", self->count);
    self->pushStrWithName(self, name, strIn);
    return 0;
}

static int pushFloatWithDefaultName(argsConst_t *self, float argInt)
{
    char strIn[ARG_CONTANT_LENGTH] = {0};
    sprintf(strIn, "%f", argInt);
    self->pushStrWithDefaultName(self, strIn);
    return 0;
}

static float getFloatByIndex(argsConst_t *self, int index)
{
    float val = 0;
    char *strOut = NULL;
    self->getStrByIndex(self, index, &strOut);
    val = atof(strOut);
    goto exit;
exit:
    return val;
}

static void *getPointerByIndex(argsConst_t *self, int index)
{
    void *pointer = NULL;
    char *strOut = NULL;
    self->getStrByIndex(self, index, &strOut);
    unsigned long int pointerTemp = 0;
    for (int i = 7; i > -1; i--)
    {
        pointerTemp = pointerTemp << 8;
        pointerTemp += ((unsigned char *)(strOut))[i];
    }
    pointer = (void *)pointerTemp;
    goto exit;
exit:
    return pointer;
}

static void *getPointerByName(argsConst_t *self, char *name)
{
    int index = 0;
    index = self->getIndexByName(self, name);
    return self->getPointerByIndex(self, index);
}

static int pushPointerWithName(argsConst_t *self, char *name, void *argPointer)
{
    unsigned char strIn[ARG_CONTANT_LENGTH] = {0};
    unsigned long int pointerTemp = (unsigned long int)argPointer;
    for (int i = 0; i < 8; i++)
    {
        strIn[i] = pointerTemp;
        pointerTemp = pointerTemp >> 8;
    }
    self->pushStrWithName(self, name, (char *)strIn);
    return 0;
}

static int pushStrWithName(argsConst_t *self, char *name, char *strIn)
{
    int errCode = 0;
    if (NULL == strIn)
    {
        errCode = 1;
        goto exit;
    }
    arg_t *argNew = New_arg(NULL);
    argNew->setName(argNew, name);
    argNew->setContant(argNew, strIn);
    self->argList[self->count] = argNew;
    self->count++;
    goto exit;

exit:
    return errCode;
}

static int getStrByName(argsConst_t *self, char *name, char **strOut)
{
    int index = self->getIndexByName(self, name);
    self->getStrByIndex(self, index, strOut);
    return 0;
}

int pushInt64WithName(argsConst_t *self, char *name, long long int64In)
{
    unsigned char strIn[ARG_CONTANT_LENGTH] = {0};
    unsigned long int int64Temp = int64In;
    for (int i = 0; i < 8; i++)
    {
        strIn[i] = int64Temp;
        int64Temp = int64Temp >> 8;
    }
    self->pushStrWithName(self, name, (char *)strIn);
    return 0;
}

static long long getInt64ByIndex(argsConst_t *self, int index)
{
    long long int64Out = 0;
    char *strOut = NULL;
    self->getStrByIndex(self, index, &strOut);
    unsigned long int int64Temp = 0;
    for (int i = 7; i > -1; i--)
    {
        int64Temp = int64Temp << 8;
        int64Temp |= (strOut)[i];
    }
    int64Out = int64Temp;
    goto exit;
exit:
    return int64Out;
}

static long long getInt64ByName(argsConst_t *self, char *name)
{
    int index = 0;
    index = self->getIndexByName(self, name);
    return self->getInt64ByIndex(self, index);
}

static void init(argsConst_t *self, argsConst_t *args)
{
    /* attrivute */
    self->context = self;
    self->count = 0;
    for (int i = 0; i < ARGS_LIST_LENGTH; i++)
    {
        self->argList[i] = NULL;
    }

    /* operation */
    self->dinit = deinit;

    self->getIndexByName = getIndexByName;

    self->pushStrWithName = pushStrWithName;
    self->pushStrWithDefaultName = pushStrWithDefaultName;
    self->getStrByIndex = getStrByIndex;
    self->getStrByName = getStrByName;

    self->pushFloatWithDefaultName = pushFloatWithDefaultName;
    self->getFloatByIndex = getFloatByIndex;

    self->pushPointerWithName = pushPointerWithName;
    self->getPointerByIndex = getPointerByIndex;
    self->getPointerByName = getPointerByName;

    self->pushInt64WithName = pushInt64WithName;
    self->getInt64ByName = getInt64ByName;
    self->getInt64ByIndex = getInt64ByIndex;

    /* object */

    /* override */
}

argsConst_t *New_argsConst(argsConst_t *args)
{
    DMEM *mem = DynMemGet(sizeof(argsConst_t));
    argsConst_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}