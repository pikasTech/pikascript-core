#include "MimiProcess.h"
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
        MimiProcess *subProcess = argEach->getPtr(argEach);
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

static void subscribeHandle(MimiProcess *self, char *name)
{
    char prefix[] = "[subscribe]";
    char subscribeName[256] = {0};
    strPrint(subscribeName, prefix);
    strPrint(subscribeName, name);
    void (*subscribeHandler)(MimiProcess * self) = self->getPtr(self, subscribeName);
    if (NULL != subscribeHandler)
    {
        subscribeHandler(self);
    }
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
    subscribeHandle(self, name);
}

static void setPointer(MimiProcess *self, char *name, void *pointer)
{
    self->attributeList->setPtr(self->attributeList, name, pointer);
    subscribeHandle(self, name);
}

static void setFloat(MimiProcess *self, char *name, float value)
{
    self->attributeList->setFloat(self->attributeList, name, value);
    subscribeHandle(self, name);
}

static void setStr(MimiProcess *self, char *name, char *str)
{
    self->attributeList->setStr(self->attributeList, name, str);
    subscribeHandle(self, name);
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
    subscribeHandle(self, name);
}

static void _beforDinit(MimiProcess *self)
{
    /* override in user code */
}

static void addSubProcess(MimiProcess *self, char *subProcessName, void *new_ProcessFun)
{
    /* class means subprocess init */
    char prifix[] = "[cls]";
    char nameBuff[64] = {0};
    strPrint(nameBuff, prifix);
    strPrint(nameBuff, subProcessName);
    self->setPtr(self, nameBuff, new_ProcessFun);
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

static void subscribe(MimiProcess *self,
                      char *subscribeVarName,
                      void (*handle)(MimiProcess *self))
{
    char prefix[] = "[subscribe]";
    char argName[256] = {0};
    strPrint(argName, prefix);
    strPrint(argName, subscribeVarName);
    self->setPtr(self, argName, handle);
}

static MimiProcess *initSubProcess(MimiProcess *self, char *name)
{
    char prifix[] = "[cls]";
    char initFunName[64] = {0};
    strPrint(initFunName, prifix);
    strPrint(initFunName, name);
    if (!self->attributeList->isArgExist(self->attributeList,
                                         initFunName))
    {
        /* no init fun, could not found subprocess */
        return NULL;
    }
    /* init the subprocess */
    Args *attributeList = self->attributeList;
    Args *initArgs = New_args(NULL);
    initArgs->setPtr(initArgs, "context", self);
    void *(*newProcessFun)(Args * initArgs) = (void *(*)(Args * initArgs)) self->attributeList->getPtr(self->attributeList,
                                                                                                       initFunName);
    void *subProcess = newProcessFun(initArgs);
    attributeList->setObject(attributeList, name, "process", subProcess);
    initArgs->deinit(initArgs);
    return self->getPtr(self,
                        name);
}

static MimiProcess *getSubProcess(MimiProcess *self, char *name)
{
    /* check subprocess */
    if (!self->attributeList->isArgExist(self->attributeList,
                                         name))
    {
        /* no inited subprocess, check subprocess init fun*/
        return initSubProcess(self, name);
    }

    /* finded subscribe, check type*/
    char *type = self->attributeList->getType(self->attributeList,
                                              name);
    if (!mimiStrEqu("_class-process", type))
    {
        /* type error, could not found subprocess */
        return NULL;
    }
    return self->getPtr(self, name);
}

static MimiProcess *goToProcess(MimiProcess *root, char *processDirectory, int deepth)
{
    MimiProcess *processNow = root;
    // sign in the argv memory
    char *directoryUnit[16] = {0};
    DMEM *processMem[16] = {0};
    for (int i = 0; i < 16; i++)
    {
        processMem[i] = DynMemGet(sizeof(char) * 256);
        directoryUnit[i] = (char *)processMem[i]->addr;
        directoryUnit[i][0] = 0;
    }
    int processArgc = devideStringBySign(processDirectory, directoryUnit, '.');
    for (int i = 0; i < processArgc - deepth; i++)
    {
        processNow = processNow->getSubProcess(processNow, directoryUnit[i]);
        if (processNow == NULL)
        {
            goto exit;
        }
    }
    goto exit;
exit:
    for (int i = 0; i < 16; i++)
    {
        DynMemPut(processMem[i]);
    }
    return processNow;
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
    self->getSubProcess = getSubProcess;
    self->goToProcess = goToProcess;
    self->dinitSubProcessByName = dinitSubProcessByName;

    /* attrivute */
    self->setPtr(self, "context", self);

    /* override */
    self->_updateHandle = _processRootUpdateHandle;
    self->_beforDinit = _beforDinit;

    /* event operation */
    self->subscribe = subscribe;

    /* args */
    if (NULL != args)
    {
        self->loadAttributeFromArgs(self, args, "context");
        self->loadAttributeFromArgs(self, args, "isEnable");
    }
}

MimiProcess *New_MimiProcess(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(MimiProcess));
    MimiProcess *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
