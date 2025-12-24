/*************  功能说明    **************

本程序基于STC8H8K64U为主控芯片的卡东欧V1.0进行编写测试.

基于FAT12的BootLoader

功能：
     从挂载有FAT12的FLASH中加载.HEX和.BIN
     引导APP跳转启动，并提供正确的中断向量

下载时, 设置IRC工作频率为 40MHz.


******************************************/
#include "_STC8X_.h"
#include "TYPE.h"
#include "oled.h"
#include "W25QXX.h"
#include "ADC.h"
#include "FAT12.h"
#include "EEPROM.h"
#include <stdio.h>
#define APP_FLag 0xffff       // APP标识扇区地址（最后一扇区，用于记录APP是否正确写入，以及写入的是那个APP）
#define APP_Entry 0x3000      // APP入口，跳转到这就能运行UserAPP了
#define BootLoaderSize 0x3000 // BootLoader区大小
#define CodeSize 0xCE00       // 65536-12288-512 //剩给APP的最大空间

//               单片机EEPROM空间分配：
//      0x0000h~0x2fff:BootLoader（不要擦写）
//      0x3000h~0xfdff:UserAPP（可供安全的擦写）
//      0xfe00h~0xffff:APPFlg 标志位保留区（不要擦写）

// 要用到的函数
uint8_t Key_Scan();
void delayms(uint16_t t);
uint32_t Get_int32(uchar a, uchar b, uchar c, uchar d);
uint8_t Get_int8(uchar c, uchar d);
void OLED_ShowStr(uchar x, uchar y, uchar str[], uchar num);

FAT12_File f;
uchar CodeBuffer[512];
uchar APP_ID, APP_NUM;
uchar keynum = 0;
uint32_t i, j;
ulong Code_ptr = 0;
ulong ByteCnt = 0, Jindu = 0;
void (*App_Jmp)(void);
ulong tt0 = 100, Press = 0;
uchar HexBuffer[64]; // 解码单元
typedef struct
{
    uchar Hex_Start[1];
    uchar HexLenth[2];
    uchar HexAddr[4];
    uchar HexAttributes[2];
    uchar HexDat[55];
} Hex_Segment;
Hex_Segment *hs;
uint32_t NxtSeg = 0, AddrTmp = 0;
uint8_t LenthTmp = 0, DataTmp = 0;
uint32_t HexOffset = 0;
sbit Wifi = P2 ^ 0;
sbit Audio = P2 ^ 1;
sbit laser = P1 ^ 3;
sbit light = P1 ^ 7;
void main()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M1 = 0xf7;
    P1M0 = 0x00; // 设置为高阻抗
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    Audio = 0;
    Wifi = 0;
    laser = 0;
    light = 0;
    EEPROM_Init(40);
    HardWareSPI_Init(SPISPEEDLL);
    ADC_Init();
    OLED_Init();
    OLED_Display_On();
    OLED_Clear();
    // 判断长按
    while (tt0--)
    {
        if (Key_Scan() != 0xff)
            Press++;
        delayms(10);
    }
    // 如果开机发现有按键事件，就进入APPSELECT
    if (Press > 50 || EEPROM_ReadByte(APP_FLag) == 0xff)
    {
        FAT12_Init();
        APP_NUM = FAT12_SearchForApp(1, &f);
        APP_ID = 1;
        OLED_ShowString(0, 0, "Select APP:", 16);
        // 显示APP名称
        while (1)
        {
            if (APP_ID > APP_NUM)
                APP_ID = 1;
            if (APP_ID < 1)
                APP_ID = APP_NUM;
            FAT12_SearchForApp(APP_ID, &f);
            if (f.File_Id != 0xffff)
            {
                OLED_ShowNum(0, 2, APP_ID, 2, 16);
                OLED_ShowString(16, 2, " ", 16);
                OLED_ShowStr(24, 2, f.File_Name, 12);
            }
            keynum = Key_Scan();
            // 上下键用于翻查
            if (keynum == 2)
                APP_ID++;
            if (keynum == 1)
                APP_ID--;
            if (APP_ID > APP_NUM)
                APP_ID = 1;
            if (APP_ID < 1)
                APP_ID = APP_NUM;
            // 如果按下"="，确认选择
            if (keynum == 3)
            {
                delayms(10);
                while (Key_Scan() == 3)
                    ;
                // 文件体积过大则提示出错，否则进入加载模块
                if (f.File_Size - BootLoaderSize > CodeSize && f.File_Name[9] == 'B')
                {
                    OLED_ShowString(0, 0, "App Too Big!", 16);
                    OLED_ShowNum(0, 2, f.File_Size - BootLoaderSize, 6, 16);
                    OLED_ShowString(48, 2, " Bytes", 16);
                    delayms(10000);
                    OLED_Clear();
                }
                else
                {
                    delayms(1000);
                    break;
                }
            }
            delayms(500);
        }
        // 加载模块：BIN文件直接烧录
        if (f.File_Name[9] == 'B')
        { // 无需解码的.BIN文件
            OLED_Clear();
            FAT12_Fopen(&f, f.File_Name);
            OLED_ShowStr(0, 0, f.File_Name, 12);
            OLED_ShowNum(0, 2, f.File_Size - BootLoaderSize, 6, 16);
            OLED_ShowString(48, 2, " Bytes", 16);
            EEPROM_SectorErase(APP_FLag);
            for (i = APP_Entry; i < 0xffff - 512; i += 512)
                EEPROM_SectorErase(i);
            OLED_ShowString(0, 4, "Load App...", 16);
            FAT12_Fseek(&f, BootLoaderSize, 0);
            Code_ptr = APP_Entry;
            Jindu = 0;
            while (!FAT12_IsEOF(&f) && Code_ptr < f.File_Size)
            {
                FAT12_Fread(&f, CodeBuffer, 512);
                for (j = 0; j < 512; j++)
                    EEPROM_WriteByte(Code_ptr++, CodeBuffer[j]);
                Jindu = Code_ptr - BootLoaderSize;
                if (Jindu && Jindu % 512 == 0)
                    OLED_ShowChar(Jindu / ((f.File_Size - BootLoaderSize) / 16) * 8, 6, '#', 16);
                // 更新进度条
            }
            OLED_ShowChar(120, 6, '#', 16);
            EEPROM_WriteByte(APP_FLag, APP_ID);
            // 烧录完成后，再更新标志位
            Code_ptr = APP_Entry;
            // 加载模块：HEX文件转码烧录
        }
        else
        { // HEX解码烧录
            OLED_Clear();
            FAT12_Fopen(&f, f.File_Name);
            OLED_ShowStr(0, 0, f.File_Name, 12);
            OLED_ShowNum(0, 2, f.File_Size, 6, 16);
            OLED_ShowString(48, 2, " Bytes", 16);
            EEPROM_SectorErase(APP_FLag);
            for (i = APP_Entry; i < 0xffff - 512; i += 512)
                EEPROM_SectorErase(i);
            OLED_ShowString(0, 4, "Load App...", 16);
            HexOffset = 0;
            FAT12_Fseek(&f, 0, 0);
            ByteCnt = 0;
            while (!FAT12_IsEOF(&f) && ByteCnt < f.File_Size)
            {
                FAT12_Fread(&f, HexBuffer, 64); // HEX文件按行读取（一行肯定小于64个字符，所以就一口气读64BYTES）
                hs = (Hex_Segment *)(HexBuffer);
                AddrTmp = Get_int32(hs->HexAddr[0], hs->HexAddr[1], hs->HexAddr[2], hs->HexAddr[3]);
                LenthTmp = Get_int8(hs->HexLenth[0], hs->HexLenth[1]);
                if (LenthTmp == 0)
                    break;
                for (i = 0; i < LenthTmp * 2; i += 2)
                {
                    DataTmp = Get_int8(hs->HexDat[i], hs->HexDat[i + 1]);
                    if (AddrTmp >= APP_Entry)
                    {
                        EEPROM_WriteByte(AddrTmp + i / 2, DataTmp);
                        ByteCnt++;
                    }
                    if (ByteCnt && ByteCnt % 256 == 0)
                        OLED_ShowChar(Jindu / (f.File_Size / 16) * 8, 6, '#', 16);
                    // 更新进度条
                }
                Jindu = HexOffset;
                HexOffset += (LenthTmp * 2 + 13);
                FAT12_Fseek(&f, HexOffset, 0); // 跳到下一行首位
            }
            EEPROM_WriteByte(APP_FLag, APP_ID);
            // 烧录完成后，再更新标志位
        }
    }
    // 跳转到APP
    App_Jmp = APP_Entry;
    App_Jmp();
    while (1)
        ;
}

