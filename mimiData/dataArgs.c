#include "dataArgs.h"
#include "dataLink.h"
#include "dataMemory.h"
#include "dataString.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void args_deinit(Args *self)
{
    DynMemPut(self->mem);
    link_deinit(self->argLinkList);
}

char *getDefaultName(Args *self, char *strOut)
{
    sprintf((char *)strOut, "arg%d", (int)self->argLinkList->TopId);
    return strOut;
}

char *args_getStrByIndex(Args *self, int index)
{
    Arg *arg = args_getArgByIndex(self, index);
    if (NULL == arg)
    {
        return NULL;
    }
    return arg->contantDynMem->addr;
}

int args_setStrWithDefaultName(Args *self, char *strIn)
{
    char buff[128];
    args_setStr(self, getDefaultName(self, buff), strIn);
    return 0;
}

int args_setFloatWithDefaultName(Args *self, float argFloat)
{
    char buff[128];
    args_setFloat(self, getDefaultName(self, buff), argFloat);
    return 0;
}

int args_setFloat(Args *self, char *name, float argFloat)
{
    Arg *argNew = New_arg(NULL);
    arg_setType(argNew, "float");
    arg_setName(argNew, name);
    arg_setFloat(argNew, argFloat);
    args_setArg(self, argNew);
    return 0;
}

float args_getFloatByIndex(Args *self, int index)
{
    float val = 0;
    Arg *arg = args_getArgByIndex(self, index);
    val = arg_getFloat(arg);
    return val;
}

void *args_getPointerByIndex(Args *self, int index)
{
    void *pointer = NULL;
    Arg *arg = args_getArgByIndex(self, index);
    pointer = arg_getPtr(arg);
    return pointer;
}

void *args_getPtr(Args *self, char *name)
{
    void *pointer = NULL;
    Arg *arg = args_getArg(self, name);
    if (NULL == arg)
    {
        return NULL;
    }

    pointer = arg_getPtr(arg);
    return pointer;
}

int args_setPtr(Args *self, char *name, void *argPointer)
{
    int errCode = 0;
    Arg *argNew = New_arg(NULL);
    arg_setType(argNew, "pointer");
    arg_setName(argNew, name);
    arg_setPtr(argNew, argPointer);
    args_setArg(self, argNew);
    return errCode;
}

int args_setStr(Args *self, char *name, char *strIn)
{
    int errCode = 0;
    Arg *argNew = New_arg(NULL);
    arg_setType(argNew, "string");
    arg_setStr(argNew, strIn);
    arg_setName(argNew, name);
    args_setArg(self, argNew);
    return errCode;
}

char *args_getStr(Args *self, char *name)
{
    Arg *arg = args_getArg(self, name);
    if (NULL == arg)
    {
        return NULL;
    }
    if (NULL == arg->contantDynMem)
    {
        return NULL;
    }
    return arg->contantDynMem->addr;
}

int args_setInt(Args *self, char *name, long long int64In)
{
    Arg *argNew = New_arg(NULL);
    arg_setName(argNew, name);
    arg_setInt(argNew, int64In);
    arg_setType(argNew, "int");
    args_setArg(self, argNew);
    return 0;
}

long long args_getIntByIndex(Args *self, int index)
{
    Arg *arg = args_getArgByIndex(self, index);
    if (NULL == arg)
    {
        return -999999999;
    }
    return arg_getInt(arg);
}

long long args_getInt(Args *self, char *name)
{
    Arg *arg = args_getArg(self, name);
    if (NULL == arg)
    {
        return -999999999;
    }
    return arg_getInt(arg);
}

int args_getSize(Args *self)
{
    return link_getSize(self->argLinkList);
}

char *args_getType(Args *self, char *name)
{
    Arg *arg = NULL;
    arg = args_getArg(self, name);
    if (NULL == arg)
    {
        return NULL;
    }
    return arg->typeDynMem->addr;
}

Arg *args_getArgByIndex(Args *self, int index)
{
    Arg *arg;
    if (index == -1)
    {
        return NULL;
    }
    LinkNode *node = link_getNode(self->argLinkList, index);
    if (NULL == node)
    {
        return NULL;
    }
    arg = node->contant;
    return arg;
}

float args_getFloat(Args *self, char *name)
{
    Arg *arg = args_getArg(self, name);
    return arg_getFloat(arg);
}

