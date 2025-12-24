#define MAIN_Fosc 40000000L // ֨ӥ׷ʱד
#include "TYPE.h"
#include "oled.h"
#include "Key.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "Gui.h"
#include "UART.h"
#include "ff.h"
#define pi 3.1415926
/*--------------------------------------------------------------------------
Cadio NetWork
Demo For Esp8266 Wifi Module
CopyRight (c) 2024~2024 NCY
All rights reserved.
--------------------------------------------------------------------------*/
#define Buffer_Size  1024 + 1  
#define Uart_Idle_Time_Out 10  //


void Timer4_Init_1000Hz(void)
{
    T4T3M |= 0x20;
    T4L = 0xC0;
    T4H = 0x63;
    T4T3M |= 0x80;
    IE2 |= 0x40;
}
uint8_t Rx_Buffer[Buffer_Size];
int Rx_Ptr = 0;
enum {
    RX_STATE_READY = 0,
    RX_STATE_IDLE,
    RX_STATE_OVERFLOW
};

uint16_t Uart_Idle_Time = 0;
uint8_t Uart_Rx_State = RX_STATE_READY;
void Uart_Handler();
void Timer4_Handler();


void Uart_Handler() interrupt 4
{

    if (RI)
    {
        RI = 0;

        if (Uart_Rx_State != RX_STATE_READY)
            return;

        Uart_Idle_Time = 0;
        Rx_Buffer[Rx_Ptr++] = SBUF;
        if (Rx_Ptr >= Buffer_Size)
        {
            Uart_Rx_State = RX_STATE_OVERFLOW;
            T4T3M &= (~0x80);
            return;
        }

    }
}
void Timer4_Handler() interrupt 20
{
    Uart_Idle_Time++;
    if (Uart_Idle_Time >= Uart_Idle_Time_Out) {

        Uart_Idle_Time = 0;
        if (Rx_Ptr > 0 && Uart_Rx_State == RX_STATE_READY)
        {
            Uart_Rx_State = RX_STATE_IDLE;
            T4T3M &= (~0x80);

            return;
        }
    }

}





int i, j, k;
uint32_t tick = 0, cnt = 0;
uint16_t length = 0;

sbit Wifi = P2 ^ 0;
sbit Audio = P2 ^ 1;
sbit laser = P1 ^ 3;
sbit light = P1 ^ 7;
sbit flash_cs1 = P3 ^ 5;
sbit flash_cs2 = P2 ^ 3;
sbit sd_cs = P1 ^ 6;

Page pMain;

void __pause()
{
    while (Key_Scan() == 0xffff)
        ;
    while (Key_Scan() != 0xffff)
        ;
}
void __show()
{

    OLED_Printf(0, 4, 16, "                ");
    for (i = 0; i < Rx_Ptr; i++)
    {
        OLED_Printf(i * 8, 4, 16, "%c", Rx_Buffer[i]);
    }
    Rx_Ptr = 0;
}
uint16_t __strlen(uint8_t* s)
{
    uint16_t i = 0;
    for (i = 0; s[i] != '\0'; i++);
    return i;
}
void __memset(uint8_t* s, uint16_t len, uint8_t val)
{
    uint16_t i = 0;
    for (i = 0; i < len; i++)
        s[i] = val;
    return;
}




void delayms(uint16_t t)
{
    uint16_t i;
    while (t--) {
        for (i = 0; i < 600; i++);
    }
}

uint8_t Key()
{
    uint8_t kl, kr, keynum;
    kl = Key_Scan() >> 8;
    kr = Key_Scan();
    if (kl != 0xff)
        keynum = kl;
    else if (kr != 0xff)
        keynum = kr;
    else
        keynum = 0xff;
    return keynum;
}

uint8_t Uart_Rx_OK()
{
    return (Uart_Rx_State == RX_STATE_IDLE);
}
void Uart_Rx_Resume()
{
    Rx_Ptr = 0;
    T4T3M |= 0x80;
    Uart_Rx_State = RX_STATE_READY;
}

