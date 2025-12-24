#ifndef __EDITOR_H__
#define __EDITOR_H__

#include "TYPE.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
//#include "_STC8X_.h"
#include "Gui.h"
#include "string.h"

#define Line_Length 19


void Select_Command(uint8_t *Str);
void Editor_ShowString(Page *p, int sx, int sy, uchar buffer[], uchar font, uchar err_pos);
void Editor_delay(uint16_t t);
void Editor_MoveR(uint8_t *str, uint8_t len, uint8_t m, uint8_t n);
void Editor_MoveL(uint8_t *str, uint8_t len, uint8_t m, uint8_t n);
uint8_t Editor_Main(Page *pa, uint8_t Code[][32]);


#endif