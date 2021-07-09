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

static void setInt64(MimiProcess *self, char *argDir, long long val)
{
    MimiProcess *processNow = self->goToProcess(self, argDir, 1);
    char name[64] = {0};
    getLastTokenBySign(argDir, name, '.');
    processNow->attributeList->setInt(processNow->attributeList,
                                      name, val);
    subscribeHandle(processNow, name);
}

static void setPointer(MimiProcess *self, char *argDir, void *pointer)
{
    MimiProcess *processNow = self->goToProcess(self, argDir, 1);
    char name[64] = {0};
    getLastTokenBySign(argDir, name, '.');
    processNow->attributeList->setPtr(processNow->attributeList,
                                      name, pointer);
    subscribeHandle(processNow, name);
}

static void setFloat(MimiProcess *self, char *argDir, float value)
{
    MimiProcess *processNow = self->goToProcess(self, argDir, 1);
    char name[64] = {0};
    getLastTokenBySign(argDir, name, '.');
    processNow->attributeList->setFloat(processNow->attributeList,
                                      name, value);
    subscribeHandle(processNow, name);
}

static void setStr(MimiProcess *self, char *argDir, char *str)
{
    MimiProcess *processNow = self->goToProcess(self, argDir, 1);
    char name[64] = {0};
    getLastTokenBySign(argDir, name, '.');
    processNow->attributeList->setStr(processNow->attributeList,
                                      name, str);
    subscribeHandle(processNow, name);
}

static long long getInt64(MimiProcess *self, char *argDir)
{
    MimiProcess *processNow = self->goToProcess(self, argDir, 1);
    char argName[64] = {0};
    getLastTokenBySign(argDir, argName, '.');
    return processNow->attributeList->getInt(processNow->attributeList,
                                             argName);
}

static void *getPointer(MimiProcess *self, char *argDir)
{
    MimiProcess *processNow = self->goToProcess(self, argDir, 1);
    char argName[64] = {0};
    getLastTokenBySign(argDir, argName, '.');
    return processNow->attributeList->getPtr(processNow->attributeList,
                                             argName);
}

static float getFloat(MimiProcess *self, char *argDir)
{
    MimiProcess *processNow = self->goToProcess(self, argDir, 1);
    char argName[64] = {0};
    getLastTokenBySign(argDir, argName, '.');
    return processNow->attributeList->getFloat(processNow->attributeList,
                                               argName);
}

char *getStr(MimiProcess *self, char *argDir)
{
    MimiProcess *processNow = self->goToProcess(self, argDir, 1);
    char argName[64] = {0};
    getLastTokenBySign(argDir, argName, '.');
    return processNow->attributeList->getStr(processNow->attributeList,
                                             argName);
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

static MimiProcess *goToProcess(MimiProcess *self, char *processDirectory, int keepToken)
{
    MimiProcess *processNow = self;
    // sign in the argv memory
    char *token[16] = {0};
    DMEM *tokenMem[16] = {0};
    for (int i = 0; i < 16; i++)
    {
        tokenMem[i] = DynMemGet(sizeof(char) * 256);
        token[i] = (char *)tokenMem[i]->addr;
        token[i][0] = 0;
    }
    int processArgc = devideStringBySign(processDirectory, token, '.');
    for (int i = 0; i < processArgc - keepToken; i++)
    {
        processNow = processNow->getSubProcess(processNow, token[i]);
        if (processNow == NULL)
        {
            goto exit;
        }
    }
    goto exit;
exit:
    for (int i = 0; i < 16; i++)
    {
        DynMemPut(tokenMem[i]);
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
