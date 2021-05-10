#include "VM_lcd.h"
#include "VM_delay.h"

int IO_operation_virtual(void)
{
	return 0;
}

/******************************************************************************
      函数说明：在指定区域填充颜色
      入口数据：xsta,ysta   起始坐标
                xend,yend   终止坐标
								color       要填充的颜色
      返回值：  无
******************************************************************************/
void LCD_Fill(unsigned short int xsta, unsigned short int ysta, unsigned short int xend, unsigned short int yend, unsigned short int color)
{
	unsigned short int i, j;
	LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); //设置显示范围
	for (i = ysta; i < yend; i++)
	{
		for (j = xsta; j < xend; j++)
		{
			LCD_WR_DATA(color);
		}
	}
}

/******************************************************************************
      函数说明：在指定位置画点
      入口数据：x,y 画点坐标
                color 点的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawPoint(unsigned short int x, unsigned short int y, unsigned short int color)
{
	LCD_Address_Set(x, y, x, y); //设置光标位置
	LCD_WR_DATA(color);
}

/******************************************************************************
      函数说明：画线
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   线的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawLine(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2, unsigned short int color)
{
	unsigned short int t;
	int xerr = 0, yerr = 0, delta_x, delta_y, distance;
	int incx, incy, uRow, uCol;
	delta_x = x2 - x1; //计算坐标增量
	delta_y = y2 - y1;
	uRow = x1; //画线起点坐标
	uCol = y1;
	if (delta_x > 0)
		incx = 1; //设置单步方向
	else if (delta_x == 0)
		incx = 0; //垂直线
	else
	{
		incx = -1;
		delta_x = -delta_x;
	}
	if (delta_y > 0)
		incy = 1;
	else if (delta_y == 0)
		incy = 0; //水平线
	else
	{
		incy = -1;
		delta_y = -delta_y;
	}
	if (delta_x > delta_y)
		distance = delta_x; //选取基本增量坐标轴
	else
		distance = delta_y;
	for (t = 0; t < distance + 1; t++)
	{
		LCD_DrawPoint(uRow, uCol, color); //画点
		xerr += delta_x;
		yerr += delta_y;
		if (xerr > distance)
		{
			xerr -= distance;
			uRow += incx;
		}
		if (yerr > distance)
		{
			yerr -= distance;
			uCol += incy;
		}
	}
}

/******************************************************************************
      函数说明：画矩形
      入口数据：x1,y1   起始坐标
                x2,y2   终止坐标
                color   矩形的颜色
      返回值：  无
******************************************************************************/
void LCD_DrawRectangle(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2, unsigned short int color)
{
	LCD_DrawLine(x1, y1, x2, y1, color);
	LCD_DrawLine(x1, y1, x1, y2, color);
	LCD_DrawLine(x1, y2, x2, y2, color);
	LCD_DrawLine(x2, y1, x2, y2, color);
}

/******************************************************************************
      函数说明：画圆
      入口数据：x0,y0   圆心坐标
                r       半径
                color   圆的颜色
      返回值：  无
******************************************************************************/
void Draw_Circle(unsigned short int x0, unsigned short int y0, unsigned char r, unsigned short int color)
{
	int a, b;
	a = 0;
	b = r;
	while (a <= b)
	{
		LCD_DrawPoint(x0 - b, y0 - a, color); //3
		LCD_DrawPoint(x0 + b, y0 - a, color); //0
		LCD_DrawPoint(x0 - a, y0 + b, color); //1
		LCD_DrawPoint(x0 - a, y0 - b, color); //2
		LCD_DrawPoint(x0 + b, y0 + a, color); //4
		LCD_DrawPoint(x0 + a, y0 - b, color); //5
		LCD_DrawPoint(x0 + a, y0 + b, color); //6
		LCD_DrawPoint(x0 - b, y0 + a, color); //7
		a++;
		if ((a * a + b * b) > (r * r)) //判断要画的点是否过远
		{
			b--;
		}
	}
}

