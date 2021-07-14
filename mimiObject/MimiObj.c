#include "MimiObj.h"
#include "dataArgs.h"
#include "dataMemory.h"
#include "dataString.h"
#include "mimiFansList.h"
#include "mimiMailbox.h"

int dinitEachSubObj(Arg *argEach, Args *handleArgs)
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
    args_foreach(args, dinitEachSubObj, NULL);
}

void obj_deinit(MimiObj *self)
{
    self->_beforDinit(self);
    dinitAllSubObj(self);
    args_deinit(self->attributeList);
    DynMemPut(self->mem);
}

void obj_update(MimiObj *self)
{
    // return if is not enable
    if (0 == obj_getInt(self, "isEnable"))
    {
        return;
    }
    self->_updateHandle(self);
}
void _UpdateHandle(MimiObj *self)
{
    // override the handle function here
}

void obj_enable(MimiObj *self)
{
    obj_setInt(self, "isEnable", 1);
}

void obj_disable(MimiObj *self)
{
    obj_setInt(self, "isEnable", 0);
}

void obj_setInt(MimiObj *self, char *argDir, long long val)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    if (NULL == obj)
    {
        printf("[error] object no found: %s\r\n, argDir", argDir);
        return;
    }
    char name[64] = {0};
    strGetLastToken(name, argDir, '.');
    args_setInt(obj->attributeList, name, val);
}

void obj_setPtr(MimiObj *self, char *argDir, void *pointer)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    if (NULL == obj)
    {
        printf("[error] object no found: %s\r\n", argDir);
        return;
    }
    char name[64] = {0};
    strGetLastToken(name, argDir, '.');
    args_setPtr(obj->attributeList,
                name, pointer);
}

void obj_setFloat(MimiObj *self, char *argDir, float value)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    if (NULL == obj)
    {
        printf("[error] object no found: %s\r\n", argDir);
        return;
    }
    char name[64] = {0};
    strGetLastToken(name, argDir, '.');
    args_setFloat(obj->attributeList,
                  name, value);
}

void obj_setStr(MimiObj *self, char *argDir, char *str)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    if (NULL == obj)
    {
        printf("[error] object no found: %s\r\n", argDir);
        return;
    }
    char name[64] = {0};
    strGetLastToken(name, argDir, '.');
    args_setStr(obj->attributeList,
                name, str);
}

long long obj_getInt(MimiObj *self, char *argDir)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    if (NULL == obj)
    {
        return -999999999;
    }
    char argName[64] = {0};
    strGetLastToken(argName, argDir, '.');
    return args_getInt(obj->attributeList,
                       argName);
}

void *obj_getPtr(MimiObj *self, char *argDir)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    if (NULL == obj)
    {
        return NULL;
    }
    char argName[64] = {0};
    strGetLastToken(argName, argDir, '.');
    return args_getPtr(obj->attributeList, argName);
}

float obj_getFloat(MimiObj *self, char *argDir)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    if (NULL == obj)
    {
        return -999.999;
    }
    char argName[64] = {0};
    strGetLastToken(argName, argDir, '.');
    return args_getFloat(obj->attributeList,
                         argName);
}

char *obj_getStr(MimiObj *self, char *argDir)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    if (NULL == obj)
    {
        return NULL;
    }
    char argName[64] = {0};
    strGetLastToken(argName, argDir, '.');
    return args_getStr(obj->attributeList,
                       argName);
}

void obj_load(MimiObj *self, Args *args, char *name)
{
    args_copyArg(args, name, self->attributeList);
}

void _beforDinit(MimiObj *self)
{
    /* override in user code */
}

void obj_newObj(MimiObj *self, char *objName, void *newFun)
{
    /* class means subprocess init */
    char prifix[] = "[cls]";
    char className[64] = {0};
    strAppend(className, prifix);
    strAppend(className, objName);
    obj_setPtr(self, className, newFun);
    /* add void process Ptr, no inited */
    args_setPtrWithType(self->attributeList, objName, "process", NULL);
}

