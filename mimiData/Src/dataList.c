#include "dataList.h"
#include "dataLink.h"
#include "dataMemory.h"
#include <stdlib.h>

static void deinit(list_t *self)
{
    DynMemPut(self->mem);
    self->argLinkList->dinit(self->argLinkList);
}

static void loadDefaultName(list_t *self)
{
    sprintf((char *)self->nameBuff, "arg%d", (int)self->argLinkList->TopId);
}

static void *getIndexWhenNameMatch(linkNode_t *node, argsConst_t *args)
{
    arg_t *arg = node->contant;
    char *argName = NULL;
    args->getStrByName(args, "argName", &argName);
    if (0 == strcmp(arg->name, argName))
    {
        return &(node->id);
    }
    return NULL;
}

static int getIndexByName(list_t *self, char *name)
{
    argsConst_t *args = New_argsConst(NULL);
    args->pushStrWithName(args,
                          "argName",
                          name);
    long long id = -1;
    long long *pId = NULL;
    pId = self->argLinkList->tranverse(self->argLinkList,
                                       getIndexWhenNameMatch,
                                       args);
    if (NULL != pId)
    {
        id = *pId;
        goto exit;
    }
    goto exit;

exit:
    args->dinit(args);
    return id;
}

static int getStrByIndex(list_t *self, int index, char **strOut)
{
    int errCode = 0;
    arg_t *arg = self->getArgByIndex(self, index);
    *strOut = arg->contant;
    return errCode;
}

static int pushStrWithDefaultName(list_t *self, char *strIn)
{
    loadDefaultName(self);
    self->pushStrWithName(self, (char *)self->nameBuff, strIn);
    return 0;
}

static int pushFloatWithDefaultName(list_t *self, float argFloat)
{
    loadDefaultName(self);
    self->pushFloatWithName(self, (char *)self->nameBuff, argFloat);
    return 0;
}

static int pushFloatWithName(list_t *self, char *name, float argFloat)
{
    arg_t *argNew = New_arg(NULL);
    argNew->setType(argNew, "float");
    argNew->setName(argNew, name);
    argNew->setFloat(argNew, argFloat);
    self->pushArg(self, argNew);
    return 0;
}

static float getFloatByIndex(list_t *self, int index)
{
    float val = 0;
    arg_t *arg = self->getArgByIndex(self, index);
    val = arg->getFloat(arg);
    return val;
}

static void *getPointerByIndex(list_t *self, int index)
{
    void *pointer = NULL;
    arg_t *arg = self->getArgByIndex(self, index);
    pointer = arg->getPointer(arg);
    return pointer;
}

static void *getPointerByName(list_t *self, char *name)
{
    void *pointer = NULL;
    arg_t *arg = self->getArgByName(self, name);
    pointer = arg->getPointer(arg);
    return pointer;
}

static int pushPointerWithName(list_t *self, char *name, void *argPointer)
{
    int errCode = 0;
    arg_t *argNew = New_arg(NULL);
    argNew->setType(argNew, "pointer");
    argNew->setName(argNew, name);
    argNew->setPointer(argNew, argPointer);
    self->pushArg(self, argNew);
    return errCode;
}

static int pushStrWithName(list_t *self, char *name, char *strIn)
{
    int errCode = 0;
    arg_t *argNew = New_arg(NULL);
    argNew->setType(argNew, "string");
    argNew->setString(argNew, strIn);
    argNew->setName(argNew, name);
    self->pushArg(self, argNew);
    return errCode;
}

static int getStrByName(list_t *self, char *name, char **strOut)
{
    arg_t *arg = self->getArgByName(self, name);
    *strOut = arg->contant;
    return 0;
}

static int pushInt64WithName(list_t *self, char *name, long long int64In)
{
    arg_t *argNew = New_arg(NULL);
    argNew->setName(argNew, name);
    argNew->setInt64(argNew, int64In);
    argNew->setType(argNew, "int64");
    self->pushArg(self, argNew);
    return 0;
}