/******************************************************************************
      函数说明：显示单个字符
      入口数据：x,y显示坐标
                num 要显示的字符
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowChar(unsigned short int x, unsigned short int y, unsigned char num, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode)
{
	unsigned char temp, sizex, t, m = 0;
	unsigned short int i, TypefaceNum; //一个字符所占字节大小
	unsigned short int x0 = x;
	sizex = sizey / 2;
	TypefaceNum = (sizex / 8 + ((sizex % 8) ? 1 : 0)) * sizey;
	num = num - ' ';									 //得到偏移后的值
	LCD_Address_Set(x, y, x + sizex - 1, y + sizey - 1); //设置光标位置
	for (i = 0; i < TypefaceNum; i++)
	{
		if (sizey == 12)
			temp = ascii_1206[num][i]; //调用6x12字体
		else if (sizey == 16)
			temp = ascii_1608[num][i]; //调用8x16字体
		else if (sizey == 24)
			temp = ascii_2412[num][i]; //调用12x24字体
		else if (sizey == 32)
			temp = ascii_3216[num][i]; //调用16x32字体
		else
			return;
		for (t = 0; t < 8; t++)
		{
			if (!mode) //非叠加模式
			{
				if (temp & (0x01 << t))
					LCD_WR_DATA(fc);
				else
					LCD_WR_DATA(bc);
				m++;
				if (m % sizex == 0)
				{
					m = 0;
					break;
				}
			}
			else //叠加模式
			{
				if (temp & (0x01 << t))
					LCD_DrawPoint(x, y, fc); //画一个点
				x++;
				if ((x - x0) == sizex)
				{
					x = x0;
					y++;
					break;
				}
			}
		}
	}
}

/******************************************************************************
      函数说明：显示字符串
      入口数据：x,y显示坐标
                *p 要显示的字符串
                fc 字的颜色
                bc 字的背景色
                sizey 字号
                mode:  0非叠加模式  1叠加模式
      返回值：  无
******************************************************************************/
void LCD_ShowString(unsigned short int x, unsigned short int y, const unsigned char *p, unsigned short int fc, unsigned short int bc, unsigned char sizey, unsigned char mode)
{
	while (*p != '\0')
	{
		LCD_ShowChar(x, y, *p, fc, bc, sizey, mode);
		x += sizey / 2;
		p++;
	}
}

/******************************************************************************
      函数说明：显示数字
      入口数据：m底数，n指数
      返回值：  无
******************************************************************************/
unsigned int mypow(unsigned char m, unsigned char n)
{
	unsigned int result = 1;
	while (n--)
		result *= m;
	return result;
}

/******************************************************************************
      函数说明：显示整数变量
      入口数据：x,y显示坐标
                num 要显示整数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowIntNum(unsigned short int x, unsigned short int y, unsigned short int num, unsigned char len, unsigned short int fc, unsigned short int bc, unsigned char sizey)
{
	unsigned char t, temp;
	unsigned char enshow = 0;
	unsigned char sizex = sizey / 2;
	for (t = 0; t < len; t++)
	{
		temp = (num / mypow(10, len - t - 1)) % 10;
		if (enshow == 0 && t < (len - 1))
		{
			if (temp == 0)
			{
				LCD_ShowChar(x + t * sizex, y, ' ', fc, bc, sizey, 0);
				continue;
			}
			else
				enshow = 1;
		}
		LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}

/******************************************************************************
      函数说明：显示两位小数变量
      入口数据：x,y显示坐标
                num 要显示小数变量
                len 要显示的位数
                fc 字的颜色
                bc 字的背景色
                sizey 字号
      返回值：  无
******************************************************************************/
void LCD_ShowFloatNum1(unsigned short int x, unsigned short int y, float num, unsigned char len, unsigned short int fc, unsigned short int bc, unsigned char sizey)
{
	unsigned char t, temp, sizex;
	unsigned short int num1;
	sizex = sizey / 2;
	num1 = num * 100;
	for (t = 0; t < len; t++)
	{
		temp = (num1 / mypow(10, len - t - 1)) % 10;
		if (t == (len - 2))
		{
			LCD_ShowChar(x + (len - 2) * sizex, y, '.', fc, bc, sizey, 0);
			t++;
			len += 1;
		}
		LCD_ShowChar(x + t * sizex, y, temp + 48, fc, bc, sizey, 0);
	}
}

