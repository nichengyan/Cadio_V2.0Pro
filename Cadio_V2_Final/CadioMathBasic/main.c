#include "_STC8X_.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "oled.h"
#include "Expression.h"
#include "Gui.h"
#include <stdarg.h>
#include "Begin.h"
#include "cal.h"
#include "fun.h"

/*--------------------------------------------------------------------------
Cadio V2.0!!!
C51 Calculator Cadio-fx
Copyleft (c) 2024~2024 NCY
All rights reversed.

--------------------------------------------------------------------------*/

// #include "FAT12_2.h"
#define Fosc 40000000L // ������ʱ��
void delayms(uint t)
{
    uint16_t i;
    while (t--)
        for (i = 0; i < 600; i++)
            ;
}
uint16_t i;
uint8_t Res;
Page pMain, text;

uint8_t sela, selb;
sbit Wifi = P2 ^ 0;
sbit Audio = P2 ^ 1;
sbit laser = P1 ^ 3;
sbit light = P1 ^ 7;
sbit sd_cs = P1 ^ 6;

void main(void)
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M1 = 0x7f;
    P1M0 = 0x00; // ����Ϊ���迹
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    Wifi = 0;
    Audio = 0;
    laser = 0;
    light = 0;
    sd_cs = 1;
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear();
    Gui_Init();
    Gui_CreatePage(&pMain, 128, 64);
    Gui_ClearBufferBit(&pMain, 0);
    Begin_Demo(Set_SkipAnimation_Flg());
    while (1)
    {
        sela = Begin_Select_Menu(&pMain);
        if (sela == 1)
        {
            Set_Menu(&pMain);
        }
        if (sela == 2)
        {
            while (1)
            {
                Gui_ClearBufferBit(&pMain, 0);
                Gui_Flush(&pMain);
                selb = Fun_Select(&pMain);
                if (selb == 0)
                {
                    sela = 0;
                    break;
                }
                if (selb == 1)
                {
                    Fun_Unary(&pMain);
                }
                if (selb == 2)
                {
                    Fun_Implicit(&pMain);
                }
                if (selb == 3)
                {
                    Fun_Dynamic(&pMain);
                }
                if (selb == 4)
                {
                    Fun_ParaEqu(&pMain);
                }
            }
        }
        if (sela == 3)
        {
            Cal_Work(&pMain);
        }
    }
}