uint16_t Word_Match(uint8_t* str1, uint8_t* str2)
{

    uint16_t len1, len2, i, j;
    uint16_t next[32];
    len1 = strlen(str1);
    len2 = strlen(str2);

    if (len2 > len1 || len2 > 32 || len1 == 0 || len2 == 0)
    {
        return 0xffff;
    }

    i = 1;
    j = 0;
    next[0] = 0;
    while (i < len2)
    {
        if (str2[i] == str2[j])
        {
            j++;
            next[i] = j;
            i++;
        }
        else
        {
            if (j != 0)
            {
                j = next[j - 1];
            }
            else
            {
                next[i] = 0;
                i++;
            }
        }
    }
    //ִ��KMPƥ��
    i = 0;
    j = 0;
    while (i < len1)
    {
        if (str1[i] == str2[j])
        {
            i++;
            j++;
        }

        if (j == len2)
        {
            return i - j;
        }
        else if (i < len1 && str1[i] != str2[j])
        {
            if (j != 0)
            {
                j = next[j - 1];
            }
            else
            {
                i++;
            }
        }
    }
    return 0xffff;
}



uint16_t Uart_Rx_String(uint8_t* str, uint16_t len, uint16_t timeout)
{
    uint16_t timecount = 0, ptr = 0;
    ES = 0;
    while (timecount < timeout && ptr < len)
    {
        if (RI)
        {
            RI = 0;
            timeout = 0;
            str[ptr] = SBUF;
            ptr++;
        }
        timeout++;
    }
    ES = 1;
    return ptr;
}
void Uart_Tx_String(uint8_t* str, uint8_t* resp)
{
    Uart_SendString(str);
    Uart_Rx_String(resp, 32, 100);
    return;
}

uint16_t sss = 153;
uint8_t response[32];
uint16_t ipd, stac, stad, connected_id, disconnected_id;
uint8_t send_status;

#define Max_Client_Num 4
#define Max_Path_Length 32
uint8_t Client_Id[Max_Client_Num];                    // ࠍ�?lޓҪʶ
uint8_t Client_Path[Max_Client_Num][Max_Path_Length]; // ࠍۧ׋�?ȳ·޶

FATFS SD_FS;
FILINFO fno,__fno;
DIR __dir;
FIL sd_file[Max_Client_Num];
FRESULT res;
uint32_t rn = 0;//读取到的字节�?
uint32_t total, current;



