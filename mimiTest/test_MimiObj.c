#include "MimiObj.h"
#include "mimiCom.h"
#include "dataString.h"

void hello2(MimiObj *obj, Args *args)
{
    char *name1 = args->getStr(args, "name1");
    char *name2 = args->getStr(args, "name2");
    char *name3 = args->getStr(args, "name3");
    printf("hello, %s, %s and %s!\r\n", name1, name2, name3);
}

void hello(MimiObj *obj, Args *args)
{
    char *name = args->getStr(args, "name");
    printf("hello, %s!\r\n", name);
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
        obj->setMethod(obj, "hello(name: string)", hello);
        obj->run(obj, "hello(name = world)");
        obj->deinit(obj);
    }
    {
        MimiObj *obj = New_MimiObj(NULL);
        obj->setMethod(obj, "hello2(name1: string, name2: string, name3: string)", hello2);
        obj->run(obj, "hello2(name2 = tom, name1 = john, name3 = cat)");
        obj->deinit(obj);
    }
    return 0;
}
