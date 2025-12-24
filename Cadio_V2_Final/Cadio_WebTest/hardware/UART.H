#ifndef _UART_
#define _UART_

#include "_STC8X_.h"
#include "TYPE.h"
#define Using_Printf 1
void Uart_Init();
void Uart_T8Bit(uint8_t dat);
void Uart_T16Bit(uint16_t dat);
void Uart_T32Bit(uint32_t dat);

uint8_t Uart_R8Bit();
uint16_t Uart_R16Bit();
uint32_t Uart_R32Bit();
void Uart_Test();
void Uart_SendString(uchar buffer[]);
void Uart_SendNum(ulong num,uchar lenth,uchar mode);
#if (Using_Printf==1)
#include <stdarg.h>
void Uart_Printf(const char *str,...);
#endif
#endif