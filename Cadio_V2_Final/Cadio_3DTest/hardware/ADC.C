/*
 * @Author: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @Date: 2025-08-17 16:33:39
 * @LastEditors: error: error: git config user.name & please set dead value or install git && error: git config user.email & please set dead value or install git & please set dead value or install git
 * @LastEditTime: 2025-09-22 14:22:13
 * @FilePath: \Cadio_3DTest\hardware\ADC.C
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
#include "ADC.h"
void ADC_Init()
{
    ADC_CONTR |= ADC_POWER;
}
void ADC_ShutDown()
{
    ADC_CONTR &= (~ADC_POWER);
}
uint16_t ADC_Result(int8 CHS, int8 SPD)
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
