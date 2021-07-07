#include "server.h"

int TEST_server(int isShow)
{
    int err = 0;
    Args *args = New_args(NULL);
    args->setInt(args, "isEnable", 0);
    server_t *server = New_server(args);

    goto exit;
exit:
    args->deinit(args);
    server->deinit(server);
    return err;
}