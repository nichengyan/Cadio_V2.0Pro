#include "D18B20.h"
void DelayX(uint t){
    t/=MCU_TYPE;
	while(t--);
}
byte D18b20_Init(){
	unsigned char status;
	DQ=0;
	DelayX(300);
	DQ=1;
	DelayX(100);
	status=DQ;
	DelayX(250);
	return status;
}
void D18b20_WriteOneByte(byte dat){
	unsigned char i;
    if(EA){
        EA=0;
        for(i=0;i<8;i++){
            DQ=0;
            DQ=dat&0x01; 
            DelayX(45);
            dat>>=1;
            DelayX(5);
            DQ=1;
        }
        DelayX(50);
        EA=1;
    }
}
byte D18b20_ReadOneByte(){
	unsigned char dat=0,i=0;
    if(EA){
        EA=0;
        for(;i<8;i++){
            DQ=0;
            dat>>=1;
            DQ=1;
            if(DQ==1) dat|=0x80;
            DelayX(50);
        }
        EA=1;
        DelayX(50);
    }
	return dat;
}
int D18b20_GetTemperature(){
	unsigned char a=0;
	unsigned char b=0;
	unsigned int t=0;
	float tt=0;
	D18b20_Init();
	D18b20_WriteOneByte(0xCC);
	D18b20_WriteOneByte(0x44);
	D18b20_Init();
	D18b20_WriteOneByte(0xCC);
	D18b20_WriteOneByte(0xBE);
	a=D18b20_ReadOneByte();
	b=D18b20_ReadOneByte();
	t=b;
	t<<=8;
	t=t|a;
	tt=t*0.0625;
	t=tt*10+0.5;
	return(t);
}


		
