#include "W25QXX.h"

#if (SPIMODE==SoftWare)
uchar W25QXX_RWByte(uchar dat){
    uchar i,res=0;
    bit flg;
    flg=EA;
    if(EA) EA=0;
    for(i=0;i<8;i++){
        SI=dat&0x80;
        CLK=1;
        dat<<=1;
        res<<=1;
        res|=SO;
        CLK=0;   
    }
    EA=flg;
    SI=1;
    //模式0：时钟极性：0 时钟相位：0
    //双方都是：时钟上升沿采样，下降沿更新数据
    return res;
}
uchar W25QXX_ReadRegisters(uchar id){
    uchar res;
    CLK=0;
    CS=0;
    if(id==1) W25QXX_RWByte(0x05);//读状态寄存器1
    if(id==2) W25QXX_RWByte(0x35);//读状态寄存器2
    res=W25QXX_RWByte(0xff);
    CS=1;
    return res;
}
void W25QXX_WriteRegisters(uchar r1,uchar r2){
    CLK=0;
    CS=0;
    W25QXX_RWByte(0x01);//写状态寄存器
    W25QXX_RWByte(r1);//状态寄存器1
    W25QXX_RWByte(r2);//状态寄存器2
    CS=1;   
}
void W25QXX_WriteEnable(uchar flg){
    CS=0;
    CLK=0;
    if(flg) W25QXX_RWByte(0x06); 
    else W25QXX_RWByte(0x04);    
    CS=1;
    return;
}
uchar W25QXX_TestBusy(){
    uchar res;
    CS=0;
    CLK=0;
    res=W25QXX_ReadRegisters(1);
    CS=1;
    if(res&0x01) return 1;
    else return 0;
}
uchar W25QXX_GetCapacity(){
    uchar res=0;
    CLK=0;
    CS=0;
    W25QXX_RWByte(0x9f);
    W25QXX_RWByte(0xff);
    W25QXX_RWByte(0xff);
    res=W25QXX_RWByte(0xff);
    CS=1;
    if(res==0x14) return 1;//1M Byte
    if(res==0x15) return 2;//2M Byte
    if(res==0x16) return 4;//4M Byte
    if(res==0x17) return 8;//8M Byte
    else return res;
}

void W25QXX_EraseChip(){
    CS=0;
    CLK=0;
    W25QXX_RWByte(0xc7);
    CS=1;
    while(W25QXX_TestBusy());
    return;
}
uchar W25QXX_ReadByte(ulong add){
    uchar res;
    while(W25QXX_TestBusy());
    CS=0;
    CLK=0;
    W25QXX_RWByte(0x03);
    W25QXX_RWByte(add>>16);
    W25QXX_RWByte(add>>8);
    W25QXX_RWByte(add);
    res=W25QXX_RWByte(0xff);
    CS=1;
    return res;   
}
void W25QXX_WriteByte(ulong add,uchar dat){
    while(W25QXX_TestBusy());
    W25QXX_WriteEnable(1);
    CS=0;
    CLK=0;
    W25QXX_RWByte(0x02);
    W25QXX_RWByte(add>>16);
    W25QXX_RWByte(add>>8);
    W25QXX_RWByte(add);
    W25QXX_RWByte(dat);
    CS=1;
    return;
}
void W25QXX_ReadPage(ulong add,uchar dat[]){
    int i;
    CS=0;
    CLK=0;
    W25QXX_RWByte(0x03);
    W25QXX_RWByte(add>>16);
    W25QXX_RWByte(add>>8);
    W25QXX_RWByte(add);
    for(i=0;i<256;i++) {dat[i]=W25QXX_RWByte(0xff);}
    CS=1;
} 
void W25QXX_WritePage(ulong add,uchar dat[]){
    uint i;
    while(W25QXX_TestBusy());
    W25QXX_WriteEnable(1);
    CS=0;
    CLK=0;
    W25QXX_RWByte(0x02);
    W25QXX_RWByte(add>>16);
    W25QXX_RWByte(add>>8);
    W25QXX_RWByte(add);
    for(i=0;i<256;i++) W25QXX_RWByte(dat[i]);
    CS=1;
    return;
}
void W25QXX_EraseSector(ulong add){
    uint i;
    while(W25QXX_TestBusy());
    W25QXX_WriteEnable(1);
    CS=0;
    CLK=0;
    W25QXX_RWByte(0x20);
    W25QXX_RWByte(add>>16);
    W25QXX_RWByte(add>>8);
    W25QXX_RWByte(add);
    CS=1;
    return; 
}    

#endif

#if (SPIMODE==HardWare)