void obj_addOther(MimiObj *self, char *subObjectName, void *new_ObjectFun)
{
    Args *initArgs = New_args(NULL);
    args_setPtr(initArgs, "context", self);
    void *(*new_Object)(Args * initArgs) = (void *(*)(Args *))new_ObjectFun;
    void *subObject = new_Object(initArgs);
    obj_setPtr(self, subObjectName, subObject);
    args_deinit(initArgs);
}

void obj_freeObj(MimiObj *self, char *subProcessName)
{
    MimiObj *subProcess = obj_getPtr(self, subProcessName);
    obj_deinit(subProcess);
}

void obj_bind(MimiObj *self, char *type, char *name, void *pointer)
{
    args_bind(self->attributeList, type, name, pointer);
}

char *obj_print(MimiObj *self, char *name)
{
    return args_print(self->attributeList, name);
}

void obj_bindInt(MimiObj *self, char *name, int *valPtr)
{
    args_bindInt(self->attributeList, name, valPtr);
}

void obj_bindFloat(MimiObj *self, char *name, float *valPtr)
{
    args_bindFloat(self->attributeList, name, valPtr);
}

void obj_bindString(MimiObj *self, char *name, char **valPtr)
{
    args_bindStr(self->attributeList, name, valPtr);
}

int obj_set(MimiObj *self, char *argDir, char *valStr)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    if (NULL == obj)
    {
        return -99999999;
    }
    char argName[64] = {0};
    strGetLastToken(argName, argDir, '.');
    return args_set(obj->attributeList, argName, valStr);
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
    char classDir[64] = {0};
    strAppend(classDir, prifix);
    strAppend(classDir, name);
    /* init the subprocess */
    void *(*newObjFun)(Args * initArgs) = args_getPtr(self->attributeList, classDir);
    if (NULL == newObjFun)
    {
        printf("[error] no such object: %s\r\n", name);
        return NULL;
    }
    newObjDirect(self, name, newObjFun);
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

MimiObj *obj_getObj(MimiObj *self, char *processDirectory, int keepToken)
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
    int processArgc = strGetToken(processDirectory, token, '.');
    for (int i = 0; i < processArgc - keepToken; i++)
    {
        obj = obj_getObjDirect(obj, token[i]);
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
    char *methodPtrDir = strAppend(strAppend(buff[i++], "[methodPtr]"), methodName);
    char *methodDeclearationDir = strAppend(strAppend(buff[i++], "[methodDec]"), methodName);
    obj_setPtr(methodHost, methodPtrDir, methodPtr);
    obj_setStr(methodHost, methodDeclearationDir, methodDeclearation);
}

static char *getMethodDeclearation(MimiObj *methodHost, char *methodName)
{
    char buff[1][128] = {0};
    int i = 0;
    char *methodDeclearationDir = strAppend(strAppend(buff[i++], "[methodDec]"), methodName);
    return obj_getStr(methodHost, methodDeclearationDir);
}

static void *getMethodPtr(MimiObj *methodHost, char *methodName)
{
    char buff[1][128] = {0};
    int i = 0;
    char *methodPtrDir = strAppend(strAppend(buff[i++], "[methodPtr]"), methodName);
    return obj_getPtr(methodHost, methodPtrDir);
}

void obj_defineMethod(MimiObj *self,
                      char *declearation,
                      void (*methodPtr)(MimiObj *self, Args *args))
{
    char buff[3][128] = {0};
    int i = 0;
    char *cleanDeclearation = strDeleteChar(buff[i++], declearation, ' ');
    char *methodDir = strGetFirstToken(buff[i++], cleanDeclearation, '(');

    MimiObj *methodHost = obj_getObj(self, methodDir, 1);
    if (NULL == methodHost)
    {
        printf("[error] object direction no found, method declearation: %s\r\n", methodDir);
        return;
    }
    char *methodName = strGetLastToken(buff[i++], methodDir, '.');

    loadMethodInfo(methodHost, methodName, cleanDeclearation, methodPtr);
}

