/*---------------------------------------------------------------------*/
/* --- STC MCU Limited ------------------------------------------------*/
/* --- STC 1T Series MCU Demo Programme -------------------------------*/
/* --- Mobile: (86)13922805190 ----------------------------------------*/
/* --- Fax: 86-0513-55012956,55012947,55012969 ------------------------*/
/* --- Tel: 86-0513-55012928,55012929,55012966 ------------------------*/
/* --- Web: www.STCMCU.com --------------------------------------------*/
/* --- Web: www.STCMCUDATA.com  ---------------------------------------*/
/* --- QQ:  800003751 -------------------------------------------------*/
/* 如果要在程序�?使用此代�?,请在程序�?注明使用了STC的资料及程序        */
/*---------------------------------------------------------------------*/

#ifndef _STC8X_H
#define _STC8X_H

#include <intrins.h>

/*  BYTE Registers  */
sfr P0 = 0x80;
sfr SP = 0x81;
sfr DPL = 0x82;
sfr DPH = 0x83;
sfr S4CON = 0x84;
sfr S4BUF = 0x85;
sfr PCON = 0x87;

sfr TCON = 0x88;
sfr TMOD = 0x89;
sfr TL0 = 0x8A;
sfr TL1 = 0x8B;
sfr TH0 = 0x8C;
sfr TH1 = 0x8D;
sfr AUXR = 0x8E;
sfr INT_CLKO = 0x8F;
sfr INTCLKO = 0x8F;

sfr P1 = 0x90;
sfr P1M1 = 0x91; // P1M1.n,P1M0.n 	=00--->Standard,	01--->push-pull		实际�?1T的都一�?
sfr P1M0 = 0x92; //					=10--->pure input,	11--->open drain
sfr P0M1 = 0x93; // P0M1.n,P0M0.n 	=00--->Standard,	01--->push-pull
sfr P0M0 = 0x94; //					=10--->pure input,	11--->open drain
sfr P2M1 = 0x95; // P2M1.n,P2M0.n 	=00--->Standard,	01--->push-pull
sfr P2M0 = 0x96; //					=10--->pure input,	11--->open drain

sfr SCON = 0x98;
sfr SBUF = 0x99;
sfr S2CON = 0x9A;   //
sfr S2BUF = 0x9B;   //
sfr IRCBAND = 0x9D; // IRC频�?�选择检�?
sfr LIRTRIM = 0x9E; // IRC频率�?调寄存器
sfr IRTRIM = 0x9F;  // IRC频率调整寄存�?

sfr P2 = 0xA0;
sfr P_SW1 = 0xA2; // 外�?��??口切换寄存器1

sfr IE = 0xA8;
sfr SADDR = 0xA9;
sfr WKTCL = 0xAA; // 唤醒定时器低字节
sfr WKTCH = 0xAB; // 唤醒定时器高字节
sfr S3CON = 0xAC;
sfr S3BUF = 0xAD;
sfr TA = 0xAE;  // DPTR时序控制寄存�?
sfr IE2 = 0xAF; // �?�?允�?�寄存器2

sfr P3 = 0xB0;
sfr P3M1 = 0xB1; // P3M1.n,P3M0.n 	=00--->Standard,	01--->push-pull
sfr P3M0 = 0xB2; //					=10--->pure input,	11--->open drain
sfr P4M1 = 0xB3; // P4M1.n,P4M0.n 	=00--->Standard,	01--->push-pull
sfr P4M0 = 0xB4; //					=10--->pure input,	11--->open drain
sfr IP2 = 0xB5;  //
sfr IP2H = 0xB6; //
sfr IPH = 0xB7;

sfr IP = 0xB8;
sfr SADEN = 0xB9;
sfr P_SW2 = 0xBA;     // 外�?��??口切换寄存器2
sfr ADC_CONTR = 0xBC; // ADC控制寄存�?
sfr ADC_RES = 0xBD;   // ADC结果高字�?
sfr ADC_RESL = 0xBE;  // ADC结果低字�?

sfr P4 = 0xC0;
sfr WDT_CONTR = 0xC1;
sfr IAP_DATA = 0xC2;
sfr IAP_ADDRH = 0xC3;
sfr IAP_ADDRL = 0xC4;
sfr IAP_CMD = 0xC5;
sfr IAP_TRIG = 0xC6;
sfr IAP_CONTR = 0xC7;

sfr ISP_DATA = 0xC2;
sfr ISP_ADDRH = 0xC3;
sfr ISP_ADDRL = 0xC4;
sfr ISP_CMD = 0xC5;
sfr ISP_TRIG = 0xC6;
sfr ISP_CONTR = 0xC7;

sfr P5 = 0xC8;     //
sfr P5M1 = 0xC9;   //	P5M1.n,P5M0.n 	=00--->Standard,	01--->push-pull
sfr P5M0 = 0xCA;   //					=10--->pure input,	11--->open drain
sfr P6M1 = 0xCB;   //	P6M1.n,P6M0.n 	=00--->Standard,	01--->push-pull
sfr P6M0 = 0xCC;   //					=10--->pure input,	11--->open drain
sfr SPSTAT = 0xCD; // SPI状态寄存器
sfr SPCTL = 0xCE;  // SPI控制寄存�?
sfr SPDAT = 0xCF;  // SPI数据寄存�?

sfr PSW = 0xD0;
sfr T4T3M = 0xD1;
sfr T4H = 0xD2;
sfr T4L = 0xD3;
sfr T3H = 0xD4;
sfr T3L = 0xD5;
sfr T2H = 0xD6;
sfr T2L = 0xD7;

sfr TH4 = 0xD2;
sfr TL4 = 0xD3;
sfr TH3 = 0xD4;
sfr TL3 = 0xD5;
sfr TH2 = 0xD6;
sfr TL2 = 0xD7;

sfr USBCLK = 0xDC; // USB时钟控制寄存器�?
sfr ADCCFG = 0xDE; // ADC配置寄存�?
sfr IP3 = 0xDF;    // �?�?优先级控制寄存器3

sfr ACC = 0xE0;
sfr P7M1 = 0xE1;   //	P7M1.n,P7M0.n 	=00--->Standard,	01--->push-pull
sfr P7M0 = 0xE2;   //					=10--->pure input,	11--->open drain
sfr DPS = 0xE3;    // DPTR指针选择寄存�?
sfr DPL1 = 0xE4;   // �?2组数�?指针低字�?
sfr DPH1 = 0xE5;   // �?2组数�?指针高字�?
sfr CMPCR1 = 0xE6; // 比较器控制寄存器1
sfr CMPCR2 = 0xE7; // 比较器控制寄存器2

sfr P6 = 0xE8;
sfr USBDAT = 0xEC;   // USB数据寄存�?
sfr IP3H = 0xEE;     // 高中�?优先级控制寄存器3
sfr AUXINTIF = 0xEF; // 扩展外部�?�?标志寄存�? B6-INT4IF, B5-INT3IF, B4-INT2IF, B2-T4IF, B1-T3IF, B0-T2IF

sfr B = 0xF0;
sfr USBCON = 0xF4;  // USB控制寄存器�?
sfr IAP_TPS = 0xf5; // IAP等待时间控制寄存�?

sfr P7 = 0xF8;
sfr USBADR = 0xFC; // USB地址寄存�?
sfr RSTCFG = 0xFF; // 复位配置寄存�?

//=============================================================================================================
#define COMEN (*(unsigned char volatile xdata *)0xfb00)   /* COM使能寄存�? */
#define SEGENL (*(unsigned char volatile xdata *)0xfb01)  /* SEG使能寄存�? */
#define SEGENH (*(unsigned char volatile xdata *)0xfb02)  /* SEG使能寄存�? */
#define LEDCTRL (*(unsigned char volatile xdata *)0xfb03) /* LED控制寄存�? */
#define LEDCKS (*(unsigned char volatile xdata *)0xfb04)  /* LED时钟分�?�寄存器 */

#define COM0_DAL (*(unsigned char volatile xdata *)0xfb10) /* 共阳模式显示数据 */
#define COM1_DAL (*(unsigned char volatile xdata *)0xfb11) /* 共阳模式显示数据 */
#define COM2_DAL (*(unsigned char volatile xdata *)0xfb12) /* 共阳模式显示数据 */
#define COM3_DAL (*(unsigned char volatile xdata *)0xfb13) /* 共阳模式显示数据 */
#define COM4_DAL (*(unsigned char volatile xdata *)0xfb14) /* 共阳模式显示数据 */
#define COM5_DAL (*(unsigned char volatile xdata *)0xfb15) /* 共阳模式显示数据 */
#define COM6_DAL (*(unsigned char volatile xdata *)0xfb16) /* 共阳模式显示数据 */
#define COM7_DAL (*(unsigned char volatile xdata *)0xfb17) /* 共阳模式显示数据 */
#define COM0_DAH (*(unsigned char volatile xdata *)0xfb18) /* 共阳模式显示数据 */
#define COM1_DAH (*(unsigned char volatile xdata *)0xfb19) /* 共阳模式显示数据 */
#define COM2_DAH (*(unsigned char volatile xdata *)0xfb1a) /* 共阳模式显示数据 */
#define COM3_DAH (*(unsigned char volatile xdata *)0xfb1b) /* 共阳模式显示数据 */
#define COM4_DAH (*(unsigned char volatile xdata *)0xfb1c) /* 共阳模式显示数据 */
#define COM5_DAH (*(unsigned char volatile xdata *)0xfb1d) /* 共阳模式显示数据 */
#define COM6_DAH (*(unsigned char volatile xdata *)0xfb1e) /* 共阳模式显示数据 */
#define COM7_DAH (*(unsigned char volatile xdata *)0xfb1f) /* 共阳模式显示数据 */

#define COM0_DCL (*(unsigned char volatile xdata *)0xfb20) /* 共阴模式显示数据 */
#define COM1_DCL (*(unsigned char volatile xdata *)0xfb21) /* 共阴模式显示数据 */
#define COM2_DCL (*(unsigned char volatile xdata *)0xfb22) /* 共阴模式显示数据 */
#define COM3_DCL (*(unsigned char volatile xdata *)0xfb23) /* 共阴模式显示数据 */
#define COM4_DCL (*(unsigned char volatile xdata *)0xfb24) /* 共阴模式显示数据 */
#define COM5_DCL (*(unsigned char volatile xdata *)0xfb25) /* 共阴模式显示数据 */
#define COM6_DCL (*(unsigned char volatile xdata *)0xfb26) /* 共阴模式显示数据 */
#define COM7_DCL (*(unsigned char volatile xdata *)0xfb27) /* 共阴模式显示数据 */
#define COM0_DCH (*(unsigned char volatile xdata *)0xfb28) /* 共阴模式显示数据 */
#define COM1_DCH (*(unsigned char volatile xdata *)0xfb29) /* 共阴模式显示数据 */
#define COM2_DCH (*(unsigned char volatile xdata *)0xfb2a) /* 共阴模式显示数据 */
#define COM3_DCH (*(unsigned char volatile xdata *)0xfb2b) /* 共阴模式显示数据 */
#define COM4_DCH (*(unsigned char volatile xdata *)0xfb2c) /* 共阴模式显示数据 */
#define COM5_DCH (*(unsigned char volatile xdata *)0xfb2d) /* 共阴模式显示数据 */
#define COM6_DCH (*(unsigned char volatile xdata *)0xfb2e) /* 共阴模式显示数据 */
#define COM7_DCH (*(unsigned char volatile xdata *)0xfb2f) /* 共阴模式显示数据 */

#define TSCHEN (*(unsigned int volatile xdata *)0xfb40) /* 触摸按键使能寄存�? */
#define TSCFG (*(unsigned int volatile xdata *)0xfb42)  /* 触摸按键配置寄存�?1 */
#define TSSTA (*(unsigned int volatile xdata *)0xfb46)  /* 触摸按键状态寄存器1 */
#define TSDAT (*(unsigned int volatile xdata *)0xfb49)  /* 触摸按键数据 */
#define TSTH00 (*(unsigned int volatile xdata *)0xfb50) /* 触摸按键0门�?�值高字节 */
#define TSTH01 (*(unsigned int volatile xdata *)0xfb52) /* 触摸按键1门�?�值高字节 */
#define TSTH02 (*(unsigned int volatile xdata *)0xfb54) /* 触摸按键2门�?�值高字节 */
#define TSTH03 (*(unsigned int volatile xdata *)0xfb56) /* 触摸按键3门�?�值高字节 */
#define TSTH04 (*(unsigned int volatile xdata *)0xfb58) /* 触摸按键4门�?�值高字节 */
#define TSTH05 (*(unsigned int volatile xdata *)0xfb5a) /* 触摸按键5门�?�值高字节 */
#define TSTH06 (*(unsigned int volatile xdata *)0xfb5c) /* 触摸按键6门�?�值高字节 */
#define TSTH07 (*(unsigned int volatile xdata *)0xfb5e) /* 触摸按键7门�?�值高字节 */
#define TSTH08 (*(unsigned int volatile xdata *)0xfb60) /* 触摸按键8门�?�值高字节 */
#define TSTH09 (*(unsigned int volatile xdata *)0xfb62) /* 触摸按键9门�?�值高字节 */
#define TSTH10 (*(unsigned int volatile xdata *)0xfb64) /* 触摸按键10门�?�值高字节 */
#define TSTH11 (*(unsigned int volatile xdata *)0xfb66) /* 触摸按键11门�?�值高字节 */
#define TSTH12 (*(unsigned int volatile xdata *)0xfb68) /* 触摸按键12门�?�值高字节 */
#define TSTH13 (*(unsigned int volatile xdata *)0xfb6a) /* 触摸按键13门�?�值高字节 */
#define TSTH14 (*(unsigned int volatile xdata *)0xfb6c) /* 触摸按键14门�?�值高字节 */
#define TSTH15 (*(unsigned int volatile xdata *)0xfb6e) /* 触摸按键15门�?�值高字节 */

