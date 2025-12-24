#include "Widget_Bmp.h"

Widget* Widget_BMP_Create(uint16_t px, uint16_t py, uint16_t sizex, uint16_t sizey, uint8_t* bmp){
    Widget* pWidget = Widget_Create();
    pWidget-> x = px;
    pWidget-> y = py;
    pWidget-> w = sizex;
    pWidget-> h = sizey;
    pWidget-> Type = BMP_WIDGET;
    pWidget-> Focus = 0;
  
    pWidget-> pPrev = pWidget-> pNext = NULL;
    pWidget-> pFeature = bmp;
    Widget_Add(pWidget);
    return pWidget;
}
void Widget_Set_BMP(Widget* pWidget, uint16_t sizex, uint16_t sizey, uint8_t *bmp){
    if(pWidget == NULL) return;
    pWidget-> h = sizey;
    pWidget-> w = sizex;
    pWidget-> pFeature = bmp;
    return;
}
void Widget_BMP_Draw(Widget* pWidget){
   Graphic_DrawBmp(pWidget->x, pWidget->y, pWidget->w, pWidget-> h, pWidget->pFeature);
}