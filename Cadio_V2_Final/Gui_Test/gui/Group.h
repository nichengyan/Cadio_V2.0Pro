#ifndef __GROUP_H__
#define __GROUP_H__

#include "Gui_Config.h"
#include "Widget.h"

typedef struct{
    Widget* pWidget ;//在Widget链表中的位置
    uint8_t FocusEn;//是否可以被关注        
}GroupItem;
typedef struct{
    GroupItem item[MAX_ITEM];
    uint8_t ItemNumber;
    uint8_t FocusIndex;
}Group;

Group* Group_Create();  
void Group_Delete(Group* pGroup);
void Group_AddWidget(Group* pGroup, Widget* pWidget, uint8_t FocusEn);
void Group_SetOffset(Group* pGroup, uint16_t deltax, uint16_t deltay);
#if OLED_USE_ANIM == 1
void Group_SetOffset_Anim(Group* pGroup, int16_t delta_x, int16_t delta_y, uint16_t duration, void* anim_end_cb);
#endif


void Group_SetWidgetFocus(Group* pGroup, Widget* pWidget, uint8_t is_focus);
Widget* Group_GetWidgetFocus(Group* pGroup);
//int16_t og_group_getFocusIndex(t_psGroup pGroup);
void Group_SetWidgetVisible(Group* pGroup, uint8_t is_visible);

#endif