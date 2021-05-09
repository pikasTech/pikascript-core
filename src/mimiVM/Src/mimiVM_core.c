/*
VM_core is a virtusl machine runing in stm32, the shell_driver is based on VM directly to support all kinds of MCU, and can be simulated in linux.
*/
#ifndef LINUX
#include "gpio.h"
#include "tim.h"
#include "usart.h"
#endif
#include "VM_iot.h"
#include "VM_memory.h"
#include "dataLog.h"
#include "mimiSH_config.h"
#include "mimiSH_core.h"
#include "mimiStr.h"
// #include "VM_gps.h"
#include "mimiVM_core.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

extern iot_t *Pub_iot;

void VM_com_TX_vIT_CallBack(struct com_t *pcom);
void VM_com_IT_SingleLine_callBack_task(struct com_t *pcom);

// The VM memory.
struct VM_t VM;

// VM_APIs, other user like shell_driver can only call VM_APIs, only the VM_APIs can operate the VM memory.

char *VM_com_TX_buff_get(struct com_t *pcom)
{
	return (*pcom).TX_buff;
}

char *VM_com_TX_vIT_pget(struct com_t *pcom)
{
	return &(*pcom).TX_vIT;
}

char *VM_com_RX_Char_pget(struct com_t *pcom)
{
	return (*pcom).RX_Char;
}

char VM_com_RX_Char_get(struct com_t *pcom)
{
	return (*pcom).RX_Char[0];
}

char VM_com_TX_vIT_get(struct com_t *pcom)
{
	return (*pcom).TX_vIT;
}

void VM_com_TX_vIT_set(struct com_t *pcom, char var)
{
	(*pcom).TX_vIT = var;
}

char *VM_com_RX_buff_pget(struct com_t *pcom)
{
	return (*pcom).RX_buff;
}

int VM_com_RX_buff_i_get(struct com_t *pcom)
{
	return (*pcom).RX_buff_i;
}

int *VM_com_RX_buff_i_pget(struct com_t *pcom)
{
	return &(*pcom).RX_buff_i;
}

char *VIM_com_RX_SingleLine_pget(struct com_t *pcom)
{
	return (*pcom).RX_SingleLine;
}

char *VIM_com_CMDOut_pget(struct com_t *pcom)
{
	return (*pcom).CMDOut;
}
void VM_com_SingleLine_free(struct com_t *pcom)
{
	for (int i = 0; i < strGetSize(pcom->RX_SingleLine); i++)
	{
		pcom->RX_SingleLine[i] = 0;
	}
}
void VM_UART_Transmit(struct com_t *pcom, char *pData)
{
	char *usart_TX_buff;
	char *pUsart_TX_uIT;

	usart_TX_buff = VM_com_TX_buff_get(pcom);
	pUsart_TX_uIT = VM_com_TX_vIT_pget(pcom);

	// push string to the hardware buffer
	strPrint(usart_TX_buff, pData);
	// set user IT to hardwore buffer to get start
	*pUsart_TX_uIT = 1;
}

void VM_com_name_set(struct com_t *pcom, char *name)
{
	strPrint(pcom->name, name);
}
char *VM_com_name_get(struct com_t *pcom)
{
	return pcom->name;
}

void buff_clear(char *buff)
{
	buff[0] = 0;
}

void VM_com_CMDOut_free(struct com_t *pcom)
{
	pcom->CMDOut[0] = 0;
}

char *VM_com_TX_buff_pget(struct com_t *pcom)
{
	return pcom->TX_buff;
}

void VM_com_init(struct com_t *pcom)
{
	pcom->RX_buff_i = 0;
	pcom->RX_buff[0] = 0;
	pcom->RX_Char[0] = 0;
	pcom->RX_IT = 0;
	pcom->RX_IT_SingleLine = 0;
	pcom->RX_SingleLine[0] = 0;

	pcom->name[0] = 0;
	pcom->CMDOut[0] = 0;
	pcom->loger = dataLog_init();

	buff_clear(VM_com_RX_buff_pget(pcom));
	buff_clear(VM_com_TX_buff_pget(pcom));
	VM_com_TX_vIT_set(pcom, 0);
	buff_clear(VM_com_RX_buff_pget(pcom));
}

void com1_SingleLine_callBack_process(struct com_t *pcom)
{
	// Push shell RX_SingleLine to shell core
	char *strOut;
	DMEM *memOut;
	memOut = (DMEM *)(PubSh->cmd(PubSh, VIM_com_RX_SingleLine_pget(pcom)));
	strOut = (char *)(memOut->addr);
	VM_UART_Transmit(pcom, strOut);
	DynMemPut(memOut);
	VM_com_SingleLine_free(pcom);
}

void com2_SingleLine_callBack_process(struct com_t *pcom)
{
	Pub_iot->callBack(Pub_iot, (void *)pcom->RX_SingleLine);
	// repost rx SingleLine to uart1
	VM_UART_Transmit(&VM.com1, "repost from uart2 >");
	VM_UART_Transmit(&VM.com1, pcom->RX_SingleLine);
	VM_UART_Transmit(&VM.com1, "\r\n");
}