#define TSCHEN1 (*(unsigned char volatile xdata *)0xfb40) /* 触摸按键使能寄存�?1 */
#define TSCHEN2 (*(unsigned char volatile xdata *)0xfb41) /* 触摸按键使能寄存�?2 */
#define TSCFG1 (*(unsigned char volatile xdata *)0xfb42)  /* 触摸按键配置寄存�?1 */
#define TSCFG2 (*(unsigned char volatile xdata *)0xfb43)  /* 触摸按键配置寄存�?2 */
#define TSWUTC (*(unsigned char volatile xdata *)0xfb44)  /* 触摸按键唤醒控制寄存�? */
#define TSCTRL (*(unsigned char volatile xdata *)0xfb45)  /* 触摸按键控制寄存�?  */
#define TSSTA1 (*(unsigned char volatile xdata *)0xfb46)  /* 触摸按键状态寄存器1 */
#define TSSTA2 (*(unsigned char volatile xdata *)0xfb47)  /* 触摸按键状态寄存器2 */

#define TSRT (*(unsigned char volatile xdata *)0xfb48)   /* 触摸按键时间控制寄存�? */
#define TSDATH (*(unsigned char volatile xdata *)0xfb49) /* 触摸按键数据高字�? */
#define TSDATL (*(unsigned char volatile xdata *)0xfb4a) /* 触摸按键数据低字�? */

#define TSTH00H (*(unsigned char volatile xdata *)0xfb50) /* 触摸按键0门�?�值高字节 */
#define TSTH00L (*(unsigned char volatile xdata *)0xfb51) /* 触摸按键0门�?�值低字节 */
#define TSTH01H (*(unsigned char volatile xdata *)0xfb52) /* 触摸按键1门�?�值高字节 */
#define TSTH01L (*(unsigned char volatile xdata *)0xfb53) /* 触摸按键1门�?�值低字节 */
#define TSTH02H (*(unsigned char volatile xdata *)0xfb54) /* 触摸按键2门�?�值高字节 */
#define TSTH02L (*(unsigned char volatile xdata *)0xfb55) /* 触摸按键2门�?�值低字节 */
#define TSTH03H (*(unsigned char volatile xdata *)0xfb56) /* 触摸按键3门�?�值高字节 */
#define TSTH03L (*(unsigned char volatile xdata *)0xfb57) /* 触摸按键3门�?�值低字节 */
#define TSTH04H (*(unsigned char volatile xdata *)0xfb58) /* 触摸按键4门�?�值高字节 */
#define TSTH04L (*(unsigned char volatile xdata *)0xfb59) /* 触摸按键4门�?�值低字节 */
#define TSTH05H (*(unsigned char volatile xdata *)0xfb5a) /* 触摸按键5门�?�值高字节 */
#define TSTH05L (*(unsigned char volatile xdata *)0xfb5b) /* 触摸按键5门�?�值低字节 */
#define TSTH06H (*(unsigned char volatile xdata *)0xfb5c) /* 触摸按键6门�?�值高字节 */
#define TSTH06L (*(unsigned char volatile xdata *)0xfb5d) /* 触摸按键6门�?�值低字节 */
#define TSTH07H (*(unsigned char volatile xdata *)0xfb5e) /* 触摸按键7门�?�值高字节 */
#define TSTH07L (*(unsigned char volatile xdata *)0xfb5f) /* 触摸按键7门�?�值低字节 */
#define TSTH08H (*(unsigned char volatile xdata *)0xfb60) /* 触摸按键8门�?�值高字节 */
#define TSTH08L (*(unsigned char volatile xdata *)0xfb61) /* 触摸按键8门�?�值低字节 */
#define TSTH09H (*(unsigned char volatile xdata *)0xfb62) /* 触摸按键9门�?�值高字节 */
#define TSTH09L (*(unsigned char volatile xdata *)0xfb63) /* 触摸按键9门�?�值低字节 */
#define TSTH10H (*(unsigned char volatile xdata *)0xfb64) /* 触摸按键10门�?�值高字节 */
#define TSTH10L (*(unsigned char volatile xdata *)0xfb65) /* 触摸按键10门�?�值低字节 */
#define TSTH11H (*(unsigned char volatile xdata *)0xfb66) /* 触摸按键11门�?�值高字节 */
#define TSTH11L (*(unsigned char volatile xdata *)0xfb67) /* 触摸按键11门�?�值低字节 */
#define TSTH12H (*(unsigned char volatile xdata *)0xfb68) /* 触摸按键12门�?�值高字节 */
#define TSTH12L (*(unsigned char volatile xdata *)0xfb69) /* 触摸按键12门�?�值低字节 */
#define TSTH13H (*(unsigned char volatile xdata *)0xfb6a) /* 触摸按键13门�?�值高字节 */
#define TSTH13L (*(unsigned char volatile xdata *)0xfb6b) /* 触摸按键13门�?�值低字节 */
#define TSTH14H (*(unsigned char volatile xdata *)0xfb6c) /* 触摸按键14门�?�值高字节 */
#define TSTH14L (*(unsigned char volatile xdata *)0xfb6d) /* 触摸按键14门�?�值低字节 */
#define TSTH15H (*(unsigned char volatile xdata *)0xfb6e) /* 触摸按键15门�?�值高字节 */
#define TSTH15L (*(unsigned char volatile xdata *)0xfb6f) /* 触摸按键15门�?�值低字节 */

#define MD3U32 (*(unsigned long volatile xdata *)0xfcf0) /* MDU数据寄存�? */
#define MD3U16 (*(unsigned int volatile xdata *)0xfcf0)  /* MDU数据寄存�? */
#define MD1U16 (*(unsigned int volatile xdata *)0xfcf2)  /* MDU数据寄存�? */
#define MD5U16 (*(unsigned int volatile xdata *)0xfcf4)  /* MDU数据寄存�? */

#define MD3 (*(unsigned char volatile xdata *)0xfcf0)   /* MDU数据寄存�? */
#define MD2 (*(unsigned char volatile xdata *)0xfcf1)   /* MDU数据寄存�? */
#define MD1 (*(unsigned char volatile xdata *)0xfcf2)   /* MDU数据寄存�? */
#define MD0 (*(unsigned char volatile xdata *)0xfcf3)   /* MDU数据寄存�? */
#define MD5 (*(unsigned char volatile xdata *)0xfcf4)   /* MDU数据寄存�? */
#define MD4 (*(unsigned char volatile xdata *)0xfcf5)   /* MDU数据寄存�? */
#define ARCON (*(unsigned char volatile xdata *)0xfcf6) /* MDU模式控制寄存�? */
#define OPCON (*(unsigned char volatile xdata *)0xfcf7) /* MDU操作控制寄存�? */

#define P0INTE (*(unsigned char volatile xdata *)0xfd00) /* P0口中�?使能寄存�? */
#define P1INTE (*(unsigned char volatile xdata *)0xfd01) /* P1口中�?使能寄存�? */
#define P2INTE (*(unsigned char volatile xdata *)0xfd02) /* P2口中�?使能寄存�? */
#define P3INTE (*(unsigned char volatile xdata *)0xfd03) /* P3口中�?使能寄存�? */
#define P4INTE (*(unsigned char volatile xdata *)0xfd04) /* P4口中�?使能寄存�? */
#define P5INTE (*(unsigned char volatile xdata *)0xfd05) /* P5口中�?使能寄存�? */
#define P6INTE (*(unsigned char volatile xdata *)0xfd06) /* P6口中�?使能寄存�? */
#define P7INTE (*(unsigned char volatile xdata *)0xfd07) /* P7口中�?使能寄存�? */

#define P0INTF (*(unsigned char volatile xdata *)0xfd10) /* P0口中�?标志寄存�? */
#define P1INTF (*(unsigned char volatile xdata *)0xfd11) /* P1口中�?标志寄存�? */
#define P2INTF (*(unsigned char volatile xdata *)0xfd12) /* P2口中�?标志寄存�? */
#define P3INTF (*(unsigned char volatile xdata *)0xfd13) /* P3口中�?标志寄存�? */
#define P4INTF (*(unsigned char volatile xdata *)0xfd14) /* P4口中�?标志寄存�? */
#define P5INTF (*(unsigned char volatile xdata *)0xfd15) /* P5口中�?标志寄存�? */
#define P6INTF (*(unsigned char volatile xdata *)0xfd16) /* P6口中�?标志寄存�? */
#define P7INTF (*(unsigned char volatile xdata *)0xfd17) /* P7口中�?标志寄存�? */

#define P0IM0 (*(unsigned char volatile xdata *)0xfd20) /* P0口中�?模式寄存�?0 */
#define P1IM0 (*(unsigned char volatile xdata *)0xfd21) /* P1口中�?模式寄存�?0 */
#define P2IM0 (*(unsigned char volatile xdata *)0xfd22) /* P2口中�?模式寄存�?0 */
#define P3IM0 (*(unsigned char volatile xdata *)0xfd23) /* P3口中�?模式寄存�?0 */
#define P4IM0 (*(unsigned char volatile xdata *)0xfd24) /* P4口中�?模式寄存�?0 */
#define P5IM0 (*(unsigned char volatile xdata *)0xfd25) /* P5口中�?模式寄存�?0 */
#define P6IM0 (*(unsigned char volatile xdata *)0xfd26) /* P6口中�?模式寄存�?0 */
#define P7IM0 (*(unsigned char volatile xdata *)0xfd27) /* P7口中�?模式寄存�?0 */

#define P0IM1 (*(unsigned char volatile xdata *)0xfd30) /* P0口中�?模式寄存�?1 */
#define P1IM1 (*(unsigned char volatile xdata *)0xfd31) /* P1口中�?模式寄存�?1 */
#define P2IM1 (*(unsigned char volatile xdata *)0xfd32) /* P2口中�?模式寄存�?1 */
#define P3IM1 (*(unsigned char volatile xdata *)0xfd33) /* P3口中�?模式寄存�?1 */
#define P4IM1 (*(unsigned char volatile xdata *)0xfd34) /* P4口中�?模式寄存�?1 */
#define P5IM1 (*(unsigned char volatile xdata *)0xfd35) /* P5口中�?模式寄存�?1 */
#define P6IM1 (*(unsigned char volatile xdata *)0xfd36) /* P6口中�?模式寄存�?1 */
#define P7IM1 (*(unsigned char volatile xdata *)0xfd37) /* P7口中�?模式寄存�?1 */

#define INTE_0 0x01 /* 使能Px.0口中�?功能 */
#define INTE_1 0x02 /* 使能Px.1口中�?功能 */
#define INTE_2 0x04 /* 使能Px.2口中�?功能 */
#define INTE_3 0x08 /* 使能Px.3口中�?功能 */
#define INTE_4 0x10 /* 使能Px.4口中�?功能 */
#define INTE_5 0x20 /* 使能Px.5口中�?功能 */
#define INTE_6 0x40 /* 使能Px.6口中�?功能 */
#define INTE_7 0x80 /* 使能Px.7口中�?功能 */

#define INTF_0 0x01 /* 使能Px.0口中�?标志 */
#define INTF_1 0x02 /* 使能Px.1口中�?标志 */
#define INTF_2 0x04 /* 使能Px.2口中�?标志 */
#define INTF_3 0x08 /* 使能Px.3口中�?标志 */
#define INTF_4 0x10 /* 使能Px.4口中�?标志 */
#define INTF_5 0x20 /* 使能Px.5口中�?标志 */
#define INTF_6 0x40 /* 使能Px.6口中�?标志 */
#define INTF_7 0x80 /* 使能Px.7口中�?标志 */

#define INT_FALL 0 /* 下降沿中�?   */
#define INT_RISE 1 /* 上升沿中�?   */
#define INT_LOW 2  /* 低电平中�?   */
#define INT_HIGH 3 /* 高电平中�?   */

