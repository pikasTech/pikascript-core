#include "MimiObj.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "mimiFansList.h"
#include "mimiMailbox.h"

int deinitEachSubObj(Arg *argEach, Args *handleArgs)
{
    if (NULL != handleArgs)
    {
        /* error: tOhis handle not need handle args */
        return 1;
    }
    if (strEqu(argEach->typeDynMem->addr, "_class-process"))
    {
        MimiObj *subObj = arg_getPtr(argEach);
        if (NULL != subObj)
        {
            obj_deinit(subObj);
        }
    }
    return 0;
}

void dinitAllSubObj(MimiObj *self)
{
    Args *args = self->attributeList;
    args_foreach(args, deinitEachSubObj, NULL);
}

int obj_deinit(MimiObj *self)
{
    self->_beforDinit(self);
    dinitAllSubObj(self);
    args_deinit(self->attributeList);
    DynMemPut(self->mem);
    return 0;
}

int obj_update(MimiObj *self)
{
    // return if is not enable
    if (0 == obj_getInt(self, "isEnable"))
    {
        return 1;
    }
    self->_updateHandle(self);
    return 0;
}
void _UpdateHandle(MimiObj *self)
{
    // override the handle function here
}

int obj_enable(MimiObj *self)
{
    obj_setInt(self, "isEnable", 1);
    return 0;
}

int obj_disable(MimiObj *self)
{
    obj_setInt(self, "isEnable", 0);
    return 0;
}

int obj_setInt(MimiObj *self, char *argPath, long long val)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        /* [error] object no found */
        return 1;
    }
    char name[64] = {0};
    strGetLastToken(name, argPath, '.');
    args_setInt(obj->attributeList, name, val);
    return 0;
}

int obj_setPtr(MimiObj *self, char *argPath, void *pointer)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return 1;
    }
    char name[64] = {0};
    strGetLastToken(name, argPath, '.');
    args_setPtr(obj->attributeList,
                name, pointer);
    return 0;
}

int obj_setFloat(MimiObj *self, char *argPath, float value)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return 1;
    }
    char name[64] = {0};
    strGetLastToken(name, argPath, '.');
    args_setFloat(obj->attributeList,
                  name, value);
    return 0;
}

int obj_setStr(MimiObj *self, char *argPath, char *str)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return 1;
    }
    char name[64] = {0};
    strGetLastToken(name, argPath, '.');
    args_setStr(obj->attributeList,
                name, str);
    return 0;
}

long long obj_getInt(MimiObj *self, char *argPath)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return -999999999;
    }
    char argName[64] = {0};
    strGetLastToken(argName, argPath, '.');
    return args_getInt(obj->attributeList,
                       argName);
}

Arg *obj_getArg(MimiObj *self, char *argPath)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return NULL;
    }
    char argName[64] = {0};
    strGetLastToken(argName, argPath, '.');
    return args_getArg(obj->attributeList, argName);
}

int obj_setArg(MimiObj *self, char *argPath, Arg *arg)
{
    /* setArg would copy arg */
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return 1;
    }
    args_copyArg(self->attributeList, arg);
    return 0;
}

void *obj_getPtr(MimiObj *self, char *argPath)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return NULL;
    }
    char argName[64] = {0};
    strGetLastToken(argName, argPath, '.');
    return args_getPtr(obj->attributeList, argName);
}

float obj_getFloat(MimiObj *self, char *argPath)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return -999.999;
    }
    char argName[64] = {0};
    strGetLastToken(argName, argPath, '.');
    return args_getFloat(obj->attributeList,
                         argName);
}

char *obj_getStr(MimiObj *self, char *argPath)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return NULL;
    }
    char argName[64] = {0};
    strGetLastToken(argName, argPath, '.');
    return args_getStr(obj->attributeList,
                       argName);
}

int obj_load(MimiObj *self, Args *args, char *name)
{
    args_copyArgByName(args, name, self->attributeList);
    return 0;
}

