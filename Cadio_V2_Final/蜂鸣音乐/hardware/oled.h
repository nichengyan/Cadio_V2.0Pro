#ifndef __OLED_H
#define __OLED_H


#include <intrins.h>
#include "TYPE.h"
#include "_STC8H_.h"

#define MODE 0 //0:iic,1:spi
	
#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

//---------------------iic------------------------

#if (MODE==0)

sbit OLED_SCL=P2^5;//SCL
sbit OLED_SDA=P2^4;//SDA
sbit OLED_RES =P6^2;//RES

#define OLED_SCL_Clr() OLED_SCL=0
#define OLED_SCL_Set() OLED_SCL=1

#define OLED_SDA_Clr() OLED_SDA=0
#define OLED_SDA_Set() OLED_SDA=1

#define OLED_RES_Clr() OLED_RES=0
#define OLED_RES_Set() OLED_RES=1

#endif


//-----------------------spi---------------------
#if (MODE==1) 
sbit OLED_SCL=P2^4;//SCL
sbit OLED_SDA=P2^5;//SDA
sbit OLED_RES =P1^2;//RES
sbit OLED_DC =P0^1;//DC
sbit OLED_CS=P2^2; //CS

#define OLED_SCL_Clr() OLED_SCL=0
#define OLED_SCL_Set() OLED_SCL=1

#define OLED_SDA_Clr() OLED_SDA=0
#define OLED_SDA_Set() OLED_SDA=1

#define OLED_RES_Clr() OLED_RES=0
#define OLED_RES_Set() OLED_RES=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

#endif

//OLED控制用函数
void delay_ms(unsigned int ms);
void OLED_ColorTurn(u8 i);
void OLED_DisplayTurn(u8 i);
void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Set_Pos(u8 x, u8 y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 sizey);
u32 oled_pow(u8 m,u8 n);
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey);
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 sizey);
void OLED_Printf(u8 x,u8 y,u8 sizey,const char *str,...);
void OLED_DrawBMP(u8 x,u8 y,u8 sizex, u8 sizey,u8 BMP[]);
void OLED_Init(void);


#endif  
	 



