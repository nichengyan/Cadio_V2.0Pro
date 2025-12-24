#ifndef _Buzzer_
#define _Buzzer_
#include "TYPE.H"
#include "_STC8X_.h"
#include "intrins.h"
sbit Buzzer=P5^4;
void Buzzer_Init();
void Buzeer_Interrupt();
void Buzzer_Stop();
#endif