#ifndef __ANIMATION_H__
#define __ANIMATION_H__
#include "Widget.h"
#include "Gui.h"
#include "Gui_Config.h"
#include <stdlib.h>
typedef struct{
	Widget* pWidget;
	int16_t start_x;
	int16_t start_y;
	int16_t distance_x;
	int16_t distance_y;
	uint16_t duration;
	uint16_t act_time;
	void* anim_end_cb;
	void *pPrev;
	void *pNext;
}Anim;

static uint16_t Anim_TimeStamp;
void Anim_Init(void);
void Anim_Delete(Anim* pAnim);
void Anim_Run(void);
void Anim_CreatePos(Widget* pWidget, int16_t end_x, int16_t end_y, uint16_t duration, void* anim_end_cb);
void Anim_CreateOffset(Widget* pWidget, int16_t delta_x, int16_t delta_y, uint16_t duration, void* anim_end_cb);
#endif