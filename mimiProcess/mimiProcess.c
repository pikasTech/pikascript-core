#include "mimiProcess.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"

static int dinitEachSubprocess(Arg *argEach, Args *handleArgs)
{
    if (NULL != handleArgs)
    {
        /* error: tOhis handle not need handle args */
        return 1;
    }
    if (mimiStrEqu(argEach->typeDynMem->addr, "_class-process"))
    {
        MimiProcess *subProcess = argEach->getPointer(argEach);
        subProcess->deinit(subProcess);
    }
    return 0;
}

static void dinitAllSubProcess(MimiProcess *self)
{
    Args *args = self->attributeList;
    args->foreach (args, dinitEachSubprocess, NULL);
}

static void deinit(MimiProcess *self)
{
    self->_beforDinit(self);
    dinitAllSubProcess(self);
    self->attributeList->deinit(self->attributeList);
    DynMemPut(self->mem);
}

static void update(MimiProcess *self)
{
    // return if is not enable
    if (0 == self->getInt(self, "isEnable"))
    {
        return;
    }
    self->_updateHandle(self);
}
static void _processRootUpdateHandle(MimiProcess *self)
{
    // override the handle function here
}
static void enable(MimiProcess *self)
{
    self->setInt(self, "isEnable", 1);
}

static void disable(MimiProcess *self)
{
    self->setInt(self, "isEnable", 0);
}

static void setInt64(MimiProcess *self, char *name, long long val)
{
    self->attributeList->setInt(self->attributeList, name, val);
}

static void setPointer(MimiProcess *self, char *name, void *pointer)
{
    self->attributeList->setPtr(self->attributeList, name, pointer);
}

static void setFloat(MimiProcess *self, char *name, float value)
{
    self->attributeList->setFloat(self->attributeList, name, value);
}

static void setStr(MimiProcess *self, char *name, char *str)
{
    self->attributeList->setStr(self->attributeList, name, str);
}

static long long getInt64(MimiProcess *self, char *name)
{
    return self->attributeList->getInt(self->attributeList, name);
}

static void *getPointer(MimiProcess *self, char *name)
{
    return self->attributeList->getPtr(self->attributeList, name);
}

static float getFloat(MimiProcess *self, char *name)
{
    return self->attributeList->getFloat(self->attributeList, name);
}

char *getStr(MimiProcess *self, char *name)
{
    return self->attributeList->getStr(self->attributeList, name);
}

static void loadAttributeFromArgs(MimiProcess *self, Args *args, char *name)
{
    args->copyArg(args, name, self->attributeList);
}

static void _beforDinit(MimiProcess *self)
{
    /* override in user code */
}

static void addSubProcess(MimiProcess *self, char *subProcessName, void *new_ProcessFun)
{
    Args *attributeList = self->attributeList;
    Args *initArgs = New_args(NULL);
    initArgs->setPtr(initArgs, "context", self);
    void *(*newProcessFun)(Args * initArgs) = (void *(*)(Args * initArgs)) new_ProcessFun;
    void *subProcess = newProcessFun(initArgs);
    attributeList->setObject(attributeList, subProcessName, "process", subProcess);
    initArgs->deinit(initArgs);
}

static void addSubobject(MimiProcess *self, char *subObjectName, void *new_ObjectFun)
{
    Args *initArgs = New_args(NULL);
    initArgs->setPtr(initArgs, "context", self);
    void *(*new_Object)(Args * initArgs) = (void *(*)(Args *))new_ObjectFun;
    void *subObject = new_Object(initArgs);
    self->setPtr(self, subObjectName, subObject);
    initArgs->deinit(initArgs);
}

static void dinitSubProcessByName(MimiProcess *self, char *subProcessName)
{
    MimiProcess *subProcess = self->getPtr(self, subProcessName);
    subProcess->deinit(subProcess);
}

static void _potableInitDefault(MimiProcess *self)
{
    /* override in user code, init the hardward dependence issues */
}

static void argBind(MimiProcess *self, char *type, char *name, void *pointer)
{
    self->attributeList->bind(self->attributeList, type, name, pointer);
}

static char *argPinrt(MimiProcess *self, char *name)
{
    return self->attributeList->print(self->attributeList, name);
}

static void argBindInt(MimiProcess *self, char *name, int *valPtr)
{
    self->attributeList->bindInt(self->attributeList, name, valPtr);
}

static void argBindFloat(MimiProcess *self, char *name, float *valPtr)
{
    self->attributeList->bindFloat(self->attributeList, name, valPtr);
}

static void argBindString(MimiProcess *self, char *name, char **valPtr)
{
    self->attributeList->bindStr(self->attributeList, name, valPtr);
}

static int argSet(MimiProcess *self, char *name, char *valStr)
{
    return self->attributeList->set(self->attributeList, name, valStr);
}

static void init(MimiProcess *self, Args *args)
{
    /* List */
    self->attributeList = New_args(NULL);

    /* operation */
    self->deinit = deinit;
    self->update = update;
    self->enable = enable;
    self->disable = disable;

    self->setInt = setInt64;
    self->setPtr = setPointer;
    self->setFloat = setFloat;
    self->setStr = setStr;

    self->getInt = getInt64;
    self->getPtr = getPointer;
    self->getFloat = getFloat;
    self->getStr = getStr;

    self->argBindInt = argBindInt;
    self->argBindFloat = argBindFloat;
    self->argBindString = argBindString;

    // arg general operations
    self->argBind = argBind;
    self->argPrint = argPinrt;
    self->argSet = argSet;

    self->loadAttributeFromArgs = loadAttributeFromArgs;
    // subObject
    self->addSubobject = addSubobject;
    self->addSubProcess = addSubProcess;
    self->dinitSubProcessByName = dinitSubProcessByName;

    /* attrivute */
    self->setInt(self, "isEnable", 1);
    self->setPtr(self, "context", self);

    /* override */
    self->_updateHandle = _processRootUpdateHandle;
    self->_beforDinit = _beforDinit;

    /* args */
    if (NULL != args)
    {
        self->loadAttributeFromArgs(self, args, "context");
        self->loadAttributeFromArgs(self, args, "isEnable");
    }
}

MimiProcess *New_mimiProcess(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(MimiProcess));
    MimiProcess *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
