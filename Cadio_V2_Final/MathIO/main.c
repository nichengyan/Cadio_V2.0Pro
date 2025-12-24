#include "_STC8X_.h"
#include "TYPE.h"
#include "editor.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "oled.h"
#include "Gui.h"
#include "ADC.h"
#include "Key.h"
#include "FAT12_2v1.h"
 #include "Expression.h"
#include "tick.h"
// #include "Gui.h"
#define SPIF 0x80 // SPI传输完成标志。写入1清0。
#define WCOL 0x40 // SPI写冲突标志。写入1清0。
/*--------------------------------------------------------------------------


--------------------------------------------------------------------------*/

#define Fosc 40000000L // ������ʱ��
uint8_t i,j;
uint8_t Res;
sbit Wifi = P2 ^ 0;
sbit Audio = P2 ^ 1;
sbit laser = P1 ^ 3;
sbit light = P1 ^ 7;
sbit sd_cs = P1 ^ 6;
uint32_t cnt = 0;
Page pMain;
static unsigned char code icon[] = {
    /*--  调入了一幅图像：C:\Users\nist\Desktop\素材\设置.bmp  --*/
    /*--  宽度x高度=32x32  --*/
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE0, 0xF8, 0xFC,
    0xFC, 0xFE, 0xFE, 0xFE, 0x0F, 0x0F, 0x03, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x80, 0xCF, 0xFF, 0xFF, 0xFF,
    0xFF, 0xFF, 0xFF, 0xFF, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF0, 0xF8, 0xFC, 0x7E, 0x3E,
    0x00, 0x00, 0x00, 0x80, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC, 0xFE, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x7F,
    0x3F, 0x1F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00,
    0x00, 0x7E, 0xFF, 0xE7, 0xC3, 0xE7, 0xFF, 0xFF, 0x3F, 0x1F, 0x0F, 0x07, 0x03, 0x01, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



uint8_t Line_Buffer[32][32];
FAT12_File f;
//uint8_t buff[128];
void main(void)
{
Again:
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M1 = 0x7f;
    P1M0 = 0x00;
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    Wifi = 0;
    Audio = 0;
    laser = 0;
    light = 0;
    sd_cs = 1;
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear();
    HardWareSPI_Init(0x00);
    FAT12_Init();
    FAT12_Fopen(&f,"PROGRAM .PRG");
    if(f.File_Id == 0xffff){
        OLED_Printf(0,0,16,"File_Error!");
        while(1);
    }
    //FAT12_Fread(&f,str0,10);
    //FAT12_Modify(&f,"Hello World",11);
    Gui_Init();
    Gui_CreatePage(&pMain, 128, 64);
    Gui_ClearBufferBit(&pMain, 0);

    //EA = 1;
    //Timer0_Init();
    //start = cnt;
    Gui_ClearBufferBit(&pMain, 1);
    // Gui_DrawBmp(&pMain,0, 0, 32, 32, icon);
    // Gui_SetPosAbs(&pMain,32,0);
    // Gui_Flush(&pMain);
    //end = cnt;
    //OLED_Printf(0, 0, 16, "%lu", end - start);

   // memset(buff, 0, 128);
    //Cal_Input(&pMain, buff, 1);
    //Editor(&pMain);
    for(i = 0 ; i < 32; i++) for(j = 0; j < 32; j++) Line_Buffer[i][j] = 0;
    while (Key_Scan() != 0xffff);
    Editor_Main(&pMain, Line_Buffer);
    while (Key_Scan() != 0xffff);
    Gui_ClearBufferBit(&pMain,0);
    Gui_Flush(&pMain);
    //OLED_Printf(0,2,16,"%s",Line_Buffer[0]);while(1);
	Calculate_Init();
    EA = 1;
    Tick_Init_1ms();
    //OLED_Printf(0,0,16,"%bu %bu", Get_NowLine(),strlen(Line_Buffer[Get_NowLine()]));while(1);
   
	while(1){
		Get_NowLine();
		Get_ErrorCode();
		//cout<<"start:"<<Get_NowLine()<<endl;
		if(Get_NowLine() > 32){
			
			//cout<<endl<<endl<<"Finish!"<<endl;
            OLED_Printf(0,6,16,"Program End");
            while(1){
                
            }
			return;
		}
		if(Get_ErrorCode()){
			//printf("Error %bu at Line %bu\n",Get_ErrorCode(),Get_NowLine());
            OLED_Printf(0,6,16,"Error:(%bu,%bu)",Get_NowLine(),Get_ErrorCode());
            while(1){
            
            }
			return;
		}
		if (strlen(Line_Buffer[Get_NowLine() - 1])) Calculate_Expression(Line_Buffer[Get_NowLine() - 1],strlen(Line_Buffer[Get_NowLine() - 1]));
		//cout<<Get_NowLine()<<":"<<str[Get_NowLine()]<<"Debug:"<<(int)Get_NowLine()<<endl; 
		Set_NowLine(Get_NowLine()+1);
	}
    
    while (1)
    {

        // led1.Set_Status(&led1,1);
        // delayms(1000);
        // led1.Set_Status(&led1,0);
        // delayms(1000);
        // led1.LED_Control(&led1);

        // OLED_Printf(0,0 ,16, "%bd",led1.Status);
        // Gui_ClearBufferBit(&pMain, 0);
        // Gui_DrawRectangleRound(&pMain,10,10,20,20,4,1);
        // Gui_Flush(&pMain);
    }
}
