/* file:mimiPort.c 
 * The mimiPort.c is used to connect the port of mimi compoment, such as conniect
 * the usart.c to VM_com.c, so the VM compoment can be hardware independent.
 * This mimiPort.c is used in HAL lib of STM32.
*/
#ifndef LINUX
#include "gpio.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#endif

#ifdef LINUX
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#endif

#include "VM_delay.h"
#include "VM_gui.h"
#include "VM_key.h"
#include "VM_lcd.h"
#include "mimiPort_market.h"

static void gui_show_info(int line, int cols, char *str);

static int PORT_pressGet_key_0(void)
{
	int out = 0;
#ifndef LINUX
	out = (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0));
#endif
	return out;
};

static int PORT_pressGet_key_1(void)
{
	int out = 0;
#ifndef LINUX
	out = !(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_9));
#endif
	return out;
};

static int PORT_pressGet_key_2(void)
{
	int out = 0;
#ifndef LINUX
	out = !(HAL_GPIO_ReadPin(GPIOC, GPIO_PIN_8));
#endif
	return out;
};

vkey_t *VM_key_init_PORT(int KEY_id)
{
	vkey_t *key = VM_key_init();
	switch (KEY_id)
	{
	case 0:
		key->PORT_pressGet = PORT_pressGet_key_0;
		break;
	case 1:
		key->PORT_pressGet = PORT_pressGet_key_1;
		break;
	case 2:
		key->PORT_pressGet = PORT_pressGet_key_2;
		break;
	}
	return key;
}

void PORT_reboot(void)
{
#ifndef LINUX
	HAL_NVIC_SystemReset();
#endif
}

static void _PORT_showLine_PORT(VMgui_t *gui, int line, int cols, char *str)
{
	// the map of line to x,y is for linux, rewrite it in other platform, shuch as lcd.
#ifndef LINUX
	int x = 0;
	int y = 0;
	x = cols * 6;
	y = line * 15 + 10;
	gui->showString(gui, x, y, str);
#endif
#ifdef LINUX
	gui->showString(gui, cols, line, str);
#endif
}

VMgui_t *VM_gui_init_PORT(void)
{
	VMgui_t *gui = VM_gui_init();
	gui->_PORT_clear = _PORT_clear;
	gui->_PORT_showString = _PORT_showString;
	gui->_PORT_showLine = _PORT_showLine_PORT;
	return gui;
}

void _PORT_clear(void)
{
#ifndef LINUX
	LCD_Fill(0, 0, LCD_W, LCD_H, WHITE);
#endif
#ifdef LINUX
	for (int i = 0; i < 9; i++)
	{
		gui_show_info(i, 0, "                       ");
	}
#endif
}

char PORT_VM_terminal_getChar_linux(void)
{

	int ch = 0;
#ifdef LINUX
	fd_set rfds;
	struct timeval tv;
	FD_ZERO(&rfds);
	FD_SET(0, &rfds);
	tv.tv_sec = 0;
	tv.tv_usec = 10; //设置等待超时时间
	//检测键盘是否有输入
	if (select(1, &rfds, NULL, NULL, &tv) > 0)
	{
		ch = getchar();
	}
#endif
	return ch;
}

void PORT_VM_terminal_exit_linux(void)
{
#ifdef LINUX
	system(STTY_DEF TTY_PATH);
	system("tput cnorm");
	_exit(0);
#endif
}

static void gui_show_info(int line, int cols, char *str)
{
#ifdef LINUX
	char buff[256] = {0};
	system("tput sc");
	sprintf(buff, "tput cup %d %d", line, cols);
	system(buff);
	sprintf(buff, "echo \"%s\r\"", str);
	system(buff);
	system("tput rc");
#endif
}

static void delay_PORT(struct VM_delay *vd, int delay_ms)
{
#ifdef LINUX
	printf("[sys]: delay : %d\r\n", delay_ms);

// printf(",delay_ms);
#endif
#ifndef LINUX
	HAL_Delay(delay_ms);
#endif
}

struct VM_delay *VM_delay_init_PORT(void)
{
	struct VM_delay *delay = VM_delay_init();
	delay->delay = delay_PORT;
	return delay;
}

static void socket_hardware_update_0(VM_socket_t *socket)
{
	if (0 == socket->isOpen)
	{
#ifndef LINUX
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
#endif
		// close the socket by hardware
		if (socket->isShowInfo)
		{
#ifdef LINUX
			printf("close the socket 0!\r\n");
#endif
		}
	}
	if (1 == socket->isOpen)
	{
#ifndef LINUX
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
#endif
		// open the socket by hardware
		if (socket->isShowInfo)
		{
#ifdef LINUX
			printf("open the socket 0!\r\n");
#endif
		}
	}
}

static void socket_hardware_update_1(VM_socket_t *socket)
{
	if (0 == socket->isOpen)
	{
#ifndef LINUX
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_SET);
#endif
		// close the socket by hardware
		if (socket->isShowInfo)
		{
#ifdef LINUX
			printf("close the socket 1!\r\n");
#endif
		}
	}
	if (1 == socket->isOpen)
	{
#ifndef LINUX
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, GPIO_PIN_RESET);
#endif
		// open the socket by hardware
		if (socket->isShowInfo)
		{
#ifdef LINUX
			printf("open the socket 1!\r\n");
#endif
		}
	}
}

VM_socket_t *VM_socket_init_PORT(int socket_ID)
{
	VM_socket_t *socket = VM_socket_init();

	if (0 == socket_ID)
	{
		socket->hardware_update = socket_hardware_update_0;
	}

	if (1 == socket_ID)
	{
		socket->hardware_update = socket_hardware_update_1;
	}
	return socket;
}

void _PORT_showString(int x, int y, char *string)
{
#ifndef LINUX
	LCD_ShowString(x, y, (unsigned char *)string, RED, WHITE, 12, 0);
#endif
#ifdef LINUX
	// LCD_ShowString(x, y, (unsigned char *)string, RED, WHITE, 12, 0);
	char buff[256] = {0};
	system("tput sc");
	sprintf(buff, "tput cup %d %d", y, x);
	system(buff);
	sprintf(buff, "echo \"%s\r\"", string);
	system(buff);
	system("tput rc");
	// sprintf(buff, "to com1 %s\r\n", string);
	// DynMemPut((DMEM *)PubSh->cmd(PubSh, buff));
#endif
}
