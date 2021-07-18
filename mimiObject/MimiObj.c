#include "MimiObj.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "strArgs.h"
#include "mimiFansList.h"
#include "mimiMailbox.h"

int deinitEachSubObj(Arg *argEach, Args *handleArgs)
{
    if (NULL != handleArgs)
    {
        /* error: tOhis handle not need handle args */
        return 1;
    }
    if (strEqu(arg_getType(argEach), "_class-process"))
    {
        MimiObj *subObj = arg_getPtr(argEach);
        if (NULL != subObj)
        {
            obj_deinit(subObj);
        }
    }
    return 0;
}

void deinitAllSubObj(MimiObj *self)
{
    Args *args = self->attributeList;
    args_foreach(args, deinitEachSubObj, NULL);
}

int obj_deinit(MimiObj *self)
{
    self->_beforDinit(self);
    deinitAllSubObj(self);
    args_deinit(self->attributeList);
    DynMemPut(self->mem);
    self = NULL;
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
    Args *buffs = New_strBuff();
    char *name = strsGetLastToken(buffs, argPath, '.');
    args_setInt(obj->attributeList, name, val);
    args_deinit(buffs);
    return 0;
}

int obj_setPtr(MimiObj *self, char *argPath, void *pointer)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return 1;
    }
    Args *buffs = New_strBuff();
    char *name = args_getBuff(buffs, 64);
    strGetLastToken(name, argPath, '.');
    args_setPtr(obj->attributeList,
                name, pointer);
    args_deinit(buffs);
    return 0;
}

int obj_setFloat(MimiObj *self, char *argPath, float value)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return 1;
    }
    Args *buffs = New_strBuff();
    char *name = args_getBuff(buffs, 64);
    strGetLastToken(name, argPath, '.');
    args_setFloat(obj->attributeList,
                  name, value);
    args_deinit(buffs);
    return 0;
}

int obj_setStr(MimiObj *self, char *argPath, char *str)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return 1;
    }
    Args *buffs = New_strBuff();
    char *name = args_getBuff(buffs, 64);
    strGetLastToken(name, argPath, '.');
    args_setStr(obj->attributeList,
                name, str);
    args_deinit(buffs);
    return 0;
}

long long obj_getInt(MimiObj *self, char *argPath)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return -999999999;
    }
    Args *buffs = New_strBuff();
    char *argName = strsGetLastToken(buffs, argPath, '.');
    args_deinit(buffs);
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
    Args *buffs = New_strBuff();
    char *argName = strsGetLastToken(buffs, argPath, '.');
    Arg *res = args_getArg(obj->attributeList, argName);
    args_deinit(buffs);
    return res;
}

int obj_setArg(MimiObj *self, char *argPath, Arg *arg)
{
    /* setArg would copy arg */
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        /* object no found */
        return 1;
    }
    args_copyArg(obj->attributeList, arg);
    return 0;
}

void *obj_getPtr(MimiObj *self, char *argPath)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return NULL;
    }
    Args *buffs = New_strBuff();
    char *argName = strsGetLastToken(buffs, argPath, '.');
    void *res = args_getPtr(obj->attributeList, argName);
    args_deinit(buffs);
    return res;
}

float obj_getFloat(MimiObj *self, char *argPath)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return -999.999;
    }
    Args *buffs = New_strBuff();
    char *argName = strsGetLastToken(buffs, argPath, '.');
    float res = args_getFloat(obj->attributeList,
                              argName);
    args_deinit(buffs);
    return res;
}

char *obj_getStr(MimiObj *self, char *argPath)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    if (NULL == obj)
    {
        return NULL;
    }
    Args *buffs = New_strBuff();
    char *argName = strsGetLastToken(buffs, argPath, '.');
    char *res = args_getStr(obj->attributeList,
                            argName);
    args_deinit(buffs);
    return res;
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

int obj_setObj(MimiObj *self, char *objName, void *newFun)
{
    /* class means subprocess init */
    char prifix[] = "[cls]";
    Args *buffs = New_strBuff();
    char *className = args_getBuff(buffs, 64);
    strAppend(className, prifix);
    strAppend(className, objName);
    obj_setPtr(self, className, newFun);
    /* add void process Ptr, no inited */
    args_setPtrWithType(self->attributeList, objName, "process", NULL);
    args_deinit(buffs);
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

int obj_freeObj(MimiObj *self, char *objPath)
{
    MimiObj *obj = obj_getPtr(self, objPath);
    obj_deinit(obj);
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
    Args *buffs = New_strBuff();
    char *argName = strsGetLastToken(buffs, argPath, '.');
    int res = args_set(obj->attributeList, argName, valStr);
    args_deinit(buffs);
    if (res == 1)
    {
        /* do not get arg */
        return 1;
    }
    if (res == 2)
    {
        /* type not match */
        return 2;
    }
    /* succeed */
    return 0;
}

void newObjDirect(MimiObj *self, char *name, void *(*newObjFun)(Args *initArgs))
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
    Args *buffs = New_strBuff();
    char *classPath = strAppend(strAppend(args_getBuff(buffs, 64), prifix), name);
    /* init the subprocess */
    void *(*newObjFun)(Args * initArgs) = args_getPtr(self->attributeList, classPath);
    if (NULL == newObjFun)
    {
        /* no such object */
        args_deinit(buffs);
        return NULL;
    }
    newObjDirect(self, name, newObjFun);
    args_deinit(buffs);
    return obj_getPtr(self, name);
}

