#include "MimiProcess.h"
#include "mimiCom.h"
#include "dataString.h"

int TEST_mimiProcess(int isShow)
{
    {
        Args *args = New_args(NULL);
        args->setInt(args, "isEnable", 0);
        MimiProcess *process = New_MimiProcess(args);
        if (isShow)
        {
            printf("the isEnable = %d\r\n", (int)process->attributeList->getInt(process->attributeList, "isEnable"));
        }
        args->deinit(args);
        process->deinit(process);
    }
    {
        MimiProcess *process = New_MimiProcess(NULL);
        float floatTest = 12.231;
        process->argBindFloat(process, "testFloatBind", &floatTest);
        if (isShow)
        {
            printf("the test of float bind: %s\r\n", process->argPrint(process, "testFloatBind"));
        }
        if (!mimiStrEqu("12.231000", process->argPrint(process, "testFloatBind")))
        {
            return 1;
        }
        process->deinit(process);
    }
    return 0;
}
