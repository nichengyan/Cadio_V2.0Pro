#include "_STC8X_.h"
#include "TYPE.h"
#include "ADC.h"
#include "Key.h"
#include "oled.h"
#include "Graphic.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
/*-----------------Boom For Cadio-----------------*/  
//扫雷V1.0 For Cadio
//$$功能：扫雷游戏

//$$玩法：
//开机，输入地雷个数
//用上下左右键控制闪烁的光标
//按左边第一列任意键是踩雷
//按左边第二列任意键是标雷，再按一下是取消标记
//标出所有雷胜利，踩到雷失败
//玩法同 经典扫雷

//输入地雷个数之后长按"=" 5秒，则进入作弊模式（显示地雷位置）
//玩的愉快！！！


//##游戏数据解析:
//核心是两个数组 a和b（a:内置游戏信息表    b：可见游戏信息表）
//a b中的数值含义：
//a:   1~8:八连通地雷个数, 10:空,11:有地雷
//b:   1~8:八连通地雷个数, 10:空,11:无效块（八连通雷数为0的 安全块） 12:小旗标记
//
//##游戏机理解析:
//a中把地雷的位置和每个块八连通块内的地雷数（即所有游戏信息）都已处理好，但玩家无权查看
//在玩家的踩雷操作中，一点一点地把a中的游戏信息（地雷阵）拷贝到b里通过屏幕呈现给玩家
//
//对于一个踩雷操作：

//若    这个格子已经访问过  
//则    结束操作

//若    踩中的格子八连通内有地雷（a[i][j]>=8&&a[i][j]<=8）
//则    把这个数字呈现给玩家(从a拷贝到b)
// 
//若    踩中的格子八连通没有地雷
//则    向周围八连通进行   踩雷操作

//以上就是递归函数 Game_Extend的内容，也是扫雷游戏的精髓
//其实就是个dfs找连通块，经典算法

//   2024/6/10
/*---------------------------------------------------*/




/*--------------------游戏宏定义---------------------*/
#define Main_Fosc 40000000    //主频：40M 用于计时
#define Maxn 10   //游戏区域大小：10*10
/*---------------------------------------------------*/




/*--------------------游戏图形资源--------------------*/
const uint8_t GameOver[]={
/*--  调入了一幅图像：C:\Users\nist\Desktop\素材\gameover.bmp  --*/
/*--  宽度x高度=80x16  --*/
0xE0,0x10,0x08,0x08,0x08,0x38,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x80,0x00,0x00,
0x40,0xC0,0x40,0xC0,0x40,0x80,0x00,0x00,0x00,0x80,0x40,0x40,0x40,0x40,0x80,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0x10,0x08,0x08,0x08,0x10,0xE0,0x00,
0x40,0xC0,0x40,0x00,0x00,0x40,0xC0,0x40,0x00,0x80,0x40,0x40,0x40,0x40,0x80,0x00,
0x40,0x40,0xC0,0x80,0x40,0x40,0xC0,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,
0x07,0x08,0x10,0x10,0x12,0x0E,0x02,0x00,0x00,0x0C,0x12,0x11,0x11,0x1F,0x10,0x00,
0x10,0x1F,0x00,0x1F,0x00,0x1F,0x10,0x00,0x00,0x0F,0x11,0x11,0x11,0x11,0x09,0x00,
0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x07,0x08,0x10,0x10,0x10,0x08,0x07,0x00,
0x00,0x00,0x07,0x18,0x04,0x03,0x00,0x00,0x00,0x0F,0x11,0x11,0x11,0x11,0x09,0x00,
0x10,0x10,0x1F,0x10,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x19,0x18,0x00,0x00,0x00

};//Game Over


