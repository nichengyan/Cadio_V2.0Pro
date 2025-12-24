#define MAIN_Fosc 40000000L // 定义主时钟
#include "TYPE.h"
#include "oled.h"
#include "Key.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Gui.h"
#include "UART.h"
#define pi 3.1415926
/*--------------------------------------------------------------------------
Cadio NetWork
Demo For Esp8266 Wifi Module
CopyRight (c) 2024~2024 NCY
All rights reserved.
--------------------------------------------------------------------------*/
uint8_t Logo[] = {
    /*--  调入了一幅图像：C:\Users\nist\Desktop\素材\CadioNetWork.bmp  --*/
    /*--  宽度x高度=96x64  --*/
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F,
    0x1F, 0x1F, 0x0F, 0x0F, 0x07, 0x07, 0x07, 0x03, 0x83, 0x83, 0xC1, 0x41, 0x01, 0x01, 0x01, 0x00,
    0x80, 0x80, 0xC0, 0x00, 0x00, 0x00, 0xC0, 0xC0, 0x80, 0x00, 0x01, 0x01, 0x01, 0x41, 0xC1, 0x83,
    0x83, 0x03, 0x07, 0x07, 0x07, 0x0F, 0x0F, 0x1F, 0x1F, 0x3F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x80, 0xC0, 0x80, 0x80,
    0x08, 0x18, 0x3C, 0x3E, 0x7E, 0x7F, 0x1F, 0x0F, 0x03, 0xC1, 0xE0, 0xF0, 0xFC, 0xFE, 0xFE, 0xFF,
    0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC, 0xF8, 0xE0, 0xC1,
    0x03, 0x07, 0x1F, 0x3F, 0x7E, 0x3C, 0x3C, 0x38, 0x70, 0xF0, 0xE0, 0xE0, 0xF0, 0xF1, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0xBF,
    0xDF, 0xDF, 0xDF, 0xDF, 0xDF, 0xDF, 0xDF, 0xDF, 0xBF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x7F, 0x0F, 0x03, 0x01, 0x00, 0x00, 0x80, 0xE0, 0xF8, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFE, 0x3E, 0x04, 0x00, 0x80, 0xF0, 0xF8, 0xF1, 0xF1, 0xF1, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3,
    0xE7, 0xE7, 0xC7, 0x00, 0x00, 0x00, 0xC7, 0xE7, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xE3, 0xF1, 0xF1,
    0xF1, 0xF8, 0xF0, 0x80, 0x00, 0x04, 0x1E, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFB,
    0xFD, 0xEE, 0xF6, 0x36, 0x36, 0xF6, 0xEE, 0xFD, 0xFB, 0xFF, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0x07, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7C, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x03, 0x00, 0x00, 0x38, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x00, 0x00, 0x00, 0x3F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F, 0x7F,
    0x7F, 0x7F, 0x7F, 0x7F, 0x7C, 0x78, 0x78, 0x7D, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x3F, 0x1F, 0xCF,
    0x4F, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0x4F, 0xCF, 0xCF, 0x4F, 0x4F, 0x4F, 0x4F, 0x4F, 0xCF,
    0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xE0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
    0xFE, 0xC0, 0x00, 0x00, 0x1E, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
    0xFE, 0xFE, 0xFE, 0x00, 0x00, 0x00, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x7F,
    0x5F, 0x6E, 0x75, 0x7B, 0x7F, 0x47, 0x79, 0x7A, 0x79, 0x47, 0x7F, 0x7F, 0x40, 0x7F, 0x7F, 0x7B,
    0x7B, 0x60, 0x7B, 0x7B, 0x5F, 0x5F, 0x5F, 0x5F, 0x7F, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFE, 0xF8, 0xC0, 0x80, 0x00, 0x00, 0x01, 0x07, 0x1F, 0x3F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0x7F, 0x3C, 0x20, 0x00, 0x01, 0x0F, 0x0F, 0x8F, 0x8F, 0x8F, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7,
    0xC7, 0xE7, 0xE3, 0x00, 0x00, 0x00, 0xE3, 0xE7, 0xE7, 0xC7, 0xC7, 0xC7, 0xC7, 0xC7, 0x8F, 0x8F,
    0x8F, 0x8F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFE,
    0xFE, 0xFE, 0xFE, 0x3E, 0x3E, 0x0E, 0x0E, 0x3E, 0x3E, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0x3E,
    0x3E, 0x3E, 0x3E, 0x3E, 0x3E, 0xFE, 0xFE, 0xFE, 0xFE, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFC, 0xF8, 0xF0, 0xE0, 0xC0, 0x80, 0x01, 0x01, 0x01, 0x00,
    0x18, 0x1C, 0x3C, 0x7E, 0x7E, 0xFC, 0xF8, 0xE0, 0xC0, 0x83, 0x07, 0x0F, 0x3F, 0x7F, 0x7F, 0xFF,
    0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F, 0x7F, 0x3F, 0x0F, 0x07, 0x83,
    0xC3, 0xE3, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF,
    0xFF, 0xFF, 0xFF, 0x9F, 0x9F, 0x7C, 0x7C, 0x9F, 0x9F, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
    0x7F, 0x5F, 0x5F, 0x7F, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFE, 0xFC,
    0xF8, 0xF8, 0xF0, 0xF0, 0xE0, 0xE0, 0xE0, 0xC1, 0xC1, 0xC1, 0x83, 0x82, 0x80, 0x80, 0x80, 0x80,
    0x01, 0x01, 0x03, 0x00, 0x00, 0x00, 0x03, 0x01, 0x01, 0x80, 0x80, 0x80, 0x80, 0x82, 0x83, 0xC1,
    0xC1, 0xC1, 0xE0, 0xE1, 0xE1, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xE0, 0xCF,
    0xCF, 0xCF, 0xCF, 0xC9, 0xC9, 0xCE, 0xCE, 0xC9, 0xC9, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCF, 0xCE,
    0xCE, 0xCA, 0xCA, 0xCE, 0xCE, 0xCF, 0xCF, 0xCF, 0xCF, 0xE0, 0xF0, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
};
uint8_t Command[][48] = {
    {"AT+RST\0"},
    {"AT+CIFSR\0"},
    {"AT+GMR\0"},
    {"AT+CWMODE=1\0"},
    {"AT+CWLAP\0"},
    {"AT+CWJAP=\"ssid\",\"pwd\"\0"},
    //{"AT+CWQA\0"},
    {"AT+UART=400000,8,1,0,0\0"},
    {"AT+CIPSTART=\"type\",\"remote ID\",remoteport\0"},
    {"AT+CIPCLOSE=0\0"},
    {"AT+CIPMUX=0\0"},
    {"AT+CIPSERVER=1\0"},
    {"AT+CIPMODE=1\0"},
    {"AT+CIPSEND\0"},
    {"+++\0"},
};
uint8_t CmdLen[]= {6, 8, 6, 11, 8, 20, 22, 39, 13, 11, 14, 12, 10, 3};
#define Char_Per_Line 20
#define MaxSize 1024 + 3
#define Width 128
#define Height 64

