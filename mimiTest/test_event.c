#include "mimiEvent.h"

void handle1(MimiObj *obj)
{
    char *message = obj->getStr(obj, "publisher.message");
    printf("%s\r\n", message);
}

int TEST_Event(int isShow)
{
    {
        MimiObj *fans = New_MimiObj_Event(NULL);
        fans->setObj(fans, "publisher", New_MimiObj_Event);
        fans->setStr(fans, "publisher.message", "hello");
        {
            Args *args = New_args(NULL);
            args->setStr(args, "argDir", "publisher.message");
            args->setPtr(args, "handle", handle1);
            fans->runMethod(fans, "follow", args);
            args->deinit(args);
        }
        {
            Args *args = New_args(NULL);
            args->setStr(args, "argDir", "publisher.message");
            fans->runMethod(fans, "publish", args);
            args->deinit(args);
        }
        fans->deinit(fans);
    }
    return 0;
}
