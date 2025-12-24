#define 	MAIN_Fosc		40000000L	//定义主时钟
#include	"_STC8X_.h"
#include    "TYPE.h"
#include    "oled.h"
#include    "FAT12_2.h"
#include    "Key.h"
#include    <stdlib.h>
#define KEY_UP 0x44
#define KEY_DOWN 0x49
#define KEY_LEFT  0x48
#define KEY_RIGHT 0x4a
#define KEY_CONFIRM 0x3d
sbit Wifi=P2^0;
sbit Audio=P2^1;
sbit laser=P1^3;
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
/*************	函数	**************/
void delayms(uint t);
void OLED_ShowStr(uchar x,uchar y,uchar str[],uchar num);
uint32_t MakeBytes(uint8_t dat[],uint8_t lenth,uint8_t op);
uint8_t disk_check();
void	PWM_config(void);
u8		Timer0_Config(u16 reload);	
void Disk_Select();
/*************	变量	**************/
bit	B_Play;
u32 Cnt=0;     //缓冲区内计数   
u32 SampleRate=0;  //采样率
u8 BytePerSample=0;//采样点大小
u32 ByteRate=0;//码率
u8 Channel=0;//声道数
u8 RealData=0;//实际上播放一次一个采样点字节数(Channel*BytePerSample);
u16 PlayTime=0;//歌曲时长
#define MusicBufferSize 8  //双缓冲大小
u8 MusicBuffer0[MusicBufferSize+3];  //+3是保险
u8 MusicBuffer1[MusicBufferSize+3];   //+3是保险
Wave Wave_FHD;  //Wav文件头结构
FAT12_File f;   //FAT12文件结构
u8 bflg=0;      //正在使用的是哪个缓冲区 0号/1号
u16 i=0;        //公共计数器变量

u8 Music_ID;    //WAV编号
u8 Music_Num;   //WAV总数
u16 keynum=0;    //键码
u32 JmpLenth;    //快进快退跳转单位长度
u8 KeyUp=1;      //按键松开标志
float Vol=1;     //声音系数（采样点乘以这个得到真正音量）
float Process=0;     //进度，播放完毕的百分比，取值(0~1)
u16 UpdateInfo;       //更新计数器
u16 UpdateKeybd=0;
u8 Second=0,Minute=0;    //播放时间
uint8_t retry=10;//磁盘检查重试次数

/******************** 主函数**************************/