void com3_SingleLine_callBack_process(struct com_t *pcom)
{
	// repost rx SingleLine to uart1
	VM_UART_Transmit(&VM.com1, "repost from uart3 >");
	VM_UART_Transmit(&VM.com1, pcom->RX_SingleLine);
	VM_UART_Transmit(&VM.com1, "\r\n");
}

void VM_com_TX_buff_free(struct com_t *pcom)
{
	for (int i = 0; i < strGetSize(pcom->TX_buff); i++)
	{
		pcom->TX_buff[i] = 0;
	}
}

char VM_com_IT_SingleLine_get(struct com_t *pcom)
{
	return pcom->RX_IT_SingleLine;
}
void VM_com_IT_SingleLine_set(struct com_t *pcom, char var)
{
	pcom->RX_IT_SingleLine = var;
}

static void VM_com_SingleLine_load(struct com_t *pcom)
{
	VIM_com_RX_SingleLine_pget(pcom)[0] = 0;
	VIM_com_CMDOut_pget(pcom)[0] = 0;

	int j;

	// Load ShellCMD from buff;
	for (j = 0; j < VM_com_RX_buff_i_get(pcom); j++)
	{
		VIM_com_RX_SingleLine_pget(pcom)[j] = VM_com_RX_buff_pget(pcom)[j];
	}
	// add '\0' to the end of RX_SingleLine
	VIM_com_RX_SingleLine_pget(pcom)[j] = 0;

	VM_com_IT_SingleLine_set(pcom, 1);
	*(VM_com_RX_buff_i_pget(pcom)) = 0;
}

void VM_com_RX_SingleLine_process(struct com_t *pcom)
{
	int RX_buff_i = *(VM_com_RX_buff_i_pget(pcom));

	// GPS_IT(VM_com_RX_Char_get(pcom));

	// return if there is too much rx
	if (!(RX_buff_i < 256))
	{
		return;
	}

	if ((VM_com_RX_Char_get(pcom) != '\r') && (VM_com_RX_Char_get(pcom) != '\n'))
	{
		VM_com_RX_buff_pget(pcom)[VM_com_RX_buff_i_get(pcom)] = VM_com_RX_Char_get(pcom);
		*(VM_com_RX_buff_i_pget(pcom)) += 1;
	}
	if (VM_com_RX_Char_get(pcom) == '\r')
	{
		VM_com_SingleLine_load(pcom);
		for (int i = 0; i < 256; i++)
		{
			pcom->RX_buff[i] = 0;
		}
	}
}

// VM_driver, only the VM_driver can operate the hardware
void VMdriver_Uart_init()
{
#ifndef LINUX
	HAL_UART_Receive_IT(&huart1, (unsigned char *)VM_com_RX_Char_pget(&VM.com1), 1); //使能串口接收中断
	HAL_UART_Receive_IT(&huart2, (unsigned char *)VM_com_RX_Char_pget(&VM.com2), 1); //使能串口接收中断
	HAL_UART_Receive_IT(&huart3, (unsigned char *)VM_com_RX_Char_pget(&VM.com3), 1); //使能串口接收中断
#endif

	VM_com_init(&VM.com1);
	VM_com_init(&VM.com2);
	VM_com_init(&VM.com3);

	VM_com_name_set(&VM.com1, "com1");
	VM_com_name_set(&VM.com2, "com2");
	VM_com_name_set(&VM.com3, "com3");
}

void VMdriver_com_Transmit(struct com_t *pcom)
{
#ifndef LINUX
	if (pcom == &VM.com1)
	{
		HAL_UART_Transmit(&huart1, (unsigned char *)VM_com_TX_buff_get(pcom), strGetSize(VM_com_TX_buff_get(pcom)), 100);
	}
	if (pcom == &VM.com2)
	{
		HAL_UART_Transmit(&huart2, (unsigned char *)VM_com_TX_buff_get(pcom), strGetSize(VM_com_TX_buff_get(pcom)), 100);
	}
	if (pcom == &VM.com3)
	{
		HAL_UART_Transmit(&huart3, (unsigned char *)VM_com_TX_buff_get(pcom), strGetSize(VM_com_TX_buff_get(pcom)), 100);
	}
#endif
#ifdef LINUX
	if (pcom != &VM.com1)
	{
		printf("(VM%s)", pcom->name);
	}
	printf("%s", VM_com_TX_buff_get(pcom));
	char buffer[256] = {0};
	sprintf(buffer, "[VM%s]: \r\n", pcom->name);
	pcom->loger->print(pcom->loger, buffer);
	pcom->loger->print(pcom->loger, VM_com_TX_buff_get(pcom));
	pcom->loger->print(pcom->loger, "\r\n");

#endif
}

void VM_UART_Transmit_direct(struct com_t *pcom, char *pData)
{
	VM_UART_Transmit(pcom, pData);
	VMdriver_com_Transmit(pcom);
}

