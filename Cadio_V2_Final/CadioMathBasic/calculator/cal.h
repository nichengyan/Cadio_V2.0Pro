#ifndef __CAL_H__
#define __CAL_H__

#include "_STC8X_.h"
#include "TYPE.h"
#include "Expression.h"
#include "Gui.h"
#include "Key.h"
#include "set.h"

#define MemSize 8   //表达式记忆深度：16
#define ExprSize 88  //表达式长度：96


typedef struct {
    //float Result;
    uint8_t Expr[ExprSize];
    uint8_t ExprLenth;
}Mem;//一个记忆条目
void Cal_ShowString(Page *p, int sx, int sy, uchar buffer[], uchar font,uchar err_pos); //打印字符串
void Cal_MemInsert(Mem mx);//向记忆库里插入一条记忆
void Cal_MoveR(uint8_t *str,uint8_t len,uint8_t m,uint8_t n);//把字符串第m及其之后(m>=0)的元素往后挪n位（n>=0）
void Cal_MoveL(uint8_t *str,uint8_t len,uint8_t m,uint8_t n);//把字符串第m及其之后(m>=0)的元素往前挪n位（n>=0）
uint8_t Cal_Input(Page *pa,uint8_t *TBuffer,uint8_t *Title);//获取输入字符串
uint8_t Cal_Work(Page *pa);//计算并显示结果，保存记忆

#endif