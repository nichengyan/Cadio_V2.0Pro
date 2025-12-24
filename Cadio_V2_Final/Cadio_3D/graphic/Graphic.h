#ifndef __Graphic_H__
#define __Grpahic_H__
/*Mini Graphic Library For oled12864*/
#include "_STC8X_.h"
#include "TYPE.h"
//#include "intrins.h"
#include "oled.h"
#include "Graphic_Font.h"
#include <math.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>

#define MaxWidth 128
#define MaxHeight 64
/*以左上角为顶点，横x,竖y*/
static unsigned char Graphic_FrameBuffer[1024];
void Graphic_Swap_int8(char *a,char *b);
uchar *Graphic_GetBuffer();
void Graphic_DrawPoint(char x,char y,uchar op);
uint8_t Graphic_GetPoint(char x,char y);
void Graphic_DrawLine(char x1,char y1,char x2,char y2,uchar op);
void Graphic_DrawCircleWired(char x,char y,uchar r,uchar op);
void Graphic_DrawCircleFilled(char x,char y,uchar r,uchar op);
void Graphic_ShowChar(char sx,char sy,uchar c,uchar op);
void Graphic_ShowString(char sx,char sy,uchar buffer[],uchar op);
void Graphic_Printf(char sx,char sy,const char *str,...);
void Graphic_ShowNum(char sx,char sy,ulong num,uchar lenth,uchar mode,uchar op);
void Graphic_DrawBmp(uchar sx,uchar sy,uchar sizex,uchar sizey,uchar BMP[]);
void Graphic_ClearBufferBit(uchar op);
void Graphic_Flush();


#endif