void _beforDinit(MimiObj *self)
{
    /* override in user code */
}

int obj_newObj(MimiObj *self, char *objName, void *newFun)
{
    /* class means subprocess init */
    char prifix[] = "[cls]";
    char className[64] = {0};
    strAppend(className, prifix);
    strAppend(className, objName);
    obj_setPtr(self, className, newFun);
    /* add void process Ptr, no inited */
    args_setPtrWithType(self->attributeList, objName, "process", NULL);
    return 0;
}

int obj_addOther(MimiObj *self, char *subObjectName, void *new_ObjectFun)
{
    Args *initArgs = New_args(NULL);
    args_setPtr(initArgs, "context", self);
    void *(*new_Object)(Args * initArgs) = (void *(*)(Args *))new_ObjectFun;
    void *subObject = new_Object(initArgs);
    obj_setPtr(self, subObjectName, subObject);
    args_deinit(initArgs);
    return 0;
}

int obj_freeObj(MimiObj *self, char *subProcessName)
{
    MimiObj *subProcess = obj_getPtr(self, subProcessName);
    obj_deinit(subProcess);
    return 0;
}

int obj_bind(MimiObj *self, char *type, char *name, void *pointer)
{
    args_bind(self->attributeList, type, name, pointer);
    return 0;
}

char *obj_print(MimiObj *self, char *name)
{
    return args_print(self->attributeList, name);
}

int obj_bindInt(MimiObj *self, char *name, int *valPtr)
{
    args_bindInt(self->attributeList, name, valPtr);
    return 0;
}

int obj_bindFloat(MimiObj *self, char *name, float *valPtr)
{
    args_bindFloat(self->attributeList, name, valPtr);
    return 0;
}

int obj_bindString(MimiObj *self, char *name, char **valPtr)
{
    args_bindStr(self->attributeList, name, valPtr);
    return 0;
}

int obj_set(MimiObj *self, char *argPath, char *valStr)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        /* cant get object */
        return 3;
    }
    char buff[64] = {0};
    char *argName = strGetLastToken(buff, argPath, '.');
    return args_set(obj->attributeList, argName, valStr);
}

void newObjPathect(MimiObj *self, char *name, void *(*newObjFun)(Args *initArgs))
{
    Args *initArgs = New_args(NULL);
    args_setPtr(initArgs, "context", self);
    args_setStr(initArgs, "name", name);
    MimiObj *newObj = newObjFun(initArgs);
    args_setPtrWithType(self->attributeList, name, "process", newObj);
    args_deinit(initArgs);
}

MimiObj *initObj(MimiObj *self, char *name)
{
    char prifix[] = "[cls]";
    char classPath[64] = {0};
    strAppend(classPath, prifix);
    strAppend(classPath, name);
    /* init the subprocess */
    void *(*newObjFun)(Args * initArgs) = args_getPtr(self->attributeList, classPath);
    if (NULL == newObjFun)
    {
        /* no such object */
        return NULL;
    }
    newObjPathect(self, name, newObjFun);
    return obj_getPtr(self, name);
}

MimiObj *obj_getObjPathect(MimiObj *self, char *name)
{
    /* check subprocess */
    if (NULL == args_getPtr(self->attributeList, name))
    {
        /* no inited subprocess, check subprocess init fun*/
        return initObj(self, name);
    }

    /* finded subscribe, check type*/
    char *type = args_getType(self->attributeList,
                              name);
    if (!strEqu("_class-process", type))
    {
        /* type error, could not found subprocess */
        return NULL;
    }
    return obj_getPtr(self, name);
}

