#include "_STC8X_.h"
#include "TYPE.h"
#include "ADC.h"
#include "Key.h"
#include "oled.h"
#include "Graphic.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

 
/*------------------Snake For Cadio-----------------*/  
//贪食蛇V1.0 For Cadio
//$$功能：贪食蛇游戏

//$$玩法：
//开机，输入游戏模式(0~2)和速度(1~4)
//用上下左右键控制蛇吃豆子来获得尽可能高的分数
//游戏中按'="是暂停

//$$游戏模式GameMode：
//0:传统模式 Tradition，蛇撞到墙壁或者咬到自己都会结束游戏
//1:无限模式 Infinte 蛇撞到墙壁会从另一面穿回来，咬到自己会结束游戏
//2:截断模式 Cutoff 蛇撞到墙壁结束游戏，咬到自己会是长度变短并相应扣分
//玩的愉快！！！

//   2024/6/10
/*---------------------------------------------------*/





/*--------------------游戏宏定义--------------------*/
//游戏区域以3*3色块为最小刷新单位

//游戏视窗大小.
#define Game_Width 30 //横向30个单位
#define Game_Height 20//纵向20个单位
#define Maxn (Game_Height*Game_Width) //单位色块的数量
/*----------------------------------------------------*/





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

};

const uint8_t Run[]={
/*--  调入了一幅图像：C:\Users\nist\Desktop\素材\run.bmp  --*/
/*--  宽度x高度=16x16  --*/
0x00,0x00,0xC0,0x20,0x10,0x08,0xE8,0x48,0x88,0x08,0x10,0x20,0xC0,0x00,0x00,0x00,
0x00,0x00,0x07,0x08,0x10,0x20,0x2F,0x24,0x22,0x21,0x10,0x08,0x07,0x00,0x00,0x00

};

const uint8_t Pause[]={
/*--  调入了一幅图像：C:\Users\nist\Desktop\素材\Pause.bmp  --*/
/*--  宽度x高度=16x16  --*/
0x00,0x00,0xC0,0x20,0x10,0x08,0xC8,0x08,0xC8,0x08,0x10,0x20,0xC0,0x00,0x00,0x00,
0x00,0x00,0x07,0x08,0x10,0x20,0x27,0x20,0x27,0x20,0x10,0x08,0x07,0x00,0x00,0x00
};
//游戏中的3*3色块
const uint8_t Tile[5][3][3]={
    {
        {1,1,1},
        {1,1,1},
        {1,1,1}
    },
    {
        {1,1,1},
        {1,0,1},
        {1,1,1}
    },
    {
        {1,0,1},
        {0,1,0},
        {1,0,1}
    },
    {
        {0,1,0},
        {1,0,1},
        {0,1,0}
    },
    {
        {0,0,0},
        {0,0,0},
        {0,0,0}
    }
}; 
/*----------------------------------------------------*/






/*-------------游戏核心数据结构：循环队列-------------*/
typedef struct{
    signed char  px,py;
}Q;//每个元素是一个点坐标
uint16_t Front=0,Rear=0;//队首队尾(Front指向队首元素的下一位，Rear指向的就是队尾元素)
Q Queue[2*Maxn];//蛇结构体
uint8_t Queue_IsEmpty();//判断空
uint8_t Queue_IsFull();//判断满
uint8_t QueueEn(Q element);//元素入队
uint8_t QueueDe();//元素出队
Q GetFront();//获取队首元素
/*----------------------------------------------------*/






/*---------------------游戏全局变量-------------------*/
Q Head,Award;//蛇头位置,奖励位置
uint16_t i,j,k;//临时计数器
uint8_t Score=0;//分数
uint8_t Direction=2,Keynum=0;//方向控制变量和按键值
uint8_t AwardFlg,PauseFlg;//奖励生成标志和暂停标志  真逻辑有效
uint32_t GameMode=1,Speed=1;//游戏模式和游戏速度    [GameMode 0~2:传统模式，无限模式，截断模式 ]   [Speed:1~4,越大越快]，
uint8_t Map[Game_Width+1][Game_Height+1];//地图结构体 0:空,1:蛇身体,2:奖励,3:障碍（此版本还未涉及）
/*----------------------------------------------------*/




/*--------------------游戏底层函数-------------------*/
void delayms(uint16_t t);
void Int_IO(uchar line,uint32_t *res,uchar ctrl);//32Bit整数输入输出.ctrl:1输入,0输出 
uint8_t KeyProcess();//按键读取  
void DrawEdge();//绘制游戏边框
void DrawTile(uint8_t px,uint8_t py,uint8_t pixel);//绘制色块
void DrawGame();//绘制游戏帧
/*----------------------------------------------------*/