#define CLKSEL (*(unsigned char volatile xdata *)0xfe00)
#define CKSEL (*(unsigned char volatile xdata *)0xfe00)  /* 主时钟源选择    */
#define CLKDIV (*(unsigned char volatile xdata *)0xfe01) /* 主时钟分�?      */
#define CKDIV (*(unsigned char volatile xdata *)0xfe01)  /* 主时钟分�?      */
// #define	IRC24MCR	(*(unsigned char volatile xdata *)0xfe02)	/* IRC 24MHZ控制   */
#define HIRCCR (*(unsigned char volatile xdata *)0xfe02)   /* 内部高速振荡器控制寄存�? */
#define XOSCCR (*(unsigned char volatile xdata *)0xfe03)   /* XOSC控制        */
#define IRC32KCR (*(unsigned char volatile xdata *)0xfe04) /* IRC 32KHZ控制   */
#define MCLKOCR (*(unsigned char volatile xdata *)0xfe05)  /* 主时钟输出控制寄存器  */
#define IRCDB (*(unsigned char volatile xdata *)0xfe06)    /* 内部高速振荡器去抖控制寄存�?   */
#define IRC48MCR (*(unsigned char volatile xdata *)0xfe07) /* 内部48M�?荡器控制寄存�?  */
#define SPFUNC (*(unsigned char volatile xdata *)0xfe08)   /* 特殊功能控制寄存�?   */
#define RSTFLAG (*(unsigned char volatile xdata *)0xfe09)  /* 复位标志寄存�?  */

#define P0PU (*(unsigned char volatile xdata *)0xfe10) /* P0口内�?3.7K上拉电阻, 0: 禁�??(默�??), 1: 允�??  */
#define P1PU (*(unsigned char volatile xdata *)0xfe11) /* P1口内�?3.7K上拉电阻  */
#define P2PU (*(unsigned char volatile xdata *)0xfe12) /* P2口内�?3.7K上拉电阻  */
#define P3PU (*(unsigned char volatile xdata *)0xfe13) /* P3口内�?3.7K上拉电阻  */
#define P4PU (*(unsigned char volatile xdata *)0xfe14) /* P4口内�?3.7K上拉电阻  */
#define P5PU (*(unsigned char volatile xdata *)0xfe15) /* P5口内�?3.7K上拉电阻  */
#define P6PU (*(unsigned char volatile xdata *)0xfe16) /* P6口内�?3.7K上拉电阻  */
#define P7PU (*(unsigned char volatile xdata *)0xfe17) /* P7口内�?3.7K上拉电阻  */

#define P0NCS (*(unsigned char volatile xdata *)0xfe18) /* P0 Non Schmit Trigger  0: 使能�?口施密特触发功能(默�??), 1: 禁�??  */
#define P1NCS (*(unsigned char volatile xdata *)0xfe19) /* P1 Non Schmit Trigger  */
#define P2NCS (*(unsigned char volatile xdata *)0xfe1a) /* P2 Non Schmit Trigger  */
#define P3NCS (*(unsigned char volatile xdata *)0xfe1b) /* P3 Non Schmit Trigger  */
#define P4NCS (*(unsigned char volatile xdata *)0xfe1c) /* P4 Non Schmit Trigger  */
#define P5NCS (*(unsigned char volatile xdata *)0xfe1d) /* P5 Non Schmit Trigger  */
#define P6NCS (*(unsigned char volatile xdata *)0xfe1e) /* P6 Non Schmit Trigger  */
#define P7NCS (*(unsigned char volatile xdata *)0xfe1f) /* P7 Non Schmit Trigger  */

#define P0SR (*(unsigned char volatile xdata *)0xfe20) /* P0口电平转换速率寄存�? 默�??1: �?通模�?, 0:高速模�? */
#define P1SR (*(unsigned char volatile xdata *)0xfe21) /* P1口电平转换速率寄存�?  */
#define P2SR (*(unsigned char volatile xdata *)0xfe22) /* P2口电平转换速率寄存�?  */
#define P3SR (*(unsigned char volatile xdata *)0xfe23) /* P3口电平转换速率寄存�?  */
#define P4SR (*(unsigned char volatile xdata *)0xfe24) /* P4口电平转换速率寄存�?  */
#define P5SR (*(unsigned char volatile xdata *)0xfe25) /* P5口电平转换速率寄存�?  */
#define P6SR (*(unsigned char volatile xdata *)0xfe26) /* P6口电平转换速率寄存�?  */
#define P7SR (*(unsigned char volatile xdata *)0xfe27) /* P7口电平转换速率寄存�?  */

#define P0DR (*(unsigned char volatile xdata *)0xfe28) /* P0口驱动电流控制寄存器 默�??1: �?通模�?, 0:大电流模�?*/
#define P1DR (*(unsigned char volatile xdata *)0xfe29) /* P1口驱动电流控制寄存器 */
#define P2DR (*(unsigned char volatile xdata *)0xfe2a) /* P2口驱动电流控制寄存器 */
#define P3DR (*(unsigned char volatile xdata *)0xfe2b) /* P3口驱动电流控制寄存器 */
#define P4DR (*(unsigned char volatile xdata *)0xfe2c) /* P4口驱动电流控制寄存器 */
#define P5DR (*(unsigned char volatile xdata *)0xfe2d) /* P5口驱动电流控制寄存器 */
#define P6DR (*(unsigned char volatile xdata *)0xfe2e) /* P6口驱动电流控制寄存器 */
#define P7DR (*(unsigned char volatile xdata *)0xfe2f) /* P7口驱动电流控制寄存器 */

#define P0IE (*(unsigned char volatile xdata *)0xfe30) /* P0口输入使能控制寄存器 */
#define P1IE (*(unsigned char volatile xdata *)0xfe31) /* P1口输入使能控制寄存器 */
#define P3IE (*(unsigned char volatile xdata *)0xfe33) /* P3口输入使能控制寄存器 */

#define RTCCR (*(unsigned char volatile xdata *)0xfe60)    /* RTC控制寄存�? */
#define RTCCFG (*(unsigned char volatile xdata *)0xfe61)   /* RTC配置寄存�? */
#define RTCIEN (*(unsigned char volatile xdata *)0xfe62)   /* RTC�?�?使能寄存�? */
#define RTCIF (*(unsigned char volatile xdata *)0xfe63)    /* RTC�?�?请求寄存�? */
#define ALAHOUR (*(unsigned char volatile xdata *)0xfe64)  /* RTC闹钟的小时�? */
#define ALAMIN (*(unsigned char volatile xdata *)0xfe65)   /* RTC闹钟的分钟�? */
#define ALASEC (*(unsigned char volatile xdata *)0xfe66)   /* RTC闹钟的�?��? */
#define ALASSEC (*(unsigned char volatile xdata *)0xfe67)  /* RTC闹钟�?1/128秒�? */
#define INIYEAR (*(unsigned char volatile xdata *)0xfe68)  /* RTC年初始化 */
#define INIMONTH (*(unsigned char volatile xdata *)0xfe69) /* RTC月初始化 */
#define INIDAY (*(unsigned char volatile xdata *)0xfe6a)   /* RTC日初始化 */
#define INIHOUR (*(unsigned char volatile xdata *)0xfe6b)  /* RTC时初始化 */
#define INIMIN (*(unsigned char volatile xdata *)0xfe6c)   /* RTC分初始化 */
#define INISEC (*(unsigned char volatile xdata *)0xfe6d)   /* RTC秒初始化 */
#define INISSEC (*(unsigned char volatile xdata *)0xfe6e)  /* RTC1/128秒初始化 */
#define YEAR (*(unsigned char volatile xdata *)0xfe70)     /* RTC的年计数�? */
#define MONTH (*(unsigned char volatile xdata *)0xfe71)    /* RTC的月计数�? */
#define DAY (*(unsigned char volatile xdata *)0xfe72)      /* RTC的日计数�? */
#define HOUR (*(unsigned char volatile xdata *)0xfe73)     /* RTC的时计数�? */
#define MIN (*(unsigned char volatile xdata *)0xfe74)      /* RTC的分计数�? */
#define SEC (*(unsigned char volatile xdata *)0xfe75)      /* RTC的�?��?�数�? */
#define SSEC (*(unsigned char volatile xdata *)0xfe76)     /* RTC�?1/128秒�?�数�? */

#define I2CCFG (*(unsigned char volatile xdata *)0xfe80)   /* I2C配置寄存�?  */
#define I2CMSCR (*(unsigned char volatile xdata *)0xfe81)  /* I2C主机控制寄存�?  */
#define I2CMSST (*(unsigned char volatile xdata *)0xfe82)  /* I2C主机状态寄存器  */
#define I2CSLCR (*(unsigned char volatile xdata *)0xfe83)  /* I2C从机控制寄存�?  */
#define I2CSLST (*(unsigned char volatile xdata *)0xfe84)  /* I2C从机状态寄存器  */
#define I2CSLADR (*(unsigned char volatile xdata *)0xfe85) /* I2C从机地址寄存�?  */
#define I2CTXD (*(unsigned char volatile xdata *)0xfe86)   /* I2C数据发送寄存器  */
#define I2CRXD (*(unsigned char volatile xdata *)0xfe87)   /* I2C数据接收寄存�?  */
#define I2CMSAUX (*(unsigned char volatile xdata *)0xfe88) /* I2C主机辅助控制寄存�?  */

#define TM2PS (*(unsigned char volatile xdata *)0xfea2)   /* 定时�?2时钟预分频寄存器  */
#define TM3PS (*(unsigned char volatile xdata *)0xfea3)   /* 定时�?3时钟预分频寄存器  */
#define TM4PS (*(unsigned char volatile xdata *)0xfea4)   /* 定时�?4时钟预分频寄存器  */
#define ADCTIM (*(unsigned char volatile xdata *)0xfea8)  /* ADC时序控制寄存�?  */
#define T3T4PIN (*(unsigned char volatile xdata *)0xfeac) /* T3/T4选择寄存�?  */

#define PWMA_ETRPS (*(unsigned char volatile xdata *)0xfeb0) /* PWMA的ETR选择寄存�?  */
#define PWMA_ENO (*(unsigned char volatile xdata *)0xfeb1)   /* PWMA输出使能寄存�?   */
#define PWMA_PS (*(unsigned char volatile xdata *)0xfeb2)    /* PWMA输出脚选择寄存�? */
#define PWMA_IOAUX (*(unsigned char volatile xdata *)0xfeb3) /* PWMA辅助寄存�?       */
#define PWMB_ETRPS (*(unsigned char volatile xdata *)0xfeb4) /* PWMB的ETR选择寄存�?  */
#define PWMB_ENO (*(unsigned char volatile xdata *)0xfeb5)   /* PWMB输出使能寄存�?   */
#define PWMB_PS (*(unsigned char volatile xdata *)0xfeb6)    /* PWMB输出脚选择寄存�? */
#define PWMB_IOAUX (*(unsigned char volatile xdata *)0xfeb7) /* PWMB辅助寄存�?       */

#define PWMA_CR1 (*(unsigned char volatile xdata *)0xfec0)  /* PWMA控制寄存�?1  */
#define PWMA_CR2 (*(unsigned char volatile xdata *)0xfec1)  /* PWMA控制寄存�?2  */
#define PWMA_SMCR (*(unsigned char volatile xdata *)0xfec2) /* PWMA从模式控制寄存器  */
#define PWMA_ETR (*(unsigned char volatile xdata *)0xfec3)  /* PWMA外部触发寄存�?  */
#define PWMA_IER (*(unsigned char volatile xdata *)0xfec4)  /* PWMA�?�?使能寄存�?  */
#define PWMA_SR1 (*(unsigned char volatile xdata *)0xfec5)  /* PWMA状态寄存器1  */
#define PWMA_SR2 (*(unsigned char volatile xdata *)0xfec6)  /* PWMA状态寄存器2  */
#define PWMA_EGR (*(unsigned char volatile xdata *)0xfec7)  /* PWMA时件发生寄存�?  */

#define PWMA_CCMR1 (*(unsigned char volatile xdata *)0xfec8) /* PWMA捕获/比较模式寄存�?1  */
#define PWMA_CCMR2 (*(unsigned char volatile xdata *)0xfec9) /* PWMA捕获/比较模式寄存�?2  */
#define PWMA_CCMR3 (*(unsigned char volatile xdata *)0xfeca) /* PWMA捕获/比较模式寄存�?3  */
#define PWMA_CCMR4 (*(unsigned char volatile xdata *)0xfecb) /* PWMA捕获/比较模式寄存�?4  */
#define PWMA_CCER1 (*(unsigned char volatile xdata *)0xfecc) /* PWMA捕获比较使能寄存�?1  */
#define PWMA_CCER2 (*(unsigned char volatile xdata *)0xfecd) /* PWMA捕获比较使能寄存�?2  */
#define PWMA_CNTRH (*(unsigned char volatile xdata *)0xfece) /* PWMA计数器高字节   */
#define PWMA_CNTRL (*(unsigned char volatile xdata *)0xfecf) /* PWMA计数器低字节   */

