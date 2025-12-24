#include "Buzzer.h"
void Buzzer_Init(){
    //t1 mode0
    TMOD|=0x00;
    TH1=0x00;
    TL1=0x00;
    ET1=1;
    TR1=0;
}
void Buzzer_Interrupt() interrupt 3 using 0{
    Buzzer=~Buzzer;
}
void Buzzer_Stop(){
    Buzzer=1;
}