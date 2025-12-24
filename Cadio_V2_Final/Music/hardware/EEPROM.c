#include "EEPROM.h"
void EEPROM_Init(uchar WT){
    IAP_CONTR|=0x80;
    IAP_TPS=WT;
}
uchar EEPROM_ReadByte(ulong add){
    IAP_ADDRL=add;
    add>>=8;
    IAP_ADDRH=add;
    IAP_CMD=0x01;
    IAP_TRIG=0x5a;
    IAP_TRIG=0xa5;
    while(IAP_CONTR&0x10) IAP_CONTR&=0xef;
    return IAP_DATA;
}  
void EEPROM_WriteByte(ulong add,uchar dat){
    IAP_DATA=dat;
    IAP_ADDRL=add;
    add>>=8;
    IAP_ADDRH=add;
    IAP_CMD=0x02;
    IAP_TRIG=0x5a;
    IAP_TRIG=0xa5;
    while(IAP_CONTR&0x10) IAP_CONTR&=0xef;
    return;
}  
void EEPROM_SectorErase(ulong add){
    IAP_ADDRL=add;
    add>>=8;
    IAP_ADDRH=add;
    IAP_CMD=0x03; 
    IAP_TRIG=0x5a;
    IAP_TRIG=0xa5;
    while(IAP_CONTR&0x10) IAP_CONTR&=0xef;
    return;
}
