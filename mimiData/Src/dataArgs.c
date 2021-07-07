#include "dataArgs.h"
#include "dataLink.h"
#include "dataMemory.h"
#include "dataString.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

static void deinit(Args *self)
{
    DynMemPut(self->mem);
    self->argLinkList->deinit(self->argLinkList);
}

static void loadDefaultName(Args *self)
{
    sprintf((char *)self->nameBuff, "arg%d", (int)self->argLinkList->TopId);
}

static char *getStrByIndex(Args *self, int index)
{
    Arg *arg = self->getArgByIndex(self, index);
    if (NULL == arg)
    {
        return NULL;
    }
    return arg->contantDynMem->addr;
}

static int setStrWithDefaultName(Args *self, char *strIn)
{
    loadDefaultName(self);
    self->setStr(self, (char *)self->nameBuff, strIn);
    return 0;
}

static int setFloatWithDefaultName(Args *self, float argFloat)
{
    loadDefaultName(self);
    self->setFloat(self, (char *)self->nameBuff, argFloat);
    return 0;
}

static int setFloatWithName(Args *self, char *name, float argFloat)
{
    Arg *argNew = New_arg(NULL);
    argNew->setType(argNew, "float");
    argNew->setName(argNew, name);
    argNew->setFloat(argNew, argFloat);
    self->setArg(self, argNew);
    return 0;
}

static float getFloatByIndex(Args *self, int index)
{
    float val = 0;
    Arg *arg = self->getArgByIndex(self, index);
    val = arg->getFloat(arg);
    return val;
}

static void *getPointerByIndex(Args *self, int index)
{
    void *pointer = NULL;
    Arg *arg = self->getArgByIndex(self, index);
    pointer = arg->getPointer(arg);
    return pointer;
}

static void *getPointerByName(Args *self, char *name)
{
    void *pointer = NULL;
    Arg *arg = self->getArgByName(self, name);
    if (NULL == arg)
    {
        return NULL;
    }

    pointer = arg->getPointer(arg);
    return pointer;
}

static int setPointerWithName(Args *self, char *name, void *argPointer)
{
    int errCode = 0;
    Arg *argNew = New_arg(NULL);
    argNew->setType(argNew, "pointer");
    argNew->setName(argNew, name);
    argNew->setPointer(argNew, argPointer);
    self->setArg(self, argNew);
    return errCode;
}

static int setStrWithName(Args *self, char *name, char *strIn)
{
    int errCode = 0;
    Arg *argNew = New_arg(NULL);
    argNew->setType(argNew, "string");
    argNew->setString(argNew, strIn);
    argNew->setName(argNew, name);
    self->setArg(self, argNew);
    return errCode;
}

static char *getStrByName(Args *self, char *name)
{
    Arg *arg = self->getArgByName(self, name);
    if(NULL == arg->contantDynMem)
    {
        return NULL;
    }
    return arg->contantDynMem->addr;
}

static int setInt64WithName(Args *self, char *name, long long int64In)
{
    Arg *argNew = New_arg(NULL);
    argNew->setName(argNew, name);
    argNew->setInt64(argNew, int64In);
    argNew->setType(argNew, "int");
    self->setArg(self, argNew);
    return 0;
}

static long long getInt64ByIndex(Args *self, int index)
{
    Arg *arg = self->getArgByIndex(self, index);
    return arg->getInt64(arg);
}

static long long getInt64ByName(Args *self, char *name)
{
    Arg *arg = self->getArgByName(self, name);
    return arg->getInt64(arg);
}

static int size(Args *self)
{
    return self->argLinkList->size(self->argLinkList);
}

char *getTypeByName(Args *self, char *name)
{
    Arg *arg = NULL;
    arg = self->getArgByName(self, name);
    if (NULL == arg)
    {
        return "[error: arg no found]";
    }
    return arg->typeDynMem->addr;
}

static Arg *getArgByIndex(Args *self, int index)
{
    Arg *arg;
    if (index == -1)
    {
        return NULL;
    }
    LinkNode *node = self->argLinkList->findNodeById(self->argLinkList,index);
    if (NULL == node)
    {
        return NULL;
    }
    arg = node->contant;
    return arg;
}

static float getFloatByName(Args *self, char *name)
{
    Arg *arg = self->getArgByName(self, name);
    return arg->getFloat(arg);
}

