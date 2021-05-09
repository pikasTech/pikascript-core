#include "server.h"

int TEST_server(int isShow)
{
    int err = 0;
    list_t *args = New_list(NULL);
    args->int64(args, "isEnable", 0);
    server_t *server = New_server(args);

    goto exit;
exit:
    args->dinit(args);
    server->dinit(server);
    return err;
}