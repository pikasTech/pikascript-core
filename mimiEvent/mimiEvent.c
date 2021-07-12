#include "MimiObj.h"
#include "dataMemory.h"
#include "mimiFansInfo.h"
#include "mimiFansList.h"
#include "mimiMailbox.h"
#include "dataString.h"

static void publish(MimiObj *self, Args *args)
{
    char *argDir = args_getStr(args, "argDir");

    MimiObj *publisher = obj_getObj(self, argDir, 1);
    char *folloedArgName = obj_getStr(publisher, "fansList.fansInfo.followedArgName");
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
    if (!mimiStrEqu(argName, folloedArgName))
    {
        return;
    }
    MimiObj *fans = obj_getPtr(publisher, "fansList.fansInfo.fansPtr");
    void (*handle)(MimiObj * obj) = obj_getPtr(publisher, "fansList.fansInfo.handle");
    handle(fans);
}

static void follow(MimiObj *self, Args *args)
{
    char *argDir = args_getStr(args, "argDir");
    void *handle = args_getPtr(args, "handle");

    MimiObj *publisher = obj_getObj(self, argDir, 1);
    MimiObj *fansInfo = obj_getObj(publisher, "fansList.fansInfo", 0);
    char argName[64];
    getLastToken(argName, argDir, '.');

    obj_setPtr(fansInfo, "fansPtr", self);
    obj_setPtr(fansInfo, "handle", handle);
    obj_setStr(fansInfo, "followedArgName", argName);
}

static void init_Event(MimiObj *self, Args *args)
{
    /* attrivute */
    obj_setObj(self, "fansList", New_MimiObj_FansList);
    obj_setObj(self, "mailBox", New_MimiObj_Mailbox);

    /* method */
    obj_setMethod(self, "follow(argDir:string, handle:pointer)", follow);
    obj_setMethod(self, "publish(argDir:string)", publish);
}

MimiObj *New_MimiObj_Event(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    init_Event(self, args);
    return self;
}