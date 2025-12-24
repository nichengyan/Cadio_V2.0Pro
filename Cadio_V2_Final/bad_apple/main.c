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
#define MAIN_Fosc 40000000L // 定义主时钟
#include "_STC8X_.h"
#include "TYPE.h"
#include "W25QXX.h"
#include "oled.h"
#include "FAT12_2.h"
#include "Key.h"
void delayms(uint t)
{
    uint i;
    while (t--)
        for (i = 0; i < 600; i++)
            ;
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
/*************	功能说明	**************

使用STC8H系列MCU的PWM6_2(P5.4)播放音视频BadAple!!

语音参数: 11k采样 单声道
视频参数：12864二进制逐帧




                                  ____
                         --------|____|
                        /
      PWM6_2(P5.4)  | /
           -------- |
                    | \ |   <--- S8050
                      - \
                        |
                        |
                      -----
                       ---
                        -
******************************************/
u16 play_lenth;
u16 LastSample;
u16 PlayCnt;
u8 code *MusicPoint;
bit B_Play;
u8 DAC_Cnt;

// 音视频缓冲区
#define MusicBufferSize 1024
u8 MusicBuffer0[MusicBufferSize];
u8 MusicBuffer1[MusicBufferSize];
u8 FrameBuffer[1024];

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
float Vol = 1;
u32 BytePerSample = 0;
u32 Cnt = 0;
u8 bflg = 0;    // 缓冲区标记
u16 FrameX = 0; // 帧率计数器
// （每当FrameX==858时，播放一帧。帧率为10fps,可实现音画同步）
FAT12_File fa; // 音频文件
FAT12_File fv; // 视频文件
void LoadMusic(void);
void PWM_config(void);
u8 Timer0_Config(u16 reload); // reload是重装值, 返回0正确, 返回1装载值过大错误.
void Delay(u16 t);
void Disk_Select();
sbit Wifi = P2 ^ 0;
sbit Audio = P2 ^ 1;
sbit laser = P1 ^ 3;
sbit light = P1 ^ 7;
/******************** 主函数**************************/
void main(void)
{

    P1M0 = 0x00;
    P1M1 = 0x00;
    P2M0 = 0x03;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 = 0xff;
    P5M1 = 0x00;
    Wifi = 0;
    laser = 0;
    Audio = 0;
    light = 0;
    PWM_config(); // 初始化PWM
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear();
    HardWareSPI_Init(SPISPEEDHL); // 使用HSPI,时钟频率拉满 到4T
    Disk_Select();
    FAT12_Init();
    FAT12_Fopen(&fa, "BADAPPLE.WAV");
    FAT12_Fopen(&fv, "BADAPPLE.VID");
    if (fa.File_Id == 0xffff || fv.File_Id == 0xffff)
    {
        OLED_ShowString(0, 0, "File Error!", 16);
        while (1)
            ;
    }
    FAT12_Fread(&fa, &Wave_FHD, sizeof(Wave_FHD) + 1);
    Timer0_Config(MAIN_Fosc / MakeBytes(Wave_FHD.SampleRate, 4, 1));
    BytePerSample = MakeBytes(Wave_FHD.BitPerSample, 2, 1) / 8;
    OLED_ShowString(0, 0, " BADAPPLE DEMO", 16);
    OLED_ShowString(0, 2, " Using FAT12 ", 16);
    OLED_ShowString(0, 4, " @MultiFile Mode", 16);
    delayms(10000);
    EA = 1; // 允许全局中断
    Audio = 1;
    while (1)
    {
        if (!B_Play && bflg == 0)
        {
            Cnt = 0;
            B_Play = 1;
            bflg = 1;
            FAT12_Fread(&fa, MusicBuffer0, MusicBufferSize);
        }
        if (!B_Play && bflg == 1)
        {
            Cnt = 0;
            B_Play = 1;
            bflg = 0;
            FAT12_Fread(&fa, MusicBuffer1, MusicBufferSize);
        }
        // 音频双缓冲
        if (FrameX > 858)
        { // 音画同步的取值
            FrameX = 0;
            FAT12_Fread(&fv, FrameBuffer, 1024);
            OLED_DrawBMP(0, 0, 128, 64, FrameBuffer);
        }
        // 视频播放
    }
}

//========================================================================
// 函数: void	PWM_config(void)
// 描述: PWM配置函数。
// 参数: none.
// 返回: none.
// 版本: VER1.0
// 日期: 2014-8-15
// 备注:
//========================================================================
void PWM_config(void)
{
    P_SW2 |= 0x80; // SFR enable

    PWMB_PSCR = 0x0000; // 预分频寄存器, 分频 Fck_cnt = Fck_psc/(PSCR[15:0}+1), 边沿对齐PWM频率 = SYSclk/((PSCR+1)*(AAR+1)), 中央对齐PWM频率 = SYSclk/((PSCR+1)*(AAR+1)*2).
    PWMB_DTR = 00;      // 死区时间配置,   n=0~127: DTR= n T,   0x80 ~(0x80+n), n=0~63: DTR=(64+n)*2T,
                        // 对PWMB没有意义  0xc0 ~(0xc0+n), n=0~31: DTR=(32+n)*8T,   0xE0 ~(0xE0+n), n=0~31: DTR=(32+n)*16T,
    PWMB_ARR = 256;     // 自动重装载寄存器, 控制PWM周期

    PWMB_CCMR2 = 0x68; // 通道模式配置, PWM模式1, 预装载允许
    PWMB_CCR2L = 128;  // 计数器比较值, 控制占空比
    PWMB_CCER1 = 0x10; // 配置通道输出使能和极性 PWM6 PWM5

    PWMB_BKR = 0x80; // 主输出使能 相当于总开关
    PWMB_IER = 0x00; // 使能中断
    PWMB_CR1 = 0x81; // 使能计数器, 允许自动重装载寄存器缓冲, 边沿对齐模式, 向上计数, bit7=1:写自动重装载寄存器缓冲(本周期不会被打扰), =0:直接写自动重装载寄存器

    PWMB_ENO = 0x04; // IO输出PWM允许,
    PWMB_PS = 0x04;  // 选择IO
    // 在P5^4输出音频
}

//========================================================================
// 函数:u8	Timer0_Config(u8 t, u32 reload)
// 描述: timer0初始化函数.
// 参数: reload: 重装值.
// 返回: 0: 初始化正确, 1: 重装值过大, 初始化错误.
// 版本: V1.0, 2018-12-20
//========================================================================
u8 Timer0_Config(u16 reload) // reload是重装值
{
    TR0 = 0; // 停止计数

    if (reload >= (65536UL * 12))
        return 1; // 值过大, 返回错误
    if (reload < 65536UL)
        AUXR |= 0x80; // 1T mode
    else
    {
        AUXR &= ~0x80; // 12T mode
        reload = reload / 12;
    }
    reload = 65536UL - reload;
    TH0 = (u8)(reload >> 8);
    TL0 = (u8)(reload);

    ET0 = 1; // 允许中断
    //	PT0 = 1;	//高优先级中断
    TMOD = (TMOD & ~0x03) | 0; // 工作模式, 0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
    //	TMOD |=  0x04;	//对外计数或分频
    //	INT_CLKO |=  0x01;	//输出时钟
    TR0 = 1; // 开始运行
    return 0;
}

//========================================================================
// 函数: void timer0_int (void) interrupt TIMER0_VECTOR
// 描述:  timer0中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2018-12-20
//========================================================================
void timer0_ISR(void) interrupt TIMER0_VECTOR
{
    // 音频双缓冲
    if (B_Play)
    {
        if (bflg == 0)
        {
            // 8位无符号wav数据送PWM占空比
            PWMB_CCR2L = (u8)(MusicBuffer0[Cnt]);
            ++Cnt;
            if (Cnt >= MusicBufferSize)
            {
                B_Play = 0;
            }
        }
        if (bflg == 1)
        {
            // 8位无符号wav数据送PWM占空比
            PWMB_CCR2L = (u8)(MusicBuffer1[Cnt]);
            ++Cnt;
            if (Cnt >= MusicBufferSize)
            {
                B_Play = 0;
            }
        }
    }
    FrameX++;
}
void Disk_Select()
{
    uchar cap = 0xff, disk0 = 0xff, disk1 = 0xff;
    uchar key = 0, disk = 0;
    OLED_Clear();
    W25QXX_SelectChip(0);
    cap = W25QXX_GetCapacity();
    if (cap == 8 || cap == 4 || cap == 2 || cap == 1)
        disk0 = cap;
    W25QXX_SelectChip(1);
    cap = W25QXX_GetCapacity();
    if (cap == 8 || cap == 4 || cap == 2 || cap == 1)
        disk1 = cap;
    if (disk0 == 0xff && disk1 == 0xff)
    {
        OLED_Printf(0, 0, 16, "Disk Error!");
        while (1)
            ;
    }
    if (disk0 != 0xff && disk1 == 0xff)
    {
        W25QXX_SelectChip(0);
        return;
    }
    if (disk0 == 0xff && disk1 != 0xff)
    {
        W25QXX_SelectChip(1);
        return;
    }
    OLED_Printf(0, 0, 16, "Select Disk:");
    while (1)
    {
        key = Key_Scan() & 0x00ff;
        if (!disk)
        {
            OLED_Printf(0, 2, 16, ">");
            OLED_Printf(0, 4, 16, " ");
        }
        if (disk)
        {
            OLED_Printf(0, 2, 16, " ");
            OLED_Printf(0, 4, 16, ">");
        }
        OLED_Printf(8, 2, 16, "Disk0: Cadio%bdM", disk0);
        OLED_Printf(8, 4, 16, "Disk1: User_%bdM", disk1);
        if (key == 0x44 || key == 0x49)
            disk = !disk;
        if (key == 0x3d)
            break;
        delayms(1000);
    }
    W25QXX_SelectChip(disk);
    OLED_Clear();
    return;
}