MimiObj *obj_getObjDirect(MimiObj *self, char *name)
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

MimiObj *obj_getObj(MimiObj *self, char *objPath, int keepToken)
{
    Args *buffs = New_strBuff();
    char *objPathBuff = strsCopy(buffs, objPath);
    int tokenNum = strGetTokenNum(objPath, '.');
    MimiObj *obj = self;
    for (int i = 0; i < tokenNum - keepToken; i++)
    {
        char *token = strsPopToken(buffs, objPathBuff, '.');
        obj = obj_getObjDirect(obj, token);
        if (obj == NULL)
        {
            goto exit;
        }
    }
    goto exit;
exit:
    args_deinit(buffs);
    return obj;
}

void loadMethodInfo(MimiObj *methodHost, char *methodName, char *methodDeclearation, void *methodPtr)
{
    Args *buffs = New_strBuff();
    char *methodPtrPath = strAppend(strAppend(args_getBuff(buffs, 128), "[methodPtr]"), methodName);
    char *methodDeclearationPath = strAppend(strAppend(args_getBuff(buffs, 128), "[methodDec]"), methodName);
    obj_setPtr(methodHost, methodPtrPath, methodPtr);
    obj_setStr(methodHost, methodDeclearationPath, methodDeclearation);
    args_deinit(buffs);
}

static char *getMethodDeclearation(MimiObj *methodHost, char *methodName)
{
    Args *buffs = New_strBuff();
    char *methodDeclearationPath = strAppend(strAppend(args_getBuff(buffs, 128), "[methodDec]"), methodName);
    char *res = obj_getStr(methodHost, methodDeclearationPath);
    args_deinit(buffs);
    return res;
}

static void *getMethodPtr(MimiObj *methodHost, char *methodName)
{
    Args *buffs = New_strBuff();
    char *methodPtrPath = strAppend(strAppend(args_getBuff(buffs, 128), "[methodPtr]"), methodName);
    void *res = obj_getPtr(methodHost, methodPtrPath);
    args_deinit(buffs);
    return res;
}

int obj_defineMethod(MimiObj *self,
                     char *declearation,
                     void (*methodPtr)(MimiObj *self, Args *args))
{
    int size = strGetSize(declearation);
    int res = 0;
    Args *buffs = New_strBuff();
    char *cleanDeclearation = strDeleteChar(args_getBuff(buffs, size), declearation, ' ');
    char *methodPath = strGetFirstToken(args_getBuff(buffs, size), cleanDeclearation, '(');

    MimiObj *methodHost = obj_getObj(self, methodPath, 1);
    if (NULL == methodHost)
    {
        /* no found method object */
        res = 1;
        goto exit;
    }
    char *methodName = strsGetLastToken(buffs, methodPath, '.');

    loadMethodInfo(methodHost, methodName, cleanDeclearation, methodPtr);
    res = 0;
    goto exit;
exit:
    args_deinit(buffs);
    return res;
}

char *getDirectStr(char *buff, char *argPath)
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
    if (strEqu(definedType, "any"))
    {
        char *directStr = getDirectStr(args_getBuff(args, 128), argPath);
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
        /* solve the string type */
        char *directStr = getDirectStr(args_getBuff(args, 128), argPath);
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
    Args *buffs = New_strBuff();
    char *typeListBuff = args_getBuff(buffs, 128);
    memcpy(typeListBuff, typeList, strGetSize(typeList));
    char *argListBuff = args_getBuff(buffs, 128);
    memcpy(argListBuff, argList, strGetSize(argList));
    Args *args = New_args(NULL);
    while (1)
    {
        char *typeToken = strPopToken(args_getBuff(buffs, 128), typeListBuff, ',');
        char *argToken = strPopToken(args_getBuff(buffs, 128), argListBuff, ',');
        if ((0 == argToken[0]) || (0 == typeToken[0]))
        {
            /* arg or type poped finised */
            break;
        }

        char *typeName = strGetFirstToken(args_getBuff(buffs, 128), typeToken, ':');
        char *typeVal = getTypeVal(args_getBuff(buffs, 128), typeToken);
        char *argPath = argToken;

        if (0 != loadArgByType(self,
                               typeName,
                               typeVal,
                               argPath,
                               args))
        {
            args_deinit(args);
            args_deinit(buffs);
            return NULL;
        }
    }
    args_deinit(buffs);
    return args;
}

