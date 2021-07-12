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
    if (mimiStrEqu(argEach->typeDynMem->addr, "_class-process"))
    {
        MimiObj *subObj = arg_getPtr(argEach);
        obj_deinit(subObj);
    }
    return 0;
}

void dinitAllSubObj(MimiObj *self)
{
    Args *args = self->attributeList;
    args_foreach (args, dinitEachSubObj, NULL);
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
void RootUpdateHandle(MimiObj *self)
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
    char name[64] = {0};
    getLastToken(name, argDir, '.');
    args_setInt(obj->attributeList,
                               name, val);
}

void obj_setPtr(MimiObj *self, char *argDir, void *pointer)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    char name[64] = {0};
    getLastToken(name, argDir, '.');
    args_setPtr(obj->attributeList,
                               name, pointer);
}

void obj_setFloat(MimiObj *self, char *argDir, float value)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    char name[64] = {0};
    getLastToken(name, argDir, '.');
    args_setFloat(obj->attributeList,
                                 name, value);
}

void obj_setStr(MimiObj *self, char *argDir, char *str)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    char name[64] = {0};
    getLastToken(name, argDir, '.');
    args_setStr(obj->attributeList,
                               name, str);
}

long long obj_getInt(MimiObj *self, char *argDir)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
    return args_getInt(obj->attributeList,
                                      argName);
}

void *obj_getPtr(MimiObj *self, char *argDir)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
    return args_getPtr(obj->attributeList,
                                      argName);
}

float obj_getFloat(MimiObj *self, char *argDir)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
    return args_getFloat(obj->attributeList,
                                        argName);
}

