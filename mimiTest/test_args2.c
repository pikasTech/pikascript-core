#include "dataList.h"
int TEST_args2(int isShow)
{
    int err = 0;
    float floatOut = 0;
    long long int64Out = 0;
    void *pointer = NULL;
    char *strOut = NULL;
    list_t *args = New_list(NULL);
    args->pushFloatWithDefaultName(args, 24.5);
    args->pushInt64WithName(args,
                            "int64Test",
                            (long long)22221);
    args->pushPointerWithName(args,
                              "pointerTest",
                              (void *)2222322);
    args->pushStrWithName(args, "strTest", "teeeds");

    floatOut = args->getFloatByIndex(args, 0);
    int64Out = args->getInt64ByName(args, "int64Test");
    pointer = args->getPointerByName(args, "pointerTest");
    args->getStrByName(args, "strTest", &strOut);

    if (isShow)
    {
        printf("the float output is: %f\r\n", floatOut);
        printf("the float type is: [%s]\r\n", args->getTypeByName(args, "arg0"));
        printf("the int64 output is: %d\r\n", (int)int64Out);
        printf("the int64 type is: [%s]\r\n", args->getTypeByName(args, "int64Test"));
        printf("the pointer output is: %d\r\n", (int)(long long)pointer);
        printf("the pointer type is: [%s]\r\n", args->getTypeByName(args, "pointerTest"));
        printf("the string output is: %s\r\n", strOut);
        printf("the string type is: [%s]\r\n", args->getTypeByName(args, "strTest"));
    }

    if ((float)24.5 != floatOut)
    {
        err = 1;
        goto exit;
    }
    if (22221 != (int)int64Out)
    {
        err = 2;
        goto exit;
    }
    if (2222322 != (int)(long long)pointer)
    {
        err = 3;
        goto exit;
    }
    if (0 != strcmp("teeeds", strOut))
    {
        err = 4;
        goto exit;
    }
    if (0 != strcmp("float", args->getTypeByName(args, "arg0")))
    {
        err = 5;
        goto exit;
    }
    if (0 != strcmp("int64", args->getTypeByName(args, "int64Test")))
    {
        err = 6;
        goto exit;
    }
    if (0 != strcmp("pointer", args->getTypeByName(args, "pointerTest")))
    {
        err = 7;
        goto exit;
    }
    if (0 != strcmp("string", args->getTypeByName(args, "strTest")))
    {
        err = 8;
        goto exit;
    }

    goto exit;

exit:
    args->dinit(args);
    return err;
}