/*------------------游戏逻辑和事件-------------------*/
uint8_t Is_Occupied(uint8_t px,uint8_t py);//是否被蛇身体占用
uint8_t Is_Award(uint8_t px,uint8_t py);//是否有奖励
uint8_t Is_Block(uint8_t px,uint8_t py);//是否有障碍
void SetAward();//生成奖励 
void End();//游戏结束GameOver!
/*----------------------------------------------------*/




/*---------------------初始化------------------------*/
void SysInit();
void GameInit();
 /*----------------------------------------------------*/

//主函数
void main(){
    SysInit();
    GameInit();
    while(1){
        Head=GetFront();
        Keynum=KeyProcess();
        if(Keynum==5){
            while(KeyProcess()==5);
            while(1){
                 Graphic_DrawBmp(100,48,16,16,Pause);
                 Graphic_Flush();
                 if(KeyProcess()==5){
                     while(KeyProcess()==5);
                     Graphic_DrawBmp(100,48,16,16,Run);
                     Graphic_Flush(); 
                     break;
                 }
            }
        }       
        if(Keynum>=1&&Keynum<=4&&(abs(Keynum-Direction)!=2)) Direction=Keynum;
        if(Direction==2){Head.py-=1;} //U
		if(Direction==4){Head.py+=1;} //D
		if(Direction==1){Head.px+=1;} //R
		if(Direction==3){Head.px-=1;} //L
        if(Head.px==Award.px&&Head.py==Award.py){
             AwardFlg=0;
             Map[Head.px][Head.py]=0;
             SetAward();
             Score++;
             Rear--; 
             Graphic_ShowNum(100,0,Score,3,1,1);  
        }   
				if(GameMode==0){ //传统模式
						if(Is_Occupied(Head.px,Head.py)||Head.px>Game_Width||Head.px<1||Head.py>Game_Height||Head.py<1) End();
				}else if(GameMode==1){//无限模式
					if(Head.px>Game_Width) Head.px=Head.px-Game_Width;
					if(Head.px<1) Head.px=Game_Width+Head.px;
	          		if(Head.py>Game_Height) Head.py=Head.py-Game_Height;
					if(Head.py<1) Head.py=Game_Height+Head.py;	
					if(Is_Occupied(Head.px,Head.py)) End();
				}else if(GameMode==2){//截断模式
						if(Head.px>Game_Width||Head.px<1||Head.py>Game_Height||Head.py<1) End();
				    if(Is_Occupied(Head.px,Head.py)){
		            k=Rear;
								while(k!=Front){
                    if(Queue[k].px==Head.px&&Queue[k].py==Head.py) {Rear=k;break;}//截断蛇身子
                     k=(k+1)%Maxn;
                } 
                Score=(Front+Maxn-Rear)%Rear;
                k=0; 
            }
				}
       
        QueueEn(Head);
        QueueDe();
        for(i=1;i<=30;i++){
            for(j=1;j<=20;j++){
                Map[i][j]=0;
            }
        } 
        k=Rear;    
		    while(k!=Front){       
            Map[Queue[k].px][Queue[k].py]=1;
            k=(k+1)%Maxn;
        }
        Map[Award.px][Award.py]=2;
        DrawGame();
        delayms(256-Speed*Speed*Speed*Speed);
    }
	 return;
}

/*-----------------------------------------------------------------*/





