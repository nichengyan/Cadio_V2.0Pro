#include "Page.h"


/**
  * @brief  注册一个基本页面，包含一个初始化函数，循环函数，退出函数，事件函数
  * @param  pageID: 页面编号
	*	@param	pageText: 页面标题的指针
	*	@param	pageIcon: 页面图标的指针
  * @param  setupCallback: 初始化函数回调
  * @param  loopCallback: 循环函数回调
  * @param  exitCallback: 退出函数回调
  * @param  eventCallback: 事件函数回调
  * @retval 无
  */
void Page_Register(
        unsigned char pageID,
		unsigned char *pageTitle,
		unsigned char *pageIcon,
	    void (*SetupCallback)(uint8_t a), //开始时时调用
        void (*LoopCallback)(void), //循环调用
        void (*ExitCallback)(uint8_t a), //退出调用
        void (*EventCallback)(Event e)
)
{
		PageList[pageID].Title = pageTitle;
		PageList[pageID].Icon = pageIcon;
		PageList[pageID].Stat = PAGE_IDLE;
        PageList[pageID].SetupCallback = SetupCallback;
        PageList[pageID].LoopCallback = LoopCallback;
        PageList[pageID].ExitCallback = ExitCallback;
        PageList[pageID].EventCallback = EventCallback;
}
/**
  * @brief  页面事件传递
  * @param  event: 事件编号
  * @retval 无
  */
void Page_EventTransmit(Event e)
{
	/*将事件传递到当前页面*/
  if((PageList[now_page].EventCallback != 0) && (PageList[now_page].Stat == PAGE_RUN))
		PageList[now_page].EventCallback(e);
}
void Page_OpenCurrentPage()
{
	if(PageList[now_page].Stat == PAGE_IDLE)
	{
		PageList[now_page].SetupCallback(PAGE_ENTER);
		PageList[now_page].Stat = PAGE_RUN;
	}
}
void Page_CloseCurrentPage()
{
	if(PageList[now_page].Stat == PAGE_RUN)
		PageList[now_page].ExitCallback(PAGE_RETURN);
}
/**
  * @brief  页面切换
  * @param  pageID：页面号
	* @retval 无
  */
void Page_Shift(unsigned char pageID, unsigned char condition)
{
	if((now_page != pageID) && (PageList[pageID].Stat == PAGE_IDLE))
	{
		if(PageList[now_page].ExitCallback != 0)
		{
			PageList[now_page].ExitCallback(condition);
		}
		now_page = pageID;
		if(PageList[now_page].SetupCallback != 0)
		{
			PageList[now_page].SetupCallback(condition);
			PageList[now_page].Stat = PAGE_RUN;
		}
	}
}
void Page_Run(void)
{
	PageList[now_page].LoopCallback();
}
void Page_SetStat(unsigned char pageID, unsigned char Stat)
{
	PageList[pageID].Stat = Stat;
}
unsigned char Page_GetStat(void)
{
	return PageList[now_page].Stat;
}
