#include "Group.h"
#include "stdio.h"

Group* Group_Create()
{
	Group* pGroup = malloc(sizeof(Group));
	if(pGroup == NULL) return;
	pGroup->ItemNumber = 0;
	pGroup->FocusIndex = 0xff;
	return pGroup;
}
void Group_Delete(Group* pGroup)
{
	uint8_t i;
	GroupItem *pItem = &pGroup->item;
	for(i = 0; i < pGroup->ItemNumber; i++)
	{
		Widget_Delete(pItem[i].pWidget);
	}
	free(pGroup);
}
void Group_AddWidget(Group* pGroup, Widget* pWidget, uint8_t FocusEn)
{
	GroupItem *pItem = &pGroup->item;
	if(pWidget == NULL) return;
	pItem[pGroup->ItemNumber].pWidget = pWidget;
	pItem[pGroup->ItemNumber].FocusEn = FocusEn;
	if(FocusEn && (pGroup->FocusIndex == 0xff))   //�ҳ���������һ����focus�Ŀؼ�
	{
		pItem[pGroup->ItemNumber].pWidget->Focus = FOCUS;
		pGroup->FocusIndex = pGroup->ItemNumber;
	}
	pGroup->ItemNumber++;
}
void Group_SetOffset(Group* pGroup, int16_t delta_x, int16_t delta_y)
{
	uint8_t i;
	GroupItem *pItem = &pGroup->item;
	for(i = 0; i < pGroup->ItemNumber; i++)
	{
		Widget_SetOffset(pItem[i].pWidget, delta_x, delta_y);
	}
}
#if (OLED_USE_ANIM == 1)
void Group_SetOffset_Anim(Group* pGroup, int16_t delta_x, int16_t delta_y, uint16_t duration, void* anim_end_cb)
{
	uint8_t i;
	GroupItem *pItem = &pGroup->item;
	for(i = 0; i < pGroup->ItemNumber; i++)
	{
		//Anim_CreateOffset(pItem[i].pWidget, delta_x, delta_y, duration, (i==(pGroup->Item_Num-1)?anim_end_cb:NULL));
	}
}
#endif

/*
void Group_SetWidgetFocus(Group* pGroup, Widget* pWidget, uitn8_t is_focus)
{
	Widget pWidget;
	GroupItem *pItem = &pGroup->item;
	int16_t i;
	int16_t bias_x, bias_y;
	if(pGroup == NULL || pGroup->ucSum == 0 || pGroup->focus_index == -1) return;
	i = pGroup->FocusIdex;
	pItem[i].pWidget->config &= ~WIDGET_FOCUS;
	if(direction == 1)
	{
		while(1)
		{
			if(pItem[++i].focus_en == 1)
			{
				pItem[i].pWidget->config |= WIDGET_FOCUS;
				break;
			}
			if(i == pGroup->ucSum)
				i = -1;
		}
	}
	else
	{
		while(1)
		{
			if(pItem[--i].focus_en == 1)
			{
				pItem[i].pWidget->config |= WIDGET_FOCUS;
				break;
			}
			if(i == -1)
				i = pGroup->ucSum;
		}
	}
	pGroup->focus_index = i;
	pWidget = pItem[i].pWidget;
	if(pWidget->x < 0)
		bias_x = -pWidget->x;
	else if((pWidget->x + pWidget->w) > OLED_WIDTH)
		bias_x = -(pWidget->x + pWidget->w - OLED_WIDTH);
	else
		bias_x = 0;
	if(pWidget->y < 0)
		bias_y = -pWidget->y;
	else if((pWidget->y + pWidget->h) > OLED_HEIGHT)
		bias_y = -(pWidget->y + pWidget->h - OLED_HEIGHT);
	else
		bias_y = 0;
	if(bias_x != 0 || bias_y != 0)
	{
		#if OLED_USE_ANIM == 1
		og_group_addAnimOffset(pGroup, bias_x , bias_y, ANIM_TIME_NORM, ANIM_NULL_CB);
		#else
		og_group_setPosOffset(pGroup, bias_x , bias_y);
		#endif
	}
}
t_psWidget og_group_getFocusWidget(t_psGroup pGroup)
{
	t_sGroupItem *pItem = &pGroup->item;
	#ifdef OLED_DEBUG
	if(pGroup == NULL || pGroup->ucSum == 0 || pGroup->focus_index == -1)
	{
		OG_PRINTERROR("no widget is being focused");
		return NULL;
	}
	#endif
	return pItem[pGroup->focus_index].pWidget;
}
int16_t og_group_getFocusIndex(t_psGroup pGroup)
{
	return pGroup->focus_index;
}
void og_group_hideOffScreenWidget(t_psGroup pGroup)
{
	uint8_t i;
	t_sGroupItem *pItem = &pGroup->item;
	t_psWidget pWidget;
	if(pGroup == NULL || pGroup->ucSum == 0)
		return;
	for(i = 0; i < pGroup->ucSum; i++)
	{
		pWidget = pItem[i].pWidget;
		if((pWidget->x >= OLED_WIDTH) || (pWidget->y >= OLED_HEIGHT))
			og_widget_setShow(pWidget, 0);
		else if((pWidget->x + pWidget->w) <= 0 || (pWidget->y + pWidget->h) <= 0)
			og_widget_setShow(pWidget, 0);
	}
}
*/
