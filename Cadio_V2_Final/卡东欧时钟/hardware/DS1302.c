#include "DS1302.h"
void rtc_delay(){
    uint i=0;
    //for(i=0;i<30;i++);
}
void DS1302_WriteByte(uchar addr,uchar value){
    uchar i;
    if(EA||1){
        clk=0; rtc_delay(); 
        ce=0;  rtc_delay();
        ce=1;  rtc_delay();
        dat=0; rtc_delay(); 
        for(i=0;i<8;i++){
            if(addr&0x01) dat=1;
            else dat=0;
            rtc_delay(); 
            clk=1;    rtc_delay();
            clk=0;       rtc_delay();
            addr>>=1;rtc_delay();
        }
        for(i=0;i<8;i++){  
            if(value&0x01) dat=1;
            else dat=0; 
            rtc_delay();        
            clk=1;         rtc_delay();
            clk=0;         rtc_delay();
            value>>=1;        rtc_delay(); 
        }
        ce=0;rtc_delay();
        //EA=1;
    }
    return;
}

//------------------下面是无可奈何之举------------------------//
uchar DS1302_ReadByte0(uchar addr){
    uchar i,res=0;
    if(EA||1){
        EA=0;
        clk=0;   rtc_delay();  
        ce=0;    rtc_delay(); 
        ce=1;    rtc_delay(); 
        for(i=0;i<8;i++){
            if(addr&0x01) dat=1;
            else dat=0;       
            rtc_delay(); 
            clk=1;         rtc_delay();
            if(i!=7) clk=0;     rtc_delay();    
            addr>>=1;rtc_delay();
        }
        dat=1;     
        for(i=0;i<8;i++){
            clk=0;        rtc_delay();
            res>>=1;       rtc_delay();
            if(dat) res|=0x80; rtc_delay();        
            if(i!=7) clk=1;    rtc_delay();     
        }
        ce=0;rtc_delay();
        //EA=1;
    }
    return res;
}
//读取到正确的内容再返回
uchar DS1302_ReadByte(uchar addr){
    uint8_t res=0xff;
    while(res==0xff) res=DS1302_ReadByte0(addr);
    return res;
}
//----------------------------------------------------------//

