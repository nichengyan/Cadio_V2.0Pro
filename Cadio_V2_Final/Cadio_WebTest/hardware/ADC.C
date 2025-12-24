/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-09-22 14:57:09
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-10-03 22:19:53
 * @FilePath: \Cadio_WebTest\hardware\ADC.C
 * @Description:
 * 
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved. 
 */
#include "ADC.h"
void ADC_Init()
{
    ADC_CONTR |= ADC_POWER;
}
void ADC_Shutdown()
{
    ADC_CONTR &= (~ADC_POWER);
}
uint16_t ADC_Result(uint8_t CHS, uint8_t SPD)
{
    ADC_CONTR &= 0XF0; // 清空之前的通道设置
    ADC_CONTR = ADC_CONTR | CHS | ADC_START;
    ADCCFG = ADCCFG | ADC_FORMAT | SPD;
    _nop_();
    _nop_();
    _nop_();
    _nop_();
    while (!(ADC_CONTR & ADC_FLAG))
        ;
    ADC_CONTR &= (~ADC_FLAG);
    return ADC_RES * 256 + ADC_RESL;
}
