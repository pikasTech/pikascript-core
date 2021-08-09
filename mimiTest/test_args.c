#include "dataArgs.h"
#include "dataString.h"
int TEST_args2(int isShow)
{
    int err = 0;
    {
        Args *args = New_args(NULL);
        float floatOut = 0;
        long long int64Out = 0;
        void *pointer = NULL;
        char *strOut = NULL;
        args_setFloatWithDefaultName(args, 24.5);
        args_setInt(args,
                     "int64Test",
                     (long long)22221);
        args_setPtr(args,
                     "pointerTest",
                     (void *)2222322);
        args_setStr(args, "strTest", "teeeds");

        floatOut = args_getFloatByIndex(args, 0);
        int64Out = args_getInt(args, "int64Test");
        pointer = args_getPtr(args, "pointerTest");
        strOut = args_getStr(args, "strTest");

        if (isShow)
        {
            printf("the float output is: %f\r\n", floatOut);
            printf("the float type is: [%s]\r\n", args_getType(args, "arg1"));
            printf("the int64 output is: %d\r\n", (int)int64Out);
            printf("the int64 type is: [%s]\r\n", args_getType(args, "int64Test"));
            printf("the pointer output is: %d\r\n", (int)(long long)pointer);
            printf("the pointer type is: [%s]\r\n", args_getType(args, "pointerTest"));
            printf("the string output is: %s\r\n", strOut);
            printf("the string type is: [%s]\r\n", args_getType(args, "strTest"));
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
        if (0 != strcmp("int", args_getType(args, "int64Test")))
        {
            err = 6;
        }
        if (0 != strcmp("pointer", args_getType(args, "pointerTest")))
        {
            err = 7;
        }
        if (0 != strcmp("str", args_getType(args, "strTest")))
        {
            err = 8;
        }
        args_deinit(args);
    }
    {
        Args *args1 = New_args(NULL);
        Args *args2 = New_args(NULL);

        args_setFloat(args1, "argtest1", 2.883);
        args_setStr(args1, "strtest1", "teststr");
        args_copyArgByName(args1, "argtest1", args2);
        args_copyArgByName(args1, "strtest1", args2);

        if (isShow)
        {
            printf("the float get from args is:%f\r\n",
                   args_getFloat(args1, "argtest1"));

            printf("the float copyed from arg1:%f\r\n",
                   args_getFloat(args2, "argtest1"));
        }
        if (2.8830f != args_getFloat(args1, "argtest1"))
        {
            err = 9;
        }

        args_deinit(args1);
        args_deinit(args2);
    }
    {
        int testint = 12333;
        Args *args = New_args(NULL);
        args_setInt(args, "testint", testint);
        if (isShow)
        {
            printf("args print test int: %s\r\n", args_print(args, "testint"));
        }
        args_deinit(args);
    }
    {
        int testInt = 124;
        Args *args = New_args(NULL);
        args_bind(args, "int", "testInt", &testInt);
        char *type = args_getType(args, "testInt");
        args_print(args, "testInt");
        if (isShow)
        {
            printf("arg bind type: %s\r\n", type);
            printf("arg bind print: %s\r\n", args_print(args, "testInt"));
        }
        if (!strEqu("124", args_print(args, "testInt")))
        {
            err = 21;
            goto exit;
        }
        args_deinit(args);
    }
    {
        Args *args = New_args(NULL);
        args_setFloat(args, "testfloat", 1.42);
        if (isShow)
        {
            printf("test arg print for float:%s\r\n", args_print(args, "testfloat"));
        }
        if (!strEqu("1.420000", args_print(args, "testfloat")))
        {
            err = 22;
            goto exit;
        }
        args_deinit(args);
    }
    {
        Args *args = New_args(NULL);
        float floatBindTest = 2.314;
        args_bind(args, "float", "floatBind", &floatBindTest);
        if (isShow)
        {
            printf("test float bind: %s\r\n", args_print(args, "floatBind"));
        }
        if (!strEqu("2.314000", args_print(args, "floatBind")))
        {
            err = 24;
            goto exit;
        }
        args_deinit(args);
    }
    {
        Args *args = New_args(NULL);
        args_setStr(args, "testString", "test string print");
        if (isShow)
        {
            printf("test string bind: %s\r\n", args_print(args, "testString"));
        }
        if (!strEqu("test string print", args_print(args, "testString")))
        {
            err = 25;
            goto exit;
        }
        args_deinit(args);
    }
    {
        Args *args = New_args(NULL);
        char strBindTest[] = "test string bind";
        args_bindStr(args, "testStringBind", (char **)&strBindTest);
        if (isShow)
        {
            printf("test string bind: %s\r\n", args_print(args, "testStringBind"));
        }
        if (!strEqu("test string bind", args_print(args, "testStringBind")))
        {
            err = 26;
            goto exit;
        }
        args_deinit(args);
    }
    {
        Args *args = New_args(NULL);
        args_setInt(args, "testInt", 1);
        char *printOutBefore = args_print(args, "testInt");
        if (isShow)
        {
            printf("test set int before: %s\r\n", printOutBefore);
        }
        if (!strEqu(printOutBefore, "1"))
        {
            return 14;
        }
        args_set(args, "testInt", "4");
        char *printOutAfter = args_print(args, "testInt");
        if (isShow)
        {
            printf("test set int after: %s\r\n", printOutAfter);
        }
        if (!strEqu(printOutAfter, "4"))
        {
            return 55;
        }
        args_deinit(args);
    }
    {
        Args *args = New_args(NULL);
        char testStrBind[256] = "testtest";
        args_bindStr(args, "testStr", (char **)&testStrBind);
        char *printOutBefore = args_print(args, "testStr");
        if (isShow)
        {
            printf("test set bind str before: %s\r\n", printOutBefore);
        }
        if (!strEqu(printOutBefore, "testtest"))
        {
            return 15;
        }
        args_set(args, "testStr", "ttww");
        char *printOutAfter = args_print(args, "testStr");
        if (isShow)
        {
            printf("test set bind str after: %s\r\n", printOutAfter);
        }
        if (!strEqu(printOutAfter, "ttww"))
        {
            return 56;
        }
        args_deinit(args);
    }
    {
        Args *args = New_args(NULL);
        args_setInt(args, "test", 13);
        args_setInt(args, "kw", 25);
        if (isShow)
        {
            printf("before remove arg\r\n");
            printf("the size of args is %d\r\n", args_getSize(args));
            printf("the val of arg test is %d\r\n", (int)args_getInt(args, "test"));
        }
        args_removeArg(args, "test");
        if (isShow)
        {
            printf("before remove arg\r\n");
            printf("the size of args is %d\r\n", args_getSize(args));
            printf("the val of arg test is %d\r\n", (int)args_getInt(args, "test"));
        }
        args_deinit(args);
    }

    goto exit;

exit:
    return err;
}