static long long getInt64ByIndex(list_t *self, int index)
{
    arg_t *arg = self->getArgByIndex(self, index);
    return arg->getInt64(arg);
}

static long long getInt64ByName(list_t *self, char *name)
{
    arg_t *arg = self->getArgByName(self, name);
    return arg->getInt64(arg);
}

static int size(list_t *self)
{
    return self->argLinkList->size(self->argLinkList);
}

char *getTypeByName(list_t *self, char *name)
{
    arg_t *arg = self->getArgByName(self, name);
    return arg->type;
}

static arg_t *getArgByIndex(list_t *self, int index)
{
    arg_t *arg;
    if (index == -1)
    {
        return NULL;
    }
    arg = self->argLinkList->findNodeById(self->argLinkList, index)->contant;
    return arg;
}

static float getFloatByName(list_t *self, char *name)
{
    arg_t *arg = self->getArgByName(self, name);
    return arg->getFloat(arg);
}

static int copyArg(list_t *self, char *name, list_t *directList)
{
    arg_t *argToBeCopy = self->getArgByName(self, name);
    if (NULL == argToBeCopy)
    {
        return 1;
    }
    arg_t *argCopied = New_arg(NULL);
    memcpy(argCopied->contant, argToBeCopy->contant, ARG_CONTANT_LENGTH);
    memcpy(argCopied->name, argToBeCopy->name, ARG_NAME_LENGTH);
    memcpy(argCopied->type, argToBeCopy->type, ARG_TYPE_LENGTH);

    directList->argLinkList->add(directList->argLinkList,
                                 argCopied,
                                 (void *)argCopied->dinit);
    return 0;
}

static int isArgExist(list_t *self, char *name)
{
    if (-1 != self->getIndexByName(self, name))
    {
        return 1;
    }
    return 0;
}

static int pushArg(list_t *self, arg_t *arg)
{
    self->argLinkList->add(self->argLinkList,
                           arg,
                           (void *)arg->dinit);
    return 0;
}

static arg_t *getArgByName(list_t *self, char *name)
{
    int index = self->getIndexByName(self, name);
    if (-1 == index)
    {
        return NULL;
    }
    return self->getArgByIndex(self, index);
}

static void init(list_t *self, list_t *args)
{
    /* attrivute */
    self->context = self;
    self->argLinkList = New_link(NULL);
    for (int i = 0; i < ARG_CONTANT_LENGTH; i++)
    {
        self->contantBuff[i] = 0;
    }
    for (int i = 0; i < ARG_NAME_LENGTH; i++)
    {
        self->nameBuff[i] = 0;
    }

    /* operation */
    self->dinit = deinit;

    self->size = size;
    self->getIndexByName = getIndexByName;
    self->getArgByIndex = getArgByIndex;
    self->getArgByName = getArgByName;
    self->pushArg = pushArg;
    self->getTypeByName = getTypeByName;
    self->copyArg = copyArg;
    self->isArgExist = isArgExist;

    self->pushInt64WithName = pushInt64WithName;
    self->getInt64ByName = getInt64ByName;
    self->getInt64ByIndex = getInt64ByIndex;

    self->pushFloatWithName = pushFloatWithName;
    self->pushFloatWithDefaultName = pushFloatWithDefaultName;
    self->getFloatByIndex = getFloatByIndex;
    self->getFloatByName = getFloatByName;

    self->pushPointerWithName = pushPointerWithName;
    self->getPointerByIndex = getPointerByIndex;
    self->getPointerByName = getPointerByName;

    self->pushStrWithName = pushStrWithName;
    self->pushStrWithDefaultName = pushStrWithDefaultName;
    self->getStrByIndex = getStrByIndex;
    self->getStrByName = getStrByName;

    /* object */

    /* override */
}

list_t *New_list(list_t *args)
{
    DMEM *mem = DynMemGet(sizeof(list_t));
    list_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