uint8_t Rx_Buffer[MaxSize];
uint8_t Show_Buffer[MaxSize];
int Rx_Ptr = 0;
Str_Ptr = 0;
// Rx_Ptr用于指向 Rx_Buffer(循环fifo)的更新位置
// Str_Ptr用于指向Show_Buffer中屏幕显示的首个字符

// 设置选项,第一个反色其实没P用
uint8_t Setting_ColorReverse = 0;   // 反色显示
uint8_t Setting_AutoRefresh = 1;    // 自动滚屏
uint8_t Setting_UseLargeBuffer = 0; // 使用大缓冲(1024字节)

// 杂项全局变量
int Buffer_Size = 512;   // 缓冲区大小，默认512,Setting_UseLargeBuffer=1时自动开到1024
int i, j, k;             // 计数器
uint8_t Refresh_Flg = 0; // 更新标志，当接受到字符后此标志自动置1
uint8_t keynum;          // 键码
uint8_t shiftflg;        // 全局shift标志

sbit Wifi = P2 ^ 0;
sbit Audio = P2 ^ 1;
sbit laser = P1 ^ 3;
sbit light = P1 ^ 7;
Page pMain; // 主页面

void Pause();                                              // 防止误按
void delayms(uint16_t t);                                  // 毫秒级延迟函数
void Refresh();                                            // 刷新屏幕内容(本质是刷新屏幕显示在Show_Buffer里的显示起始点)
uint8_t Key();                                             // 按键读取函数，读取左右键盘的键码并仲裁
void Uart_Handler();                                       // 串口接收函数
void Uart_Assistant_Rx(Page *pa, uint8_t *str, int start); // 串口助手接收页面，str是待显示的字符串，start是显示起始位置
// 显示机制：只显示start后面的字符，遇到\n或满一行就换行，遇到str末尾(Buffer_Size)或显示满一屏就退出
void Uart_Assistant_Tx(Page *pa);    // 串口助手发送页面,集成简易Ascii码输入模块
void Uart_Protect(uint8_t *Command); // 波特率保护，禁止执行危险的指令 AT+UART
void Settings(Page *pa);             // 设置页面

