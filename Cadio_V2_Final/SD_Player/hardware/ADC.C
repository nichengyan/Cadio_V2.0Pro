#include "ADC.h"
void ADC_Init(){
    ADC_CONTR|=ADC_POWER;
}
void ADC_Shutdown(){
    ADC_CONTR&=(~ADC_POWER);
}
uint16_t ADC_Result(uchar CHS,uchar SPD){
    ADC_CONTR&=0XF0;//清空之前的通道设置
	ADC_CONTR=ADC_CONTR|CHS|ADC_START;
    ADCCFG=ADCCFG|ADC_FORMAT|SPD;
    _nop_();                        
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR&ADC_FLAG));
    ADC_CONTR&=(~ADC_FLAG);
    return ADC_RES*256+ADC_RESL;
}  
	

