#include "mimiEvent.h"

void handle1(MimiObj *obj)
{
    char *message = obj_getStr(obj, "publisher.message");
    printf("%s\r\n", message);
}

int TEST_Event(int isShow)
{
    {
        MimiObj *root = New_MimiObj_Event(NULL);
        obj_setObj(root, "publisher", New_MimiObj_Event);
        obj_setStr(root, "publisher.message", "hello");
        obj_setPtr(root, "handle", handle1);

        obj_run(root, "follow(argDir = \"publisher.message\", handle = handle)");
        obj_run(root, "publisher.publish(argDir = \"message\")");

        obj_deinit(root);
    }
    return 0;
}