uint8_t YouWin[]={
/*--  调入了一幅图像：C:\Users\nist\Desktop\素材\Win.bmp  --*/
/*--  宽度x高度=64x16  --*/
0x08,0x38,0xC8,0x00,0xC8,0x38,0x08,0x00,0x00,0x00,0x80,0x40,0x40,0x80,0x00,0x00,
0x40,0xC0,0x00,0x00,0x00,0x40,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x08,0xF8,0x00,0xF8,0x00,0xF8,0x08,0x00,0x00,0x40,0x58,0xD8,0x00,0x00,0x00,0x00,
0x40,0xC0,0x80,0x40,0x40,0x40,0x80,0x00,0x00,0x00,0x00,0xFC,0x00,0x00,0x00,0x00,
0x00,0x00,0x10,0x1F,0x10,0x00,0x00,0x00,0x00,0x07,0x08,0x10,0x10,0x08,0x07,0x00,
0x00,0x0F,0x10,0x10,0x10,0x08,0x1F,0x10,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
0x00,0x01,0x1F,0x00,0x1F,0x01,0x00,0x00,0x00,0x10,0x10,0x1F,0x10,0x10,0x00,0x00,
0x10,0x1F,0x10,0x00,0x00,0x10,0x1F,0x10,0x00,0x00,0x00,0x19,0x18,0x00,0x00,0x00
};//You Win


const uint8_t Tile[][5][5]={
    {                             
        {0,1,1,1,0},
        {0,1,0,1,0},
        {0,1,0,1,0},
        {0,1,0,1,0},
        {0,1,1,1,0}
    },//0
    {                             
        {0,0,1,0,0},
        {0,1,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,1,1,1,0}
    },//1
    {                             
        {0,1,1,1,0},
        {0,0,0,1,0},
        {0,1,1,1,0},
        {0,1,0,0,0},
        {0,1,1,1,0}
    },//2
    {                             
        {0,1,1,1,0},
        {0,0,0,1,0},
        {0,1,1,1,0},
        {0,0,0,1,0},
        {0,1,1,1,0}
    },//3
    {                             
        {0,1,0,1,0},
        {0,1,0,1,0},
        {0,1,1,1,0},
        {0,0,0,1,0},
        {0,0,0,1,0}
    },//4
    {                             
        {0,1,1,1,0},
        {0,1,0,0,0},
        {0,1,1,1,0},
        {0,0,0,1,0},
        {0,1,1,1,0}
    },//5
    {                             
        {0,1,1,1,0},
        {0,1,0,0,0},
        {0,1,1,1,0},
        {0,1,0,1,0},
        {0,1,1,1,0}
    },//6
    {                             
        {0,1,1,1,0},
        {0,0,0,1,0},
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,1,0,0}
    },//7
    {                             
        {0,1,1,1,0},
        {0,1,0,1,0},
        {0,1,1,1,0},
        {0,1,0,1,0},
        {0,1,1,1,0}
    },//8
    {                             
        {0,1,1,1,0},
        {0,1,0,1,0},
        {0,1,1,1,0},
        {0,0,0,1,0},
        {0,1,1,1,0}
    },//9
    {                             
        {1,1,1,1,1},
        {1,1,1,1,1},
        {1,1,0,1,1},
        {1,1,1,1,1},
        {1,1,1,1,1}
    },//Block 10
    {                             
        {1,0,0,0,1},
        {0,1,0,1,0},
        {0,0,1,0,0},
        {0,1,0,1,0},
        {1,0,0,0,1}
    },//Invlid 11
    {                             
        {0,0,1,1,0},
        {0,0,1,0,1},
        {0,0,1,1,0},
        {0,0,1,0,0},
        {0,1,1,1,1}
    },//Flag 12
    {                             
        {0,0,0,1,0},
        {0,0,1,0,0},
        {0,1,1,1,0},
        {1,1,1,1,1},
        {0,1,1,1,0}
    }//Boom
}; //5*5 画面基本元素

/*----------------------------------------------------*/




/*---------------------游戏全局变量-------------------*/
uint8_t  a[Maxn+1][Maxn+1],b[Maxn+1][Maxn+1];//信息数组
//a[i][j]:  10无地雷,11有地雷
//b[i][j]:  10:Block   11:Flag  12:Invalid
uint16_t i,j,k;//临时计数变量
uint8_t Keynum=0;//键码值
uint16_t Direction=0;//光标方向控制
uint8_t ix,iy,op;//当前坐标x,y值，以及操作op
uint16_t CurBlink=0,GameTime=0; //光标闪烁频率计数器  游戏计数器
uint16_t MsCnt=0;//毫秒基准计数器
uint8_t flg;//剩余的小旗个数
uint8_t Boom_Num;//总共地雷个数
uint8_t Boom_Num_Record;//当前剩余未被标记的的地雷个数
uint8_t Cheat=0;//作弊模式   0：关 1：开
/*----------------------------------------------------*/



