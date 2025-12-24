
//#include "Widget.h"
#include "Animation.h"

static Anim *pFirst,*pLast;
uint8_t AnimNum;

void Anim_Init(){
    AnimNum = 0;
    pFirst = NULL;
    pLast = NULL;

}
void Anim_Delete(Anim* pAnim)
{
	if(pAnim == pFirst)
	{
		if(pAnim->pNext != NULL)
		{
			((Anim*)pAnim->pNext)->pPrev = NULL;
			pFirst = (Anim*)pAnim->pNext;
		}
		else
		{
			pFirst = NULL;
			pLast = NULL;
		}
	}
	else if(pAnim == pLast)
	{
		((Anim*)pAnim->pPrev)->pNext = NULL; 
		pLast = (Anim*)pAnim->pPrev;
	}
	else
	{
		((Anim*)pAnim->pPrev)->pNext = pAnim->pNext;
		((Anim*)pAnim->pNext)->pPrev = pAnim->pPrev;
	}
	AnimNum--;
	free(pAnim);
}
uint16_t elaps ;
void Anim_Run(){
	Anim* pAnim = pFirst; //获取动画链表中的第一个元素
    void (*anim_end_cb)(void);//回调函数
    
	elaps = Gui_TickElaps(Anim_TimeStamp);//计算两次更新动画之间的时间
	Anim_TimeStamp = Gui_GetTimeStamp();	
	while(pAnim != NULL)
	{
        
		float p;
		pAnim->act_time += elaps;
		if(pAnim->act_time > pAnim->duration){
			//pAnim->act_time = pAnim->duration;
        }
       //OLED_Printf(0, 0, 16 ,"%d",elaps); 
		//p = animate_fx((float)pAnim->act_time / pAnim->duration);
        p = (float)(pAnim->act_time / pAnim->duration);
        

		pAnim->pWidget->x = pAnim->start_x + pAnim->distance_x * p;
		pAnim->pWidget->y = pAnim->start_y + pAnim->distance_y * p;
        
        pAnim->act_time ++;
		//pAnim->pWidget->x += pAnim->start_x + pAnim->distance_x * 0.1;
		//pAnim->pWidget->y += pAnim->start_y + pAnim->distance_y * 0.1;
        //pAnim->distance_x = pAnim->
		if(pAnim->act_time == pAnim->duration)
		{
			anim_end_cb = pAnim->anim_end_cb;
			Anim_Delete(pAnim);
			if(pAnim->anim_end_cb != NULL)
				anim_end_cb();
		}
		pAnim = (Anim*)pAnim->pNext;
	}
}
//新建节点
void Anim_CreatePos(Widget* pWidget, int16_t end_x, int16_t end_y, uint16_t duration, void* anim_end_cb){
	Anim* pAnim = pFirst;
	if(pWidget == NULL) return;
	while(pAnim != NULL)	//遍历动画列表检查当前控件是否在动画列表里
	{
		if(pAnim->pWidget == pWidget)	//当前控件在动画列表里
			goto anim_overwrite;				//直接重写动画参数，不用重新申请内存
		pAnim = (Anim*)pAnim->pNext;
	}
	pAnim = malloc(sizeof(Anim));		//为动画参数申请一部分内存
	if(AnimNum == 0)
	{
		pAnim->pPrev = NULL;
		pAnim->pNext = NULL;
		pFirst = pAnim;
		pLast = pAnim;
	}
	else
	{
		pAnim->pPrev = pLast;
		pAnim->pNext = NULL;
		pLast->pNext = pAnim;
		pLast = pAnim;
	}
	AnimNum++;
	
	pAnim->pWidget = pWidget;
anim_overwrite:
	pAnim->start_x = pWidget->x;
	pAnim->start_y = pWidget->y;
	pAnim->distance_x = end_x - pWidget->x;
	pAnim->distance_y = end_y - pWidget->y;
	pAnim->act_time = 0;
	pAnim->duration = duration;
	pAnim->anim_end_cb = anim_end_cb;
}

void Anim_CreateOffset(Widget* pWidget, int16_t delta_x, int16_t delta_y, uint16_t duration, void* anim_end_cb)
{
	Anim* pAnim;
	if(pWidget == NULL) return;
	pAnim = pFirst;
	while(pAnim != NULL)	//遍历动画列表检查当前控件是否在动画列表里
	{
		if(pAnim->pWidget == pWidget)	//当前控件在动画列表里
			goto anim_overwrite;				//直接重写动画参数，不用重新申请内存
		pAnim = (Anim*)pAnim->pNext;
	}
	pAnim = malloc(sizeof(Anim));		//为动画申请一部分内存
	if(AnimNum == 0)
	{
		pAnim->pPrev = NULL;
		pAnim->pNext = NULL;
		pFirst = pAnim;
		pLast = pAnim;
	}
	else
	{
		pAnim->pPrev = pLast;
		pAnim->pNext = NULL;
		pLast->pNext = pAnim;
		pLast = pAnim;
	}
	AnimNum++;
	
	pAnim->pWidget = pWidget;
	pAnim->distance_x = delta_x;
	pAnim->distance_y = delta_y;
	goto anim_end;
	anim_overwrite:
	pAnim->distance_x = delta_x + pAnim->distance_x - (pWidget->x - pAnim->start_x);
	pAnim->distance_y = delta_y + pAnim->distance_y - (pWidget->y - pAnim->start_y);
	anim_end:
	pAnim->start_x = pWidget->x;
	pAnim->start_y = pWidget->y;
	pAnim->act_time = 0;
	pAnim->duration = duration;
	pAnim->anim_end_cb = anim_end_cb;
}