void HardWareSPI_Init(uchar spd){
    WP=HOLD=SI=1;
    SPCTL |=  (1 << 7); //忽略 SS 引脚功能，使用 MSTR 确定器件是主机还是从机
    SPCTL |=  (1 << 6); //使能 SPI 功能
    SPCTL &= ~(1 << 5); //先发送/接收数据的高位（ MSB）
    SPCTL |=  (1 << 4); //设置主机模式
    SPCTL &= ~(1 << 3); //SCLK 空闲时为低电平，SCLK 的前时钟沿为上升沿，后时钟沿为下降沿
    SPCTL &= ~(1 << 2); //数据 SS 管脚为低电平驱动第一位数据并在 SCLK 的后时钟沿改变数据
    //采用漏极开路+上拉电阻方式驱动，需要调低SPI频率才能正常与Flash通信
    SPCTL =  SPCTL|spd;   //SPI 时钟频率选择, 0: 4T, 1: 8T,  2: 16T,  3: 2T
    P_SW1 =  P_SW1|0x0c;     //IO口切换. 0: P1.2/P5.4 P1.3 P1.4 P1.5, 1: P2.2 P2.3 P2.4 P2.5, 2: P5.4 P4.0 P4.1 P4.3, 3: P3.5 P3.4 P3.3 P3.2

    SPSTAT = SPIF + WCOL;   //清0 SPIF和WCOL标志
}
uchar W25QXX_RWByte(uchar dat){
    SPDAT = dat;
    while((SPSTAT & SPIF) == 0) ;
    SPSTAT = SPIF + WCOL;   //清0 SPIF和WCOL标志    
    return SPDAT;
}
uchar W25QXX_ReadRegisters(uchar id){
    uchar res;
    CS=0;
    if(id==1) W25QXX_RWByte(0x05);//读状态寄存器1
    if(id==2) W25QXX_RWByte(0x35);//读状态寄存器2
    res=W25QXX_RWByte(0xff);
    CS=1;
    return res;
}
void W25QXX_WriteRegisters(uchar r1,uchar r2){
    CS=0;
    W25QXX_RWByte(0x01);//写状态寄存器
    W25QXX_RWByte(r1);//状态寄存器1
    W25QXX_RWByte(r2);//状态寄存器2
    CS=1;   
}
void W25QXX_WriteEnable(uchar flg){
    CS=0;
    if(flg) W25QXX_RWByte(0x06); 
    else W25QXX_RWByte(0x04);    
    CS=1;
    return;
}
uchar W25QXX_TestBusy(){
    uchar res;
    CS=0;
    res=W25QXX_ReadRegisters(1);
    CS=1;
    if(res&0x01) return 1;
    else return 0;
}
uchar W25QXX_GetCapacity(){
    uchar res=0;
    CS=0;
    W25QXX_RWByte(0x9f);
    W25QXX_RWByte(0xff);
    W25QXX_RWByte(0xff);
    res=W25QXX_RWByte(0xff);
    CS=1;
    if(res==0x14) return 1;//1M Byte
    if(res==0x15) return 2;//2M Byte
    if(res==0x16) return 4;//4M Byte
    if(res==0x17) return 8;//8M Byte
    else return res;
}

void W25QXX_EraseChip(){
    CS=0;
    W25QXX_RWByte(0xc7);
    CS=1;
    while(W25QXX_TestBusy());
    return;
}
uchar W25QXX_ReadByte(ulong add){
    uchar res;
    while(W25QXX_TestBusy());
    CS=0;
    W25QXX_RWByte(0x03);
    W25QXX_RWByte(add>>16);
    W25QXX_RWByte(add>>8);
    W25QXX_RWByte(add);
    res=W25QXX_RWByte(0xff);
    CS=1;
    return res;   
}
void W25QXX_WriteByte(ulong add,uchar dat){
    while(W25QXX_TestBusy());
    W25QXX_WriteEnable(1);
    CS=0;
    W25QXX_RWByte(0x02);
    W25QXX_RWByte(add>>16);
    W25QXX_RWByte(add>>8);
    W25QXX_RWByte(add);
    W25QXX_RWByte(dat);
    CS=1;
    return;
}
void W25QXX_ReadPage(ulong add,uchar dat[]){
    int i;
    CS=0;
    W25QXX_RWByte(0x03);
    W25QXX_RWByte(add>>16);
    W25QXX_RWByte(add>>8);
    W25QXX_RWByte(add);
    for(i=0;i<256;i++) {dat[i]=W25QXX_RWByte(0xff);}
    CS=1;
} 
void W25QXX_WritePage(ulong add,uchar dat[]){
    uint i;
    while(W25QXX_TestBusy());
    W25QXX_WriteEnable(1);
    CS=0;
    W25QXX_RWByte(0x02);
    W25QXX_RWByte(add>>16);
    W25QXX_RWByte(add>>8);
    W25QXX_RWByte(add);
    for(i=0;i<256;i++) W25QXX_RWByte(dat[i]);
    CS=1;
    return;
}
void W25QXX_EraseSector(ulong add){
    uint i;
    while(W25QXX_TestBusy());
    W25QXX_WriteEnable(1);
    CS=0;
    W25QXX_RWByte(0x20);
    W25QXX_RWByte(add>>16);
    W25QXX_RWByte(add>>8);
    W25QXX_RWByte(add);
    CS=1;
    return; 
}    

#endif