static int copyArg(Args *self, char *name, Args *directArgs)
{
    Arg *argToBeCopy = self->getArgByName(self, name);
    if (NULL == argToBeCopy)
    {
        return 1;
    }
    Arg *argCopied = New_arg(NULL);
    argCopied->setContant(argCopied, argToBeCopy->contantDynMem->addr, argToBeCopy->contantDynMem->size);
    argCopied->setName(argCopied, argToBeCopy->nameDynMem->addr);
    argCopied->setType(argCopied, argToBeCopy->typeDynMem->addr);

    directArgs->setArg(directArgs, argCopied);

    return 0;
}

static int isArgExist(Args *self, char *name)
{
    if (NULL != self->getArgByName(self, name))
    {
        return 1;
    }
    return 0;
}

static int updateArg(Args *self, Arg *argNew)
{
    // arg New must be a new arg
    Arg *argOld = self->getArgByName(self, argNew->nameDynMem->addr);

    if (0 != strcmp(argOld->typeDynMem->addr, argNew->typeDynMem->addr))
    {
        return 1;
        // type do not match
    }
    argOld->setContant(argOld, argNew->contantDynMem->addr, argNew->contantDynMem->size);
    argNew->deinit(argNew);
    return 0;
}

static int setArg(Args *self, Arg *arg)
{
   
    if (!self->isArgExist(self, arg->nameDynMem->addr))
    {
        self->argLinkList->add(self->argLinkList,
                               arg,
                               (void (*)(void *))arg->deinit);
        return 0;
    }
    updateArg(self, arg);
    return 0;
}

static Arg *getArgByName(Args *self, char *name)
{
    LinkNode *nodeNow = self->argLinkList->firstNode;
    if (NULL == nodeNow)
    {
        return NULL;
    }
    while(1)
    {
        Arg *arg = nodeNow->contant;
        if(0 == strcmp(name, arg->nameDynMem->addr))
        {
            return arg;
        }
        if(NULL == nodeNow->nextNode)
        {
            return NULL;
        }
        nodeNow = nodeNow->nextNode;
    }
}

static void bind(Args *self, char *type, char *name, void *pointer)
{
    char typeWithBind[32] = "_bind-";
    strPrint(typeWithBind, type);
    Arg *argNew = New_arg(NULL);
    argNew->setType(argNew, typeWithBind);
    argNew->setName(argNew, name);
    argNew->setPointer(argNew, pointer);
    self->setArg(self, argNew);
    return;
}

static void bindInt(Args *self, char *name, int *intPtr)
{
    self->bind(self, "int", name, intPtr);
}

static void bindFloat(Args *self, char *name, float *floatPtr)
{
    self->bind(self, "float", name, floatPtr);
}

static void bindStr(Args *self, char *name, char **stringPtr)
{
    self->bind(self, "string", name, stringPtr);
}

static char *getPrintSring(Args *self, char *name, char *valString)
{
    char printName[32] = {0};
    strPrint(printName, ".print-");
    strPrint(printName, name);
    char printString[256] = {0};
    sprintf(printString, "%s", valString);
    self->setStr(self, printName, printString);
    return self->getStr(self, printName);
}

static char *getPrintStringFromInt(Args *self, char *name, int val)
{
    char valString[256] = {0};
    sprintf(valString, "%d", val);
    return getPrintSring(self, name, valString);
}

static char *getPrintStringFromFloat(Args *self, char *name, float val)
{
    char valString[256] = {0};
    sprintf(valString, "%f", val);
    return getPrintSring(self, name, valString);
}

static char *getPrintStringFromPtr(Args *self, char *name, void *val)
{
    char valString[256] = {0};
    long long intVal = (long long)val;
    sprintf(valString, "0x%x", val);
    return getPrintSring(self, name, valString);
}

static char *print(Args *self, char *name)
{
    char *type = self->getTypeByName(self, name);

    if (mimiStrEqu(type, "int"))
    {
        int val = self->getInt(self, name);
        return getPrintStringFromInt(self, name, val);
    }

    if (mimiStrEqu(type, "float"))
    {
        float val = self->getFloat(self, name);
        return getPrintStringFromFloat(self, name, val);
    }

    if (mimiStrEqu(type, "string"))
    {
        return getStrByName(self, name);
    }

    if (mimiStrEqu(type, "pointer"))
    {
        void *val = self->getPtr(self, name);
        return getPrintStringFromPtr(self, name, val);
    }

    char bindTypePrefix[] = "_bind-";
    if (isStartWith(type, bindTypePrefix))
    {
        char typeWithoutBind[32] = {0};
        mimiStrRemovePrefix(type, bindTypePrefix, typeWithoutBind);
        if (mimiStrEqu(typeWithoutBind, "int"))
        {
            int *valPtr = self->getPtr(self, name);
            int val = *valPtr;
            return getPrintStringFromInt(self, name, val);
        }
        if (mimiStrEqu(typeWithoutBind, "float"))
        {
            float *valPtr = self->getPtr(self, name);
            float val = *valPtr;
            return getPrintStringFromFloat(self, name, val);
        }
        if (mimiStrEqu(typeWithoutBind, "string"))
        {
            // the value of &string is equal to string it self
            char *string = self->getPtr(self, name);
            return string;
        }
    }
    return "[error: arg no found]";
}

