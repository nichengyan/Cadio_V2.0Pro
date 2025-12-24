#include "Gui.h"
#include "Graphic_Font.h"
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "oled.h"
#define Pi 3.1415926
static uint8_t *PBuffer[Gui_PageCnt];  //对应显存池
static uint8_t Gui_IdMap[Gui_PageCnt/8];  //页面ID使用标记
static uint8_t mempool[Gui_MemPoolSize];
static uint16_t MemLeft;
/*----------------------------Initialize------------------------------*/
#if (Use_Page0_As_RootPage==0)  //别用这个了，麻烦事儿
void Gui_Init(){
    uint8_t i;
    init_mempool(&mempool,Gui_MemPoolSize);
    for(i=0;i<2;i++) Gui_IdMap[i]=0;
    Gui_IdMap[0]|=0x80;//0号页面不能用
    MemLeft=Gui_MemPoolSize;
}
#else

void Gui_Init(Page *RootPage,int sizex,int sizey){
    uint8_t i;
    if(sizey%8!=0) sizey=(sizey/8+1)*8; //自动对齐
    init_mempool(&mempool,Gui_MemPoolSize);
    for(i=0;i<2;i++) Gui_IdMap[i]=0;
    Gui_IdMap[0]|=0x80;//0号页面不能用
    if(MemLeft<sizex*sizey/8) return;
    MemLeft=Gui_MemPoolSize-sizex*sizey/8;
    PBuffer[0]=malloc(sizex*sizey/8);
    RootPage->PIdentify=0;
    RootPage->PSizex=sizex;
    RootPage->PSizey=sizey;
    RootPage->PStartx=0;
    RootPage->PStarty=0;    
    return;
}

#endif
/*--------------------------------------------------------------------*/





/*------------------------------Memory---------------------------------*/
uint8_t Gui_GetPageId(){ //0~15;  0:Unused 1:Used
    uint8_t i,j,BitCheck,Cnt=0;
    for(i=0;i<2;i++){
        BitCheck=0x80;  
        for(j=0;j<8;j++){
            if(((~Gui_IdMap[i])&BitCheck)){ //Unused    1101 1111 
                Gui_IdMap[i]|=BitCheck;
                return i*8+j;
            }
            BitCheck>>=1;
        }
    }
    return 0;  //无可用ID
}   
uint8_t  Gui_FreePageId(uint8_t n){
    uint8_t BitCheck=0x80;
    if(n==0) return 0xff;//invalid
    if((~Gui_IdMap[n/8])&(BitCheck>>(n%8))) return 0xff;
    Gui_IdMap[n/8]&=(~(BitCheck>>(n%8)));
    return 0;
}                  
uint8_t Gui_CreatePage(Page *p,int sizex,int sizey) {
    uint8_t id=0;
    id=Gui_GetPageId();
    if(sizey%8!=0) sizey=(sizey/8+1)*8; //自动对齐
        
    if(!id||MemLeft<=sizex*sizey/8) return 0;
    PBuffer[id]=malloc(sizex*sizey/8);
    p->PIdentify=id;
    p->PSizex=sizex;
    p->PSizey=sizey;
    p->PStartx=0;
    p->PStarty=0;
    //p->PDepth=0;
    MemLeft-=(sizex*sizey/8);
    return id;
}
uint8_t Gui_FreePage(Page *p){
    uint8_t flg,checkbit=0x80;
    flg=Gui_FreePageId(p->PIdentify);
    if(flg) return 0xff;
    free(PBuffer[p->PIdentify]);
    MemLeft+=(p->PSizex*p->PSizey/8);
    p->PIdentify=0;
    p->PSizex=0;
    p->PSizey=0;
    p->PStartx=0;
    p->PStarty=0;
   //p->PDepth=0;
    return 0;
}   
/*--------------------------------------------------------------------*/