/*--------------------游戏绘图函数--------------------*/
void DrawTile(uint8_t px,uint8_t py,uint8_t pixel);//绘制色块
void Game_Draw(uint8_t CheatMode);//绘制游戏帧  CheatMode传入1 开启作弊模式,会显示地雷位置
void ColorRev(uint8_t px,uint8_t py);//对某一个色块反色，用于光标闪烁
/*----------------------------------------------------*/



/*--------------------游戏事件函数--------------------*/
void Game_Extend(uint8_t si,uint8_t sj) reentrant;//踩雷操作函数。  本质是DFS的八连通拓展   传入当前点xy坐标
//需要改Startup.A51以在Large模式下支持递归
//Line68改为：  XBPSTACK        EQU     1       ; set to 1 if \large \reentrant is used.
//Line71改为:   XBPSTACKTOP     EQU     0x1FFF +1   ; default 0FFFFH+1 
//不修改以上两处，程序会在本函数卡死
void Win();//赢游戏
void Loss();//输游戏
/*-----------------------------------------------------*/



/*---------------------游戏按键函数--------------------*/
void Int_IO(uchar line,uint32_t *res,uchar ctrl);//整数输入, ctrl:  1输入，0输出
uint8_t KeyProcess();//按键输入
/*-----------------------------------------------------*/




/*--------------------游戏初始化函数-------------------*/
void Sys_Init();//硬件初始化
uint8_t	Timer0_Init(u16 reload);	//Timer0初始化,reload是中断频率/Hz
void Game_Init();//游戏初始化
/*-----------------------------------------------------*/



/*---------------------游戏计时函数--------------------*/
void Timer0_Isr();   //计时器中断  
void delayms(uint t);//延迟一小段时间
/*-----------------------------------------------------*/



/*---------------------游戏主循环----------------------*/
void main(){
    Sys_Init();
    Timer0_Init(Main_Fosc/1000); //产生1000hz的中断
    Game_Init();
    Graphic_Flush();//刷屏
	Game_Draw(Cheat);
    Graphic_Flush();//刷屏
    EA=1;//开启计时器
	while(1){ 
        Keynum=KeyProcess();  
        if(Keynum>=1&&Keynum<=4) Direction=Keynum;
        else Direction=0;
        if(Keynum==5) op=1;//踩雷
        else if(Keynum==6) op=2;//标雷
        else op=0;
		if(Direction==2){iy-=1;} //U
		if(Direction==4){iy+=1;} //D
		if(Direction==1){ix+=1;} //R
		if(Direction==3){ix-=1;} //L  
		if(ix>Maxn) ix=ix-Maxn;
		if(ix<1) ix=Maxn+ix;
	    if(iy>Maxn) iy=iy-Maxn;
		if(iy<1) iy=Maxn+iy;
        Graphic_ShowString(64,0,"Flag:",1);         
        Graphic_ShowNum(112,0,flg,2,0,1);
        Graphic_ShowString(64,24,"Time:",1); 
        Graphic_ShowNum(104,24,GameTime,3,0,1); 
        
        if(Cheat) Graphic_ShowString(68,48,"M-Cheat",1);        
		if(op==1){
			if(a[ix][iy]==11){
				Game_Draw(Cheat);
				Loss();
				break;
			}else if(b[ix][iy]!=12){
				Game_Extend(ix,iy);
				Game_Draw(Cheat);
			}
		}else if(op==2){
			if(b[ix][iy]==10&&flg!=0){
				b[ix][iy]=12;
				Game_Draw(Cheat);
				flg--;
				if(a[ix][iy]==11)  Boom_Num_Record--;
			}else if(b[ix][iy]==12&&flg<Boom_Num){
				b[ix][iy]=10;
				Game_Draw(Cheat);
				flg++;
				if(a[ix][iy]==11)  Boom_Num_Record++;
			}
		}
		if( Boom_Num_Record==0){
			Win();
			break;	
		}
        Game_Draw(Cheat);
        if(CurBlink>=2){
            ColorRev(ix,iy);
            CurBlink=0;
        }
        CurBlink++;        
        Graphic_Flush();
        delayms(500);
	}
    while(1);
}