MimiObj *obj_getObj(MimiObj *self, char *processPathectory, int keepToken)
{
    MimiObj *obj = self;
    // sign in the argv memory
    char *token[16] = {0};
    DMEM *tokenMem[16] = {0};
    for (int i = 0; i < 16; i++)
    {
        tokenMem[i] = DynMemGet(sizeof(char) * 256);
        token[i] = (char *)tokenMem[i]->addr;
        token[i][0] = 0;
    }
    int processArgc = strGetToken(processPathectory, token, '.');
    for (int i = 0; i < processArgc - keepToken; i++)
    {
        obj = obj_getObjPathect(obj, token[i]);
        if (obj == NULL)
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
    return obj;
}

void loadMethodInfo(MimiObj *methodHost, char *methodName, char *methodDeclearation, void *methodPtr)
{
    char buff[2][128] = {0};
    int i = 0;
    char *methodPtrPath = strAppend(strAppend(buff[i++], "[methodPtr]"), methodName);
    char *methodDeclearationPath = strAppend(strAppend(buff[i++], "[methodDec]"), methodName);
    obj_setPtr(methodHost, methodPtrPath, methodPtr);
    obj_setStr(methodHost, methodDeclearationPath, methodDeclearation);
}

static char *getMethodDeclearation(MimiObj *methodHost, char *methodName)
{
    char buff[1][128] = {0};
    int i = 0;
    char *methodDeclearationPath = strAppend(strAppend(buff[i++], "[methodDec]"), methodName);
    return obj_getStr(methodHost, methodDeclearationPath);
}

static void *getMethodPtr(MimiObj *methodHost, char *methodName)
{
    char buff[1][128] = {0};
    int i = 0;
    char *methodPtrPath = strAppend(strAppend(buff[i++], "[methodPtr]"), methodName);
    return obj_getPtr(methodHost, methodPtrPath);
}

int obj_defineMethod(MimiObj *self,
                     char *declearation,
                     void (*methodPtr)(MimiObj *self, Args *args))
{
    char buff[3][128] = {0};
    int i = 0;
    char *cleanDeclearation = strDeleteChar(buff[i++], declearation, ' ');
    char *methodPath = strGetFirstToken(buff[i++], cleanDeclearation, '(');

    MimiObj *methodHost = obj_getObj(self, methodPath, 1);
    if (NULL == methodHost)
    {
        /* no found method object */
        return 1;
    }
    char *methodName = strGetLastToken(buff[i++], methodPath, '.');

    loadMethodInfo(methodHost, methodName, cleanDeclearation, methodPtr);
    return 0;
}

char *getPathectStr(char *buff, char *argPath)
{
    char *directStr = NULL;
    directStr = strCut(buff, argPath, '"', '"');
    if (NULL != directStr)
    {
        return directStr;
    }
    directStr = strCut(buff, argPath, '\'', '\'');
    if (NULL != directStr)
    {
        return directStr;
    }
    return NULL;
}

static int loadArgByType(MimiObj *self,
                         char *definedName,
                         char *definedType,
                         char *argPath,
                         Args *args)
{
    if (strEqu(definedType, ""))
    {
        char buff[1][128] = {0};
        int i = 0;
        char *directStr = getPathectStr(buff[i++], argPath);
        if (NULL != directStr)
        {
            /* direct value */
            args_setStr(args, definedName, directStr);
            /* ok */
            return 0;
        }
        if ((argPath[0] >= '0') && (argPath[0] <= '9'))
        {
            /* direct value */
            char *argName = argPath;
            if (strIsContain(argName, '.'))
            {
                args_setFloat(args, definedName, 0);
                args_set(args, definedName, argPath);
                /* succeed */
                return 0;
            }
            args_setInt(args, definedName, 0);
            args_set(args, definedName, argPath);
            /* succeed */
            return 0;
        }
        Arg *arg = obj_getArg(self, argPath);
        if (arg == NULL)
        {
            /* cand get arg */
            return 3;
        }
        Arg *argCopied = arg_copy(arg);
        arg_setName(argCopied, definedName);
        args_setArg(args, argCopied);
        return 0;
    }
    if (strEqu(definedType, "string"))
    {
        char buff[1][128] = {0};
        int i = 0;
        /* solve the string type */
        char *directStr = getPathectStr(buff[i++], argPath);
        if (NULL != directStr)
        {
            /* direct value */
            args_setStr(args, definedName, directStr);
            /* ok */
            return 0;
        }
        /* reference value */
        char *refStr = obj_getStr(self, argPath);
        if (NULL == refStr)
        {
            /* faild */
            return 1;
        }
        args_setStr(args, definedName, refStr);
        /* succeed */
        return 0;
    }
    if (strEqu(definedType, "int"))
    {
        /* solve the int type */
        args_setInt(args, definedName, 0);
        if ((argPath[0] >= '0') && (argPath[0] <= '9'))
        {
            /* direct value */
            args_set(args, definedName, argPath);
            /* succeed */
            return 0;
        }
        /* reference value */
        int referenceVal = obj_getInt(self, argPath);
        args_setInt(args, definedName, referenceVal);
        /* succeed */
        return 0;
    }
    if (strEqu(definedType, "float"))
    {
        /* solve the float type */
        args_setFloat(args, definedName, 0);
        if ((argPath[0] >= '0') && (argPath[0] <= '9'))
        {
            /* direct value */
            args_set(args, definedName, argPath);
            /* succeed */
            return 0;
        }
        /* reference value */
        float referenceVal = obj_getFloat(self, argPath);
        args_setFloat(args, definedName, referenceVal);
        /* succeed */
        return 0;
    }
    if (strEqu(definedType, "pointer"))
    {
        /* only support reference value */
        void *ptr = obj_getPtr(self, argPath);
        args_setPtr(args, definedName, ptr);
        return 0;
    }
    /* type match faild */
    return 2;
}

char *getTypeVal(char *buff, char *typeToken)
{
    if (!strIsContain(typeToken, ':'))
    {
        return strAppend(buff, "");
    }
    return strGetLastToken(buff, typeToken, ':');
}

static Args *getArgsBySort(MimiObj *self, char *typeList, char *argList)
{
    char buff[2][128] = {0};
    int i = 0;
    char *typeListBuff = buff[i++];
    memcpy(typeListBuff, typeList, strGetSize(typeList));
    char *argListBuff = buff[i++];
    memcpy(argListBuff, argList, strGetSize(argList));
    Args *args = New_args(NULL);
    while (1)
    {
        char buff[4][128] = {0};
        int i = 0;
        char *typeToken = strPopToken(buff[i++], typeListBuff, ',');
        char *argToken = strPopToken(buff[i++], argListBuff, ',');
        if ((0 == argToken[0]) || (0 == typeToken[0]))
        {
            /* arg or type poped finised */
            break;
        }

        char *typeName = strGetFirstToken(buff[i++], typeToken, ':');
        char *typeVal = getTypeVal(buff[i++], typeToken);
        char *argPath = argToken;

        if (0 != loadArgByType(self,
                               typeName,
                               typeVal,
                               argPath,
                               args))
        {
            args_deinit(args);
            return NULL;
        }
    }
    return args;
}

static Args *getArgsByNameMatch(MimiObj *self, char *typeList, char *argList)
{
    char buff[2][128] = {0};
    int i = 0;
    char *typeListBuff = buff[i++];
    memcpy(typeListBuff, typeList, strGetSize(typeList));
    Args *args = New_args(NULL);
    while (1)
    {
        char buff[4][128] = {0};
        int i = 0;
        char *typeToken = strPopToken(buff[i++], typeListBuff, ',');

        /* poped all type from typeList */
        if (0 == typeToken[0])
        {
            break;
        }

        char *typeName = strGetFirstToken(buff[i++], typeToken, ':');
        char *typeVal = getTypeVal(buff[i++], typeToken);

        char *argListBuff = buff[i++];
        memcpy(argListBuff, argList, strGetSize(argList));
        while (1)
        {
            char buff[4][64] = {0};
            int i = 0;
            char *argToken = strPopToken(buff[i++], argListBuff, ',');
            char *argName = strGetFirstToken(buff[i++], argToken, '=');
            char *argVal = strGetLastToken(buff[i++], argToken, '=');

            if (0 == argToken[0])
            {
                /* arg poped finised */
                break;
            }

            if (!strEqu(typeName, argName))
            {
                /* name not match */
                continue;
            }

            if (0 != loadArgByType(self,
                                   typeName,
                                   typeVal,
                                   argVal,
                                   args))
            {
                args_deinit(args);
                return NULL;
            }
        }
    }
    return args;
}

static Args *getArgsBySentence(MimiObj *self, char *typeList, char *argList)
{
    if (strIsContain(argList, '='))
    {
        return getArgsByNameMatch(self, typeList, argList);
    }
    return getArgsBySort(self, typeList, argList);
}

static void transferReturnVal(MimiObj *self, char *returnType, char *returnName, Args *args)
{
    if (strEqu("->int", returnType))
    {
        obj_setInt(self, returnName, args_getInt(args, "return"));
    }
    if (strEqu("->float", returnType))
    {
        obj_setFloat(self, returnName, args_getFloat(args, "return"));
    }
    if (strEqu("->string", returnType))
    {
        obj_setStr(self, returnName, args_getStr(args, "return"));
    }
}

char *getMethodPath(char *buff, char *methodToken)
{
    if (strIsContain(methodToken, '='))
    {
        return strGetLastToken(buff, methodToken, '=');
    }
    else
    {
        return methodToken;
    }
}

int obj_run(MimiObj *self, char *cmd)
{
    char buff[8][128] = {0};
    int i = 0;
    char *cleanCmd = strDeleteChar(buff[i++], cmd, ' ');
    char *methodToken = strGetFirstToken(buff[i++], cleanCmd, '(');
    char *methodPath = getMethodPath(buff[i++], methodToken);

    MimiObj *methodHost = obj_getObj(self, methodPath, 1);
    if (NULL == methodHost)
    {
        return 1;
    }
    char *methodName = strGetLastToken(buff[i++], methodPath, '.');
    void (*methodPtr)(MimiObj * self, Args * args) = getMethodPtr(methodHost, methodName);
    char *methodDeclearation = getMethodDeclearation(methodHost, methodName);

    if ((NULL == methodDeclearation) || (NULL == methodPtr))
    {
        return 2;
    }

    char *typeList = strCut(buff[i++], methodDeclearation, '(', ')');
    if (typeList == NULL)
    {
        return 3;
    }

    char *argList = strCut(buff[i++], cleanCmd, '(', ')');
    {
        if (argList == NULL)
        {
            return 4;
        }
    }

    char *returnType = strGetLastToken(buff[i++], methodDeclearation, ')');
    /* get type */
    Args *args = getArgsBySentence(self, typeList, argList);
    if (NULL == args)
    {
        /* get args faild */
        return 5;
    }
    /* run method */
    methodPtr(methodHost, args);
    /* transfer return */
    if (strIsContain(methodToken, '='))
    {
        char *returnName = strGetFirstToken(buff[i++], methodToken, '=');
        transferReturnVal(self, returnType, returnName, args);
    }
    args_deinit(args);
    /* succeed */
    return 0;
}

int obj_init(MimiObj *self, Args *args)
{
    /* List */
    self->attributeList = New_args(NULL);

    /* override */
    self->_updateHandle = _UpdateHandle;
    self->_beforDinit = _beforDinit;

    /* attrivute */
    obj_setPtr(self, "context", self);
    obj_setStr(self, "name", "root");

    /* load */
    if (NULL != args)
    {
        obj_load(self, args, "name");
        obj_load(self, args, "context");
    }
    self->name = obj_getStr(self, "name");
    return 0;
}

MimiObj *New_MimiObj(Args *args)
{
    DMEM *mem = DynMemGet(sizeof(MimiObj));
    if (NULL == mem)
    {
        printf("[error] memory is empty!");
        while (1)
            ;
    }
    MimiObj *self = (void *)(mem->addr);
    self->mem = mem;
    obj_init(self, args);
    return self;
}
