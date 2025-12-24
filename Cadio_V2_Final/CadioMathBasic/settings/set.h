#ifndef __SET_H__
#define __SET_H__

#include "_STC8X_.h"
#include "TYPE.h"
#include "Gui.h"
#include "cal.h"
#include "Key.h"


void Set_Menu(Page *pa);
uint8_t Set_ColorReverse_Flg();//反色显示  defualt:0
uint8_t Set_SkipAnimation_Flg();//跳过开机动画 default:0
uint8_t Set_AutoContinue_Flg();//使用自动连续性补偿 default:1
uint8_t Set_ErrorDetection_Flg();//自动查错   defualt:1
uint8_t Set_LongExpression_Flg();//长表达式   defualt:0

#endif