#define PWMA_PSCRH (*(unsigned char volatile xdata *)0xfed0) /* PWMA预分频器高字�? */
#define PWMA_PSCRL (*(unsigned char volatile xdata *)0xfed1) /* PWMA预分频器低字�? */
#define PWMA_ARRH (*(unsigned char volatile xdata *)0xfed2)  /* PWMA�?动重装寄存器高字�? */
#define PWMA_ARRL (*(unsigned char volatile xdata *)0xfed3)  /* PWMA�?动重装寄存器低字�? */
#define PWMA_RCR (*(unsigned char volatile xdata *)0xfed4)   /* PWMA重�?��?�数寄存�? */
#define PWMA_CCR1H (*(unsigned char volatile xdata *)0xfed5) /* PWMA比较捕获寄存�?1高字�? */
#define PWMA_CCR1L (*(unsigned char volatile xdata *)0xfed6) /* PWMA比较捕获寄存�?1低字�? */
#define PWMA_CCR2H (*(unsigned char volatile xdata *)0xfed7) /* PWMA比较捕获寄存�?2高字�? */
#define PWMA_CCR2L (*(unsigned char volatile xdata *)0xfed8) /* PWMA比较捕获寄存�?2低字�? */
#define PWMA_CCR3H (*(unsigned char volatile xdata *)0xfed9) /* PWMA比较捕获寄存�?3高字�? */
#define PWMA_CCR3L (*(unsigned char volatile xdata *)0xfeda) /* PWMA比较捕获寄存�?3低字�? */
#define PWMA_CCR4H (*(unsigned char volatile xdata *)0xfedb) /* PWMA比较捕获寄存�?4高字�? */
#define PWMA_CCR4L (*(unsigned char volatile xdata *)0xfedc) /* PWMA比较捕获寄存�?4低字�? */
#define PWMA_BKR (*(unsigned char volatile xdata *)0xfedd)   /* PWMA刹车寄存�? */
#define PWMA_DTR (*(unsigned char volatile xdata *)0xfede)   /* PWMA死区控制寄存�? */
#define PWMA_OISR (*(unsigned char volatile xdata *)0xfedf)  /* PWMA输出空闲状态寄存器 */

#define PWMB_CR1 (*(unsigned char volatile xdata *)0xfee0)  /* PWMB控制寄存�?1  */
#define PWMB_CR2 (*(unsigned char volatile xdata *)0xfee1)  /* PWMB控制寄存�?2  */
#define PWMB_SMCR (*(unsigned char volatile xdata *)0xfee2) /* PWMB从模式控制寄存器  */
#define PWMB_ETR (*(unsigned char volatile xdata *)0xfee3)  /* PWMB外部触发寄存�?  */
#define PWMB_IER (*(unsigned char volatile xdata *)0xfee4)  /* PWMB�?�?使能寄存�?  */
#define PWMB_SR1 (*(unsigned char volatile xdata *)0xfee5)  /* PWMB状态寄存器1  */
#define PWMB_SR2 (*(unsigned char volatile xdata *)0xfee6)  /* PWMB状态寄存器2  */
#define PWMB_EGR (*(unsigned char volatile xdata *)0xfee7)  /* PWMB时间发生寄存�?  */

#define PWMB_CCMR1 (*(unsigned char volatile xdata *)0xfee8) /* PWMB捕获/比较模式寄存�?1  */
#define PWMB_CCMR2 (*(unsigned char volatile xdata *)0xfee9) /* PWMB捕获/比较模式寄存�?2  */
#define PWMB_CCMR3 (*(unsigned char volatile xdata *)0xfeea) /* PWMB捕获/比较模式寄存�?3  */
#define PWMB_CCMR4 (*(unsigned char volatile xdata *)0xfeeb) /* PWMB捕获/比较模式寄存�?4  */
#define PWMB_CCER1 (*(unsigned char volatile xdata *)0xfeec) /* PWMB捕获比较使能寄存�?1  */
#define PWMB_CCER2 (*(unsigned char volatile xdata *)0xfeed) /* PWMB捕获比较使能寄存�?2  */
#define PWMB_CNTRH (*(unsigned char volatile xdata *)0xfeee) /* PWMB计数器高字节   */
#define PWMB_CNTRL (*(unsigned char volatile xdata *)0xfeef) /* PWMB计数器低字节   */

#define PWMB_PSCRH (*(unsigned char volatile xdata *)0xfef0) /* PWMB预分频器高字�? */
#define PWMB_PSCRL (*(unsigned char volatile xdata *)0xfef1) /* PWMB预分频器低字�? */
#define PWMB_ARRH (*(unsigned char volatile xdata *)0xfef2)  /* PWMB�?动重装寄存器高字�? */
#define PWMB_ARRL (*(unsigned char volatile xdata *)0xfef3)  /* PWMB�?动重装寄存器低字�? */
#define PWMB_RCR (*(unsigned char volatile xdata *)0xfef4)   /* PWMB重�?��?�数寄存�? */
#define PWMB_CCR1H (*(unsigned char volatile xdata *)0xfef5) /* PWMB比较捕获寄存�?1高字�? */
#define PWMB_CCR1L (*(unsigned char volatile xdata *)0xfef6) /* PWMB比较捕获寄存�?1低字�? */
#define PWMB_CCR2H (*(unsigned char volatile xdata *)0xfef7) /* PWMB比较捕获寄存�?2高字�? */
#define PWMB_CCR2L (*(unsigned char volatile xdata *)0xfef8) /* PWMB比较捕获寄存�?2低字�? */
#define PWMB_CCR3H (*(unsigned char volatile xdata *)0xfef9) /* PWMB比较捕获寄存�?3高字�? */
#define PWMB_CCR3L (*(unsigned char volatile xdata *)0xfefa) /* PWMB比较捕获寄存�?3低字�? */
#define PWMB_CCR4H (*(unsigned char volatile xdata *)0xfefb) /* PWMB比较捕获寄存�?4高字�? */
#define PWMB_CCR4L (*(unsigned char volatile xdata *)0xfefc) /* PWMB比较捕获寄存�?4低字�? */
#define PWMB_BKR (*(unsigned char volatile xdata *)0xfefd)   /* PWMB刹车寄存�? */
#define PWMB_DTR (*(unsigned char volatile xdata *)0xfefe)   /* PWMB死区控制寄存�? */
#define PWMB_OISR (*(unsigned char volatile xdata *)0xfeff)  /* PWMB输出空闲状态寄存器 */

#define PWMA_CNTR (*(unsigned int volatile xdata *)0xfece) /* PWMA计数�?16位�?�问 */
#define PWMA_PSCR (*(unsigned int volatile xdata *)0xfed0) /* PWMA预分频器16位�?�问 */
#define PWMA_ARR (*(unsigned int volatile xdata *)0xfed2)  /* PWMA�?动重装寄存器16位�?�问 */
#define PWMA_CCR1 (*(unsigned int volatile xdata *)0xfed5) /* PWMA比较捕获寄存�?1 16位�?�问 */
#define PWMA_CCR2 (*(unsigned int volatile xdata *)0xfed7) /* PWMA比较捕获寄存�?2 16位�?�问 */
#define PWMA_CCR3 (*(unsigned int volatile xdata *)0xfed9) /* PWMA比较捕获寄存�?3 16位�?�问 */
#define PWMA_CCR4 (*(unsigned int volatile xdata *)0xfedB) /* PWMA比较捕获寄存�?4 16位�?�问 */

#define PWMB_CNTR (*(unsigned int volatile xdata *)0xfeee) /* PWMA计数�?16位�?�问 */
#define PWMB_PSCR (*(unsigned int volatile xdata *)0xfef0) /* PWMA预分频器16位�?�问 */
#define PWMB_ARR (*(unsigned int volatile xdata *)0xfef2)  /* PWMA�?动重装寄存器16位�?�问 */
#define PWMB_CCR1 (*(unsigned int volatile xdata *)0xfef5) /* PWMA比较捕获寄存�?1 16位�?�问 */
#define PWMB_CCR2 (*(unsigned int volatile xdata *)0xfef7) /* PWMA比较捕获寄存�?2 16位�?�问 */
#define PWMB_CCR3 (*(unsigned int volatile xdata *)0xfef9) /* PWMA比较捕获寄存�?3 16位�?�问 */
#define PWMB_CCR4 (*(unsigned int volatile xdata *)0xfefB) /* PWMA比较捕获寄存�?4 16位�?�问 */

//=============================================================================================================

#define EAXSFR() P_SW2 |= 0x80  /* MOVX A,@DPTR/MOVX @DPTR,A指令的操作�?�象为扩展SFR(XSFR) */
#define EAXRAM() P_SW2 &= ~0x80 /* MOVX A,@DPTR/MOVX @DPTR,A指令的操作�?�象为扩展RAM(XRAM) */

#define I2C_USE_P15P14() P_SW2 &= ~0x30                 /* 将I2C切换到P1.5(SCL) P1.4(SDA)(上电默�??).*/
#define I2C_USE_P25P24() P_SW2 = (P_SW2 & ~0x30) | 0x10 /* 将I2C切换到P2.5(SCL) P2.4(SDA).*/
#define I2C_USE_P32P33() P_SW2 |= 0x30                  /* 将I2C切换到P3.2(SCL) P3.3(SDA).*/

#define MainFosc_IRC24M() CKSEL = (CKSEL & ~0x03)      /* 选择内部24MHZ时钟 */
#define MainFosc_XTAL() CKSEL = (CKSEL & ~0x03) | 0x01 /* 选择外部晶振或时�? */
#define EXT_CLOCK() XOSCCR = 0x80                      /* 选择外部时钟 */
#define EXT_CRYSTAL() XOSCCR = 0xC0                    /* 选择外部晶振 */
#define MainFosc_IRC32K() CKSEL = CKSEL | 0x03         /* 选择内部32K时钟 */
#define MCLKO_None() MCLKOCR = 0                       /* 主时钟不输出 	    */
#define MCLKO54_DIV(n) MCLKOCR = n                     /* 主时钟从P5.4分�?�输�? */
#define MCLKO16_DIV(n) MCLKOCR = n | 0x80              /* 主时钟从P1.6分�?�输�? */

/*  BIT Registers  */
/*  PSW   */
sbit CY = PSW ^ 7;
sbit AC = PSW ^ 6;
sbit F0 = PSW ^ 5;
sbit RS1 = PSW ^ 4;
sbit RS0 = PSW ^ 3;
sbit OV = PSW ^ 2;
sbit F1 = PSW ^ 1;
sbit P = PSW ^ 0;

/*  TCON  */
sbit TF1 = TCON ^ 7; // 定时�?1溢出�?�?标志�?
sbit TR1 = TCON ^ 6; // 定时�?1运�?�控制位
sbit TF0 = TCON ^ 5; // 定时�?0溢出�?�?标志�?
sbit TR0 = TCON ^ 4; // 定时�?0运�?�控制位
sbit IE1 = TCON ^ 3; // 外中�?1标志�?
sbit IT1 = TCON ^ 2; // 外中�?1信号方式控制位，1：下降沿�?�?�?0：上升下降均�?�?�?
sbit IE0 = TCON ^ 1; // 外中�?0标志�?
sbit IT0 = TCON ^ 0; // 外中�?0信号方式控制位，1：下降沿�?�?�?0：上升下降均�?�?�?

/*  P0  */
sbit P00 = P0 ^ 0;
sbit P01 = P0 ^ 1;
sbit P02 = P0 ^ 2;
sbit P03 = P0 ^ 3;

/*  P1  */
sbit P10 = P1 ^ 0;
sbit P11 = P1 ^ 1;
sbit P12 = P1 ^ 2;
sbit P13 = P1 ^ 3;
sbit P14 = P1 ^ 4;
sbit P15 = P1 ^ 5;
sbit P16 = P1 ^ 6;
sbit P17 = P1 ^ 7;

/*  P2  */
sbit P20 = P2 ^ 0;
sbit P21 = P2 ^ 1;
sbit P22 = P2 ^ 2;
sbit P23 = P2 ^ 3;
sbit P24 = P2 ^ 4;
sbit P25 = P2 ^ 5;
sbit P26 = P2 ^ 6;
sbit P27 = P2 ^ 7;

/*  P3  */
sbit P30 = P3 ^ 0;
sbit P31 = P3 ^ 1;
sbit P32 = P3 ^ 2;
sbit P33 = P3 ^ 3;
sbit P34 = P3 ^ 4;
sbit P35 = P3 ^ 5;
sbit P36 = P3 ^ 6;
sbit P37 = P3 ^ 7;