int args_copyArg(Args *self, Arg *argToBeCopy)
{
    if (NULL == argToBeCopy)
    {
        return 1;
    }
    Arg *argCopied = arg_copy(argToBeCopy);
    args_setArg(self, argCopied);

    return 0;
}

int args_copyArgByName(Args *self, char *name, Args *directArgs)
{
    Arg *argToBeCopy = args_getArg(self, name);
    args_copyArg(directArgs, argToBeCopy);
    return 0;
}

int args_isArgExist(Args *self, char *name)
{
    if (NULL != args_getArg(self, name))
    {
        return 1;
    }
    return 0;
}

int updateArg(Args *self, Arg *argNew)
{
    // arg New must be a new arg
    Arg *argOld = args_getArg(self, argNew->nameDynMem->addr);

    if (0 != strcmp(argOld->typeDynMem->addr, argNew->typeDynMem->addr))
    {
        return 1;
        // type do not match
    }
    arg_setContant(argOld, argNew->contantDynMem->addr, argNew->contantDynMem->size);
    arg_deinit(argNew);
    return 0;
}

int args_setArg(Args *self, Arg *arg)
{
    if (!args_isArgExist(self, arg->nameDynMem->addr))
    {
        link_addNode(self->argLinkList,
                     arg,
                     (void (*)(void *))arg_deinit);
        return 0;
    }
    updateArg(self, arg);
    return 0;
}

Arg *args_getArg(Args *self, char *name)
{
    LinkNode *nodeNow = self->argLinkList->firstNode;
    if (NULL == nodeNow)
    {
        return NULL;
    }
    while (1)
    {
        Arg *arg = nodeNow->contant;
        if (strEqu(name, arg->nameDynMem->addr))
        {
            return arg;
        }
        if (NULL == nodeNow->nextNode)
        {
            return NULL;
        }
        nodeNow = nodeNow->nextNode;
    }
}

void args_bind(Args *self, char *type, char *name, void *pointer)
{
    char typeWithBind[32] = "_bind-";
    strAppend(typeWithBind, type);
    Arg *argNew = New_arg(NULL);
    arg_setType(argNew, typeWithBind);
    arg_setName(argNew, name);
    arg_setPtr(argNew, pointer);
    args_setArg(self, argNew);
    return;
}

void args_bindInt(Args *self, char *name, int *intPtr)
{
    args_bind(self, "int", name, intPtr);
}

void args_bindFloat(Args *self, char *name, float *floatPtr)
{
    args_bind(self, "float", name, floatPtr);
}

void args_bindStr(Args *self, char *name, char **stringPtr)
{
    args_bind(self, "string", name, stringPtr);
}

char *getPrintSring(Args *self, char *name, char *valString)
{
    char printName[32] = {0};
    strAppend(printName, "[printBuff]");
    strAppend(printName, name);
    char printString[256] = {0};
    sprintf(printString, "%s", valString);
    args_setStr(self, printName, printString);
    return args_getStr(self, printName);
}

char *getPrintStringFromInt(Args *self, char *name, int val)
{
    char valString[256] = {0};
    sprintf(valString, "%d", val);
    return getPrintSring(self, name, valString);
}

char *getPrintStringFromFloat(Args *self, char *name, float val)
{
    char valString[256] = {0};
    sprintf(valString, "%f", val);
    return getPrintSring(self, name, valString);
}

char *getPrintStringFromPtr(Args *self, char *name, void *val)
{
    char valString[256] = {0};
    unsigned int intVal = (unsigned int)val;
    sprintf(valString, "0x%x", intVal);
    return getPrintSring(self, name, valString);
}

