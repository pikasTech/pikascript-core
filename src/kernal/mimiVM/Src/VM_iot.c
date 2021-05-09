#include "VM_iot.h"
#include "VM_delay.h"
#include "dataMemory.h"
#include "mimiSH_core.h"
#include "dataString.h"
#include "mimiVM_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define HUM_INT 7
#define HUM_DEC 8
#define TEMP_INT 9
#define TEMP_DEC 10
#define LED_STA 12
#define CRC_MSB 14
#define CRC_LSB 13

iot_t *Pub_iot;

void DeInit(struct VM_iot *VM_iot)
{
    DynMemPut(VM_iot->mem);
    VM_iot->vd->deinit(VM_iot->vd);
}

int test_send(struct VM_iot *VM_iot, int isShow)
{
    VM_iot->send(VM_iot, "AT\r\n");
    return 0;
}

void send(struct VM_iot *VM_iot, char *str)
{
    VM_UART_Transmit(&VM.com2, str);
}

void info(struct VM_iot *VM_iot, char *str)
{
    VM_UART_Transmit(&VM.com1, str);
}

int VM_iot_simulate(int (*test)(struct VM_iot *, int), struct VM_iot *VM_iot, int isShow)
{
    sh_t *sh = ShellCore_init();
    DynMemPut(sh->cmd(sh, "uart init"));
    struct VM_iot *iot = VM_iot_Init();
    test(iot, isShow);
    for (int i = 0; i < 100; i++)
    {
        VM_main();
        VM_vIT();
    }
    if (isShow)
    {
        printf("%s", VM.com1.loger->log);
        printf("%s", VM.com2.loger->log);
        printf("the log size of test is: %d\r\n", VM.com1.loger->log_size + VM.com2.loger->log_size);
    }
    VM.com1.loger->deinit(VM.com1.loger);
    VM.com2.loger->deinit(VM.com2.loger);
    sh->deinit(sh);
    iot->DeInit(iot);
    return (VM.com1.loger->log_size + VM.com2.loger->log_size);
}

int connect(struct VM_iot *iot, int isShow)
{
    iot->info(iot, "connecting.....\r\n");
    iot->send(iot, "AT\r\n");
    iot->delay(iot, 500);
    iot->send(iot, "AT+CGSN=1\r\n");
    iot->delay(iot, 500);
    iot->send(iot, "AT+NCDP=117.60.157.137,5683\r\n");
    iot->delay(iot, 500);
    iot->send(iot, "AT+NRB\r\n");
    iot->delay(iot, 3000);
    iot->delay(iot, 3000);

    iot->send(iot, "AT+NCDP?\r\n");
    iot->delay(iot, 500);
    iot->send(iot, "AT+NSMI=1\r\n");
    iot->delay(iot, 500);
    iot->send(iot, "AT+NNMI=2\r\n");
    iot->delay(iot, 500);
    iot->info(iot, "connect ok!\r\n");
    iot->delay(iot, 15000);
    return 0;
}

void VM_iot_delay(struct VM_iot *vi, int delay_ms)
{
    vi->vd->delay(vi->vd, delay_ms);
}

int test_delay(struct VM_iot *vi, int isShow)
{
    vi->delay(vi, 1000);
    return 0;
}

int test_upload(iot_t *iot, int isShow)
{
    iot->upload(iot, "3,313233");
    return 0;
}