/*  P5  */
sbit P54 = P5 ^ 4;
sbit P55 = P5 ^ 5;

sbit RXD2 = P1 ^ 0;
sbit TXD2 = P1 ^ 1;
sbit ECI = P1 ^ 2;
sbit CCP0 = P1 ^ 1;
sbit CCP1 = P1 ^ 0;
sbit CCP2 = P3 ^ 7;
sbit ECI_2 = P3 ^ 4;
sbit CCP0_2 = P3 ^ 5;
sbit CCP1_2 = P3 ^ 6;
sbit CCP2_2 = P3 ^ 7;

sbit SPI_SS = P1 ^ 2;
sbit SPI_MOSI = P1 ^ 3;
sbit SPI_MISO = P1 ^ 4;
sbit SPI_SCLK = P1 ^ 5;

sbit SPI_SS_2 = P2 ^ 2;
sbit SPI_MOSI_2 = P2 ^ 3;
sbit SPI_MISO_2 = P2 ^ 4;
sbit SPI_SCLK_2 = P2 ^ 5;

sbit SPI_SS_4 = P3 ^ 5;
sbit SPI_MOSI_4 = P3 ^ 4;
sbit SPI_MISO_4 = P3 ^ 3;
sbit SPI_SCLK_4 = P3 ^ 2;

sbit RXD = P3 ^ 0;
sbit TXD = P3 ^ 1;
sbit INT0 = P3 ^ 2;
sbit INT1 = P3 ^ 3;
sbit T0 = P3 ^ 4;
sbit T1 = P3 ^ 5;

sbit INT2 = P3 ^ 6;
sbit INT3 = P3 ^ 7;
sbit INT4 = P3 ^ 0;

sbit CLKOUT0 = P3 ^ 5;
sbit CLKOUT1 = P3 ^ 4;

/*  SCON  */
sbit SM0 = SCON ^ 7; // SM0/FE		SM0 SM1 = 00 ~ 11: 方式0~3
sbit SM1 = SCON ^ 6; //
sbit SM2 = SCON ^ 5; // 多机通�??
sbit REN = SCON ^ 4; // 接收允�??
sbit TB8 = SCON ^ 3; // 发送数�?�?8�?
sbit RB8 = SCON ^ 2; // 接收数据�?8�?
sbit TI = SCON ^ 1;  // 发送中�?标志�?
sbit RI = SCON ^ 0;  // 接收�?�?标志�?

/*  IE   */
sbit EA = IE ^ 7;   // �?�?允�?�总控制位
sbit ELVD = IE ^ 6; // 低压监测�?�?允�?�位
sbit EADC = IE ^ 5; // ADC �?�? 允�?�位
sbit ES = IE ^ 4;   // 串�?�中�? 允�?�控制位
sbit ET1 = IE ^ 3;  // 定时�?�?1允�?�控制位
sbit EX1 = IE ^ 2;  // 外部�?�?1允�?�控制位
sbit ET0 = IE ^ 1;  // 定时�?�?0允�?�控制位
sbit EX0 = IE ^ 0;  // 外部�?�?0允�?�控制位

sbit ACC0 = ACC ^ 0;
sbit ACC1 = ACC ^ 1;
sbit ACC2 = ACC ^ 2;
sbit ACC3 = ACC ^ 3;
sbit ACC4 = ACC ^ 4;
sbit ACC5 = ACC ^ 5;
sbit ACC6 = ACC ^ 6;
sbit ACC7 = ACC ^ 7;

sbit B0 = B ^ 0;
sbit B1 = B ^ 1;
sbit B2 = B ^ 2;
sbit B3 = B ^ 3;
sbit B4 = B ^ 4;
sbit B5 = B ^ 5;
sbit B6 = B ^ 6;
sbit B7 = B ^ 7;

//                                     7    6    5    4    3     2       1       0      Reset Value
// INT_CLKO:  �?�?与时钟输出控制寄存器  -   EX4  EX3  EX2   -   T2CLKO  T1CLKO  T0CLKO    0000,0000
#define INT4_Enable() INT_CLKO |= (1 << 6)
#define INT3_Enable() INT_CLKO |= (1 << 5)
#define INT2_Enable() INT_CLKO |= (1 << 4)
#define INT1_Enable() EX1 = 1
#define INT0_Enable() EX0 = 1

#define INT4_Disable() INT_CLKO &= ~(1 << 6)
#define INT3_Disable() INT_CLKO &= ~(1 << 5)
#define INT2_Disable() INT_CLKO &= ~(1 << 4)
#define INT1_Disable() EX1 = 0
#define INT0_Disable() EX0 = 0

//                               7    6       5       4      3    2     1     0      Reset Value
// AUXINTIF:  辅助�?�?标志寄存�?  -  INT4IF  INT3IF  INT2IF   -   T4IF  T3IF  T2IF    0000,0000
#define INT4IF 0x40
#define INT3IF 0x20
#define INT2IF 0x10
#define T4IF 0x04
#define T3IF 0x02
#define T2IF 0x01

#define INT4_Clear() AUXINTIF &= ~INT4IF /* 清除外中�?4标志�? */
#define INT3_Clear() AUXINTIF &= ~INT3IF /* 清除外中�?3标志�? */
#define INT2_Clear() AUXINTIF &= ~INT2IF /* 清除外中�?2标志�? */
#define INT1_Clear() IE1 = 0             /* 清除外中�?1标志�? */
#define INT0_Clear() IE0 = 0             /* 清除外中�?0标志�? */

#define INT0_Fall() IT0 = 1     /* INT0 下降沿中�?           */
#define INT0_RiseFall() IT0 = 0 /* INT0 下降沿上升沿均中�?   */
#define INT1_Fall() IT1 = 1     /* INT1 下降沿中�?           */
#define INT1_RiseFall() IT0 = 0 /* INT1 下降沿上升沿均中�?   */

//							7     6     5    4    3    2    1     0    Reset Value
// sfr IE2       = 0xAF;		-     -     -    -    -    -   ESPI  ES2   0000,0000B	//Auxiliary Interrupt
#define SPI_INT_ENABLE() IE2 |= 2     /* 允�?�SPI�?�?		*/
#define SPI_INT_DISABLE() IE2 &= ~2   /* 允�?�SPI�?�?		*/
#define UART2_INT_ENABLE() IE2 |= 1   /* 允�?�串�?2�?�?	*/
#define UART2_INT_DISABLE() IE2 &= ~1 /* 允�?�串�?2�?�?	*/

//                                          7     6     5    4    3    2    1    0    Reset Value
// sfr IP      = 0xB8; //�?�?优先级低�?      PPCA  PLVD  PADC  PS   PT1  PX1  PT0  PX0   0000,0000
//--------
sbit PPCA = IP ^ 7; // PCA 模块�?�?优先�?
sbit PLVD = IP ^ 6; // 低压监测�?�?优先�?
sbit PADC = IP ^ 5; // ADC �?�?优先�?
sbit PS = IP ^ 4;   // 串�?�中�?0优先级�?�定�?
sbit PT1 = IP ^ 3;  // 定时�?�?1优先级�?�定�?
sbit PX1 = IP ^ 2;  // 外部�?�?1优先级�?�定�?
sbit PT0 = IP ^ 1;  // 定时�?�?0优先级�?�定�?
sbit PX0 = IP ^ 0;  // 外部�?�?0优先级�?�定�?

//                                           7      6      5     4     3     2    1     0        Reset Value
// sfr IPH   = 0xB7; //�?�?优先级高�?       PPCAH  PLVDH  PADCH  PSH  PT1H  PX1H  PT0H  PX0H   0000,0000
// sfr IP2   = 0xB5; //                       -      -      -     -     -     -   PSPI   PS2   xxxx,xx00
// sfr IP2H  = 0xB6; //                       -      -      -     -     -     -   PSPIH  PS2H  xxxx,xx00
#define PPCAH 0x80
#define PLVDH 0x40
#define PADCH 0x20
#define PSH 0x10
#define PT1H 0x08
#define PX1H 0x04
#define PT0H 0x02
#define PX0H 0x01

#define PCA_InterruptFirst() PPCA = 1
#define LVD_InterruptFirst() PLVD = 1
#define ADC_InterruptFirst() PADC = 1
#define UART1_InterruptFirst() PS = 1
#define Timer1_InterruptFirst() PT1 = 1
#define INT1_InterruptFirst() PX1 = 1
#define Timer0_InterruptFirst() PT0 = 1
#define INT0_InterruptFirst() PX0 = 1

/*************************************************************************************************/

//                       7      6     5    4    3    2     1      0        Reset Value
// sfr CMPCR1 = 0xE6;   CMPEN  CMPIF  PIE  NIE  PIS  NIS  CMPOE  CMPRES      00000000B
#define CMPEN 0x80  // 1: 允�?�比较器, 0: 禁�??,关闭比较器电�?
#define CMPIF 0x40  // 比较器中�?标志, 包括上升沿或下降沿中�?, �?件清0
#define PIE 0x20    // 1: 比较结果�?0�?1, 产生上升沿中�?
#define NIE 0x10    // 1: 比较结果�?1�?0, 产生下降沿中�?
#define PIS 0x08    // 输入正极性选择, 0: 选择内部P3.7做�?�输�?,           1: 由ADC_CHS[3:0]所选择的ADC输入�?做�?�输�?.
#define NIS 0x04    // 输入负极性选择, 0: 选择内部BandGap电压BGv做负输入, 1: 选择外部P3.6做输�?.
#define CMPOE 0x02  // 1: 允�?�比较结果输�?, 0: 禁�??.
#define CMPRES 0x01 // 比较结果, 1: CMP+电平高于CMP-,  0: CMP+电平低于CMP-,  �?�?

#define CMP_P_P37 0x00 // 输入正极性选择, 0: 选择内部P3.7做�?�输�?
#define CMP_P_ADC 0x08 // 输入正极性选择, 1: 由ADC_CHS[3:0]所选择的ADC输入�?做�?�输�?.
#define CMP_N_GAP 0x00 // 输入负极性选择, 0: 选择内部BandGap电压BGv做负输入.
#define CMP_N_P36 0x04 // 输入负极性选择, 1: 选择外部P3.6做输�?.

#define CMPO_P34() P_SW2 &= ~0x08 // 结果输出到P3.4.

//                       7        6       5  4  3  2  1  0    Reset Value
// sfr CMPCR2 = 0xE7;   INVCMPO  DISFLT       LCDTY[5:0]       00001001B
#define INVCMPO 0x80 // 1: 比较器输出IO取反,  0: 不取�?
#define DISFLT 0x40  // 1: 关闭0.1uF滤波,   	0: 允�??
#define LCDTY 0x00   // 0~63, 比较结果变化延时周期�?

/*************************************************************************************************/
//                     7     6     5    4    3    2   1   0       Reset Value
// sfr SCON  = 0x98;   SM0   SM1   SM2  REN  TB8  RB8  TI  RI      00000000B		 //S1 Control

#define S1_DoubleRate() PCON |= 0x80
#define S1_SHIFT() SCON &= 0x3f

#define S1_8bit() SCON = (SCON & 0x3f) | 0x40
#define S1_9bit() SCON = (SCON & 0x3f) | 0xc0
#define S1_RX_Enable() SCON |= 0x10
#define S1_RX_Disable() SCON &= ~0x10
#define TI1 TI                  /* 判断TI1�?否发送完�?								 */
#define RI1 RI                  /* 判断RI1�?否接收完�?								 */
#define SET_TI1() TI = 1        /* 设置TI1(引起�?�?)								 */
#define CLR_TI1() TI = 0        /* 清除TI1											 */
#define CLR_RI1() RI = 0        /* 清除RI1											 */
#define S1TB8_SET() TB8 = 1     /* 设置TB8											 */
#define S1TB8_CLR() TB8 = 0     /* 清除TB8											 */
#define S1_Int_Enable() ES = 1  /* 串口1允�?�中�?									 */
#define S1_Int_Disable() ES = 0 /* 串口1禁�??�?�?									 */
#define S1_BRT_UseTimer1() AUXR &= ~1
#define S1_BRT_UseTimer2() AUXR |= 1
#define S1_USE_P30P31() P_SW1 &= ~0xc0                 // UART1 使用P30 P31�?	默�??
#define S1_USE_P36P37() P_SW1 = (P_SW1 & ~0xc0) | 0x40 // UART1 使用P36 P37�?
#define S1_USE_P16P17() P_SW1 = (P_SW1 & ~0xc0) | 0x80 // UART1 使用P16 P17�?

//						  7      6      5      4      3      2     1     0        Reset Value
// sfr S2CON = 0x9A;		S2SM0    -    S2SM2  S2REN  S2TB8  S2RB8  S2TI  S2RI      00000000B		 //S2 Control

