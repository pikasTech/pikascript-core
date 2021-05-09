#include "VM_com.h"
#include "VM_memory.h"
#include "mimiStr.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifndef LINUX
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#endif
void VM_com_DeInit(VM_com_t *pcom)
{
    DynMemPut(pcom->mem);
}

void VM_com_send(VM_com_t *pcom, char *pData)
{
    // push string to the hardware buffer
    strPrint(pcom->TX_buff, pData);
    // set user IT to hardwore buffer to get start
    pcom->TX_vIT = 1;
}

void VM_com_DriverInit(VM_com_t *pcom)
{
#ifndef LINUX
    HAL_UART_Receive_IT(&huart1, (unsigned char *)VM_com_RX_Char_pget(&VM.com1), 1); //使能串口接收中断
    HAL_UART_Receive_IT(&huart2, (unsigned char *)VM_com_RX_Char_pget(&VM.com2), 1); //使能串口接收中断
#endif
}

char *VM_com_RX_charPointerGet(VM_com_t *pcom)
{
    return pcom->RX_Char;
}

char *VM_com_sendWithName(VM_com_t *pcom, char *pData, char *strOut)
{
    char *usart_TX_buff;

    usart_TX_buff = pcom->TX_buff;

    // push string to the hardware buffer
    strPrint(usart_TX_buff, pData);
    strPrint(usart_TX_buff, "\r\n");

    // print debug info
    strPrint(strOut, "have send data to ");
    strPrint(strOut, pcom->name);
    strPrint(strOut, " >");

    pcom->send(pcom, strOut);

    return strOut;
}

static void VM_com_SingleLine_load(VM_com_t *pcom)
{
    pcom->RX_SingleLine[0] = 0;
    pcom->CMDOut[0] = 0;
    int j;

    // Load ShellCMD from buff;
    for (j = 0; j < pcom->RX_buff_i; j++)
    {
        pcom->RX_SingleLine[j] = pcom->RX_buff[j];
    }
    // add '\0' to the end of RX_SingleLine
    pcom->RX_SingleLine[j] = 0;

    // set the RX_IT_SsingeLine, to tragle the call back of single line IT
    pcom->RX_IT_SingleLine = 1;
    pcom->RX_buff_i = 0;
}

static void VM_com_RX_CharCallBack(VM_com_t *pcom)
{
    if ((pcom->RX_Char[0] != '\r') && (pcom->RX_Char[0] != '\n'))
    {
        pcom->RX_buff[pcom->RX_buff_i] = pcom->RX_Char[0];
        pcom->RX_buff_i += 1;
    }
    if (pcom->RX_Char[0] == '\r')
    {
        VM_com_SingleLine_load(pcom);
    }
}

static void VMdriver_comTransmitLinux(VM_com_t *pcom)
{
    char buffer[512] = {0};
    sprintf(buffer, "[vm%s]: \r\n%s\r\n", pcom->name, pcom->TX_buff);
    if (pcom->isShow)
    {
        printf("%s", buffer);
    }
    int outSize = strGetSize(buffer);
    pcom->outSize = outSize;
}

void VMdriver_comTransmitHuart2(VM_com_t *pcom)
{
#ifndef LINUX
    HAL_UART_Transmit(&huart2, pcom->TX_buff strGetSize(pcom->TX_buff), 100);
#endif
#ifdef LINUX
    VMdriver_comTransmitLinux(pcom);
#endif
}

static void VMdriver_comTransmitHuart1(VM_com_t *pcom)
{
#ifndef LINUX
    HAL_UART_Transmit(&huart1, (unsigned char *)pcom->TX_buff, strGetSize(pcom->TX_buff), 100);
#endif
#ifdef LINUX
    VMdriver_comTransmitLinux(pcom);
#endif
}

static void VM_com_TX_buffCallBack(VM_com_t *pcom)
{
    // send TX_buffer to hardware
    pcom->DriverSend(pcom);
    // clear the TX_vIT
    pcom->TX_vIT = 0;
}
// dont modify the default call back, if need to add sub call back, rewrite another call back, and link it to pcom->IT_CallBack

static void VM_com_IT_CallBack_default(VM_com_t *pcom)
{
    VM_com_RX_CharCallBack(pcom);
}

// dont modify the default call back, if need to add sub call back, rewrite another call back, and link it to pcom->vIT_CallBack

static void VM_com_vIT_CallBack_default(VM_com_t *pcom)
{
    VM_com_TX_buffCallBack(pcom);
}

VM_com_t *VM_com_init_v2(void)
{
    DMEM *mem = DynMemGet(sizeof(VM_com_t));
    VM_com_t *pcom = (VM_com_t *)(mem->addr);
    pcom->mem = mem;
    pcom->DriverInit = VM_com_DriverInit;
    // init the driver
    pcom->DriverInit(pcom);
    // init the var
    pcom->RX_buff_i = 0;
    pcom->RX_buff[0] = 0;
    pcom->RX_Char[0] = 0;
    pcom->RX_IT = 0;
    pcom->RX_IT_SingleLine = 0;
    pcom->RX_SingleLine[0] = 0;

    pcom->TX_buff[0] = 0;
    pcom->TX_vIT = 0;

    pcom->name[0] = 0;
    pcom->CMDOut[0] = 0;

    pcom->isShow = 0;
    pcom->outSize = 0;

    // set default name to com1, can be redefined
    strPrint(pcom->name, "com1");

    // init the mathods
    pcom->DeInit = VM_com_DeInit;
    pcom->send = VM_com_send;
    pcom->sendWithName = VM_com_sendWithName;

    // default vIT call back, can be redefined
    pcom->IT_CallBack = VM_com_IT_CallBack_default;
    // default vIT call back, can be redefined
    pcom->vIT_CallBack = VM_com_vIT_CallBack_default;
    //set driver send default to usart1
    pcom->DriverSend = VMdriver_comTransmitHuart1;

    return pcom;
}
