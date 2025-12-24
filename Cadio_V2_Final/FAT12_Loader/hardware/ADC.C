/*
 * @Author: 1234 && 5678
 * @Date: 2025-08-17 16:33:39
 * @LastEditors: 1234 && 5678
 * @LastEditTime: 2025-10-03 22:57:10
 * @FilePath: \FAT12_Loader\hardware\ADC.C
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
int16 ADC_Result(int8 CHS, int8 SPD)
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
