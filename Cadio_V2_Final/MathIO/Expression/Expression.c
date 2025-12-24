#include "Expression.h"
#include <string.h>
#include <ctype.h>
#include "oled.h"
#include "Gui.h"
#include "Key.h"
#include "tick.h"

extern Page pMain;

//所有函数对应的元素数 
const uint8_t Func_Num[] = { 0 , 1 ,1 ,1, 1, 1, 1, 1 ,1, 1, 1, 1, 1, 0, 2, 1, 2, 2, 1, 2};

static uint8_t ErrorIndex = 0;
static uint8_t ErrorCode = 0; // 错误代码
static float Ans = 0;		  // 上一次的计算结果

// 用于快速代入计算的中间变量
//static Cal_String FastCal_Input[FastCal_Cnt];							 // 字符输入
//static Cal_Array FastCal_Tokens[FastCal_Cnt], FastCal_Post[FastCal_Cnt]; // Tokens  和  后缀式
static uint8_t Line_Ptr = 0;
static uint8_t Text_Line = 0;

uint8_t Get_NowLine(){
	return Line_Ptr;	
}
void Set_NowLine(uint8_t l){
	Line_Ptr = l;	
}




static float Variable[32]; //
//static float Math_Ram[RAM_SIZE]; 


//static uint32_t 
/*----------------------Stack-------------------*/

uint8_t Stack_IsEmpty(Cal_Stack *stk)
{
	if (stk->Stack_Ptr == 0)
		return 1;
	else
		return 0;
}
uint8_t Stack_Push(Cal_Stack *stk, Cal_Token Element)
{
	if (stk->Stack_Ptr >= MaxLenth)
		return 0xff; // 入栈错误
	stk->token[stk->Stack_Ptr++] = Element;
	return 0;
}

uint8_t Stack_Pop(Cal_Stack *stk)
{
	if (stk->Stack_Ptr == 0)
		return 0xff; // 出栈错误
	stk->Stack_Ptr--;
	return 0;
}
Cal_Token Stack_Get_Top(Cal_Stack *stk)
{
	Cal_Token res;
	res.type = 0;
	res.value = 0;
	if (stk->Stack_Ptr == 0)
		return res; // Invalid
	res = stk->token[stk->Stack_Ptr - 1];
	return res;
}



/*------------------------------------------------*/
// 错误处理，如果抛出错误，就交给它了
/*
void ErrorHandler(uint8_t *bf)
{
	if (ErrorCode == 0x01)
		sprintf(bf, "Syntax Error %02x", ErrorCode);
	if (ErrorCode == 0x02)
		sprintf(bf, "Syntax Error %02x", ErrorCode);
	if (ErrorCode == 0x03)
		sprintf(bf, "Syntax Error %02x", ErrorCode);
	if (ErrorCode == 0x04)
		sprintf(bf, "Syntax Error %02x", ErrorCode);
	if (ErrorCode == 0x05)
		sprintf(bf, "Syntax Error %02x", ErrorCode);
	if (ErrorCode == 0x06)
		sprintf(bf, "Unkwon Error");
	if (ErrorCode == 0x07)
		sprintf(bf, "Math Error");
	sprintf(bf, "\n");
	ErrorCode = 0;
}
*/
// 函数名：Cal_Get_Token(Cal_String *s,Cal_Array *tk)
// 传入参数：Cal_String *s,是一个待解析的字符串，Cal_Array *tk存放解析出来的一个一个token
// 功能：从字符串*s里解析出token,放入*tk中
// 返回0：过程终止（可能是因为解析结束，也可能是因为遇到错误，具体看ErrorCode的值），可借此判断是否扫描结束
// 否则：返回的数字对应该token类型



/*---------------------Delay-------------------*/

void Delay1ms(void)	//@40.000MHz
{
	unsigned char data i, j;

	_nop_();
	i = 52;
	j = 240;
	do
	{
		while (--j);
	} while (--i);
}
void __Sleep(uint16_t ms){
    while(ms--) Delay1ms();
}

