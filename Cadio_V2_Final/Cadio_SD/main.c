#include "_STC8X_.h"
#include "TYPE.h"
#include "oled.h"
#include "ff.h"
#include "SD.h"
#include "Graphic.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "Key.h"

#define MAIN_Fosc 40000000L // ������ʱ��
sbit Wifi = P2 ^ 0;
sbit Audio = P2 ^ 1;
sbit laser = P1 ^ 3;
sbit light = P1 ^ 7;
sbit flash_cs1 = P3 ^ 5;
sbit flash_cs2 = P2 ^ 3;
sbit sd_cs = P1 ^ 6;
typedef struct
{
    uint8_t ChunkID[4];
    uint8_t ChunkSize[4];
    uint8_t Format[4];
    uint8_t Subchunk1ID[4];
    uint8_t Sbuchunk1Size[4];
    uint8_t AudioFormat[2];
    uint8_t NumChannels[2];
    uint8_t SampleRate[4];
    uint8_t ByteRate[4];
    uint8_t BlockAlign[2];
    uint8_t BitPerSample[2];
    uint8_t Subchunk2ID[4];
    uint8_t Subchunk2Size[4];
} Wave;
Wave Wave_FHD;

void Delay1000ms(void) //@40.000MHz
{
    unsigned char data i, j, k;

    _nop_();
    i = 203;
    j = 236;
    k = 131;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void Delay100ms(void) //@40.000MHz
{
    unsigned char data i, j, k;

    _nop_();
    i = 21;
    j = 75;
    k = 189;
    do
    {
        do
        {
            while (--k)
                ;
        } while (--j);
    } while (--i);
}

void Sys_Init()
{

    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M1 = 0x3f;
    P1M0 = 0x00; // ����Ϊ���迹
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 = 0xff;
    P5M1 = 0x00;

    Wifi = 0;
    laser = 0;
    Audio = 0;
    light = 0;
    flash_cs1 = 1;
    flash_cs2 = 1;
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(1);
    OLED_Clear();
    Delay1000ms();
}
FATFS SD_FS;
FRESULT res, res1, res2;
FIL testfile;
DIR dir;
FILINFO fno;
uint16_t rn = 1;
uint32_t tick = 0;
uint8_t sfn[13], lfn[16];
// unsigned edata bf[512];

uint8_t Timer1_Config(u16 reload) // reloadˇא׏ƵÊ/Hz
{
    TR1 = 0;      // ֹ݆ͣ˽
    AUXR |= 0x40; // 定时器时�?1T模式
    TMOD &= 0x0F; // 设置定时器模�?
    reload = 0xffff - reload;
    TH1 = (uint8_t)(reload >> 8);
    TL1 = (uint8_t)(reload);
    ET1 = 1; // Պѭא׏
    TR1 = 1; // ߪʼՋѐ
    return 0;
}
void PWM_Config(void)
{
    P_SW2 |= 0x80; // SFR enable

    PWMB_PSCR = 0x0000; // Ԥ��Ƶ�Ĵ���, ��Ƶ Fck_cnt = Fck_psc/(PSCR[15:0}+1), ���ض���PWMƵ�� = SYSclk/((PSCR+1)*(AAR+1)), �������PWMƵ�� = SYSclk/((PSCR+1)*(AAR+1)*2).
    PWMB_DTR = 00;      // ����ʱ������,   n=0~127: DTR= n T,   0x80 ~(0x80+n), n=0~63: DTR=(64+n)*2T,
                        // ��PWMBû������  0xc0 ~(0xc0+n), n=0~31: DTR=(32+n)*8T,   0xE0 ~(0xE0+n), n=0~31: DTR=(32+n)*16T,
    PWMB_ARR = 256;     // �Զ���װ�ؼĴ���, ����PWM����

    PWMB_CCMR2 = 0x68; // ͨ��ģʽ����, PWMģʽ1, Ԥװ������
    PWMB_CCR2L = 128;  // �������Ƚ�ֵ, ����ռ�ձ�
    PWMB_CCER1 = 0x10; // ����ͨ�����ʹ�ܺͼ���? PWM6 PWM5

    PWMB_BKR = 0x80; // �����ʹ��? �൱���ܿ���
    PWMB_IER = 0x00; // ʹ���ж�
    PWMB_CR1 = 0x81; // ʹ�ܼ�����, �����Զ���װ�ؼĴ�������, ���ض���ģʽ, ���ϼ���, bit7=1:д�Զ���װ�ؼĴ�������(�����ڲ��ᱻ����), =0:ֱ��д�Զ���װ�ؼĴ���

    PWMB_ENO = 0x04; // IO���PWM����,
    PWMB_PS = 0x04;  // ѡ��IO
    // ��P5^4������?
}

#define MusicBufferSize 512
uint16_t Cnt = 0;
uint8_t bflg = 0, B_Play = 0;
uint8_t MusicBuffer0[MusicBufferSize + 2], MusicBuffer1[MusicBufferSize + 2];

#define KEY_UP 0x44
#define KEY_DOWN 0x49
#define KEY_LEFT 0x48
#define KEY_RIGHT 0x4a
#define KEY_BACK 0x45
#define KEY_EQUAL 0x3d
#define KEY_SHIFT 0x41

int i;
uint32_t SampleRate = 0;
uint8_t BytePerSample = 0;
uint32_t ByteRate = 0;
uint8_t Channel = 0;
uint8_t RealData = 0;
uint8_t index = 0, k = 1, k1 = 0, File_Num = 0;
uint8_t Next_Flg = 0, Prev_Flg = 0, Pause_Flg = 0;
uint8_t Play_Again_Auto = 0, Play_Again_Manu = 0;
uint8_t Use_SFN = 0, len_sfn = 0, len_lfn = 0;
uint8_t path[300];
uint8_t Keyleft = 0, Keyright = 0;

float Vol = 1;
uint32_t UpdateInfo = 0;
uint32_t UpdateKeybd = 0;
uint32_t PlayTime = 0;
uint8_t Second = 0, Minute = 0, tmp = 0;
uint32_t JmpLenth = 0;
float Process = 0;
uint32_t File_Size = 0;
uint32_t File_CurrentOffset = 0;
uint8_t Play_Mode = 1;

typedef struct
{
    uint8_t name[256];
    uint8_t altname[14];
    uint8_t lenth;
} Item;
Item wav_file;

uint8_t __is_ascii(uint8_t c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_') || (c == '.') || (c == '-');
}
uint8_t checkfile(FILINFO f)
{
    uint8_t i;
    if ((f.altname[9] != 'W') || (f.altname[10] != 'A') || (f.altname[11] != 'V'))
        return 0; // 不是.wav文件
    for (i = 0; f.fname[i] != '\0'; i++)
        if (__is_ascii(f.fname[i]) == 0)
            return 0; // �?有非法字符（或者存在中文）
    return 1;
}
void generatepath(uint8_t *filename)
{
    uint16_t i;
    for (i = 0; i < 300; i++)
        path[i] = 0;
    path[0] = '1';
    path[1] = ':';
    path[2] = '\\';
    path[3] = 'm';
    path[4] = 'u';
    path[5] = 's';
    path[6] = 'i';
    path[7] = 'c';
    path[8] = '\\';
    for (i = 0; filename[i] != '\0'; i++)
    {
        path[i + 9] = filename[i];
    }
}
uint32_t MakeBytes(uint8_t dat[], uint8_t lenth, uint8_t op)
{
    uint32_t res = 0;
    if (op == 1)
    { // Small Endian
        if (lenth >= 1)
            res |= ((uint32_t)(dat[0]));
        if (lenth >= 2)
            res |= ((uint32_t)(dat[1])) << 8;
        if (lenth >= 3)
            res |= ((uint32_t)(dat[2])) << 16;
        if (lenth >= 4)
            res |= ((uint32_t)(dat[3])) << 24;
    }
    else
    { // Big Endian
        if (lenth >= 1)
            res |= ((uint32_t)(dat[3]));
        if (lenth >= 2)
            res |= ((uint32_t)(dat[2])) << 8;
        if (lenth >= 3)
            res |= ((uint32_t)(dat[1])) << 16;
        if (lenth >= 4)
            res |= ((uint32_t)(dat[0])) << 24;
    }
    return res;
}
uint8_t __is_wav(uint8_t *str)
{
    return (str[9] == 'W') && (str[10] == 'A') && (str[11] == 'V');
}
uint8_t __strlen(uint8_t *str)
{
    uint8_t i;
    for (i = 0; str[i] != '\0'; i++)
        ;
    return i;
}
void __memset(uint8_t *s, uint8_t lenth, uint8_t value)
{
    uint16_t i = 0;
    for (i = 0; i < lenth; i++)
        *(s + i) = value;
    return;
}
uint8_t Get_Filenum(uint8_t *path)
{
    DIR __dir;
    FILINFO __fno;
    uint8_t file_cnt = 0;
    res = f_opendir(&__dir, path);
    while (1)
    {
        res = f_readdir(&__dir, &__fno); // 读取�?录项
        if (res != FR_OK || __fno.fname[0] == 0)
            break;
        if (__fno.fname[0] == '.')
            continue; // 跳过当前�?录和父目录条�?
        // 判断�?文件还是�?�?
        if (__fno.fattrib & AM_DIR)
        {
            // �?�?�?
            // OLED_Printf(0, cnt1*2, 16, "%s", fno.fname);
            // cnt1++;
            // index++;
        }
        else
        {
            // �?文件
            // OLED_Printf(0, cnt2*2, 16, "%s", fno.fname);
            // Graphic_Printf(0,index*8,1,"%s",fno.fname);
            // if(fno.)

            if (checkfile(__fno))
                file_cnt++;
        }
    }
    return file_cnt;
}
void Get_Nth(uint8_t *path, uint8_t n, uint8_t *filename, uint8_t *altname)
{
    uint16_t i = 0;
    DIR __dir;
    FILINFO __fno;
    uint8_t file_index = 0;
    for (i = 0; i < FF_MAX_LFN; i++)
        __fno.fname[i] = 0;
    for (i = 0; i < FF_SFN_BUF; i++)
        __fno.altname[i] = 0;
    res = f_opendir(&__dir, path);
    while (1)
    {
        res = f_readdir(&__dir, &__fno); // 读取�?录项
        if (res != FR_OK || __fno.fname[0] == 0)
            break;
        if (__fno.fname[0] == '.')
            continue; // 跳过当前�?录和父目录条�?
        // 判断�?文件还是�?�?
        if (__fno.fattrib & AM_DIR)
        {
            // �?�?�?
            // OLED_Printf(0, cnt1*2, 16, "%s", fno.fname);
            // cnt1++;
            // index++;
        }
        else
        {
            // �?文件
            // OLED_Printf(0, cnt2*2, 16, "%s", fno.fname);
            // Graphic_Printf(0,index*8,1,"%s",fno.fname);
            // if(fno.)
            if (checkfile(__fno))
                file_index++;
            if (file_index == n)
            {
                for (i = 0; __fno.fname[i] != 0; i++)
                {
                    filename[i] = __fno.fname[i];
                }
                for (i = 0; __fno.altname[i] != 0; i++)
                {
                    altname[i] = __fno.altname[i];
                }
                f_closedir(&dir);
                return;
            }
        }
    }
    filename = NULL;
    f_closedir(&dir);
    return;
}
uint16_t pos = 0;

unsigned char code Icons[3][32] = {
    {// 单曲�?�?
     /*--  调入了一幅图像：C:\Users\nichengyan\Desktop\205.bmp  --*/
     /*--  宽度x高度=16x16  --*/
     0x7F, 0x7F, 0x1F, 0x07, 0x07, 0x43, 0x63, 0xE1, 0xE1, 0x7F, 0x3F, 0x1F, 0x0F, 0x0F, 0x1F, 0x3F,
     0xFE, 0xF8, 0xF0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0x87, 0xC7, 0xC2, 0xC0, 0xE0, 0xF0, 0xFE, 0xFE},

    {// 顺序�?�?
     /*--  调入了一幅图像：C:\Users\nichengyan\Desktop\270.bmp  --*/
     /*--  宽度x高度=16x16  --*/
     0xE1, 0xE1, 0xE1, 0xE1, 0x61, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0x80, 0xC0, 0xC0, 0xE1, 0xF3,
     0xF3, 0xE1, 0xC0, 0xC0, 0x80, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1, 0xE1

    },
    {// 随机�?�?
     /*--  调入了一幅图像：C:\Users\nichengyan\Desktop\242.bmp  --*/
     /*--  宽度x高度=16x16  --*/
     0xE1, 0xE1, 0xE1, 0x83, 0x87, 0x0F, 0x1F, 0x0F, 0x07, 0x83, 0xC1, 0x80, 0xC0, 0xC0, 0xE1, 0xF3,
     0x87, 0x87, 0x87, 0xC1, 0xE0, 0xF0, 0xF8, 0xF8, 0xF0, 0xC1, 0xC3, 0x05, 0x03, 0x03, 0x87, 0xCF}};
unsigned char code Str[][5] = {
    "Once",
    "Sequ",
    "Rand"};







uint8_t res = 0;
void main()
{
    
    Sys_Init();
    res = SD_Init();
    OLED_Printf(0,0,16,"%bd",res);
    //while(1);
    res = f_mount(&SD_FS, "1:", 1);
    if (res != FR_OK)
    {
        // OLED_ShowString(0,0,(uint8_t*)"Disk Error!",16);
        OLED_Printf(0, 2, 16, "Disk Error %bd", res);
        while (1)
            ;
    }


    
Menu:
    File_Num = Get_Filenum("1:\\music");
    //__memset(Name,255,0);
    // Get_Nth("1:\\music",2,Name);

    Graphic_SetFont(1);
    while (1)
    {
        Graphic_ClearBufferBit(0);
        /*
        if(k1 > 7){
            k1 = 7;
        }
        for(i = 0; i < 7; i++){
            __memset(wav_file[i].name,255,0);
            wav_file[i].lenth = 0;
            if(i+k < File_Num){
                Get_Nth("1:\\music",k + i,wav_file[i].name);
            }

            Graphic_Printf(0,i*8,1,"%s",wav_file[i].name);
        }
        */
        __memset(wav_file.name, 255, 0);
        __memset(wav_file.altname, 14, 0);
        wav_file.lenth = 0;

        Get_Nth("1:\\music", k, wav_file.name, wav_file.altname);

        wav_file.lenth = __strlen(wav_file.name);
        Graphic_Printf(0, 0, 0, "Wav To Play:");
        Graphic_Printf(0, 32, 0, "Play Mode:");
        if (wav_file.lenth <= 16)
        {
            Graphic_Printf(0, 16, 1, "%s", wav_file.name);
        }
        else
        {
            Graphic_Printf(4 - pos, 16, 1, "%s", wav_file.name);
            pos++;
            if (pos > wav_file.lenth * 8 - 124)
                pos = 0;
        }
        Graphic_Printf(24, 48, 1, "%s", Str[Play_Mode]);
        Graphic_DrawBmp(64, 48, 16, 16, Icons[Play_Mode]);
        Graphic_Flush();
        Keyright = Key_Scan() & 0x00ff;
        if (Keyright == KEY_DOWN)
        {
            if (k == File_Num)
                k = 1;
            else
                k++;
        }
        if (Keyright == KEY_UP && k > 1)
        {
            if (k == 1)
                k = File_Num;
            else
                k--;
        }
        if (Keyright == KEY_RIGHT)
        {
            if (Play_Mode == 2)
                Play_Mode = 0;
            else
                Play_Mode++;
        }
        if (Keyright == KEY_LEFT)
        {
            if (Play_Mode == 0)
                Play_Mode = 2;
            else
                Play_Mode--;
        }
        if (Keyright == KEY_EQUAL)
            break;
    }

Again:
    //
    if (Play_Again_Manu)
    {
        Play_Again_Manu = 0;
        if (Next_Flg)
        {
            Next_Flg = 0;
            if (k == File_Num)
                k = 1;
            else
                k++;
        }
        else if (Prev_Flg)
        {
            Prev_Flg = 0;
            if (k == 1)
                k = File_Num;
            else
                k--;
        }
    }
    else if (Play_Again_Auto)
    {
        Play_Again_Auto = 0;
        if (Play_Mode == 0)
        { // 单曲�?�?
            k = k;
        }
        else if (Play_Mode == 1)
        { // 顺序�?�?
            if (k == File_Num)
                k = 1;
            else
                k++;
        }
        else if (Play_Mode == 2)
        {
            k = rand() % File_Num + 1;
        }
    }

    __memset(wav_file.name, 255, 0);
    __memset(wav_file.altname, 14, 0);
    wav_file.lenth = 0;
    Get_Nth("1:\\music", k, wav_file.name, wav_file.altname);
    wav_file.lenth = __strlen(wav_file.name);
    generatepath(wav_file.name);
    Graphic_SetFont(0);
    // Graphic_Printf(0,32,1,"%s",path);
    // Graphic_SetFont(1);
    // Graphic_Flush();
    res1 = f_open(&testfile, path, FA_READ);
    // res1 =f_open(&vid, "1:\\Test\\badapple.vid", FA_READ);

    if (res1 != FR_OK)
    {
        OLED_Printf(0, 0, 16, (uint8_t *)"File Error! %bd", res1);
        while (1)
            ;
    }
    // OLED_Printf(0,0,16,"ok");
    f_read(&testfile, &Wave_FHD, sizeof(Wave_FHD), &rn);

    // OLED_ShowStr(0,0,f.File_Name,12);
    File_Size = f_size(&testfile);
    // OLED_Printf(0,0,16,"%ld",File_Size);
    // while(1);
    OLED_Clear();
    JmpLenth = 2 * ((File_Size - sizeof(Wave_FHD)) / 200); // һ֨Ҫˇٶż˽áááһȻܡԶխ
    // OLED_Printf(0,0,16,"%ld",JmpLenth);
    SampleRate = MakeBytes(Wave_FHD.SampleRate, 4, 1);
    BytePerSample = MakeBytes(Wave_FHD.BitPerSample, 2, 1) / 8;
    ByteRate = MakeBytes(Wave_FHD.ByteRate, 4, 1);   // ëÊ(Byte/s)
    Channel = MakeBytes(Wave_FHD.NumChannels, 2, 1); // ʹրٶ˽è˫ʹր۰ߍѡձ۶ÔǤאһٶʹրé
    RealData = Channel * BytePerSample;
    PlayTime = (File_Size - sizeof(Wave_FHD)) / ByteRate; // t=datasize/byterate
    if (ByteRate > 44100 || Channel != 1)
    {
        OLED_Clear();
        OLED_ShowString(0, 0, "Format Error!", 16);
        OLED_ShowString(0, 2, "Can't be Played", 16);
        goto Menu;
    }
    OLED_Printf(0, 0, 16, "%s", wav_file.altname);
    EA = 1;
    Timer1_Config(MAIN_Fosc / SampleRate);
    PWM_Config();
    Audio = 1;
    OLED_ShowNum(40, 6, Minute / 10, 1, 16);
    OLED_ShowNum(48, 6, Minute % 10, 1, 16);
    OLED_ShowChar(56, 6, ':', 16);
    OLED_ShowNum(64, 6, Second / 10, 1, 16);
    OLED_ShowNum(72, 6, Second % 10, 1, 16);
    f_rewind(&testfile);
    /*
    while(1){
        if(!B_Play&&bflg==0){
            Cnt=0;
            B_Play=1;
            bflg=1;
            res1 = f_read(&testfile,MusicBuffer0,MusicBufferSize,&rn);
        }
        if(!B_Play&&bflg==1){
            Cnt=0;
            B_Play=1;
            bflg=0;
            res1 = f_read(&testfile,MusicBuffer1,MusicBufferSize,&rn);
        }
    }
    */

    while (1)
    {
        if (!B_Play && bflg == 0)
        {
            Cnt = 0;
            B_Play = 1;
            bflg = 1;
            res1 = f_read(&testfile, MusicBuffer0, MusicBufferSize, &rn);
        }
        if (!B_Play && bflg == 1)
        {
            Cnt = 0;
            B_Play = 1;
            bflg = 0;
            res1 = f_read(&testfile, MusicBuffer1, MusicBufferSize, &rn);
        }
        if (res1 != FR_OK || rn == 0)
            break;
        Keyright = 0xff;
        Keyleft = 0xff;
        if (UpdateKeybd > 4000)
        {
            UpdateKeybd = 0;
            Keyright = Key_Scan();
            Keyleft = Key_Scan() >> 8;
            File_CurrentOffset = f_tell(&testfile);
            if (Keyright == KEY_RIGHT)
            {
                // B_Play = 1;
                if (Keyleft == KEY_SHIFT)
                {
                    Next_Flg = 1;
                    Play_Again_Auto = 0;
                    Play_Again_Manu = 1;
                    goto Again;
                }
                else
                {
                    if (File_CurrentOffset + JmpLenth <= File_Size)
                    {
                        // FAT12_Fseek(&f,File_CurrentOffset+JmpLenth,0);
                        f_lseek(&testfile, File_CurrentOffset + JmpLenth);
                    }
                }
            }
            if (Keyright == KEY_LEFT)
            {
                // B_Play = 1;
                if (Keyleft == KEY_SHIFT)
                {
                    Prev_Flg = 1;
                    Play_Again_Auto = 0;
                    Play_Again_Manu = 1;
                    goto Again;
                }
                else
                {
                    if (File_CurrentOffset >= 2 * JmpLenth)
                    {
                        // FAT12_Fseek(&f,File_CurrentOffset-4*JmpLenth,0);
                        f_lseek(&testfile, File_CurrentOffset - 2 * JmpLenth);
                    }
                }
            }
            if (Keyright == KEY_UP)
            {
                if (Vol <= 2)
                    Vol += 0.1;
                OLED_ShowNum(40, 2, Vol * 100, 3, 16);
            }
            if (Keyright == KEY_DOWN)
            {
                if (Vol >= 0)
                    Vol -= 0.1;
                OLED_ShowNum(40, 2, Vol * 100, 3, 16);
            }
            if (Keyleft == KEY_BACK)
            {
                goto Menu;
            }
            if (Keyright == KEY_EQUAL)
            {
                while (Key_Scan() != 0xffff)
                    ;
                OLED_Printf(0, 6, 16, "     Pause       ");
                EA = 0;
                while (1)
                {
                    Delay100ms();
                    Keyright = Key_Scan();
                    if (Keyright == KEY_EQUAL)
                        break;
                }
                OLED_ShowNum(40, 6, Minute / 10, 1, 16);
                OLED_ShowNum(48, 6, Minute % 10, 1, 16);
                OLED_ShowChar(56, 6, ':', 16);
                OLED_ShowNum(64, 6, Second / 10, 1, 16);
                OLED_ShowNum(72, 6, Second % 10, 1, 16);
                EA = 1;
            }
        }
        if (UpdateInfo > 10000)
        {
            UpdateInfo = 0;
            OLED_ShowString(0, 2, "Vol:", 16);
            OLED_ShowNum(40, 2, Vol * 100, 3, 16);
            OLED_ShowString(72, 2, "%", 16);
            Process = (float)(File_CurrentOffset - sizeof(Wave_FHD)) / (float)File_Size;
            for (i = 0; i < 16; i++)
            {
                if (i < 16 * Process)
                    OLED_ShowChar(i * 8, 4, '#', 16);
                else
                    OLED_ShowChar(i * 8, 4, ' ', 16);
            }
            Second = (u8)(PlayTime * Process) % 60;
            Minute = (u8)(PlayTime * Process) / 60;
            OLED_ShowNum(64, 6, Second / 10, 1, 16);
            OLED_ShowNum(72, 6, Second % 10, 1, 16);
            if (tmp != Minute)
            {
                tmp = Minute;
                OLED_ShowNum(40, 6, Minute / 10, 1, 16);
                OLED_ShowNum(48, 6, Minute % 10, 1, 16);
            }
        }
    }
    EA = 0;
    Play_Again_Auto = 1;
    Play_Again_Manu = 0;
    goto Again;
    while (1)
        ;
}
// �ö�ʱ��1�ж�
void timer0_ISR(void) interrupt 3
{

    if (B_Play)
    {
        if (bflg == 0)
        {
            // 8λϞػۅwav˽ߝ̍PWMռࠕ�?
            if (BytePerSample == 2)
            {
                PWMB_CCR2L = (u8)(MusicBuffer0[Cnt + 1] + 128) * Vol;
                Cnt += 2;
            }
            else if (BytePerSample == 1)
            {
                PWMB_CCR2L = (u8)(MusicBuffer0[Cnt]) * Vol;
                Cnt += 1;
            }
            if (Cnt >= MusicBufferSize)
            {
                B_Play = 0;
            }
        }
        if (bflg == 1)
        {
            // 8λϞػۅwav˽ߝ̍PWMռࠕ�?
            if (BytePerSample == 2)
            {
                PWMB_CCR2L = (u8)(MusicBuffer1[Cnt + 1] + 128) * Vol;
                Cnt += 2;
            }
            else if (BytePerSample == 1)
            {
                PWMB_CCR2L = (u8)(MusicBuffer1[Cnt]) * Vol;
                Cnt += 1;
            }
            if (Cnt >= MusicBufferSize)
            {
                B_Play = 0;
            }
        }
    }
    UpdateInfo++;
    UpdateKeybd++;
}