uint8_t HTTP_Header_SendText(uint8_t cid, uint32_t Content_Length)
{ // Type: 0ԫτѾ 1ԫؽݾ
    uint8_t tx_length = 0;
    uint8_t buf1[128], buf2[32], resp[32];
    if (cid + 1 > Max_Client_Num)
        return 1; // Clinet IDһۏר
    __memset(buf1, 128, 0);
    __memset(buf2, 32, 0);
    __memset(resp, 32, 0); // ȥࠕؖػ�?
    // Gui_Printf(&pMain,0,48,1,0," %s",resp);
    sprintf(buf1, "HTTP/1.1 200 OK\r\nContent-Type: Text/html\r\nContent-Length: %ld\r\n\r\n", Content_Length);
    tx_length = __strlen(buf1);
    sprintf(buf2, "AT+CIPSEND=%bd,%bd\r\n", cid, tx_length);
    Uart_Tx_String(buf2, resp); //if(Word_Match(
    // delayms(1000);
    Uart_Tx_String(buf1, resp);

    return 0;
}
void HTTP_Header_SendFile(uint8_t cid, uint8_t* file_name, uint32_t file_size)
{
    uint8_t buf1[256], buf2[32], resp[32];
    uint16_t tx_length = 0;
    if (cid + 1 > Max_Client_Num)
        return; // Clinet IDһۏר
    __memset(buf1, 256, 0);
    __memset(buf2, 32, 0);
    __memset(resp, 32, 0); // ȥࠕؖػ�?

    sprintf(buf1, "HTTP/1.1 200 OK\r\nContent-Type: application/octet-stream\r\nContent-Disposition: attachment;\r\nfilename=\"%s\"\r\nContent-Length: %lu\r\n\r\n", file_name, file_size);
    tx_length = __strlen(buf1);
    sprintf(buf2, "AT+CIPSEND=%bd,%d\r\n", cid, tx_length);
    //Gui_ClearBufferBit(&pMain,0);
    //Gui_Printf(&pMain,0,0,1,0,"%s %lu",buf1+64,file_size);
    //Gui_Flush(&pMain);while(1);
    Uart_Tx_String(buf2, resp);
    Uart_Tx_String(buf1, resp);
}
uint8_t HTTP_Content_SendByte(uint8_t cid, uint8_t* dat, uint16_t Length)
{
    uint8_t retry = 20;
    uint8_t buf1[32], resp[32];
    uint16_t i;
    if (cid + 1 > Max_Client_Num)
        return 3; // Clinet IDһۏר
    __memset(buf1, 32, 0);
    __memset(resp, 32, 0); // ȥࠕؖػ�?
    sprintf(buf1, "AT+CIPSEND=%bd,%d\r\n", cid, Length);

    while (retry--) {
        Uart_Tx_String(buf1, resp);
        if (Word_Match(resp, "link") != 0xffff) return 2;//连接无效，直接寄
        if (Word_Match(resp, ">") == 0xffff) break;
        if (Word_Match(resp, "busy") == 0xffff) break;
    }

    if (retry == 0) return 1;
    for (i = 0; i < Length; i++)
        Uart_T8Bit(dat[i]);
    return 0;
}




void List_file(uint8_t *path)
{
    uint8_t file_cnt = 0;
    res = f_opendir(&__dir, path);
    while (1)
    {
        res = f_readdir(&__dir, &__fno); // 读取目录项
        if (res != FR_OK || __fno.fname[0] == 0)
            break;
        if (__fno.fname[0] == '.')
            continue; // 跳过当前目录和父目录条目
        // 判断是文件还是目录
        if (__fno.fattrib & AM_DIR)
        {       
                // �?�?�?
                // OLED_Printf(0, cnt1*2, 16, "%s", fno.fname);
                // cnt1++;
                // index++;
            
        }
        else
        {
            // �?文件
            // OLED_Printf(0, cnt2*2, 16, "%s", fno.fname);
            // Graphic_Printf(0,index*8,1,"%s",fno.fname);
            // if(fno.)
        }
    }
    //return file_cnt;
}