/*-----------------------------------------------------*/



/*---------------------函（屎）数（山）----------------*/
void DrawTile(uint8_t px,uint8_t py,uint8_t pixel){
    uint8_t i,j;
    if(px<1||px>10||py<1||py>10) return;
    for(i=0;i<5;i++){
       for(j=0;j<5;j++){
           Graphic_DrawPoint(6*px-4+i,6*py-4+j,Tile[pixel][j][i]);  
       }
   }
} 
void Game_Draw(uint8_t CheatMode){
	uint8_t i,j; 
	for(i=1;i<=Maxn;i++){
		for(j=1;j<=Maxn;j++){ 
			if(b[i][j]==12) DrawTile(i,j,12);//Flag
			else if(a[i][j]==11&&CheatMode==1) DrawTile(i,j,13);//Boom
			else if(b[i][j]==11) DrawTile(i,j,11);//Invalid
			else if(b[i][j]==10) DrawTile(i,j,10);//Block
			else if(b[i][j]>=1&&b[i][j]<=8) DrawTile(i,j,b[i][j]);//Number1~8	
		} 
	}
} 
void ColorRev(uint8_t px,uint8_t py){
    uint8_t i,j;
    for(i=0;i<5;i++){
       for(j=0;j<5;j++){
           Graphic_DrawPoint(6*px-4+i,6*py-4+j,!Graphic_GetPoint(6*px-4+i,6*py-4+j)); //Color Reverse
       }
    }
}    

void Game_Extend(uint8_t si,uint8_t sj) reentrant { 
    uint8_t tmpx,tmpy,k; 
	if(a[si][sj]>=1&&a[si][sj]<=8){
		b[si][sj]=a[si][sj];
		return;
	}else if(b[si][sj]==12){  //标记过的地方
		return;
	}else{
		b[si][sj]=11;
		for(k=0;k<8;k++){
			if(k==0){tmpx=si-1;tmpy=sj-1;}
			if(k==1){tmpx=si+0;tmpy=sj-1;}
			if(k==2){tmpx=si+1;tmpy=sj-1;}
			if(k==3){tmpx=si-1;tmpy=sj+0;}
		    if(k==4){tmpx=si+1;tmpy=sj+0;}
			if(k==5){tmpx=si-1;tmpy=sj+1;}
			if(k==6){tmpx=si+0;tmpy=sj+1;}
			if(k==7){tmpx=si+1;tmpy=sj+1;}
			if(tmpx>=1&&tmpx<=Maxn&&tmpy>=1&&tmpy<=Maxn&&b[tmpx][tmpy]==10){
				Game_Extend(tmpx,tmpy);
			}
		}
	}
}
void Win(){
    Graphic_DrawBmp(6,24,64,16,YouWin);
    Graphic_Flush();
}

void Loss(){
    Graphic_DrawBmp(6,24,80,16,GameOver);
    Graphic_Flush();
}

void Int_IO(uchar line,uint32_t *res,uchar ctrl){
    uchar cbuffer[16],ptr=0,kcode=0,i;
    uchar cur_blink=0;
    if(ctrl==1){
        while(1){
            kcode=Key_Scan()&0x00ff;
            if(kcode==0x3d) break;
            cbuffer[ptr]=0;
            if(kcode>=0x30&&kcode<=0x39&&ptr<16) cbuffer[ptr++]=kcode-0x30+48;
            if(kcode==0x55&&ptr>=1) ptr--; 
            for(i=0;i<ptr;i++) OLED_ShowChar(i*8,line,cbuffer[i],16);
            for(i=ptr+1;i<16;i++) OLED_ShowChar(i*8,line,' ',16);
            if(cur_blink>3) OLED_ShowChar(ptr*8,line,'_',16);
            else OLED_ShowChar(ptr*8,line,' ',16); 
            if(cur_blink>6) cur_blink=0; 
            cur_blink++;
            delayms(1000);
        }
        OLED_ShowChar(ptr*8,line,' ',16); 
        sscanf(cbuffer,"%ld",res);
        return;
    }else{
        sprintf(cbuffer,"%ld",*res);
        OLED_ShowString(0,line,cbuffer,16);
        return;
    }
}  
uint8_t KeyProcess(){
    uint16_t knum;
    knum=Key_Scan();
    if(knum==0xff4a) return 1;//R
    if(knum==0xff44) return 2;//U
    if(knum==0xff48) return 3;//L
    if(knum==0xff49) return 4;//D
    if(knum==0x41ff||knum==0x4cff||knum==0x52ff||knum==0x4fff||knum==0x45ff) return 5;//Step踩雷
    if(knum==0x58ff||knum==0x4dff||knum==0x53ff||knum==0x50ff||knum==0x56ff) return 6;//Mark标雷   
    return 0xff;//Empty
}