static unsigned int ccitt_table[256] =
    {
        0x0000,
        0xC0C1,
        0xC181,
        0x0140,
        0xC301,
        0x03C0,
        0x0280,
        0xC241,
        0xC601,
        0x06C0,
        0x0780,
        0xC741,
        0x0500,
        0xC5C1,
        0xC481,
        0x0440,
        0xCC01,
        0x0CC0,
        0x0D80,
        0xCD41,
        0x0F00,
        0xCFC1,
        0xCE81,
        0x0E40,
        0x0A00,
        0xCAC1,
        0xCB81,
        0x0B40,
        0xC901,
        0x09C0,
        0x0880,
        0xC841,
        0xD801,
        0x18C0,
        0x1980,
        0xD941,
        0x1B00,
        0xDBC1,
        0xDA81,
        0x1A40,
        0x1E00,
        0xDEC1,
        0xDF81,
        0x1F40,
        0xDD01,
        0x1DC0,
        0x1C80,
        0xDC41,
        0x1400,
        0xD4C1,
        0xD581,
        0x1540,
        0xD701,
        0x17C0,
        0x1680,
        0xD641,
        0xD201,
        0x12C0,
        0x1380,
        0xD341,
        0x1100,
        0xD1C1,
        0xD081,
        0x1040,
        0xF001,
        0x30C0,
        0x3180,
        0xF141,
        0x3300,
        0xF3C1,
        0xF281,
        0x3240,
        0x3600,
        0xF6C1,
        0xF781,
        0x3740,
        0xF501,
        0x35C0,
        0x3480,
        0xF441,
        0x3C00,
        0xFCC1,
        0xFD81,
        0x3D40,
        0xFF01,
        0x3FC0,
        0x3E80,
        0xFE41,
        0xFA01,
        0x3AC0,
        0x3B80,
        0xFB41,
        0x3900,
        0xF9C1,
        0xF881,
        0x3840,
        0x2800,
        0xE8C1,
        0xE981,
        0x2940,
        0xEB01,
        0x2BC0,
        0x2A80,
        0xEA41,
        0xEE01,
        0x2EC0,
        0x2F80,
        0xEF41,
        0x2D00,
        0xEDC1,
        0xEC81,
        0x2C40,
        0xE401,
        0x24C0,
        0x2580,
        0xE541,
        0x2700,
        0xE7C1,
        0xE681,
        0x2640,
        0x2200,
        0xE2C1,
        0xE381,
        0x2340,
        0xE101,
        0x21C0,
        0x2080,
        0xE041,
        0xA001,
        0x60C0,
        0x6180,
        0xA141,
        0x6300,
        0xA3C1,
        0xA281,
        0x6240,
        0x6600,
        0xA6C1,
        0xA781,
        0x6740,
        0xA501,
        0x65C0,
        0x6480,
        0xA441,
        0x6C00,
        0xACC1,
        0xAD81,
        0x6D40,
        0xAF01,
        0x6FC0,
        0x6E80,
        0xAE41,
        0xAA01,
        0x6AC0,
        0x6B80,
        0xAB41,
        0x6900,
        0xA9C1,
        0xA881,
        0x6840,
        0x7800,
        0xB8C1,
        0xB981,
        0x7940,
        0xBB01,
        0x7BC0,
        0x7A80,
        0xBA41,
        0xBE01,
        0x7EC0,
        0x7F80,
        0xBF41,
        0x7D00,
        0xBDC1,
        0xBC81,
        0x7C40,
        0xB401,
        0x74C0,
        0x7580,
        0xB541,
        0x7700,
        0xB7C1,
        0xB681,
        0x7640,
        0x7200,
        0xB2C1,
        0xB381,
        0x7340,
        0xB101,
        0x71C0,
        0x7080,
        0xB041,
        0x5000,
        0x90C1,
        0x9181,
        0x5140,
        0x9301,
        0x53C0,
        0x5280,
        0x9241,
        0x9601,
        0x56C0,
        0x5780,
        0x9741,
        0x5500,
        0x95C1,
        0x9481,
        0x5440,
        0x9C01,
        0x5CC0,
        0x5D80,
        0x9D41,
        0x5F00,
        0x9FC1,
        0x9E81,
        0x5E40,
        0x5A00,
        0x9AC1,
        0x9B81,
        0x5B40,
        0x9901,
        0x59C0,
        0x5880,
        0x9841,
        0x8801,
        0x48C0,
        0x4980,
        0x8941,
        0x4B00,
        0x8BC1,
        0x8A81,
        0x4A40,
        0x4E00,
        0x8EC1,
        0x8F81,
        0x4F40,
        0x8D01,
        0x4DC0,
        0x4C80,
        0x8C41,
        0x4400,
        0x84C1,
        0x8581,
        0x4540,
        0x8701,
        0x47C0,
        0x4680,
        0x8641,
        0x8201,
        0x42C0,
        0x4380,
        0x8341,
        0x4100,
        0x81C1,
        0x8081,
        0x4040,
};

