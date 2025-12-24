#ifndef _UART_
#define _UART_

#include "STC8h.h"
#include "TYPE.h"
void Uart_Init();
void Uart_T8Bit(int8 dat);
void Uart_T16Bit(int16 dat);
void Uart_T32Bit(int32 dat);

int8 Uart_R8Bit();
int16 Uart_R16Bit();
int32 Uart_R32Bit();
void Uart_Test();
void Uart_SendString(uchar buffer[]);
void Uart_SendNum(ulong num,uchar lenth,uchar mode);

#endif