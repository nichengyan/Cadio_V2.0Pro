#ifndef __Gui_h__
/*--------------------------------------------------------------------------
Gui.H

Simple Gui For OLED12864
Copyleft (c) 2024~2024 NCY
All rights reversed.

--------------------------------------------------------------------------*/
#define __Gui_h__
#include "TYPE.h"
#include "_STC8X_.h"
#include "intrins.h"
#include "oled.h"
#include "Graphic_Font.h"
#include "Key.h"
#include <math.h>

#define ScreenHeight 64  //屏幕高
#define ScreenWidth 128  //屏幕宽
#define Gui_MemPoolSize 1024+2048 //内存池大小
#define Gui_PageCnt 8 //最大页面数量，必须是8的倍数！！
#define Use_Page0_As_RootPage 0   //别动这个了


//声明：

//相对坐标&绝对坐标
//以下方便起见，用[]表示屏幕坐标系内的绝对坐标；p()表示页面p内的坐标，即相对于页面p的相对坐标
//可以看出,所有的绘图函数都是针对页面p的,因此其参数都是p页面内的坐标，即相对p的坐标坐标p（）
//OLED12864左上角是(0,0),右下角是(128,64)

//op&font:
//函数传入的op参数是颜色值，0黑 1白
//字符函数传入的font是字体，0:6*8小字  1:8*16大字
//无特殊说明了话，op和font的含义就是上面的了


typedef struct{  //页面 是一个独立的刷新区域，是SimpleGui的核心数据结构
    uint8_t PIdentify;   //Id号 从1开始。0:Invalid
    int PStartx,PStarty,PSizex,PSizey;  //页面的xy起始位置（屏幕坐标系的绝对坐标）[x,y]（默认屏幕左上角为[0,0],x轴向右，y轴向下）
    //uint8_t PDepth;//相对直接父页面的深度。最近直接父页面的深度是0
}Page;


//初始化模块
/*----------------------------Initialize------------------------------*/
#if (Use_Page0_As_RootPage==0) //这个不要改，不要使用1模式了，容易内存错误
void Gui_Init();//初始化Gui
#else
void Gui_Init(Page *RootPage,int sizex,int sizey);//初始化Gui
#endif
/*--------------------------------------------------------------------*/

//内存管理模块
/*------------------------------Memory---------------------------------*/
uint8_t  Gui_GetPageId();//为页面申请Id号
uint8_t  Gui_FreePageId(uint8_t n);//释放页面Id号
uint8_t  Gui_CreatePage(Page *p,int sizex,int sizey);//创建页面
uint8_t  Gui_FreePage(Page *p);//释放页面
uint16_t Gui_GetMemLeft();  //获取剩余内存量
/*--------------------------------------------------------------------*/



//字符模块
/*-------------------------------Text---------------------------------*/
void Gui_ShowChar(Page *p,int sx,int sy,uchar c,uchar op,uchar font) ;//在页面p上的p(sx,xy)位置显示一个字符坐标任取，允许超界，超界自动截取
//oneline是指不自动换行，超界截断的单行显示模式
//没有oneline，是指到达页面边界会自动换行的多行模式
void Gui_ShowString(Page *p,int sx,int sy,uchar buffer[],uchar op,uchar font);
void Gui_ShowString_OneLine(Page *p,int sx,int sy,uchar buffer[],uchar op,uchar font);
void Gui_Printf(Page *p,int sx,int sy,uchar op,uchar font,const char *str,...);
void Gui_Printf_OneLine(Page *p,int sx,int sy,uchar op,uchar font,const char *str,...);
void Gui_ShowNum(Page *p,int sx,int sy,ulong num,uchar lenth,uchar mode,uchar op,uchar font);
void Gui_ShowNum_OneLine(Page *p,int sx,int sy,ulong num,uchar lenth,uchar mode,uchar op,uchar font);
/*--------------------------------------------------------------------*/



//图形模块
/*------------------------------Graphic--------------------------------*/
void Gui_DrawPoint(Page *p,int x,int y,uchar op);//在页面p上p(x,y)画一个颜色是op的点(op取0/1)
uint8_t Gui_GetPoint(Page *p,int x,int y) ;//读取页面p上p(x,y)位置的点的颜色(0/1)

