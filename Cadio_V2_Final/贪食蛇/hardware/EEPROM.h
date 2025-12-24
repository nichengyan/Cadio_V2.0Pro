#ifndef _EEPROM_
#define _EEPROM_

#define FAIL 0x10;

#include "TYPE.h"
#include "STC8H.h"



void EEPROM_Init(uchar WT);
uchar EEPROM_ReadByte(ulong add);
void EEPROM_WriteByte(ulong add,uchar dat);
void EEPROM_SectorErase(ulong add);

#endif
 