/*-------------------------------Text---------------------------------*/
void Gui_ShowChar(Page *p,int sx,int sy,uchar c,uchar op,uchar font) {
    int x,y,nowx,nowy;
    if(font==1){
        for(y=0;y<16;y++){
            for(x=0;x<8;x++){
                nowx=sx+x;
                nowy=sy+y;  
                if(Asc1608[c-32][(uchar)((y)/8)*8+x]&(0x01<<(uchar)((y)%8))){ 
                    Gui_DrawPoint(p,nowx,nowy,op);
                }else{
                    Gui_DrawPoint(p,nowx,nowy,!op);
                }
            }
        }
    }else{
        for(y=0;y<8;y++){
            for(x=0;x<6;x++){
                nowx=sx+x;
                nowy=sy+y;  
                if(Asc0806[c-32][(uchar)((y)/8)*6+x]&(0x01<<(uchar)((y)%8))){ 
                    Gui_DrawPoint(p,nowx,nowy,op);
                }else{
                    Gui_DrawPoint(p,nowx,nowy,!op);
                }
            }
        }
   }
}
void Gui_ShowString(Page *p,int sx,int sy,uchar buffer[],uchar op,uchar font){
    uchar i,nowx,nowy,dx,dy;
    nowx=sx;
    nowy=sy;
    if(font==1){dx=8;dy=16;}
    else if(font==0){dx=6;dy=8;}
    else return;
    for(i=0;buffer[i]!='\0';i++){
        if(nowx>p->PSizex-dx){
            nowx=0;
            nowy+=dy;
        }
        if(buffer[i]!='\n'){
            Gui_ShowChar(p,nowx,nowy,buffer[i],op,font);
            nowx+=dx;  
        }else{
            nowx=0;
            nowy+=dy;
        }  
    }
    return;
}
void Gui_ShowString_OneLine(Page *p,int sx,int sy,uchar buffer[],uchar op,uchar font){
    uchar i,dx;
    if(font==1) dx=8;
    else if(font==0) dx=6;
    else return;
    for(i=0;buffer[i]!='\0';i++) Gui_ShowChar(p,sx+i*dx,sy,buffer[i],op,font);
    return;
}
void Gui_Printf_OneLine(Page *p,int sx,int sy,uchar op,uchar font,const char *str,...){
    uint8_t buffer[32];
    va_list args;
    va_start(args,str);
    vsprintf(buffer,str,args);
    va_end(args);
    sprintf("%s\n",buffer);
    Gui_ShowString_OneLine(p,sx,sy,buffer,op,font);
}
void Gui_Printf(Page *p,int sx,int sy,uchar op,uchar font,const char *str,...){
    uint8_t buffer[32];
    va_list args;
    va_start(args,str);
    vsprintf(buffer,str,args);
    va_end(args);
    sprintf("%s\n",buffer);
    Gui_ShowString(p,sx,sy,buffer,op,font);
}
void Gui_ShowNum_OneLine(Page *p,int sx,int sy,ulong num,uchar lenth,uchar mode,uchar op,uchar font){
    uchar i,flg=0,tmp=0,buffer[15],nowx,dx;
    if(font==0) dx=8;
    else if(font==1) dx=6;
    else return;
    nowx=sx;
    for(i=0;i<lenth;i++){    
        buffer[i]=num%10;
        num/=10;
    }
    for(i=lenth;i>0;i--){
        tmp=buffer[i-1];
        if(mode==0){
             Gui_ShowChar(p,sx+i*dx,sy,tmp+48,op,font);
             nowx+=dx;
        }else{
            if(tmp!=0&&flg==0) flg=1;
            if(flg==0&&tmp==0) continue;
            if(flg==1){
                Gui_ShowChar(p,sx+i*dx,sy,tmp+48,op,font);
                nowx+=dx;
            }
        }
    }
    return;
}
void Gui_ShowNum(Page *p,int sx,int sy,ulong num,uchar lenth,uchar mode,uchar op,uchar font){
    uchar i,flg=0,tmp=0,buffer[15],nowx,nowy,dx,dy;
    nowx=sx;
    nowy=sy;
    if(font==0){dx=8;dy=16;}
    else if(font==1){dx=6;dy=8;}
    else return;
    for(i=0;i<lenth;i++){    
        buffer[i]=num%10;
        num/=10;
    }
    for(i=lenth;i>0;i--){
        tmp=buffer[i-1];
        if(mode==0){
             Gui_ShowChar(p,nowx,nowy,tmp+48,op,font);
             nowx+=8;
             if(nowx>p->PSizex){
                 nowx=0;
                 nowy+=dy;
             }
        }else{
            if(tmp!=0&&flg==0) flg=1;
            if(flg==0&&tmp==0) continue;
            if(flg==1){
                Gui_ShowChar(p,nowx,nowy,tmp+48,op,font);
                nowx+=dx;
                if(nowx>p->PSizey){
                   nowx=0;
                   nowy+=dy;
                }
            }
        }
    }
    return;
}
/*--------------------------------------------------------------------*/





