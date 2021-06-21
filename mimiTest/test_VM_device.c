#include "test_VM_device.h"
#include "VM_device.h"
#include "VM_device_light.h"
#include "VM_device_voltage.h"
#include "VM_device_zigbee.h"
#include <stdlib.h>

int TEST_VM_device(int isShow)
{
    int err = 0;
    device_t *device = New_device(NULL);
    device_t *zigbee = New_device_zigbee(NULL);
    device_t *voltage = New_device_voltage(NULL);
    device_t *light = New_device_light(NULL);

    args_t *args_in = New_args(NULL);
    args_t *args_in_zigbee = New_args(NULL);
    args_t *args_in_voltage = New_args(NULL);
    args_t *args_out = New_args(NULL);
    args_t *args_out_voltage = New_args(NULL);
    args_t *args_in_float = New_args(NULL);

    float lightVarRead = light->read_float(light, NULL);
    if (isShow)
    {
        printf("the light Var is %f\r\n", lightVarRead);
    }

    float floatInput = 4.278;
    float floatOutput = 0;
    args_in_float->setFloatWithDefaultName(args_in_float, floatInput);
    floatOutput = device->read_float(device, args_in_float);

    if (isShow)
    {
        printf("the float output is %f\r\n", floatOutput);
    }

    if ((float)4.278 != floatOutput)
    {
        err = 7;
        goto exit;
    }

    if ((float)55 != lightVarRead)
    {
        err = 8;
        goto exit;
    }

    char *strOut1 = NULL, *strOut2 = NULL;
    args_in->setStrWithDefaultName(args_in, "arg1");
    args_in->setStrWithDefaultName(args_in, "arg2");
    args_in_zigbee->setStrWithDefaultName(args_in_zigbee, "test print to zigbee\r\n");

    if (0 != device->read(device, args_in, args_out))
    {
        err = 1;
        goto exit;
    }

    strOut1 = args_out->getStrByIndex(args_out, 0);
    strOut2 = args_out->getStrByIndex(args_out, 1);

    if (0 != strcmp(strOut1, "arg1"))
    {
        err = 2;
        goto exit;
    }
    if (0 != strcmp(strOut2, "arg2"))
    {
        err = 3;
        goto exit;
    }

    if (2 != device->write(device, args_in))
    {
        err = 4;
        goto exit;
    }

    if (0 != zigbee->write(zigbee, args_in_zigbee))
    {
        err = 5;
        goto exit;
    }

    if (0 != voltage->read(voltage, args_in_voltage, args_out_voltage))
    {
        err = 6;
        goto exit;
    }

    if (isShow)
    {
        printf("read from device :%s\r\n", strOut1);
        printf("read from device :%s\r\n", strOut2);
    }

exit:
    device->dinit(device);
    args_in->dinit(args_in);
    args_in_zigbee->dinit(args_in_zigbee);
    args_in_voltage->dinit(args_in_voltage);
    args_out->dinit(args_out);
    args_out_voltage->dinit(args_out_voltage);
    args_in_float->dinit(args_in_float);
    zigbee->dinit(zigbee);
    voltage->dinit(voltage);
    light->dinit(light);
    return err;
}