char *obj_getStr(MimiObj *self, char *argDir)
{
    MimiObj *obj = obj_getObj(self, argDir, 1);
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
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

void obj_setObj(MimiObj *self, char *subProcessName, void *newFun)
{
    /* class means subprocess init */
    char prifix[] = "[cls]";
    char nameBuff[64] = {0};
    strAppend(nameBuff, prifix);
    strAppend(nameBuff, subProcessName);
    obj_setPtr(self, nameBuff, newFun);
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
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
    return args_set(obj->attributeList, argName, valStr);
}

MimiObj *initObj(MimiObj *self, char *name)
{
    char prifix[] = "[cls]";
    char initFunName[64] = {0};
    strAppend(initFunName, prifix);
    strAppend(initFunName, name);
    if (!args_isArgExist(self->attributeList,
                                         initFunName))
    {
        /* no init fun, could not found subprocess */
        return NULL;
    }
    /* init the subprocess */
    Args *attributeList = self->attributeList;
    Args *initArgs = New_args(NULL);
    args_setPtr(initArgs, "context", self);
    args_setStr(initArgs, "name", name);
    void *(*newObjFun)(Args * initArgs) = (void *(*)(Args * initArgs)) args_getPtr(self->attributeList,
                                                                                                   initFunName);
    MimiObj *newObj = newObjFun(initArgs);
    args_setPtrWithType(attributeList, name, "process", newObj);
    args_deinit(initArgs);
    return obj_getPtr(self,
                        name);
}

MimiObj *obj_getDirectObj(MimiObj *self, char *name)
{
    /* check subprocess */
    if (!args_isArgExist(self->attributeList,
                                         name))
    {
        /* no inited subprocess, check subprocess init fun*/
        return initObj(self, name);
    }

    /* finded subscribe, check type*/
    char *type = args_getType(self->attributeList,
                                              name);
    if (!mimiStrEqu("_class-process", type))
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
    int processArgc = getToken(processDirectory, token, '.');
    for (int i = 0; i < processArgc - keepToken; i++)
    {
        obj = obj_getDirectObj(obj, token[i]);
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


void obj_setMethod(MimiObj *self,
                      char *declearation,
                      void (*methodPtr)(MimiObj *self, Args *args))
{
    char buff[5][128] = {0};
    int i = 0;
    char *cleanDeclearation = strDeleteChar(buff[i++], declearation, ' ');
    char *methodDir = getFirstToken(buff[i++], cleanDeclearation, '(');
    char *returnType = getLastToken(buff[i++], cleanDeclearation, ':');
    char *typeList = strCut(buff[i++], cleanDeclearation, '(', ')');
    if (typeList == NULL || methodDir == NULL)
    {
        printf("[error]: method declearation error!\r\n");
        printf("[info]: declearation: %s\r\n", declearation);
        while (1)
        {
            /* code */
        }
    }

    MimiObj *methodHost = obj_getObj(self, methodDir, 1);
    char *methodName = getLastToken(buff[i++], methodDir, '.');
    obj_setObj(methodHost, methodName, New_MimiObj);
    MimiObj *methodObj = obj_getObj(self, methodDir, 0);

    obj_setStr(methodObj, "typeList", typeList);
    obj_setStr(methodObj, "returnType", returnType);
    obj_setPtr(methodObj, "methodPtr", methodPtr);
}

Args *getArgsBySentence(MimiObj *self, char *typeList, char *argList)
{
    Args *args = New_args(NULL);
    char typeListBuff[128] = {0};
    memcpy(typeListBuff, typeList, strGetSize(typeList));
    while (1)
    {
        char buff[4][128] = {0};
        int i = 0;
        char *type = popToken(buff[i++], typeListBuff, ',');
        char *defineName = getFirstToken(buff[i++], type, ':');
        char *defineType = getLastToken(buff[i++], type, ':');

        char *argListBuff = buff[i++];
        memcpy(argListBuff, argList, strGetSize(argList));
        while (1)
        {

            char buff[4][64] = {0};
            int i = 0;
            char *arg = popToken(buff[i++], argListBuff, ',');
            char *argName = getFirstToken(buff[i++], arg, '=');
            char *argContant = getLastToken(buff[i++], arg, '=');

            if (0 == arg[0])
            {
                /* arg poped finised */
                break;
            }

            if (!mimiStrEqu(defineName, argName))
            {
                /* name not match */
                continue;
            }

            if (mimiStrEqu(defineType, "string"))
            {
                /* solve the string type */
                char *directStr = strCut(buff[i++], argContant, '"', '"');
                if (NULL != directStr)
                {
                    /* direct value */
                    args_setStr(args, defineName, directStr);
                    continue;
                }
                /* reference value */
                args_setStr(args, defineName, obj_getStr(self, argContant));
                continue;
            }
            if (mimiStrEqu(defineType, "int"))
            {
                /* solve the int type */
                args_setInt(args, defineName, 0);
                if ((argContant[0] >= '0') && (argContant[0] <= '9'))
                {
                    /* direct value */
                    args_set(args, defineName, argContant);
                    continue;
                }
                /* reference value */
                int referenceVal = obj_getInt(self, argContant);
                args_setInt(args, defineName, referenceVal);
                continue;
            }
            if (mimiStrEqu(defineType, "float"))
            {
                /* solve the float type */
                args_setFloat(args, defineName, 0);
                if ((argContant[0] >= '0') && (argContant[0] <= '9'))
                {
                    /* direct value */
                    args_set(args, defineName, argContant);
                    continue;
                }
                /* reference value */
                float referenceVal = obj_getFloat(self, argContant);
                args_setFloat(args, defineName, referenceVal);
                continue;
            }
            if (mimiStrEqu(defineType, "pointer"))
            {
                /* only support reference value */
                void *ptr = obj_getPtr(self, argContant);
                args_setPtr(args, defineName, ptr);
                continue;
            }

            /* type match faild */
            printf("[error]: type not match, input type: %s\r\n", defineType);
            while (1)
            {
            }
        }

        /* poped all type from typeList */
        if (0 == typeListBuff[0])
        {
            break;
        }
    }

    return args;
}

void obj_run(MimiObj *self, char *cmd)
{
    char buff[5][128] = {0};
    int i = 0;
    char *cleanCmd = strDeleteChar(buff[i++], cmd, ' ');
    char *methodSentence = getFirstToken(buff[i++], cleanCmd, '(');
    char *returnName = getFirstToken(buff[i++], methodSentence, '=');
    char *methodName = getLastToken(buff[i++], methodSentence, '=');

    MimiObj *methodObj = obj_getObj(self, methodName, 0);
    MimiObj *methodHost = obj_getObj(self, methodName, 1);

    if (NULL == methodObj)
    {
        printf("[error]: method %s no found.\r\n", methodName);
        return;
    }
    char *argList = strCut(buff[i++], cleanCmd, '(', ')');
    char *typeList = obj_getStr(methodObj, "typeList");
    char *returnType = obj_getStr(methodObj, "returnType");
    void (*methodFun)(MimiObj * self, Args * args) = obj_getPtr(methodObj, "methodPtr");

    Args *args = getArgsBySentence(self, typeList, argList);
    methodFun(methodHost, args);
    if (mimiStrEqu("int", returnType))
    {
        obj_setInt(self, returnName, args_getInt(args, "return"));
    }
    if (mimiStrEqu("float", returnType))
    {
        obj_setFloat(self, returnName, args_getFloat(args, "return"));
    }
    if (mimiStrEqu("string", returnType))
    {
        obj_setStr(self, returnName, args_getStr(args, "return"));
    }
    args_deinit(args);
}

void obj_init(MimiObj *self, Args *args)
{
    /* List */
    self->attributeList = New_args(NULL);


    /* override */
    self->_updateHandle = RootUpdateHandle;
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
        printf("[error]: memory is empty!");
        while (1)
        {
        }
    }
    MimiObj *self = mem->addr;
    self->mem = mem;
    obj_init(self, args);
    return self;
}
