#define 	MAIN_Fosc		40000000L	//定义主时钟
#include	"_STC8X_.h"
#include    "TYPE.h"
#include    "oled.h"
#include    "FAT12_2v1.h"
#include    "Key.h"
#include    <stdlib.h>
sbit Audio=P2^1;
sbit Wifi=P2^0;
sbit Laser=P1^3;
sbit light=P1^7;

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
Wave *Wav,Wv;//文件头,*Wav是写入用的 Wv是读取用的
uint8_t Wave_Template[]={
0x52,0x49,0x46,0x46,0xff,0xff,0xff,0xff,0x57,0x41,0x56,0x45,0x66,0x6D,0x74,0x20,
0x10,0x00,0x00,0x00,0x01,0x00,0x01,0x00,0x11,0x2B,0x00,0x00,0x11,0x2B,0x00,0x00,
0x01,0x00,0x08,0x00,0x64,0x61,0x74,0x61,0xff,0xff,0xff,0xff
};
/*************	函数	**************/
void delayms(uint t);
void	PWM_config(void);
uint8_t	Timer0_Config(u16 reload);
/******************** 主函数**************************/

uint16_t Volume[]={4096,48,44,40,36,32,28,24,20,16,16};//音量表 0%~100%
uint32_t SampleRate[]={4096,8192,11025,22100,30000};//采样率
uint8_t Vol=0,Sam=0;

uint8_t Change_Voice=0;//启动变声模式，萝莉音
uint16_t Play_Cnt=0; //扩音模式指针

uint32_t Stream_Lenth=0;//音频流长度
int res;
bit	B_Play,B_Record;//放音，录音标志
u32 Cnt=0;     //缓冲区内计数 

u32 Voice_Change;//变声标志 
uint8_t Mode=0,File=0;//0:录音  1:放音
u8 bflg=0;      //正在使用的是哪个缓冲区 0号/1号