/*----------------------------函（屎）数（山）---------------------*/
void delayms(uint16_t t){
    uint16_t i;
    while(t--) for(i=0;i<600;i++);
    return;
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

void DrawEdge(){
    Graphic_DrawLine(0,0,0,64,1);
    Graphic_DrawLine(1,0,1,64,1);
    Graphic_DrawLine(92,0,92,64,1);
    Graphic_DrawLine(93,0,93,64,1);
    Graphic_DrawLine(0,0,93,0,1);
    Graphic_DrawLine(0,1,93,1,1);
    Graphic_DrawLine(0,62,93,62,1);
    Graphic_DrawLine(0,63,93,63,1);
    Graphic_Flush();
}

void DrawTile(uint8_t px,uint8_t py,uint8_t pixel){
    uint8_t i,j;
    if(px<1||px>30||py<1||py>20) return;
    for(i=0;i<3;i++){
       for(j=0;j<3;j++){
           Graphic_DrawPoint(px*3-1+i,py*3-1+j,Tile[pixel][i][j]);  
       }
   }
}    

uint8_t Queue_IsEmpty(){
    if(Front==Rear) return 1;
    else return 0;
}

uint8_t Queue_IsFull(){
    if(Rear==Front%Maxn+1) return 1;
    else return 0;
}

uint8_t QueueEn(Q element){
    if(Queue_IsFull()) return 0xff;
    Queue[Front]=element;
    Front=(Front+1)%Maxn;
    return 0;
}

uint8_t QueueDe(){
    if(Queue_IsEmpty()) return 0xff;
    Rear=(Rear+1)%Maxn;
    return 0; 
}   

Q GetFront(){
    return Queue[(Front+Maxn-1)%Maxn];
}

uint8_t Is_Occupied(uint8_t px,uint8_t py){
    if(Map[px][py]==1) return 0xff;
    else return 0;
} 

uint8_t Is_Award(uint8_t px,uint8_t py){
    if(Map[px][py]==2) return 0xff;
    else return 0;
}

uint8_t Is_Block(uint8_t px,uint8_t py){
    if(Map[px][py]==3) return 0xff;
    else return 0;
}

void SetAward(){
    uint8_t ax,ay;  
    ax=rand()%20+1;
    ay=rand()%20+1;
    while(1){
        if(Is_Award(ax,ay)||Is_Occupied(ax,ay)||Is_Block(ax,ay)){   
             ax=rand()%20+1;
             ay=rand()%20+1;
        }else{
             break;
        }
    }
    Map[ax][ay]=2;
    AwardFlg=1;
    Award.px=ax;
		Award.py=ay;
}

void End(){
    Graphic_DrawBmp(6,24,80,16,GameOver);
    Graphic_DrawBmp(100,48,16,16,Pause);
    Graphic_Flush();
    while(1);
}

uint8_t KeyProcess(){
    uint16_t knum;
    knum=Key_Scan()&0x00ff;
    if(knum==0x4a) return 1;//R
    if(knum==0x44) return 2;//U
    if(knum==0x48) return 3;//L
    if(knum==0x49) return 4;//D
    if(knum==0x3d) return 5;//Confirm
    return 0xff;//Empty
}

void DrawGame(){
    uint8_t i,j;
    for(i=1;i<=30;i++){
        for(j=1;j<=20;j++){
            if(Map[i][j]==0) DrawTile(i,j,4);
            if(Map[i][j]==1) DrawTile(i,j,0);
            if(Map[i][j]==2) DrawTile(i,j,3);
        }
    } 
    Graphic_Flush();
}  

sbit Wifi=P2^0;
sbit Audio=P2^1;
sbit laser=P1^3;
sbit light=P1^7;
void SysInit(){
    P0M0=0x00;
    P0M1=0x00;
    P1M0=0x08;   //设置为高阻抗
    P1M1=0x03; 
    P2M0=0x00;
    P2M1=0x00;
    P3M0=0x00;
    P3M1=0x00;
    P5M0=0x00;
    P5M1=0x00; 
    light=0;
    Wifi=0;
    Audio=0;
    laser=0;
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear();    
    ADC_Init();
    Graphic_ClearBufferBit(0);
}  

void GameInit(){
    Front=0;
    Rear=0;
    for(i=1;i<=30;i++){
        for(j=1;j<=20;j++){
            Map[i][j]=0;
        }
     }
    for(i=0;i<Maxn;i++){
        Queue[i].px=0;
        Queue[i].py=0;
    }
    Score=0;
    Head.px=15;
    Head.py=10;
    QueueEn(Head);
    Map[Head.px][Head.py]=1;//设置占用
    Direction=2;
    while(1){
        OLED_ShowString(0,0,"GameMode:0~2",16);
        Int_IO(2,&GameMode,1);
        delayms(1000);  
        if(GameMode>2){
            OLED_ShowString(0,2,"Out of Range!",16);
            delayms(1000);
        }else{
            break;
        }
    }
    while(1){
        OLED_ShowString(0,4,"Speed:1~4",16);
        Int_IO(6,&Speed,1);
        delayms(1000);  
        if(Speed<1||Speed>4){
            OLED_ShowString(0,6,"Out of Range!",16);
            delayms(1000);
        }else{
            break;
        }
    }
    DrawEdge();
    SetAward();
    Graphic_DrawBmp(100,48,16,16,Run);   
    DrawGame();
    while(KeyProcess()==0xff);
}      
/*------------------------End-------------------------*/