#include "test_VM_args.h"
#include <stdlib.h>

int TEST_VM_args(int isShow)
{
    int errCode = 0;
    args_t *args = New_args(NULL);
    args->setStrWithDefaultName(args, "arg1");
    args->setStrWithDefaultName(args, "arg2");

    char *arg1 = NULL, *arg2 = NULL;
    args->getStrByIndex(args, 0, &arg1);
    args->getStrByIndex(args, 1, &arg2);
    if (isShow)
    {
        printf("the arg1 = %s\r\n", arg1);
        printf("the arg2 = %s\r\n", arg2);
    }

    if (0 != strcmp(arg1, "arg1"))
    {
        errCode = 1;
        goto exit;
    }
    if (0 != strcmp(arg2, "arg2"))
    {
        errCode = 2;
        goto exit;
    }

    goto exit;

exit:
    args->dinit(args);
    return errCode;
}