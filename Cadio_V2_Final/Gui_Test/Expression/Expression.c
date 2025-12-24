#include "Expression.h"
static uint8_t ErrorCode=0;//错误代码
static float Ans=0;//上一次的计算结果 
/*----------------------Stack-------------------*/

uint8_t Stack_IsEmpty(Cal_Stack *stk){
	if(stk->Stack_Ptr==0) return 1;
	else return 0;
}
uint8_t Stack_Push(Cal_Stack *stk,Cal_Token Element){
	if(stk->Stack_Ptr>=MaxLenth) return 0xff;//入栈错误 
	stk->token[stk->Stack_Ptr++]=Element;
	return 0;
} 

uint8_t  Stack_Pop(Cal_Stack *stk){
	if(stk->Stack_Ptr==0) return 0xff;//出栈错误 
	stk->Stack_Ptr--; 
	return 0;	
}
Cal_Token Stack_Get_Top(Cal_Stack *stk){
	Cal_Token res;
	res.type=0;
	res.value=0;
	if(stk->Stack_Ptr==0) return res;//Invalid
	res=stk->token[stk->Stack_Ptr-1]; 
	return res;
}
/*------------------------------------------------*/
//错误处理，如果抛出错误，就交给它了
void ErrorHandler(uint8_t *bf){
	if(ErrorCode==0x01) sprintf(bf,"Syntax Error %02x",ErrorCode);	
	if(ErrorCode==0x02) sprintf(bf,"Syntax Error %02x",ErrorCode);		
	if(ErrorCode==0x03) sprintf(bf,"Syntax Error %02x",ErrorCode);		
	if(ErrorCode==0x04) sprintf(bf,"Syntax Error %02x",ErrorCode);		
	if(ErrorCode==0x05) sprintf(bf,"Syntax Error %02x",ErrorCode);		
	if(ErrorCode==0x06) sprintf(bf,"Unkwon Error");		
	if(ErrorCode==0x07) sprintf(bf,"Math Error");	
	sprintf(bf,"\n");
	ErrorCode=0;
}
//函数名：Cal_Get_Token(Cal_String *s,Cal_Array *tk)
//传入参数：Cal_String *s,是一个待解析的字符串，Cal_Array *tk存放解析出来的一个一个token
//功能：从字符串*s里解析出token,放入*tk中
//返回0：过程终止（可能是因为解析结束，也可能是因为遇到错误，具体看ErrorCode的值），可借此判断是否扫描结束
//否则：返回的数字对应该token类型
uint8_t Cal_Get_Token(Cal_String *s,Cal_Array *tk){ 
	Cal_Token Token_Tmp; 
	uint8_t Num_Tmp[MaxLenth],Num_Ptr=0,i=0;//数字缓存数组 和它的指针
	uint8_t Dot_Flg=0; //用于.的计数，以判断奇葩的...错误 
	uint8_t Degree_Flg=0;  //度数指示：输入的值是否是角度值 
	Token_Tmp.type=0;
	Token_Tmp.value=0;
	for(i=0;i<MaxLenth;i++){
		Num_Tmp[i]=0;
	}
	if(s->Str[s->Str_Ptr]=='\0'||ErrorCode) return TYPE_INVALID;//结束了
	if(s->Str[s->Str_Ptr]>='0'&&s->Str[s->Str_Ptr]<='9')  //如果是数字 
	{  
		for(i=0;i<MaxLenth;i++) Num_Tmp[i]=0; //清空一波 
		Num_Tmp[Num_Ptr++]=s->Str[s->Str_Ptr++];
		if(s->Str[s->Str_Ptr]=='.') Dot_Flg++;
		if(s->Str[s->Str_Ptr]=='d') Degree_Flg++;
		while((s->Str[s->Str_Ptr]>='0'&&s->Str[s->Str_Ptr]<='9')||s->Str[s->Str_Ptr]=='.'||s->Str[s->Str_Ptr]=='d'){
			//cout<<num[p];
			
			Num_Tmp[Num_Ptr++]=s->Str[s->Str_Ptr++];
			if(s->Str[s->Str_Ptr]=='.') Dot_Flg++;
			if(s->Str[s->Str_Ptr]=='d') Degree_Flg++;
			if(Dot_Flg>1||Degree_Flg>1){//如果有多个d或多个. 肯定输入有错 
				//cout<<"Syntax Error 1 1";
				//exit(0);
				ErrorCode=0x01; 
				return 0;
			}
		}
		if(Num_Tmp[Num_Ptr-1]!='d'&&Degree_Flg){//如果输入了d，d却不是字符串末尾,就说明输入有问题 例："sin(30d1)" -> Error 0x01 
			ErrorCode=0x01;
			return 0;
		}
		Token_Tmp.type=TYPE_NUMBER;
		if(Degree_Flg) Token_Tmp.value=strtod(Num_Tmp,NULL)*Pi/180; //角度 
		else Token_Tmp.value=strtod(Num_Tmp,NULL); //数值 
		tk->token[tk->Array_Ptr++]=Token_Tmp;
		return TYPE_NUMBER;
	}	
	if(s->Str[s->Str_Ptr]=='e'||s->Str[s->Str_Ptr]=='p'||s->Str[s->Str_Ptr]=='A'){
		Token_Tmp.type=TYPE_NUMBER;
		if(s->Str[s->Str_Ptr]=='e'){
			Token_Tmp.value=Exp;
			s->Str_Ptr+=1;
			tk->token[tk->Array_Ptr++]=Token_Tmp;
			return TYPE_NUMBER;
		}
		if(s->Str[s->Str_Ptr]=='p'){
			Token_Tmp.value=Pi;
			s->Str_Ptr+=2;
			tk->token[tk->Array_Ptr++]=Token_Tmp;
			return TYPE_NUMBER;
		}
		if(s->Str[s->Str_Ptr]=='A'){
			Token_Tmp.value=Ans;
			s->Str_Ptr+=3;
			tk->token[tk->Array_Ptr++]=Token_Tmp;
			return TYPE_NUMBER;
		}
	} 
	if(s->Str[s->Str_Ptr]=='u'||s->Str[s->Str_Ptr]=='v'||s->Str[s->Str_Ptr]=='w'||s->Str[s->Str_Ptr]=='x'||s->Str[s->Str_Ptr]=='y'||s->Str[s->Str_Ptr]=='z'){
		Token_Tmp.type=TYPE_PARAMETER;
		Token_Tmp.value=s->Str[s->Str_Ptr]-(int)'u'+1;
		s->Str_Ptr+=1;
		tk->token[tk->Array_Ptr++]=Token_Tmp;
		return TYPE_PARAMETER;
	}
	if(s->Str[s->Str_Ptr]=='-'||s->Str[s->Str_Ptr]=='+'||s->Str[s->Str_Ptr]=='*'||s->Str[s->Str_Ptr]=='/'||s->Str[s->Str_Ptr]=='^'||s->Str[s->Str_Ptr]=='('||s->Str[s->Str_Ptr]==')'||s->Str[s->Str_Ptr]==','){
		if(s->Str[s->Str_Ptr]=='-'||s->Str[s->Str_Ptr]=='+'){
			if(s->Str_Ptr==0||s->Str[s->Str_Ptr-1]=='('){
				Token_Tmp.type=TYPE_NUMBER;
				Token_Tmp.value=0; //补一个0 
				tk->token[tk->Array_Ptr++]=Token_Tmp;			
			}	
			Token_Tmp.type=TYPE_OPERATOR;
			if(s->Str[s->Str_Ptr]=='+') Token_Tmp.value=ADD_FLAG;
			if(s->Str[s->Str_Ptr]=='-') Token_Tmp.value=MIN_FLAG;
			tk->token[tk->Array_Ptr++]=Token_Tmp;		
			s->Str_Ptr++;					
		}else{
			Token_Tmp.type=TYPE_OPERATOR;
			if(s->Str[s->Str_Ptr]=='*') Token_Tmp.value=MUL_FLAG;
			if(s->Str[s->Str_Ptr]=='/') Token_Tmp.value=DIV_FLAG;
			if(s->Str[s->Str_Ptr]=='^') Token_Tmp.value=POW_FLAG;
			if(s->Str[s->Str_Ptr]=='(') Token_Tmp.value=PLEFT_FLAG;
			if(s->Str[s->Str_Ptr]==')') Token_Tmp.value=PRIGHT_FLAG;
			if(s->Str[s->Str_Ptr]==',') Token_Tmp.value=COMMA_FLAG;
			tk->token[tk->Array_Ptr++]=Token_Tmp;	
			s->Str_Ptr++;	
		}
		return TYPE_OPERATOR;
	}
	if(s->Str[s->Str_Ptr]=='a'||s->Str[s->Str_Ptr]=='s'||s->Str[s->Str_Ptr]=='c'||s->Str[s->Str_Ptr]=='t'||s->Str[s->Str_Ptr]=='l'||s->Str[s->Str_Ptr]=='r'||s->Str[s->Str_Ptr]=='f'){
		Token_Tmp.type=TYPE_FUNCTION;
		if(s->Str[s->Str_Ptr]=='a'){
			if(s->Str[s->Str_Ptr+1]=='c'){Token_Tmp.value=ACOS_FLAG;s->Str_Ptr+=4;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}
			if(s->Str[s->Str_Ptr+1]=='s'){Token_Tmp.value=ASIN_FLAG;s->Str_Ptr+=4;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}
			if(s->Str[s->Str_Ptr+1]=='t'){Token_Tmp.value=ATAN_FLAG;s->Str_Ptr+=4;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}
			if(s->Str[s->Str_Ptr+1]=='b'){Token_Tmp.value=ABS_FLAG;s->Str_Ptr+=3;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}
		}
		if(s->Str[s->Str_Ptr]=='s'){
			if(s->Str[s->Str_Ptr+3]=='h'){Token_Tmp.value=SINH_FLAG;s->Str_Ptr+=4;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}
			else {Token_Tmp.value=SIN_FLAG;s->Str_Ptr+=3;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}			
		}
		if(s->Str[s->Str_Ptr]=='c'){
			if(s->Str[s->Str_Ptr+3]=='h'){Token_Tmp.value=COSH_FLAG;s->Str_Ptr+=4;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}
			else {Token_Tmp.value=COS_FLAG;s->Str_Ptr+=3;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}			
		}
		if(s->Str[s->Str_Ptr]=='t'){
			if(s->Str[s->Str_Ptr+3]=='h'){Token_Tmp.value=TANH_FLAG;s->Str_Ptr+=4;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}
			else {Token_Tmp.value=TAN_FLAG;s->Str_Ptr+=3;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}			
		}
		if(s->Str[s->Str_Ptr]=='l'){
			if(s->Str[s->Str_Ptr+1]=='n'){Token_Tmp.value=LN_FLAG;s->Str_Ptr+=2;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}
			if(s->Str[s->Str_Ptr+1]=='g'){Token_Tmp.value=LG_FLAG;s->Str_Ptr+=2;tk->token[tk->Array_Ptr++]=Token_Tmp;return TYPE_FUNCTION;}			
		}	
		if(s->Str[s->Str_Ptr]=='r'){  
			if(s->Str[s->Str_Ptr+4]=='o'){  //random()
				Token_Tmp.value=RANDOM_FLAG;
				s->Str_Ptr+=6;
				tk->token[tk->Array_Ptr++]=Token_Tmp;
				return TYPE_FUNCTION;
			}else{  //rand()
				Token_Tmp.value=RAND_FLAG;
				s->Str_Ptr+=4;
				tk->token[tk->Array_Ptr++]=Token_Tmp;
				return TYPE_FUNCTION;					
			}
		}	
		if(s->Str[s->Str_Ptr]=='f'){  
			if(s->Str[s->Str_Ptr+1]=='a'){  //fact()
				Token_Tmp.value=FACTORIAL_FLAG;
				s->Str_Ptr+=4;
				tk->token[tk->Array_Ptr++]=Token_Tmp;
				return TYPE_FUNCTION;
			}
		}			
	}	
	//cout<<"Syntax Error 0"; //存在错误字符 
	//exit(0);
	ErrorCode=0x03;
	return TYPE_INVALID;
}
//函数名：Cal_cmp(Cal_Token a,Cal_Token b)
//传入参数：两个token a和b
//功能：如果a和b均满足条件，即都是算符或者函数，比较其优先级大小
//返回：见函数内注释
uint8_t Cal_cmp(Cal_Token a,Cal_Token b){
	int pa=0,pb=0;
	if(a.type<=TYPE_PARAMETER||b.type<=TYPE_PARAMETER||ErrorCode) return CMP_INVALID;//invalid
	if(a.type==TYPE_OPERATOR){
		if(a.value==ADD_FLAG||a.value==MIN_FLAG) pa=PRIORITY1;//   +- 
		else if(a.value==MUL_FLAG||a.value==DIV_FLAG) pa=PRIORITY2;//  */
		else if(a.value==POW_FLAG) pa=PRIORITY3; // ^
		else return CMP_INVALID;		
	} 	
	if(b.type==TYPE_OPERATOR){
		if(b.value==ADD_FLAG||b.value==MIN_FLAG) pb=PRIORITY1;//   +- 
		else if(b.value==MUL_FLAG||b.value==DIV_FLAG) pb=PRIORITY2;//  */
		else if(b.value==POW_FLAG) pb=PRIORITY3; // ^
		else return CMP_INVALID;		
	}    
	if(a.type==TYPE_FUNCTION){
	    pa=PRIORITY4;	
	}

	if(b.type==4){
	    pb=PRIORITY4;	
	}	
	if(pa>pb) return CMP_LARGER;//a>b
	if(pa==pb) return CMP_SAME;//a==b;
	if(pa<pb) return CMP_SMALLER;//a<b
	return CMP_INVALID;//invalid
}
//函数名：Cal_Mid2Post(Cal_Array *Tokens,Cal_Array *Post,float *Para)
//传入参数：*Token是一堆刚解析出来的新鲜token序列 *Post存储处理得到的后缀式，即逆波兰式，*Para是参数表
//功能：逐个扫描*Tokens,将token序列转为后缀式序列存储在*Post中 
//返回 0:正常 0xff异常
uint8_t Cal_Mid2Post(Cal_Array *Tokens,Cal_Array *Post){
	Cal_Stack Stk;
	Cal_Token Tmp;
	uint8_t i;
	Stk.Stack_Ptr=0;
	Post->Array_Ptr=0;
    for(i=0;i<MaxLenth;i++){
		Stk.token[i].type=0;
		Stk.token[i].value=0;
		Post->token[i].type=0;
		Post->token[i].value=0;	
	}
    Tmp.value=0;
    Tmp.type=0;
	//cout<<tk_ptr;
	if(ErrorCode) return 0xff;
	for(i=0;i<Tokens->Array_Ptr;i++){
		if(Tokens->token[i].type==TYPE_NUMBER){
			Post->token[Post->Array_Ptr++]=Tokens->token[i];
			//cout<<Post->Array_Ptr;
		}else if(Tokens->token[i].type==TYPE_PARAMETER){ //遇到参数，直接把参数扔进逆波兰式，不作处理 
			Tmp.type=TYPE_PARAMETER;
			Tmp.value=Tokens->token[i].value;
			Post->token[Post->Array_Ptr++]=Tmp;
		}else if(Tokens->token[i].type==TYPE_OPERATOR||Tokens->token[i].type==TYPE_FUNCTION){
			if(Tokens->token[i].value==PLEFT_FLAG&&Tokens->token[i].type==TYPE_OPERATOR){ //如果是'('直接入栈 
				Stack_Push(&Stk,Tokens->token[i]);
			}else if(Tokens->token[i].value==PRIGHT_FLAG&&Tokens->token[i].type==TYPE_OPERATOR){ //如果是')'，把元素退栈到pst里直到遇到'('，弹出并丢弃 '('
				while(Stack_Get_Top(&Stk).type!=TYPE_OPERATOR||Stack_Get_Top(&Stk).value!=PLEFT_FLAG){
					Post->token[Post->Array_Ptr++]=Stack_Get_Top(&Stk);
					Stack_Pop(&Stk);
					if(Stack_IsEmpty(&Stk)){
						//cout<<"Syntax Error 1 2";//括号匹配失败 
						//exit(0);
						ErrorCode=0x02;
						return 0xff;
					}
				}
				Stack_Pop(&Stk);//弹出')' 
			}else if(Tokens->token[i].value==COMMA_FLAG&&Tokens->token[i].type==TYPE_OPERATOR){ //如果是','，把元素退栈到pst里直到遇到'('，不弹出'(' 
				while(Stack_Get_Top(&Stk).type!=TYPE_OPERATOR||Stack_Get_Top(&Stk).value!=PLEFT_FLAG){
					Post->token[Post->Array_Ptr++]=Stack_Get_Top(&Stk);
					Stack_Pop(&Stk);
					if(Stack_IsEmpty(&Stk)){
						//cout<<"Syntax Error 1 3";//括号匹配失败 
						//exit(0);
						ErrorCode=0x02;
						return 0xff;
					}
				}			
			}else{
				if((Tokens->token[i].type==TYPE_OPERATOR&&Stack_Get_Top(&Stk).value==PLEFT_FLAG)||Stack_IsEmpty(&Stk)){
					Stack_Push(&Stk,Tokens->token[i]);
				}else{
					while((Cal_cmp(Tokens->token[i],Stack_Get_Top(&Stk))==CMP_SMALLER||Cal_cmp(Tokens->token[i],Stack_Get_Top(&Stk))==CMP_SAME)&&(!Stack_IsEmpty(&Stk))){ 
						Post->token[Post->Array_Ptr++]=Stack_Get_Top(&Stk);
						Stack_Pop(&Stk);		
					}
					Stack_Push(&Stk,Tokens->token[i]);
				}
			}
		}
	}
	while(!Stack_IsEmpty(&Stk)){
		Post->token[Post->Array_Ptr++]=Stack_Get_Top(&Stk);
		Stack_Pop(&Stk);
	}	
	return 0;		
}
//函数名：Cal_Calculate_RPN(Cal_Array *Post,float *Para)
//传入参数：*Post是一堆刚转换出来的新鲜逆波兰式，*Para是参数表
//功能：把逆波兰式算出数值
//注：参数表内依次存有负责替换token中uvw xyz的数值   
//返回：计算结果
float Cal_Calculate_RPN(Cal_Array *Post,float *Para){
	Cal_Stack Stk;
	Cal_Token numa,numb,res;
	uint8_t i;
	Stk.Stack_Ptr=0;
	if(ErrorCode) return 0;
    for(i=0;i<MaxLenth;i++){
		Stk.token[i].type=0;
		Stk.token[i].value=0;
	}	
	if(Post->Array_Ptr==0){
		//cout<<"Syntax Error 2";
		//exit(0);
		ErrorCode=0x04;
		return 0; 
	}
	Stk.Stack_Ptr=0;
	for(i=0;i<Post->Array_Ptr;i++){
		if(Post->token[i].type==TYPE_NUMBER){
			Stack_Push(&Stk,Post->token[i]);
		}
		if(Post->token[i].type==TYPE_PARAMETER){
			res.type=TYPE_NUMBER;
			res.value=Para[(int)Post->token[i].value];//从参数表获取参数来压栈 
			Stack_Push(&Stk,res);
		}
		if(Post->token[i].type==TYPE_OPERATOR){
			if(Stk.Stack_Ptr<2){
				//cout<<"Syntax Error 3";
				//exit(0);
				ErrorCode=0x05;
				return 0;
			}
			res.type=TYPE_NUMBER;
			if(Post->token[i].value==ADD_FLAG) {
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);numb=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=numa.value+numb.value;
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==MIN_FLAG)	{
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);numb=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=numb.value-numa.value;
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==MUL_FLAG) {
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);numb=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=numa.value*numb.value;
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==DIV_FLAG) {
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);numb=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=numb.value/numa.value;
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==POW_FLAG) {
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);numb=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=pow(numb.value,numa.value);
				Stack_Push(&Stk,res);
			}else{
				//cout<<"Unknown Error";
				//exit(0);
				ErrorCode=0x06;
				return 0;
			}
		}
		if(Post->token[i].type==TYPE_FUNCTION){
			res.type=1;
			if(Stk.Stack_Ptr<1&&((Post->token[i].value>=1&&Post->token[i].value<=12)||Post->token[i].value==15)){ //1元函数 
				//cout<<"Syntax Error 3 1";
				//exit(0);
				ErrorCode=0x05;
				return 0;
			}
			if(Stk.Stack_Ptr<2&&(Post->token[i].type==14)){//2元函数 
				//cout<<"Syntax Error 3 2";
				//exit(0);
				ErrorCode=0x05;
				return 0;				
			}
			//13是0元函数 
			if(Post->token[i].value==SIN_FLAG){  //
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=sin(numa.value);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==COS_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=cos(numa.value);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==TAN_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=tan(numa.value);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==ASIN_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=asin(numa.value);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==ACOS_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=acos(numa.value);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==ATAN_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=atan(numa.value);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==ABS_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=fabs(numa.value);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==SINH_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=sinh(numa.value);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==COSH_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=cosh(numa.value);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==TANH_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=tanh(numa.value);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==LN_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=log(numa.value);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==LG_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				res.value=log(numa.value)/log(10);
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==RAND_FLAG){
				res.value=(float)rand()/32768.0;
				Stack_Push(&Stk,res);
			}else if(Post->token[i].value==RANDOM_FLAG){
				numa=Stack_Get_Top(&Stk);Stack_Pop(&Stk);numb=Stack_Get_Top(&Stk);Stack_Pop(&Stk);
				if(numa.value<=numb.value){
					//cout<<"Math Error 1";
					//exit(0);
					ErrorCode=0x07;
					return 0; 
				}
				res.value=rand()%(((int)numa.value-(int)numb.value)+(int)numb.value);
				Stack_Push(&Stk,res);				
			}else if(Post->token[i].value==FACTORIAL_FLAG){
				numa=Stack_Get_Top(&Stk);
				Stack_Pop(&Stk);
				if(numa.value!=(int)numa.value){
					//cout<<"Math Error 1";
					//exit(0);
					ErrorCode=0x07;
					return 0; 
				}
				res.value=1;
				numa.value=(int)numa.value;
				while(numa.value>=1) {
					res.value*=numa.value;
					numa.value--;
				}
				Stack_Push(&Stk,res);				
			}
		}
	}
	if(Stk.Stack_Ptr!=1){
		ErrorCode=0x06;
		return 0;
	}
	Ans=Stk.token[Stk.Stack_Ptr-1].value; 
	return Stk.token[Stk.Stack_Ptr-1].value;
} 
//函数名：Calculate_Expression(char *s,uint8_t l, float *para)
//传入参数：*s是字符串，l是其有效长度（要正确的给出！），*para是参数表
//功能：顶层功能在此集成，一个函数把字符串算出数值并返回
//注：参数表内依次存有负责替换token中uvw xyz的数值   
//返回：计算结果
float Calculate_Expression(char *s,uint8_t l, float *para){
	uint8_t i;
	float res;
	Cal_String Input;
	Cal_Array tks,Post;	
	Input.Str_Ptr=0;
	tks.Array_Ptr=0;
	Post.Array_Ptr=0;
	ErrorCode=0;
	for(i=0;i<MaxLenth;i++){
		Input.Str[i]=0;
		tks.token[i].value=0;
		tks.token[i].type=0;
		Post.token[i].value=0;
		Post.token[i].type=0;
	}
	for(i=0;i<l;i++){
		Input.Str[i]=s[i];
	}
	while(Cal_Get_Token(&Input,&tks));
	Cal_Mid2Post(&tks,&Post);
	res=Cal_Calculate_RPN(&Post,para);
	return res;
}
uint8_t Get_ErrorCode(){
    return ErrorCode;
}
