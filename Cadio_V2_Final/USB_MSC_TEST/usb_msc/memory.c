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


#define SD_CARD



#ifdef FLASH

#include "stc.h"
#include "memory.h"
#include "oled.h"


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

extern void delay(uint t);
void Flash_SelectChip(uchar FlashID)
{ // 0:Using 8M Flash 1:Using 4M Flash
    if (FlashID)
        Flash_Select = 1;
    else
        Flash_Select = 0;
}
void Flash_CSx(unsigned char id)
{
    if (Flash_Select == 0)
        CS = id;
    else
        CS1 = id;
}
#if Enable_HardWareSPI

static BYTE shift(BYTE dat)
{
    SPSTAT = SPIF + WCOL; // 清0 SPIF和WCOL标志
    SPDAT = dat;
    while ((SPSTAT & SPIF) == 0)
        ;

    return SPDAT;
}
#else
static BYTE shift(uchar dat)
{
    uchar i, res = 0;
    for (i = 0; i < 8; i++)
    {
        SI = dat & 0x80;
        CLK = 1;
        dat <<= 1;
        res <<= 1;
        res |= SO;
        CLK = 0;
    }
    SI = 1;
    // 模式0：时钟极性：0 时钟相位：0
    // 双方都是：时钟上升沿采样，下降沿更新数据
    return res;
}
#endif
void flash_init(uchar id)
{
#if Enable_HardWareSPI
    P_SW1 |= 0x0c; // IO口切换. 0: P1.2/P5.4 P1.3 P1.4 P1.5, 1: P2.2 P2.3 P2.4 P2.5, 2: P5.4 P4.0 P4.1 P4.3, 3: P3.5 P3.4 P3.3 P3.2
    SPCTL = 0xd1;
#endif
    Flash_CSx(1);
    WP = 1;
    SPSTAT = SPIF + WCOL; // 清0 SPIF和WCOL标志
    fFlashOK = 0;
    Flash_SelectChip(id);
    flash_check();
}

BOOL flash_check()
{
    BYTE dat1;
    BYTE dat2;
    BYTE dat3;
    if (!fFlashOK)
    {
        CLK = 0;
        Flash_CSx(0);
        shift(SFC_RDJID);
        dat1 = shift(0x00);
        dat2 = shift(0x00);
        dat3 = shift(0x00);
        // showbit(dat1);
        // showbit(dat2);
        // showbit(dat3);
        Flash_CSx(1);

        // C8 40 13 (GD25Q40)
        // EF 30 13 (W25X40)
        // EF 40 13 (W25Q40)
        if (1 || ((dat1 == 0xef) && (dat2 == 0x30)) ||
            ((dat1 == 0xef) && (dat2 == 0x40)) ||
            ((dat1 == 0xc8) && (dat2 == 0x40)))
        {
            if (dat3 == 0x11)
            {
                dwMemorySize = 128 * 1024UL; // GD25Q10/W25X10/W25X10
            }
            else if (dat3 == 0x12)
            {
                dwMemorySize = 256 * 1024UL; // GD25Q20/W25X20/W25X20
            }
            else if (dat3 == 0x13)
            {
                dwMemorySize = 512 * 1024UL; // GD25Q40/W25X40/W25X40
            }
            else if (dat3 == 0x14)
            {
                dwMemorySize = 1024 * 1024UL; // GD25Q80/W25X80/W25X80
            }
            else if (dat3 == 0x15)
            {
                dwMemorySize = 2048 * 1024UL; // GD25Q16/W25X16/W25X16
            }
            else if (dat3 == 0x16)
            {
                dwMemorySize = 4096 * 1024UL; // GD25Q32/W25X32/W25X32
            }
            else if (dat3 == 0x17)
            {
                dwMemorySize = 4096 * 2 * 1024UL; // GD25Q32/W25X32/W25X32
            }
            else
            {
                while (1)
                    OLED_ShowString(0, 0, "Flash_Error!", 16);
            }
            wSectorSize = 4096UL;
            dwSectorNumber = dwMemorySize / wSectorSize;

            fFlashOK = 1;
        }
        else
        {
            while (1)
            {
                OLED_ShowString(0, 0, "Flash_Error!", 16);
            }
            dwMemorySize = 0;
            wSectorSize = 0;
            dwSectorNumber = 0;
        }
    }

    return fFlashOK;
}

BOOL flash_isbusy()
{
    BYTE dat;
    CLK = 0;
    Flash_CSx(0);
    shift(SFC_RDSR);
    dat = shift(0);
    Flash_CSx(1);

    return (dat & 0x01);
}

void flash_write_enable()
{
    while (flash_isbusy())
        ;
    CLK = 0;
    Flash_CSx(0);
    shift(SFC_WREN);
    Flash_CSx(1);
}

void flash_readsector(DWORD addr, BYTE *pdat)
{
    WORD size;

    if (!flash_check())
        return;

    while (flash_isbusy())
        ;
    CLK = 0;
    Flash_CSx(0);
    shift(SFC_READ);
    shift((BYTE)(addr >> 16));
    shift((BYTE)(addr >> 8));
    shift((BYTE)(addr));
    for (size = 4096; size; size--)
    {
        *pdat++ = shift(0);
    }
    Flash_CSx(1);
}

void flash_writesector(DWORD addr, BYTE *pdat)
{
    WORD size;

    if (!flash_check())
        return;

    flash_write_enable();
    CLK = 0;
    Flash_CSx(0);
    shift(SFC_SECTORER);
    shift((BYTE)(addr >> 16));
    shift((BYTE)(addr >> 8));
    shift((BYTE)(addr));
    Flash_CSx(1);
    size = 4096;
    while (size)
    {
        flash_write_enable();
        CLK = 0;
        Flash_CSx(0);
        shift(SFC_PAGEPROG);
        shift((BYTE)(addr >> 16));
        shift((BYTE)(addr >> 8));
        shift((BYTE)(addr));
        while (size--)
        {
            shift(*pdat++);
            addr++;

            if ((BYTE)(addr) == 0x00)
                break;
        }
        Flash_CSx(1);
    }
}

#endif



#ifdef SD_CARD

#include "stc.h"
#include "memory.h"
#include "oled.h"
#include "SD.h"

DWORD dwMemorySize;
WORD wSectorSize;
DWORD dwSectorNumber;
uint8_t fFlashOK;


void flash_init(uchar id){
    uint8_t res = 0;
    res = SD_Init();
    if(res){
        OLED_Printf(0,0,16,"Error: %bu",res);
        while(1);
    }

    flash_check();
    
    return;
}

BOOL flash_check()
{
    
    dwMemorySize = 4096 * 2 * 1024UL;
    wSectorSize = 4096UL;
    dwSectorNumber = dwMemorySize / wSectorSize;
    fFlashOK = 1;
    return fFlashOK;
}

BOOL flash_isbusy()
{

}

void flash_write_enable()
{

}

void flash_readsector(DWORD addr, BYTE *pdat)
{
    uint8_t i;
    for(i = 0;i < 8; i++) SD_ReadDisk(pdat,addr*8 + i,1);
}

void flash_writesector(DWORD addr, BYTE *pdat)
{
    SD_WriteDisk(pdat,addr*8,8);
}





#endif