void Process_Data(uint8_t* str)
{
    uint8_t buf1[256], buf2[32], resp[32];
    uint16_t i;
    uint8_t tmp = 0;
    uint8_t CID_Ptr = 0; // ө֒ژݼؖքǰʼλ׃
    uint8_t Path_Start = 0;
    uint8_t Path_End = 0;
    uint8_t cid = 0xff; // client id
    uint16_t fno_cnt = 0;
    uint16_t max_length = 0;
    uint16_t posx = 0, posy = 0;
    uint16_t font_size = 15;
    uint16_t list_length = 30;
    uint16_t http_len = 0;
    tmp = Word_Match(str, "+IPD");
    if (tmp == 0xffff)
        return;
    else
        CID_Ptr = tmp + 5;

    tmp = Word_Match(str, "GET");
    if (tmp == 0xffff)
        return;
    else
        Path_Start = tmp + 4;

    tmp = Word_Match(str, " HTTP/1.1");
    if (tmp == 0xffff)
        return;
    else
        Path_End = tmp;

    cid = str[CID_Ptr] - '0';
    if (cid + 1 > Max_Client_Num || Client_Id[cid])
        return;
    __memset(Client_Path[cid], Max_Path_Length, 0); // ȥࠕ·޶ۺԥȸ�?׼Ѹͮɫт·޶
    // τݾϵͳٹĿ¼ 1:
    Client_Path[cid][0] = '1';
    Client_Path[cid][1] = ':';
    // ͮɫʣЂҿؖ
    for (i = Path_Start; i < Path_End; i++)
    {
        // ӲΪ.՚䰀F��΀Ĵِؖػ�?ӲՋԃ?պͦc}ɧú1:/music/a?wav ܴպҭ 1:/music/a.wav
        if (str[i] == '?')
            Client_Path[cid][2 + i - Path_Start] = '.';
        else
            Client_Path[cid][2 + i - Path_Start] = str[i]; // ͮɫ·޶
    }

    res = f_stat(Client_Path[cid], &fno);
       // res = f_open(&sd_file[cid], Client_Path[cid], FA_READ);
    // for(i = 0;i < Max_Path_Length; i++)
    if (res != FR_OK)
    {
           
        // OLED_ShowString(0,0,(uint8_t*)"Disk Error!",16);
        // OLED_Printf(0,0,16,"File Error %bd",res);
        // delayms(10000);
        //__memset(resp,32,0);
        // Uart_Tx_String("AT+CIPSEND=0,86\r\n",resp);
        // Gui_Printf(&pMain,0,48,1,0," %s",resp);
        // Uart_SendString("HTTP/1.1 200 OK \r\nContent-Type: Text/html\r\nContent-Length: 21\r\n\r\n\<h1>Hello World!</h1>12345\r\n");
        __memset(buf1, 256, 0);
        __memset(buf2, 32, 0);
        __memset(resp, 32, 0);
        // ДʾխϳхϢ


        sprintf(buf1, "<h1> %s is an invalid path!</h1>", Client_Path[cid]);
        HTTP_Header_SendText(cid,__strlen(buf1));
        HTTP_Content_SendByte(cid, buf1, __strlen(buf1));

         delayms(1000);
         sprintf(buf2,"AT+CIPCLOSE=%bd\r\n",cid);
         Uart_Tx_String(buf2,resp);

        Client_Id[cid] = 0;
        return;
    }
    else
    {
        
        __memset(buf1, 256, 0);
        __memset(buf2, 32, 0);
        __memset(resp, 32, 0);
        //sprintf(buf1, "<h1> %s path!</h1>", Client_Path[cid]);
        //HTTP_Header_SendText(cid, 1000);
        //HTTP_Content_SendByte(cid, buf1, __strlen(buf1));
        
        if (fno.fattrib & AM_DIR) {  //是目录      
            
            f_opendir(&__dir, Client_Path[cid]);
            while (1)
            {   
                res = f_readdir(&__dir, &__fno); // 读取目录项
                if (res != FR_OK || __fno.fname[0] == 0)
                    break;
                if (__fno.fname[0] == '.')
                    continue; // 跳过当前目录和父目录条目
                    // 判断是文件还是目录
                if (__fno.fattrib & AM_DIR)
                {           
                    __memset(buf1,256,0);
                    sprintf(buf1,"<div style=\"position:fixed;left:%upx;font-size:%u;top:%upx;font-weight:bold\">%s</div>\r\n\r\n", posx, (uint16_t)font_size, posy, __fno.fname);
                    //HTTP_Content_SendByte(cid, buf1, __strlen(buf1));
                    //if( max_length < __strlen(buf1)) max_length = __strlen(buf1);
                    //posy += font_size;
                    //delayms(500);
                    fno_cnt++;
                    http_len += __strlen(buf1);
                            // �?�?�?
                            // OLED_Printf(0, cnt1*2, 16, "%s", fno.fname);
                            // cnt1++;
                            // index++;
                }
                else
                {   
                    __memset(buf1,256,0);
                    sprintf(buf1,"<div style=\"position:fixed;left:%upx;font-size:%u;top:%upx;\">%s</div>\r\n\r\n", posx, (uint16_t)font_size, posy, __fno.fname);
                    //HTTP_Content_SendByte(cid, buf1, __strlen(buf1));
                    //if( max_length < __strlen(buf1)) max_length = __strlen(buf1);
                    //posy += font_size;
                    //list_length += __strlen(buf1);
                    //delayms(500);
                    fno_cnt++;
                    http_len += __strlen(buf1);
                            // �?文件
                            // OLED_Printf(0, cnt2*2, 16, "%s", fno.fname);
                            // Graphic_Printf(0,index*8,1,"%s",fno.fname);
                            // if(fno.)
                }
                //if(posy >= font_size*list_length ){
                 //   posy = 0;
                 //   posx += max_length*2;
                 //   max_length = 0;
               // }
            }
            f_opendir(&__dir, Client_Path[cid]);
            

            //sprintf(buf1, "<div style=\"position:fixed;left:50px;font-size:60;top:30px;color:red\">Files</div>");
            //HTTP_Header_SendText(cid,__strlen(buf1));
            //HTTP_Content_SendByte(cid, buf1, __strlen(buf1));  
  
            //delayms(1000);

            HTTP_Header_SendText(cid, http_len);
            while (1)
            {   
                res = f_readdir(&__dir, &__fno); // 读取目录项
                if (res != FR_OK || __fno.fname[0] == 0)
                    break;
                if (__fno.fname[0] == '.')
                    continue; // 跳过当前目录和父目录条目
                    // 判断是文件还是目录
                if (__fno.fattrib & AM_DIR)
                {           
                    __memset(buf1,256,0);
                    sprintf(buf1,"<div style=\"position:fixed;left:%upx;font-size:%u;top:%upx;font-weight:bold\">%s</div>\r\n\r\n",posx , (uint16_t)font_size,50 + posy, __fno.fname);
                    HTTP_Content_SendByte(cid, buf1, __strlen(buf1));
                    if( max_length < __strlen(buf1)) max_length = __strlen(buf1);
                    posy += font_size;
                    delayms(500);
                    fno_cnt++;
                    //http_len += __strlen(buf1);
                            // �?�?�?
                            // OLED_Printf(0, cnt1*2, 16, "%s", fno.fname);
                            // cnt1++;
                            // index++;
                }
                else
                {   
                    __memset(buf1,256,0);
                    sprintf(buf1,"<div style=\"position:fixed;left:%upx;font-size:%u;top:%upx;\">%s</div>\r\n\r\n", posx,  (uint16_t)font_size,50 + posy, __fno.fname);
                    HTTP_Content_SendByte(cid, buf1, __strlen(buf1));
                    if( max_length < __strlen(buf1)) max_length = __strlen(buf1);
                    posy += font_size;
                    //list_length += __strlen(buf1);
                    delayms(500);
                    fno_cnt++;
                    //http_len += __strlen(buf1);
                            // �?文件
                            // OLED_Printf(0, cnt2*2, 16, "%s", fno.fname);
                            // Graphic_Printf(0,index*8,1,"%s",fno.fname);
                            // if(fno.)
                }
                if(posy >= font_size*list_length ){
                    posy = 0;
                    posx += max_length*2;
                    max_length = 0;
                }
            }
            //sprintf(buf1, "<h1> %s path!</h1>", Client_Path[cid]);
            //HTTP_Header_SendText(cid, http_len);

            for(i = 0;i < fno_cnt; i++){
                
            }
            //HTTP_Content_SendByte(cid, buf1, __strlen(buf1));
            delayms(1000);

            sprintf(buf2, "AT+CIPCLOSE=%bd\r\n", cid);
            Uart_Tx_String(buf2, resp);
            Client_Id[cid] = 0;
            return;
            //sprintf(buf1, "<div style=\"position:fixed;left:30px;top:50px;font-size:20px\">%s", Client_Path[cid]);
            //HTTP_Header_SendText(cid, __strlen(buf1));
            //HTTP_Content_SendByte(cid, buf1, __strlen(buf1));
        }

        else {   
            //是文件,就打开它

            // ДʾխϳхϢ
            // sprintf(buf1,"<h6>%s OK</h6>",Client_Path[cid]);
            // HTTP_Header_SendText(cid,__strlen(buf1));
            // HTTP_Content_SendByte(cid,buf1,__strlen(buf1));
            res = f_open(&sd_file[cid], Client_Path[cid], FA_READ);
            HTTP_Header_SendFile(cid, "File.fil", f_size(&sd_file[cid]));
            Client_Id[cid] = 1;
            delayms(1000);
            //sprintf(buf2,"AT+CIPCLOSE=%bd\r\n",cid);
            //Uart_Tx_String(buf2,resp);
        // while (1)
        //{
            //OLED_Printf(0,0,
                //HTTP_Content_SendByte(cid,buf1,256*8);
            // Uart_SendString(
        //}
        // HTTP_Content_SendByte(cid,buf1,__strlen(buf1));
        // ׏ߪձǰClientlޓ
        // sprintf(buf2,"AT+CIPCLOSE=%bd\r\n",cid);
        // Uart_Tx_String(buf2,resp);
        // Client_Id[cid] = 0;
            
        }


    }
    //
    Gui_Flush(&pMain);
}


