#include "_STC8X_.h"
#include "TYPE.h"
#include "ADC.h"
#include "Key.h"
#include "oled.h"
#include "Buzzer.h"
#define F0 44236800L
uchar tune, level, stop = 0;
uint music;
sbit key = P3 ^ 2;
// 1,1#,2,2#,3,4,4#,5,5#,6,6#,7;
void delayms(uint t)
{
    uint i;
    while (t--)
        for (i = 0; i < 600; i++)
            ;
}
code unsigned int Music_Table[4][15] = {

    {262, 277, 294, 311, 330, 349, 370, 392, 415, 440, 466, 494},
    {523, 554, 587, 622, 659, 698, 740, 784, 831, 880, 932, 988},
    {1046, 1109, 1175, 1245, 1318, 1397, 1480, 1568, 1661, 1760, 1865, 1976},
    {2093, 2218, 2349, 2489, 2637, 2794, 2960, 3136, 3323, 3520, 3729, 3951}
    /*
    {61719,61925,62134,62320,62505,62670,62833,62984,63126,63263,63390,63511},
    {63623,63697,63832,63928,64018,64103,64184,64260,64332,64399,64463,64523},
    {64579,64634,64684,64732,64777,64820,64860,64898,64933,64967,64999,65029},
    {65058,65085,65110,65134,65156,65171,65198,65217,65234,65251,65267,65282}
    */
};
unsigned int Music_Trans(int note, int up)
{
    if (up == 0)
    {
        switch (note)
        {
        case 1:
            return 0;
        case 2:
            return 2;
        case 3:
            return 4;
        case 4:
            return 5;
        case 5:
            return 7;
        case 6:
            return 9;
        case 7:
            return 11;
        }
    }
    else if (up == 1)
    {
        switch (note)
        {
        case 1:
            return 1;
        case 2:
            return 4;
        case 4:
            return 6;
        case 5:
            return 8;
        case 6:
            return 10;
        }
    }
}
void Key_Handler()
{
    uint keynum;
    uchar kleft = 0, kright = 0;
    keynum = Key_Scan();
    kright = keynum;
    keynum >>= 8;
    kleft = keynum;
    tune = 12;
    if (kright == 0x31)
        tune = 1;
    if (kright == 0x32)
        tune = 2;
    if (kright == 0x33)
        tune = 3;
    if (kright == 0x34)
        tune = 4;
    if (kright == 0x35)
        tune = 5;
    if (kright == 0x36)
        tune = 6;
    if (kright == 0x37)
        tune = 7;
    level = 1;
    if (kleft == 0x4c || kleft == 0x52 || kleft == 0x4f)
        level = 0;
    if (kleft == 0x4d || kleft == 0x53 || kleft == 0x54)
        level = 2;
    if (kleft == 0x4e || kleft == 0x54 || kleft == 0x51)
        level = 3;
}
sbit Wifi = P2 ^ 0;
sbit Audio = P2 ^ 1;
sbit laser = P1 ^ 3;
sbit light = P1 ^ 7;
void main()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M1 = 0xf7;
    P1M0 = 0x00; // ÉèÖÃÎª¸ß×è¿¹
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    light = 0;
    Wifi = 0;
    Audio = 1;
    laser = 0;
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear();
    Buzzer_Init();
    ADC_Init();
    EA = 1;
    TR1 = 0;
    while (1)
    {
        Key_Handler();
        OLED_Printf(0, 0, 16, "Level:%bd     ", level);
        OLED_Printf(0, 4, 16, "Frequence:%u  ", Music_Table[level][Music_Trans(tune, 0)]);
        if (tune == 12)
            OLED_Printf(0, 2, 16, "Note:Pause      ");
        else
            OLED_Printf(0, 2, 16, "Note:%bd        ", tune);
        if (tune != 12)
        {
            TR1 = 1;
            music = 0xffff - (F0 / Music_Table[level][Music_Trans(tune, 0)] / 12);
            TH1 = music / 256;
            TL1 = music % 256;
            delayms(20);
        }
        else
        {
            TR1 = 0;
            Buzzer_Stop();
        }
    }
    stop = 1;
    return;
}