#define S2_MODE0() S2CON &= ~(1 << 7)      /* 串口2模式0�?8位UART，波特率 = 定时�?2的溢出率 / 4 */
#define S2_MODE1() S2CON |= (1 << 7)       /* 串口2模式1�?9位UART，波特率 = 定时�?2的溢出率 / 4 */
#define S2_8bit() S2CON &= ~(1 << 7)       /* 串口2模式0�?8位UART，波特率 = 定时�?2的溢出率 / 4 */
#define S2_9bit() S2CON |= (1 << 7)        /* 串口2模式1�?9位UART，波特率 = 定时�?2的溢出率 / 4 */
#define S2_RX_Enable() S2CON |= (1 << 4)   /* 允�?�串2接收										 */
#define S2_RX_Disable() S2CON &= ~(1 << 4) /* 禁�??�?2接收										 */
#define TI2 (S2CON & 2)                    /* 判断TI2�?否发送完�?								 */
#define RI2 (S2CON & 1)                    /* 判断RI2�?否接收完�?								 */
#define SET_TI2() S2CON |= (1 << 1)        /* 设置TI2(引起�?�?)								 */
#define CLR_TI2() S2CON &= ~(1 << 1)       /* 清除TI2											 */
#define CLR_RI2() S2CON &= ~1              /* 清除RI2											 */
#define S2TB8_SET() S2CON |= (1 << 3)      /* 设置TB8											 */
#define S2TB8_CLR() S2CON &= ~(1 << 3)     /* 清除TB8											 */
#define S2_Int_Enable() IE2 |= 1           /* 串口2允�?�中�?									 */
#define S2_Int_Disable() IE2 &= ~1         /* 串口2禁�??�?�?									 */
#define S2_USE_P10P11() P_SW2 &= ~1        /* UART2 使用P1�?	默�??							 */

/**********************************************************/
//						   7     6       5      4     3      2      1      0    Reset Value
// sfr AUXR  = 0x8E;		T0x12 T1x12 UART_M0x6  T2R  T2_C/T T2x12 EXTRAM  S1ST2  0000,0000	//Auxiliary Register

#define InternalXdata_Disable() AUXR |= 2 /* 禁�??使用内部xdata, 所有�?�问xdata都是访问外部xdata  */
#define InternalXdata_Enable() AUXR &= ~2 /* 允�?�使用内部xdata, 当�?�问的地址在内部xdata范围�?, 访问内部的xadta, 当地址超过内部xdata�?, 访问外部xdata  */
#define S1_M0x6() AUXR |= (1 << 5)        /* UART Mode0 Speed is 6x Standard       */
#define S1_M0x1() AUXR &= ~(1 << 5)       /* default,	UART Mode0 Speed is Standard */

//====================================
#define Timer0_16bitAutoReload() TMOD &= ~0x03               /* 16位自动重�?	*/
#define Timer0_16bit() TMOD = (TMOD & ~0x03) | 0x01          /* 16�?         */
#define Timer0_8bitAutoReload() TMOD = (TMOD & ~0x03) | 0x02 /* 8位自动重�?	*/
#define Timer0_16bitAutoRL_NoMask() TMOD |= 0x03             /* 16位自动重装不�?屏蔽�?�?	*/
#define Timer0_Run() TR0 = 1                                 /* 允�?�定时器0计数			*/
#define Timer0_Stop() TR0 = 0                                /* 禁�?�定时器0计数			*/
#define Timer0_Gate_INT0_P32() TMOD |= (1 << 3)              /* 时器0由�?�部INT0高电平允许定时�?�数 */
#define Timer0_AsTimer() TMOD &= ~(1 << 2)                   /* 时器0用做定时�?	*/
#define Timer0_AsCounter() TMOD |= (1 << 2)                  /* 时器0用做计数�?	*/
#define Timer0_AsCounterP34() TMOD |= (1 << 2)               /* 时器0用做计数�?	*/
#define Timer0_1T() AUXR |= (1 << 7)                         /* Timer0 clodk = fo	*/
#define Timer0_12T() AUXR &= ~(1 << 7)                       /* Timer0 clodk = fo/12	12分�??,	default	*/
#define Timer0_CLKO_Enable() INT_CLKO |= 1                   /* 允�?? T0 溢出脉冲在T0(P3.5)脚输出，Fck0 = 1/2 T0 溢出率，T0�?�?1T�?12T�?	*/
#define Timer0_CLKO_Disable() INT_CLKO &= ~1
#define Timer0_CLKO_Enable_P35() INT_CLKO |= 1 /* 允�?? T0 溢出脉冲在T0(P3.5)脚输出，Fck0 = 1/2 T0 溢出率，T0�?�?1T�?12T�?	*/
#define Timer0_CLKO_Disable_P35() INT_CLKO &= ~1
#define Timer0_InterruptEnable() ET0 = 1  /* 允�?�Timer1�?�?.*/
#define Timer0_InterruptDisable() ET0 = 0 /* 禁�??Timer1�?�?.*/

#define T0_Load(n) TH0 = (n) / 256, TL0 = (n) % 256
#define T0_Load_us_1T(n) Timer0_AsTimer(), Timer0_1T(), Timer0_16bitAutoReload(), TH0 = (65536 - ((MAIN_Fosc / 1000) * (n) + 500) / 1000) / 256, TL0 = (65536 - ((MAIN_Fosc / 1000) * (n) + 500) / 1000) % 256
#define T0_Load_us_12T(n) Timer0_AsTimer(), Timer0_12T(), Timer0_16bitAutoReload(), TH0 = (65536 - ((MAIN_Fosc / 12000) * (n) + 500) / 1000) / 256, TL0 = (65536 - ((MAIN_Fosc / 12000) * (n) + 500) / 1000) % 256
#define T0_Frequency_1T_P35(n) ET0 = 0, Timer0_AsTimer(), Timer0_1T(), Timer0_16bitAutoReload(), TH0 = (65536 - (n / 2 + MAIN_Fosc / 2) / (n)) / 256, TL0 = (65536 - (n / 2 + MAIN_Fosc / 2) / (n)) % 256, INT_CLKO |= bit0, TR0 = 1     /* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define T0_Frequency_12T_P35(n) ET0 = 0, Timer0_AsTimer(), Timer0_12T(), Timer0_16bitAutoReload(), TH0 = (65536 - (n / 2 + MAIN_Fosc / 24) / (n)) / 256, TL0 = (65536 - (n / 2 + MAIN_Fosc / 24) / (n)) % 256, INT_CLKO |= bit0, TR0 = 1 /* fx=fosc/(2*M)/n,  M=1 or M=12 */

//====================================
#define Timer1_16bitAutoReload() TMOD &= ~0x30               /* 16位自动重�?	*/
#define Timer1_16bit() TMOD = (TMOD & ~0x30) | 0x10          /* 16�?			*/
#define Timer1_8bitAutoReload() TMOD = (TMOD & ~0x30) | 0x20 /* 8位自动重�?	*/
#define Timer1_Run() TR1 = 1                                 /* 允�?�定时器1计数			*/
#define Timer1_Stop() TR1 = 0                                /* 禁�?�定时器1计数			*/
#define Timer1_Gate_INT1_P33() TMOD |= (1 << 7)              /* 时器1由�?�部INT1高电平允许定时�?�数	*/
#define Timer1_AsTimer() TMOD &= ~(1 << 6)                   /* 时器1用做定时�?			*/
#define Timer1_AsCounter() TMOD |= (1 << 6)                  /* 时器1用做计数�?			*/
#define Timer1_AsCounterP35() TMOD |= (1 << 6)               /* 时器1用做计数�?			*/
#define Timer1_1T() AUXR |= (1 << 6)                         /* Timer1 clodk = fo		*/
#define Timer1_12T() AUXR &= ~(1 << 6)                       /* Timer1 clodk = fo/12	12分�??,	default	*/
#define Timer1_CLKO_Enable() INT_CLKO |= 2                   /* 允�?? T1 溢出脉冲在T1(P3.4)脚输出，Fck1 = 1/2 T1 溢出率，T1�?�?1T�?12T�?	*/
#define Timer1_CLKO_Disable() INT_CLKO &= ~2
#define Timer1_CLKO_Enable_P34() INT_CLKO |= 2 /* 允�?? T1 溢出脉冲在T1(P3.4)脚输出，Fck1 = 1/2 T1 溢出率，T1�?�?1T�?12T�?	*/
#define Timer1_CLKO_Disable_P34() INT_CLKO &= ~2
#define Timer1_InterruptEnable() ET1 = 1  /* 允�?�Timer1�?�?.	*/
#define Timer1_InterruptDisable() ET1 = 0 /* 禁�??Timer1�?�?.	*/

#define T1_Load(n) TH1 = (n) / 256, TL1 = (n) % 256
#define T1_Load_us_1T(n) Timer1_AsTimer(), Timer1_1T(), Timer1_16bitAutoReload(), TH1 = (65536 - ((MAIN_Fosc / 1000) * (n) + 500) / 1000) / 256, TL1 = (65536 - ((MAIN_Fosc / 1000) * (n) + 500) / 1000) % 256
#define T1_Load_us_12T(n) Timer1_AsTimer(), Timer1_12T(), Timer1_16bitAutoReload(), TH1 = (65536 - ((MAIN_Fosc / 12000) * (n) + 500) / 1000) / 256, TL1 = (65536 - ((MAIN_Fosc / 12000) * (n) + 500) / 1000) % 256
#define T1_Frequency_1T_P34(n) ET1 = 0, Timer1_AsTimer(), Timer1_1T(), Timer1_16bitAutoReload(), TH1 = (65536 - (n / 2 + MAIN_Fosc / 2) / (n)) / 256, TL1 = (65536 - (n / 2 + MAIN_Fosc / 2) / (n)) % 256, INT_CLKO |= bit1, TR1 = 1     /* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define T1_Frequency_12T_P34(n) ET1 = 0, Timer1_AsTimer(), Timer1_12T(), Timer1_16bitAutoReload(), TH1 = (65536 - (n / 2 + MAIN_Fosc / 24) / (n)) / 256, TL1 = (65536 - (n / 2 + MAIN_Fosc / 24) / (n)) % 256, INT_CLKO |= bit1, TR1 = 1 /* fx=fosc/(2*M)/n,  M=1 or M=12 */

//====================================
#define Timer2_Run() AUXR |= (1 << 4)          /* 允�?�定时器2计数	*/
#define Timer2_Stop() AUXR &= ~(1 << 4)        /* 禁�?�定时器2计数	*/
#define Timer2_AsTimer() AUXR &= ~(1 << 3)     /* 时器2用做定时�?	*/
#define Timer2_AsCounter() AUXR |= (1 << 3)    /* 时器2用做计数�?	*/
#define Timer2_AsCounterP31() AUXR |= (1 << 3) /* 时器2用做计数�?	*/
#define Timer2_1T() AUXR |= (1 << 2)           /* Timer0 clock = fo	*/
#define Timer2_12T() AUXR &= ~(1 << 2)         /* Timer0 clock = fo/12	12分�??,	default	*/
#define Timer2_CLKO_Enable() INT_CLKO |= 4     /* 允�?? T2 溢出脉冲在P1.3脚输出，Fck2 = 1/2 T2 溢出率，T2�?�?1T�?12T�?	*/
#define Timer2_CLKO_Disable() INT_CLKO &= ~4
#define Timer2_CLKO_Enable_P13() INT_CLKO |= 4 /* 允�?? T2 溢出脉冲在P1.3脚输出，Fck2 = 1/2 T2 溢出率，T2�?�?1T�?12T�?	*/
#define Timer2_CLKO_Disable_P13() INT_CLKO &= ~4
#define Timer2_InterruptEnable() IE2 |= (1 << 2)   /* 允�?�Timer2�?�?.	*/
#define Timer2_InterruptDisable() IE2 &= ~(1 << 2) /* 禁�??Timer2�?�?.	*/

#define T2_Load(n) TH2 = (n) / 256, TL2 = (n) % 256
#define T2_Load_us_1T(n) Timer2_AsTimer(), Timer2_1T(), TH2 = (65536 - ((MAIN_Fosc / 1000) * (n) + 500) / 1000) / 256, TL2 = (65536 - ((MAIN_Fosc / 1000) * (n) + 500) / 1000) % 256
#define T2_Load_us_12T(n) Timer2_AsTimer(), Timer2_12T(), TH2 = (65536 - ((MAIN_Fosc / 12000) * (n) + 500) / 1000) / 256, TL2 = (65536 - ((MAIN_Fosc / 12000) * (n) + 500) / 1000) % 256
#define T2_Frequency_1T_P13(n) Timer2_InterruptDisable(), Timer2_AsTimer(), Timer2_1T(), TH2 = (65536 - (n / 2 + MAIN_Fosc / 2) / (n)) / 256, TL2 = (65536 - (n / 2 + MAIN_Fosc / 2) / (n)) % 256, Timer2_CLKO_Enable_P13(), Timer2_Run()     /* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define T2_Frequency_12T_P13(n) Timer2_InterruptDisable(), Timer2_AsTimer(), Timer2_12T(), TH2 = (65536 - (n / 2 + MAIN_Fosc / 24) / (n)) / 256, TL2 = (65536 - (n / 2 + MAIN_Fosc / 24) / (n)) % 256, Timer2_CLKO_Enable_P13(), Timer2_Run() /* fx=fosc/(2*M)/n,  M=1 or M=12 */

