#ifndef __WIDGET_H__
#define __WIDGET_H__

#include "Gui_Config.h"
#include "TYPE.h"
#include "Graphic.h"
//#include "canvas.h"
typedef struct{
    uint16_t x;
    uint16_t y;
    uint16_t w;
    uint16_t h;  
    uint8_t Type;//属性
    uint8_t Focus;//关注
    uint8_t Show;//图层
    void* pFeature;//特殊属性指针
    void* pPrev;
    void* pNext;   
}Widget;

#define FOCUS 1
#define NONFOCUS 0
enum Widget_Type{
    INVALID_WIDGET,          //不合法控件
	BMP_WIDGET,				// 图片控件
	LABEL_WIDGET,			// 标签控件
	CANVAS_WIDGET,		// 画布控件
	TEXTAREA_WIDGET,	    // 文本编辑器控件
	SLIDER_WIDGET,		// 滑块控件
	SWITCH_WIDGET			// 开关控件
};
//typedef Widget*
void Widget_Init();
Widget* Widget_Create(void);
void Widget_Delete(Widget* pWidget);
void Widget_Add(Widget* pWidget);
void Widget_SetPos(Widget* pWidget,uint16_t posx,uint16_t posy);
void Widget_SetOffset(Widget* pWidget,int16_t deltax, int16_t deltay);
void Widget_Draw();


#endif