/******************************************************************************
      函数说明：显示图片
      入口数据：x,y起点坐标
                length 图片长度
                width  图片宽度
                pic[]  图片数组
      返回值：  无
******************************************************************************/
void LCD_ShowPicture(unsigned short int x, unsigned short int y, unsigned short int length, unsigned short int width, const unsigned char pic[])
{
	unsigned short int i, j;
	unsigned int k = 0;
	LCD_Address_Set(x, y, x + length - 1, y + width - 1);
	for (i = 0; i < length; i++)
	{
		for (j = 0; j < width; j++)
		{
			LCD_WR_DATA8(pic[k * 2]);
			LCD_WR_DATA8(pic[k * 2 + 1]);
			k++;
		}
	}
}

void LCD_GPIO_Init(void)
{
	// do not need in HAL lib
}

/******************************************************************************
      函数说明：LCD串行数据写入函数
      入口数据：dat  要写入的串行数据
      返回值：  无
******************************************************************************/
void LCD_Writ_Bus(unsigned char dat)
{
	unsigned char i;
	LCD_CS_Clr();
	for (i = 0; i < 8; i++)
	{
		LCD_SCLK_Clr();
		if (dat & 0x80)
		{
			LCD_MOSI_Set();
		}
		else
		{
			LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat <<= 1;
	}
	LCD_CS_Set();
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA8(unsigned char dat)
{
	LCD_Writ_Bus(dat);
}

/******************************************************************************
      函数说明：LCD写入数据
      入口数据：dat 写入的数据
      返回值：  无
******************************************************************************/
void LCD_WR_DATA(unsigned short int dat)
{
	LCD_Writ_Bus(dat >> 8);
	LCD_Writ_Bus(dat);
}

/******************************************************************************
      函数说明：LCD写入命令
      入口数据：dat 写入的命令
      返回值：  无
******************************************************************************/
void LCD_WR_REG(unsigned char dat)
{
	LCD_DC_Clr(); //写命令
	LCD_Writ_Bus(dat);
	LCD_DC_Set(); //写数据
}

/******************************************************************************
      函数说明：设置起始和结束地址
      入口数据：x1,x2 设置列的起始和结束地址
                y1,y2 设置行的起始和结束地址
      返回值：  无
******************************************************************************/
void LCD_Address_Set(unsigned short int x1, unsigned short int y1, unsigned short int x2, unsigned short int y2)
{
	if (USE_HORIZONTAL == 0)
	{
		LCD_WR_REG(0x2a); //列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); //行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); //储存器写
	}
	else if (USE_HORIZONTAL == 1)
	{
		LCD_WR_REG(0x2a); //列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); //行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); //储存器写
	}
	else if (USE_HORIZONTAL == 2)
	{
		LCD_WR_REG(0x2a); //列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); //行地址设置
		LCD_WR_DATA(y2);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); //储存器写
	}
	else
	{
		LCD_WR_REG(0x2a); //列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b); //行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c); //储存器写
	}
}

void LCD_Init(void)
{
	LCD_GPIO_Init(); //初始化GPIO

	//板子自带复位电路  无需手动复位
	//	LCD_RES_Clr();//复位
	//	delay_ms(100);
	//	LCD_RES_Set();
	//	delay_ms(100);

	LCD_BLK_Set(); //打开背光
	Pub_VM_Delay->delay(Pub_VM_Delay, 100);

	//************* Start Initial Sequence **********//
	LCD_WR_REG(0x11);						//Sleep out
	Pub_VM_Delay->delay(Pub_VM_Delay, 120); //Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	LCD_WR_REG(0xB1);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB2);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_REG(0xB3);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x05);
	LCD_WR_DATA8(0x3C);
	LCD_WR_DATA8(0x3C);
	//------------------------------------End ST7735S Frame Rate---------------------------------//
	LCD_WR_REG(0xB4); //Dot inversion
	LCD_WR_DATA8(0x03);
	//------------------------------------ST7735S Power Sequence---------------------------------//
	LCD_WR_REG(0xC0);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x08);
	LCD_WR_DATA8(0x04);
	LCD_WR_REG(0xC1);
	LCD_WR_DATA8(0XC0);
	LCD_WR_REG(0xC2);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x00);
	LCD_WR_REG(0xC3);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0x2A);
	LCD_WR_REG(0xC4);
	LCD_WR_DATA8(0x8D);
	LCD_WR_DATA8(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	LCD_WR_REG(0xC5); //VCOM
	LCD_WR_DATA8(0x1A);
	LCD_WR_REG(0x36); //MX, MY, RGB mode
	if (USE_HORIZONTAL == 0)
		LCD_WR_DATA8(0x00);
	else if (USE_HORIZONTAL == 1)
		LCD_WR_DATA8(0xC0);
	else if (USE_HORIZONTAL == 2)
		LCD_WR_DATA8(0x70);
	else
		LCD_WR_DATA8(0xA0);
	//------------------------------------ST7735S Gamma Sequence---------------------------------//
	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x22);
	LCD_WR_DATA8(0x07);
	LCD_WR_DATA8(0x0A);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x30);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2A);
	LCD_WR_DATA8(0x28);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x2E);
	LCD_WR_DATA8(0x3A);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x03);
	LCD_WR_DATA8(0x13);
	LCD_WR_REG(0xE1);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x16);
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x0D);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x26);
	LCD_WR_DATA8(0x23);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x27);
	LCD_WR_DATA8(0x25);
	LCD_WR_DATA8(0x2D);
	LCD_WR_DATA8(0x3B);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x04);
	LCD_WR_DATA8(0x13);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------//
	LCD_WR_REG(0x3A); //65k mode
	LCD_WR_DATA8(0x05);
	LCD_WR_REG(0x29); //Display on
}

