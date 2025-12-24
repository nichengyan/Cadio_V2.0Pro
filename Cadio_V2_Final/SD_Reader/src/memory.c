/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCAI.com ---------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- BBS: www.STCAIMCU.com  -----------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序中使用此代码,请在程序中注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/

#include "stc.h"
#include "memory.h"
#include "oled.h"
#include "SD.h"

DWORD dwMemorySize;
WORD wSectorSize;
DWORD dwSectorNumber;

#define SFC_WREN 0x06
#define SFC_WRDI 0x04
#define SFC_RDSR 0x05
#define SFC_WRSR 0x01
#define SFC_READ 0x03
#define SFC_FASTREAD 0x0B
#define SFC_RDID 0xAB
#define SFC_PAGEPROG 0x02
#define SFC_RDCR 0xA1
#define SFC_WRCR 0xF1
#define SFC_SECTORER 0x20
#define SFC_BLOCKER 0xD8
#define SFC_CHIPER 0xC7
#define SFC_RDJID 0x9F

#define SPIF 0x80 // SPI传输完成标志。写入1清0。
#define WCOL 0x40 // SPI写冲突标志。写入1清0。
#define Enable_HardWareSPI 0
static BOOL fFlashOK;
static unsigned char Flash_Select = 0;

sbit CS1 = P2 ^ 3;
sbit CS = P3 ^ 5;
sbit SO = P3 ^ 3;
sbit SI = P3 ^ 4;
sbit CLK = P3 ^ 2;
sbit WP = P3 ^ 7;
sbit HOLD = P3 ^ 6;
sbit light = P1^7;

extern void delay(uint t);

void flash_init(uchar id)
{
    uint8_t res;
    res = SD_Init();
    OLED_Printf(0,0,16,"%bu",res);
    dwMemorySize = 4096ul*1024ul;
    wSectorSize = 512ul;
    dwSectorNumber = dwMemorySize/wSectorSize;
}

BOOL flash_check()
{
    

    //return 0;
}

void flash_readsector(DWORD addr, BYTE *pdat)
{
    //OLED_Printf(0,4, 16,"read");
    EA = 0;
    while(1);
    SD_ReadDisk(pdat,addr*8,8);
    light = ~light;
}

void flash_writesector(DWORD addr, BYTE *pdat)
{
    SD_WriteDisk(pdat,addr*8,8);
}
