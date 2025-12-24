#ifndef _D18B20_
#define _D18B20_
#include "TYPE.H"
#include "_STC8H_.h"
#include "intrins.h"
sbit DQ=P4^5;
#define MCU_TYPE 1 //1T:1 12T:12


void DelayX(uint t);
byte D18b20_Init();
byte D18b20_ReadOneByte();
void D18b20_WriteOneByte(byte dat);
int D18b20_GetTemperature();

#endif
 