unsigned char FontBuf[130]; //字库缓存
/******************************************************************************
      函数说明：向字库写入命令
      入口数据：dat  要写入的命令
      返回值：  无
******************************************************************************/
void ZK_command(unsigned char dat)
{
	unsigned char i;
	for (i = 0; i < 8; i++)
	{
		LCD_SCLK_Clr();
		if (dat & 0x80)
		{
			LCD_MOSI_Set();
		}
		else
		{
			LCD_MOSI_Clr();
		}
		LCD_SCLK_Set();
		dat <<= 1;
	}
}

/******************************************************************************
      函数说明：从字库读取数据
      入口数据：无
      返回值：  ret_data 读取的数据
******************************************************************************/
unsigned char get_data_from_ROM(void)
{
	unsigned char i;
	unsigned char ret_data = 0; //返回数据初始化
	for (i = 0; i < 8; i++)
	{
		LCD_SCLK_Clr(); //字库时钟拉低
		ret_data <<= 1;
		if (ZK_MISO)
		{
			ret_data++;
		}
		LCD_SCLK_Set(); //字库时钟拉高
	}
	return ret_data; //返回读出的一个字节
}

/******************************************************************************
      函数说明：读取N个数据
      入口数据：AddrHigh  写地址高字节
                AddrMid   写地址中字节
                AddrLow   写地址低字节
                *pBuff    读取的数据
                DataLen   读取数据的长度
      返回值：  无
******************************************************************************/
void get_n_bytes_data_from_ROM(unsigned char AddrHigh, unsigned char AddrMid, unsigned char AddrLow, unsigned char *pBuff, unsigned char DataLen)
{
	unsigned char i;
	ZK_CS_Clr();		  //字库片选
	ZK_command(0x03);	  //写指令
	ZK_command(AddrHigh); //写地址高字节
	ZK_command(AddrMid);  //写地址中字节
	ZK_command(AddrLow);  //写地址低字节
	for (i = 0; i < DataLen; i++)
	{
		*(pBuff + i) = get_data_from_ROM(); //读一个字节数据
	}

	ZK_CS_Set(); //取消字库片选
}

