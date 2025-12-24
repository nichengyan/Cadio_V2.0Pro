#ifndef __GUI_H__
#define __GUI_H__

#include "Gui_Config.h"
#include "Widget.h"
#include  "Page.h"

#if OLED_USE_ANIM == 1
#include "Animation.h"
#endif

#if OLED_USE_GROUP == 1
#include "Group.h"
#endif

#if OLED_USE_BMP == 1
#include "Widget_Bmp.h"
#endif

#if OLED_USE_LABEL == 1
//#include "oled_label.h"
#endif

#if OLED_USE_CHECKBOX == 1
//#include "oled_checkbox.h"
#endif

#if OLED_USE_SLIDER == 1
//#include "oled_slider.h"
#endif

#if OLED_USE_SWITCH == 1
//#include "oled_switch.h"
#endif

#if OLED_USE_CANVAS == 1
//#include "oled_canvas.h"
#endif


void Gui_TimeTick(void);
uint16_t Gui_GetTimeStamp(void);
uint16_t Gui_TickElaps(uint16_t pre_tick);
void Gui_Init(void);
void Gui_Run(void);

#endif
