#include "mimiEvent.h"

void handle1(MimiObj *obj)
{
    char *message = obj->getStr(obj, "publisher.message");
    printf("%s\r\n", message);
}

int TEST_Event(int isShow)
{
    {
        MimiObj *root = New_MimiObj(NULL);
        root->setObj(root, "fans", New_MimiObj_Event);

        root->setObj(root, "publisher", New_MimiObj_Event);
        root->setStr(root, "publisher.message", "hello");
        {
            Args *args = New_args(NULL);
            args->setStr(args, "argDir", "publisher.message");
            args->setPtr(args, "handle", handle1);
            root->runMethod(root, "fans.follow", args);
            args->deinit(args);
        }
        {
            Args *args = New_args(NULL);
            args->setStr(args, "argDir", "publisher.message");
            root->runMethod(root, "publisher.publish", args);
            args->deinit(args);
        }
        root->deinit(root);
    }
    return 0;
}