/**
* @brief  Calculate CRC16
* @param  q: Pointer to data
* @param  len: data length
* @retval CRC16 value
*/
static unsigned short int crc16(unsigned char *q, int len)
{
    unsigned int crc = 0xffff;

    while (len-- > 0)
        crc = ccitt_table[(crc ^ *q++) & 0xff] ^ (crc >> 8);

    return crc;
}

void *upload_bit_get(unsigned char *address, unsigned char *data)
{
    int i = 0;
    int len = 0;
    DMEM *mem = DynMemGet(sizeof(unsigned char) * 256);
    unsigned char *buffer = (unsigned char *)(mem->addr);

    unsigned int crc;

    unsigned char tmp[] = {0x01, 0x45, 0x00, 0x00, 0x00, 0x01, 0x01, 0x22, 0x00, 0x00};

    tmp[2 + 0] = address[0];
    tmp[2 + 1] = address[1];

    tmp[7] = data[1];

    memset(buffer, 0, 256);

    crc = crc16(tmp, sizeof(tmp) - 2);

    tmp[sizeof(tmp) - 1] = crc >> 8;
    tmp[sizeof(tmp) - 2] = crc & 0xff;

    len = strlen("AT+NMGS=10,");
    memcpy(buffer, "AT+NMGS=10,", len);

    for (i = 0; i < sizeof(tmp); i++)
    {
        sprintf((char *)buffer + len, "%02X", tmp[i]);
        len += 2;
    }

    sprintf((char *)buffer + len, "\r\n");

    return mem;
}

int test_upload_bit_get(iot_t *iot, int isShow)
{
    unsigned char address[] = {0x00, 0x00};
    unsigned char data[] = {0x00};
    unsigned char *strOut;
    unsigned char buff[256] = {0};
    DMEM *memOut = upload_bit_get(address, data);
    strOut = memOut->addr;
    sprintf((char *)buff, "the output of test of bit date= %s\r\n", strOut);
    int strOut_size = 0;
    strOut_size = strGetSize((char *)buff);
    if (isShow)
    {
        printf("%s", buff);
        printf("the size of strOut is: %d\r\n", strOut_size);
    }
    if (67 != strOut_size)
    {
        return 1;
    }
    DynMemPut(memOut);
    return 0;
}

void *upload_data_get(unsigned char *address, unsigned char *data)
{
    int i = 0;
    int len = 0;

    DMEM *mem = DynMemGet(sizeof(unsigned char) * 256);
    unsigned char *buffer = mem->addr;

    unsigned int crc;

    unsigned char tmp[] = {0x01, 0x46, 0x00, 0x01, 0x00, 0x01, 0x02, 0x22, 0x22, 0x00, 0x00};

    tmp[2 + 0] = address[0];
    tmp[2 + 1] = address[1];

    tmp[7 + 0] = data[0];
    tmp[7 + 1] = data[1];

    memset(buffer, 0, 256);

    crc = crc16(tmp, sizeof(tmp) - 2);

    tmp[sizeof(tmp) - 1] = crc >> 8;
    tmp[sizeof(tmp) - 2] = crc & 0xff;

    len = strlen("AT+NMGS=11,");
    memcpy(buffer, "AT+NMGS=11,", len);

    for (i = 0; i < sizeof(tmp); i++)
    {
        sprintf((char *)buffer + len, "%02X", tmp[i]);
        len += 2;
    }

    sprintf((char *)buffer + len, "\r\n");

    return mem;
}

int test_upload_data_get(iot_t *iot, int isShow)
{
    unsigned char address[] = {0x00, 0x01};
    unsigned char data[] = {0x00, 0x03};
    unsigned char *strOut;
    DMEM *memOut = upload_data_get(address, data);
    strOut = memOut->addr;
    if (isShow)
    {
        printf("the output of test of notify_data = %s\r\n", strOut);
    }
    DynMemPut(memOut);
    return 0;
}

void upload(iot_t *iot, char *str)
{
    char AT_cmd[] = "AT+NMGS=";
    char buff[256] = {0};
    strPrint(buff, AT_cmd);
    strPrint(buff, str);
    strPrint(buff, "\r\n");
    iot->send(iot, buff);
}

