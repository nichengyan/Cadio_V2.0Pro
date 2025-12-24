#include "UART.h"

void Uart_Init(void)	//115200bps@40.000MHz
{
	SCON = 0x50;		//8位数据,可变波特率
	AUXR |= 0x40;		//定时器时钟1T模式
	AUXR &= 0xFE;		//串口1选择定时器1为波特率发生器
	TMOD &= 0x0F;		//设置定时器模式
	TL1 = 0xA9;			//设置定时初始值
	TH1 = 0xFF;			//设置定时初始值
	ET1 = 0;			//禁止定时器中断
	TR1 = 1;			//定时器1开始计时
}



void Uart_T8Bit(int8 dat)
{
	SBUF = dat;
	while (!TI)
		;
	TI = 0;
}
void Uart_T16Bit(int16 dat)
{
	int8 a, b;
	b = dat;
	dat >>= 8;
	a = dat;
	SBUF = a;
	while (!TI)
		;
	TI = 0;
	SBUF = b;
	while (!TI)
		;
	TI = 0;
}
void Uart_T32Bit(int32 dat)
{
	int8 a, b, c, d;
	d = dat;
	dat >>= 8;
	c = dat;
	dat >>= 8;
	b = dat;
	dat >>= 8;
	a = dat;
	SBUF = a;
	while (!TI)
		;
	TI = 0;
	SBUF = b;
	while (!TI)
		;
	TI = 0;
	SBUF = c;
	while (!TI)
		;
	TI = 0;
	SBUF = d;
	while (!TI)
		;
	TI = 0;
}

int8 Uart_R8Bit()
{
	while (!RI)
		;
	RI = 0;
	return SBUF;
}
int16 Uart_R16Bit()
{
	int16 dat;
	dat <<= 8;
	while (!RI)
		;
	RI = 0;
	dat |= SBUF;
	dat <<= 8;
	while (!RI)
		;
	RI = 0;
	dat |= SBUF;
	return dat;
}
int32 Uart_R32Bit()
{
	int32 dat = 0;
	dat <<= 8;
	while (!RI)
		;
	RI = 0;
	dat |= SBUF;
	dat <<= 8;
	while (!RI)
		;
	RI = 0;
	dat |= SBUF;
	dat <<= 8;
	while (!RI)
		;
	RI = 0;
	dat |= SBUF;
	dat <<= 8;
	while (!RI)
		;
	RI = 0;
	dat |= SBUF;
	return dat;
}
void Uart_Test()
{
	Uart_T8Bit(Uart_R8Bit());
	return;
}
void Uart_SendString(uchar buffer[])
{
	uchar i;
	for (i = 0; buffer[i] != '\0'; i++)
	{
		SBUF = buffer[i];
		while (!TI)
			;
		TI = 0;
	}
	return;
}
void Uart_SendNum(ulong num, uchar lenth, uchar mode)
{
	uchar i, flg = 0, tmp = 0, buffer[15];
	for (i = 0; i < lenth; i++)
	{
		buffer[i] = num % 10;
		num /= 10;
	}
	for (i = lenth; i > 0; i--)
	{
		tmp = buffer[i - 1];
		if (mode == 0)
		{
			Uart_T8Bit(tmp + 48);
		}
		else
		{
			if (tmp != 0 && flg == 0)
				flg = 1;
			if (flg == 0 && tmp == 0)
				continue;
			if (flg == 1)
				Uart_T8Bit(tmp + 48);
		}
	}
	return;
}
#if (Using_Printf == 1)
#include <stdarg.h>
#include <stdio.h>
void Uart_Printf(const char *str, ...)
{
	char buffer[64];
	va_list args;
	va_start(args, str);
	vsprintf(buffer, str, args);
	va_end(args);
	sprintf("%s\n", buffer);
	Uart_SendString(buffer);
}
#endif