/*------------------------------Graphic--------------------------------*/
void Int16_Swap(int *a,int *b){int tmp=0;tmp=*a;*a=*b;*b=tmp;}
int Int16_Max(int a,int b){return a>=b?a:b;}
int Int16_Min(int a,int b){return a<=b?a:b;}
uint16_t Gui_GetMemLeft(){ return MemLeft;}
void Gui_DrawPoint(Page *p,int x,int y,uchar op) {
    if(x>=p->PSizex||x<0||y>=p->PSizey||y<0) return;
    if(op) PBuffer[p->PIdentify][((uchar)y/8)*p->PSizex+(uchar)x]|=(0x01<<(uchar)(y%8));
    else PBuffer[p->PIdentify][((uchar)y/8)*p->PSizex+(uchar)x]&=(~(0x01<<(uchar)(y%8)));
    return;
}
uint8_t Gui_GetPoint(Page *p,int x,int y){
    if(x>=p->PSizex||x<0||y>=p->PSizey||y<0) return 0;
    if(PBuffer[p->PIdentify][((uchar)y/8)*p->PSizex+(uchar)x]&(0x01<<(uchar)(y%8))) return 1;
    else return 0;
}    
void Gui_DrawLine(Page *p,int x_1,int y_1,int x_2,int y_2,uchar op){
    float k,i,step=0;;       
    if(x_1==x_2&&y_1!=y_2){
        if(y_1>y_2) Int16_Swap(&y_1,&y_2);
        for(i=y_1;i<=y_2;i++) Gui_DrawPoint(p,x_1,i,op);  
    }
    if(x_1!=x_2&&y_1==y_2){
        if(x_1>x_2) Int16_Swap(&x_1,&x_2);
        for(i=x_1;i<=x_2;i++) Gui_DrawPoint(p,i,y_1,op);
    }
    if(x_1!=x_2&&y_1!=y_2){
        k=(float)(y_1-y_2)/(float)(x_1-x_2);
        if(abs(k)<=1){
            if(x_1>x_2) {Int16_Swap(&x_1,&x_2);Int16_Swap(&y_1,&y_2);}
            for(i=x_1;i<=x_2;i++) Gui_DrawPoint(p,i,y_1+k*(float)(i-x_1),op);
        }else{             
            if(y_1>y_2) {Int16_Swap(&x_1,&x_2);Int16_Swap(&y_1,&y_2);}
            for(i=y_1;i<=y_2;i++) Gui_DrawPoint(p,x_1+(float)(i-y_1)/k,i,op);
        }    
    }    
}
void Gui_DrawRectangleWired(Page *p,int sx,int sy,int sizex,int sizey,uint8_t op){
    Gui_DrawLine(p,sx,sy,sx+sizex,sy,op);
    Gui_DrawLine(p,sx+sizex,sy,sx+sizex,sy+sizey,op);
    Gui_DrawLine(p,sx+sizex,sy+sizey,sx,sy+sizey,op);
    Gui_DrawLine(p,sx,sy+sizey,sx,sy,op);
}
void Gui_DrawRectangleFilled(Page *p,int sx,int sy,int sizex,int sizey,uint8_t op){
    int i,j;
    for(i=sx;i<=sx+sizex;i++){
        for(j=sy;j<=sy+sizey;j++){
            Gui_DrawPoint(p,i,j,op);
        }
    }
}
//弧度（角度）符合高中课本通用标准
//       /
//      /
//     /   <-theta 正角theta,逆时针转角度增大。
//     ----------------
void Gui_DrawArc(Page *p,int ox,int oy,float theta1,float theta2,int r,uchar op){//画弧形,弧度制，逆时针弧度递增
    float i,dtheta;
    if(theta1>=theta2) return;
    dtheta=(theta2-theta1)/(float)r*0.15;
    for(i=theta1;i<=theta2;i+=dtheta){
        Gui_DrawPoint(p,ox+r*cos(i),oy-r*sin(i),op);
    }
}

