#ifndef _TYPE_
#define _TYPE_
#include <intrins.h>
#include "STC8H.h"
#define MAIN_Fosc       43000000L   //定义主时钟

typedef bit BOOL;
typedef unsigned char BYTE;
typedef unsigned int WORD;
typedef unsigned long DWORD;

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
typedef unsigned char int8;
typedef unsigned int int16;
typedef unsigned long int32;

typedef unsigned char uchar;
typedef unsigned int uint;
typedef unsigned int ushort;
typedef unsigned long ulong;

typedef unsigned char uint8_t;
typedef unsigned int uint16_t;
typedef unsigned long uint32_t; 
#ifndef NULL
 #define NULL ((void *) 0)
#endif

#endif