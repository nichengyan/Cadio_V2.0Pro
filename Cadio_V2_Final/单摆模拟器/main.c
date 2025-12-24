#include  "_STC8X_.h"
#include "TYPE.h"
#include "oled.h"
#include "Key.h"
#include "Graphic.h"
#include <math.h>
#include <stdio.h>
sbit key=P3^2;
sbit Wifi=P2^0;
sbit Audio=P2^1;
sbit laser=P1^3;
sbit light=P1^7;
uint i;
//---------------------------------------------------------------
typedef struct{
	float x,y;
}point;
typedef struct{
	float px,py,vx,vy,ax,ay,m;
}stuff;
stuff s;
float t=0,dt=0.05;
float g=9.8;
float l,theta,sx,sy,k,kf;
const float pi=3.1415926;
void delayms(uint t){
    uint i=0;
    while(t--) for(i=0;i<600;i++);
}
void Int_IO(uchar line,uint32_t *res,uchar ctrl){
    uchar cbuffer[16],ptr=0,kcode=0;
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
void Float_IO(uchar line,float *res,uchar ctrl){
    uchar cbuffer[16],ptr=0,kcode=0;
    uchar cur_blink=0;
    if(ctrl==1){
        while(1){
            kcode=Key_Scan()&0x00ff;
            if(kcode==0x3d) break;
            cbuffer[ptr]=0;
            if(kcode>=0x30&&kcode<=0x39&&ptr<16) cbuffer[ptr++]=kcode-0x30+48; 
            if(kcode==0x6e&&ptr<16) cbuffer[ptr++]='.';  
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
        sscanf(cbuffer,"%f",res);
        return;
    }else{
        sprintf(cbuffer,"%f",*res);
        OLED_ShowString(0,line,cbuffer,16);        
        return;
    }
} 
void Str_Out(uchar line,uchar *str){
    OLED_ShowString(0,line,str,16);
}

void system_init() {
    P0M0=0x00;
    P0M1=0x00;
    P1M1=0xf7; 
    P1M0=0x00;  
    P2M0=0x00;
    P2M1=0x00;
    P3M0=0x00;
    P3M1=0x00;
    P4M0=0x00;
    P4M1=0x00;
    P5M0=0x00;
    P5M1=0x00;
    Wifi=0;
    Audio=0;
    laser=0;
    light=0;
    OLED_Init();
    OLED_ColorTurn(0);
    OLED_DisplayTurn(0);
    OLED_Clear();
}
// 绘图回调函数
float dis(float x1,float y1,float x2,float y2){
	return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
}
float max(float a,float b){
    return a>b?a:b;
}
float min(float a,float b){
    return a<b?a:b;
}
void physic_init(){
    Str_Out(0,"NCY~~~");
    Str_Out(2," Simple");
    Str_Out(4,"   Simuator!");
    Str_Out(6,"<^=^> -> physic!");
    delayms(1000);
    while((Key_Scan()&0x00ff)!=0x3d);
    OLED_Clear();
    while(1){
        Str_Out(0,"Enter PosX");
        Float_IO(2,&sx,1);
        Float_IO(4,&sx,0);
        delay_ms(1000);
        OLED_Clear();    
        if(sx>1||sx<-1){
            Str_Out(6,"Out of Range!");
            delay_ms(1000);
            OLED_Clear();
        }else{
            break;
        }
    }
    while(1){
        Str_Out(0,"Enter PosY");
        Float_IO(2,&sy,1);
        Float_IO(4,&sy,0);
        delay_ms(1000);
        OLED_Clear();    
        if(sy>1||sy<-1){
            Str_Out(6,"Out of Range!");
            delay_ms(1000);
            OLED_Clear();
        }else{
            break;
        }
    }
    while(1){
        Str_Out(0,"Enter Mass(kg):");
        Float_IO(2,&s.m,1);
        Float_IO(4,&s.m,0);
        delay_ms(1000);
        OLED_Clear();    
        if(s.m<=0){
            Str_Out(6,"Out of Range!");
            delay_ms(1000);
            OLED_Clear();
        }else{
            break;
        }
    }
    while(1){
        Str_Out(0,"Enter Lenth(m):");
        Float_IO(2,&l,1);
        Float_IO(4,&l,0);
        delay_ms(1000);
        OLED_Clear();    
        if(l>1.5||l<=0){
            Str_Out(6,"Out of Range!");
            delay_ms(1000);
            OLED_Clear();
        }else{
            break;
        }
    }
    while(1){
        Str_Out(0,"Enter Theta(rad)");
        Float_IO(2,&theta,1);
        Float_IO(4,&theta,0);
        delay_ms(1000);
        OLED_Clear();    
        break;
    }
    while(1){
        Str_Out(0,"Enter kf(kg/s):");
        Float_IO(2,&kf,1);
        Float_IO(4,&kf,0);
        delay_ms(1000);
        OLED_Clear();    
        if(kf<0){
            Str_Out(6,"Out of Range!");
            delay_ms(1000);
            OLED_Clear();
        }else{
            break;
        }
    }
    while(1){
        Str_Out(0,"Enter k(N/m):");
        Float_IO(2,&k,1);
        Float_IO(4,&k,0);
        delay_ms(1000);
        OLED_Clear();    
        if(k<=0){
            Str_Out(6,"Out of Range!");
            delay_ms(1000);
            OLED_Clear();
        }else{
            break;
        }
    }
     while(1){
         Str_Out(0,"Enter dt(s):");
        Float_IO(2,&dt,1);
        Float_IO(4,&dt,0);
        delay_ms(1000);
        OLED_Clear();    
        if(dt<=0){
            Str_Out(6,"Out of Range!");
            delay_ms(1000);
            OLED_Clear();
        }else{
            break;
        }
    }
    Str_Out(0,"Start Simu!");
    while((Key_Scan()&0x00ff)!=0x3d);
    
	s.px=sx+l*cos(theta);
	s.py=sy-l*sin(theta);
	//cout<<s.px<<" "<<s.py<<endl;
	s.vx=0;
	s.vy=0;
	s.ax=max(0.0,(dis(sx,sy,s.px,s.py)-l))*k*cos(theta)/s.m; 
	s.ay=-g+max(0.0,(dis(sx,sy,s.px,s.py)-l))*k*sin(theta)/s.m;	
}
void physic_display() { 
    uchar stop=0;
    uchar cbuffer[16];
	Graphic_ClearBufferBit(0);
    while(t<=200){ 
        Float_IO(0,&t,0);
        if((Key_Scan()&0x00ff)==0x3d){
            delayms(10);
            (Key_Scan()&0x00ff)==0x3d;
            stop=~stop;
        }
        if(stop==0){        
            s.ax=max(0.0,(dis(sx,sy,s.px,s.py)-l))*k*cos(theta)/s.m-kf*s.vx; 
            s.ay=-g+max(0.0,(dis(sx,sy,s.px,s.py)-l))*k*sin(theta)/s.m-kf*s.vy;
            s.vx+=s.ax*dt;
            s.vy+=s.ay*dt;
            s.px+=s.vx*dt;
            s.py+=s.vy*dt;
            theta=acos((sx-s.px)/(sqrt((s.px-sx)*(s.px-sx)+(s.py-sy)*(s.py-sy))));
            Graphic_DrawCircleFilled((s.px+1)*64,(1-s.py)*32,5,1);
            Graphic_DrawLine((sx+1)*64,(1-sy)*32,(s.px+1)*64,(1-s.py)*32,1);
            Graphic_Flush();
            while(stop==1) if(Key_Scan()&0x00ff==0x3d) stop=0;	
            sprintf(cbuffer,"%.3f",t);
            OLED_ShowString(0,0,cbuffer,16);  
            Graphic_ClearBufferBit(0); // 清除之前帧数据
            t+=dt;
        }else{
            
        }    
	}
	while(1);	
}
uint32_t num;
float f=0;
void main(){
    
    system_init();
    physic_init();
    physic_display();
    while(1);
    return;
}