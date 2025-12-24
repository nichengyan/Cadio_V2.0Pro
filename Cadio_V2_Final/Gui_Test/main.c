
#include  "_STC8X_.h"
#include  <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "oled.h"
#include "Graphic.h"
#include "Page0.h"
#include "Gui.h"
#include "Key.h"
#include "Widget_Bmp.h"


       //uint8_t xdata a[10];
sbit Wifi=P2^0;
sbit Audio=P2^1;
sbit laser=P1^3;
sbit Ir=P1^6;
void main()
{
    P0M0=0x00;
    P0M1=0x00;
    P1M1=0xf7; 
    P1M0=0x00;   //����Ϊ���迹
    P2M0=0x00;
    P2M1=0x00;
    P3M0=0x00;
    P3M1=0x00;
    P5M0=0x00;
    P5M1=0x00;
    Wifi=0;
    Audio=0;
    laser=0;
    Ir=1;
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear(); 
    Gui_Init();
    //Page0_Setup(1);
    //Page_Register(0,"Test 0",NULL,Page0_Setup,Page0_Loop,Page0_Exit,Page0_Event);
    //Page_OpenCurrentPage();
    while(1){
        //Gui_Run();  
        //OLED_Printf(0, 0, 16, "%d", rand());

    }
}