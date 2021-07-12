#include "mimiEvent.h"

void handle1(MimiObj *obj)
{
    char *message = obj->getStr(obj, "publisher.message");
    printf("%s\r\n", message);
}

int TEST_Event(int isShow)
{
    {
        MimiObj *root = New_MimiObj_Event(NULL);
        root->setObj(root, "publisher", New_MimiObj_Event);
        root->setStr(root, "publisher.message", "hello");
        root->setPtr(root, "handle", handle1);

        root->run(root, "follow(argDir = \"publisher.message\", handle = handle)");
        root->run(root, "publisher.publish(argDir = \"message\")");

        root->deinit(root);
    }
    return 0;
}
