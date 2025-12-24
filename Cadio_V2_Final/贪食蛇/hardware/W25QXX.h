#ifndef __W25QXX_H__

#define __W25QXX_H__

#include <intrins.h>
#include "TYPE.h"
#include "_STC8H_.h"



#define SoftWare 0
#define HardWare 1
#define QuadIO 2

#define SPIMODE HardWare
//普通软件SPI
#if (SPIMODE==SoftWare)
sbit CS=P3^5;
sbit SO=P3^3;
sbit SI=P3^4;
sbit CLK=P3^2;
sbit WP=P3^7;
sbit HOLD=P3^6;
uchar W25QXX_RWByte(uchar dat);
 
uchar W25QXX_ReadRegisters(uchar id);
void W25QXX_WriteRegisters(uchar r1,uchar r2);
void W25QXX_WriteEnable(uchar flg);
uchar W25QXX_TestBusy();
uchar W25QXX_GetCapacity();

void W25QXX_EraseChip();
uchar W25QXX_ReadByte(ulong add);
void W25QXX_WriteByte(ulong add,uchar dat);   //每写完一次写保护会自动开启！！！！！！
void W25QXX_ReadPage(ulong add,uchar dat[]);   //256Byte
void W25QXX_WritePage(ulong add,uchar dat[]);  //256Byte
void W25QXX_EraseSector(ulong add);
#endif
#if (SPIMODE==HardWare)

sbit CS=P3^5;
sbit SO=P3^3;
sbit SI=P3^4;
sbit CLK=P3^2;
sbit WP=P3^7;
sbit HOLD=P3^6;
#define SPISPEEDLL 0x10
#define SPISPEEDLH 0x01
#define SPISPEEDHL 0x00
#define SPISPEEDHH 0x11
#define SPIF    0x80        //SPI传输完成标志。写入1清0。
#define WCOL    0x40        //SPI写冲突标志。写入1清0。
void HardWareSPI_Init(uchar spd);

uchar W25QXX_RWByte(uchar dat);
 
uchar W25QXX_ReadRegisters(uchar id);
void W25QXX_WriteRegisters(uchar r1,uchar r2);
void W25QXX_WriteEnable(uchar flg);
uchar W25QXX_TestBusy();
uchar W25QXX_GetCapacity();

void W25QXX_EraseChip();
uchar W25QXX_ReadByte(ulong add);
void W25QXX_WriteByte(ulong add,uchar dat);   //每写完一次写保护会自动开启！！！！！！
void W25QXX_ReadPage(ulong add,uchar dat[]);   //256Byte
void W25QXX_WritePage(ulong add,uchar dat[]);  //256Byte
void W25QXX_EraseSector(ulong add);
#endif
#if (SPIMODE==QuadIO)
uchar W25QXX_RWByte(uchar dat);
 
uchar W25QXX_ReadRegisters(uchar id);
void W25QXX_WriteRegisters(uchar r1,uchar r2);
void W25QXX_WriteEnable(uchar flg);
uchar W25QXX_TestBusy();
uchar W25QXX_GetCapacity();

void W25QXX_EraseChip();
uchar W25QXX_ReadByte(ulong add);
void W25QXX_WriteByte(ulong add,uchar dat);   //每写完一次写保护会自动开启！！！！！！
void W25QXX_ReadPage(ulong add,uchar dat[]);   //256Byte
void W25QXX_WritePage(ulong add,uchar dat[]);  //256Byte
void W25QXX_EraseSector(ulong add);
#endif
#endif