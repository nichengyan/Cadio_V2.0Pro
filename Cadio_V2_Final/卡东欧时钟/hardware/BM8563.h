#ifndef __BM8563_H__

#define __BM8563_H__


#include "TYPE.H"
#include "_STC8X_.h"
#include "intrins.h"

sbit BM8563_SCL=P2^5;//SCL
sbit BM8563_SDA=P2^4;//SDA


typedef struct{
    uchar sec,min,hour,mode,ap;
    uchar day,month,weekday;
    uchar year;
}time;


void BM8563_Init();//默认初始化为 2006.11.22 00:00
void BM8563_WriteByte(uint8_t addr,uint8_t dat);
uint8_t BM8563_ReadByte(uint8_t addr);
void BM8563_WriteTime(time *t);
void BM8563_ReadTime(time *tt);
/*
typedef struct{
    uchar ctrl,hour_s,min_s,lenth;
}alarm;
alarm BM8563_ReadAlarm(uint8_t num);
void BM8563_WriteAlarm(alarm a,uint8_t id);
uint8_t BM8563_MatchAlarm(time tnow);    
*/