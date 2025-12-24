#ifndef _D18B20_
#define _D18B20_
#include "TYPE.H"
#include "_STC8X_.h"
#include "intrins.h"
sbit DQ=P2^7;
#define usrNOP()    _nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_();_nop_()


void delay_us(uint8_t t);
uint8_t D18b20_Init();
uint8_t D18b20_ReadOneByte();
void D18b20_WriteOneByte(uint8_t dat);
int D18b20_GetTemperature();
#endif
 
