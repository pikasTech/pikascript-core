#ifndef LINUX
#include "gpio.h"
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "VM_lcd.h"
#endif

#ifdef LINUX
#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/time.h>
#include <unistd.h>
#endif

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

void PORT_gui_clear(void)
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

void PORT_gui_showXY(int x, int y, char *string)
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