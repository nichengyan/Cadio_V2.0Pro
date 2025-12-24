#ifndef __GUI_CONFIG_H__
#define __GUI_CONFIG_H__

#include "_STC8X_.h"
#include "TYPE.h"
#include <stdlib.h>




#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define HEAP_SIZE 1024
#define MAX_PAGE 8
#define MAX_ITEM 8
#define CODE code
#define XDATA xdata

#define OLED_REFRESH_T  30




/********选择想要用的功能***********/
#define	OLED_USE_LOG			0					// 要使用串口打印LOG功能，请初始化串口并做好putChar()函数的重定向
#define	OLED_USE_ANIM			1
#define	OLED_USE_GROUP		1
/********选择想要用的控件***********/
#define	OLED_USE_BMP			1		//图片控件
#define	OLED_USE_LABEL		0		//标签控件
#define	OLED_USE_CHECKBOX	0		//复选框控件
#define	OLED_USE_SLIDER		0		//滑块控件
#define	OLED_USE_SWITCH		0		//开关控件
#define	OLED_USE_CANVAS		0		//画布控件


#endif


