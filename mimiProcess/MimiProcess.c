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
        MimiObj *subProcess = argEach->getPtr(argEach);
        subProcess->deinit(subProcess);
    }
    return 0;
}

static void dinitAllSubProcess(MimiObj *self)
{
    Args *args = self->attributeList;
    args->foreach (args, dinitEachSubprocess, NULL);
}

static void deinit(MimiObj *self)
{
    self->_beforDinit(self);
    dinitAllSubProcess(self);
    self->attributeList->deinit(self->attributeList);
    DynMemPut(self->mem);
}

static void update(MimiObj *self)
{
    // return if is not enable
    if (0 == self->getInt(self, "isEnable"))
    {
        return;
    }
    self->_updateHandle(self);
}
static void _processRootUpdateHandle(MimiObj *self)
{
    // override the handle function here
}

static void subscribeHandle(MimiObj *self, char *argDir)
{
    char prefixedArgDir[256] = {0};
    strAppend(prefixedArgDir, "[subscribe]");
    strAppend(prefixedArgDir, argDir);
    void (*subscribeHandler)(MimiObj * self) = self->getPtr(self, prefixedArgDir);
    if (NULL != subscribeHandler)
    {
        subscribeHandler(self);
    }
}

static void enable(MimiObj *self)
{
    self->setInt(self, "isEnable", 1);
}

static void disable(MimiObj *self)
{
    self->setInt(self, "isEnable", 0);
}

static void setInt64(MimiObj *self, char *argDir, long long val)
{
    MimiObj *processNow = self->gotoObj(self, argDir, 1);
    char name[64] = {0};
    getLastTokenBySign(argDir, name, '.');
    processNow->attributeList->setInt(processNow->attributeList,
                                      name, val);
}

static void setPointer(MimiObj *self, char *argDir, void *pointer)
{
    MimiObj *processNow = self->gotoObj(self, argDir, 1);
    char name[64] = {0};
    getLastTokenBySign(argDir, name, '.');
    processNow->attributeList->setPtr(processNow->attributeList,
                                      name, pointer);
}

static void setFloat(MimiObj *self, char *argDir, float value)
{
    MimiObj *processNow = self->gotoObj(self, argDir, 1);
    char name[64] = {0};
    getLastTokenBySign(argDir, name, '.');
    processNow->attributeList->setFloat(processNow->attributeList,
                                        name, value);
}

static void setStr(MimiObj *self, char *argDir, char *str)
{
    MimiObj *processNow = self->gotoObj(self, argDir, 1);
    char name[64] = {0};
    getLastTokenBySign(argDir, name, '.');
    processNow->attributeList->setStr(processNow->attributeList,
                                      name, str);
}

static long long getInt64(MimiObj *self, char *argDir)
{
    MimiObj *processNow = self->gotoObj(self, argDir, 1);
    char argName[64] = {0};
    getLastTokenBySign(argDir, argName, '.');
    return processNow->attributeList->getInt(processNow->attributeList,
                                             argName);
}

static void *getPointer(MimiObj *self, char *argDir)
{
    MimiObj *processNow = self->gotoObj(self, argDir, 1);
    char argName[64] = {0};
    getLastTokenBySign(argDir, argName, '.');
    return processNow->attributeList->getPtr(processNow->attributeList,
                                             argName);
}

static float getFloat(MimiObj *self, char *argDir)
{
    MimiObj *processNow = self->gotoObj(self, argDir, 1);
    char argName[64] = {0};
    getLastTokenBySign(argDir, argName, '.');
    return processNow->attributeList->getFloat(processNow->attributeList,
                                               argName);
}

char *getStr(MimiObj *self, char *argDir)
{
    MimiObj *processNow = self->gotoObj(self, argDir, 1);
    char argName[64] = {0};
    getLastTokenBySign(argDir, argName, '.');
    return processNow->attributeList->getStr(processNow->attributeList,
                                             argName);
}

