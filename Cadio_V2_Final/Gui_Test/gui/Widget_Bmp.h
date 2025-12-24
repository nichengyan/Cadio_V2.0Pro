#ifndef __WG_BMP_H__
#define __WG_BMP_H__


#include "Widget.h"
#include "Graphic.h"

Widget* Widget_BMP_Create(uint16_t px, uint16_t py,uint16_t sizex, uint16_t sizey, uint8_t *bmp);
void Widget_BMP_Set(Widget* pWidget, uint16_t sizex, uint16_t sizey, uint8_t *bmp);
void Widget_BMP_Draw(Widget* pWidget);



#endif