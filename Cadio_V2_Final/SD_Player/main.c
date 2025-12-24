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
#include "Volume.h"
#define MAIN_Fosc 40000000L // ������ʱ��
//定义管脚，和Cadio硬件平台有关

sbit Wifi = P2 ^ 0;
sbit Audio = P2 ^ 1;
sbit laser = P1 ^ 3;
sbit light = P1 ^ 7;
sbit flash_cs1 = P3 ^ 5;
sbit flash_cs2 = P2 ^ 3;
sbit sd_cs1 = P1 ^ 6;


/*----------------Wav文件信息有关部分----------------*/

//定义wav文件头结构体
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


uint32_t SampleRate = 0;
uint8_t BytePerSample = 0;
uint32_t ByteRate = 0;
uint8_t Channel = 0;
uint8_t RealData = 0;

/* ------------------------------------------- */

/// @brief 延迟1000ms
/// @param void

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

/// @brief 延迟100ms
/// @param void
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

/// @brief 系统初�?�化
/// @param void
void Sys_Init(void)
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
    OLED_ColorTurn(0);
    OLED_Clear();
    //Delay1000ms();
}
/* -------------文件系统有关部分---------------- */
FATFS SD_FS; //文件系统对象
FRESULT res, res1, res2; //操作的结果
FIL testfile;//文件对象
DIR dir;//目录对象
FILINFO fno;//文件信息对象
uint16_t rn = 1;//接受的字节数
uint8_t path[300];//生成的目录字符串，记录每次打开的文件路径
uint8_t File_Num = 0;//当前目录下的文件数

/* ------------------------------------------- */

uint32_t tick = 0;//滴答计数


/// @brief 定时�?1配置
/// @param reload  set timer1 reload value
/// @return void
void Timer1_Config(u16 reload) // reloadˇא׏ƵÊ/Hz
{
    TR1 = 0;      // ֹ݆ͣ˽
    AUXR |= 0x40; // 定时器时�?1T模式
    TMOD &= 0x0F; // 设置定时器模�?
    reload = 0xffff - reload;
    TH1 = (uint8_t)(reload >> 8);
    TL1 = (uint8_t)(reload);
    ET1 = 1; // Պѭא׏
    TR1 = 1; // ߪʼՋѐ
    return;
}

/// @brief PWM配置
/// @param void
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


/* --------------播放采样有关部分----------------- */
#define MusicBufferSize 512
uint16_t Cnt = 0;
uint8_t bflg = 0, B_Play = 0;
uint8_t MusicBuffer0[MusicBufferSize + 2], MusicBuffer1[MusicBufferSize + 2];


/* ------------------播放控制部分----------------------- */

//键码值
#define KEY_UP 0x44     //音量加
#define KEY_DOWN 0x49   //音量减
#define KEY_RIGHT 0x4a  //快进（按shift再按这个是下一首）
#define KEY_LEFT 0x48   //快退 (按shift再按这个是上一首)
#define KEY_BACK 0x45   //按返回退回主页
#define KEY_EQUAL 0x3d  //按等号是确认
#define KEY_SHIFT 0x41  //Shift+ left/right键是切歌




uint8_t Next_Flg = 0, Prev_Flg = 0, Pause_Flg = 0; //下一首上一首暂停
uint8_t Play_Again_Auto = 0, Play_Again_Manu = 0;  //播完自动切换 播过程中手动切换
uint8_t Keyleft = 0, Keyright = 0; //左右按键组
uint8_t Vol = 10; //音量系数 
uint16_t UpdateInfo = 0;  //更新播放信息，进度时间什么的
uint16_t UpdateKeybd = 0; //更新按键信息，采样按键
uint32_t PlayTime = 0;    //播放时间
uint8_t Second = 0, Minute = 0,tmp = 0; //播放时间 分秒 刷新时间需要的中间变量

uint32_t JmpLenth = 0; //快进快退长度
float Process = 0;     //播放进度       
uint32_t File_Size = 0;   //文件大小，精确的
uint32_t File_CurrentOffset = 0;//当前文件偏移量

uint8_t Play_Mode = 1;//播放模式  //1单曲循环（默认） 2顺序播放 3随机播放


/*------------------------------------------------------------*/


//杂项
int i; //循环变量
uint8_t index = 0, k = 1, k1_ = 0; //index是选中的当前的文件序号，k是遍历文件的计数器


typedef struct
{
    uint8_t name[256];
    uint8_t altname[14];
    uint8_t lenth;
} Item;
Item wav_file;