void Cal_MoveR(uint8_t *str, uint8_t len, uint8_t m, uint8_t n)
{ // 把字符串第m及其之后(m>=0)的元素往右边挪n位（n>=0）
    char i;
    if (m + n >= len)
        return;
    for (i = len - 1; i >= m; i--)
    {
        if (i >= m + n)
            str[i] = str[i - n];
        else
            str[i] = 0;
    }
}
//========================================================================
// 函数: Cal_MoveL(uint8_t *str,uint8_t len,uint8_t m,uint8_t n)
// 描述: //把字符串第m及其之后(m>=0)的元素往左边边挪n位（n>=0）
// 参数: Str:目标字符串  len:长度   m:第几个（从0数起）     n:挪几位（从0数起）
// 返回: 无
// 版本: V2.0, 2024-8-2
//========================================================================
void Cal_MoveL(uint8_t *str, uint8_t len, uint8_t m, uint8_t n)
{ // 把字符串第m及其之后(m>=0)的元素往左挪n位（n>=0）
    char i;
    if (m + n >= len)
        return;
    for (i = m; i <= len; i++)
    {
        if (i <= len - n)
            str[i] = str[i + n];
        else
            str[i] = 0;
    }
}

void main(void)
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M0 = 0x08; // 设置为高阻抗
    P1M1 = 0x03;
    P2M0 = 0x01;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    light = 0;
    Audio = 0;
    laser = 0;
    Wifi = 0;
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear();
    Gui_Init();
    Gui_CreatePage(&pMain, 128, 64);
    Gui_ClearBufferBit(&pMain, 0);
    Gui_Flush(&pMain);
    Gui_DrawBmp(&pMain, 16, 0, 96, 64, Logo);
    Gui_Flush(&pMain);
    delayms(5000);
    P_SW1 = (P_SW1 & ~0xc0) | 0x40; // UART1/USART1: RxD(P3.6), RxD(P3
    Uart_Init();
    EA = 1;
    ES = 1;
    delayms(100);

    for (i = 0; i < Buffer_Size; i++)
        Rx_Buffer[i] = ' ';
    Wifi = 1;
    while (1)
    {
        keynum = Key();
        if (keynum == 0x49)
        {
            shiftflg = 0;
            if ((Str_Ptr - (Char_Per_Line - 1)) < Buffer_Size)
                Str_Ptr += (Char_Per_Line - 1);
        }
        if (keynum == 0x44)
        {
            shiftflg = 0;
            if (Str_Ptr >= (Char_Per_Line - 1))
                Str_Ptr -= (Char_Per_Line - 1);
        }
        if (keynum == 0x41)
            shiftflg = 1;
        if (shiftflg && keynum >= 0x30 && keynum <= 0x39)
            Uart_Assistant_Tx(&pMain);
        else if (shiftflg && keynum == 0x4b)
            Settings(&pMain);
        else
            Uart_Assistant_Rx(&pMain, Show_Buffer, Str_Ptr);
        if (Setting_UseLargeBuffer)
            Buffer_Size = 1024;
        else
            Buffer_Size = 512;
        Gui_Flush(&pMain);
        Gui_ClearBufferBit(&pMain, 0);
    }
}

