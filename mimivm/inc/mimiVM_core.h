#ifndef __VM_STM32__

#define __VM_STM32__
#include "dataLog.h"
#ifndef LINUX
#include "main.h"
#endif

#define VM_IT_COM1 1
#define VM_IT_COM2 2
#define VM_IT_COM3 3

struct com_t
{
	char RX_Char[1];

	char RX_IT;
	int RX_buff_i;
	char RX_buff[256];
	char RX_SingleLine[256];
	char RX_IT_SingleLine;

	char TX_buff[256];
	char TX_vIT;
	char name[16];
	char CMDOut[256];

	loger_t *loger;
};

struct VM_t
{
	struct com_t com1;
	struct com_t com2;
	struct com_t com3;
};

extern struct VM_t VM;

void VMdriver_Uart_init(void);
char *VM_com_RX_Char_pget(struct com_t *pcom);
void VM_UART_Transmit(struct com_t *pcom, char *pData);
int TEST_VM_com_test_list(int isShow);
char *VM_usart_send_by_comName(char *com_name, char *pData, char *strOut);
void VM_UART_Transmit_direct(struct com_t *pcom, char *pData);
void VM_IT(int it_name);
void VM_vIT(void);
void VM_main(void);


#endif
