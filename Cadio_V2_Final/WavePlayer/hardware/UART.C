#include "UART.h"


void Uart_Init(void)	//9600bps@24.000MHz
{
	PCON &= 0x7F;		//²¨ÌØÂÊ²»±¶ËÙ
	SCON = 0x50;		//8Î»Ê¾Ý,¿É±ä²¨ÌØÂÊ
	AUXR |= 0x40;		//¶¨Ê±Æ÷Ê±ÖÓ1TÄ£Ê½
	AUXR &= 0xFE;		//´®¿Ú1Ñ¡Ôñ¶¨Ê±Æ÷1Îª²¨ÌØÂÊ·¢ÉúÆ÷
	TMOD &= 0x0F;		//ÉèÖÃ¶¨Ê±Æ÷Ä£Ê½
	TMOD |= 0x20;		//ÉèÖÃ¶¨Ê±Æ÷Ä£Ê½
	TL1 = 0xB2;			//ÉèÖÃ¶¨Ê±³õÊ¼Öµ
	TH1 = 0xB2;			//ÉèÖÃ¶¨Ê±ÖØÔØÖµ
	ET1 = 0;			//½ûÖ¹¶¨Ê±Æ÷ÖÐ¶Ï
	TR1 = 1;			//¶¨Ê±Æ÷1¿ªÊ¼¼ÆÊ±
}


void Uart_T8Bit(int8 dat){
	SBUF=dat;
	while(!TI);
	TI=0;
}
void Uart_T16Bit(int16 dat){
	int8 a,b;
	b=dat;
	dat>>=8;
	a=dat;
	SBUF=a;
	while(!TI);
	TI=0;
	SBUF=b;
	while(!TI);
	TI=0;
}
void Uart_T32Bit(int32 dat){
	int8 a,b,c,d;
	d=dat;
	dat>>=8;
	c=dat;
	dat>>=8;
	b=dat;
	dat>>=8;
	a=dat;
	SBUF=a;
	while(!TI);
	TI=0;
	SBUF=b;
	while(!TI);
	TI=0;
	SBUF=c;
	while(!TI);
	TI=0;
	SBUF=d;
	while(!TI);
	TI=0;
}
		
int8 Uart_R8Bit(){
	while(!RI);
	RI=0;
	return SBUF;
} 
int16 Uart_R16Bit(){
	int16 dat;
	dat<<=8;
	while(!RI);
	RI=0;
	dat|=SBUF;
	dat<<=8;
	while(!RI);
	RI=0;
	dat|=SBUF;
	return dat;
}
int32 Uart_R32Bit(){
	int32 dat=0;
	dat<<=8;
	while(!RI);
	RI=0;
	dat|=SBUF;
	dat<<=8;
	while(!RI);
	RI=0;
	dat|=SBUF;
	dat<<=8;
	while(!RI);
	RI=0;
	dat|=SBUF;
	dat<<=8;
	while(!RI);
	RI=0;
	dat|=SBUF;
	return dat;
}
void Uart_Test(){
    Uart_T8Bit(Uart_R8Bit());
    return;
}
void Uart_SendString(uchar buffer[]){
    uchar i;
    for(i=0;buffer[i]!='$';i++){
	    SBUF=buffer[i];
	    while(!TI);
	    TI=0;
    }
    return;
}
void Uart_SendNum(ulong num,uchar lenth,uchar mode){
    uchar i,flg=0,tmp=0,buffer[15];
    for(i=0;i<lenth;i++){    
        buffer[i]=num%10;
        num/=10;
    }
    for(i=lenth;i>0;i--){
        tmp=buffer[i-1];
        if(mode==0){
             Uart_T8Bit(tmp+48);
        }else{
            if(tmp!=0&&flg==0) flg=1;
            if(flg==0&&tmp==0) continue;
            if(flg==1) Uart_T8Bit(tmp+48);
        }
    }
    return;
}