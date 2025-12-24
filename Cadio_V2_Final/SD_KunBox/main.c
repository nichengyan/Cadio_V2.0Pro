#define 	MAIN_Fosc		40000000L	//������ʱ��
#include	"_STC8X_.h"
#include    "TYPE.h"
#include    "oled.h"
#include    "ff.h"
#include    "Key.h"
sbit Wifi=P2^0;
sbit Audio=P2^1;
sbit laser=P1^3;
sbit light=P1^7;
sbit flash_cs1 = P3^5;
sbit flash_cs2 = P2^3;
sbit sd_cs = P1^6;
void GPIO_Init(){        //���ų�ʼ��
    P0M0=0x00;
    P0M1=0x00;
    P1M1=0x00; 
    P1M0=0x00;   //����Ϊ���迹
    P2M0=0x00;
    P2M1=0x00;
    P3M0=0x00;
    P3M1=0x00;
    P5M0=0xff;
    P5M1=0x00;
    Wifi=0;
    laser=0;
    Audio=1;
    light=0;
    flash_cs1 = 1;
    flash_cs2 = 1;
}  
void delayms(uint t){         //�ӳ�һС��ʱ��
    uint i;
    while(t--) for(i=0;i<600;i++);                                                            
}
void Delay1000ms(void)	//@40.000MHz
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
			while (--k);
		} while (--j);
	} while (--i);
}
uint8_t __IsAscii(uint8_t c){
    return ((c>='a')&&(c<='z'))||((c>='A')&&(c<='Z'));
}
void OLED_ShowStr(uint8_t x,uint8_t y,uint8_t str[],uint8_t num){  //��ʾ�ַ���
    uint8_t i=0;
    for(i=0;i<num;i++) if(x+i*8<120&&__IsAscii(str[i])) OLED_ShowChar(x+i*8,y,str[i],16);
}    
typedef struct{
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
}Wave;
uint32_t MakeBytes(uint8_t dat[],uint8_t lenth,uint8_t op){  //���ֽڰ��ض�˳�򣨴��/С�ˣ��ϳ�����
	uint32_t res=0;
    if(op==1){ //Small Endian
        if(lenth>=1) res|=((uint32_t)(dat[0]));
        if(lenth>=2) res|=((uint32_t)(dat[1]))<<8;
        if(lenth>=3) res|=((uint32_t)(dat[2]))<<16;
        if(lenth>=4) res|=((uint32_t)(dat[3]))<<24;
    }else{      //Big Endian
        if(lenth>=1) res|=((uint32_t)(dat[3]));
        if(lenth>=2) res|=((uint32_t)(dat[2]))<<8;
        if(lenth>=3) res|=((uint32_t)(dat[1]))<<16;
        if(lenth>=4) res|=((uint32_t)(dat[0]))<<24;
    }  
    return res;      
}
void NameGen(uint8_t *str,uint8_t num){   //����ö���ļ���
    uint8_t i=0;
    for(i=0;i<12;i++) str[i]=' ';
    str[3]='.';
    str[4]='W';
    str[5]='A';
    str[6]='V'; 
    str[0]=num/100+48;
    str[1]=num/10%10+48;
    str[2]=num%10+48;
}
uint8_t KeyRemapping(uint8_t knum){ //�Ѱ���ӳ�䵽�ļ�����1~23
    if(knum==0x4b) return 1;
    if(knum==0x46) return 2;
    if(knum==0x47) return 3;
    if(knum==0x3d) return 4;
    if(knum==0x31) return 5;
    if(knum==0x32) return 6;
    if(knum==0x33) return 7;
    if(knum==0x6f) return 8;
    if(knum==0x34) return 9;
    if(knum==0x35) return 10;
    if(knum==0x36) return 11;
    if(knum==0x6a) return 12;
    if(knum==0x37) return 13;
    if(knum==0x38) return 14;
    if(knum==0x39) return 15;
    if(knum==0x6d) return 16;
    if(knum==0x55) return 17;
    if(knum==0x30) return 18;
    if(knum==0x6e) return 19;
    if(knum==0x6b) return 20;
    if(knum==0x4a) return 21;
    if(knum==0x48) return 22;
    if(knum==0x44) return 23;
    return 0xff;
}   
    