void Pause()
{
    while (Key_Scan() != 0xffff)
        ;
}
void delayms(uint16_t t)
{
    uint16_t i;
    while (t--)
        for (i = 0; i < 600; i++)
            ;
}
void Refresh()
{
    uint8_t LineCnt = 0, CharCnt = 0, Res_Cnt = 0;
    while (LineCnt < 6)
    {
        if (Show_Buffer[Buffer_Size - Res_Cnt - 1] == '\n' || CharCnt % Char_Per_Line == 0)
        {
            LineCnt++;
            CharCnt = 0;
        }
        else
        {
            CharCnt++;
        }
        Res_Cnt++;
    }
    Str_Ptr = Buffer_Size - Res_Cnt;
}
uint8_t Key()
{
    uint8_t kl, kr, keynum;
    kl = Key_Scan() >> 8;
    kr = Key_Scan();
    if (kl != 0xff)
        keynum = kl;
    else if (kr != 0xff)
        keynum = kr;
    else
        keynum = 0xff;
    return keynum;
}
void Uart_Handler() interrupt 4
{
    if (RI)
    {
        RI = 0;
        Rx_Buffer[Rx_Ptr] = SBUF;
        Rx_Ptr = (Rx_Ptr + 1) % Buffer_Size;
        Refresh_Flg = 1;
    }
}
void Uart_Assistant_Rx(Page *pa, uint8_t *str, int start)
{
    int posx, posy, ptr, i, Show_Ptr = 0;
    uint8_t nowc;
    posx = 0;
    posy = 0;
    ptr = start;
    for (i = 0; i < Buffer_Size; i++)
        Show_Buffer[i] = ' ';
    for (i = Rx_Ptr; i < Buffer_Size; i++)
    {
        Show_Buffer[Show_Ptr] = Rx_Buffer[i];
        Show_Ptr++;
    }
    for (i = 0; i < Rx_Ptr; i++)
    {
        Show_Buffer[Show_Ptr] = Rx_Buffer[i];
        Show_Ptr++;
    }
    if (Refresh_Flg && Setting_AutoRefresh)
    {
        Refresh_Flg = 0;
        Refresh();
    }
    while (1)
    {
        nowc = str[ptr];
        if (posx + 1 > (Char_Per_Line - 1))
        {
            posy += 1;
            posx = 0;
        }
        if (posy > 8 || ptr > Buffer_Size)
            break;
        if (nowc == 0x0d || nowc == 0x0a)
        {
            posx = 0;
            posy += 1;
        }
        else
        {
            Gui_ShowChar(pa, 3 + posx * 6, 3 + posy * 8, nowc, 1, 0);
            posx += 1;
        }
        ptr++;
    }
    Gui_DrawRectangleWired(pa, 0, 0, Width - 6 - 2, Height - 1, 1);
    Gui_DrawRoundRectangleWired(pa, Width - 6 - 1, 0, 6, Height - 1, 3, 1);
    Gui_DrawRoundRectangleFilled(pa, Width - 6 - 1, start * (1.0 / Buffer_Size) * (Height - 8) - (Height * 7 * (Char_Per_Line - 1) / Buffer_Size) + 4, 6, 4 + (Height * 7 * (Char_Per_Line - 1) / Buffer_Size), 3, 1);
}

