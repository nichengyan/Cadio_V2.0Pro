/*
 * @Author: 1234 && 5678
 * @Date: 2025-08-17 16:33:39
 * @LastEditors: 1234 && 5678
 * @LastEditTime: 2025-10-03 22:44:57
 * @FilePath: \Cadio_WebTestc:\Keil_v5\projects\CadioV2.0\Cadio_3D\comm\TYPE.H
 * @Description:
 *
 * Copyright (c) 2025 by ${git_name_email}, All Rights Reserved.
 */
#ifndef _TYPE_
#define _TYPE_
#include <intrins.h>
#define MAIN_Fosc 43000000L // 定义主时钟

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
#define NULL ((void *)0)
#endif

#endif