//====================================
#define Timer3_Run() T4T3M |= (1 << 3)             /* 允�?�定时器3计数	*/
#define Timer3_Stop() T4T3M &= ~(1 << 3)           /* 禁�?�定时器3计数	*/
#define Timer3_AsTimer() T4T3M &= ~(1 << 2)        /* 时器3用做定时�?	*/
#define Timer3_AsCounter() T4T3M |= (1 << 2)       /* 时器3用做计数�?, P0.5为�?�部脉冲	*/
#define Timer3_AsCounterP05() T4T3M |= (1 << 2)    /* 时器3用做计数�?, P0.5为�?�部脉冲	*/
#define Timer3_1T() T4T3M |= (1 << 1)              /* 1T模式	*/
#define Timer3_12T() T4T3M &= ~(1 << 1)            /* 12T模式,	default	*/
#define Timer3_CLKO_Enable() T4T3M |= 1            /* 允�?�T3溢出脉冲在T3(P0.4)脚输出，Fck = 1/2 T2 溢出率，T2�?�?1T�?12T�?	*/
#define Timer3_CLKO_Disable() T4T3M &= ~1          /* 禁�??T3溢出脉冲在T3(P0.4)脚输�?	*/
#define Timer3_CLKO_Enable_P04() T4T3M |= 1        /* 允�?�T3溢出脉冲在T3(P0.4)脚输出，Fck = 1/2 T2 溢出率，T2�?�?1T�?12T�?	*/
#define Timer3_CLKO_Disable_P04() T4T3M &= ~1      /* 禁�??T3溢出脉冲在T3(P0.4)脚输�?	*/
#define Timer3_InterruptEnable() IE2 |= (1 << 5)   /* 允�?�Timer3�?�?.	*/
#define Timer3_InterruptDisable() IE2 &= ~(1 << 5) /* 禁�??Timer3�?�?.	*/

#define T3_Load(n) TH3 = (n) / 256, TL3 = (n) % 256
#define T3_Load_us_1T(n) Timer3_AsTimer(), Timer3_1T(), TH3 = (65536 - ((MAIN_Fosc / 1000) * (n) + 500) / 1000) / 256, TL3 = (65536 - ((MAIN_Fosc / 1000) * (n) + 500) / 1000) % 256
#define T3_Load_us_12T(n) Timer3_AsTimer(), Timer3_12T(), TH3 = (65536 - ((MAIN_Fosc / 12000) * (n) + 500) / 1000) / 256, TL3 = (65536 - ((MAIN_Fosc / 12000) * (n) + 500) / 1000) % 256
#define T3_Frequency_1T_P04(n) Timer3_InterruptDisable(), Timer3_AsTimer(), Timer3_1T(), TH3 = (65536 - (n / 2 + MAIN_Fosc / 2) / (n)) / 256, TL3 = (65536 - (n / 2 + MAIN_Fosc / 2) / (n)) % 256, Timer3_CLKO_P04_Enable, Timer3_Run()     /* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define T3_Frequency_12T_P04(n) Timer3_InterruptDisable(), Timer3_AsTimer(), Timer3_12T(), TH3 = (65536 - (n / 2 + MAIN_Fosc / 24) / (n)) / 256, TL3 = (65536 - (n / 2 + MAIN_Fosc / 24) / (n)) % 256, Timer3_CLKO_P04_Enable, Timer3_Run() /* fx=fosc/(2*M)/n,  M=1 or M=12 */

//====================================
#define Timer4_Run() T4T3M |= (1 << 7)               /* 允�?�定时器4计数	*/
#define Timer4_Stop() T4T3M &= ~(1 << 7)             /* 禁�?�定时器4计数	*/
#define Timer4_AsTimer() T4T3M &= ~(1 << 6)          /* 时器4用做定时�?  */
#define Timer4_AsCounter() T4T3M |= (1 << 6)         /* 时器4用做计数�?, P0.7为�?�部脉冲	*/
#define Timer4_AsCounterP07() T4T3M |= (1 << 6)      /* 时器4用做计数�?, P0.7为�?�部脉冲	*/
#define Timer4_1T() T4T3M |= (1 << 5)                /* 1T模式	*/
#define Timer4_12T() T4T3M &= ~(1 << 5)              /* 12T模式,	default	*/
#define Timer4_CLKO_Enable() T4T3M |= (1 << 4)       /* 允�?�T4溢出脉冲在T4(P0.6)脚输出，Fck = 1/2 T2 溢出率，T2�?�?1T�?12T�?	*/
#define Timer4_CLKO_Disable() T4T3M &= ~(1 << 4)     /* 禁�??T4溢出脉冲在T4(P0.6)脚输�?	*/
#define Timer4_CLKO_Enable_P06() T4T3M |= (1 << 4)   /* 允�?�T4溢出脉冲在T4(P0.6)脚输出，Fck = 1/2 T2 溢出率，T2�?�?1T�?12T�?	*/
#define Timer4_CLKO_Disable_P06() T4T3M &= ~(1 << 4) /* 禁�??T4溢出脉冲在T4(P0.6)脚输�?	*/
#define Timer4_InterruptEnable() IE2 |= (1 << 6)     /* 允�?�Timer4�?�?.	*/
#define Timer4_InterruptDisable() IE2 &= ~(1 << 6)   /* 禁�??Timer4�?�?.	*/

#define T4_Load(n) TH4 = (n) / 256, TL4 = (n) % 256
#define T4_Load_us_1T(n) Timer4_AsTimer(), Timer4_1T(), TH4 = (65536 - ((MAIN_Fosc / 1000) * (n) + 500) / 1000) / 256, TL4 = (65536 - ((MAIN_Fosc / 1000) * (n) + 500) / 1000) % 256
#define T4_Load_us_12T(n) Timer4_AsTimer(), Timer4_12T(), TH4 = (65536 - ((MAIN_Fosc / 12000) * (n) + 500) / 1000) / 256, TL4 = (65536 - ((MAIN_Fosc / 12000) * (n) + 500) / 1000) % 256
#define T4_Frequency_1T_P06(n) Timer4_InterruptDisable(), Timer4_AsTimer(), Timer4_1T(), TH4 = (65536 - (n / 2 + MAIN_Fosc / 2) / (n)) / 256, TL4 = (65536 - (n / 2 + MAIN_Fosc / 2) / (n)) % 256, Timer4_CLKO_P06_Enable(), Timer4_Run()     /* fx=fosc/(2*M)/n,  M=1 or M=12 */
#define T4_Frequency_12T_P06(n) Timer4_InterruptDisable(), Timer4_AsTimer(), Timer4_12T(), TH4 = (65536 - (n / 2 + MAIN_Fosc / 24) / (n)) / 256, TL4 = (65536 - (n / 2 + MAIN_Fosc / 24) / (n)) % 256, Timer4_CLKO_P06_Enable(), Timer4_Run() /* fx=fosc/(2*M)/n,  M=1 or M=12 */
//====================================================================================================================

// sfr WDT_CONTR = 0xC1; //Watch-Dog-Timer Control register
//                                       7     6     5      4       3      2   1   0     Reset Value
//                                   WDT_FLAG  -  EN_WDT CLR_WDT IDLE_WDT PS2 PS1 PS0    xx00,0000
#define D_WDT_FLAG (1 << 7)
#define D_EN_WDT (1 << 5)
#define D_CLR_WDT (1 << 4)  /* auto clear	*/
#define D_IDLE_WDT (1 << 3) /* WDT counter when Idle	*/
#define D_WDT_SCALE_2 0
#define D_WDT_SCALE_4 1
#define D_WDT_SCALE_8 2 /* T=393216*N/fo	*/
#define D_WDT_SCALE_16 3
#define D_WDT_SCALE_32 4
#define D_WDT_SCALE_64 5
#define D_WDT_SCALE_128 6
#define D_WDT_SCALE_256 7

#define WDT_reset(n) WDT_CONTR = D_EN_WDT + D_CLR_WDT + D_IDLE_WDT + (n) /* 初�?�化WDT，喂�? */

//						  7     6      5    4     3      2    1     0     Reset Value
// sfr PCON   = 0x87;	SMOD  SMOD0  LVDF  POF   GF1    GF0   PD   IDL    0001,0000	 //Power Control
// SMOD		//串口双倍�?
// SMOD0
#define LVDF (1 << 5) /* P4.6低压检测标�? */
// POF
// GF1
// GF0
// #define 	D_PD		2		/* set 1, power down mode */
// #define 	D_IDLE		1		/* set 1, idle mode */
#define MCU_IDLE() PCON |= 1       /* MCU 进入 IDLE 模式 */
#define MCU_POWER_DOWN() PCON |= 2 /* MCU 进入 睡眠 模式 */

// sfr ISP_CMD   = 0xC5;
#define ISP_STANDBY() ISP_CMD = 0 /* ISP空闲命令（�?��??�?*/
#define ISP_READ() ISP_CMD = 1    /* ISP读出命令		*/
#define ISP_WRITE() ISP_CMD = 2   /* ISP写入命令		*/
#define ISP_ERASE() ISP_CMD = 3   /* ISP擦除命令		*/

// sfr ISP_TRIG  = 0xC6;
#define ISP_TRIG() ISP_TRIG = 0x5A, ISP_TRIG = 0xA5 /* ISP触发命令 */

//							  7    6    5      4    3    2    1     0    Reset Value
// sfr IAP_CONTR = 0xC7;		IAPEN SWBS SWRST CFAIL  -    -    -     -    0000,x000	//IAP Control Register
#define ISP_EN (1 << 7)
#define ISP_SWBS (1 << 6)
#define ISP_SWRST (1 << 5)
#define ISP_CMD_FAIL (1 << 4)

/* ADC Register */
//								7       6      5       4         3      2    1    0   Reset Value
// sfr ADC_CONTR = 0xBC;		ADC_POWER SPEED1 SPEED0 ADC_FLAG ADC_START CHS2 CHS1 CHS0 0000,0000	/* AD �?换控制寄存器 */
// sfr ADC_RES  = 0xBD;		ADCV.9 ADCV.8 ADCV.7 ADCV.6 ADCV.5 ADCV.4 ADCV.3 ADCV.2	  0000,0000	/* A/D �?换结果高8�? */
// sfr ADC_RESL = 0xBE;												  ADCV.1 ADCV.0	  0000,0000	/* A/D �?换结果低2�? */
// sfr ADC_CONTR  = 0xBC;	//直接用MOV操作，不要用与或

// sfr SPCTL  = 0xCE;	SPI控制寄存�?
//    7       6       5       4       3       2       1       0    	Reset Value
//	SSIG	SPEN	DORD	MSTR	CPOL	CPHA	SPR1	SPR0		0x00

#define SPI_SSIG_None() SPCTL |= (1 << 7)       /* 1: 忽略SS�?	*/
#define SPI_SSIG_Enable() SPCTL &= ~(1 << 7)    /* 0: SS脚用于决定主从机	*/
#define SPI_Enable() SPCTL |= (1 << 6)          /* 1: 允�?�SPI	*/
#define SPI_Disable() SPCTL &= ~(1 << 6)        /* 0: 禁�??SPI	*/
#define SPI_LSB_First() SPCTL |= (1 << 5)       /* 1: LSB先发	*/
#define SPI_MSB_First() SPCTL &= ~(1 << 5)      /* 0: MSB先发	*/
#define SPI_Master() SPCTL |= (1 << 4)          /* 1: 设为主机	*/
#define SPI_Slave() SPCTL &= ~(1 << 4)          /* 0: 设为从机	*/
#define SPI_SCLK_NormalH() SPCTL |= (1 << 3)    /* 1: 空闲时SCLK为高电平	*/
#define SPI_SCLK_NormalL() SPCTL &= ~(1 << 3)   /* 0: 空闲时SCLK为低电平	*/
#define SPI_PhaseH() SPCTL |= (1 << 2)          /* 1: 	*/
#define SPI_PhaseL() SPCTL &= ~(1 << 2)         /* 0: 	*/
#define SPI_Speed(n) SPCTL = (SPCTL & ~3) | (n) /*设置速度, 0 -- fosc/4, 1 -- fosc/8, 2 -- fosc/16, 3 -- fosc/32	*/