static Args *getArgsByNameMatch(MimiObj *self, char *typeList, char *argList)
{
    Args *buffs = New_strBuff();
    char *typeListBuff = args_getBuff(buffs, 128);
    memcpy(typeListBuff, typeList, strGetSize(typeList));
    Args *args = New_args(NULL);
    while (1)
    {
        char *typeToken = strPopToken(args_getBuff(buffs, 128), typeListBuff, ',');
        /* poped all type from typeList */
        if (0 == typeToken[0])
        {
            break;
        }

        char *typeName = strGetFirstToken(args_getBuff(buffs, 128), typeToken, ':');
        char *typeVal = getTypeVal(args_getBuff(buffs, 128), typeToken);

        char *argListBuff = args_getBuff(buffs, 128);
        memcpy(argListBuff, argList, strGetSize(argList));
        while (1)
        {
            char *argToken = strPopToken(args_getBuff(buffs, 128), argListBuff, ',');
            char *argName = strGetFirstToken(args_getBuff(buffs, 128), argToken, '=');
            char *argVal = strGetLastToken(args_getBuff(buffs, 128), argToken, '=');

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
                args_deinit(buffs);
                return NULL;
            }
        }
    }
    args_deinit(buffs);
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
    if (strEqu(":int", returnType))
    {
        obj_setInt(self, returnName, args_getInt(args, "return"));
    }
    if (strEqu(":float", returnType))
    {
        obj_setFloat(self, returnName, args_getFloat(args, "return"));
    }
    if (strEqu(":string", returnType))
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

Args *obj_run(MimiObj *self, char *cmd)
{
    Args *res = New_args(NULL);
    Args *buffs = New_strBuff();
    int size = strGetSize(cmd);
    char *cleanCmd = strDeleteChar(args_getBuff(buffs, size), cmd, ' ');
    char *methodToken = strGetFirstToken(args_getBuff(buffs, size), cleanCmd, '(');
    char *methodPath = getMethodPath(args_getBuff(buffs, size), methodToken);

    MimiObj *methodHost = obj_getObj(self, methodPath, 1);
    if (NULL == methodHost)
    {
        /* error, not found object */
        args_setInt(res, "errCode", 1);
        goto exit;
    }
    char *methodName = strGetLastToken(args_getBuff(buffs, size), methodPath, '.');
    void (*methodPtr)(MimiObj * self, Args * args) = getMethodPtr(methodHost, methodName);
    char *methodDeclearation = getMethodDeclearation(methodHost, methodName);

    if ((NULL == methodDeclearation) || (NULL == methodPtr))
    {
        /* error, method no found */
        args_setInt(res, "errCode", 2);
        goto exit;
    }

    char *typeList = strCut(args_getBuff(buffs, size), methodDeclearation, '(', ')');
    if (typeList == NULL)
    {
        /* typeList no found */
        args_setInt(res, "errCode", 3);
        goto exit;
    }

    char *argList = strCut(args_getBuff(buffs, size), cleanCmd, '(', ')');
    {
        if (argList == NULL)
        {
            /* argL List no found */
            args_setInt(res, "errCode", 4);
            goto exit;
        }
    }

    char *returnType = strGetLastToken(args_getBuff(buffs, size), methodDeclearation, ')');
    /* get type */
    Args *args = getArgsBySentence(self, typeList, argList);
    if (NULL == args)
    {
        /* get args faild */
        args_setInt(res, "errCode", 5);
        goto exit;
    }
    /* run method */
    methodPtr(methodHost, args);
    /* transfer return */
    if (strIsContain(methodToken, '='))
    {
        char *returnName = strGetFirstToken(args_getBuff(buffs, size), methodToken, '=');
        transferReturnVal(self, returnType, returnName, args);
    }
    char *sysOut = args_getStr(args, "sysOut");
    if (NULL != sysOut)
    {
        args_setStr(res, "sysOut", args_getStr(args, "sysOut"));
    }
    args_deinit(args);
    /* succeed */
    args_setInt(res, "errCode", 0);
    goto exit;
exit:
    args_deinit(buffs);
    return res;
}

int obj_removeArg(MimiObj *self, char *argPath)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    Args *buffs = New_strBuff();
    int err = 0;
    if (NULL == obj)
    {
        /* [error] object no found */
        args_deinit(buffs);
        err = 1;
        goto exit;
    }
    char *argName = strGetLastToken(args_getBuff(buffs, 64), argPath, '.');
    int res = args_removeArg(obj->attributeList, argName);
    if (1 == res)
    {
        /*[error] not found arg*/
        args_deinit(buffs);
        err = 2;
        goto exit;
    }
    goto exit;
exit:
    args_deinit(buffs);
    return err;
}

int obj_isArgExist(MimiObj *self, char *argPath)
{
    MimiObj *obj = obj_getObj(self, argPath, 1);
    Args *buffs = New_strBuff();
    int res = 0;
    if (NULL == obj)
    {
        /* [error] object no found */
        res = 1;
        goto exit;
    }
    char *argName = strGetLastToken(args_getBuff(buffs, 64), argPath, '.');
    Arg *arg = args_getArg(obj->attributeList, argName);
    if (NULL == arg)
    {
        /* no found arg */
        res = 0;
        goto exit;
    }
    /* found arg */
    res = 1;
    goto exit;

exit:
    args_deinit(buffs);
    return res;
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

void obj_runNoRes(MimiObj *slef, char *cmd)
{
    args_deinit(obj_run(slef, cmd));
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