/*---------------------Input-------------------*/
float Input_Num(uint8_t Line){
    
    uint8_t kl = 0, kr = 0, keynum = 0;
    //OLED_Printf(0,Line * 2,16,">               ");
    uint8_t buf[20];
    uint8_t ptr = 0, i;
    float res = 0;
    memset(buf,0,20);
    //OLED_Printf(0,0,16,"aa");
    while (Key_Scan() != 0xffff);
    while(1){
        kl = Key_Scan() >> 8;
        kr = Key_Scan();
        // tick ++;
        if (kl != 0xff)
            keynum = kl;
        else if (kr != 0xff)
            keynum = kr;
        else
            keynum = 0xff;
        if(keynum >= 0x30 && keynum <= 0x39){
            buf[ptr] = keynum - 0x30 + '0';
            if(ptr < 15 ) ptr ++;
        }
        if(keynum == 0x6e){
            buf[ptr] = '.';
            if(ptr < 15 ) ptr ++;
        }
        if(keynum == 0x55){
            if(ptr > 0 ) ptr--;
        }
        for(i = ptr; i < 16; i++){
            buf[i] = '\0';
        }        
        if(keynum == 0x3d){
            res = atof(buf);
            return res;
        }    
        //OLED_Printf(0,0,16,"0x%02bx",keynum);
        OLED_Printf(0,Line * 2,16,">               ");
        OLED_Printf(0,Line * 2,16,">%s",buf);
        __Sleep(100);
    }
}
/* 改进的词法分析器 - 所有功能集成在一个函数里 */
uint8_t Cal_Get_Token(Cal_String *s, Cal_Array *tk)
{
    Cal_Token Token_Tmp;
    uint8_t Num_Tmp[MaxLenth], Num_Ptr = 0, i = 0;
    uint8_t Dot_Flg = 0;
    uint8_t Degree_Flg = 0;
    uint8_t func_name[10] = {0};
    uint8_t name_len = 0;
    char current_char;
    Token_Tmp.type = TYPE_INVALID;
    Token_Tmp.value = 0;
    
    /* 清空临时缓冲区 */
    for (i = 0; i < MaxLenth; i++) {
        Num_Tmp[i] = 0;
    }
    
    /* 检查结束或错误 */
    if (s->Str[s->Str_Ptr] == '\0' || ErrorCode) {
        return TYPE_INVALID;
    }
    if (s->Str[s->Str_Ptr] == ':' ) {
        tk->Array_Ptr = 0; //丢弃以前的所有东西，重开吧 
         s->Str_Ptr++;
        return TYPE_FUNCTION;
    }  
    /* 跳过空白字符 */
    
    while (s->Str[s->Str_Ptr] == ' ' || s->Str[s->Str_Ptr] == '\t') {
        s->Str_Ptr++;
    }
    
    current_char = s->Str[s->Str_Ptr];
    
    
    /* 1. 处理数字 */
    if (isdigit(current_char) || current_char == '.') {
        Token_Tmp.type = TYPE_CONSTANT;
        /* 读取数字部分 */
        while (isdigit(s->Str[s->Str_Ptr]) || s->Str[s->Str_Ptr] == '.' || 
               s->Str[s->Str_Ptr] == 'd') {
            
            /* 检查小数点计数 */
            if (s->Str[s->Str_Ptr] == '.') {
                Dot_Flg++;
                if (Dot_Flg > 1) {
                    ErrorCode = 0x01;
                    return TYPE_INVALID;
                }
            }
            
            /* 检查角度标识 */
            if (s->Str[s->Str_Ptr] == 'd') {
                Degree_Flg++;
                if (Degree_Flg > 1) {
                    ErrorCode = 0x01;
                    return TYPE_INVALID;
               	}
            }
            
            Num_Tmp[Num_Ptr++] = s->Str[s->Str_Ptr++];
        }
        
        /* 错误检查 */
        if (Degree_Flg && Num_Tmp[Num_Ptr - 1] != 'd') {
            ErrorCode = 0x01;
            return TYPE_INVALID;
        }
        
        /* 转换为数值 */
        Num_Tmp[Num_Ptr] = '\0';
        if (Degree_Flg) {
        	Token_Tmp.type = TYPE_CONSTANT;
            Num_Tmp[Num_Ptr - 1] = '\0'; // 去掉'd'
            Token_Tmp.value =  atof((char*)Num_Tmp) * Pi / 180;
        } else {
        	Token_Tmp.type = TYPE_CONSTANT;
            Token_Tmp.value = atof((char*)Num_Tmp);
        }
        
        tk->token[tk->Array_Ptr++] = Token_Tmp;
        return TYPE_CONSTANT;
    }
    
    /* 2. 处理常量 e 和 pi */
    if (current_char == 'e' || current_char == 'P') {
        Token_Tmp.type = TYPE_CONSTANT;
        if (current_char == 'e') {
            Token_Tmp.value = Exp;
            s->Str_Ptr += 1;
        } else if (current_char == 'P' && s->Str[s->Str_Ptr + 1] == 'i') {
            Token_Tmp.value = Pi;
            s->Str_Ptr += 2;
        } else {
            ErrorCode = 0x03;
            return TYPE_INVALID;
        }
        tk->token[tk->Array_Ptr++] = Token_Tmp;
        return TYPE_CONSTANT;
    }
    /* 2. 处理常量 Ans */
    /*
    if (current_char == 'a') {
        Token_Tmp.type = TYPE_NUMBER;
        if (s->Str[s->Str_Ptr + 1] == 'n'&&s->Str[s->Str_Ptr + 2] == 's') {
            Token_Tmp.value = Ans;
            s->Str_Ptr += 3;
        }else{
            ErrorCode = 0x03;
            return TYPE_INVALID;
        }
        tk->token[tk->Array_Ptr++] = Token_Tmp;
        return TYPE_NUMBER;
    }
	*/
	
    /* 3. 处理变量（大写字母） */
    if ((current_char >= 'A' && current_char <= 'F') || 
        (current_char >= 'I' && current_char <= 'K') ||
		(current_char >= 'U' && current_char <= 'Z')  ) {
        Token_Tmp.type = TYPE_VARIABLE;
        
        if(current_char >= 'A' && current_char <= 'F'){
        	Token_Tmp.value =  current_char - 'A' ;
		}
		
		/*
		else if(current_char >= 'I' && current_char <= 'K' ){
			Token_Tmp.value = 
		}else if(current_char >= 'U' && current_char <= 'Z'){
			Token_Tmp.value = 
		}
        */
        s->Str_Ptr += 1;
        tk->token[tk->Array_Ptr++] = Token_Tmp;
        return TYPE_VARIABLE;
    }
    
    /* 4. 处理运算符 */
    if (strchr("+-*/^(),", current_char)) {
        Token_Tmp.type = TYPE_OPERATOR;
        
        /* 处理一元正负号 */
        if (current_char == '-' || current_char == '+') {
            if (s->Str_Ptr == 0 || s->Str[s->Str_Ptr - 1] == '(') {
                /* 插入0作为一元操作数的左操作数 */
                Cal_Token zero_token;
                zero_token.type = TYPE_CONSTANT;
                zero_token.value = 0;
                tk->token[tk->Array_Ptr++] = zero_token;
            }
        }
        
        /* 设置运算符值 */
        switch (current_char) {
            case '+': Token_Tmp.value = ADD_FLAG; break;
            case '-': Token_Tmp.value = MIN_FLAG; break;
            case '*': Token_Tmp.value = MUL_FLAG; break;
            case '/': Token_Tmp.value = DIV_FLAG; break;
            case '^': Token_Tmp.value = POW_FLAG; break;
            case '(': Token_Tmp.value = PLEFT_FLAG; break;
            case ')': Token_Tmp.value = PRIGHT_FLAG; break;
            case ',': Token_Tmp.value = COMMA_FLAG; break;
            default: 
                ErrorCode = 0x03;
                return TYPE_INVALID;
        }
        
        s->Str_Ptr++;
        tk->token[tk->Array_Ptr++] = Token_Tmp;
        return TYPE_OPERATOR;
    }
    
    /* 5. 处理函数（小写字母） */
    if (isalpha(current_char)) {
        Token_Tmp.type = TYPE_FUNCTION;
        
        /* 读取函数名（最多7个字符） */
        name_len = 0;
        for (i = 0; i < 10; i++) func_name[i] = 0;
        
        while (isalpha(s->Str[s->Str_Ptr]) && name_len < 7) {
            func_name[name_len++] = s->Str[s->Str_Ptr++];
        }
        func_name[name_len] = '\0';
        
        /* 根据函数名设置函数标志 */
        if (strcmp((char*)func_name, "sin") == 0) {
            Token_Tmp.value = SIN_FLAG;
        } else if (strcmp((char*)func_name, "cos") == 0) {
            Token_Tmp.value = COS_FLAG;
        } else if (strcmp((char*)func_name, "tan") == 0) {
            Token_Tmp.value = TAN_FLAG;
        } else if (strcmp((char*)func_name, "asin") == 0) {
            Token_Tmp.value = ASIN_FLAG;
        } else if (strcmp((char*)func_name, "acos") == 0) {
            Token_Tmp.value = ACOS_FLAG;
        } else if (strcmp((char*)func_name, "atan") == 0) {
            Token_Tmp.value = ATAN_FLAG;
        } else if (strcmp((char*)func_name, "abs") == 0) {
            Token_Tmp.value = ABS_FLAG;
        } else if (strcmp((char*)func_name, "sinh") == 0) {
            Token_Tmp.value = SINH_FLAG;
        } else if (strcmp((char*)func_name, "cosh") == 0) {
            Token_Tmp.value = COSH_FLAG;
        } else if (strcmp((char*)func_name, "tanh") == 0) {
            Token_Tmp.value = TANH_FLAG;
        } else if (strcmp((char*)func_name, "ln") == 0) {
            Token_Tmp.value = LN_FLAG;
        } else if (strcmp((char*)func_name, "lg") == 0) {
            Token_Tmp.value = LG_FLAG;
        } else if (strcmp((char*)func_name, "log") == 0) {
            Token_Tmp.value = LG_FLAG;
        } else if (strcmp((char*)func_name, "rand") == 0) {
            Token_Tmp.value = RAND_FLAG;
        } else if (strcmp((char*)func_name, "random") == 0) {
            Token_Tmp.value = RANDOM_FLAG;
        } else if (strcmp((char*)func_name, "fact") == 0) {
            Token_Tmp.value = FACTORIAL_FLAG;
        } else if (strcmp((char*)func_name, "rv") == 0) {
            Token_Tmp.value = READVARIOUS_FLAG;
        }  else if (strcmp((char*)func_name, "wv") == 0) {
            Token_Tmp.value = WRITEVARIOUS_FLAG;
        }  else if (strcmp((char*)func_name, "ad") == 0) {
            Token_Tmp.value = ADDRESS_FLAG;
        }  else if (strcmp((char*)func_name, "jz") == 0) {
            Token_Tmp.value = JUDGEZEROJMP_FLAG;
        }  else if (strcmp((char*)func_name, "jnz") == 0) {
            Token_Tmp.value = JUDGENONEZEROJMP_FLAG;
        } else if (strcmp((char*)func_name, "cmp") == 0) {
            Token_Tmp.value = COMPARE_FLAG;
        } else if (strcmp((char*)func_name, "put") == 0) {
            Token_Tmp.value = PUT_FLAG;
        } else if (strcmp((char*)func_name, "get") == 0) {
            Token_Tmp.value = GET_FLAG;
        } else if (strcmp((char*)func_name, "wait") == 0) {
            Token_Tmp.value = WAIT_FLAG;
        } else if (strcmp((char*)func_name, "clr") == 0) {
            Token_Tmp.value = CLEAR_FLAG;
        } else if (strcmp((char*)func_name, "fls") == 0) {
            Token_Tmp.value = FLUSH_FLAG;
        } else if (strcmp((char*)func_name, "dis") == 0) {
            Token_Tmp.value = DISPLAY_FLAG;
        } else if (strcmp((char*)func_name, "dpt") == 0) {
            Token_Tmp.value = DRAWPOINT_FLAG;
        } else if (strcmp((char*)func_name, "gpt") == 0) {
            Token_Tmp.value = GETPOINT_FLAG;
        } else if (strcmp((char*)func_name, "tick") == 0) {
            Token_Tmp.value = TICK_FLAG;
        }  else{
            /* 未知函数名 */
            ErrorCode = 0x03;
            return TYPE_INVALID;
        }
        
        tk->token[tk->Array_Ptr++] = Token_Tmp;
        return TYPE_FUNCTION;
    }
    
    /* 未知字符 */
    ErrorCode = 0x03;
    return TYPE_INVALID;
}