/******************************************************************************
      函数说明：显示ASCII码
      入口数据：x,y      写入的坐标
                zk_num   1:5*7   2:5*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_Asc(unsigned short int x, unsigned short int y, unsigned char zk_num, unsigned short int fc, unsigned short int bc)
{
	unsigned char i, k;
	switch (zk_num)
	{
	case 1:
	{
		LCD_Address_Set(x, y, x + 7, y + 7);
		for (i = 0; i < 7; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //5x7 ASCII

	case 2:
	{
		LCD_Address_Set(x, y, x + 7, y + 7);
		for (i = 0; i < 8; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //	  7x8 ASCII

	case 3:
	{
		LCD_Address_Set(x, y, x + 7, y + 11);
		for (i = 0; i < 12; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  6x12 ASCII

	case 4:
	{
		LCD_Address_Set(x, y, x + 7, y + 15);
		for (i = 0; i < 16; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  8x16 ASCII

	case 5:
	{
		LCD_Address_Set(x, y, x + 15, y + 24);
		for (i = 0; i < 48; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  12x24 ASCII

	case 6:
	{
		LCD_Address_Set(x, y, x + 15, y + 31);
		for (i = 0; i < 64; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  16x32 ASCII
	}
}

/******************************************************************************
      函数说明：显示ASCII码
      入口数据：x,y      写入的坐标
                zk_num   1:5*7   2:5*8   3:6*12,  4:8*16,  5:12*24,  6:16*32
                text[]   要显示的字符串
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_Asc_String(unsigned short int x, unsigned short int y, unsigned short int zk_num, unsigned char text[], unsigned short int fc, unsigned short int bc)
{
	unsigned char i = 0;
	unsigned char AddrHigh, AddrMid, AddrLow; //字高、中、低地址
	unsigned int FontAddr = 0;				  //字地址
	unsigned int BaseAdd = 0;				  //字库基地址
	unsigned char n, d;						  // 不同点阵字库的计算变量
	switch (zk_num)
	{
	//n个数，d:字间距
	case 1:
		BaseAdd = 0x1DDF80;
		n = 8;
		d = 6;
		break; //	  5x7 ASCII
	case 2:
		BaseAdd = 0x1DE280;
		n = 8;
		d = 8;
		break; //   7x8 ASCII
	case 3:
		BaseAdd = 0x1DBE00;
		n = 12;
		d = 6;
		break; //  6x12 ASCII
	case 4:
		BaseAdd = 0x1DD780;
		n = 16;
		d = 8;
		break; //  8x16 ASCII
	case 5:
		BaseAdd = 0x1DFF00;
		n = 48;
		d = 12;
		break; //  12x24 ASCII
	case 6:
		BaseAdd = 0x1E5A50;
		n = 64;
		d = 16;
		break; //  16x32 ASCII
	}
	while ((text[i] > 0x00))
	{
		if ((text[i] >= 0x20) && (text[i] <= 0x7E))
		{
			FontAddr = text[i] - 0x20;
			FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

			AddrHigh = (FontAddr & 0xff0000) >> 16;							   /*地址的高8位,共24位*/
			AddrMid = (FontAddr & 0xff00) >> 8;								   /*地址的中8位,共24位*/
			AddrLow = FontAddr & 0xff;										   /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n); /*取一个汉字的数据，存到"FontBuf[]"*/
			Display_Asc(x, y, zk_num, fc, bc);								   /*显示一个ascii到LCD上 */
		}
		i++;	//下个数据
		x += d; //下一个字坐标
	}
}

