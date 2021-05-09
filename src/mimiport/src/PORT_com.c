#include <stdio.h>
#include "mimiStr.h"
#ifndef LINUX
#include "usart.h"
#include "main.h"
#endif

void PORT_send_to_com(int com_num, char *str)
{
#ifndef LINUX
		if(2 == com_num)
		{
			HAL_UART_Transmit(&huart2, (unsigned char *)str, strGetSize(str), 100);
		}	
	
		if(3 == com_num)
		{
			HAL_UART_Transmit(&huart3, (unsigned char *)str, strGetSize(str), 100);
		}
		
#endif
	
#ifdef LINUX
    printf("[com%d]: %s", com_num, str);
#endif
}