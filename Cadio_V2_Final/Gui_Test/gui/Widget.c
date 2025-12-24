#define _OLED_WIDGET_C
#include "Widget.h"


#if OLED_USE_BMP == 1
#include "Widget_Bmp.h"
#endif

#if OLED_USE_LABEL == 1
#include "oled_label.h"
#endif

#if OLED_USE_CHECKBOX == 1
#include "oled_checkbox.h"
#endif

#if OLED_USE_SLIDER == 1
#include "oled_slider.h"
#endif

#if OLED_USE_SWITCH == 1
#include "oled_switch.h"
#endif

#if OLED_USE_CANVAS == 1
#include "oled_canvas.h"
#endif

static Widget* pFirst, *pLast;
uint8_t WidgetNum;
//链表初始化
void Widget_Init(){
    WidgetNum = 0;
    pFirst = NULL;
    pLast = NULL;

}
//新建节点
Widget* Widget_Create(void){
    Widget* pWidget = malloc(sizeof(Widget));
    pWidget->Type = 0;
    pWidget->Show = 1;
    pWidget->pFeature=NULL;
    pWidget->pPrev=NULL;
    pWidget->pNext=NULL;
    return pWidget;   
}
void Widget_Delete(Widget* pWidget){	
	if(pWidget == pFirst)
	{
		if(pWidget->pNext != NULL)
		{
			((Widget*)pWidget->pNext)->pPrev = NULL;
			pFirst = (Widget*)pWidget->pNext;
		}
		else
		{
			pFirst = NULL;
			pLast = NULL;
		}
	}
	else if(pWidget == pLast)
	{
		((Widget*)pWidget->pPrev)->pNext = NULL; 
		pLast = (Widget*)pWidget->pPrev;
	}
	else
	{
		((Widget*)pWidget->pPrev)->pNext = pWidget->pNext;
		((Widget*)pWidget->pNext)->pPrev = pWidget->pPrev;
	}
	WidgetNum--;
	switch(pWidget->Type)
	{
		case LABEL_WIDGET:
		case SLIDER_WIDGET:
		case CANVAS_WIDGET:
		{
			if(pWidget->pFeature != NULL)
				free(pWidget->pFeature);
			break;
		}
		case BMP_WIDGET:
		case TEXTAREA_WIDGET:
		case SWITCH_WIDGET:
			break;
	}
	free(pWidget);
	pWidget = NULL;
}
void Widget_Add(Widget* pWidget){
    if(pWidget == NULL) return;
    if(WidgetNum==0){
        pFirst = pWidget;
        pLast = pWidget;
        pWidget->pNext = NULL;
        pWidget->pPrev = NULL;
    }else{
        pWidget->pPrev = pLast;
        pWidget->pNext = NULL;
        pLast->pNext = pWidget;
        pLast = pWidget; 
    }
    WidgetNum++;
    return;
}

void Widget_SetPos(Widget* pWidget,uint16_t posx,uint16_t posy){
    if(pWidget == NULL) return;
    pWidget->x = posx;
    pWidget->y = posy;
}
void Widget_SetOffset(Widget* pWidget,int16_t deltax,int16_t deltay){
    if(pWidget == NULL) return;
    pWidget->x += deltax;
    pWidget->y += deltay;
}
void Widget_Draw(){
	Widget* pWidget = pFirst;
	//og_clearDisplayBuf();      

    Graphic_ClearBufferBit(0);     
    //OLED_Printf(0 , 2, 16, "pFir: 0x%06lx" , pFirst);
	while(pWidget != NULL)
	{
		if((pWidget->Show ) == 0)
			goto next;
		if((pWidget->x >= SCREEN_WIDTH) || (pWidget->x + pWidget->w) <= 0)
			goto next;
		if((pWidget->y >= SCREEN_HEIGHT) || (pWidget->y + pWidget->h) <= 0)
			goto next;

		switch(pWidget->Type)
		{
			#if OLED_USE_BMP == 1
			case BMP_WIDGET:
			{
				Widget_BMP_Draw(pWidget);
				break;
			}
			#endif
			#if OLED_USE_SWITCH == 1
			case SWITCH_WIDGET:
			{
				og_switch_draw(pWidget);
				break;
			}
			#endif
			#if OLED_USE_CHECKBOX == 1
			case CHECKBOX_WIDGET:
			{
				og_checkbox_draw(pWidget);
				break;
			}
			#endif
			#if OLED_USE_LABEL == 1
			case LABEL_WIDGET:
			{
				og_label_draw(pWidget);
				break;
			}
			#endif
			#if OLED_USE_SLIDER == 1
			case SLIDER_WIDGET:
			{
				og_slider_draw(pWidget);
				break;
			}
			#endif
			#if OLED_USE_CANVAS == 1
			case CANVAS_WIDGET:
			{
				og_canvas_draw(pWidget);
				break;
			}
			#endif
		}
		next:
		pWidget = (Widget*)pWidget->pNext;
	}
    Graphic_Flush();
	//og_screenRefresh();
}