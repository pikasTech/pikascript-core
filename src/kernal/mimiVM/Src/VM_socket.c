#include "VM_socket.h"
#include "VM_iot.h"
#include "dataMemory.h"
#include "dataString.h"
#include "mimiVM_core.h"
//#include "mimiSH_core.h"

//extern sh_t * PubSh;

 VM_socket_t *socket_1;
 VM_socket_t *socket_2;

static void VM_iot_callBack_socket(iot_t *iot, void *data);

static struct VM_iot *VM_iot_Init_socket(void)
{
	iot_t *iot = VM_iot_Init();
	iot->callBack = VM_iot_callBack_socket;
	return iot;
}

// need to be redefine in application software
static void hardware_update_default(VM_socket_t *socket)
{
	if (0 == socket->isOpen)
	{
		// close the socket by hardware
		if (socket->isShowInfo)
		{
			printf("close the socket!\r\n");
		}
	}
	if (1 == socket->isOpen)
	{
		// open the socket by hardware
		if (socket->isShowInfo)
		{
			printf("open the socket!\r\n");
		}
	}
}

static void DeInit(VM_socket_t *socket)
{
	DynMemPut(socket->mem);
}

static void open(VM_socket_t *socket)
{
	socket->isOpen = 1;
	socket->hardware_update(socket);
}

static void close(VM_socket_t *socket)
{
	socket->isOpen = 0;
	socket->hardware_update(socket);
}

static void openTimeSet(VM_socket_t *socket, int time_s)
{
	socket->openTime = time_s;
}

static void callback_1s(VM_socket_t *socket)
{
	struct VM_iot *iot = VM_iot_Init_socket();

	//	char buff[256];
	//	sprintf(buff,"to com1 the time opened is: %d\r\n",socket->Time_opened);
	//	PubSh->cmd(PubSh,buff);

	if (socket->openTime > 0)
	{
		socket->openTime--;
		socket->open(socket);

		unsigned char openTimeStr[] = {0x00, 0x00};
		openTimeStr[0] = (unsigned char)(socket->openTime >> 8);
		openTimeStr[1] = (unsigned char)socket->openTime;

		iot->data_upload(iot, socket->iot_addr, openTimeStr);
		iot->delay(iot, 100);

		if (0 == socket->openTime)
		{
			socket->close(socket);
		}
	}

	iot->DeInit(iot);
}

static void Time_opened_update(struct VM_socket *socket)
{

	struct VM_iot *iot = VM_iot_Init_socket();

	//	char buff[256];
	//	sprintf(buff,"to com1 the time opened is: %d\r\n",socket->Time_opened);
	//	PubSh->cmd(PubSh,buff);

	if (socket->isOpen)
	{
		// add per 1 second
		socket->Time_opened += 5;
		unsigned char openedTimeStr[] = {0x00, 0x00};
		openedTimeStr[0] = (unsigned char)(socket->Time_opened >> 8);
		openedTimeStr[1] = (unsigned char)socket->Time_opened;
		iot->data_upload(iot, socket->iot_openTime_addr, openedTimeStr);
		iot->delay(iot, 100);
	}
	iot->DeInit(iot);
}

void iot_data_init(struct VM_socket *socket)
{
	struct VM_iot *iot = VM_iot_Init_socket();

	unsigned char data_void[] = {0x00, 0x00};

	iot->data_upload(iot, socket->iot_openTime_addr, data_void);
	iot->delay(iot, 500);
	iot->data_upload(iot, socket->iot_addr, data_void);
	iot->delay(iot, 500);
	iot->bit_upload(iot, socket->iot_bit_addr, data_void);
	iot->delay(iot, 500);
	iot->DeInit(iot);
}

static void VM_iot_callBack_socket(iot_t *iot, void *data)
{
	char *strGet = (char *)data;

	if (isStartWith(strGet, "8,0106"))
	{
		int addr;
		int val;
		char buff[256] = {0};
		iot->send(iot, "AT+NMGS=");
		iot->send(iot, strGet);
		iot->send(iot, "\r\n");
		iot->info(iot, "get the message of time: [");
		iot->info(iot, strGet);
		iot->info(iot, "]\r\n");
		val = iot->get_value_function_06(iot, strGet);
		addr = iot->get_addr_function_06(iot, strGet);
		sprintf(buff, "the val of message is: %d, the addr of message is %d\r\n", val, addr);
		iot->info(iot, buff);
		VM_socket_t *socket;
		if (2 == addr)
		{
			socket = socket_1;
		}
		if (3 == addr)
		{
			socket = socket_2;
		}
		socket->openTimeSet(socket, val);
	}
	if (0 == strcmp("8,010500000000CDCA", strGet))
	{
		iot->send(iot, "AT+NMGS=8,010500000000CDCA\r\n");
		socket_1->close(socket_1);
		VM_UART_Transmit(&VM.com1, "get : set the var to 0\r\n");
	}
	if (0 == strcmp("8,01050000FF008C3A", strGet))
	{
		iot->send(iot, "AT+NMGS=8,01050000FF008C3A\r\n");
		socket_1->open(socket_1);
		VM_UART_Transmit(&VM.com1, "get : set the var to 1\r\n");
	}
	if (0 == strcmp("8,0105000100009C0A", strGet))
	{
		iot->send(iot, "AT+NMGS=8,0105000100009C0A\r\n");
		socket_2->close(socket_2);
		VM_UART_Transmit(&VM.com1, "get : set the var to 0\r\n");
	}
	if (0 == strcmp("8,01050001FF00DDFA", strGet))
	{
		iot->send(iot, "AT+NMGS=8,01050001FF00DDFA\r\n");
		socket_2->open(socket_2);
		VM_UART_Transmit(&VM.com1, "get : set the var to 0\r\n");
	}
}

VM_socket_t *VM_socket_init(void)
{
	DMEM *mem = DynMemGet(sizeof(VM_socket_t));
	VM_socket_t *socket = (VM_socket_t *)mem->addr;
	socket->mem = mem;
	socket->openTime = 0;
	socket->Time_opened = 0;
	socket->DeInit = DeInit;
	socket->isOpen = 0;
	socket->open = open;
	socket->close = close;
	socket->hardware_update = hardware_update_default;
	socket->isShowInfo = 0;
	socket->openTimeSet = openTimeSet;
	socket->callback_1s = callback_1s;
	socket->Time_opened_update = Time_opened_update;
	socket->iot_addr[0] = 0x00;
	socket->iot_addr[1] = 0x00;
	socket->iot_bit_addr[0] = 0x00;
	socket->iot_bit_addr[1] = 0x00;
	socket->iot_openTime_addr[0] = 0x00;
	socket->iot_openTime_addr[1] = 0x00;
	socket->iot_data_init = iot_data_init;

	socket->context= socket;

	return socket;
}