// sfr SPDAT  = 0xCF; //SPI Data Register                                                     0000,0000
// sfr SPSTAT  = 0xCD;	//SPI状态寄存器
//    7       6      5   4   3   2   1   0    	Reset Value
//	SPIF	WCOL	-	-	-	-	-	-
#define SPIF 0x80 /* SPI传输完成标志。写�?1�?0�?*/
#define WCOL 0x40 /* SPI写冲突标志。写�?1�?0�?  */

#define SPI_USE_P12P13P14P15() P_SW1 &= ~0x0c                 /* 将SPI切换到P12(SS) P13(MOSI) P14(MISO) P15(SCLK)(上电默�??)�?*/
#define SPI_USE_P22P23P24P25() P_SW1 = (P_SW1 & ~0x0c) | 0x04 /* 将SPI切换到P22(SS) P23(MOSI) P24(MISO) P25(SCLK)�?*/
#define SPI_USE_P35P34P33P32() P_SW1 = P_SW1 | 0x0C           /* 将SPI切换到P35(SS) P34(MOSI) P33(MISO) P32(SCLK)�?*/

/*
;PCA_PWMn:    7       6     5   4   3   2     1       0
;			EBSn_1	EBSn_0	-	-	-	-	EPCnH	EPCnL
;B5-B2:		保留
;B1(EPCnH):	在PWM模式下，与CCAPnH组成9位数�?
;B0(EPCnL):	在PWM模式下，与CCAPnL组成9位数�?
*/
#define PWM0_NORMAL() PCA_PWM0 &= ~3              /* PWM0正常输出(默�??)	*/
#define PWM0_OUT_0() PCA_PWM0 |= 3, CCAP0H = 0xff /* PWM0一直输�?0		*/
#define PWM0_OUT_1() PCA_PWM0 &= 0xc0, CCAP0H = 0 /* PWM0一直输�?1		*/

#define PWMA_NORMAL() PCA_PWMA &= ~3              /* PWMA正常输出(默�??)	*/
#define PWMA_OUT_0() PCA_PWMA |= 3, CCAP1H = 0xff /* PWMA一直输�?0		*/
#define PWMA_OUT_1() PCA_PWMA &= 0xc0, CCAP1H = 0 /* PWMA一直输�?1		*/

#define PWMB_NORMAL() PCA_PWMB &= ~3              /* PWMB正常输出(默�??)	*/
#define PWMB_OUT_0() PCA_PWMB |= 3, CCAP2H = 0xff /* PWMB一直输�?0		*/
#define PWMB_OUT_1() PCA_PWMB &= 0xc0, CCAP2H = 0 /* PWMB一直输�?1		*/

/**********************************************************/
#define NOP1() _nop_()
#define NOP2() NOP1(), NOP1()
#define NOP3() NOP2(), NOP1()
#define NOP4() NOP3(), NOP1()
#define NOP5() NOP4(), NOP1()
#define NOP6() NOP5(), NOP1()
#define NOP7() NOP6(), NOP1()
#define NOP8() NOP7(), NOP1()
#define NOP9() NOP8(), NOP1()
#define NOP10() NOP9(), NOP1()
#define NOP11() NOP10(), NOP1()
#define NOP12() NOP11(), NOP1()
#define NOP13() NOP12(), NOP1()
#define NOP14() NOP13(), NOP1()
#define NOP15() NOP14(), NOP1()
#define NOP16() NOP15(), NOP1()
#define NOP17() NOP16(), NOP1()
#define NOP18() NOP17(), NOP1()
#define NOP19() NOP18(), NOP1()
#define NOP20() NOP19(), NOP1()
#define NOP21() NOP20(), NOP1()
#define NOP22() NOP21(), NOP1()
#define NOP23() NOP22(), NOP1()
#define NOP24() NOP23(), NOP1()
#define NOP25() NOP24(), NOP1()
#define NOP26() NOP25(), NOP1()
#define NOP27() NOP26(), NOP1()
#define NOP28() NOP27(), NOP1()
#define NOP29() NOP28(), NOP1()
#define NOP30() NOP29(), NOP1()
#define NOP31() NOP30(), NOP1()
#define NOP32() NOP31(), NOP1()
#define NOP33() NOP32(), NOP1()
#define NOP34() NOP33(), NOP1()
#define NOP35() NOP34(), NOP1()
#define NOP36() NOP35(), NOP1()
#define NOP37() NOP36(), NOP1()
#define NOP38() NOP37(), NOP1()
#define NOP39() NOP38(), NOP1()
#define NOP40() NOP39(), NOP1()
#define NOP(N) NOP##N()

/**********************************************/
#define Pin0 0x01   // IO引脚 Px.0
#define Pin1 0x02   // IO引脚 Px.1
#define Pin2 0x04   // IO引脚 Px.2
#define Pin3 0x08   // IO引脚 Px.3
#define Pin4 0x10   // IO引脚 Px.4
#define Pin5 0x20   // IO引脚 Px.5
#define Pin6 0x40   // IO引脚 Px.6
#define Pin7 0x80   // IO引脚 Px.7
#define PinAll 0xFF // IO所有引�?

#define P0n_standard(bitn) P0M1 &= ~(bitn), P0M0 &= ~(bitn)  /* 00  */
#define P0n_push_pull(bitn) P0M1 &= ~(bitn), P0M0 |= (bitn)  /* 01  */
#define P0n_pure_input(bitn) P0M1 |= (bitn), P0M0 &= ~(bitn) /* 10  */
#define P0n_open_drain(bitn) P0M1 |= (bitn), P0M0 |= (bitn)  /* 11  */

#define P1n_standard(bitn) P1M1 &= ~(bitn), P1M0 &= ~(bitn)
#define P1n_push_pull(bitn) P1M1 &= ~(bitn), P1M0 |= (bitn)
#define P1n_pure_input(bitn) P1M1 |= (bitn), P1M0 &= ~(bitn)
#define P1n_open_drain(bitn) P1M1 |= (bitn), P1M0 |= (bitn)

#define P2n_standard(bitn) P2M1 &= ~(bitn), P2M0 &= ~(bitn)
#define P2n_push_pull(bitn) P2M1 &= ~(bitn), P2M0 |= (bitn)
#define P2n_pure_input(bitn) P2M1 |= (bitn), P2M0 &= ~(bitn)
#define P2n_open_drain(bitn) P2M1 |= (bitn), P2M0 |= (bitn)

#define P3n_standard(bitn) P3M1 &= ~(bitn), P3M0 &= ~(bitn)
#define P3n_push_pull(bitn) P3M1 &= ~(bitn), P3M0 |= (bitn)
#define P3n_pure_input(bitn) P3M1 |= (bitn), P3M0 &= ~(bitn)
#define P3n_open_drain(bitn) P3M1 |= (bitn), P3M0 |= (bitn)

#define P4n_standard(bitn) P4M1 &= ~(bitn), P4M0 &= ~(bitn)
#define P4n_push_pull(bitn) P4M1 &= ~(bitn), P4M0 |= (bitn)
#define P4n_pure_input(bitn) P4M1 |= (bitn), P4M0 &= ~(bitn)
#define P4n_open_drain(bitn) P4M1 |= (bitn), P4M0 |= (bitn)

#define P5n_standard(bitn) P5M1 &= ~(bitn), P5M0 &= ~(bitn)
#define P5n_push_pull(bitn) P5M1 &= ~(bitn), P5M0 |= (bitn)
#define P5n_pure_input(bitn) P5M1 |= (bitn), P5M0 &= ~(bitn)
#define P5n_open_drain(bitn) P5M1 |= (bitn), P5M0 |= (bitn)

#define P6n_standard(bitn) P6M1 &= ~(bitn), P6M0 &= ~(bitn)
#define P6n_push_pull(bitn) P6M1 &= ~(bitn), P6M0 |= (bitn)
#define P6n_pure_input(bitn) P6M1 |= (bitn), P6M0 &= ~(bitn)
#define P6n_open_drain(bitn) P6M1 |= (bitn), P6M0 |= (bitn)

#define P7n_standard(bitn) P7M1 &= ~(bitn), P7M0 &= ~(bitn)
#define P7n_push_pull(bitn) P7M1 &= ~(bitn), P7M0 |= (bitn)
#define P7n_pure_input(bitn) P7M1 |= (bitn), P7M0 &= ~(bitn)
#define P7n_open_drain(bitn) P7M1 |= (bitn), P7M0 |= (bitn)

/****************************************************************/

// sfr INT_CLKO = 0x8F;	//附加�? SFR WAKE_CLKO (地址�?0x8F)
/*
    7   6    5    4   3     2        1       0         Reset Value
    -  EX4  EX3  EX2  -   T2CLKO   T1CLKO  T0CLKO      0000,0000B
b6 -  EX4      : 外中断INT4允�??
b5 -  EX3      : 外中断INT3允�??
b4 -  EX2      : 外中断INT2允�??
b2 - T1CLKO    : 允�?? T2 溢出脉冲在P3.0脚输出，Fck1 = 1/2 T1 溢出�?
b1 - T1CLKO    : 允�?? T1 溢出脉冲在P3.4脚输出，Fck1 = 1/2 T1 溢出�?
b0 - T0CLKO    : 允�?? T0 溢出脉冲在P3.5脚输出，Fck0 = 1/2 T0 溢出�?
*/

#define LVD_InterruptEnable() ELVD = 1
#define LVD_InterruptDisable() ELVD = 0

// sfr WKTCL = 0xAA;	//STC11F\10和STC15系列 唤醒定时器低字节
// sfr WKTCH = 0xAB;	//STC11F\10和STC15系列 唤醒定时器高字节
//	B7		B6	B5	B4	B3	B2	B1	B0		B7	B6	B5	B4	B3	B2	B1	B0
//	WKTEN				S11	S10	S9	S8		S7	S6	S5	S4	S3	S2	S1	S0	n * 560us
#define WakeTimerDisable() WKTCH &= 0x7f                                        /* WKTEN = 0		禁�?�睡眠唤醒定时器 */
#define WakeTimerSet(scale) WKTCL = (scale) % 256, WKTCH = (scale) / 256 | 0x80 /* WKTEN = 1	允�?�睡眠唤醒定时器 */

// sfr BUS_SPEED = 0xA1; //Stretch register      -   -  -  -   -   -  EXRTS1  EXRTSS0 xxxx,xx10
#define BUS_SPEED_1T() BUS_SPEED = BUS_SPEED & ~3
#define BUS_SPEED_2T() BUS_SPEED = (BUS_SPEED & ~3) | 1
#define BUS_SPEED_4T() BUS_SPEED = (BUS_SPEED & ~3) | 2
#define BUS_SPEED_8T() BUS_SPEED = (BUS_SPEED & ~3) | 3

#define BUS_RD_WR_P44_P43() BUS_SPEED = BUS_SPEED & 0x3f
#define BUS_RD_WR_P37_P36() BUS_SPEED = (BUS_SPEED & 0x3f) | 0x40
#define BUS_RD_WR_P42_P40() BUS_SPEED = (BUS_SPEED & 0x3f) | 0x80

/*   interrupt vector */
#define INT0_VECTOR 0
#define TIMER0_VECTOR 1
#define INT1_VECTOR 2
#define TIMER1_VECTOR 3
#define UART1_VECTOR 4
#define ADC_VECTOR 5
#define LVD_VECTOR 6
#define PCA_VECTOR 7
#define UART2_VECTOR 8
#define SPI_VECTOR 9
#define INT2_VECTOR 10
#define INT3_VECTOR 11
#define TIMER2_VECTOR 12
#define INT4_VECTOR 16
#define UART3_VECTOR 17
#define UART4_VECTOR 18
#define TIMER3_VECTOR 19
#define TIMER4_VECTOR 20
#define CMP_VECTOR 21
#define I2C_VECTOR 24
#define USB_VECTOR 25
#define PWMA_VECTOR 26  /* 高级PWMA */
#define PWMB_VECTOR 27  /* 高级PWMB */
#define TKSU_VECTOR 35  /* 触摸按键�?�? */
#define RTC_VECTOR 36   /* RTC�?�? */
#define P0INT_VECTOR 37 /* P0.0~P0.7�?�? */
#define P1INT_VECTOR 38 /* P1.0~P1.7�?�? */
#define P2INT_VECTOR 39 /* P2.0~P2.7�?�? */
#define P3INT_VECTOR 40 /* P3.0~P3.7�?�? */
#define P4INT_VECTOR 41 /* P4.0~P4.7�?�? */
#define P5INT_VECTOR 42 /* P5.0~P5.4�?�? */
#define P6INT_VECTOR 43 /* P6.0~P6.7�?�? */
#define P7INT_VECTOR 44 /* P7.0~P7.7�?�? */

//=============================================================

#define TRUE 1
#define FALSE 0

#define PolityLow 0  // 低优先级�?�?
#define PolityHigh 1 // 高优先级�?�?

#define ENABLE 1
#define DISABLE 0

#endif