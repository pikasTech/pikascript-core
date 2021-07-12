#include "MimiObj.h"
#include "mimiCom.h"
#include "dataString.h"

void testFloat(MimiObj *obj, Args *args)
{
    float val1 = args->getFloat(args, "val1");
    float val2 = args->getFloat(args, "val2");
    int isShow = args->getInt(args, "isShow");
    if (isShow)
    {
        printf("the float val1 is: %f\r\n", val1);
        printf("the float val2 is: %f\r\n", val2);
    }
}

void hello2(MimiObj *obj, Args *args)
{
    char *name1 = args->getStr(args, "name1");
    char *name2 = args->getStr(args, "name2");
    char *name3 = args->getStr(args, "name3");
    char *myName = obj->name;
    int isShow = args->getInt(args, "isShow");
    if (isShow)
    {
        printf("hello, %s, %s and %s!\r\n", name1, name2, name3);
        printf("my name is %s.\r\n", myName);
    }
}

void hello(MimiObj *obj, Args *args)
{
    char *name = args->getStr(args, "name");
    int isShow = args->getInt(args, "isShow");
    if (isShow)
    {
        printf("hello, %s!\r\n", name);
    }
}

int TEST_MimiObj(int isShow)
{
    {
        Args *args = New_args(NULL);
        args->setInt(args, "isEnable", 0);
        MimiObj *process = New_MimiObj(args);
        if (isShow)
        {
            printf("the isEnable = %d\r\n", (int)process->attributeList->getInt(process->attributeList, "isEnable"));
        }
        args->deinit(args);
        process->deinit(process);
    }
    {
        MimiObj *process = New_MimiObj(NULL);
        float floatTest = 12.231;
        process->bindFloat(process, "testFloatBind", &floatTest);
        if (isShow)
        {
            printf("the test of float bind: %s\r\n", process->print(process, "testFloatBind"));
        }
        if (!mimiStrEqu("12.231000", process->print(process, "testFloatBind")))
        {
            return 1;
        }
        process->deinit(process);
    }
    {
        MimiObj *obj = New_MimiObj(NULL);
        obj->setMethod(obj, "hello(name: string, isShow: int)", hello);
        obj->setInt(obj, "isShow", isShow);
        obj->run(obj, "hello(name = \"world\", isShow = isShow)");
        obj->deinit(obj);
    }
    {
        MimiObj *obj = New_MimiObj(NULL);
        obj->setMethod(obj, "hello2(name1: string, name2: string, name3: string, isShow: int)", hello2);
        obj->setInt(obj, "isShow", isShow);
        obj->run(obj, "hello2(name2 = \"tom\", \
                              name1 = \"john\", \
                              name3 = \"cat\", \
                              isShow = isShow) ");
        obj->deinit(obj);
    }
    {
        MimiObj *obj = New_MimiObj(NULL);
        obj->setObj(obj, "hello", New_MimiObj);
        obj->setMethod(obj, "hello.hello2(name1: string, name2: string, name3: string, isShow: int)", hello2);
        obj->setInt(obj, "isShow", isShow);
        obj->run(obj, "hello.hello2(name2 = \"tom\", \
                                    name1 = \"john\", \
                                    name3 = \"cat\", \
                                    isShow = isShow) ");
        obj->deinit(obj);
    }
    {
        MimiObj *obj = New_MimiObj(NULL);
        obj->setObj(obj, "hello", New_MimiObj);
        obj->setStr(obj, "name1", "john");
        obj->setInt(obj, "isShow", isShow);
        obj->setMethod(obj, "hello.hello2(name1: string, name2: string, name3: string, isShow: int)", hello2);
        obj->run(obj, "hello.hello2(name2 = \"tom\", \
                                    name1 = name1, \
                                    name3 = \"cat\", \
                                    isShow = isShow) ");
        obj->deinit(obj);
    }
    {
        MimiObj *obj = New_MimiObj(NULL);
        obj->setMethod(obj, "testFloat(val1:float, val2:float, isShow:int)", testFloat);
        obj->setInt(obj, "isShow", 1);
        obj->setFloat(obj, "val2", 3.11);
        obj->run(obj, "testFloat(val1 = 3.22, val2 = val2, isShow = isShow)");
        obj->deinit(obj);
    }

    return 0;
}