/// @brief 判断字�?�是否为文件名可以支持的ASCII字�??
/// @param uint8_t c
/// @return true or false
uint8_t __is_ascii(uint8_t c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9') || (c == '_') || (c == '.') || (c == '-');
}

/// @brief 获取字�?�串长度
/// @param uint8_t *str
/// @return 字�?�串长度
uint8_t __strlen(uint8_t *str)
{
    uint8_t i;
    for (i = 0; str[i] != '\0'; i++)
        ;
    return i;
}

/// @brief 内存设置
/// @param uint8_t *s
/// @param uint8_t lenth
/// @param uint8_t value
/// @return void
void __memset(uint8_t *s, uint8_t lenth, uint8_t value)
{
    uint16_t i = 0;
    for (i = 0; i < lenth; i++)
        *(s + i) = value;
    return;
}

/// @brief 判断是不是.wav文件且文件名合法
/// @param FILINFO f
/// @return true or false
uint8_t checkfile(FILINFO f)
{
    uint8_t i;
    uint8_t l = strlen(f.altname);
    if(l <=4 ) return 0;
    if ((f.altname[l - 3] != 'W') || (f.altname[l - 2] != 'A') || (f.altname[l - 1] != 'V')){
        return 0; // 不是.wav文件   
    }
    for (i = 0; f.fname[i] != '\0'; i++)
        if (__is_ascii(f.fname[i]) == 0)
            return 0; // 有非法字符（或者存在中文）
    return 1;
}

/// @brief 生成文件绝对路径1:\music\filename
/// @param uint8_t *filename
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
/// @brief 将字节数组转换为整型数组
/// @param uint8_t dat
/// @param uint8_t lenth
/// @param uint8_t op
/// @return 根据op是0/1,按照大小端序整数拼接成的整数
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

