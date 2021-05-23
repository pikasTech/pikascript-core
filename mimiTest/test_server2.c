#include "server2.h"

int TEST_server2(int isShow)
{
    int err = 0;
    list_t *args = New_list(NULL);
    args->int64(args, "isEnable", 0);
    server2_t *server = New_server2(args);

    goto exit;
exit:
    args->dinit(args);
    server->dinit(server);
    return err;
}