char *args_print(Args *self, char *name)
{
    char *type = args_getType(self, name);
    if (NULL == type)
    {
        return NULL;
    }

    if (strEqu(type, "int"))
    {
        int val = args_getInt(self, name);
        return getPrintStringFromInt(self, name, val);
    }

    if (strEqu(type, "float"))
    {
        float val = args_getFloat(self, name);
        return getPrintStringFromFloat(self, name, val);
    }

    if (strEqu(type, "string"))
    {
        return args_getStr(self, name);
    }

    if (strEqu(type, "pointer"))
    {
        void *val = args_getPtr(self, name);
        return getPrintStringFromPtr(self, name, val);
    }

    char bindTypePrefix[] = "_bind-";
    if (strIsStartWith(type, bindTypePrefix))
    {
        char typeWithoutBind[32] = {0};
        strRemovePrefix(type, bindTypePrefix, typeWithoutBind);
        if (strEqu(typeWithoutBind, "int"))
        {
            int *valPtr = args_getPtr(self, name);
            int val = *valPtr;
            return getPrintStringFromInt(self, name, val);
        }
        if (strEqu(typeWithoutBind, "float"))
        {
            float *valPtr = args_getPtr(self, name);
            float val = *valPtr;
            return getPrintStringFromFloat(self, name, val);
        }
        if (strEqu(typeWithoutBind, "string"))
        {
            // the value of &string is equal to string it self
            char *string = args_getPtr(self, name);
            return string;
        }
    }
    return NULL;
}

int args_set(Args *self, char *name, char *valStr)
{
    char *type = args_getType(self, name);

    if (NULL == type)
    {
        /* do not get arg */
        return 1;
    }

    if (strEqu("int", type))
    {
        int val = atoi(valStr);
        args_setInt(self, name, val);
        // operation succeed
        return 0;
    }
    if (strEqu("float", type))
    {
        float val = atof(valStr);
        args_setFloat(self, name, val);
        // operation succeed
        return 0;
    }
    if (strEqu("string", type))
    {
        args_setStr(self, name, valStr);
        // operation succeed
        return 0;
    }

    char bindTypePrefix[] = "_bind-";
    if (strIsStartWith(type, bindTypePrefix))
    {
        char typeWithoutBind[32] = {0};
        strRemovePrefix(type, bindTypePrefix, typeWithoutBind);
        if (strEqu(typeWithoutBind, "int"))
        {
            int *valPtr = args_getPtr(self, name);
            int val = atoi(valStr);
            *valPtr = val;
            // operation succeed
            return 0;
        }
        if (strEqu(typeWithoutBind, "float"))
        {
            float *valPtr = args_getPtr(self, name);
            float val = atof(valStr);
            *valPtr = val;
            // operation succeed
            return 0;
        }
        if (strEqu(typeWithoutBind, "string"))
        {
            char *stringBinded = args_getPtr(self, name);
            /* size add 1 to copy the '\0' */
            memcpy(stringBinded, valStr, strGetSize(valStr) + 1);
            // operation succeed
            return 0;
        }
    }
    // unknown error
    return 2;
}

int args_setPtrWithType(Args *self, char *objectName, char *className, void *objectPtr)
{
    char typeWithClass[32] = "_class-";
    strAppend(typeWithClass, className);
    Arg *argNew = New_arg(NULL);
    arg_setName(argNew, objectName);
    arg_setPtr(argNew, objectPtr);
    arg_setType(argNew, typeWithClass);
    args_setArg(self, argNew);
    return 0;
}

int args_foreach(Args *self, int (*eachHandle)(Arg *argEach, Args *handleArgs), Args *handleArgs)
{
    LinkNode *nodeNow = self->argLinkList->firstNode;
    while (1)
    {
        Arg *argNow = nodeNow->contant;
        if (NULL == argNow)
        {
            continue;
        }
        eachHandle(argNow, handleArgs);

        if (NULL == nodeNow->nextNode)
        {
            break;
        }
        nodeNow = nodeNow->nextNode;
    }
    return 0;
}

void args_removeArg(Args *self, char *name)
{
    Arg *argNow = args_getArg(self, name);
    link_removeNode(self->argLinkList, argNow);
}

void args_returnStr(Args *args, char *val)
{
    args_setStr(args, "return", val);
}

void args_returnInt(Args *args, int val)
{
    args_setInt(args, "return", val);
}

void args_returnFloat(Args *args, float val)
{
    args_setFloat(args, "return", val);
}

void args_returnPtr(Args *args, void *val)
{
    args_setPtr(args, "return", val);
}

void args_init(Args *self, Args *args)
{
    /* attrivute */
    self->context = self;
    self->argLinkList = New_link(NULL);

    /* arg */

    /* object */

    /* override */
}

Args *New_args(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(Args));
    Args *self = (void *)(mem->addr);
    self->mem = mem;
    args_init(self, args);
    return self;
}