/*************	WAV���Ų��ֱ�������	**************/
bit	B_Play;  //���Ž���/ֹͣ
u32 Cnt=0;  //�������ڼ���
#define MusicBufferSize 1024
uint8_t MusicBuffer0[MusicBufferSize+3];
uint8_t MusicBuffer1[MusicBufferSize+3]; 
uint8_t bflg=0;
uint8_t BytePerSample=0;
float Vol=1;
Wave Wave_FHD;//Wav�ļ�ͷ
FATFS SD_FS;
FIL f;         //FatFs API  ������  FAT12_FILE f;
FRESULT res;   //FatFs API  ������  FAT12_FRESULT res;
uint8_t rn;       //FatFs API  ������  FAT12_UINT rn;
uint8_t Name[12];
void	PWM_config(void);
uint8_t		Timer0_Config(u16 reload);
/*************  �߼��������ֱ�������	**************/
uint32_t i=0;
uint8_t Stop=0;
uint16_t keynum=0;

/******************** ������**************************/  
void main(void)
{
    GPIO_Init();
    ADC_Init(); 
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(1);
    OLED_Clear(); 
   PWM_config();	
    Delay1000ms();
//��ʼ��PWM
    if(f_mount(&SD_FS, "1:", 1) != FR_OK){
        OLED_ShowString(0,0,"Disk Error!",16);
        while(1);
    }        
    
    //����PWM
	EA = 1;			//����ȫ���ж�
    while(1){
        //FAT12_Fclose(&f);//���Ž�����һ��Ҫ�ر��ļ����ͷ���Դ��
        f_close(&f); //FatFs API  关闭文件  FAT12_Fclose(&f);
        keynum=Key_Scan()&0x00ff; //��ȡ�Ҽ�����ֵ������ŷ�ļ��̷ֶ������������룬��߶�Ӧ���ֽڣ��ұ����ǵ��ֽڣ�
        if(keynum!=0xff){
            NameGen(Name,KeyRemapping(keynum));   //����ö����
            //FAT12_Fopen(&f,Name);                 //���ļ�
            //FAT12_Fread(&f,&Wave_FHD,sizeof(Wave_FHD));   //��ȡWav���ļ�ͷ
            //FAT12_Fseek(&f,5000,0);  //����ǰ10kBû�������Ĳ��֣�����������������
            res = f_open(&f, "1:\\024.wav", FA_READ);            //FatFs API  打开文件  FAT12_Fopen(&f,Name);
            res = f_read(&f, &Wave_FHD, sizeof(Wave_FHD), &rn); //FatFs API  读取文件  FAT12_Fread(&f,&Wave_FHD,sizeof(Wave_FHD));
            f_lseek(&f,5000);  //����ǰ10kBû�������Ĳ��֣�����������������
            BytePerSample=MakeBytes(Wave_FHD.BitPerSample,2,1)/8;
            Timer0_Config(MAIN_Fosc/MakeBytes(Wave_FHD.SampleRate,4,1));    
            OLED_ShowStr(0,0,Name,12); 
            //OLED_ShowNum(0,4,f.File_Size,6,16);   
            OLED_ShowString(60,4,"Bytes",16);             
            while (1)
            {
                //˫����Routine
                if(!B_Play&&bflg==0){
                    Cnt=0;
                    B_Play=1;
                    bflg=1;
                    res = f_read(&f,MusicBuffer0,MusicBufferSize,&rn);
                }
                if(!B_Play&&bflg==1){
                    Cnt=0;
                    B_Play=1;
                    bflg=0;
                    res = f_read(&f,MusicBuffer1,MusicBufferSize,&rn);
                }
                keynum=Key_Scan()&0x00ff;
                if(keynum!=0xff) break;//�������ֹ����
                //if(res != FR_OK || rn==0) break;
            }
        }    
    }   
}
//========================================================================
// ����: PWM_config(void)
// ����: PWM��ʼ����������P5.4���PWM��Ƶ
// ����: ��
// ����: ��
// �汾: V2.0, 2024-5-3
//========================================================================
void PWM_config(void)
{
	P_SW2 |= 0x80;		//SFR enable   
	PWMB_PSCR = 0x0000;	// Ԥ��Ƶ�Ĵ���, ��Ƶ Fck_cnt = Fck_psc/(PSCR[15:0}+1), ���ض���PWMƵ�� = SYSclk/((PSCR+1)*(AAR+1)), �������PWMƵ�� = SYSclk/((PSCR+1)*(AAR+1)*2).
	PWMB_DTR  = 00;		// ����ʱ������,   n=0~127: DTR= n T,   0x80 ~(0x80+n), n=0~63: DTR=(64+n)*2T,  
						// ��PWMBû������  0xc0 ~(0xc0+n), n=0~31: DTR=(32+n)*8T,   0xE0 ~(0xE0+n), n=0~31: DTR=(32+n)*16T,
	PWMB_ARR = 256;		// �Զ���װ�ؼĴ���, ����PWM����

	PWMB_CCMR2 = 0x68;	// ͨ��ģʽ����, PWMģʽ1, Ԥװ������
	PWMB_CCR2L = 0;	
	PWMB_CCER1 = 0x10;	// ����ͨ�����ʹ�ܺͼ��� PWM6 PWM5
    
	PWMB_BKR = 0x80;	// �����ʹ�� �൱���ܿ���
	PWMB_IER = 0x00;	// ʹ���ж�
	PWMB_CR1 = 0x81;	// ʹ�ܼ�����, �����Զ���װ�ؼĴ�������, ���ض���ģʽ, ���ϼ���, bit7=1:д�Զ���װ�ؼĴ�������(�����ڲ��ᱻ����), =0:ֱ��д�Զ���װ�ؼĴ���
	

	PWMB_ENO  = 0x04;	//IO���PWM����, bit6: ENO8P,  bit4: ENO7P,  bit2: ENO6P,  bit0: ENO5P
	PWMB_PS   = 0x04;	//ѡ��IO, 4��Ӹߵ���(������)��ӦPWM8 PWM7 PWM6 PWM5
    PWMB_CCR2L = 0;   
}