void DS1302_WriteTime(time t){
    uchar h=0x00;;
    DS1302_WriteByte(0x8e,0x00);
    DS1302_WriteByte(0x80,((t.sec/10)<<4)|(t.sec%10));
    DS1302_WriteByte(0x82,((t.min/10)<<4)|(t.min%10));
    if(t.mode==24) DS1302_WriteByte(0x84,((t.hour/10)<<4)|(t.hour%10)); 
    else{   
        h=((t.hour/10)<<4)|(t.hour%10);
        h=h|(0x20&t.ap);
        h|=0x80;
        DS1302_WriteByte(0x84,h);
    } 
    DS1302_WriteByte(0x86,((t.day/10)<<4)|(t.day%10));
    DS1302_WriteByte(0x88,((t.month/10)<<4)|(t.month%10));
    DS1302_WriteByte(0x8a,t.weekday);
    DS1302_WriteByte(0x8c,((t.year/10)<<4)|(t.year%10));
    DS1302_WriteByte(0x8e,0x80);
    return;
}
time DS1302_ReadTime(){
    time tt;
    tt.sec=DS1302_ReadByte(0x81); tt.sec=((tt.sec>>4)&0x07)*10+(tt.sec&0x0f)*1;
    tt.min=DS1302_ReadByte(0x83); tt.min=((tt.min>>4)&0x07)*10+(tt.min&0x0f)*1;
    tt.hour=DS1302_ReadByte(0x85); 
    if(tt.hour&0x80){
        tt.mode=12;
        tt.ap=0x20&&tt.hour;
        tt.hour=((tt.hour>>4)&0x01)*10+(tt.hour&0x0f)*1;
    }else{
        tt.mode=24;
        tt.hour=((tt.hour>>4)&0x0f)*10+(tt.hour&0x0f)*1;
    }
    tt.day=DS1302_ReadByte(0x87); tt.day=((tt.day>>4)&0x03)*10+(tt.day&0x0f);
    tt.month=DS1302_ReadByte(0x89); tt.month=((tt.month>>4)&0x01)*10+(tt.month&0x0f);
    tt.weekday=DS1302_ReadByte(0x8b); tt.weekday=tt.weekday&0x07;
    tt.year=DS1302_ReadByte(0x8d); tt.year=((tt.year>>4)&0x0f)*10+(tt.year&0x0f);  
    return tt;  
}
void DS1302_Init(){
    //2000.1.1 thurs 00:00
    uchar addr;  
    DS1302_WriteByte(0x8e,0x00);
    DS1302_WriteByte(0x80,0x00);
    DS1302_WriteByte(0x82,0x00);
    DS1302_WriteByte(0x84,0x00); 
    DS1302_WriteByte(0x86,0x01);
    DS1302_WriteByte(0x88,0x01);
    DS1302_WriteByte(0x8a,0x02);
    DS1302_WriteByte(0x8c,0x00);              
    for(addr=0xc2;addr<=0xfc;addr+=2){
         DS1302_WriteByte(addr,0x00);
    }
    DS1302_WriteByte(0x8e,0x80);
    return;
}
//Ram_structure:
//4byte per alarm:
//ram1~4: ctrl,hour,min,lenth
//ctrl:[7]:en(on:1 off,0)  [6]: mode (weekday_only:1,whole_week:0)   [5]: func(beep:0 word:1)
alarm DS1302_ReadAlarm(uchar num){
    alarm anow;
    anow.ctrl=DS1302_ReadByte(0xc3+8*(num-1));
    anow.hour_s=DS1302_ReadByte(0xc5+8*(num-1));
    anow.hour_s=((anow.hour_s>>4)&0x0f)*10+(anow.hour_s&0x0f);
    anow.min_s=DS1302_ReadByte(0xc7+8*(num-1));
    anow.min_s=((anow.min_s>>4)&0x0f)*10+(anow.min_s&0x0f);
    anow.lenth=DS1302_ReadByte(0xc9+8*(num-1));
    return anow;
}  
void DS1302_WriteAlarm(alarm a,uchar num){
    DS1302_WriteByte(0x8e,0x00);
    DS1302_WriteByte(0xc2+8*(num-1),a.ctrl);
    DS1302_WriteByte(0xc4+8*(num-1),((a.hour_s/10)<<4)|(a.hour_s%10));
    DS1302_WriteByte(0xc6+8*(num-1),((a.min_s/10)<<4)|(a.min_s%10));
    DS1302_WriteByte(0xc8+8*(num-1),a.lenth);
    DS1302_WriteByte(0x8e,0x80);
    return;
}
uchar DS1302_MatchAlarm(time tnow){
    alarm anow;
    uchar min_e,flag[3]={0,0,0};
    uchar i=0;
    for(i=0;i<3;i++){
        flag[i]=0;        
        anow=DS1302_ReadAlarm(i+1);
        if(anow.ctrl==0) continue;
        if((anow.ctrl==2)&&(tnow.weekday==6||tnow.weekday==7)) continue;
        if(anow.min_s+anow.lenth>59){
            min_e=(anow.min_s+anow.lenth)%60;
            if(anow.hour_s+1>23) { if((tnow.hour==23||tnow.hour==24)&&(tnow.min>=anow.min_s&&tnow.min<=59)||(tnow.min>=0&&tnow.min<=min_e) ) flag[i]=1;}
            else if((tnow.hour==anow.hour_s||tnow.hour==anow.hour_s+1)&&((tnow.min>=anow.min_s&&tnow.min<=59)||(tnow.min>=0&&tnow.min<=min_e))) flag[i]=1;
            else flag[i]=0;
        }else{
            min_e=anow.min_s+anow.lenth;
            if(tnow.hour==anow.hour_s&&tnow.min>=anow.min_s&&tnow.min<=min_e) flag[i]=1;
            else flag[i]=0;
        }       
    }
    return flag[2]*4+flag[1]*2+flag[0]; 
    /* 
    for(i=0;i<3;i++){
        if(flag[i]) return i+1;
        else return 0;
    }
    */
}   