/*
uint8_t Cal_Get_Token(Cal_String *s, Cal_Array *tk)
{
	Cal_Token Token_Tmp;
	uint8_t Num_Tmp[MaxLenth], Num_Ptr = 0, i = 0; // 数字缓存数组 和它的指针
	uint8_t Dot_Flg = 0;						   // 用于.的计数，以判断奇葩的...错误
	uint8_t Degree_Flg = 0;						   // 度数指示：输入的值是否是角度值
	Token_Tmp.type = 0;
	Token_Tmp.value = 0;
	for (i = 0; i < MaxLenth; i++)
	{
		Num_Tmp[i] = 0;
	}
	if (s->Str[s->Str_Ptr] == '\0' || ErrorCode)
		return TYPE_INVALID;									// 结束了
	if (s->Str[s->Str_Ptr] >= '0' && s->Str[s->Str_Ptr] <= '9') // 如果是数字
	{
		for (i = 0; i < MaxLenth; i++)
			Num_Tmp[i] = 0; // 清空一波
		Num_Tmp[Num_Ptr++] = s->Str[s->Str_Ptr++];
		if (s->Str[s->Str_Ptr] == '.')
			Dot_Flg++;
		if (s->Str[s->Str_Ptr] == 'd')
			Degree_Flg++;
		while ((s->Str[s->Str_Ptr] >= '0' && s->Str[s->Str_Ptr] <= '9') || s->Str[s->Str_Ptr] == '.' || s->Str[s->Str_Ptr] == 'd')
		{
			// cout<<num[p];

			Num_Tmp[Num_Ptr++] = s->Str[s->Str_Ptr++];
			if (s->Str[s->Str_Ptr] == '.')
				Dot_Flg++;
			if (s->Str[s->Str_Ptr] == 'd')
				Degree_Flg++;
			if (Dot_Flg > 1 || Degree_Flg > 1)
			{ // 如果有多个d或多个. 肯定输入有错
				// cout<<"Syntax Error 1 1";
				// exit(0);
				ErrorCode = 0x01;
				return 0;
			}
		}
		if (Num_Tmp[Num_Ptr - 1] != 'd' && Degree_Flg)
		{ // 如果输入了d，d却不是字符串末尾,就说明输入有问题 例："sin(30d1)" -> Error 0x01
			ErrorCode = 0x01;
			return 0;
		}
		Token_Tmp.type = TYPE_NUMBER;
		if (Degree_Flg)
			Token_Tmp.value = strtod((const char*)Num_Tmp, NULL) * Pi / 180; // 角度
		else
			Token_Tmp.value = strtod((const char*)Num_Tmp, NULL); // 数值
		tk->token[tk->Array_Ptr++] = Token_Tmp;
		return TYPE_NUMBER;
	}
	if (s->Str[s->Str_Ptr] == 'e' || s->Str[s->Str_Ptr] == 'p')
	{
		Token_Tmp.type = TYPE_NUMBER;
		if (s->Str[s->Str_Ptr] == 'e')
		{
			Token_Tmp.value = Exp;
			s->Str_Ptr += 1;
			tk->token[tk->Array_Ptr++] = Token_Tmp;
			return TYPE_NUMBER;
		}
		if (s->Str[s->Str_Ptr] == 'p')
		{
			Token_Tmp.value = Pi;
			s->Str_Ptr += 2;
			tk->token[tk->Array_Ptr++] = Token_Tmp;
			return TYPE_NUMBER;
		}
		/*
		if (s->Str[s->Str_Ptr] == 'A')
		{
			Token_Tmp.value = Ans;
			s->Str_Ptr += 3;
			tk->token[tk->Array_Ptr++] = Token_Tmp;
			return TYPE_NUMBER;
		}
		*//*
	}
	
	
	if (s->Str[s->Str_Ptr] == 'u' || s->Str[s->Str_Ptr] == 'v' || s->Str[s->Str_Ptr] == 'w' || s->Str[s->Str_Ptr] == 'x' || s->Str[s->Str_Ptr] == 'y' || s->Str[s->Str_Ptr] == 'z')
	{
		Token_Tmp.type = TYPE_PARAMETER;
		Token_Tmp.value = s->Str[s->Str_Ptr] - (int)'u' + 1;
		s->Str_Ptr += 1;
		tk->token[tk->Array_Ptr++] = Token_Tmp;
		return TYPE_PARAMETER;
	}
	*/
	/*
	if ((s->Str[s->Str_Ptr] >= 'A' && s->Str[s->Str_Ptr] <= 'K')|| (s->Str[s->Str_Ptr] >= 'X' && s->Str[s->Str_Ptr] <= 'Z'))
	{
		Token_Tmp.type = TYPE_VARIABLE;
		Token_Tmp.value = (int)s->Str[s->Str_Ptr]  ;
		s->Str_Ptr += 1;
		tk->token[tk->Array_Ptr++] = Token_Tmp;
		return TYPE_VARIABLE;
	}
	
	
	
	
	
	if (s->Str[s->Str_Ptr] == '-' || s->Str[s->Str_Ptr] == '+' || s->Str[s->Str_Ptr] == '*' || s->Str[s->Str_Ptr] == '/' || s->Str[s->Str_Ptr] == '^' || s->Str[s->Str_Ptr] == '(' || s->Str[s->Str_Ptr] == ')' || s->Str[s->Str_Ptr] == ',')
	{
		if (s->Str[s->Str_Ptr] == '-' || s->Str[s->Str_Ptr] == '+')
		{
			if (s->Str_Ptr == 0 || s->Str[s->Str_Ptr - 1] == '(')
			{
				Token_Tmp.type = TYPE_NUMBER;
				Token_Tmp.value = 0; // 补一个0
				tk->token[tk->Array_Ptr++] = Token_Tmp;
			}
			Token_Tmp.type = TYPE_OPERATOR;
			if (s->Str[s->Str_Ptr] == '+')
				Token_Tmp.value = ADD_FLAG;
			if (s->Str[s->Str_Ptr] == '-')
				Token_Tmp.value = MIN_FLAG;
			tk->token[tk->Array_Ptr++] = Token_Tmp;
			s->Str_Ptr++;
		}
		else
		{
			Token_Tmp.type = TYPE_OPERATOR;
			if (s->Str[s->Str_Ptr] == '*')
				Token_Tmp.value = MUL_FLAG;
			if (s->Str[s->Str_Ptr] == '/')
				Token_Tmp.value = DIV_FLAG;
			if (s->Str[s->Str_Ptr] == '^')
				Token_Tmp.value = POW_FLAG;
			if (s->Str[s->Str_Ptr] == '(')
				Token_Tmp.value = PLEFT_FLAG;
			if (s->Str[s->Str_Ptr] == ')')
				Token_Tmp.value = PRIGHT_FLAG;
			if (s->Str[s->Str_Ptr] == ',')
				Token_Tmp.value = COMMA_FLAG;
			tk->token[tk->Array_Ptr++] = Token_Tmp;
			s->Str_Ptr++;
		}
		return TYPE_OPERATOR;
	}
	
	if (s->Str[s->Str_Ptr] == 'a' || s->Str[s->Str_Ptr] == 's' || s->Str[s->Str_Ptr] == 'c' || s->Str[s->Str_Ptr] == 't' || s->Str[s->Str_Ptr] == 'l' || s->Str[s->Str_Ptr] == 'w' || s->Str[s->Str_Ptr] == 'r' || s->Str[s->Str_Ptr] == 'f')
	{
		Token_Tmp.type = TYPE_FUNCTION;
		if (s->Str[s->Str_Ptr] == 'a')
		{
			if (s->Str[s->Str_Ptr + 1] == 'c')
			{
				Token_Tmp.value = ACOS_FLAG;
				s->Str_Ptr += 4;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
			if (s->Str[s->Str_Ptr + 1] == 's')
			{
				Token_Tmp.value = ASIN_FLAG;
				s->Str_Ptr += 4;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
			if (s->Str[s->Str_Ptr + 1] == 't')
			{
				Token_Tmp.value = ATAN_FLAG;
				s->Str_Ptr += 4;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
			if (s->Str[s->Str_Ptr + 1] == 'b')
			{
				Token_Tmp.value = ABS_FLAG;
				s->Str_Ptr += 3;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
		}
		if (s->Str[s->Str_Ptr] == 's')
		{
			if (s->Str[s->Str_Ptr + 3] == 'h')
			{
				Token_Tmp.value = SINH_FLAG;
				s->Str_Ptr += 4;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
			else
			{
				Token_Tmp.value = SIN_FLAG;
				s->Str_Ptr += 3;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
		}
		if (s->Str[s->Str_Ptr] == 'c')
		{
			if (s->Str[s->Str_Ptr + 3] == 'h')
			{
				Token_Tmp.value = COSH_FLAG;
				s->Str_Ptr += 4;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
			else
			{
				Token_Tmp.value = COS_FLAG;
				s->Str_Ptr += 3;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
		}
		if (s->Str[s->Str_Ptr] == 't')
		{
			if (s->Str[s->Str_Ptr + 3] == 'h')
			{
				Token_Tmp.value = TANH_FLAG;
				s->Str_Ptr += 4;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
			else
			{
				Token_Tmp.value = TAN_FLAG;
				s->Str_Ptr += 3;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
		}
		if (s->Str[s->Str_Ptr] == 'l')
		{
			if (s->Str[s->Str_Ptr + 1] == 'n')
			{
				Token_Tmp.value = LN_FLAG;
				s->Str_Ptr += 2;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
			if (s->Str[s->Str_Ptr + 1] == 'g')
			{
				Token_Tmp.value = LG_FLAG;
				s->Str_Ptr += 2;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
		}
		if (s->Str[s->Str_Ptr] == 'r')
		{
			if (s->Str[s->Str_Ptr + 4] == 'o')
			{ // random()
				Token_Tmp.value = RANDOM_FLAG;
				s->Str_Ptr += 6;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
			else
			{ // rand()
				Token_Tmp.value = RAND_FLAG;
				s->Str_Ptr += 4;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
		}
		if (s->Str[s->Str_Ptr] == 'f')
		{
			if (s->Str[s->Str_Ptr + 1] == 'a')
			{ // fact()
				Token_Tmp.value = FACTORIAL_FLAG;
				s->Str_Ptr += 4;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
		}
		if (s->Str[s->Str_Ptr] == 'w')
		{
			if (s->Str[s->Str_Ptr + 1] == 'r')
			{ // fact()
				Token_Tmp.value = WRITEVARIOUS_FLAG;
				s->Str_Ptr += 2;
				tk->token[tk->Array_Ptr++] = Token_Tmp;
				return TYPE_FUNCTION;
			}
		}
	}
	// cout<<"Syntax Error 0"; //存在错误字符
	// exit(0);
	ErrorCode = 0x03;
	return TYPE_INVALID;
}

*/





