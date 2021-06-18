#include "server2.h"

int TEST_server2(int isShow)
{
    int err = 0;
    args_t *args = New_args(NULL);
    args->setInt64WithName(args, "isEnable", 0);
    server2_t *server = New_server2(args);

    if (isShow)
    {
        printf("the isEnable = %d\r\n", (int)server->attributeList->getInt64ByName(server->attributeList, "isEnable"));
    }

    goto exit;
exit:
    args->dinit(args);
    server->dinit(server);
    return err;
}
