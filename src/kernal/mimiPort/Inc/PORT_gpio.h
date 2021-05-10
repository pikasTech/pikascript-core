#ifndef _PORT_gpio__H
#define _PORT_gpio__H
#define PORT_GPIO_GROUP_A  1
#define PORT_GPIO_GROUP_B  2

void PORT_gpio_set(int portGroup, int portNum, int val);

#endif