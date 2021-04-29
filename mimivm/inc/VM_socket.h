#ifndef __VM_SOCKET_H
#define __VM_SOCKET_H
#include "VM_memory.h"
typedef struct VM_socket
{
	void(*DeInit)(struct VM_socket *socket);
	DMEM *mem;
	int isShowInfo;
	int isOpen;
	int openTime;
	// the time have opened
	int Time_opened;
	void(*open)(struct VM_socket *socket);
	void(*close)(struct VM_socket *socket);
	void(*hardware_update)(struct VM_socket *socket);
	void(*openTimeSet)(struct VM_socket *socekt, int time_s);
	void(*callback_1s)(struct VM_socket *socket);
	// update per second
	void(*Time_opened_update)(struct VM_socket *socket);
	unsigned char iot_addr[2];
	unsigned char iot_bit_addr[2];
	unsigned char iot_openTime_addr[2];
	void(*iot_data_init)(struct VM_socket *socket);

	void *context;

}VM_socket_t;


VM_socket_t * VM_socket_init(void);
#endif