/******************************************************************************
      函数说明：显示ASCII码(Arial&Times New Roman)
      入口数据：x,y      写入的坐标
                zk_num   1:6*12,  2:8*16,  3:12*24,  4:16*32
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_Arial_TimesNewRoman(unsigned short int x, unsigned short int y, unsigned char zk_num, unsigned short int fc, unsigned short int bc)
{
	unsigned char i, k;
	switch (zk_num)
	{
	case 1:
	{
		LCD_Address_Set(x, y, x + 15, y + 12);
		for (i = 2; i < 26; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  6x12 ASCII

	case 2:
	{
		LCD_Address_Set(x, y, x + 15, y + 17);
		for (i = 2; i < 34; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  8x16 ASCII
	case 3:
	{
		LCD_Address_Set(x, y, x + 23, y + 23);
		for (i = 2; i < 74; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  12x24 ASCII

	case 4:
	{
		LCD_Address_Set(x, y, x + 31, y + 31);
		for (i = 2; i < 130; i++)
		{
			for (k = 0; k < 8; k++)
			{
				if ((FontBuf[i] & (0x80 >> k)) != 0)
				{
					LCD_WR_DATA(fc);
				}
				else
				{
					LCD_WR_DATA(bc);
				}
			}
		}
	}
	break; //  16x32 ASCII
	}
}

/******************************************************************************
      函数说明：显示ASCII(Arial类型)
      入口数据：x,y      写入的坐标
                zk_num   1:6*12,  2:8*16,  3:12*24,  4:16*32
                text[]   要显示的字符串
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_Arial_String(unsigned short int x, unsigned short int y, unsigned short int zk_num, unsigned char text[], unsigned short int fc, unsigned short int bc)
{
	unsigned char i = 0;
	unsigned char AddrHigh, AddrMid, AddrLow; //字高、中、低地址
	unsigned int FontAddr = 0;				  //字地址
	unsigned int BaseAdd = 0;				  //字库基地址
	unsigned char n, d;						  // 不同点阵字库的计算变量
	switch (zk_num)
	{
	//n:个数，d:字间距
	case 1:
		BaseAdd = 0x1DC400;
		n = 26;
		d = 8;
		break; //  8x12 ASCII(Arial类型)
	case 2:
		BaseAdd = 0x1DE580;
		n = 34;
		d = 12;
		break; //  12x16 ASCII(Arial类型)
	case 3:
		BaseAdd = 0x1E22D0;
		n = 74;
		d = 16;
		break; //  16x24 ASCII(Arial类型)
	case 4:
		BaseAdd = 0x1E99D0;
		n = 130;
		d = 24;
		break; //  24x32 ASCII(Arial类型)
	}
	while ((text[i] > 0x00))
	{
		if ((text[i] >= 0x20) && (text[i] <= 0x7E))
		{
			FontAddr = text[i] - 0x20;
			FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

			AddrHigh = (FontAddr & 0xff0000) >> 16;							   /*地址的高8位,共24位*/
			AddrMid = (FontAddr & 0xff00) >> 8;								   /*地址的中8位,共24位*/
			AddrLow = FontAddr & 0xff;										   /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n); /*取一个汉字的数据，存到"FontBuf[]"*/
			Display_Arial_TimesNewRoman(x, y, zk_num, fc, bc);				   /*显示一个ascii到LCD上 */
		}
		i++;	//下个数据
		x += d; //下一个字坐标
	}
}

/******************************************************************************
      函数说明：显示ASCII(Arial类型)
      入口数据：x,y      写入的坐标
                zk_num   1:6*12,  2:8*16,  3:12*24,  4:16*32
                text[]   要显示的字符串
                fc 字体颜色
                bc 背景颜色
      返回值：  无
******************************************************************************/
void Display_TimesNewRoman_String(unsigned short int x, unsigned short int y, unsigned short int zk_num, unsigned char text[], unsigned short int fc, unsigned short int bc)
{
	unsigned char i = 0;
	unsigned char AddrHigh, AddrMid, AddrLow; //字高、中、低地址
	unsigned int FontAddr = 0;				  //字地址
	unsigned int BaseAdd = 0;				  //字库基地址
	unsigned char n, d;						  // 不同点阵字库的计算变量
	switch (zk_num)
	{
	//n:个数，d:字间距
	case 1:
		BaseAdd = 0x1DCDC0;
		n = 26;
		d = 8;
		break; //  6x12 ASCII(TimesNewRoman类型)
	case 2:
		BaseAdd = 0x1DF240;
		n = 34;
		d = 12;
		break; //  12x16 ASCII	(TimesNewRoman类型)
	case 3:
		BaseAdd = 0x1E3E90;
		n = 74;
		d = 16;
		break; //  12x24 ASCII(TimesNewRoman类型)
	case 4:
		BaseAdd = 0x1ECA90;
		n = 130;
		d = 24;
		break; //  16x32 ASCII(TimesNewRoman类型)
	}
	while ((text[i] > 0x00))
	{
		if ((text[i] >= 0x20) && (text[i] <= 0x7E))
		{
			FontAddr = text[i] - 0x20;
			FontAddr = (unsigned long)((FontAddr * n) + BaseAdd);

			AddrHigh = (FontAddr & 0xff0000) >> 16;							   /*地址的高8位,共24位*/
			AddrMid = (FontAddr & 0xff00) >> 8;								   /*地址的中8位,共24位*/
			AddrLow = FontAddr & 0xff;										   /*地址的低8位,共24位*/
			get_n_bytes_data_from_ROM(AddrHigh, AddrMid, AddrLow, FontBuf, n); /*取一个汉字的数据，存到"FontBuf[]"*/
			Display_Arial_TimesNewRoman(x, y, zk_num, fc, bc);				   /*显示一个ascii到LCD上 */
		}
		i++;	//下个数据
		x += d; //下一个字坐标
	}
}