static void loadArgByType(MimiObj *self,
                          char *typeName,
                          char *typeVal,
                          char *argVal,
                          Args *args)
{
    char buff[2][128] = {0};
    int i = 0;
    if (strEqu(typeVal, "string"))
    {
        /* solve the string type */
        char *directStr = strCut(buff[i++], argVal, '"', '"');
        if (NULL != directStr)
        {
            /* direct value */
            args_setStr(args, typeName, directStr);
            return;
        }
        /* reference value */
        char *refStr = obj_getStr(self, argVal);
        if (NULL == refStr)
        {
            printf("[error] can not get string from reference: %s\r\n", argVal);
            return;
        }
        args_setStr(args, typeName, refStr);
        return;
    }
    if (strEqu(typeVal, "int"))
    {
        /* solve the int type */
        args_setInt(args, typeName, 0);
        if ((argVal[0] >= '0') && (argVal[0] <= '9'))
        {
            /* direct value */
            args_set(args, typeName, argVal);
            return;
        }
        /* reference value */
        int referenceVal = obj_getInt(self, argVal);
        args_setInt(args, typeName, referenceVal);
        return;
    }
    if (strEqu(typeVal, "float"))
    {
        /* solve the float type */
        args_setFloat(args, typeName, 0);
        if ((argVal[0] >= '0') && (argVal[0] <= '9'))
        {
            /* direct value */
            args_set(args, typeName, argVal);
            return;
        }
        /* reference value */
        float referenceVal = obj_getFloat(self, argVal);
        args_setFloat(args, typeName, referenceVal);
        return;
    }
    if (strEqu(typeVal, "pointer"))
    {
        /* only support reference value */
        void *ptr = obj_getPtr(self, argVal);
        args_setPtr(args, typeName, ptr);
        return;
    }
    /* type match faild */
    printf("[error] type not match, input type: %s\r\n", typeVal);
    while (1)
        ;
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
        char *typeVal = strGetLastToken(buff[i++], typeToken, ':');
        char *argVal = argToken;

        loadArgByType(self,
                      typeName,
                      typeVal,
                      argVal,
                      args);
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
        char *typeVal = strGetLastToken(buff[i++], typeToken, ':');

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

            loadArgByType(self,
                          typeName,
                          typeVal,
                          argVal,
                          args);
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

char *getMethodDir(char *buff, char *methodToken)
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

void obj_run(MimiObj *self, char *cmd)
{
    char buff[8][128] = {0};
    int i = 0;
    char *cleanCmd = strDeleteChar(buff[i++], cmd, ' ');
    char *methodToken = strGetFirstToken(buff[i++], cleanCmd, '(');
    char *methodDir = getMethodDir(buff[i++], methodToken);

    MimiObj *methodHost = obj_getObj(self, methodDir, 1);
    if (NULL == methodHost)
    {
        printf("[error] object direction no found, method declearation: %s\r\n", methodDir);
        return;
    }
    char *methodName = strGetLastToken(buff[i++], methodDir, '.');
    void (*methodPtr)(MimiObj * self, Args * args) = getMethodPtr(methodHost, methodName);
    char *methodDeclearation = getMethodDeclearation(methodHost, methodName);

    if ((NULL == methodDeclearation) || (NULL == methodPtr))
    {
        printf("[error] object '%s' don't have method '%s'.\r\n", methodHost->name, methodName);
        return;
    }

    char *typeList = strCut(buff[i++], methodDeclearation, '(', ')');
    if (typeList == NULL)
    {
        printf("[error] method declearation error!\r\n");
        printf("[info]: declearation: %s\r\n", methodDeclearation);
        return;
    }

    char *argList = strCut(buff[i++], cleanCmd, '(', ')');
    {
        if (argList == NULL)
        {
            printf("[error] method used error, method input: '%s'\r\n", cleanCmd);
            return;
        }
    }

    char *returnType = strGetLastToken(buff[i++], methodDeclearation, ')');
    /* get type */
    Args *args = getArgsBySentence(self, typeList, argList);
    /* run method */
    methodPtr(methodHost, args);
    /* transfer return */
    if (strIsContain(methodToken, '='))
    {
        char *returnName = strGetFirstToken(buff[i++], methodToken, '=');
        transferReturnVal(self, returnType, returnName, args);
    }
    args_deinit(args);
}

void obj_init(MimiObj *self, Args *args)
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
    MimiObj *self = mem->addr;
    self->mem = mem;
    obj_init(self, args);
    return self;
}
