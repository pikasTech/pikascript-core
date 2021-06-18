#include "server.h"

int TEST_server(int isShow)
{
    int err = 0;
    args_t *args = New_args(NULL);
    args->pushInt64WithName(args, "isEnable", 0);
    server_t *server = New_server(args);

    goto exit;
exit:
    args->dinit(args);
    server->dinit(server);
    return err;
}