#define MusicBufferSize 1024
uint8_t MusicBuffer0[MusicBufferSize+3];
uint8_t MusicBuffer1[MusicBufferSize+3];
FAT12_File f;//文件
uint8_t keynum;
int q[100],ptr=0;
uint8_t NameX[][12]={
{"            "},
{"RECORD1 .WAV"},
{"RECORD2 .WAV"},
{"RECORD3 .WAV"},
{"RECORD4 .WAV"},
};
uint16_t i=0,j=0,k=0;
uint32_t MakeBytes(uint8_t dat[],uint8_t lenth,uint8_t op){
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
void Mode_Select(){
    while(1){
        OLED_Printf(0,0,16,"Mode Select:1~3");
        keynum=Key_Scan()&0x00ff;
        if(keynum>=0x31&&keynum<=0x33){
            Mode=keynum-0x30;
            break;
        }   
    }
    delayms(1000);
    if(Mode==1||Mode==2){
        OLED_Clear();
        while(1){
            OLED_Printf(0,0,16,"File Select: 1~4");
            keynum=Key_Scan()&0x00ff;
            if(keynum>=0x31&&keynum<=0x34){
                File=keynum-0x30;
                break;
            }
        }    
        FAT12_Fopen(&f,NameX[File]);
        if(f.File_Id==0xffff){
            OLED_ShowString(0,2,"File Error!",16);
            while(1);
        }else{
            OLED_ShowString(0,2,"Succeed!",16);
        }
        delayms(3000);
        OLED_Clear();
    }
    OLED_Clear();
}
void Play_Mode(){
    uint32_t SPR=0,RecSize=0;
    uint16_t i=0,keynum;
    OLED_Printf(0,0,16,"Play Mode:");
    Audio=1;
    Stream_Lenth=0;
    while(1){
        OLED_Printf(0,0,16,"Play Mode:");
        OLED_Printf(0,6,16,"Again With '='!");
        FAT12_Fseek(&f,0,0);
        FAT12_Fread(&f,&Wv,sizeof(Wave));
        Timer0_Config(MAIN_Fosc/MakeBytes(Wv.SampleRate,4,1));
        EA=1;
        for(i=0;i<8;i++) OLED_ShowChar(i*8,2,f.File_Name[i],16);
        OLED_ShowChar(8*8,2,'.',16);
        for(i=9;i<12;i++) OLED_ShowChar(i*8,2,f.File_Name[i],16);
        Stream_Lenth=0;
        RecSize=MakeBytes(Wv.ChunkSize,4,1)+8;
        while(1){
            keynum=Key_Scan()&0x00ff;
            if(!B_Play&&bflg==0){
                Cnt=0;
                B_Play=1;
                bflg=1;
                FAT12_Fread(&f,MusicBuffer0,MusicBufferSize);
            }
            if(!B_Play&&bflg==1){
                Cnt=0;
                B_Play=1;
                bflg=0;
                FAT12_Fread(&f,MusicBuffer1,MusicBufferSize);
            } 
            for(i=0;i<16*(float)(Stream_Lenth)/(float)(RecSize);i++) OLED_ShowChar(8*i,4,'#',16);
            if(FAT12_IsEOF(&f)||Stream_Lenth>RecSize||keynum==0x3d) break; 
        }
        EA=0;
        while(Key_Scan()!=0xffff);
        OLED_Clear();
    }
}
void Record_Mode(){
    uint32_t tmp=0;
    uint8_t keynum;
    OLED_Printf(0,0,16,"Record Mode:");
    Stream_Lenth=0;
    Sam=1;
    while(1){
        OLED_Printf(0,2,16,"SPR:%ld Hz",SampleRate[Sam]);
        OLED_Printf(0,4,16,"Start With '='!");
        keynum=Key_Scan()&0x00ff;
        if(keynum==0x44&&Sam<2) {Sam++;}
        if(keynum==0x49&&Sam>0) {Sam--;}
        if(keynum==0x3d) break;   
        delayms(1000);
    }
    delayms(1000);
    OLED_Printf(0,2,16,"                ");
    OLED_Printf(0,4,16,"                ");
    Timer0_Config(MAIN_Fosc/SampleRate[Sam]);
    FAT12_Fseek(&f,sizeof(Wave),0);
    EA=1;
    while(1){
        OLED_Printf(0,2,16,"Bytes: %ld",Stream_Lenth);  
        for(i=0;i<16*(float)(Stream_Lenth)/(float)(f.File_Size);i++) OLED_ShowChar(8*i,4,'#',16);
        EA=0;
        keynum=Key_Scan()&0x00ff;//防止重复调用ADC函数
        EA=1;
        if(!B_Record&&bflg==0){
            Cnt=0;
            B_Record=1;
            bflg=1;
            FAT12_Modify(&f,MusicBuffer0,MusicBufferSize);
        }
        if(!B_Record&&bflg==1){
            Cnt=0;
            B_Record=1;
            bflg=0;
            FAT12_Modify(&f,MusicBuffer1,MusicBufferSize);
        }             
        if(FAT12_IsEOF(&f)||keynum==0x3d){
            EA=0;
            tmp=Stream_Lenth+36;
            (Wave*)(Wav)=Wave_Template;
            Wav->ChunkSize[0]=tmp>>0;
            Wav->ChunkSize[1]=tmp>>8;
            Wav->ChunkSize[2]=tmp>>16;
            Wav->ChunkSize[3]=tmp>>24;
            Wav->Subchunk2Size[0]=Stream_Lenth>>0;
            Wav->Subchunk2Size[1]=Stream_Lenth>>8;
            Wav->Subchunk2Size[2]=Stream_Lenth>>16;
            Wav->Subchunk2Size[3]=Stream_Lenth>>24;   
            tmp=SampleRate[Sam];
            Wav->SampleRate[0]=tmp>>0;Wav->ByteRate[0]=tmp>>0;
            Wav->SampleRate[1]=tmp>>8;Wav->ByteRate[1]=tmp>>8;
            Wav->SampleRate[2]=tmp>>16;Wav->ByteRate[2]=tmp>>16;
            Wav->SampleRate[3]=tmp>>24;Wav->ByteRate[3]=tmp>>24;
            FAT12_Fseek(&f,0,0);
            FAT12_Modify(&f,Wav,sizeof(Wave));
            OLED_Printf(0,6,16,"OK!");
            while(1);
        }
    }
}
void Speaker_Mode(){
    OLED_Printf(0,0,16,"Speaker Mode:");
    Audio=1;
    EA=1;
    while(1){
        OLED_Printf(0,2,16,"VOL:%bd %%",Vol);
        OLED_Printf(0,4,16,"SPR:%ld Hz",SampleRate[Sam]);
        if(Voice_Change) OLED_Printf(0,6,16,"Mode:Loli V",SampleRate[Sam]);
        else OLED_Printf(0,6,16,"Mode:Normal",SampleRate[Sam]);
        EA=0;
        keynum=Key_Scan()&0x00ff;//防止重复调用ADC函数
        EA=1;
        if(keynum==0x44&&Vol<10) {Vol++;}
        if(keynum==0x49&&Vol>0) {Vol--;}
        if(keynum==0x4a&&Sam<4) {Sam++;Timer0_Config(MAIN_Fosc/SampleRate[Sam]);}
        if(keynum==0x48&&Sam>0) {Sam--;Timer0_Config(MAIN_Fosc/SampleRate[Sam]);}
        if(keynum==0x3d) Voice_Change=!Voice_Change;
        delayms(8192000/SampleRate[Sam]);
    }
}
void Disk_Select(){
    uchar cap=0xff,disk0=0xff,disk1=0xff;
    uchar key=0,disk=0;
    OLED_Clear();
    W25QXX_SelectChip(0);
    cap=W25QXX_GetCapacity();
    if(cap==8||cap==4||cap==2||cap==1) disk0=cap;
    W25QXX_SelectChip(1);
    cap=W25QXX_GetCapacity();
    if(cap==8||cap==4||cap==2||cap==1) disk1=cap;
    if(disk0==0xff&&disk1==0xff){
        OLED_Printf(0,0,16,"Disk Error!");
        while(1);
    }
    if(disk0!=0xff&&disk1==0xff){
        W25QXX_SelectChip(0);
        return;
    }
    if(disk0==0xff&&disk1!=0xff){
        W25QXX_SelectChip(1);
        return;
    }
    OLED_Printf(0,0,16,"Select Disk:");
    while(1){
        key=Key_Scan()&0x00ff;
        if(!disk) {OLED_Printf(0,2,16,">");OLED_Printf(0,4,16," ");}
        if(disk) {OLED_Printf(0,2,16," ");OLED_Printf(0,4,16,">");}
        OLED_Printf(8,2,16,"Disk0: Cadio%bdM",disk0);
        OLED_Printf(8,4,16,"Disk1: User_%bdM",disk1);
        if(key==0x44||key==0x49) disk=!disk;
        if(key==0x3d) break;
        delayms(1000); 
    }
    W25QXX_SelectChip(disk);
    OLED_Clear();
    return;
}
void main(void)
{
    u8 tmp=0;
    P1M0=0x90;
    P1M1=0x1f;
    P3M0=0x00;
    P3M1=0x00;
    P2M0=0xff;
    P2M1=0x00;
    P5M0=0xff;
    P5M1=0x00;
    Audio=0;
    Wifi=0;
    Laser=0;
    light=0;    
    ADC_Init();
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear();
    HardWareSPI_Init(SPISPEEDHH);
    Disk_Select();
    FAT12_Init();  
    PWM_config();
    Timer0_Config(MAIN_Fosc/8192);

    Mode_Select();   
    B_Play=1;
    B_Record=1;
    Voice_Change=0;
    Sam=1;
    if(Mode==1) Play_Mode();
    if(Mode==2) Record_Mode();
    if(Mode==3) Speaker_Mode();
     
}
void PWM_config(void)
{
	P_SW2 |= 0x80;		//SFR enable   
	PWMB_PSCR = 0x0000;	// 预分频寄存器, 分频 Fck_cnt = Fck_psc/(PSCR[15:0}+1), 边沿对齐PWM频率 = SYSclk/((PSCR+1)*(AAR+1)), 中央对齐PWM频率 = SYSclk/((PSCR+1)*(AAR+1)*2).
	PWMB_DTR  = 00;		// 死区时间配置,   n=0~127: DTR= n T,   0x80 ~(0x80+n), n=0~63: DTR=(64+n)*2T,  
						// 对PWMB没有意义  0xc0 ~(0xc0+n), n=0~31: DTR=(32+n)*8T,   0xE0 ~(0xE0+n), n=0~31: DTR=(32+n)*16T,
	PWMB_ARR = 256;		// 自动重装载寄存器, 控制PWM周期

	PWMB_CCMR2 = 0x68;	// 通道模式配置, PWM模式1, 预装载允许
	PWMB_CCR2L = 0;	
	PWMB_CCER1 = 0x10;	// 配置通道输出使能和极性 PWM6 PWM5
    
	PWMB_BKR = 0x80;	// 主输出使能 相当于总开关
	PWMB_IER = 0x00;	// 使能中断
	PWMB_CR1 = 0x81;	// 使能计数器, 允许自动重装载寄存器缓冲, 边沿对齐模式, 向上计数, bit7=1:写自动重装载寄存器缓冲(本周期不会被打扰), =0:直接写自动重装载寄存器
	

	PWMB_ENO  = 0x04;	//IO输出PWM允许, bit6: ENO8P,  bit4: ENO7P,  bit2: ENO6P,  bit0: ENO5P
	PWMB_PS   = 0x04;	//选择IO, 4项从高到低(从左到右)对应PWM8 PWM7 PWM6 PWM5
    PWMB_CCR2L = 0;   
}
void delayms(uint t){
    uint i;
    while(t--) for(i=0;i<600;i++);                                                            
}
//========================================================================
// 函数:uint8_t	Timer0_Config(uint8_t t, u32 reload)
// 描述: timer0初始化函数.
// 参数: reload: 重装值.
// 返回: 0: 初始化正确, 1: 重装值过大, 初始化错误.
// 版本: V1.0, 2018-12-20
//========================================================================
uint8_t	Timer0_Config(u16 reload)	//reload是中断频率/Hz
{
	TR0 = 0;	//停止计数
    AUXR |= 0x80; //1T Mode
    reload=0xffff-reload;
	TH0 = (uint8_t)(reload >> 8);
	TL0 = (uint8_t)(reload);
	ET0 = 1;	//允许中断
	TMOD = (TMOD & ~0x00) | 0;	//工作模式, 0: 16位自动重装, 1: 16位定时/计数, 2: 8位自动重装, 3: 16位自动重装, 不可屏蔽中断
	TR0 = 1;	//开始运行
	return 0;
}
//========================================================================
// 函数: void timer0_int (void) interrupt TIMER0_VECTOR
// 描述:  timer0中断函数.
// 参数: none.
// 返回: none.
// 版本: V1.0, 2018-12-20
//========================================================================

void timer0_ISR (void) interrupt TIMER0_VECTOR
{   
    if(Mode==1){
        if(B_Play)
        {
            if(bflg==0){  
                //8位无符号wav数据送PWM占空比
                 PWMB_CCR2L = (u8)(MusicBuffer0[Cnt]);
                 Cnt+=1;           
                if(Cnt >=MusicBufferSize){
                    B_Play=0;
                }
            }
            if(bflg==1){                                                          
                //8位无符号wav数据送PWM占空比
                PWMB_CCR2L = (u8)(MusicBuffer1[Cnt]);
                Cnt+=1;  
                if(Cnt >=MusicBufferSize){
                    B_Play=0;
                }
            }    
        }
        Stream_Lenth++;
    }else if(Mode==2){
        if(B_Record)
        {
            if(bflg==0){  
                //8位无符号wav数据送PWM占空比
                MusicBuffer0[Cnt++]=ADC_Result(ADC_CH4,ADC_SPEEDHH)/16;      
                if(Cnt >=MusicBufferSize){
                    B_Record=0;
                }
            }
            if(bflg==1){                                                          
                //8位无符号wav数据送PWM占空比
                MusicBuffer1[Cnt++]=ADC_Result(ADC_CH4,ADC_SPEEDHH)/16;
                if(Cnt >=MusicBufferSize){
                    B_Record=0;
                }
            }    
        }       
        Stream_Lenth++;
    }else if(Mode==3){
        if(1)
        {
            if(bflg==0){  
                //8位无符号wav数据送PWM占空比
                //MusicBuffer0[Cnt++]=T_Alaw_encode[ADC_Result(ADC_CH4,ADC_SPEEDHH)/2];  
                MusicBuffer0[Cnt++]=ADC_Result(ADC_CH4,ADC_SPEEDHH)/Volume[Vol];
                if(Voice_Change==0){
                    PWMB_CCR2L=MusicBuffer1[Play_Cnt];  
                    Play_Cnt+=1;
                }else{
                    PWMB_CCR2L=MusicBuffer1[Play_Cnt];  
                    Play_Cnt+=2;                    
                    if(Play_Cnt>MusicBufferSize/2) Play_Cnt=1;
                }
                //if(Play_Cnt>MusicBufferSize/2) Play_Cnt=1;
                //Play_Cnt++;
                //Play_Cnt%=MusicBufferSize;
                if(Cnt >=MusicBufferSize){
                    bflg=1;
                    Cnt=0;
                    Play_Cnt=0;
                }
            }
            if(bflg==1){                                                          
                //8位无符号wav数据送PWM占空比
               // MusicBuffer1[Cnt++]=T_Alaw_encode[ADC_Result(ADC_CH4,ADC_SPEEDHH)/2];
                MusicBuffer1[Cnt++]=ADC_Result(ADC_CH4,ADC_SPEEDHH)/Volume[Vol];
                if(Voice_Change==0){
                    PWMB_CCR2L=MusicBuffer0[Play_Cnt];  
                    Play_Cnt+=1;
                }else{
                    PWMB_CCR2L=MusicBuffer0[Play_Cnt];  
                    Play_Cnt+=2;                    
                    if(Play_Cnt>MusicBufferSize/2) Play_Cnt=1;
                }
                if(Cnt >=MusicBufferSize){
                    bflg=0;
                    Cnt=0;
                    Play_Cnt=0;
                }
            }    
        }    
        //PWMB_CCR2L=ADC_Result(ADC_CH4,ADC_SPEEDHH)/16;
    }
}