void Gui_DrawSec(Page *p,int ox,int oy,float theta1,float theta2,int r,uchar op){//画扇形
    uint i=0;
    for(i=0;i<r;i++){
        Gui_DrawArc(p,ox,oy,theta1,theta2,i,op);
    }
}

void Gui_DrawRectangleRound(Page *p,int sx,int sy,int sizex,int sizey,int r,uchar op){//画圆角矩形框
    if(r>Int16_Min(sizex,sizey)/2) return;//圆角太大了，画不了
    Gui_DrawArc(p,sx+sizex-r,sy+r,0,Pi/2,r,op);
    Gui_DrawArc(p,sx+r,sy+r,Pi/2,Pi,r,op);
    Gui_DrawArc(p,sx+r,sy+sizey-r,Pi,Pi*1.5,r,op);
    Gui_DrawArc(p,sx+sizex-r,sy+sizey-r,Pi*1.5,Pi*2,r,op);
    Gui_DrawLine(p,sx+r,sy,sx+sizex-r,sy,op);
    Gui_DrawLine(p,sx,sy+r,sx,sy+sizey-r,op);
    Gui_DrawLine(p,sx+r,sy+sizey,sx+sizex-r,sy+sizey,op);
    Gui_DrawLine(p,sx+sizex,sy+r,sx+sizex,sy+sizey-r,op);
}

void Gui_DrawCircleWired(Page *p,int x,int y,int r,uchar op){
    int i,j,dis2;
    for(i=x-r;i<=x+r;i++){
        for(j=y-r;j<=y+r;j++){
            dis2=(i-x)*(i-x)+(j-y)*(j-y);
            if(abs(dis2-r*r)<r) Gui_DrawPoint(p,i,j,op);
        }
    }    
   return;    
}
void  Gui_DrawCircleFilled(Page *p,int x,int y,int r,uchar op){
    int i,j,dis2,tmp=1;
    tmp*=r;
    tmp*=r;
    for(i=x-r;i<=x+r;i++){
        for(j=y-r;j<y+r;j++){
            dis2=(i-x)*(i-x)+(j-y)*(j-y);
            if(dis2<tmp) Gui_DrawPoint(p,i,j,op);
        }
    }    
    return;    
}

void Gui_DrawTriangleWired(Page *p,int x_1,int y_1,int x_2,int y_2,int x_3,int y_3,uchar op){
    Gui_DrawLine(p,x_1,y_1,x_2,y_2,op);
    Gui_DrawLine(p,x_2,y_2,x_3,y_3,op);
    Gui_DrawLine(p,x_1,y_1,x_3,y_3,op);
}
//填充三角形用到的中间函数

long get_area(int x_1,int y_1,int x_2,int y_2,int x_3,int y_3){//向量叉积
	return ((long)(x_3-x_2)*(long)(y_1-y_2)-(long)(x_1-x_2)*(long)(y_3-y_2)); 
}
uchar inside(int x_1,int y_1,int x_2,int y_2,int x_3,int y_3,int x_4,int y_4){//平面向量叉乘算法，判定一个点是否在三角形内。
	if(get_area(x_4,y_4,x_1,y_1,x_2,y_2)>=0&&get_area(x_4,y_4,x_2,y_2,x_3,y_3)>=0&&get_area(x_4,y_4,x_3,y_3,x_1,y_1)>=0) return 1;
    if(get_area(x_4,y_4,x_1,y_1,x_2,y_2)<=0&&get_area(x_4,y_4,x_2,y_2,x_3,y_3)<=0&&get_area(x_4,y_4,x_3,y_3,x_1,y_1)<=0) return 1;
	else return 0;
}   
void Gui_DrawTriangleFilled(Page *p,int x_1,int y_1,int x_2,int y_2,int x_3,int y_3,uchar op){
    
    int xmax,xmin,ymax,ymin,i,j;
    xmax=Int16_Max(x_1,Int16_Max(x_2,x_3)); ymax=Int16_Max(y_1,Int16_Max(y_2,y_3));
    xmin=Int16_Min(x_1,Int16_Min(x_2,x_3)); ymin=Int16_Min(y_1,Int16_Min(y_2,y_3));
    for(i=xmin;i<=xmax;i++){
        for(j=ymin;j<=ymax;j++){
            if(inside(x_1,y_1,x_2,y_2,x_3,y_3,i,j)) Gui_DrawPoint(p,i,j,op);
        }
    }
}
/*--------------------------------------------------------------------*/





