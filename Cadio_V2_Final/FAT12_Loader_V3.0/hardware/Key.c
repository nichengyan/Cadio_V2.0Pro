#include "Key.h"

/*
key0ãÐÖµ
3250
2474
2320
2166
2010
1853
1695
1534
1369
1200
1028
853
673
488

key1ãÐÖµ
3463
2923
2815
2707
2597
2488
2380
2270
2158
2045
1929
1803
1686
1577
1457
1335
1209
1081
950
819
680
533
381
209
*/
uint Key_GetADC(uchar chs){
    uchar i,j;
    uint tmp,samples[11];
    ADC_Init();
    for(i=0;i<10;i++){
        if(chs==0) samples[i]=ADC_Result(ADC_CH0,ADC_SPEEDLL);
        else samples[i]=ADC_Result(ADC_CH1,ADC_SPEEDLL); 
        for(j=i-1;j>0&&i>0;j--){
            if(samples[j+1]<samples[j]){
                tmp=samples[j+1];
                samples[j+1]=samples[j];
                samples[j]=tmp;
            }
        }          
    }
    return samples[5];
} 
uint Key_Scan(void){
    uint key_0=0,key_1=0,keynum=0;
    uchar res0=0,res1=0;
    key_0=Key_GetADC(1);
    key_1=Key_GetADC(0);
    //key0:
    if(key_0>3250) res0 = 255;
    else if(key_0<=3250&&key_0>2474) res0 = 0x45 ;
    else if(key_0<=2474&&key_0>2320) res0 = 0x4f ;
    else if(key_0<=2320&&key_0>2166) res0 = 0x52 ;
    else if(key_0<=2166&&key_0>2010) res0 = 0x4c ;
    else if(key_0<=2010&&key_0>1853) res0 = 0x41 ;
    else if(key_0<=1853&&key_0>1695) res0 = 0x58 ;
    else if(key_0<=1695&&key_0>1534) res0 = 0x4d ;
    else if(key_0<=1534&&key_0>1369) res0 = 0x53 ;
    else if(key_0<=1369&&key_0>1200) res0 = 0x50 ;
    else if(key_0<=1200&&key_0>1028) res0 = 0x56 ;
    else if(key_0<=1028&&key_0>853) res0 = 0x57 ;
    else if(key_0<=853&&key_0>673) res0 = 0x51 ;
    else if(key_0<=673&&key_0>488) res0 = 0x54 ;
    else if(key_0<=488&&key_0>0) res0 = 0x4e ;



    if(key_1>3463) res1 = 255;
    else if(key_1<=3463&&key_1>2923) res1 = 0x4b ;
    else if(key_1<=2923&&key_1>2815) res1 = 0x31 ;
    else if(key_1<=2815&&key_1>2707) res1 = 0x34 ;
    else if(key_1<=2707&&key_1>2597) res1 = 0x37 ;
    else if(key_1<=2597&&key_1>2488) res1 = 0x55 ;
    else if(key_1<=2488&&key_1>2380) res1 = 0x30 ;
    else if(key_1<=2380&&key_1>2270) res1 = 0x38 ;
    else if(key_1<=2270&&key_1>2158) res1 = 0x35 ;
    else if(key_1<=2158&&key_1>2045) res1 = 0x32 ;
    else if(key_1<=2045&&key_1>1929) res1 = 0x46 ;
    else if(key_1<=1929&&key_1>1803) res1 = 0x47 ;
    else if(key_1<=1803&&key_1>1686) res1 = 0x33 ;
    else if(key_1<=1686&&key_1>1577) res1 = 0x36 ;
    else if(key_1<=1577&&key_1>1457) res1 = 0x39 ;
    else if(key_1<=1457&&key_1>1335) res1 = 0x6e ;
    else if(key_1<=1335&&key_1>1209) res1 = 0x6b ;
    else if(key_1<=1209&&key_1>1081) res1 = 0x6d ;
    else if(key_1<=1081&&key_1>950) res1 = 0x6a ;
    else if(key_1<=950&&key_1>819) res1 = 0x6f ;
    else if(key_1<=819&&key_1>680) res1 = 0x3d ;
    else if(key_1<=680&&key_1>533) res1 = 0x4a ;
    else if(key_1<=533&&key_1>381) res1 = 0x44 ;
    else if(key_1<=381&&key_1>209) res1 = 0x48 ;
    else if(key_1<=209&&key_1>0) res1 = 0x49 ;

    keynum|=res0;
    keynum<<=8;
    keynum|=res1;
    return keynum;
}       
