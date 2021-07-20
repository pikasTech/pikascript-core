#include "MimiObj.h"
#include "baseObj.h"
#include "strArgs.h"
#include "dataMemory.h"
#include "mimiFansInfo.h"
#include "mimiFansList.h"
#include "mimiMailbox.h"
#include "dataString.h"

static void publish(MimiObj *self, Args *args)
{
    char *argPath = args_getStr(args, "argPath");
    args_setInt(args, "errCode", 0);

    MimiObj *publisher = obj_getObj(self, argPath, 1);
    char *folloedArgName = obj_getStr(publisher, "fansList.fansInfo.followedArgName");
    char *argName = args_getBuff(args, 256);
    strGetLastToken(argName, argPath, '.');
    if (!strEqu(argName, folloedArgName))
    {
        return;
    }
    MimiObj *fans = obj_getPtr(publisher, "fansList.fansInfo.fansPtr");
    void (*handle)(MimiObj * obj, Args *args) = obj_getPtr(publisher, "fansList.fansInfo.handle");
    Args *argsHandle = New_args(NULL);
    handle(fans, argsHandle);
    args_deinit(argsHandle);
}

static void follow(MimiObj *self, Args *args)
{
    char *argPath = args_getStr(args, "argPath");
    void *handle = args_getPtr(args, "handle");
    args_setInt(args, "errCode", 0);

    MimiObj *publisher = obj_getObj(self, argPath, 1);
    MimiObj *fansInfo = obj_getObj(publisher, "fansList.fansInfo", 0);
    char *argName = args_getBuff(args, 256);
    strGetLastToken(argName, argPath, '.');

    obj_setPtr(fansInfo, "fansPtr", self);
    obj_setPtr(fansInfo, "handle", handle);
    obj_setStr(fansInfo, "followedArgName", argName);
}

static void init_Event(MimiObj *self, Args *args)
{
    /* attribute */
    obj_setObj(self, "fansList", New_MimiObj_FansList);
    obj_setObj(self, "mailBox", New_MimiObj_Mailbox);

    /* method */
    obj_defineMethod(self, "follow(argPath:string, handle:pointer)", follow);
    obj_defineMethod(self, "publish(argPath:string)", publish);
}

MimiObj *New_MimiObj_Event(Args *args)
{
    MimiObj *self = New_baseObj(args);
    init_Event(self, args);
    return self;
}
