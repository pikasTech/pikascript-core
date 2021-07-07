#include "PORT_adc.h"
#include "VM_device.h"
#include "dataMemory.h"

static int _read_handle(device_t *self, Args *args_in, Args *args_out)
{
		int err = 0;
    float val = 0;
	
		if(NULL == args_in)
		{
			val = (float)PORT_adc_get(1) * (3.3/4096);
			args_out->setFloatWithDefaultName(args_out, val);
			goto exit;
		}
		
exit:
		
    return err;
}

static void init_voltage(device_t *self, Args *args)
{
    /* attrivute */

    /* operation */

    /* object */

    /* override */
    self->_read_handle = _read_handle;
}

device_t *New_device_voltage(Args *args)
{
    device_t *self = New_device(args);
    self->init = init_voltage;
    self->init(self, args);
    return self;
}