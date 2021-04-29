#include "VM_device.h"
#include "VM_memory.h"
#include "mimiStr.h"
#include "PORT_com.h"

static int _writeHandle(device_t *self, args_t *args_in)
{
    char *str = args_in->list[0];
    PORT_send_to_com(3, str);
    return 0;
}

static void init_blueTeeth(device_t *self, int argc, char **argv)
{
    /* attrivute */

    /* operation */

    /* object */

    /* override */
    self->_writeHandle = _writeHandle;
}

device_t *New_device_blueTeeth(int argc, char **argv)
{
    device_t *self = New_device(argc, argv);
    self->init = init_blueTeeth;
    self->init(self, argc, argv);
    return self;
}