int test_data_upload(iot_t *iot, int isShow)
{
    unsigned char address[] = {0x00, 0x00};
    unsigned char data[] = {0x00, 0x22};
    iot->data_upload(iot, address, data);
    return 0;
}

void VM_iot_data_upload(iot_t *iot, unsigned char *address, unsigned char *data)
{
    unsigned char *strOut = 0;
    DMEM *memOut = upload_data_get(address, data);
    strOut = memOut->addr;
    iot->send(iot, (char *)strOut);
    DynMemPut(memOut);
}

void VM_iot_bit_upload(iot_t *iot, unsigned char *address, unsigned char *data)
{
    unsigned char *strOut = 0;
    DMEM *memOut = upload_bit_get(address, data);
    strOut = memOut->addr;
    iot->send(iot, (char *)strOut);
    // free(strOut);
    DynMemPut(memOut);
}

int VM_iot_test(struct VM_iot *VM_iot, int isShow)
{

    // if (0 != VM_iot_simulate(test_send, VM_iot, isShow))
    // {
    //     return 1;
    // }
    // if (0 != VM_iot_simulate(connect, VM_iot, isShow))
    // {
    //     return 2;
    // }
    // if (0 != VM_iot_simulate(test_delay, VM_iot, isShow))
    // {
    // return 3;
    // }
    if (61 != VM_iot_simulate(test_upload, VM_iot, isShow))
    {
        return 4;
    }

    if (0 != test_upload_data_get(VM_iot, isShow))
    {
        return 6;
    }
    if (78 != VM_iot_simulate(test_data_upload, VM_iot, isShow))
    {
        return 7;
    }
    if (0 != test_upload_bit_get(VM_iot, isShow))
    {
        return 8;
    }
    return 0;
}

int VM_iot_get_value_function_06(iot_t *iot, char *strGet)
{
    char val_str[5] = {0};
    int val_each[5] = {0};
    int val = 0;

    val_str[0] = strGet[10];
    val_str[1] = strGet[11];
    val_str[2] = strGet[12];
    val_str[3] = strGet[13];

    for (int i = 0; i < 4; i++)
    {
        val *= 16;
        val_each[i] = val_str[i] - 0x30;
        if (val_each[i] > 9)
        {
            val_each[i] -= 7;
        }
        val += val_each[i];
    }

    return val;
}

int VM_iot_get_addr_function_06(iot_t *iot, char *strGet)
{
    char addr_str[3] = {0};
    int addr = 0;
    addr_str[0] = strGet[8];
    addr_str[1] = strGet[9];
    addr = atoi(addr_str);
    return addr;
}

void VM_iot_callBack_defult(iot_t *iot, void *data)
{
    // char *strGet = (char *)data;
}

void VM_iot_readBuff(iot_t *iot)
{
    iot->send(iot, "AT+NMGR\r\n");
}

static void data_upload_int(iot_t *iot, int addr_int, int data_int)
{
    unsigned char addr[2] = {0};
    unsigned char data[2] = {0};

    data[0] = (unsigned char)(data_int >> 8);
    data[1] = (unsigned char)(data_int);

    addr[0] = (unsigned char)(addr_int >> 8);
    addr[1] = (unsigned char)(addr_int);

    iot->data_upload(iot, addr, data);
}
struct VM_iot *VM_iot_Init(void)
{
    DMEM *mem = DynMemGet(sizeof(struct VM_iot));
    struct VM_iot *iot = (struct VM_iot *)mem->addr;
    iot->mem = mem;
    iot->DeInit = DeInit;
    iot->send = send;
    iot->info = info;
    iot->connect = connect;
    iot->test = VM_iot_test;
    iot->vd = VM_delay_init();
    iot->delay = VM_iot_delay;
    iot->upload = upload;
    iot->data_upload = VM_iot_data_upload;
    // should be rewrite in user code
    iot->callBack = VM_iot_callBack_defult;
    iot->readBuff = VM_iot_readBuff;
    iot->bit_upload = VM_iot_bit_upload;

    iot->get_value_function_06 = VM_iot_get_value_function_06;
    iot->get_addr_function_06 = VM_iot_get_addr_function_06;

    iot->data_upload_int = data_upload_int;
    iot->context = iot;
    return iot;
}