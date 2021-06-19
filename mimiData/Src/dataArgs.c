#include "dataArgs.h"
#include "dataLink.h"
#include "dataMemory.h"
#include <stdlib.h>

static void deinit(args_t *self)
{
    DynMemPut(self->mem);
    self->argLinkList->dinit(self->argLinkList);
}

static void loadDefaultName(args_t *self)
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

static int getIndexByName(args_t *self, char *name)
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

static int getStrByIndex(args_t *self, int index, char **strOut)
{
    int errCode = 0;
    arg_t *arg = self->getArgByIndex(self, index);
    *strOut = arg->contant;
    return errCode;
}

static int setStrWithDefaultName(args_t *self, char *strIn)
{
    loadDefaultName(self);
    self->setStrWithName(self, (char *)self->nameBuff, strIn);
    return 0;
}

static int setFloatWithDefaultName(args_t *self, float argFloat)
{
    loadDefaultName(self);
    self->setFloatWithName(self, (char *)self->nameBuff, argFloat);
    return 0;
}

static int setFloatWithName(args_t *self, char *name, float argFloat)
{
    arg_t *argNew = New_arg(NULL);
    argNew->setType(argNew, "float");
    argNew->setName(argNew, name);
    argNew->setFloat(argNew, argFloat);
    self->setArg(self, argNew);
    return 0;
}

static float getFloatByIndex(args_t *self, int index)
{
    float val = 0;
    arg_t *arg = self->getArgByIndex(self, index);
    val = arg->getFloat(arg);
    return val;
}

static void *getPointerByIndex(args_t *self, int index)
{
    void *pointer = NULL;
    arg_t *arg = self->getArgByIndex(self, index);
    pointer = arg->getPointer(arg);
    return pointer;
}

static void *getPointerByName(args_t *self, char *name)
{
    void *pointer = NULL;
    arg_t *arg = self->getArgByName(self, name);
    pointer = arg->getPointer(arg);
    return pointer;
}

static int setPointerWithName(args_t *self, char *name, void *argPointer)
{
    int errCode = 0;
    arg_t *argNew = New_arg(NULL);
    argNew->setType(argNew, "pointer");
    argNew->setName(argNew, name);
    argNew->setPointer(argNew, argPointer);
    self->setArg(self, argNew);
    return errCode;
}

static int setStrWithName(args_t *self, char *name, char *strIn)
{
    int errCode = 0;
    arg_t *argNew = New_arg(NULL);
    argNew->setType(argNew, "string");
    argNew->setString(argNew, strIn);
    argNew->setName(argNew, name);
    self->setArg(self, argNew);
    return errCode;
}

static int getStrByName(args_t *self, char *name, char **strOut)
{
    arg_t *arg = self->getArgByName(self, name);
    *strOut = arg->contant;
    return 0;
}

static int setInt64WithName(args_t *self, char *name, long long int64In)
{
    arg_t *argNew = New_arg(NULL);
    argNew->setName(argNew, name);
    argNew->setInt64(argNew, int64In);
    argNew->setType(argNew, "int64");
    self->setArg(self, argNew);
    return 0;
}

static long long getInt64ByIndex(args_t *self, int index)
{
    arg_t *arg = self->getArgByIndex(self, index);
    return arg->getInt64(arg);
}

static long long getInt64ByName(args_t *self, char *name)
{
    arg_t *arg = self->getArgByName(self, name);
    return arg->getInt64(arg);
}

static int size(args_t *self)
{
    return self->argLinkList->size(self->argLinkList);
}

char *getTypeByName(args_t *self, char *name)
{
    arg_t *arg = self->getArgByName(self, name);
    return arg->type;
}

static arg_t *getArgByIndex(args_t *self, int index)
{
    arg_t *arg;
    if (index == -1)
    {
        return NULL;
    }
    arg = self->argLinkList->findNodeById(self->argLinkList, index)->contant;
    return arg;
}

static float getFloatByName(args_t *self, char *name)
{
    arg_t *arg = self->getArgByName(self, name);
    return arg->getFloat(arg);
}

static int copyArg(args_t *self, char *name, args_t *directArgs)
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

    directArgs->setArg(directArgs, argCopied);

		return 0;
}

static int isArgExist(args_t *self, char *name)
{
    if (-1 != self->getIndexByName(self, name))
    {
        return 1;
    }
    return 0;
}

static int updateArg(args_t *self, arg_t *argNew)
{
    // arg New must be a new arg
    arg_t *argOld = self->getArgByName(self, argNew->name);

    if (0 != strcmp(argOld->type, argNew->type))
    {
        return 1;
        // type do not match
    }

    memcpy(argOld->contant, argNew->contant, ARG_CONTANT_LENGTH);
    argNew->dinit(argNew);
    return 0;
}

static int setArg(args_t *self, arg_t *arg)
{
    // input arg of setArg must be a new created arg
    if (!self->isArgExist(self, arg->name))
    {
        self->argLinkList->add(self->argLinkList,
                               arg,
                               (void (*)(void *))arg->dinit);
        return 0;
    }
    updateArg(self, arg);
    return 0;
}

static arg_t *getArgByName(args_t *self, char *name)
{
    int index = self->getIndexByName(self, name);
    if (-1 == index)
    {
        return NULL;
    }
    return self->getArgByIndex(self, index);
}

static void init(args_t *self, args_t *args)
{
    /* attrivute */
    self->context = self;
    self->argLinkList = New_link(NULL);
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
    self->setArg = setArg;
    self->getTypeByName = getTypeByName;
    self->copyArg = copyArg;
    self->isArgExist = isArgExist;

    self->setInt64WithName = setInt64WithName;
    self->getInt64ByName = getInt64ByName;
    self->getInt64ByIndex = getInt64ByIndex;

    self->setFloatWithName = setFloatWithName;
    self->setFloatWithDefaultName = setFloatWithDefaultName;
    self->getFloatByIndex = getFloatByIndex;
    self->getFloatByName = getFloatByName;

    self->setPointerWithName = setPointerWithName;
    self->getPointerByIndex = getPointerByIndex;
    self->getPointerByName = getPointerByName;

    self->setStrWithName = setStrWithName;
    self->setStrWithDefaultName = setStrWithDefaultName;
    self->getStrByIndex = getStrByIndex;
    self->getStrByName = getStrByName;

    /* object */

    /* override */
}

args_t *New_args(args_t *args)
{
    DMEM *mem = DynMemGet(sizeof(args_t));
    args_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