// 函数名：Cal_cmp(Cal_Token a,Cal_Token b)
// 传入参数：两个token a和b
// 功能：如果a和b均满足条件，即都是算符或者函数，比较其优先级大小
// 返回：见函数内注释
uint8_t Cal_cmp(Cal_Token a, Cal_Token b)
{
	int pa = 0, pb = 0;
	if (a.type <= TYPE_VARIABLE || b.type <= TYPE_VARIABLE || ErrorCode)
		return CMP_INVALID; // invalid
	if (a.type == TYPE_OPERATOR)
	{
		if (a.value == ADD_FLAG || a.value == MIN_FLAG)
			pa = PRIORITY2; //   +-
		else if (a.value == MUL_FLAG || a.value == DIV_FLAG)
			pa = PRIORITY3; //  */
		else if (a.value == POW_FLAG)
			pa = PRIORITY4; // ^
		else
			return CMP_INVALID;
	}
	if (b.type == TYPE_OPERATOR)
	{
		if (b.value == ADD_FLAG || b.value == MIN_FLAG)
			pb = PRIORITY2; //   +-
		else if (b.value == MUL_FLAG || b.value == DIV_FLAG)
			pb = PRIORITY3; //  */
		else if (b.value == POW_FLAG)
			pb = PRIORITY4; // ^
		else
			return CMP_INVALID;
	}
	if (a.type == TYPE_FUNCTION)
	{
		pa = PRIORITY5;
	}

	if (b.type == 4)
	{
		pb = PRIORITY5;
	}
	if (pa > pb)
		return CMP_LARGER; // a>b
	if (pa == pb)
		return CMP_SAME; // a==b;
	if (pa < pb)
		return CMP_SMALLER; // a<b
	return CMP_INVALID;		// invalid
}
// 函数名：Cal_Mid2Post(Cal_Array *Tokens,Cal_Array *Post,float *Para)
// 传入参数：*Token是一堆刚解析出来的新鲜token序列 *Post存储处理得到的后缀式，即逆波兰式，*Para是参数表
// 功能：逐个扫描*Tokens,将token序列转为后缀式序列存储在*Post中
// 返回 0:正常 0xff异常
uint8_t Cal_Mid2Post(Cal_Array *Tokens, Cal_Array *Post)
{
	Cal_Stack Stk;
	Cal_Token Tmp;
	uint8_t i;
	Stk.Stack_Ptr = 0;
	Post->Array_Ptr = 0;
	for (i = 0; i < MaxLenth; i++)
	{
		Stk.token[i].type = 0;
		Stk.token[i].value = 0;
		Post->token[i].type = 0;
		Post->token[i].value = 0;
	}
	Tmp.value = 0;
	Tmp.type = 0;
	// cout<<tk_ptr;
	if (ErrorCode)
		return 0xff;
	for (i = 0; i < Tokens->Array_Ptr; i++)
	{
		if (Tokens->token[i].type == TYPE_CONSTANT)
		{
			Post->token[Post->Array_Ptr++] = Tokens->token[i];
			// cout<<"hh"<<(int)Tokens->token[i].type<<" "<<(int)Tokens->token[i].value<<endl;
			// cout<<Math_Const[0];
		}
		else if (Tokens->token[i].type == TYPE_VARIABLE)
		{ // 遇到变量，直接把其地址扔进逆波兰式，不作处理
			Tmp.type = TYPE_VARIABLE;
			Tmp.value = Tokens->token[i].value;
			Post->token[Post->Array_Ptr++] = Tmp;
		}
		else if (Tokens->token[i].type == TYPE_OPERATOR || Tokens->token[i].type == TYPE_FUNCTION)
		{
			if (Tokens->token[i].value == PLEFT_FLAG && Tokens->token[i].type == TYPE_OPERATOR)
			{ // 如果是'('直接入栈
				Stack_Push(&Stk, Tokens->token[i]);
			}
			else if (Tokens->token[i].value == PRIGHT_FLAG && Tokens->token[i].type == TYPE_OPERATOR)
			{ // 如果是')'，把元素退栈到pst里直到遇到'('，弹出并丢弃 '('
				while (Stack_Get_Top(&Stk).type != TYPE_OPERATOR || Stack_Get_Top(&Stk).value != PLEFT_FLAG)
				{
					Post->token[Post->Array_Ptr++] = Stack_Get_Top(&Stk);
					Stack_Pop(&Stk);
					if (Stack_IsEmpty(&Stk))
					{
						// cout<<"Syntax Error 1 2";//括号匹配失败
						// exit(0);
						ErrorCode = 0x02;
						return 0xff;
					}
				}
				Stack_Pop(&Stk); // 弹出')'
			}
			else if (Tokens->token[i].value == COMMA_FLAG && Tokens->token[i].type == TYPE_OPERATOR)
			{ // 如果是','，把元素退栈到pst里直到遇到'('，不弹出'('
				while (Stack_Get_Top(&Stk).type != TYPE_OPERATOR || Stack_Get_Top(&Stk).value != PLEFT_FLAG)
				{
					Post->token[Post->Array_Ptr++] = Stack_Get_Top(&Stk);
					Stack_Pop(&Stk);
					if (Stack_IsEmpty(&Stk))
					{
						// cout<<"Syntax Error 1 3";//括号匹配失败
						// exit(0);
						ErrorCode = 0x02;
						return 0xff;
					}
				}
			}
			else
			{
				if ((Tokens->token[i].type == TYPE_OPERATOR && Stack_Get_Top(&Stk).value == PLEFT_FLAG) || Stack_IsEmpty(&Stk))
				{
					Stack_Push(&Stk, Tokens->token[i]);
				}
				else
				{
					while ((Cal_cmp(Tokens->token[i], Stack_Get_Top(&Stk)) == CMP_SMALLER || Cal_cmp(Tokens->token[i], Stack_Get_Top(&Stk)) == CMP_SAME) && (!Stack_IsEmpty(&Stk)))
					{
						Post->token[Post->Array_Ptr++] = Stack_Get_Top(&Stk);
						Stack_Pop(&Stk);
					}
					Stack_Push(&Stk, Tokens->token[i]);
				}
			}
		}
	}
	while (!Stack_IsEmpty(&Stk))
	{
		Post->token[Post->Array_Ptr++] = Stack_Get_Top(&Stk);
		Stack_Pop(&Stk);
	}
	return 0;
}








