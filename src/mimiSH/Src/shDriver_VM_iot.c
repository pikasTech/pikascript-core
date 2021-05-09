#include "shDriver_VM_iot.h"
#include "VM_iot.h"
#include "mimiSH_core.h"
#include "mimiStr.h"
#include <stdlib.h>

struct VM_iot *iot;

void *sDriver_VM_iot_main(int argc, char **argv)
{
    DMEM *strOut_mem = DynMemGet(sizeof(char) * 256);
    ((char *)(strOut_mem->addr))[0] = 0;
    if (isStartWith(argv[0], "init"))
    {
        iot = VM_iot_Init();
    }
    if (isStartWith(argv[0], "deinit"))
    {
        iot->DeInit(iot);
    }
    if (isStartWith(argv[0], "connect"))
    {
        iot->connect(iot, 1);
    }
    if (isStartWith(argv[0], "upload"))
    {
        iot->upload(iot, argv[1]);
    }
    if (isStartWith(argv[0], "set"))
    {
        void (*upload)(iot_t *, unsigned char *, unsigned char *) = NULL;
        unsigned char address[] = {0x00, 0x01};
        unsigned int data_int = 0;
        unsigned char data[] = {0x00, 0x00};
        if (isStartWith(argv[1], "tem"))
        {
            address[0] = 0x00;
            address[1] = 0x00;
            upload = iot->data_upload;
        }
        if (isStartWith(argv[1], "hum"))
        {
            address[0] = 0x00;
            address[1] = 0x01;
            upload = iot->data_upload;
        }
        if (isStartWith(argv[1], "smoke"))
        {
            address[0] = 0x00;
            address[1] = 0x02;
            upload = iot->data_upload;
        }
        if (isStartWith(argv[1], "mq2"))
        {
            address[0] = 0x00;
            address[1] = 0x03;
            upload = iot->data_upload;
        }
        if (isStartWith(argv[1], "bit"))
        {
            address[0] = 0x00;
            address[1] = 0x00;
            upload = iot->bit_upload;
        }
        if (NULL == upload)
        {
            strPrint((char *)strOut_mem->addr, "[error] set error, target no found!\r\n");
            return strOut_mem;
        }

        data_int = atoi(argv[2]);

        data[0] = (unsigned char)(data_int >> 8);

        data[1] = (unsigned char)data_int;

        upload(iot, address, data);
    }
    if (isStartWith(argv[0], "read"))
    {
        iot->readBuff(iot);
    }

    strPrint((char *)strOut_mem->addr, "Iot Process Finished\r\n");
    return strOut_mem;
}