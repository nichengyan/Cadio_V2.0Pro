#include "_STC8X_.h"
#include "TYPE.h"
#include "oled.h"
#include "ff.h"
#include "SD.h"
#include <stdlib.h>
// #include "ADC.h"
#define MAIN_Fosc 40000000L // ������ʱ��
sbit Wifi = P2 ^ 0;
sbit Audio = P2 ^ 1;
sbit laser = P1 ^ 3;
sbit light = P1 ^ 7;
sbit flash_cs1 = P3 ^ 5;
sbit flash_cs2 = P2 ^ 3;
sbit sd_cs = P1 ^ 6;
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

void Sys_Init()
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M1 = 0x00;
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
FRESULT res1, res2;
FIL testfile;
uint16_t rn = 1;
uint32_t tick = 0;
// unsigned edata bf[512];

void Timer1_Config(void) // 122΢��@40.000MHz
{
    AUXR |= 0x40; // ��ʱ��ʱ��1Tģʽ
    TMOD &= 0x0F; // ���ö�ʱ��ģʽ
    TL1 = 0xF0;   // ���ö�ʱ��ʼֵ
    TH1 = 0xEC;   // ���ö�ʱ��ʼֵ
    TF1 = 0;      // ���TF1��־
    TR1 = 1;      // ��ʱ��1��ʼ��ʱ
    ET1 = 1;      // ʹ�ܶ�ʱ��1�ж�
    // 8192 hz�ж�
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
    PWMB_CCER1 = 0x10; // ����ͨ�����ʹ�ܺͼ���?PWM6 PWM5

    PWMB_BKR = 0x80; // �����ʹ��?�൱���ܿ���
    PWMB_IER = 0x00; // ʹ���ж�
    PWMB_CR1 = 0x81; // ʹ�ܼ�����, �����Զ���װ�ؼĴ�������, ���ض���ģʽ, ���ϼ���, bit7=1:д�Զ���װ�ؼĴ�������(�����ڲ��ᱻ����), =0:ֱ��д�Զ���װ�ؼĴ���

    PWMB_ENO = 0x04; // IO���PWM����,
    PWMB_PS = 0x04;  // ѡ��IO
    // ��P5^4������?
}

#define MusicBufferSize 512
FATFS SD_FS;
FRESULT res1, res2;
FIL testfile, vid;
uint16_t Cnt = 0;
uint8_t bflg = 0, B_Play = 0;
uint8_t MusicBuffer0[MusicBufferSize + 2], MusicBuffer1[MusicBufferSize + 2];
uint8_t FrameBuffer[1024];
uint32_t _tick = 0, FrameX = 0;
uint16_t fps_tick = 0;
uint8_t fps = 0;
void main()
{
    Sys_Init();
    if (f_mount(&SD_FS, "1:", 1) != FR_OK)
    {
        OLED_ShowString(0, 0, (uint8_t *)"Disk Error!", 16);
        while (1)
            ;
    }
    res1 = f_open(&testfile, "1:\\Test\\badapple.wav", FA_READ);
    res1 = f_open(&vid, "1:\\Test\\badapple.vid", FA_READ);

    if (res1 != FR_OK)
    {
        OLED_ShowString(0, 0, (uint8_t *)"File Error!", 16);
        while (1)
            ;
    }
    EA = 1;
    Timer1_Config();
    PWM_Config();
    Audio = 1;
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

        if (FrameX > 858)
        { // ����ͬ����ȡ��
            FrameX = 0;
            res1 = f_read(&vid, FrameBuffer, 1024, &rn);
            OLED_DrawBMP(0, 0, 128, 64, FrameBuffer);
        }
    }
}
// �ö�ʱ��1�ж�
void timer0_ISR(void) interrupt 3
{

    if (B_Play)
    {
        if (bflg == 0)
        {
            ////8��?????wav??????PWM????(u8)
            // Timer4_SetDuty((MusicBuffer0[Cnt])-1);
            PWMB_CCR2L = MusicBuffer0[Cnt];
            ++Cnt;
            if (Cnt >= MusicBufferSize)
            {
                B_Play = 0;
            }
        }
        if (bflg == 1)
        {
            // 8��?????wav??????PWM????MusicBuffer1[Cnt])
            // Timer4_SetDuty((MusicBuffer1[Cnt])-1);
            PWMB_CCR2L = MusicBuffer1[Cnt];
            ++Cnt;
            if (Cnt >= MusicBufferSize)
            {
                B_Play = 0;
            }
        }
    }
    FrameX++;
    fps_tick++;
}