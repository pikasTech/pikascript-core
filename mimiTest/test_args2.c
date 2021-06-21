#include "dataArgs.h"
int TEST_args2(int isShow)
{
    int err = 0;
    {
        args_t *args = New_args(NULL);
        float floatOut = 0;
        long long int64Out = 0;
        void *pointer = NULL;
        char *strOut = NULL;
        args->setFloatWithDefaultName(args, 24.5);
        args->setInt(args,
                                "int64Test",
                                (long long)22221);
        args->setPoi(args,
                                  "pointerTest",
                                  (void *)2222322);
        args->setStr(args, "strTest", "teeeds");

        floatOut = args->getFloatByIndex(args, 0);
        int64Out = args->getInt(args, "int64Test");
        pointer = args->getPoi(args, "pointerTest");
        strOut = args->getStr(args, "strTest");

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
        }
        if (22221 != (int)int64Out)
        {
            err = 2;
        }
        if (2222322 != (int)(long long)pointer)
        {
            err = 3;
        }
        if (0 != strcmp("teeeds", strOut))
        {
            err = 4;
        }
        if (0 != strcmp("float", args->getTypeByName(args, "arg0")))
        {
            err = 5;
        }
        if (0 != strcmp("int64", args->getTypeByName(args, "int64Test")))
        {
            err = 6;
        }
        if (0 != strcmp("pointer", args->getTypeByName(args, "pointerTest")))
        {
            err = 7;
        }
        if (0 != strcmp("string", args->getTypeByName(args, "strTest")))
        {
            err = 8;
        }
        args->dinit(args);
    }
    {
        args_t *args1 = New_args(NULL);
        args_t *args2 = New_args(NULL);

        args1->setFlt(args1, "argtest1", 2.883);
        args1->setStr(args1, "strtest1", "teststr");
        args1->copyArg(args1, "argtest1", args2);
        args1->copyArg(args1, "strtest1", args2);

        if (isShow)
        {
            printf("the float get from args is:%f\r\n",
                   args1->getFlt(args1, "argtest1"));

            printf("the float copyed from arg1:%f\r\n",
                   args2->getFlt(args2, "argtest1"));
        }
        if (2.8830f != args1->getFlt(args1, "argtest1"))
        {
            err = 9;
        }

        args1->dinit(args1);
        args2->dinit(args2);
    }

    goto exit;

exit:
    return err;
}