sbit Wifi=P2^0;
sbit Audio=P2^1;
sbit laser=P1^3;
sbit light=P1^7;
void Sys_Init(){
    P0M0=0x00;
    P0M1=0x00;
    P1M1=0xf7; 
    P1M0=0x00;   //设置为高阻抗
    P2M0=0x00;
    P2M1=0x00;
    P3M0=0x00;
    P3M1=0x00;
    P5M0=0x00;
    P5M1=0x00;
    Wifi=0;
    Audio=0;
    laser=0;
    light=0;
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear();    
    ADC_Init();
    Graphic_ClearBufferBit(0);
    EA=0;
    ix=5;
    iy=5;
}
uint8_t	Timer0_Init(u16 reload)	//reload是中断频率/Hz
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
void Game_Init(){
    uint32_t bm_num,pd=0;
	uint8_t tmpx,tmpy,i,j,k,cnt; 
    while(1){
        OLED_ShowString(0,0,"Boom Num:0~15",16);
        Int_IO(2,&bm_num,1);
        delayms(1000);  
        if(bm_num>15||bm_num==0){
            OLED_ShowString(0,2,"Out of Range!",16);
            delayms(1000);
        }else{
            break;
        }
    }
    while(Key_Scan()!=0xffff){
        pd++;
    }
    if(pd>=1500) Cheat=1;
    else Cheat=0;
    OLED_Clear();
	flg=bm_num;
	Boom_Num=bm_num;
    Boom_Num_Record=Boom_Num;
	for(i=1;i<=Maxn;i++){
		for(j=1;j<=Maxn;j++){
		    a[i][j]=10;	  
            b[i][j]=10;
        }
	} 
	for(i=1;i<=Boom_Num;i++){
		tmpx=rand()%Maxn+1;
		tmpy=rand()%Maxn+1;
		if(a[tmpx][tmpy]==11) i--;//去重复 
		a[tmpx][tmpy]=11;	
	} 
	for(i=1;i<=Maxn;i++){
		for(j=1;j<=Maxn;j++){
			if(a[i][j]!=11){ //统计每个块8连通的雷数
				cnt=0;	
				for(k=0;k<8;k++){
                    if(k==0){tmpx=i-1;tmpy=j-1;}
					if(k==1){tmpx=i+0;tmpy=j-1;}
					if(k==2){tmpx=i+1;tmpy=j-1;}
					if(k==3){tmpx=i-1;tmpy=j+0;}
					if(k==4){tmpx=i+1;tmpy=j+0;}
					if(k==5){tmpx=i-1;tmpy=j+1;}
					if(k==6){tmpx=i+0;tmpy=j+1;}
					if(k==7){tmpx=i+1;tmpy=j+1;}
                    //平移数组里有负数，容易出错！就笨方法老老实实来吧
					if(tmpx>=1&&tmpx<=Maxn&&tmpy>=1&&tmpy<=Maxn&&a[tmpx][tmpy]==11) cnt++;
				}
				a[i][j]=cnt;
			}
		}
	} 
}


void Timer0_Isr() interrupt 1{
    MsCnt++;
    if(MsCnt>=1000){
        MsCnt=0;
        GameTime++;
    }
    return;
}
void delayms(uint t){
    uint i=0;
    while(t--) for(i=0;i<600;i++);
}
/*------------------------End-------------------------*/