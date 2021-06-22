#include "mimiProcess.h"
#include "dataArgs.h"
#include "dataMemory.h"

static void deinit(mimiProcess_t *self)
{
    self->_beforDinit(self);
    self->subProcessList->dinit(self->subProcessList);
    self->attributeList->dinit(self->attributeList);
    DynMemPut(self->mem);
}

static void update(mimiProcess_t *self)
{
    // return if is not enable
    if (0 == self->getInt64(self, "isEnable"))
    {
        return;
    }
    self->_updateHandle(self);
}
static void _processRootUpdateHandle(mimiProcess_t *self)
{
    // override the handle function here
}
static void enable(mimiProcess_t *self)
{
    self->setInt64(self, "isEnable", 1);
}

static void disable(mimiProcess_t *self)
{
    self->setInt64(self, "isEnable", 0);
}

static void setInt64(mimiProcess_t *self, char *name, long long val)
{
    self->attributeList->setInt(self->attributeList, name, val);
}

static void setPointer(mimiProcess_t *self, char *name, void *pointer)
{
    self->attributeList->setPoi(self->attributeList, name, pointer);
}

static void setFloat(mimiProcess_t *self, char *name, float value)
{
    self->attributeList->setFlt(self->attributeList, name, value);
}

static void setStr(mimiProcess_t *self, char *name, char *str)
{
    self->attributeList->setStr(self->attributeList, name, str);
}

static long long getInt64(mimiProcess_t *self, char *name)
{
    return self->attributeList->getInt(self->attributeList, name);
}

static void *getPointer(mimiProcess_t *self, char *name)
{
    return self->attributeList->getPoi(self->attributeList, name);
}

static float getFloat(mimiProcess_t *self, char *name)
{
    return self->attributeList->getFlt(self->attributeList, name);
}

char *getStr(mimiProcess_t *self, char *name)
{
    return self->attributeList->getStr(self->attributeList, name);
}

static void loadAttributeFromArgs(mimiProcess_t *self, args_t *args, char *name)
{
    args->copyArg(args, name, self->attributeList);
}

static void _beforDinit(mimiProcess_t *self)
{
    /* override in user code */
}

static void addSubProcess(mimiProcess_t *self, char *subProcessName, void *new_ProcessFun)
{
    args_t *initArgs = New_args(NULL);
    initArgs->setPoi(initArgs, "context", self);
    void *(*new_Object)(args_t * initArgs) = new_ProcessFun;
    void *subObject = new_Object(initArgs);
    self->setPoi(self, subProcessName, subObject);
    initArgs->dinit(initArgs);
}

static void addSubobject(mimiProcess_t *self, char *subObjectName, void *new_ObjectFun)
{
    args_t *initArgs = New_args(NULL);
    initArgs->setPoi(initArgs, "context", self);
    void *(*new_Object)(args_t * initArgs) = new_ObjectFun;
    void *subObject = new_Object(initArgs);
    self->setPoi(self, subObjectName, subObject);
    initArgs->dinit(initArgs);
}

static void dinitSubProcess(mimiProcess_t *self, char *subProcessName)
{
    mimiProcess_t *subProcess = self->getPointer(self, subProcessName);
    subProcess->dinit(subProcess);
}

static void _potableInitDefault(mimiProcess_t *self)
{
    /* override in user code, init the hardward dependence issues */
}

static void init(mimiProcess_t *self, args_t *args)
{
    /* List */
    self->subProcessList = New_link(NULL);
    self->attributeList = New_args(NULL);

    /* operation */
    self->dinit = deinit;
    self->update = update;
    self->enable = enable;
    self->disable = disable;

    self->setInt64 = setInt64;
    self->setPoi = setPointer;
    self->setFloat = setFloat;
    self->setStr = setStr;

    self->getInt64 = getInt64;
    self->getPointer = getPointer;
    self->getFloat = getFloat;
    self->getStr = getStr;

    self->loadAttributeFromArgs = loadAttributeFromArgs;
    // subObject
    self->addSubobject = addSubobject;
    self->addSubProcess = addSubProcess;
    self->dinitSubProcess = dinitSubProcess;

    /* attrivute */
    self->setInt64(self, "isEnable", 1);
    self->setPoi(self, "context", self);

    /* override */
    self->_updateHandle = _processRootUpdateHandle;
    self->_beforDinit = _beforDinit;
    self->_portableInit = _potableInitDefault;

    /* args */
    if (NULL != args)
    {
        self->loadAttributeFromArgs(self, args, "context");
        self->loadAttributeFromArgs(self, args, "isEnable");
        self->loadAttributeFromArgs(self, args, "_portableInit");
    }


}

mimiProcess_t *New_mimiProcess(args_t *args)
{
    DMEM *mem = DynMemGet(sizeof(mimiProcess_t));
    mimiProcess_t *self = mem->addr;
    self->mem = mem;
    self->init = init;
    self->init(self, args);
    return self;
}