int VM_IT_check(int it_name, int it_name_defined)
{
	return (it_name == it_name_defined);
}

char *VM_usart_send_by_comName(char *com_name, char *pData, char *strOut)
{
	char *usart_TX_buff;
	char *pUsart_TX_uIT;
	if (!((strcmp(com_name, "com1") == 0) || (strcmp(com_name, "com2") == 0) || (strcmp(com_name, "com3") == 0)))
	{
		strPrint(strOut, "[error] com [");
		strPrint(strOut, com_name);
		strPrint(strOut, "] no found.\r\n");
		return strOut;
	}
	if (strcmp(com_name, "com1") == 0)
	{
		usart_TX_buff = VM_com_TX_buff_get(&VM.com1);
		pUsart_TX_uIT = VM_com_TX_vIT_pget(&VM.com1);
	}
	if (strcmp(com_name, "com2") == 0)
	{
		usart_TX_buff = VM_com_TX_buff_get(&VM.com2);
		pUsart_TX_uIT = VM_com_TX_vIT_pget(&VM.com2);
	}
	if (strcmp(com_name, "com3") == 0)
	{
		usart_TX_buff = VM_com_TX_buff_get(&VM.com3);
		pUsart_TX_uIT = VM_com_TX_vIT_pget(&VM.com3);
	}
	// push string to the hardware buffer
	strPrint(usart_TX_buff, pData);
	strPrint(usart_TX_buff, "\r\n");

	// print debug info
	strPrint(strOut, "have send data to ");
	strPrint(strOut, com_name);
	strPrint(strOut, " >");
	strPrint(strOut, usart_TX_buff);

	// set user IT to hardwore buffer to get start
	*pUsart_TX_uIT = 1;

	return strOut;
}
// VM_vIT function of VM server, the VM_vIT should be called in highest priority, to makesure the performance of hardware.
// VM_vIT can call the hardware api, such as stm32_hal api.
// VM_vIT can only operate VM api, cant operate VM memory directly.
// VM_vIT process the virtual IT, witch is triggered by VM_memory.
void VM_vIT(void)
{
	VM_com_TX_vIT_CallBack(&VM.com1);
	VM_com_TX_vIT_CallBack(&VM.com2);
	VM_com_TX_vIT_CallBack(&VM.com3);

	VM_com_IT_SingleLine_callBack_task(&VM.com2);
	VM_com_IT_SingleLine_callBack_task(&VM.com3);
}

// VM_IT
// The VM_IT function sould be called in hardwore IT call_back function.
void VM_IT(int it_name)
{
	if (VM_IT_check(it_name, VM_IT_COM1))
	{
		VM_com_RX_SingleLine_process(&VM.com1);
	}
	if (VM_IT_check(it_name, VM_IT_COM2))
	{
		VM_com_RX_SingleLine_process(&VM.com2);
	}
	if (VM_IT_check(it_name, VM_IT_COM3))
	{
		VM_com_RX_SingleLine_process(&VM.com3);
	}
}

// VM task, the spitial api,should be run in task level
void VM_com_IT_SingleLine_callBack_task(struct com_t *pcom)
{
	if (VM_com_IT_SingleLine_get(pcom))
	{
		if (&VM.com1 == pcom)
		{
			com1_SingleLine_callBack_process(pcom);
		}

		if (&VM.com2 == pcom)
		{
			com2_SingleLine_callBack_process(pcom);
		}

		if (&VM.com3 == pcom)
		{
			com3_SingleLine_callBack_process(pcom);
		}

		VM_com_IT_SingleLine_set(pcom, 0);
	}
}

// VM_main, is the main funcion of VM, should run in task level, such as the while(1) in main().
void VM_main(void)
{
	VM_com_IT_SingleLine_callBack_task(&VM.com1);
}

void VM_com_TX_vIT_CallBack(struct com_t *pcom)
{
	if (VM_com_TX_vIT_get(pcom))
	{
		VMdriver_com_Transmit(pcom);
		VM_com_TX_buff_free(pcom);
		VM_com_TX_vIT_set(pcom, 0);
	}
}

// Hardware_IT should only call the VM_IT, or VM_vIT
#ifndef LINUX
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1)
	{
		VM_IT(VM_IT_COM1);
		HAL_UART_Receive_IT(&huart1, (unsigned char *)VM_com_RX_Char_pget(&VM.com1), 1);
	}

	if (huart->Instance == USART2)
	{
		VM_IT(VM_IT_COM2);
		HAL_UART_Receive_IT(&huart2, (unsigned char *)VM_com_RX_Char_pget(&VM.com2), 1);
	}
	if (huart->Instance == USART3)
	{
		VM_IT(VM_IT_COM3);
		HAL_UART_Receive_IT(&huart3, (unsigned char *)VM_com_RX_Char_pget(&VM.com3), 1);
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim == (&htim2))
	{
		VM_vIT();

		// VM_vIT() should be runed in IT server, like tim IT, to improve real time peference.
	};
}
#endif
