#define 	MAIN_Fosc		40000000L	//定义主时钟
#include	"_STC8X_.h"
#include    "TYPE.h"
#include    "oled.h"
#include    "FAT12_2.h"
#include    "Key.h"
sbit Wifi=P2^0;
sbit Audio=P2^1;
sbit laser=P1^3;
sbit light=P1^7;
void GPIO_Init(){        //引脚初始化
    P1M0=0x00;
    P1M1=0x1f;           //按键占用的脚一定要开高阻
    P3M0=0x00;
    P3M1=0x00;
    P5M0=0xff;
    P5M1=0x00;
    light=0;
    Wifi=0;
    Audio=1;
    laser=0;
}  
void delayms(uint t){         //延迟一小段时间
    uint i;
    while(t--) for(i=0;i<600;i++);                                                            
}
void OLED_ShowStr(uint8_t x,uint8_t y,uint8_t str[],uint8_t num){  //显示字符串
    uint8_t i=0;
    for(i=0;i<num;i++) if(x+i*8<120&&FAT12_IsAscii(str[i])) OLED_ShowChar(x+i*8,y,str[i],16);
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
uint32_t MakeBytes(uint8_t dat[],uint8_t lenth,uint8_t op){  //把字节按特定顺序（大端/小端）合成整数
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
void NameGen(uint8_t *str,uint8_t num){   //生成枚举文件名
    uint8_t i=0;
    for(i=0;i<12;i++) str[i]=' ';
    str[8]='.';
    str[9]='W';
    str[10]='A';
    str[11]='V'; 
    str[0]=num/100+48;
    str[1]=num/10%10+48;
    str[2]=num%10+48;
}
uint8_t KeyRemapping(uint8_t knum){ //把按键映射到文件序数1~23
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
    
/*************	WAV播放部分变量声明	**************/
bit	B_Play;  //播放进行/停止
u32 Cnt=0;  //缓冲区内计数
#define MusicBufferSize 1024
uint8_t MusicBuffer0[MusicBufferSize+3];
uint8_t MusicBuffer1[MusicBufferSize+3]; 
uint8_t bflg=0;
uint8_t BytePerSample=0;
float Vol=1;
Wave Wave_FHD;//Wav文件头
FAT12_File f;//Wav文件
uint8_t Name[12];
void	PWM_config(void);
uint8_t		Timer0_Config(u16 reload);
/*************  逻辑处理部分变量声明	**************/
uint32_t i=0;
uint8_t Stop=0;
uint16_t keynum=0;

/******************** 主函数**************************/  
void main(void)
{
    GPIO_Init();
    HardWareSPI_Init(SPISPEEDHL);//不要使用SPISPEEDHH，太快了会出错！ 
    ADC_Init(); 
    OLED_Init();
    FAT12_Init();
    //一系列初始化
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear(); 
    //启动显示
    PWM_config();	//初始化PWM
    //启动PWM
	EA = 1;			//允许全局中断
    while(1){
        FAT12_Fclose(&f);//播放结束后一定要关闭文件，释放资源！
        keynum=Key_Scan()&0x00ff; //读取右键盘数值（卡东欧的键盘分独立的左右两半，左边对应高字节，右边则是低字节）
        if(keynum!=0xff){
            NameGen(Name,KeyRemapping(keynum));   //生成枚举名
            FAT12_Fopen(&f,Name);                 //打开文件
            FAT12_Fread(&f,&Wave_FHD,sizeof(Wave_FHD));   //读取Wav的文件头
            FAT12_Fseek(&f,5000,0);  //跳过前10kB没有声音的部分，并且用于消除破音
            BytePerSample=MakeBytes(Wave_FHD.BitPerSample,2,1)/8;
            Timer0_Config(MAIN_Fosc/MakeBytes(Wave_FHD.SampleRate,4,1));    
            OLED_ShowStr(0,0,Name,12); 
            OLED_ShowNum(0,4,f.File_Size,6,16);   
            OLED_ShowString(60,4,"Bytes",16);             
            while (!FAT12_IsEOF(&f))
            {
                //双缓冲Routine
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
                keynum=Key_Scan()&0x00ff;
                if(keynum!=0xff) break;//任意键终止播放
            } 
        }    
    }   
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
}