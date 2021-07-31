#include "sysObj.h"
#include "baseObj.h"
#include "dataStrs.h"
#include "dataMemory.h"
#include "mimiFansInfo.h"
#include "mimiFansList.h"
#include "mimiMailbox.h"
#include "dataString.h"

static void publish(MimiObj *self, Args *args)
{
    char *argPath = args_getStr(args, "argPath");
    args_setInt(args, "errCode", 0);

    MimiObj *publishHost = obj_getObj(self, argPath, 1);
    char *folloedArgName = obj_getStr(publishHost, "fansList.fansInfo.followedArgName");
    char *argName = strsGetLastToken(args, argPath, '.');
    if (!strEqu(argName, folloedArgName))
    {
        method_sysOut(args, "[error] publish: publish arg no found.");
        method_setErrorCode(args, 1);
        return;
    }
    MimiObj *fansPtr = obj_getPtr(publishHost, "fansList.fansInfo.fansPtr");
    void (*handle)(MimiObj * obj, Args * args) = obj_getPtr(publishHost, "fansList.fansInfo.handle");
    Args *argsHandle = New_args(NULL);
    handle(fansPtr, argsHandle);
    args_deinit(argsHandle);
}

static void event_follow(MimiObj *self, char *publishArgPath, void *handle)
{
    Args *buffs = New_args(NULL);
    MimiObj *publishHost = obj_getObj(self, publishArgPath, 1);
    MimiObj *fansInfo = obj_getObj(publishHost, "fansList.fansInfo", 0);
    char *argName = strsGetLastToken(buffs, publishArgPath, '.');

    obj_setPtr(fansInfo, "fansPtr", self);
    obj_setPtr(fansInfo, "handle", handle);
    obj_setStr(fansInfo, "followedArgName", argName);
    args_deinit(buffs);
}

static void followMethod(MimiObj *self, Args *args)
{
    char *argPath = args_getStr(args, "argPath");
    void *handle = args_getPtr(args, "handle");
    args_setInt(args, "errCode", 0);
    event_follow(self, argPath, handle);
}

static void init_Event(MimiObj *self, Args *args)
{
    /* attribute */
    obj_import(self, "FansList", New_MimiObj_FansList);
    obj_import(self, "MailBox", New_MimiObj_Mailbox);
    obj_newObj(self, "fansList", "FansList");
    obj_newObj(self, "mailBox", "MailBox");

    /* method */
    class_defineMethod(self, "follow(argPath:string, handle:pointer)", followMethod);
    class_defineMethod(self, "publish(argPath:string)", publish);
}

MimiObj *New_MimiObj_Event(Args *args)
{
    MimiObj *self = New_MimiObj_sys(args);
    init_Event(self, args);
    return self;
}