void Uart_Assistant_Tx(Page *pa)
{

    int i, posx, posy, selectx, selecty;
    int Lenth, Ptr, curx, cury;
    uint8_t keynum, shiftflg = 0, hypflg = 0, curflg = 1;
    uint8_t TBuffer[64], nowc;
    uint8_t curcnt = 0;
    uint8_t page = 0, option = 0, id_now = 0;
    int nowy = 0, desy = 0;
    float dy = 0;
    int nowx = 0, desx = 0;
    float dx = 0;
    selectx = 0;
    selecty = 0;
    Ptr = 0;
    Lenth = 0;
    for (i = 0; i < 64; i++)
        TBuffer[i] = 0;
    while (1)
    {
        keynum = Key();
        Gui_ClearBufferBit(pa, 0);
        Gui_DrawRectangleWired(pa, 0, 0, 127, 63, 1);
        Gui_DrawLine(pa, 0, 36, 127, 36, 1);
        posx = 0;
        posy = 0;
        if (keynum == 0x4e)
        {
            shiftflg = 0;
            if (Ptr < Lenth)
            {
                Ptr += 1;
            }
            else
            {
                Ptr = 0;
            }
        }
        if (keynum == 0x4c)
        {
            shiftflg = 0;
            if (Ptr > 0)
            {
                Ptr -= 1;
            }
            else
            {
                Ptr = Lenth;
            }
        }
        if (keynum == 0x49)
        {
            shiftflg = 0;
            if (selecty < 3)
            {
                selecty += 1;
            }
            else
            {
                selecty = 0;
            }
        }
        if (keynum == 0x44)
        {
            shiftflg = 0;
            if (selecty > 0)
            {
                selecty -= 1;
            }
            else
            {
                selecty = 3;
            }
        }
        if (keynum == 0x4a)
        {
            shiftflg = 0;
            if (selectx < (Char_Per_Line - 1))
            {
                selectx += 1;
            }
            else
            {
                selectx = 0;
            }
        }
        if (keynum == 0x48)
        {
            shiftflg = 0;
            if (selectx > 0)
            {
                selectx -= 1;
            }
            else
            {
                selectx = (Char_Per_Line - 1);
            }
        }
        if (selectx >= 5 && selecty == 3)
        {
            shiftflg = 0;
            selectx = 4;
            shiftflg = 0;
        }
        if (keynum == 0x41)
            shiftflg = !shiftflg;
        if (keynum == 0x51)
            hypflg = !hypflg;
        if (keynum == 0x3d)
        {
            nowc = ' ' + selecty * Char_Per_Line + selectx;
            if (shiftflg && nowc >= 'A' && nowc <= 'Z')
                nowc += 32;
            if (Ptr < 3 * Char_Per_Line)
            {
                Lenth++;
                Cal_MoveR(TBuffer, Lenth, Ptr, 1);
                TBuffer[Ptr] = nowc;
                Ptr++;
            }
        }
        if (keynum == 0x55)
        {
            if (Ptr > 0 && Lenth > 0)
            {
                Cal_MoveL(TBuffer, Lenth, Ptr - 1, 1);
                Lenth--;
                Ptr--;
            }
        }
        if (keynum == 0x56)
        {
            for (i = 0; i < 64; i++)
                TBuffer[i] = ' ';
            Ptr = 0;
            Lenth = 0;
        }
        if (Lenth >= 3 * Char_Per_Line)
            Lenth = 3 * Char_Per_Line - 1;
        if (keynum == 0x45)
        {
            //Uart_Protect(TBuffer);
            for (i = 0; i < Ptr; i++)
                Uart_T8Bit(TBuffer[i]);
            if (shiftflg == 0)
                Uart_T8Bit(0x0d);
            Uart_T8Bit(0x0a);
            return;
        }
        if (keynum == 0x4b && shiftflg)
        {
            for (i = 0; i < 64; i++)
                TBuffer[i] = ' ';
            Gui_ClearBufferBit(pa, 0);
            Lenth = 0;
            Ptr = 0;
            while (1)
            {
                keynum = Key_Scan() & 0x00ff;
                if (keynum == 0x44 && option > 0)
                    option--;
                if (keynum == 0x49 && option < 13)
                    option++;
                if (keynum == 0x3d)
                {
                    Lenth += CmdLen[option];
                    Cal_MoveR(TBuffer, Lenth, Ptr, 1);
                    for (i = 0; i < CmdLen[option]; i++)
                    {
                        TBuffer[Ptr] = Command[option][i];
                        Ptr++;
                    }
                    break;
                }
                page = option / 7;
                desy = 8 * (option % 7);
                desx = (CmdLen[option] * 6 + 2 < 112 ? CmdLen[option] * 6 + 2 : 120);
                for (i = 0; i < 7; i++)
                {
                    id_now = page * 7 + i + 1;
                    Gui_Printf_OneLine(pa, 0, i * 8, 1, 0, "%bd:%s", id_now, Command[id_now - 1]);
                }
                Gui_DrawRectangleWired(pa, 12 + (option / 10) * 6, nowy, nowx, 9, 1);
                nowy += dy;
                dy = (desy - nowy) * 0.5;
                nowx += dx;
                dx = (desx - nowx) * 0.5;
                Gui_Flush(pa);
                Gui_ClearBufferBit(pa, 0);
            }
            Pause();
            shiftflg = 0;
        }
        if (shiftflg)
            Gui_Printf(pa, 48, 27, 1, 0, "shift");
        else if (shiftflg)
            Gui_Printf(pa, 48, 27, 1, 0, "     ");
        for (i = 0; i < 65; i++)
        {
            if (posx == selectx && posy == selecty)
                Gui_ShowChar(pa, 3 + posx * 6, 3 + posy * 8, i + ' ', 0, 0);
            else
                Gui_ShowChar(pa, 3 + posx * 6, 3 + posy * 8, i + ' ', 1, 0);
            posx += 1;
            if (posx > (Char_Per_Line - 1))
            {
                posx = 0;
                posy += 1;
            }
        }
        for (i = 0; i < Lenth && i < 3 * (Char_Per_Line - 1); i++)
        {
            Gui_ShowChar(pa, 3 + 6 * (i % (Char_Per_Line - 1)), 37 + 8 * (i / (Char_Per_Line - 1)), TBuffer[i], 1, 0);
        }
        curx = Ptr % (Char_Per_Line - 1);
        cury = Ptr / Char_Per_Line;
        if (curflg)
            Gui_DrawLine(pa, curx * 6 + 3, 37 + cury * 8, curx * 6 + 3, 37 + cury * 8 + 8, 1);
        else
            Gui_DrawLine(pa, curx * 6 + 3, 37 + cury * 8, curx * 6 + 3, 37 + cury * 8 + 8, 0);
        if (curcnt > 3)
        {
            curcnt = 0;
            curflg = !curflg;
        }
        curcnt++;
        Gui_Flush(pa);
    }
}
void Uart_Protect(uint8_t *Command)
{
    
    uint8_t Cmd[] = "AT+UART=";
    uint8_t i, j, Match = 1;
    return ;
    for (i = 0; i < 64; i++)
    {
        if (Match == 0)
            break;
        if (Command[i] == 'A')
        {
            for (j = 0; j < 8; j++)
            {
                if (Command[i + j] != Cmd[j])
                {
                    Match = 0;
                    break;
                }
            }
            break;
        }
        else if (Command[i] != ' ')
            Match = 0;
    }
    if (Match == 1)
    {
        for (i = 0; i < 64; i++)
            Command[i] = ' ';
        OLED_Clear();
        OLED_ShowString(0, 0, "Dangerous Cmd", 16);
        OLED_ShowString(0, 2, "Baud Rate Could", 16);
        OLED_ShowString(0, 4, "Not Be Changed", 16);
        delayms(5000);
    }
    return;
}
void Settings(Page *pa)
{
    int posx, posy, desx, desy;
    float vx, vy;
    uint8_t option = 1, keynum;
    uint8_t setting_table[4];
    uint8_t lenth_table[4] = {0, 13, 11, 14};
    desx = lenth_table[1] * 6 + 2;
    posx = desx;
    desy = option * 16;
    posy = desy;
    setting_table[1] = Setting_ColorReverse;
    setting_table[2] = Setting_AutoRefresh;
    setting_table[3] = Setting_UseLargeBuffer;
    while (1)
    {
        Gui_ShowString(pa, 0, 0, "Setting:", 1, 1);
        keynum = Key_Scan() & 0x00ff;
        if (keynum == 0x3d)
            break;
        if (keynum == 0x44 && option > 1)
            option--;
        if (keynum == 0x49 && option < 3)
            option++;
        if (keynum == 0x48 || keynum == 0x4a)
            setting_table[option] = !setting_table[option];
        if (setting_table[1])
        {
            Gui_ShowString(pa, 0, 16, "ColorReverse T", 1, 1);
            Setting_ColorReverse = 1;
            OLED_ColorTurn(1);
        }
        else
        {
            Gui_ShowString(pa, 0, 16, "ColorReverse F", 1, 1);
            Setting_ColorReverse = 0;
            OLED_ColorTurn(0);
        }
        if (setting_table[2])
        {
            Gui_ShowString(pa, 0, 32, "AutoRefresh T", 1, 1);
            Setting_AutoRefresh = 1;
        }
        else
        {
            Gui_ShowString(pa, 0, 32, "AutoRefresh F", 1, 1);
            Setting_AutoRefresh = 0;
        }
        if (setting_table[3])
        {
            Gui_ShowString(pa, 0, 48, "UseLargeBuffer T", 1, 1);
            Setting_UseLargeBuffer = 1;
        }
        else
        {
            Gui_ShowString(pa, 0, 48, "UseLargeBuffer F", 1, 1);
            Setting_UseLargeBuffer = 0;
        }
        desy = option * 16;
        desx = lenth_table[option] * 8 + 2;
        vy = (desy - posy) * 0.7;
        vx = (desx - posx) * 0.7;
        posy += vy;
        posx += vx;
        Gui_DrawRectangleWired(pa, 0, posy, posx, 16, 1);
        Gui_Flush(pa);
        Gui_ClearBufferBit(pa, 0);
    }
    while (Key_Scan() != 0xffff);
}