//========================================================================
// ����:uint8_t	Timer0_Config(uint8_t t, u32 reload)
// ����: timer0��ʼ������.
// ����: reload: ��װֵ.
// ����: 0: ��ʼ����ȷ, 1: ��װֵ����, ��ʼ������.
// �汾: V1.0, 2018-12-20
//========================================================================
uint8_t	Timer0_Config(u16 reload)	//reload���ж�Ƶ��/Hz
{
	TR0 = 0;	//ֹͣ����
    AUXR |= 0x80; //1T Mode
    reload=0xffff-reload;
	TH0 = (uint8_t)(reload >> 8);
	TL0 = (uint8_t)(reload);
	ET0 = 1;	//�����ж�
	TMOD = (TMOD & ~0x00) | 0;	//����ģʽ, 0: 16λ�Զ���װ, 1: 16λ��ʱ/����, 2: 8λ�Զ���װ, 3: 16λ�Զ���װ, ���������ж�
	TR0 = 1;	//��ʼ����
	return 0;
}

//========================================================================
// ����: void timer0_int (void) interrupt TIMER0_VECTOR
// ����:  timer0�жϺ���.
// ����: none.
// ����: none.
// �汾: V1.0, 2018-12-20
//========================================================================
void timer0_ISR (void) interrupt TIMER0_VECTOR
{
	if(B_Play)
	{
        if(bflg==0){  
            //8λ�޷���wav������PWMռ�ձ�
            if(BytePerSample==2){
                PWMB_CCR2L = (u8)(MusicBuffer0[Cnt+1]+128)*Vol;
                Cnt+=2;
            }else if(BytePerSample==1){
                PWMB_CCR2L = (u8)(MusicBuffer0[Cnt])*Vol;
                Cnt+=1;
            }                
            if(Cnt >=MusicBufferSize){
                B_Play=0;
            }
        }
        if(bflg==1){                                                          
            //8λ�޷���wav������PWMռ�ձ�
            if(BytePerSample==2){
                PWMB_CCR2L = (u8)(MusicBuffer1[Cnt+1]+128)*Vol;
                Cnt+=2;
            }else if(BytePerSample==1){
                PWMB_CCR2L = (u8)(MusicBuffer1[Cnt])*Vol;
                Cnt+=1;
            }   
            if(Cnt >=MusicBufferSize){
                B_Play=0;
            }
        }    
	}
}