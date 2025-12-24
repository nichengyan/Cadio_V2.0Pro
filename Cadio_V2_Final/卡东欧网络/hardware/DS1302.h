#ifndef _DS1302_H

#define _DS1302_H

#define uchar unsigned char 
#define uint unsigned int

#include "TYPE.H"
#include "_STC8H_.h"
#include "intrins.h"

sbit clk=P1^7;
sbit dat=P1^3;
sbit ce=P5^4;
typedef struct{
    uchar sec,min,hour,mode,ap;
    uchar day,month,weekday;
    uchar year;
}time;
typedef struct{
    uchar ctrl,hour_s,min_s,lenth;
}alarm;

void DS1302_Init();
void DS1302_WriteByte(uchar value,uchar addr);
uchar DS1302_ReadByte(uchar addr);
void DS1302_WriteTime(time t);
time DS1302_ReadTime();
alarm DS1302_ReadAlarm(uchar num);
void DS1302_WriteAlarm(alarm a,uchar id);
uchar DS1302_MatchAlarm(time tnow);    


#endif

/*更新日志*/

//2023/8/19 修复12小时制bug
//2023/8/20 增添闹钟功能
//2024/2/12 加入中断保护
