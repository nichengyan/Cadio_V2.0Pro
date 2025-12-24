#ifndef __FUN_H__
#define __FUN_H__

#include "_STC8X_.h"
#include "TYPE.h"
#include "Expression.h"
#include "Gui.h"
#include "cal.h"
#include "Key.h"
void Fun_delay(uint16_t t);
uint8_t Fun_Select(Page *pa);
void Fun_Unary(Page *pa);
void Fun_Implicit(Page *pa);
void Fun_Dynamic(Page *pa);
void Fun_ParaEqu(Page *pa);
#endif