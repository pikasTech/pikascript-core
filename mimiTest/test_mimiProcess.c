#include "mimiProcess.h"

int TEST_mimiProcess(int isShow)
{
    int err = 0;
    args_t *args = New_args(NULL);
    args->setInt(args, "isEnable", 0);
    mimiProcess_t *process = New_process(args);

    if (isShow)
    {
        printf("the isEnable = %d\r\n", (int)process->attributeList->getInt(process->attributeList, "isEnable"));
    }

    goto exit;
exit:
    args->dinit(args);
    process->dinit(process);
    return err;
}
