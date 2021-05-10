#ifndef __PORT_BH1750_H
#define __PORT_BH1750_H

#define BHAddWrite     0x46      //从机地址+最后写方向位
#define BHAddRead      0x47      //从机地址+最后读方向位
#define BHPowDown      0x00      //关闭模块
#define BHPowOn        0x01      //打开模块等待测量指令
#define BHReset        0x07      //重置数据寄存器值在PowerOn模式下有效
#define BHModeH1       0x10      //高分辨率 单位1lx 测量时间120ms
#define BHModeH2       0x11      //高分辨率模式2 单位0.5lx 测量时间120ms
#define BHModeL        0x13      //低分辨率 单位4lx 测量时间16ms
#define BHSigModeH     0x20      //一次高分辨率 测量 测量后模块转到 PowerDown模式
#define BHSigModeH2    0x21      //同上类似
#define BHSigModeL     0x23      // 上类似


#ifndef LINUX
#include "main.h"
//IO方向设置
 
#define SDA_IN() 		IIC_SDA_input() //{GPIOD->CRL&=0X0FFFFFFF;GPIOD->CRL|=(u32)8<<28;}
#define SDA_OUT() 	IIC_SDA_output()//{GPIOD->CRL&=0X0FFFFFFF;GPIOD->CRL|=(u32)3<<28;}

#define IIC_SCL_SET  		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_SET)  //PDout(6) //SCL
#define IIC_SCL_RESET 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET)  //PDout(6) //SCL

#define IIC_SDA_SET  		HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_SET) //PDout(6) //SCL
#define IIC_SDA_RESET 	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_7,GPIO_PIN_RESET)  //PDout(6) //SCL

#define READ_SDA  HAL_GPIO_ReadPin(GPIOA,GPIO_PIN_7) //PDin(7)  //输入SDA 

#define ADDR 0x23//0100011
#define uchar unsigned char 

//IIC所有操作函数
void IIC_Init(void);                //初始化IIC的IO口				 
void IIC_Start(void);				//发送IIC开始信号
void IIC_Stop(void);	  			//发送IIC停止信号
void IIC_Send_Byte(unsigned char txd);			//IIC发送一个字节
unsigned char IIC_Read_Byte(unsigned char ack);//IIC读取一个字节
unsigned char IIC_Wait_Ack(void); 				//IIC等待ACK信号
void IIC_Ack(void);					//IIC发送ACK信号
void IIC_NAck(void);				//IIC不发送ACK信号

void IIC_Write_One_Byte(unsigned char daddr,unsigned char addr,unsigned char data);
unsigned IIC_Read_One_Byte(unsigned char daddr,unsigned char addr);	
void Single_Write_BH1750(uchar REG_Address);

#endif

void PORT_bh_Init(void);
void PORT_bh_data_send(unsigned char command);
unsigned short int PORT_bh_data_read(void);

#endif