void Gui_DrawLine(Page *p,int x_1,int y_1,int x_2,int y_2,uchar op) ;//画任意直线过p(x1,y1)和p(x2,y2)

void Gui_DrawRectangleWired(Page *p,int sx,int sy,int sizex,int sizey,uint8_t op);//画线框矩形 左上角p(sx,sy),横向sizex，竖向sizey
void Gui_DrawRectangleFilled(Page *p,int sx,int sy,int sizex,int sizey,uint8_t op);//画实心矩形  左上角p(sx,sy),横向sizex，竖向sizey

void Gui_DrawRoundRectangleWired(Page *p,int sx,int sy,int sizex,int sizey,int r,uchar op);//画圆角矩形框   圆角矩形外接矩形框的左上角p(sx,sy),横向直线部分sizex，纵向直线部分sizey,圆弧半径r
void Gui_DrawRoundRectangleFilled(Page *p,int sx,int sy,int sizex,int sizey,int r,uchar op);//画实心圆角矩形 同理

void Gui_DrawArc(Page *p,int ox,int oy,float theta1,float theta2,int r,uchar op);//画弧形 以p(ox,oy)为圆心，以r为半径，从theta1弧度转到theta2弧度弧度，故theta2要大于theta1。（弧度增长方向的定义按照高中课本通用标准)
void Gui_DrawSec(Page *p,int ox,int oy,float theta1,float theta2,int r,uchar op);//画扇形 以p(ox,oy)为圆心，,以r为半径，从theta1弧度转到theta2弧度弧度，故theta2要大于theta1。（弧度增长方向的定义按照高中课本通用标准)

void Gui_DrawCircleWired(Page *p,int x,int y,int r,uchar op) ;//画线框圆，圆心p(x,y),半径r
void Gui_DrawCircleFilled(Page *p,int x,int y,int r,uchar op) ;//画实心圆，圆心p(x,y),半径r

void Gui_DrawTriangleWired(Page *p,int x_1,int y_1,int x_2,int y_2,int x_3,int y_3,uchar op);//以p(x1,y1) p(x2,y2) p(x3,y3)三点画三角形框
void Gui_DrawTriangleFilled(Page *p,int x_1,int y_1,int x_2,int y_2,int x_3,int y_3,uchar op);//以p(x1,y1) p(x2,y2) p(x3,y3)三点画实心三角形
/*--------------------------------------------------------------------*/




//位图模块
/*-------------------------------BMP----------------------------------*/
void Gui_DrawBmp(Page *p,int sx,int sy,int sizex,int sizey,uchar BMP[]) ;//画位图。C51格式，超界自动截取
void Gui_RotateBmp(Page *p,int ox,int oy,int sizex,int sizey,float theta,float alpha,uchar BMP[]);//旋转缩放图片.theta是弧度，alpha是倍数,计算量很大.刷一屏得数秒钟，用之前得斟酌一下
/*--------------------------------------------------------------------*/


//控制模块
 /*---------------------------Control---------------------------------*/
void Gui_ClearBufferBit(Page *p,uchar op);//清空页面，op是指清空为全黑op=0,或全白op=1
void Gui_CopyPage(Page *From,Page *To,uint8_t op);//把From页面拷到To页面上，op=0:覆盖 op=1:并集 op=2:交集 op=3:异或
void Gui_SetPosAbs(Page *p,int sx,int sy);//设置页面左上角的绝对坐标[sx,sy]
void Gui_SetPosRel(Page *Ref,Page *p,int sx,int sy);//设置相对于页面Ref的相对坐标Ref(sx,sy)
void Gui_Flush(Page *px); //把页面打到屏上
/*--------------------------------------------------------------------*/



//交互模块
/*---------------------------Interaction------------------------------*/
uint8_t Gui_TextBox(Page *p,int sx,int sy,uint8_t *Str,uchar op,uchar font);
//交互模块
uint8_t Gui_MessageBox(Page *p,int sx,int sy,uint8_t *Title,uint8_t *Content,uint8_t Type);  //默认
/*--------------------------------------------------------------------*/    
#endif