uchar Key_Scan()
{
    uchar i, j;
    uint tmp, samples[11];
    ADC_Init();
    for (i = 0; i < 10; i++)
    {
        samples[i] = ADC_Result(ADC_CH0, ADC_SPEEDLL);
        for (j = i - 1; j > 0 && i > 0; j--)
        {
            if (samples[j + 1] < samples[j])
            {
                tmp = samples[j + 1];
                samples[j + 1] = samples[j];
                samples[j] = tmp;
            }
        }
    }
    if (samples[5] >= 0 && samples[5] <= 209)
        return 1; // 向下
    else if (samples[5] >= 381 && samples[5] <= 533)
        return 2; // 向上
    else if (samples[5] >= 680 && samples[5] <= 819)
        return 3; // 确认
    else
        return 0xff;
}
void delayms(uint t)
{
    uint i;
    while (t--)
        for (i = 0; i < 600; i++)
            ;
    return;
}
uint32_t Get_int32(uchar a, uchar b, uchar c, uchar d)
{
    uint32_t res = 0;
    if (a >= '0' && a <= '9')
        res += ((a - 48) * 4096);
    else
        res += ((a - 55) * 4096);
    if (b >= '0' && b <= '9')
        res += ((b - 48) * 256);
    else
        res += ((b - 55) * 256);
    if (c >= '0' && c <= '9')
        res += ((c - 48) * 16);
    else
        res += ((c - 55) * 16);
    if (d >= '0' && d <= '9')
        res += ((d - 48) * 1);
    else
        res += ((d - 55) * 1);
    return res;
}
uint8_t Get_int8(uchar c, uchar d)
{
    uint8_t res = 0;
    if (c >= '0' && c <= '9')
        res += ((c - 48) * 16);
    else
        res += ((c - 55) * 16);
    if (d >= '0' && d <= '9')
        res += ((d - 48) * 1);
    else
        res += ((d - 55) * 1);
    return res;
}
void OLED_ShowStr(uchar x, uchar y, uchar str[], uchar num)
{
    uchar i = 0;
    for (i = 0; i < num; i++)
        if (x + i * 8 < 120)
            OLED_ShowChar(x + i * 8, y, str[i], 16);
}