uint8_t tx[2048 + 2];
void Fs_Init() {
    res = f_mount(&SD_FS, "1:", 1);
    if (res != FR_OK)
    {
        OLED_Printf(0, 0, 16, "Disk Error %bd", res);
        while (1);
    }
}
void Server_Init() {
    for (i = 0;i < 4; i++) Client_Id[i] = 0;
    __memset(response, 32, 0);
    //Uart_Tx_String("AT+UART=500000,8,1,0,0\r\n", response);
    Uart_Tx_String("AT+CIPMUX=1\r\n", response);
    //Gui_Printf(&pMain, 0, 10, 1, 0, "%u", Word_Match(response, "OK"));

    __memset(response, 32, 0);
    Uart_Tx_String("AT+CIPSERVER=1,80\r\n", response);
    //Gui_Printf(&pMain, 0, 20, 1, 0, "%u", Word_Match(response, "OK"));

    // Uart_Tx_String("AT+RST\r\n",response);
    Gui_Flush(&pMain);
    delayms(1000);
}
void Server_run() {
    uint16_t i;
    //for(i = 0;i < 4; i++) Client_Id[i]=0;
    if (Uart_Rx_State == RX_STATE_IDLE) {
        Rx_Buffer[Rx_Ptr] = 0;
        // ipd = Word_Match(Rx_Buffer,"");
        //__memset(response, 1024, 0);
       // for (i = 0; i < 1024 && i < Rx_Ptr; i++) {response[i] = Rx_Buffer[i];}
        Process_Data(Rx_Buffer);
        stac = Word_Match(Rx_Buffer, "+STA_CONNECTED");
        stad = Word_Match(Rx_Buffer, "+STA_DISCONNECTED");
        connected_id = Word_Match(Rx_Buffer, ",CONNECT");
        disconnected_id = Word_Match(Rx_Buffer, ",CLOSE");
        if (stac != 0xffff)
        {
            //Gui_Printf(&pMain, 0, 56, 1, 0, "Connected");
        }
        if (stad != 0xffff)
        {
            //Gui_Printf(&pMain, 0, 56, 1, 0, "Disconnected");
        }
        if (connected_id != 0xffff)
        {
            //Gui_Printf(&pMain, 0, 56, 1, 0, "Connected id: %c", response[connected_id - 1]);
        }
        if (disconnected_id != 0xffff)
        {
            //Gui_Printf(&pMain, 0, 56, 1, 0, "Disonnected id: %c", response[disconnected_id - 1]);
            //Client_Id[response[disconnected_id - 1] - '0'] = 0;
        }
        // Gui_Printf(&pMain,0,0,1,0,"%s", response);
        // Gui_Printf(&pMain,0,56,1,0,"%d", Rx_Ptr);
        //for (i = 0; i < Rx_Ptr && i < 100; i++)
        //{
           // Gui_Printf(&pMain, (i % 20) * 6, (i / 20) * 8, 1, 0, "%c", response[i]);
        //}
       //Gui_Printf(&pMain,0,0,1,0,"eee");
        //Gui_Flush(&pMain);
        Uart_Rx_Resume();
    }

    if (Uart_Rx_State == RX_STATE_OVERFLOW)
    {
        //  OLED_Printf(0, 0, 16, "OverFlow");
     // delayms(100);
        Uart_Rx_Resume();
    }
    //OLED_Printf(0,0,16,"%lu",Send_Tick);

    i = 0;

    Gui_ClearBufferBit(&pMain, 0);
    for (i = 0;i < Max_Client_Num;i++) {
        //OLED_Printf(0,i*2,16,"Task%bu:%bu",i,Client_Id[i]);
        rn = 0;
        if (Client_Id[i] == 1) {
            total = f_size(&sd_file[i]);
            current = f_tell(&sd_file[i]);

            Gui_Printf(&pMain, 0, i * 10 + 2, 1, 0, "Task%u:", i);
            Gui_Printf(&pMain, 42, i * 10 + 2, 1, 0, "%lu/%lu", current, total);
            Gui_DrawRectangleWired(&pMain, 38, i * 10 + 1, 84, 10, 2);
            Gui_DrawRectangleFilled(&pMain, 38, i * 10 + 1, 84 * current / total, 10, 2);

            f_read(&sd_file[i], tx, 2048, &rn);
            if (rn == 0) {
                //OLED_Printf(0,0,16,"IDLE");   
                Client_Id[i] = 0;
                f_close(&sd_file[i]);
                continue;
            }
            else {
                send_status = HTTP_Content_SendByte(i, tx, 2048);
                //OLED_Printf(48,i*2,16,"%lu",f_tell(&sd_file[i]));

                if (send_status == 2) { //如果连接失败，直接结�?
                    f_close(&sd_file[i]);
                    Client_Id[i] = 0;
                    continue;
                }
            }
        }
        else {
            Gui_Printf(&pMain, 0, i * 10 + 2, 1, 0, "Task%u: Idle", i);
            //Gui_DrawRectangleFilled(&pMain,0,32 + 8*i ,64,8,2);
        }

    }
    //Gui_Printf(&pMain,0,40,1,0,"%bd %bd %bd %bd",Client_Id[0],Client_Id[1],Client_Id[2],Client_Id[3]);
    Gui_Flush(&pMain);
}
//uint32_t rx[128 + 10];
const uint8_t rotate_char[] = "/|\\-";
uint8_t c_ptr = 0;
uint16_t bd = 0;
uint8_t resp[32];
void main(void)
{
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M1 = 0x0f;
    P1M0 = 0x20; // 锟斤拷锟斤拷为锟斤拷锟借抗
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 = 0xff;
    P5M1 = 0x00;

    Wifi = 0;
    laser = 0;
    Audio = 0;
    light = 0;
    flash_cs1 = 1;
    flash_cs2 = 1;
    sd_cs = 0;

    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear();


    Gui_Init();
    Gui_CreatePage(&pMain, 128, 64);
    Gui_ClearBufferBit(&pMain, 0);
    Gui_Flush(&pMain);
    Gui_Flush(&pMain);
    OLED_Printf(0, 0, 16, "Loading...");
    OLED_Printf(0, 2, 16, "IP:192.168.4.1");
    P_SW1 = (P_SW1 & ~0xc0) | 0x40; // UART1/USART1: RxD(P3.6), RxD(P3
    Uart_Init();
    Timer4_Init_1000Hz();
    Uart_Rx_Resume();
    EA = 1;
    ES = 1;
    Wifi = 1;
    delayms(10000);
    Server_Init();
    Fs_Init();
    while (1) {
        // OLED_Printf(0,0,16,"%d %bd ",Rx_Ptr,Uart_Rx_State);
        Server_run();
        OLED_Printf(0, 6, 16, "Server_run... %c", rotate_char[c_ptr]);
        c_ptr++;
        if (c_ptr > 3) c_ptr = 0;
    }




}
