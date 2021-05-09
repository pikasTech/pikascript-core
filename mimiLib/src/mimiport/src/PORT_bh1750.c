#include "PORT_bh1750.h"
#ifndef LINUX
#include "main.h"
#include "tim.h"
#endif
#include <stdio.h>
#include <stdlib.h>

#ifndef LINUX
static void IIC_SDA_input(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

static void IIC_SDA_output(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	GPIO_InitStruct.Pin = GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

typedef unsigned char BYTE;
//BYTE    BUF[8];                         //接收数据缓存区
//iic接口初始化

void Single_Write_BH1750(uchar REG_Address)
{
	IIC_Start();				//起始信号
	IIC_Send_Byte(BHAddWrite);	//发送设备地址+写信号
	IIC_Send_Byte(REG_Address); //内部寄存器地址，
								//  BH1750_SendByte(REG_data);       //内部寄存器数据，
	IIC_Stop();					//发送停止信号
}

//产生IIC起始信号
void IIC_Start(void)
{
	SDA_OUT(); //sda线输出
	IIC_SDA_SET;
	IIC_SCL_SET;
	delay_us(4);
	IIC_SDA_RESET; //START:when CLK is high,DATA change form high to low
	delay_us(4);
	IIC_SCL_RESET; //钳住I2C总线，准备发送或接收数据
}
//产生IIC停止信号
void IIC_Stop(void)
{
	SDA_OUT(); //sda线输出
	IIC_SCL_RESET;
	IIC_SDA_RESET; //STOP:when CLK is high DATA change form low to high
	delay_us(4);
	IIC_SCL_SET;
	IIC_SDA_SET; //发送I2C总线结束信号
	delay_us(4);
}
//等待应答信号到来
//返回值：1，接收应答失败
//        0，接收应答成功
unsigned char IIC_Wait_Ack(void)
{
	unsigned char ucErrTime = 0;
	SDA_IN(); //SDA设置为输入
	IIC_SDA_SET;
	delay_us(1);
	IIC_SCL_SET;
	delay_us(1);
	while (READ_SDA)
	{
		ucErrTime++;
		if (ucErrTime > 250)
		{
			IIC_Stop();
			return 1;
		}
	}
	IIC_SCL_RESET; //时钟输出0
	return 0;
}
//产生ACK应答
void IIC_Ack(void)
{
	IIC_SCL_RESET;
	SDA_OUT();
	IIC_SDA_RESET;
	delay_us(2);
	IIC_SCL_SET;
	delay_us(2);
	IIC_SCL_RESET;
}
//不产生ACK应答
void IIC_NAck(void)
{
	IIC_SCL_RESET;
	SDA_OUT();
	IIC_SDA_SET;
	delay_us(2);
	IIC_SCL_SET;
	delay_us(2);
	IIC_SCL_RESET;
}
//IIC发送一个字节
//返回从机有无应答
//1，有应答
//0，无应答
void IIC_Send_Byte(unsigned char txd)
{
	unsigned char t;
	SDA_OUT();
	IIC_SCL_RESET; //拉低时钟开始数据传输
	for (t = 0; t < 8; t++)
	{
		//IIC_SDA=(txd&0x80)>>7;
		if ((txd & 0x80) >> 7)
			IIC_SDA_SET;
		else
			IIC_SDA_RESET;
		txd <<= 1;
		delay_us(2); //对TEA5767这三个延时都是必须的
		IIC_SCL_SET;
		delay_us(2);
		IIC_SCL_RESET;
		delay_us(2);
	}
}
//读1个字节，ack=1时，发送ACK，ack=0，发送nACK
unsigned char IIC_Read_Byte(unsigned char ack)
{
	unsigned char i, receive = 0;
	SDA_IN(); //SDA设置为输入
	for (i = 0; i < 8; i++)
	{
		IIC_SCL_RESET;
		delay_us(2);
		IIC_SCL_SET;
		receive <<= 1;
		if (READ_SDA)
			receive++;
		delay_us(1);
	}
	if (!ack)
		IIC_NAck(); //发送nACK
	else
		IIC_Ack(); //发送ACK
	return receive;
}

/*************************************************************************************/
#endif

void PORT_bh_data_send(unsigned char command)
{
#ifndef LINUX
	do
	{
		IIC_Start();			   //iic起始信号
		IIC_Send_Byte(BHAddWrite); //发送器件地址
	} while (IIC_Wait_Ack());	   //等待从机应答
	IIC_Send_Byte(command);		   //发送指令
	IIC_Wait_Ack();				   //等待从机应答
	IIC_Stop();					   //iic停止信号
#endif
#ifdef LINUX
	printf("send to bh1750 %d\r\n", command);
#endif
}

unsigned short int PORT_bh_data_read(void)
{
#ifndef LINUX
	unsigned short int buf;
	IIC_Start();					  //iic起始信号
	IIC_Send_Byte(BHAddRead);		  //发送器件地址+读标志位
	IIC_Wait_Ack();					  //等待从机应答
	buf = IIC_Read_Byte(1);			  //读取数据
	buf = buf << 8;					  //读取并保存高八位数据
	buf += 0x00ff & IIC_Read_Byte(0); //读取并保存第八位数据
	IIC_Stop();						  //发送停止信号
	return buf;
#endif
#ifdef LINUX
	unsigned short int buf = 55;
	return buf;
#endif
}

void PORT_bh_Init(void)
{
#ifndef LINUX
	Single_Write_BH1750(0x01);
#endif
#ifdef LINUX
	printf("init bh1750_PORT\r\n");
#endif
}
