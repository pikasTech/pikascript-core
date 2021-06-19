#include "mimiProcess.h"

int TEST_server2(int isShow)
{
    isShow = 1;
    int err = 0;
    args_t *args = New_args(NULL);
    args->setInt64WithName(args, "isEnable", 0);
    process_t *process = New_process(args);

    if (isShow)
    {
        printf("the isEnable = %d\r\n", (int)process->attributeList->getInt64ByName(process->attributeList, "isEnable"));
    }

    goto exit;
exit:
    args->dinit(args);
    process->dinit(process);
    return err;
}
