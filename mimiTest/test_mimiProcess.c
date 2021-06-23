#include "mimiProcess.h"
#include "mimiCom.h"
#include "dataString.h"

int TEST_mimiProcess(int isShow)
{
    {
        args_t *args = New_args(NULL);
        args->setInt(args, "isEnable", 0);
        mimiProcess_t *process = New_mimiProcess(args);
        if (isShow)
        {
            printf("the isEnable = %d\r\n", (int)process->attributeList->getInt(process->attributeList, "isEnable"));
        }
        args->dinit(args);
        process->dinit(process);
    }
    {
        mimiProcess_t *process = New_mimiProcess(NULL);
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
        process->dinit(process);
    }
    return 0;
}
