#include "sysObj.h"
#include "mimiCom.h"
#include "baseObj.h"

void testFloat(MimiObj *obj, Args *args)
{
    float val1 = args_getFloat(args, "val1");
    float val2 = args_getFloat(args, "val2");
    int isShow = args_getInt(args, "isShow");
    if (isShow)
    {
        printf("the float val1 is: %f\r\n", val1);
        printf("the float val2 is: %f\r\n", val2);
    }
    method_returnFloat(args, val1 + val2);
}

void hello2(MimiObj *obj, Args *args)
{
    char *name1 = args_getStr(args, "name1");
    char *name2 = args_getStr(args, "name2");
    char *name3 = args_getStr(args, "name3");
    char *myName = obj->name;
    int isShow = args_getInt(args, "isShow");
    if (isShow)
    {
        printf("hello, %s, %s and %s!\r\n", name1, name2, name3);
        printf("my name is %s.\r\n", myName);
    }
}

void hello(MimiObj *obj, Args *args)
{
    char *name = args_getStr(args, "name");
    int isShow = args_getInt(args, "isShow");
    if (isShow)
    {
        printf("hello, %s!\r\n", name);
    }
}

void add(MimiObj *obj, Args *args)
{
    int val1 = args_getInt(args, "val1");
    int val2 = args_getInt(args, "val2");
    method_returnInt(args, val1 + val2);
}

MimiObj *New_MimiObj_test(Args *args)
{
    MimiObj *self = New_MimiObj_sys(args);
    obj_import(self, "Hello", New_MimiObj_test);
    obj_newObj(self, "hello", "Hello");
    class_defineMethod(self, "hello(name:string, isShow:int)", hello);
    class_defineMethod(self, "hello2(name1:string, name2:string, name3:string, isShow:int)", hello2);
    class_defineMethod(self, "testFloat(val1:float, val2:float, isShow:int)->float", testFloat);
    class_defineMethod(self, "add(val1:int, val2:int)->int", add);
    return self;
}

int TEST_MimiObj(int isShow)
{
    {
        Args *args = New_args(NULL);
        args_setInt(args, "isEnable", 0);
        MimiObj *process = New_MimiObj_sys(args);
        if (isShow)
        {
            printf("the isEnable = %d\r\n", (int)args_getInt(process->attributeList, "isEnable"));
        }
        args_deinit(args);
        obj_deinit(process);
    }
    {
        MimiObj *process = newRootObj("sys",New_MimiObj_sys);
        float floatTest = 12.231;
        obj_bindFloat(process, "testFloatBind", &floatTest);
        if (isShow)
        {
            printf("the test of float bind: %s\r\n", obj_print(process, "testFloatBind"));
        }
        if (!strEqu("12.231000", obj_print(process, "testFloatBind")))
        {
            return 1;
        }
        obj_deinit(process);
    }
    {
        MimiObj *obj = newRootObj("test", New_MimiObj_test);
        obj_setInt(obj, "isShow", isShow);
        obj_run(obj, "hello(name = 'world', isShow = isShow)");
        obj_deinit(obj);
    }
    {
        MimiObj *obj = newRootObj("test", New_MimiObj_test);
        obj_setInt(obj, "isShow", isShow);
        obj_run(obj, "hello2(name2='tom', name1='john', name3='cat', isShow=isShow) ");
        obj_deinit(obj);
    }
    {
        MimiObj *obj = newRootObj("test", New_MimiObj_test);
        obj_setInt(obj, "isShow", isShow);
        obj_run(obj, "hello.hello2(name2 = 'tom', \
                                    name1 = 'john', \
                                    name3 = 'cat', \
                                    isShow = isShow) ");
        obj_deinit(obj);
    }
    {
        MimiObj *obj = newRootObj("test", New_MimiObj_test);
        obj_setInt(obj, "isShow", isShow);
        obj_setFloat(obj, "val2", 3.11);
        obj_run(obj, "res = testFloat(val1 = 3.22,val2 = val2,isShow = isShow)");
        float res = obj_getFloat(obj, "res");
        if (isShow)
        {
            printf("the res is %f \r\n", res);
        }
        if (6.33 - res > 0.001)
        {
            return 2;
        }
        obj_deinit(obj);
    }
    {
        MimiObj *obj = newRootObj("test", New_MimiObj_test);
        obj_run(obj, "res = add(val1 = 1, val2 = 2)");
        int res = obj_getInt(obj, "res");
        if (isShow)
        {
            printf("the res is %d.\r\n", res);
        }
        if (3 != res)
        {
            return 1;
        }
        obj_deinit(obj);
    }
    {
        MimiObj *obj = newRootObj("test", New_MimiObj_test);
        obj_run(obj, "res = add(1, 2)");
        int res = obj_getInt(obj, "res");
        if (isShow)
        {
            printf("the res is %d.\r\n", res);
        }
        if (3 != res)
        {
            return 3;
        }
        obj_deinit(obj);
    }
    {
        MimiObj *sys = newRootObj("sys",New_MimiObj_sys);
        int a = 0;
        obj_bind(sys, "int", "a", &a);
        obj_run(sys, "set('a', 1)");
        obj_deinit(sys);
        if (isShow)
        {
            printf("the 'a' after set is: %d\r\n", a);
        }
        if (1 != a)
        {
            return 4;
        }
    }
    {
        MimiObj *sys = newRootObj("sys",New_MimiObj_sys);
        obj_run(sys, "set('a', 1)");
        obj_run(sys, "del('a')");
        obj_deinit(sys);
    }
    {
        MimiObj *sys = newRootObj("sys",New_MimiObj_sys);
        obj_run(sys, "ls()");
        obj_setPtr(sys, "baseClass", New_TinyObj);
        obj_run(sys, "ls()");
        obj_deinit(sys);
    }

    return 0;
}