/*-------------------------------BMP----------------------------------*/
void Gui_DrawBmp(Page *p,int sx,int sy,int sizex,int sizey,uchar BMP[]){
    uint x,y,nowx,nowy;
    uint8_t pixel;
    for(y=0;y<sizey;y++){
        for(x=0;x<sizex;x++){
            nowy=sy+y;
            nowx=sx+x;
            if(1){
                if(BMP[(uchar)(y/8)*sizex+x]&(0x01<<(uchar)(y%8))) pixel=0x00;
                else pixel=0xff;
                Gui_DrawPoint(p,nowx,nowy,pixel);
			}
        }
    }
    return;
}   
void Gui_DrawBmpColorRev(Page *p,int sx,int sy,int sizex,int sizey,uchar BMP[]){
    uint x,y,nowx,nowy;
    uint8_t pixel;
    for(y=0;y<sizey;y++){
        for(x=0;x<sizex;x++){
            nowy=sy+y;
            nowx=sx+x;
            if(1){
                if(BMP[(uchar)(y/8)*sizex+x]&(0x01<<(uchar)(y%8))) pixel=0xff;
                else pixel=0x00;
                Gui_DrawPoint(p,nowx,nowy,pixel);
			}
        }
    }
    return;
}   


//很耗时间，计算量较大！！！
void Gui_RotateBmp(Page *p,int ox,int oy,int sizex,int sizey,float theta,float alpha,uchar BMP[]){//把图片以(ox,oy)为中心,转theta 弧度,放缩alpha倍
	float rx,ry,_x,_y;
    float cosx,sinx;
    int x,y;
    uint8_t pixel;
    cosx=cos(theta);sinx=sin(theta);
    rx=sizex*0.5;ry=sizey*0.5;
	for(y=0;y<sizey;y++){
		for(x=0;x<sizex;x++){
				_x=((x-rx)*cosx-(y-ry)*sinx)*alpha+ox;
				_y=((x-rx)*sinx+(y-ry)*cosx)*alpha+oy;
                if(BMP[(uchar)(y/8)*sizex+x]&(0x01<<(uchar)(y%8))) pixel=0x00;
                else pixel=0xff;
                Gui_DrawPoint(p,(int)_x,(int)_y,pixel);
		}
	}	
}
/*--------------------------------------------------------------------*/




 /*---------------------------Control---------------------------------*/
void Gui_ClearBufferBit(Page *p,uchar op){
    uint i;
    for(i=0;i<p->PSizex*p->PSizey/8;i++) PBuffer[p->PIdentify][i]=op?0xff:0;
    return;
}
void Gui_Flush(Page *p){
    OLED_DrawBMP(p->PStartx,p->PStarty/8,p->PSizex,p->PSizey,PBuffer[p->PIdentify]);    
}   
void Gui_CopyPage(Page *From,Page *To,uint8_t op){ //把一个页面贴到另一个页面上  op=0:覆盖 op=1:并集 op=2:交集 op=3:异或
    int i,j;
    uint8_t pixela,pixelb;
    for(i=0;i<From->PSizex;i++){
        for(j=0;j<From->PSizey;j++){
            pixela=Gui_GetPoint(From,i,j);
            pixelb=Gui_GetPoint(To,From->PStartx+i-To->PStartx,From->PStarty+j-To->PStarty);
            if(op==0) Gui_DrawPoint(To,From->PStartx+i-To->PStartx,From->PStarty+j-To->PStarty,pixela);
            else if(op==1) Gui_DrawPoint(To,From->PStartx+i-To->PStartx,From->PStarty+j-To->PStarty,pixela|pixelb);
            else if(op==2) Gui_DrawPoint(To,From->PStartx+i-To->PStartx,From->PStarty+j-To->PStarty,pixela&pixelb);
            else if(op==3) Gui_DrawPoint(To,From->PStartx+i-To->PStartx,From->PStarty+j-To->PStarty,pixela^pixelb);
        }
    }
}
void Gui_SetPosAbs(Page *p,int sx,int sy){//设置绝对坐标
    if(p->PIdentify==0) return;
    p->PStartx=sx;
    p->PStarty=sy;
}
void Gui_SetPosRel(Page *Ref,Page *p,int sx,int sy){//设置相对某页面Ref的坐标，用于窗口绘制
    if(Ref->PIdentify==0&&(!(Use_Page0_As_RootPage))) return;
    Gui_SetPosAbs(p,+Ref->PStartx+sx,Ref->PStarty+sy);
}
/*--------------------------------------------------------------------*/