static void load(MimiObj *self, Args *args, char *name)
{
    args->copyArg(args, name, self->attributeList);
}

static void _beforDinit(MimiObj *self)
{
    /* override in user code */
}

static void setObj(MimiObj *self, char *subProcessName, void *new_ProcessFun)
{
    /* class means subprocess init */
    char prifix[] = "[cls]";
    char nameBuff[64] = {0};
    strAppend(nameBuff, prifix);
    strAppend(nameBuff, subProcessName);
    self->setPtr(self, nameBuff, new_ProcessFun);
}

static void addOther(MimiObj *self, char *subObjectName, void *new_ObjectFun)
{
    Args *initArgs = New_args(NULL);
    initArgs->setPtr(initArgs, "context", self);
    void *(*new_Object)(Args * initArgs) = (void *(*)(Args *))new_ObjectFun;
    void *subObject = new_Object(initArgs);
    self->setPtr(self, subObjectName, subObject);
    initArgs->deinit(initArgs);
}

static void freeObj(MimiObj *self, char *subProcessName)
{
    MimiObj *subProcess = self->getPtr(self, subProcessName);
    subProcess->deinit(subProcess);
}

static void bind(MimiObj *self, char *type, char *name, void *pointer)
{
    self->attributeList->bind(self->attributeList, type, name, pointer);
}

static char *print(MimiObj *self, char *name)
{
    return self->attributeList->print(self->attributeList, name);
}

static void bindInt(MimiObj *self, char *name, int *valPtr)
{
    self->attributeList->bindInt(self->attributeList, name, valPtr);
}

static void bindFloat(MimiObj *self, char *name, float *valPtr)
{
    self->attributeList->bindFloat(self->attributeList, name, valPtr);
}

static void bindString(MimiObj *self, char *name, char **valPtr)
{
    self->attributeList->bindStr(self->attributeList, name, valPtr);
}

static int set(MimiObj *self, char *argDir, char *valStr)
{
    MimiObj *processNow = self->gotoObj(self, argDir, 1);
    char argName[64] = {0};
    getLastTokenBySign(argDir, argName, '.');
    return processNow->attributeList->set(processNow->attributeList, argName, valStr);
}

static void subscribe(MimiObj *self,
                      char *argDir,
                      void (*handle)(MimiObj *self))
{
    char argName[256] = {0};
    strAppend(argName, "[subscribe]");
    strAppend(argName, argDir);
    self->attributeList->setPtr(self->attributeList,
                                argName, handle);
}

static MimiObj *initSubProcess(MimiObj *self, char *name)
{
    char prifix[] = "[cls]";
    char initFunName[64] = {0};
    strAppend(initFunName, prifix);
    strAppend(initFunName, name);
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

static MimiObj *getObj(MimiObj *self, char *name)
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

static MimiObj *gotoObj(MimiObj *self, char *processDirectory, int keepToken)
{
    MimiObj *processNow = self;
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
        processNow = processNow->getObj(processNow, token[i]);
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

static void publish(MimiObj *self, char *argDir)
{
    subscribeHandle(self, argDir);
}

static void init(MimiObj *self, Args *args)
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

    self->bindInt = bindInt;
    self->bindFloat = bindFloat;
    self->bindString = bindString;

    // arg general operations
    self->bind = bind;
    self->print = print;
    self->set = set;

    self->load = load;
    // subObject
    self->addOther = addOther;
    self->setObj = setObj;
    self->getObj = getObj;
    self->gotoObj = gotoObj;
    self->freeObj = freeObj;

    /* attrivute */
    self->setPtr(self, "context", self);

    /* override */
    self->_updateHandle = _processRootUpdateHandle;
    self->_beforDinit = _beforDinit;

    /* event operation */
    self->subscribe = subscribe;
    self->publish = publish;

    /* args */
    if (NULL != args)
    {
        self->load(self, args, "context");
    }
}

MimiObj *New_MimiProcess(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(MimiObj));
    MimiObj *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