//从float变量地址读取值 
float Read_Memory(Cal_Token tk){
	if(tk.type == TYPE_CONSTANT){ //直接寻址 
		return tk.value;
	}else if(tk.type == TYPE_VARIABLE){ //直接寻址 
		return Variable[(int)tk.value];
	}else{
		ErrorCode = 0x03;
		return 0;		
	}		
}

//向float变量地址写入值 
float Write_Memory(Cal_Token tk, float val){
	if(tk.type == TYPE_CONSTANT){
		ErrorCode = 0x08;//写入常量,寄 
		return 0; 
		//Math_Const[tk.value] = val;
	}else if(tk.type == TYPE_VARIABLE){
		Variable[(int)tk.value] = val;
	}else{
		ErrorCode = 0x03;
		return 0;		
	}	
	return val;
}



// 函数名：Cal_Calculate_RPN(Cal_Array *Post,float *Para)
// 传入参数：*Post是一堆刚转换出来的新鲜逆波兰式，*Para是参数表
// 功能：把逆波兰式算出数值
// 注：参数表内依次存有负责替换token中uvw xyz的数值
// 返回：计算结果
float Cal_Calculate_RPN(Cal_Array *Post)
{
	Cal_Stack Stk;
	Cal_Token numa, numb, numc, tmp, res;
	float resnum;
	float tmpa,tmpb, tmpc;
    float vnow = 0;
	uint8_t i;
	Stk.Stack_Ptr = 0;
	if (ErrorCode)
		return 0;
	for (i = 0; i < MaxLenth; i++)
	{
		Stk.token[i].type = 0;
		Stk.token[i].value = 0;
	}
	if (Post->Array_Ptr == 0)
	{
		// cout<<"Syntax Error 2";
		// exit(0);
		ErrorCode = 0x04;
		return 0;
	}
	Stk.Stack_Ptr = 0;
	
	for (i = 0; i < Post->Array_Ptr; i++)
	{
		
		if (Post->token[i].type == TYPE_CONSTANT || Post->token[i].type == TYPE_VARIABLE)
		{
			Stack_Push(&Stk, Post->token[i]);
		}
		if (Post->token[i].type == TYPE_OPERATOR)
		{
			if (Stk.Stack_Ptr < 2)
			{
				// cout<<"Syntax Error 3";
				// exit(0);
				ErrorCode = 0x05;
				return 0;
			}
			res.type = TYPE_TEMP;
			if (Post->token[i].value == ADD_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   Read_Memory(numb) + Read_Memory(numa);
				//cout<<"ADD"<<" "<<Read_Memory(numb)<<" "<<Read_Memory(numa)<<endl;
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);		
			}
			else if (Post->token[i].value == MIN_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   Read_Memory(numb) - Read_Memory(numa);
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);		
			}
			else if (Post->token[i].value == MUL_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   Read_Memory(numb) * Read_Memory(numa);
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);		
			}
			else if (Post->token[i].value == DIV_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   Read_Memory(numb) / Read_Memory(numa);
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);		
			}
			else if (Post->token[i].value == POW_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   pow(Read_Memory(numb) , Read_Memory(numa));
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);		
			}
			else
			{
				// cout<<"Unknown Error";
				// exit(0);
				ErrorCode = 0x06;
				return 0;
			}
		}
		if (Post->token[i].type == TYPE_FUNCTION)
		{
			res.type = TYPE_TEMP;
			if (Stk.Stack_Ptr < 1 && ((Post->token[i].value >= 1 && Post->token[i].value <= 12) || Post->token[i].value == 15))
			{ // 1元函数
				// cout<<"Syntax Error 3 1";
				// exit(0);
				ErrorCode = 0x05;
				return 0;
			}
			if (Stk.Stack_Ptr < 2 && (Post->token[i].type == 14))
			{ // 2元函数
				// cout<<"Syntax Error 3 2";
				// exit(0);
				ErrorCode = 0x05;
				return 0;
			}
			// 13是0元函数
			if (Post->token[i].value == SIN_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				tmpa = Read_Memory(numa);
				//数值操作 
				resnum =   sin(tmpa);
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				//cout<<"sin"<<" "<< resnum<<endl;
				Stack_Push(&Stk, res);		
			}
			else if (Post->token[i].value == COS_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   cos(Read_Memory(numa));
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == TAN_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   tan(Read_Memory(numa));
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == ASIN_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   asin(Read_Memory(numa));
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == ACOS_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   acos(Read_Memory(numa));
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == ATAN_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   atan(Read_Memory(numa));
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == ABS_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   abs(Read_Memory(numa));
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == SINH_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   sinh(Read_Memory(numa));
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == COSH_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   cosh(Read_Memory(numa));
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == TANH_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   tanh(Read_Memory(numa));
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == LN_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   log(Read_Memory(numa)); 
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == LG_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   log(Read_Memory(numa))/ln10;
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == RAND_FLAG)
			{
				//numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//数值操作 
				resnum =   (float)rand()/32768.0;
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == RANDOM_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				tmpa = Read_Memory(numa);
				tmpb = Read_Memory(numb);
				if (tmpa <= tmpb)
				{
					// cout<<"Math Error 1";
					// exit(0);
					ErrorCode = 0x07;
					return 0;
				}
				if(tmpa != (int)tmpa || tmpb != (int)tmpb){
					ErrorCode = 0x07;
					return 0;		
				}
				//数值操作 
				resnum = rand() % (((int)tmpa - (int)tmpb + 1)) + (int)tmpb;
				
				res.type = TYPE_CONSTANT;
				res.value = resnum;	
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == FACTORIAL_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				//数值操作
				tmpa = Read_Memory(numa);
				
				if (tmpa!= (int)tmpa)
				{
					// cout<<"Math Error 1";
					// exit(0);
					ErrorCode = 0x07;
					return 0;
				}		
				
				resnum = 1;
				tmpa = (int)tmpa;
				while (tmpa >= 1)
				{
					resnum *= tmpa;
					tmpa--;
				}			
				

				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);	
			}
			else if (Post->token[i].value == READVARIOUS_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				resnum = Read_Memory(numa);

				res.type = TYPE_CONSTANT;
				res.value = resnum; 

				Stack_Push(&Stk, res);
				
			}	
			else if (Post->token[i].value == WRITEVARIOUS_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				tmpa = Read_Memory(numa); //要写入的数值 
				//数值操作
				resnum = Write_Memory(numb,tmpa); //写变量 
							
				res.type = TYPE_CONSTANT;
				res.value = resnum;		

				Stack_Push(&Stk, res);
				//数值操作

			}	
			
			else if (Post->token[i].value == ADDRESS_FLAG)
			{
				
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				tmpa = Read_Memory(numa);
				resnum = tmpa;
				
				res.type = TYPE_VARIABLE;
				res.value = resnum;

				Stack_Push(&Stk, res);
			}	
		
			
			else if (Post->token[i].value == JUDGEZEROJMP_FLAG)
			{
				
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numc = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				
				tmpa = Read_Memory(numa); //误差 
				tmpb = Read_Memory(numb); //行标 
				tmpc = Read_Memory(numc); //判定值 
				resnum = tmpb;
				
				if(fabs(tmpc) <= tmpa){
					if(tmpb <= 0){
						ErrorCode = 0x08;
						return 0; 
					}
					Set_NowLine((tmpb - 1));
				}else{
					
				}
				
							
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);		
			}	
			else if (Post->token[i].value == JUDGENONEZEROJMP_FLAG)
			{
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numc = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				
				tmpa = Read_Memory(numa); //误差 
				tmpb = Read_Memory(numb); //行标 
				tmpc = Read_Memory(numc); //判定值 
				resnum = tmpb;
				
				if(fabs(tmpc) > tmpa){
					if(tmpb <= 0){
						ErrorCode = 0x08;
						return 0; 
					}
					Set_NowLine((tmpb - 1));
				}else{
					
				}
	
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);		
			}
			else if (Post->token[i].value == COMPARE_FLAG)
			{
					
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numc = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				tmpa = Read_Memory(numa); //mode 
				tmpb = Read_Memory(numb); //val2
				tmpc = Read_Memory(numc); //val1
				
				resnum = tmpb;

				switch((uint8_t)tmpa){
					case 0: resnum = (uint8_t)(tmpc < tmpb ? 1:0);break;
					case 1: resnum = (uint8_t)(tmpc <= tmpb ? 1:0);break;
					case 2: resnum = (uint8_t)(tmpc > tmpb ? 1:0);break;
					case 3: resnum = (uint8_t)(tmpc >= tmpb ? 1:0);break;
					case 4: resnum = (uint8_t)(tmpc == tmpb ? 1:0);break;
				}
				//cout<<"res"<<resnum;
				//常量折叠 
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);		
			}	
			
			else if (Post->token[i].value == PUT_FLAG)
			{
					//cout<<"ggggg";
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				tmpa = Read_Memory(numa); //value
				
				resnum = tmpa;

				OLED_Printf(0,Text_Line * 2,16,"%f ",resnum);
                Text_Line ++ ;
                if(Text_Line > 3) Text_Line = 0;
				//cout<<"res"<<resnum;
				//常量折叠 
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);		
			}	
			else if (Post->token[i].value == GET_FLAG)
			{
					//cout<<"ggggg";
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				//float vnow;
				//cin>>vnow
                vnow = Input_Num(Text_Line);
				resnum = Write_Memory(numa,vnow);
				
				//resnum = tmpa;
				//cout <<"ddd"<<tmpc<<" "<<tmpb<<" "<<tmpa<<endl;
				//寄存器优化
				//cout<< tmpa<<" "<<tmpb<<" "<<tmpc<<endl;

				//cout<<"res"<<resnum;
				//常量折叠 
						
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);		
			}	
			else if (Post->token[i].value == WAIT_FLAG)
			{
					//cout<<"ggggg";
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				tmpa = Read_Memory(numa); //value
				
                
				__Sleep(tmpa);
				
				resnum = tmpa;

				//cout<<"res"<<resnum;
				//常量折叠 
						
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);		
			}
			else if (Post->token[i].value == CLEAR_FLAG)
			{
					//cout<<"ggggg";
				//numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				//tmpa = Read_Memory(numa); //value
				
                Gui_ClearBufferBit(&pMain,0);
                //Gui_Flush(&pMain);
				//__Sleep(tmpa);
				
				resnum = 0;

				//cout<<"res"<<resnum;
				//常量折叠 
						
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);		
			}		
			else if (Post->token[i].value == FLUSH_FLAG)
			{
					//cout<<"ggggg";
				//numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				
				//tmpa = Read_Memory(numa); //value
				
                //Gui_ClearBufferBit(&pMain,0);
                Gui_Flush(&pMain);
				//__Sleep(tmpa);
				
				resnum = 0;

				//cout<<"res"<<resnum;
				//常量折叠 
						
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);			
			}		
			else if (Post->token[i].value == DISPLAY_FLAG)
			{
					//cout<<"ggggg";
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);  //value
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);  //y
                numc = Stack_Get_Top(&Stk); Stack_Pop(&Stk);  //x

				tmpa = Read_Memory(numa); //value
				tmpb = Read_Memory(numb); // y
                tmpc = Read_Memory(numc); // x
                Gui_Printf_OneLine(&pMain,tmpc,tmpb,1,0,"%f",tmpa);
				//__Sleep(tmpa);
				
				resnum = tmpa;

				//cout<<"res"<<resnum;
				//常量折叠 
						
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);		
			}		
			else if (Post->token[i].value == DRAWPOINT_FLAG)
			{
					//cout<<"ggggg";
				//numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);  //y
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);  //x
				tmpa = Read_Memory(numa); // y
				tmpb = Read_Memory(numb); // x
				//tmpa = Read_Memory(numa); //value
				Gui_DrawPoint(&pMain,tmpb,tmpa,1);
                
				//__Sleep(tmpa);
				
				resnum = 0;

				//cout<<"res"<<resnum;
				//常量折叠 
						
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);		
			}	
			else if (Post->token[i].value == GETPOINT_FLAG)
			{
					//cout<<"ggggg";
				//numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);  //y
				numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);  //x
				tmpa = Read_Memory(numa); // y
				tmpb = Read_Memory(numb); // x
				//tmpa = Read_Memory(numa); //value
                
				//__Sleep(tmpa);
				
				resnum = Gui_GetPoint(&pMain,tmpb,tmpa);

				//cout<<"res"<<resnum;
				//常量折叠 
						
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);		
			}	
			else if (Post->token[i].value == TICK_FLAG)
			{
					//cout<<"ggggg";
				//numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);
				//numa = Stack_Get_Top(&Stk); Stack_Pop(&Stk);  //y
				//numb = Stack_Get_Top(&Stk); Stack_Pop(&Stk);  //x
				//tmpa = Read_Memory(numa); // y
				//tmpb = Read_Memory(numb); // x
				//tmpa = Read_Memory(numa); //value
				//Gui_DrawPoint(&pMain,tmpb,tmpa,1);
                
				//__Sleep(tmpa);
				
				resnum = Tick_Get()*0.001;

				//cout<<"res"<<resnum;
				//常量折叠 
						
				res.type = TYPE_CONSTANT;
				res.value = resnum;
				Stack_Push(&Stk, res);		
			}		
		}
	}
	if (Stk.Stack_Ptr != 1)
	{
		ErrorCode = 0x06;
		return 0;
	}
	tmp = Stk.token[Stk.Stack_Ptr - 1];
	resnum = Read_Memory(tmp);
	return resnum;
}
// 函数名：Cal_UpdateAns(float ans)
// 传入参数：ans
// 功能：更新维护结果变量Ans的值
// 注：表达式中的"Ans"都会被替换为（float）Ans变量的值
// 返回：无
void Cal_UpdateAns(float ans)
{
	Ans = ans;
	return;
}
// 函数名：Calculate_Expression(char *s,uint8_t l)
// 传入参数：*s是字符串，l是其有效长度（要正确的给出！）
// 功能：顶层功能在此集成，一个函数把字符串算出数值并返回
// 返回：计算结果
void Calculate_Init(){
    uint8_t i = 0;
	ErrorCode = 0;
    for(i = 0;i < 32; i++) Variable[i] = 0;
	//Const_Clear();
	//Temp_Clear();
	Set_NowLine(1);	
}
float Calculate_Expression(char *s, uint8_t l)
{
	uint8_t i;
	float res;
	Cal_String Input;
	Cal_Array tks, Post;
	Input.Str_Ptr = 0;
	tks.Array_Ptr = 0;
	Post.Array_Ptr = 0;
	//ErrorCode = 0;
	//Const_Clear();
	//Temp_Clear();
	for (i = 0; i < MaxLenth; i++)
	{
		Input.Str[i] = 0;
		tks.token[i].value = 0;
		tks.token[i].type = 0;
		Post.token[i].value = 0;
		Post.token[i].type = 0;
	}
	for (i = 0; i < l; i++)
	{
		Input.Str[i] = s[i];
	}
	while (Cal_Get_Token(&Input, &tks));

	Cal_Mid2Post(&tks, &Post);
	res = Cal_Calculate_RPN(&Post);
	return res;
}
void Debug(){
	//printf("Memory:\n")
	//for(int i = 0; i < )
}