void main(void)
{
    u8 tmp=0;
    P1M0=0x00;
    P1M1=0x1f;
    P3M0=0x00;
    P3M1=0x00;
    P2M0=0xff;
    P2M1=0x00;
    P5M0=0xff;
    P5M1=0x00;
    Audio=0;
    Wifi=0;
    laser=0;
    light=0;
    ADC_Init();
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear();
    delayms(100);
    HardWareSPI_Init(0x0c);
    delayms(100);
    Disk_Select();
    FAT12_Init();
    OLED_ShowString(0,0,"Wavs To Play:",16);
    Music_Num=FAT12_SearchForFile(1,&f,"WAV");
    Music_ID=1;
    while(1){
        if(Music_ID>Music_Num) Music_ID=1;
        if(Music_ID<1) Music_ID=Music_Num;
        FAT12_SearchForFile(Music_ID,&f,"WAV");
        if(f.File_Id!=0xffff){
            OLED_ShowNum(0,2,Music_ID,2,16);
            OLED_ShowString(16,2," ",16);
            OLED_ShowStr(24,2,f.File_Name,12);
        }
        keynum=Key_Scan()&0x00ff;
        if(keynum==KEY_UP) Music_ID++;
        if(keynum==KEY_DOWN) Music_ID--;
        if(Music_ID>Music_Num) Music_ID=1;
        if(Music_ID<1) Music_ID=Music_Num;
        if(keynum==KEY_CONFIRM){
            delayms(10);
            while(Key_Scan()==KEY_CONFIRM);
            break;
        }    
        delayms(1000);
    }     
    OLED_Clear();
    FAT12_Fopen(&f,f.File_Name);
    FAT12_Fread(&f,&Wave_FHD,sizeof(Wave_FHD));	
    
    OLED_ShowStr(0,0,f.File_Name,12);
    JmpLenth=2*((f.File_Size-sizeof(Wave_FHD))/200);   //一定要是个偶数！！！不然会出错
    SampleRate=MakeBytes(Wave_FHD.SampleRate,4,1);
    BytePerSample=MakeBytes(Wave_FHD.BitPerSample,2,1)/8;
    ByteRate=MakeBytes(Wave_FHD.ByteRate,4,1);//码率(Byte/s)
    Channel=MakeBytes(Wave_FHD.NumChannels,2,1);//声道个数（双声道了话就选择忽略其中一个声道）
    RealData=Channel*BytePerSample;
    PlayTime=(f.File_Size-sizeof(Wave_FHD))/ByteRate;//t=datasize/byterate
    if(ByteRate>44100||Channel!=1){
        OLED_Clear();
        OLED_ShowString(0,0,"Format Error!",16);
        OLED_ShowString(0,2,"Can't be Played",16);
        while(1);
    } 
    
    EA = 1;		
    PWM_config();	//初始化PWM
    Timer0_Config(MAIN_Fosc/MakeBytes(Wave_FHD.SampleRate,4,1));	//t=0: reload值是主时钟周期数,  
    //delayms(10000);
		//允许全局中断    
    delayms(1000);
    Audio=1;
    OLED_ShowNum(40,6,Minute/10,1,16);
    OLED_ShowNum(48,6,Minute%10,1,16); 
    OLED_ShowChar(56,6,':',16);
    OLED_ShowNum(64,6,Second/10,1,16);  
    OLED_ShowNum(72,6,Second%10,1,16);        
	while (!FAT12_IsEOF(&f))
	{
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
        if(UpdateKeybd>4000){ 
            UpdateKeybd=0;            
            keynum=Key_Scan()&0x00ff;

            if(keynum==KEY_RIGHT){
                if(f.File_CurrentOffset+JmpLenth<=f.File_Size){
                    FAT12_Fseek(&f,f.File_CurrentOffset+JmpLenth,0);
                }             
            }
            if(keynum==KEY_LEFT){
                if(f.File_CurrentOffset>=2*JmpLenth){
                    FAT12_Fseek(&f,f.File_CurrentOffset-4*JmpLenth,0);
                }             
            }       
            if(keynum==KEY_UP){
                if(Vol<=2) Vol+=0.1; 
                OLED_ShowNum(40,2,Vol*100,3,16);             
            }
            if(keynum==KEY_DOWN){
                if(Vol>=0) Vol-=0.1; 
                OLED_ShowNum(40,2,Vol*100,3,16); 
            }    
        }  
        if(UpdateInfo>10000){
            UpdateInfo=0; 
            OLED_ShowString(0,2,"Vol:",16);
            OLED_ShowNum(40,2,Vol*100,3,16);
            OLED_ShowString(72,2,"%",16);  
            Process=(float)(f.File_CurrentOffset-sizeof(Wave_FHD))/(float)f.File_Size;
            for(i=0;i<16;i++){
                if(i<16*Process) OLED_ShowChar(i*8,4,'#',16);
                else OLED_ShowChar(i*8,4,' ',16);
            }    
            Second=(u8)(PlayTime*Process)%60; 
            Minute=(u8)(PlayTime*Process)/60;         
            OLED_ShowNum(64,6,Second/10,1,16);  
            OLED_ShowNum(72,6,Second%10,1,16);             
            if(tmp!=Minute){
                tmp=Minute;
                OLED_ShowNum(40,6,Minute/10,1,16);
                OLED_ShowNum(48,6,Minute%10,1,16);
            }                    
        }      
	}
    EA=0;
    while(1);
    return;
}
//========================================================================
// 函数: PWM_config(void)
// 描述: PWM初始化函数，在P5.4输出PWM音频
// 参数: 无
// 返回: 无
// 版本: V2.0, 2024-5-3
//========================================================================
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
	if(B_Play)
	{
        if(bflg==0){  
            //8位无符号wav数据送PWM占空比
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
            //8位无符号wav数据送PWM占空比
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
    UpdateInfo++;  
    UpdateKeybd++;
}
void delayms(uint t){
    uint i;
    while(t--) for(i=0;i<600;i++);                                                            
}
void OLED_ShowStr(uchar x,uchar y,uchar str[],uchar num){
    uchar i=0;
    for(i=0;i<num;i++) if(x+i*8<120&&FAT12_IsAscii(str[i])) OLED_ShowChar(x+i*8,y,str[i],16);
}    
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
uint8_t disk_check(){
    uint8_t cap=0xff;
    cap=W25QXX_GetCapacity();
    if(cap==8||cap==4||cap==2||cap==1)  return 0;
    return 0xff;
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