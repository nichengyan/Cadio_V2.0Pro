#ifndef __Expression_H__
#define __Expression_H__
/*--------------------------------------------------------------------------
EXPRESSION.H

Basic Math Expression Calculator
Copyleft (c) 2024~2024 NCY
All rights reversed.

--------------------------------------------------------------------------*/
//#include "TYPE.h"
//#include "_STC8X_.h"
#include "TYPE.h"

#include "Gui.h"
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

extern Page pMain;

//宏定义
#define Pi 3.14159 
#define Exp 2.71828
#define ln10 2.302585092
#define MaxLenth 36
//#define FastCal_Cnt //快速计算通道数（当绘制函数图像时，可能会遇到两组快速计算的情况。但其他时候一般用不到）


//#define NAN 0.0/0.0
//#define INF 1.0/0.0
//算符标志
#define OPERATOR_INVALID 0 //无效算符
#define ADD_FLAG 1 // +
#define MIN_FLAG 2 // -
#define MUL_FLAG 3 // *
#define DIV_FLAG 4 // /
#define POW_FLAG 5 // ^

#define BIGGER_FLAG  6 // >
#define SMALLER_FLAG  7 // <
#define EQUAL_FLAG 8  // ==
#define BIGGER_EQUAL_FLAG 9 // >=
#define SMALLER_EQUAL_FLAG 10 //<=

#define MOD_FLAG 11 //mod


#define PLEFT_FLAG 12  //  (
#define PRIGHT_FLAG 13 //  )
#define COMMA_FLAG 14  //  ,


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


#define READVARIOUS_FLAG  18//rv 读取变量  rv(address)
#define WRITEVARIOUS_FLAG  19//wv 写入变量 wv(address,value)
#define ADDRESS_FLAG 20 //ad(address,space)
//#define LABEL_FLAG 21  //lb(const)
#define JUDGEZEROJMP_FLAG  22 //jz(value, linenum, error) 判0跳转 
#define JUDGENONEZEROJMP_FLAG  23 //jnz(value, linenum, error) 判非0跳转 
#define JMP_FLAG 24
#define COMPARE_FLAG 25 //cmp(value1 , value2  mode) 0 1 2 3 4  < <= > >= ==

#define WAIT_FLAG 26//wait(ms)

//文本IO 不走缓冲区直接往屏幕上打，不需要手动刷新
#define PUT_FLAG 27 //put(value); 
#define GET_FLAG 28 //get(value); 

//图形IO 写入缓冲区，需要手动刷新
#define CLEAR_FLAG 29 //clr()
#define FLUSH_FLAG 30 //fls(value);
#define DISPLAY_FLAG 31//dis(x,y,value);
#define DRAWPOINT_FLAG 32//dpt(x,y)
#define GETPOINT_FLAG 32//gpt(x,y)


#define TICK_FLAG 35  //tick()
//#define DRAWPOINT_FLAG 31//dpt(x,y)
//#define DRAWPOINT_FLAG 31//dpt(x,y)
//#define DRAWCIRCLE

//#define READ_FLAG 17

#define TYPE_INVALID 0  //无效类型
#define TYPE_CONSTANT 1   //数字
#define TYPE_VARIABLE 2  //参数（本质还是数字，在转逆波兰式时候就替换为对应数值了） 
#define TYPE_OPERATOR 3  //算符（符号）
#define TYPE_FUNCTION 4  //函数
#define TYPE_TEMP 5

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
#define PRIORITY5 5
//函数/比较运算符 》 '^'  》 '*' '/' 》 '+''-' 》  >=< ==Invalid
// 5      4          3          2          1      0


//错误类型
//0x01:Syntax Error 0x01 存在多余的小数点 
//0x02:Syntax Error 0x02 括号不匹配
//0x03:Syntax Error 0x03 存在未定义的字符 
//0x04:Syntax Error 0x04 无效输入
//0x05:Syntax Error 0x05 错误数学表达式
//0x06:Unkwon Error 0x06 未知错误 
//0x07:Math Error 0x07 数学错误 
//0x08:Out of Range 越界 


//Cal_token是最基础元素
typedef struct{
	uint8_t type;
	float value;
	
}Cal_Token;

/*

Cal_token.type:

type==0:无效  invalid
--------------------------------------
type==1:常量  
--------------------------------------
type==2:变量
		
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
----------------------------------------                  

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
float Cal_Calculate_RPN(Cal_Array *Post);
void Cal_UpdateAns(float ans);//更新Ans的值，想用就用。
//User:
uint8_t Get_NowLine();
void Set_NowLine(uint8_t l);
float Mem_Read(uint8_t addr);
void Mem_Write(uint8_t addr ,float val);
void Calculate_Init();
float Calculate_Expression(char *s,uint8_t l);//这个可以直接用

void OutPut_RPN();

/* 
typedef struct vm{
	uint8_t NowLine;
	uint16_t Instruct_Ptr;
};
void Init_RPN();
void Execute_RPN(); 
void Debug();
*/
//void FastCal_Init(char *s,uint8_t l,uint8_t FastCal_Id);
//float FastCal_Work(uint8_t FastCal_Id);
//FastCal快速计算功能（详见Expression.c）: 
//使用时先给定字符串调用一次Init，再给出参数循环调用Work来计算不同参数时的函数值
uint8_t Get_ErrorCode();
#endif
