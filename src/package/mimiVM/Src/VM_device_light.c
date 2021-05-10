#include "PORT_bh1750.h"
#include "VM_device.h"
#include "dataMemory.h"

int _read_handle(device_t *self, list_t *args_in, list_t *args_out)
{
    unsigned short int val_int = 0;
    float val = 0;
    val_int = PORT_bh_data_read();
    val = (float)val_int;
    args_out->pushFloatWithDefaultName(args_out, val);
    return 0;
}

static void init_light(device_t *self, list_t *args)
{
    PORT_bh_Init();
    PORT_bh_data_send(BHPowOn);
    PORT_bh_data_send(BHReset);
    PORT_bh_data_send(BHModeL); //选择的是分辨率是4lx，测量时间是16ms的
    /* attrivute */

    /* operation */

    /* object */

    /* override */
    self->_read_handle = _read_handle;
}

device_t *New_device_light(list_t *args)
{
    device_t *self = New_device(args);
    self->init = init_light;
    self->init(self, args);
    return self;
}