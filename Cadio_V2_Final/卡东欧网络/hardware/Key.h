#ifndef __key_h__
#define __key_h__
#include "TYPE.h"
#include "ADC.h"
uint16_t Key_GetADC(uchar chs);
uint Key_Scan(void);
#endif