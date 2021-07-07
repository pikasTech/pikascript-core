#include "server.h"
#include "dataMemory.h"

static void init_tempS(server_t *self, Args *args)
{
    /* attrivute */

    /* operation */

    /* object */

    /* override */
}

server_t *New_server_tempS(Args *args)
{
    server_t *self = New_server(args);
    self->init = init_tempS;
    self->init(self, args);
    return self;
}