/*---------------------------Interaction------------------------------*/        
uint8_t Gui_BoxedText(Page *p,int sx,int sy,uint8_t *Str,uchar op,uchar font){
    if(font==0) {Gui_DrawRectangleWired(p,sx,sy,strlen(Str)*6+1,8+1,1);Gui_ShowString(p,sx+1,sy+1,Str,op,font);}
    if(font==1) {Gui_DrawRectangleWired(p,sx,sy,strlen(Str)*8+1,16+1,1);Gui_ShowString(p,sx+1,sy+1,Str,op,font);}
    return 0;
}
        
    
    
/*
uint8_t Gui_MessageBox(Page *p,int sx,int sy,uint8_t *Title,uint8_t *Content,uint8_t Type){
    Page pBody,pTitle,pContent;
    uint8_t keynum,selection=1;    
    int mx=0,more;
    uint8_t roll_flg=0;
    Gui_CreatePage(&pTitle,128,8);Gui_ClearBufferBit(&pTitle,0);
    Gui_CreatePage(&pBody,80,48);Gui_ClearBufferBit(&pBody,0);
    Gui_CreatePage(&pContent,60,24);Gui_ClearBufferBit(&pContent,0);
        
    if(Type==1){//Yes|No 返回：0:No/1:Yes
        more=0;
        if(strlen(Title)*6>40) {roll_flg=1;more=strlen(Title)*6-40;}  
        while(1){
            //Gui_DrawRectangleWired(&pTitle,1,1,63,8,1);
            //ClearBufferBit(        

            keynum=Key_Scan()&0x00ff;
            if(keynum==0x4a||keynum==0x48) selection=!selection;
            if(keynum==0x3d) break;
            Gui_ClearBufferBit(&pBody,0);
            Gui_ClearBufferBit(&pTitle,0);
            Gui_SetPosRel(p,&pBody,sx,sy);
            Gui_SetPosRel(&pBody,&pContent,18,12);
            Gui_ClearBufferBit(&pContent,0); 
            Gui_ShowString(&pContent,1,0,Content,1,0);           
            Gui_DrawBmp(&pBody,2,16,16,16,Icon_Information); 
            Gui_CopyPage(&pContent,&pBody,0);  
            Gui_SetPosRel(&pBody,&pTitle,mx,1);            
            Gui_Printf(&pTitle,0,0,1,0,"              ");  
            Gui_ShowString(&pTitle,1,0,"Title:",1,0);  
            Gui_ShowString(&pTitle,1,0,Title,37,0); 
            Gui_CopyPage(&pTitle,&pBody,0);              
            Gui_DrawLine(&pBody,0,10,79,10,1);
            Gui_DrawRectangleWired(&pBody,0,0,79,47,1); 
            if(selection==1){
                Gui_BoxedText(&pBody,5,36,"Yes",0,0);
                Gui_BoxedText(&pBody,54,36,"No ",1,0);
            }else if(selection==0){
                Gui_BoxedText(&pBody,5,36,"Yes",1,0);
                Gui_BoxedText(&pBody,54,36,"No ",0,0);
            }               
            
            Gui_CopyPage(&pBody,p,0);
            Gui_Flush(p);   
            //Gui_MoveTo(&pBody,30,30,1,0);
            if(roll_flg){
                mx-=3;
                if(mx<=-more){
                    mx=0;
                }
            }
        }
    }
    Gui_FreePage(&pTitle);
    Gui_FreePage(&pBody);
    Gui_FreePage(&pContent);
    return selection;
}
*/
/*--------------------------------------------------------------------*/        
                 
                
