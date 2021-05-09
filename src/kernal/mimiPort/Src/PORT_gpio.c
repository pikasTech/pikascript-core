#include "PORT_gpio.h"
#include <stdio.h>

#ifndef LINUX
#include "main.h"
#include "gpio.h"
#endif

void PORT_gpio_set(int portGroup, int portNum, int val)
{
#ifdef LINUX
    if (PORT_GPIO_GROUP_A == portGroup)
    {
        printf("[GPIO A]: ");
    }
    if (PORT_GPIO_GROUP_B == portGroup)
    {
        printf("[GPIO B]: ");
    }
    printf("set port%d to %d\r\n", portNum, val);
#endif
#ifndef LINUX
		uint16_t GPIO_PIN;
	
		if(0 == portNum)
		{
			GPIO_PIN = GPIO_PIN_0;
		}
		if(1 == portNum)
		{
			GPIO_PIN = GPIO_PIN_1;
		}		
		if(2 == portNum)
		{
			GPIO_PIN = GPIO_PIN_2;
		}	
		if(3 == portNum)
		{
			GPIO_PIN = GPIO_PIN_3;
		}		
		if(4 == portNum)
		{
			GPIO_PIN = GPIO_PIN_4;
		}		
		if(5 == portNum)
		{
			GPIO_PIN = GPIO_PIN_5;
		}		
		if(6 == portNum)
		{
			GPIO_PIN = GPIO_PIN_6;
		}		
		if(7 == portNum)
		{
			GPIO_PIN = GPIO_PIN_7;
		}		
		if(8 == portNum)
		{
			GPIO_PIN = GPIO_PIN_8;
		}		
		if(9 == portNum)
		{
			GPIO_PIN = GPIO_PIN_9;
		}		
		if(10 == portNum)
		{
			GPIO_PIN = GPIO_PIN_10;
		}		
		if(11 == portNum)
		{
			GPIO_PIN = GPIO_PIN_11;
		}			
		if(12 == portNum)
		{
			GPIO_PIN = GPIO_PIN_12;
		}		
		if(13 == portNum)
		{
			GPIO_PIN = GPIO_PIN_13;
		}		
		if(14 == portNum)
		{
			GPIO_PIN = GPIO_PIN_14;
		}		
		if(15 == portNum)
		{
			GPIO_PIN = GPIO_PIN_15;
		}				
    if (PORT_GPIO_GROUP_A == portGroup)
    {

			HAL_GPIO_WritePin(GPIOA,GPIO_PIN,val);
    }
    if (PORT_GPIO_GROUP_B == portGroup)
    {
			HAL_GPIO_WritePin(GPIOB,GPIO_PIN,val);
    }
#endif
}