float Mem_Read(uint8_t addr){
	return Variable[addr];
}
void Mem_Write(uint8_t addr,float val){
	Variable[addr] = val;
	return;
}
/*
// 关于快速计算：
// 在绘制函数图像等计算密集型任务里
// 如果频繁调用Calculate_Expression
// 就意味着每次都要一口气走完  拆解Tokens->转逆波兰式->计算结果   这三步
// 显然，对于每次计算同一个表达式，前两步是多余的，可以提前做好，后边就不再做了
// 于是，就有了下面的两个函数:  (以下简称为Init和Work了)
//-------------------------------
// Init函数负责做前两步，即  拆解Tokens->转逆波兰式
// Work函数则只用做最后一步，即 代入参数计算结果
// 这样，使用时先调用一次Init,后边再重复调用Work,就会快很多！

// 函数名：Calculate_Expression_Fast_Init(char *s,uint8_t l,uint8_t FastCal_Id)
// 传入参数：*s是字符串，l是其有效长度（要正确的给出！）,FastCal_Id是快速计算通道号，从0开始且切勿超过FastCal_Cnt-1
// 功能：预处理给出的字符串，拆解tokens并转成逆波兰式，存在对应通道的外部变量里供重复利用。在后续工作函数中直接代入参数即可，起到优化作用
// 返回：无
void FastCal_Init(char *s, uint8_t l, uint8_t FastCal_Id)
{
	uint8_t i;
	FastCal_Input[FastCal_Id].Str_Ptr = 0;
	FastCal_Tokens[FastCal_Id].Array_Ptr = 0;
	FastCal_Post[FastCal_Id].Array_Ptr = 0;
	ErrorCode = 0;
	if (FastCal_Id > FastCal_Cnt - 1)
		return;
	for (i = 0; i < MaxLenth; i++)
	{
		FastCal_Input[FastCal_Id].Str[i] = 0;
		FastCal_Tokens[FastCal_Id].token[i].value = 0;
		FastCal_Tokens[FastCal_Id].token[i].type = 0;
		FastCal_Post[FastCal_Id].token[i].value = 0;
		FastCal_Post[FastCal_Id].token[i].type = 0;
	}
	for (i = 0; i < l; i++)
	{
		FastCal_Input[FastCal_Id].Str[i] = s[i];
	}
	while (Cal_Get_Token(&FastCal_Input[FastCal_Id], &FastCal_Tokens[FastCal_Id]))
		;
	Cal_Mid2Post(&FastCal_Tokens[FastCal_Id], &FastCal_Post[FastCal_Id]);
	return;
}
// 函数名：FastCal_Work(uint8_t FastCal_Id)
// 传入参数：uint8_t FastCal_Id
// 功能：代入参数计算结果
// 返回：计算结果
// 注意：在整个快速计算流程期间，重新绑定变量是无效的，因为最开始是后绑定的变量地址已经被转化进逆波兰式里了。 
float FastCal_Work(uint8_t FastCal_Id)
{
	if (FastCal_Id > FastCal_Cnt - 1)
		return 0;
	return Cal_Calculate_RPN(&FastCal_Post[FastCal_Id]);
}
*/
uint8_t Get_ErrorCode()
{
	return ErrorCode;
}
uint8_t Get_ErrorIndex(){
	return ErrorIndex;
}
