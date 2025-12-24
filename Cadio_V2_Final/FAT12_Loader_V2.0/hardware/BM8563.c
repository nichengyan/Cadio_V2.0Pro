#include "BM8563.h"
/*-----------------------IIC-----------------------*/
void _IIC_Delay()
{
    uint8_t i = 0;
    for (i = 0; i < 128; i++)
        ;
}
void IIC_Start()
{
    BM8563_SDA = 1;
    _IIC_Delay();
    BM8563_SCL = 1;
    _IIC_Delay();
    BM8563_SDA = 0;
    _IIC_Delay();
    BM8563_SCL = 0;
    _IIC_Delay();
    return;
}
void IIC_Stop()
{
    BM8563_SCL = 0;
    _IIC_Delay();
    BM8563_SCL = 1;
    _IIC_Delay();
    BM8563_SDA = 1;
    _IIC_Delay();
    return;
}
void IIC_Ack(uint8_t a)
{
    if (a == 0x00)
        BM8563_SDA = 0;
    else
        BM8563_SDA = 1;
    _IIC_Delay();
    BM8563_SCL = 1;
    _IIC_Delay();
    BM8563_SCL = 0;
    _IIC_Delay();
    _IIC_Delay();
}
void IIC_SendByte(uint8_t c)
{
    uchar i;
    for (i = 0; i < 8; i++)
    {
        if ((c << i) & 0x80)
            BM8563_SDA = 1;
        else
            BM8563_SDA = 0;
        _IIC_Delay();
        BM8563_SCL = 1;
        _IIC_Delay();
        BM8563_SCL = 0;
    }
    // WaitAck
    _IIC_Delay();
    BM8563_SDA = 1;
    _IIC_Delay();
    BM8563_SCL = 1;
    _IIC_Delay();
    BM8563_SCL = 0;
    _IIC_Delay();
    return;
}
uchar IIC_RcvByte(void)
{
    uchar i, rect;
    rect = 0;
    BM8563_SDA = 1;
    for (i = 0; i < 8; i++)
    {
        _IIC_Delay();
        BM8563_SCL = 0;
        _IIC_Delay();
        BM8563_SCL = 1;
        _IIC_Delay();
        rect = rect << 1;
        if (BM8563_SDA == 1)
            rect = rect + 1;
        _IIC_Delay();
    }
    BM8563_SCL = 0;
    _IIC_Delay();
    return rect;
}

/*----------------------------------------------*/

/*---------------------RW-----------------------*/
void BM8563_WriteByte(uint8_t addr, uint8_t dat)
{
    uchar i, res;
    IIC_Start();
    IIC_SendByte(0xa2);
    IIC_SendByte(addr);
    IIC_SendByte(dat);
    IIC_Stop();
    return;
}
uint8_t BM8563_ReadByte(uint8_t addr)
{
    uchar i, res;
    IIC_Start();
    IIC_SendByte(0xa2);
    IIC_SendByte(addr);
    IIC_Start();
    IIC_SendByte(0xa2 + 1);
    res = IIC_RcvByte();
    IIC_Ack(1);
    IIC_Stop(); // 除最后一个字节外，其他都要从 MASTER 发应答。
    return res;
}
/*----------------------------------------------------*/
void BM8563_Init()
{                                 // 默认初始化为 2006.11.22 00:00
    BM8563_WriteByte(0x00, 0x00); // ctrl_1
    BM8563_WriteByte(0x01, 0x00); // ctrl_2

    BM8563_WriteByte(0x02, 0x00); // sec
    BM8563_WriteByte(0x03, 0x00); // min
    BM8563_WriteByte(0x04, 0x00); // hour
    BM8563_WriteByte(0x05, 0x22); // day
    BM8563_WriteByte(0x06, 0x03); // week
    BM8563_WriteByte(0x07, 0x11); // month&&century
    BM8563_WriteByte(0x08, 0x06); // year
}
void BM8563_WriteTime(time *t)
{
    BM8563_WriteByte(0x00, 0x00);
    BM8563_WriteByte(0x01, 0x00);

    BM8563_WriteByte(0x02, ((t->sec / 10) << 4) | (t->sec % 10));
    BM8563_WriteByte(0x03, ((t->min / 10) << 4) | (t->min % 10));
    BM8563_WriteByte(0x04, ((t->hour / 10) << 4) | (t->hour % 10));
    BM8563_WriteByte(0x05, ((t->day / 10) << 4) | (t->day % 10));
    BM8563_WriteByte(0x06, t->weekday);
    BM8563_WriteByte(0x07, ((t->month / 10) << 4) | (t->month % 10));
    BM8563_WriteByte(0x08, t->year);
}
void BM8563_ReadTime(time *tt)
{
    tt->sec = BM8563_ReadByte(0x02);
    tt->sec = ((tt->sec >> 4) & 0x07) * 10 + (tt->sec & 0x0f) * 1;
    tt->min = BM8563_ReadByte(0x03);
    tt->min = ((tt->min >> 4) & 0x07) * 10 + (tt->min & 0x0f) * 1;
    tt->hour = BM8563_ReadByte(0x04);
    tt->mode = 24;
    tt->hour = ((tt->hour >> 4) & 0x0f) * 10 + (tt->hour & 0x0f) * 1;
    tt->day = BM8563_ReadByte(0x05);
    tt->day = ((tt->day >> 4) & 0x03) * 10 + (tt->day & 0x0f);
    tt->month = BM8563_ReadByte(0x07);
    tt->month = ((tt->month >> 4) & 0x01) * 10 + (tt->month & 0x0f);
    tt->weekday = BM8563_ReadByte(0x06);
    tt->weekday = tt->weekday & 0x07;
    tt->year = BM8563_ReadByte(0x08);
    tt->year = tt->year;
}
