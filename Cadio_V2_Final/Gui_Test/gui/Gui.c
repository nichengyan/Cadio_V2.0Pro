#include "Gui.h"
#include "oled.h"
uint8_t XDATA mem_buf[HEAP_SIZE];	// 堆内存池，用于分配控件参数
uint16_t gui_tick = 0;	// 当前时间戳
uint16_t sss = 0;
void Gui_TimeTick(void)
{
	gui_tick ++;
}
//返回当前时间戳
uint16_t Gui_GetTimeStamp(void)
{
	return gui_tick;
}
// 返回当前时间戳和给定时间戳之间的时间间隔（ms）
uint16_t Gui_TickElaps(uint16_t pre_tick)
{
  if(gui_tick >= pre_tick)  //正常情况下 Gui_tick会更大的
	{
		pre_tick = gui_tick - pre_tick;
    } 
	else                    //如果小了，大概率是溢出一轮了
	{
		pre_tick = 0xffff - pre_tick + 1; 
        pre_tick += gui_tick;
  }
  return pre_tick;
}
void Gui_Init(void)
{
	#if defined(__C251__) || defined(__C51__)	
	uint16_t i;
	for(i = 0; i < HEAP_SIZE; i++)		// 初始化栈			
		mem_buf[i] = 0;
	init_mempool(mem_buf, HEAP_SIZE); // 初始化内存池
    //OLED_Printf(0 ,4,16," pMem: 0x%06lx" , mem_buf);
	#endif
	Widget_Init();		// 初始化控件管理器
	#if OLED_USE_ANIM == 1
    Anim_Init();			// 初始化动画系统
	#endif
}
void Gui_Run(void)
{
	static uint16_t gui_timestamp = 0;
    Gui_TimeTick();
	if(Gui_TickElaps(gui_timestamp) >= OLED_REFRESH_T)
	{

		gui_timestamp = Gui_GetTimeStamp();
        Page_Run();
		//#if OLED_USE_ANIM == 1
		Anim_Run();
		//#endif
       
		Widget_Draw();
        //OLED_Printf(0 , 0, 16, "%d" , rand());
        //while(1);
     //OLED_Printf(0, 0, 16, "DBG");  
	}
}