/// @brief 获取目录下符合要求的文件数目
/// @param uint8_t *path
/// @return 文件数量
uint8_t Get_Filenum(uint8_t *path)
{
    DIR __dir;
    FILINFO __fno;
    uint8_t file_cnt = 0;
    res = f_opendir(&__dir, path);
    while (1)
    {
        res = f_readdir(&__dir, &__fno); // 读取目录项
        if (res != FR_OK || __fno.fname[0] == 0)
            break;
        if (__fno.fname[0] == '.')
            continue; // 跳过当前目录和父目录条目
        // 判断是文件还是目录
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

/// @brief 获取目录下第n个符合要求的文件名
/// @param uint8_t *path 文件所在文件夹�?�?
/// @param uint8_t n 第n个文件
/// @param uint8_t *filename 文件名
/// @param uint8_t *altname 文件短名
/// @return void

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
void main()
{

    Sys_Init();
    Delay1000ms();
    res = f_mount(&SD_FS, "1:", 1);
    if (res != FR_OK)
    {
        // OLED_ShowString(0,0,(uint8_t*)"Disk Error!",16);
        OLED_Printf(0, 0, 16, "Disk Error %bd", res);
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
        if (Keyright == KEY_UP)
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

    res1 = f_open(&testfile, path, FA_READ);

    if (res1 != FR_OK)
    {
        OLED_Printf(0, 0, 16, (uint8_t *)"File Error! %bd", res1);
        while (1)
            ;
    }
    f_read(&testfile, &Wave_FHD, sizeof(Wave_FHD), &rn);

    File_Size = f_size(&testfile);
    OLED_Clear();
    JmpLenth = 2 * ((File_Size - sizeof(Wave_FHD)) / 200); // 计算跳转长度

    SampleRate = MakeBytes(Wave_FHD.SampleRate, 4, 1);
    BytePerSample = MakeBytes(Wave_FHD.BitPerSample, 2, 1) / 8;
    ByteRate = MakeBytes(Wave_FHD.ByteRate, 4, 1);   // 码率
    Channel = MakeBytes(Wave_FHD.NumChannels, 2, 1); // 声道数
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
        if (UpdateKeybd > 40000)
        {
            UpdateKeybd = 0;
            Keyright = Key_Scan();
            Keyleft = Key_Scan() >> 8;
            File_CurrentOffset = f_tell(&testfile);
            if (Keyright == KEY_RIGHT)
            {
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
                        f_lseek(&testfile, File_CurrentOffset + JmpLenth);
                    }
                }
            }
            if (Keyright == KEY_LEFT)
            {
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
                        f_lseek(&testfile, File_CurrentOffset - 2 * JmpLenth);
                    }
                }
            }
            if (Keyright == KEY_UP)
            {
                if (Vol < 20)
                    Vol += 1;
                OLED_ShowNum(40, 2, Vol * 10, 3, 16);
            }
            if (Keyright == KEY_DOWN)
            {
                if (Vol >= 0)
                    Vol -= 1;
                OLED_ShowNum(40, 2, Vol * 10, 3, 16);
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
        if (UpdateInfo > 40000)
        {
            UpdateInfo = 0;
            OLED_ShowString(0, 2, "Vol:", 16);
            OLED_ShowNum(40, 2, Vol * 10, 3, 16);
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
        UpdateInfo++;
        UpdateKeybd++;
    }
    EA = 0;
    Play_Again_Auto = 1;
    Play_Again_Manu = 0;
    goto Again;
    while (1)
        ;
}
//在中断里喂采样点
uint16_t pcm16 = 0;
void timer0_ISR(void) interrupt 3
{

    pcm16 = 0;
    if (B_Play)
    {
        if (bflg == 0)
        {
            
            if (BytePerSample == 2)
            {
                PWMB_CCR2L = Volume[Vol][(u8)(MusicBuffer0[Cnt + 1]) + 128];
                Cnt += 2;
            }
            else if (BytePerSample == 1)
            {
                PWMB_CCR2L = Volume[Vol][(u8)(MusicBuffer0[Cnt])];
                Cnt += 1;
            }
            if (Cnt >= MusicBufferSize)
            {
                B_Play = 0;
            }
        }
        if (bflg == 1)
        {
            
            if (BytePerSample == 2)
            {
                PWMB_CCR2L = Volume[Vol][(u8)(MusicBuffer1[Cnt + 1]) +128 ];
                Cnt += 2;
            }
            else if (BytePerSample == 1)
            {
                PWMB_CCR2L = Volume[Vol][(u8)(MusicBuffer1[Cnt])];
                Cnt += 1;
            }
            if (Cnt >= MusicBufferSize)
            {
                B_Play = 0;
            }
        }
    }
}
//final
/*
timer0_ISR:
	PUSH 	ACC
	PUSH 	DPH
	PUSH 	DPL
	PUSH 	PSW
	MOV  	PSW,#00H
	PUSH 	AR4
	PUSH 	AR5
	PUSH 	AR6
	PUSH 	AR7
	USING	0
			; SOURCE LINE # 775
; {
; 
;     pcm16 = 0;
			; SOURCE LINE # 778
	MOV  	DPTR,#pcm16
	CLR  	A
;     if (B_Play)
			; SOURCE LINE # 779
	LCALL	L?0273
	MOVX 	A,@DPTR
	JNZ  	$ + 5H
	LJMP 	?C0186
;     {
			; SOURCE LINE # 780
;         if (bflg == 0)
			; SOURCE LINE # 781
	MOV  	DPTR,#bflg
	MOVX 	A,@DPTR
	JNZ  	?C0176
;         {
			; SOURCE LINE # 782
;             
;             if (BytePerSample == 2)
			; SOURCE LINE # 784
	LCALL	L?0289
	JNZ  	?C0177
;             {
			; SOURCE LINE # 785
;                 PWMB_CCR2L = Volume[Vol][(u8)(MusicBuffer0[Cnt + 1]) + 128];
			; SOURCE LINE # 786
	LCALL	L?0257
	LCALL	L?0290
	PUSH 	DPH
	PUSH 	DPL
	LCALL	L?0260
	ADD  	A,#LOW (MusicBuffer0+01H)
	MOV  	DPL,A
	MOV  	A,#HIGH (MusicBuffer0+01H)
	ADDC 	A,R6
	MOV  	DPH,A
	MOVX 	A,@DPTR
	POP  	DPL
	POP  	DPH
;                 Cnt += 2;
			; SOURCE LINE # 787
	LCALL	L?0227
	LCALL	L?0285
;                 
;             }
			; SOURCE LINE # 789
	SJMP 	?C0178
?C0177:
;             else if (BytePerSample == 1)
			; SOURCE LINE # 790
	MOV  	A,R5
	XRL  	A,#01H
	ORL  	A,R4
	JNZ  	?C0178
;             {
			; SOURCE LINE # 791
;                 PWMB_CCR2L = Volume[Vol][(u8)(MusicBuffer0[Cnt])];
			; SOURCE LINE # 792
	LCALL	L?0257
	LCALL	L?0291
	PUSH 	DPH
	PUSH 	DPL
	LCALL	L?0260
	ADD  	A,#LOW (MusicBuffer0)
	MOV  	DPL,A
	MOV  	A,#HIGH (MusicBuffer0)
	ADDC 	A,R6
	MOV  	DPH,A
	MOVX 	A,@DPTR
	POP  	DPL
	POP  	DPH
;                 Cnt += 1;
			; SOURCE LINE # 793
	LCALL	L?0227
	INC  	A
	MOVX 	@DPTR,A
	JNZ  	?C0199
	MOV  	DPTR,#Cnt
	MOVX 	A,@DPTR
	INC  	A
	MOVX 	@DPTR,A
?C0199:
;             }
			; SOURCE LINE # 794
?C0178:
;             if (Cnt >= MusicBufferSize)
			; SOURCE LINE # 795
	CLR  	C
	MOV  	DPTR,#Cnt
	MOVX 	A,@DPTR
	SUBB 	A,#02H
	JC   	?C0176
;             {
			; SOURCE LINE # 796
;                 B_Play = 0;
			; SOURCE LINE # 797
	MOV  	DPTR,#B_Play
	CLR  	A
	MOVX 	@DPTR,A
;             }
			; SOURCE LINE # 798
;         }
			; SOURCE LINE # 799
?C0176:
;         if (bflg == 1)
			; SOURCE LINE # 800
	MOV  	DPTR,#bflg
	MOVX 	A,@DPTR
	XRL  	A,#01H
	JNZ  	?C0186
;         {
			; SOURCE LINE # 801
;             
;             if (BytePerSample == 2)
			; SOURCE LINE # 803
	LCALL	L?0289
	JNZ  	?C0182
;             {
			; SOURCE LINE # 804
;                 PWMB_CCR2L = Volume[Vol][(u8)(MusicBuffer1[Cnt + 1]) +128 ];
			; SOURCE LINE # 805
	LCALL	L?0258
	LCALL	L?0290
	PUSH 	DPH
	PUSH 	DPL
	LCALL	L?0261
	ADD  	A,#LOW (MusicBuffer1+01H)
	MOV  	DPL,A
	MOV  	A,#HIGH (MusicBuffer1+01H)
	ADDC 	A,R6
	MOV  	DPH,A
	MOVX 	A,@DPTR
	POP  	DPL
	POP  	DPH
;                 Cnt += 2;
			; SOURCE LINE # 806
	LCALL	L?0228
	LCALL	L?0285
;             }
			; SOURCE LINE # 807
	SJMP 	?C0183
?C0182:
;             else if (BytePerSample == 1)
			; SOURCE LINE # 808
	MOV  	A,R5
	XRL  	A,#01H
	ORL  	A,R4
	JNZ  	?C0183
;             {
			; SOURCE LINE # 809
;                 PWMB_CCR2L = Volume[Vol][(u8)(MusicBuffer1[Cnt])];
			; SOURCE LINE # 810
	LCALL	L?0258
	LCALL	L?0291
	PUSH 	DPH
	PUSH 	DPL
	LCALL	L?0261
	ADD  	A,#LOW (MusicBuffer1)
	MOV  	DPL,A
	MOV  	A,#HIGH (MusicBuffer1)
	ADDC 	A,R6
	MOV  	DPH,A
	MOVX 	A,@DPTR
	POP  	DPL
	POP  	DPH
;                 Cnt += 1;
			; SOURCE LINE # 811
	LCALL	L?0228
	INC  	A
	MOVX 	@DPTR,A
	JNZ  	?C0200
	MOV  	DPTR,#Cnt
	MOVX 	A,@DPTR
	INC  	A
	MOVX 	@DPTR,A
?C0200:
;             }
			; SOURCE LINE # 812
?C0183:
;             if (Cnt >= MusicBufferSize)
			; SOURCE LINE # 813
	CLR  	C
	MOV  	DPTR,#Cnt
	MOVX 	A,@DPTR
	SUBB 	A,#02H
	JC   	?C0186
;             {
			; SOURCE LINE # 814
;                 B_Play = 0;
			; SOURCE LINE # 815
	MOV  	DPTR,#B_Play
	CLR  	A
	MOVX 	@DPTR,A
;             }
			; SOURCE LINE # 816
;         }
			; SOURCE LINE # 817
;     }
			; SOURCE LINE # 818
; }
			; SOURCE LINE # 819
?C0186:
	POP  	AR7
	POP  	AR6
	POP  	AR5
	POP  	AR4
	POP  	PSW
	POP  	DPL
	POP  	DPH
	POP  	ACC
	RETI 	
; END OF timer0_ISR

	END

*/
