#ifndef __Expression_H__
#define __Expression_H__
/*--------------------------------------------------------------------------
EXPRESSION.H

Basic Math Expression Calculator
Copyleft (c) 2024~2024 NCY
All rights reversed.

--------------------------------------------------------------------------*/
#include "TYPE.h"
#include "_STC8X_.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
//宏定义
#define Pi 3.14159 
#define Exp 2.71828
#define MaxLenth 32
//#define NAN 0.0/0.0
//#define INF 1.0/0.0
//算符标志
#define OPERATOR_INVALID 0 //无效算符
#define ADD_FLAG 1 // +
#define MIN_FLAG 2 // -
#define MUL_FLAG 3 // *
#define DIV_FLAG 4 // /
#define POW_FLAG 5 // ^
#define PLEFT_FLAG 6  //  (
#define PRIGHT_FLAG 7 //  )
#define COMMA_FLAG 8  //  ,


#define FUNC_INVALID 0  //无效函数
#define	SIN_FLAG 1  //sin
#define	COS_FLAG 2  //cos
#define	TAN_FLAG 3  //tan
#define	ASIN_FLAG 4  //asin
#define	ACOS_FLAG 5  //acos
#define	ATAN_FLAG 6  //atan
#define	ABS_FLAG 7  //abs
#define	SINH_FLAG 8  //sinh
#define	COSH_FLAG 9  //cosh
#define	TANH_FLAG 10 //tanh
#define LN_FLAG 11  // ln
#define	LG_FLAG 12  //lg
#define	RAND_FLAG 13  // rand
#define RANDOM_FLAG  14  //random
#define FACTORIAL_FLAG 15// factorial

#define TYPE_INVALID 0  //无效类型
#define TYPE_NUMBER 1   //数字
#define TYPE_PARAMETER 2  //参数（本质还是数字，在转逆波兰式时候就替换为对应数值了）
#define TYPE_OPERATOR 3  //算符（符号）
#define TYPE_FUNCTION 4  //函数


#define CMP_INVALID 0   //无效比较结果
#define CMP_LARGER 1    //优先级a>b
#define CMP_SAME 2      //优先级a==b
#define CMP_SMALLER 3   //优先级a<b

//四个优先级
#define PRIORITY0 0
#define PRIORITY1 1
#define PRIORITY2 2
#define PRIORITY3 3
#define PRIORITY4 4
//函数 》 '^'  》 '*' '/' 》 '+''-' 》  Invalid
// 4      3          2          1          0


//错误类型
//0x01:Syntax Error 0x01 存在多余的小数点 
//0x02:Syntax Error 0x02 括号不匹配
//0x03:Syntax Error 0x03 存在未定义的字符 
//0x04:Syntax Error 0x04 无效输入
//0x05:Syntax Error 0x05 错误数学表达式
//0x06:Unkwon Error 0x06 未知错误 
//0x07:Math Error 0x07 数学错误 


//Cal_token是最基础元素
typedef struct{
	uint8_t type;
	float value;		
}Cal_Token;

/*

Cal_token.type:

type==0:无效  invalid
--------------------------------------
type==1:数字  value:就是浮点数值 
--------------------------------------
type==2:常量/变量  
              u,v,w  x y z  
              value: 1 2 3  4 5 6 
--------------------------------------	   
type==3符号           +-*\^(),  
              value:  12345678    
--------------------------------------	           
	                1元 1元 1元  1元  1元 1元  1元 1元   1元  1元  1元 1元  0元  2元       //参数个数，不是标价!（^_^） 
type==4函数         sin cos tan asin acos atan abs sinh cosh tanh  ln  lg  rand random 
              value: 1   2   3   4    5    6    7    8    9    10  11  12   13    14    
              a: asin acos atan abs
              s:sin sinh
              c:cos cosh
              t:tan tanh
              l:ln lg
              r:rand random 
              注：rand()产生0~1内随机浮点数 
                  random(a,b) b>a>0 产生在a~b内随机整数 

*/
typedef struct{
	Cal_Token token[MaxLenth];
	uint8_t Stack_Ptr;	
}Cal_Stack;
typedef struct{
	Cal_Token token[MaxLenth];
	uint8_t Array_Ptr;		
}Cal_Array;
typedef struct{
	uint8_t Str[MaxLenth];
	uint8_t Str_Ptr;
}Cal_String; 

//Stack
uint8_t Stack_IsEmpty(Cal_Stack *stk);
uint8_t Stack_Push(Cal_Stack *stk,Cal_Token Element);
uint8_t  Stack_Pop(Cal_Stack *stk);
Cal_Token Stack_Get_Top(Cal_Stack *stk);
//ErrorHandler(Uselessly...)
void ErrorHandler(uint8_t *bf);
//Working Functions
uint8_t Cal_Get_Token(Cal_String *s,Cal_Array *tk);
uint8_t Cal_cmp(Cal_Token a,Cal_Token b);
uint8_t Cal_Mid2Post(Cal_Array *Tokens,Cal_Array *Post);
float Cal_Calculate_RPN(Cal_Array *Post,float *Para);
//User:
float Calculate_Expression(char *s,uint8_t l, float *para);
uint8_t Get_ErrorCode();
#endif