#include "D18B20.h"
void delay_us(uint8_t t){
    do{
        usrNOP();
        usrNOP();
    }while(--t);
}
uint8_t D18b20_Init(){
    DQ = 0;                     //送出低电平复位信号
    delay_us(240);              //延时至少480us
    delay_us(240);
    DQ = 1;                     //释放数据线
    delay_us(60);               //等待60us
    CY = DQ;                    //检测存在脉冲
    delay_us(240);              //等待设备释放数据线
    delay_us(180);
    return CY;
}
void D18b20_WriteOneByte(uint8_t dat){
    char i;
    for (i=0; i<8; i++)             //8位计数器
    {
        DQ = 0;                     //开始时间片
        delay_us(1);                //延时等待
        dat >>= 1;                  //送出数据
        DQ = CY;
        delay_us(60);               //等待时间片结束
        DQ = 1;                     //恢复数据线
        delay_us(1);                //恢复延时
    }
}
uint8_t D18b20_ReadOneByte(){
    u8 i;
    u8 dat = 0;

    for (i=0; i<8; i++)             //8位计数器
    {
        dat >>= 1;
        DQ = 0;                     //开始时间片
        delay_us(1);                //延时等待
        DQ = 1;                     //准备接收
        delay_us(1);                //接收延时
        if (DQ) dat |= 0x80;        //读取数据
        delay_us(60);               //等待时间片结束
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


		
