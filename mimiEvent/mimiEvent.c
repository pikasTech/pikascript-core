#include "MimiObj.h"
#include "dataMemory.h"
#include "mimiFansInfo.h"
#include "mimiFansList.h"
#include "mimiMailbox.h"
#include "dataString.h"

static void publish(MimiObj *self, Args *args)
{
    char *argDir = args->getStr(args, "argDir");

    MimiObj *publisher = self->getObj(self, argDir, 1);
    char *folloedArgName = publisher->getStr(publisher, "fansList.fansInfo.followedArgName");
    char argName[64] = {0};
    getLastToken(argName, argDir, '.');
    if (!mimiStrEqu(argName, folloedArgName))
    {
        return;
    }
    MimiObj *fans = publisher->getPtr(publisher, "fansList.fansInfo.fansPtr");
    void (*handle)(MimiObj * obj) = publisher->getPtr(publisher, "fansList.fansInfo.handle");
    handle(fans);
}

static void follow(MimiObj *self, Args *args)
{
    char *argDir = args->getStr(args, "argDir");
    void *handle = args->getPtr(args, "handle");

    MimiObj *publisher = self->getObj(self, argDir, 1);
    MimiObj *fansInfo = publisher->getObj(publisher, "fansList.fansInfo", 0);
    char argName[64];
    getLastToken(argName, argDir, '.');

    fansInfo->setPtr(fansInfo, "fansPtr", self);
    fansInfo->setPtr(fansInfo, "handle", handle);
    fansInfo->setStr(fansInfo, "followedArgName", argName);
}

static void init_Event(MimiObj *self, Args *args)
{
    /* attrivute */
    self->setObj(self, "fansList", New_MimiObj_FansList);
    self->setObj(self, "mailBox", New_MimiObj_Mailbox);

    /* method */
    self->setMethod(self, "follow()", follow);
    self->setMethod(self, "publish()", publish);
}

MimiObj *New_MimiObj_Event(Args *args)
{
    MimiObj *self = New_MimiObj(args);
    self->init = init_Event;
    self->init(self, args);
    return self;
}