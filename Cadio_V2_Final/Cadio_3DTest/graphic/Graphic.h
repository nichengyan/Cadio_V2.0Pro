#ifndef __Graphic_H__
#define __Grpahic_H__
/*Mini Graphic Library For oled12864*/
#include "_STC8X_.h"
#include "TYPE.h"
#include "intrins.h"
#include "oled.h"
#include <math.h>
#define MaxWidth 128
#define MaxHeight 64
/*以左上角为顶点，横x,竖y*/
static xdata uchar Graphic_FrameBuffer[1024];
uchar *Graphic_GetBuffer();
void Graphic_Swap_int16(int *a,int *b);
void Graphic_DrawPoint(int x,int y,uchar op);
void Graphic_DrawLine(int x1,int y1,int x2,int y2,uchar op);
void Graphic_DrawCircleWired(int x,int y,uint r,uchar op);
void Graphic_DrawCircleFilled(int x,int y,uint r,uchar op);
void Graphic_ClearBufferBit(uchar op);
void Graphic_Flush();

#endif