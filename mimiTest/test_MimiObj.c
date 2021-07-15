#include "MimiObj.h"
#include "sysObj.h"
#include "mimiCom.h"
#include "dataString.h"

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
    args_setFloat(args, "return", val1 + val2);
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
    args_setInt(args, "return", val1 + val2);
}

int TEST_MimiObj(int isShow)
{
    {
        Args *args = New_args(NULL);
        args_setInt(args, "isEnable", 0);
        MimiObj *process = New_MimiObj(args);
        if (isShow)
        {
            printf("the isEnable = %d\r\n", (int)args_getInt(process->attributeList, "isEnable"));
        }
        args_deinit(args);
        obj_deinit(process);
    }
    {
        MimiObj *process = New_MimiObj(NULL);
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
        MimiObj *obj = New_MimiObj(NULL);
        obj_defineMethod(obj, "hello(name: string, isShow: int)", hello);
        obj_setInt(obj, "isShow", isShow);
        obj_run(obj, "hello(name = 'world', isShow = isShow)");
        obj_deinit(obj);
    }
    {
        MimiObj *obj = New_MimiObj(NULL);
        obj_defineMethod(obj, "hello2(name1: string, name2: string, name3: string, isShow: int)", hello2);
        obj_setInt(obj, "isShow", isShow);
        obj_run(obj, "hello2(name2 = 'tom', \
                              name1 = 'john', \
                              name3 = 'cat', \
                              isShow = isShow) ");
        obj_deinit(obj);
    }
    {
        MimiObj *obj = New_MimiObj(NULL);
        obj_setObj(obj, "hello", New_MimiObj);
        obj_defineMethod(obj, "hello.hello2(name1: string, name2: string, name3: string, isShow: int)", hello2);
        obj_setInt(obj, "isShow", isShow);
        obj_run(obj, "hello.hello2(name2 = 'tom', \
                                    name1 = 'john', \
                                    name3 = 'cat', \
                                    isShow = isShow) ");
        obj_deinit(obj);
    }
    {
        MimiObj *obj = New_MimiObj(NULL);
        obj_setObj(obj, "hello", New_MimiObj);
        obj_setStr(obj, "name1", "john");
        obj_setInt(obj, "isShow", isShow);
        obj_defineMethod(obj, "hello.hello2(name1: string, \
                                          name2: string, \
                                          name3: string, \
                                          isShow: int)",
                         hello2);
        obj_run(obj, "hello.hello2(name2 = 'tom', \
                                    name1 = name1, \
                                    name3 = 'cat', \
                                    isShow = isShow) ");
        obj_deinit(obj);
    }
    {
        MimiObj *obj = New_MimiObj(NULL);
        obj_defineMethod(obj, "testFloat(            \
                                       val1:float, \
                                       val2:float, \
                                       isShow:int) \
                                       :float",
                         testFloat);
        obj_setInt(obj, "isShow", isShow);
        obj_setFloat(obj, "val2", 3.11);
        obj_run(obj, "res = testFloat(val1 = 3.22, \
                                       val2 = val2, \
                                       isShow = isShow)");
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
        MimiObj *obj = New_MimiObj(NULL);
        obj_defineMethod(obj, "add(val1:int, val2:int):int", add);
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
        MimiObj *obj = New_MimiObj(NULL);
        obj_defineMethod(obj, "add(val1:int, val2:int):int", add);
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
        MimiObj *sys = New_MimiObj_sys(NULL);
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
        MimiObj *sys = New_MimiObj_sys(NULL);
        obj_setObj(sys, "b", New_MimiObj);
        obj_run(sys, "set('b.a', 1)");
        int a = obj_getInt(sys, "b.a");
        obj_deinit(sys);
        if (isShow)
        {
            printf("the 'b.a' after set is: %d\r\n", a);
        }
        if (1 != a)
        {
            return 4;
        }
    }
    {
        MimiObj *sys = New_MimiObj_sys(NULL);
        obj_run(sys, "set('a', 1)");
        obj_run(sys, "del('a')");
        obj_deinit(sys);
    }
    {
        MimiObj *sys = New_MimiObj_sys(NULL);
        obj_setObj(sys, "b", New_MimiObj);
        obj_run(sys, "set('b.a', 1)");
        obj_run(sys, "del('b.a')");
        obj_run(sys, "set('b.a',1.2)");
        float a = obj_getFloat(sys, "b.a");
        obj_deinit(sys);
        if(isShow)
        {
            printf("the a after del and reset is: %f\r\n", a);
        }
        if((1.2-a)*(1.2-a)>0.0000001)
        {
            return 5;
        }
    }

    return 0;
}