static int set(Args *self, char *name, char *valStr)
{
    char *type = self->getTypeByName(self, name);

    if (mimiStrEqu("[error: arg no found]", type))
    {
        return 1;
    }

    if (mimiStrEqu("int", type))
    {
        int val = atoi(valStr);
        self->setInt(self, name, val);
        // operation succeed
        return 0;
    }
    if (mimiStrEqu("float", type))
    {
        float val = atof(valStr);
        self->setFloat(self, name, val);
        // operation succeed
        return 0;
    }
    if (mimiStrEqu("string", type))
    {
        self->setStr(self, name, valStr);
        // operation succeed
        return 0;
    }

    char bindTypePrefix[] = "_bind-";
    if (isStartWith(type, bindTypePrefix))
    {
        char typeWithoutBind[32] = {0};
        mimiStrRemovePrefix(type, bindTypePrefix, typeWithoutBind);
        if (mimiStrEqu(typeWithoutBind, "int"))
        {
            int *valPtr = self->getPtr(self, name);
            int val = atoi(valStr);
            *valPtr = val;
            // operation succeed
            return 0;
        }
        if (mimiStrEqu(typeWithoutBind, "float"))
        {
            float *valPtr = self->getPtr(self, name);
            float val = atof(valStr);
            *valPtr = val;
            // operation succeed
            return 0;
        }
        if (mimiStrEqu(typeWithoutBind, "string"))
        {
            char *stringBinded = self->getPtr(self, name);
            /* size add 1 to copy the '\0' */
            memcpy(stringBinded, valStr, strGetSize(valStr) + 1);
            // operation succeed
            return 0;
        }
    }
    // unknown error
    return 2;
}

static int setObject(Args *self, char *objectName, char *className, void *objectPtr)
{
    char typeWithClass[32] = "_class-";
    strPrint(typeWithClass, className);
    Arg *argNew = New_arg(NULL);
    argNew->setName(argNew, objectName);
    argNew->setPointer(argNew, objectPtr);
    argNew->setType(argNew, typeWithClass);
    self->setArg(self, argNew);
    return 0;
}

static int foreach(Args *self, int (*eachHandle)(Arg *argEach, Args *handleArgs), Args *handleArgs)
{
    int argsSize = self->size(self);
    for (int i = 0; i < argsSize; i++)
    {
        Arg *argNow = self->getArgByIndex(self, i);
        if (NULL == argNow)
        {
            continue;
        }
        eachHandle(argNow, handleArgs);
    }
    return 0;
}

static void init(Args *self, Args *args)
{
    /* attrivute */
    self->context = self;
    self->argLinkList = New_link(NULL);
    for (int i = 0; i < 32; i++)
    {
        self->nameBuff[i] = 0;
    }

    /* operation */
    self->deinit = deinit;

    self->size = size;
    self->getArgByIndex = getArgByIndex;
    self->getArgByName = getArgByName;
    self->setArg = setArg;
    self->getTypeByName = getTypeByName;
    self->copyArg = copyArg;
    self->isArgExist = isArgExist;

    self->setInt = setInt64WithName;
    self->getInt = getInt64ByName;
    self->getInt64ByIndex = getInt64ByIndex;

    self->setFloat = setFloatWithName;
    self->getFloat = getFloatByName;
    self->setFloatWithDefaultName = setFloatWithDefaultName;
    self->getFloatByIndex = getFloatByIndex;

    self->setPtr = setPointerWithName;
    self->getPtr = getPointerByName;
    self->getPointerByIndex = getPointerByIndex;

    self->setStr = setStrWithName;
    self->getStr = getStrByName;
    self->setStrWithDefaultName = setStrWithDefaultName;
    self->getStrByIndex = getStrByIndex;

    /* arg general operations */
    self->bind = bind;
    self->bindInt = bindInt;
    self->bindFloat = bindFloat;
    self->bindStr = bindStr;
    self->print = print;
    self->set = set;
    self->setObject = setObject;
    self->foreach = foreach;

    /* arg */

    /* object */

    /* override */
}

Args *New_args(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(Args));
    Args *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
