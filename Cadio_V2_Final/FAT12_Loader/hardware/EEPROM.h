/*
 * @Author: 1234 && 5678
 * @Date: 2025-08-17 16:33:39
 * @LastEditors: 1234 && 5678
 * @LastEditTime: 2025-10-03 22:58:29
 * @FilePath: \FAT12_Loader\hardware\EEPROM.h
 * @Description: 
 * 
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved. 
 */
#ifndef _EEPROM_
#define _EEPROM_

#define FAIL 0x10;

#include "TYPE.h"
#include "_STC8X_.h"

void EEPROM_Init(uchar WT);
uchar EEPROM_ReadByte(ulong add);
void EEPROM_WriteByte(ulong add, uchar dat);
void EEPROM_SectorErase(ulong add);

#endif
