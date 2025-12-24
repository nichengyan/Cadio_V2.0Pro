#include "editor.h"
#include "TYPE.h"
#include "Gui.h"
#include <string.h>
#include "_STC8X_.h"
#include "ADC.h"
#include "Key.h"
#include "FAT12_2v1.h"


extern FAT12_File f;
static uint8_t Prog_Id = 0xff; //当前程序标志 0xff时并没有选择程序
uint8_t Load_Program(uint8_t  Code[][32]){
    uint8_t id, i,j,k; 
    uint8_t kl,kr,keynum;
    uint8_t select_id = 0;
    uint8_t offset_y = 0;
    Page pLoad;
    Gui_CreatePage(&pLoad, 48, 48);
    Gui_ClearBufferBit(&pLoad, 0);
    Gui_SetPosAbs(&pLoad,40,8);
    Gui_ClearBufferBit(&pLoad,1);
    Gui_Flush(&pLoad);
    while(1){
        kl = Key_Scan() >> 8;
        kr = Key_Scan();
        // tick ++;
        if (kl != 0xff)
            keynum = kl;
        else if (kr != 0xff)
            keynum = kr;
        else
            keynum = 0xff;
        if (keynum == 0x3d )
            break;
        if (keynum == 0x45 ){
            //memset(Str,0,32);
            Gui_FreePage(&pLoad);
            //一定记得free!!!
            return 0xff;
        }      
        
        if (keynum == 0x44)
        {
            if(select_id > 0 ){
                select_id --;    
            }           
        }            
        if (keynum == 0x49)
        {
             if(select_id < 31 ){
                select_id ++;  
             }     
        }    

        if(select_id  >= offset_y + 3 ){
            offset_y = select_id - 3;
        }

        if(select_id < offset_y){
            offset_y = select_id; 
        }
        Gui_ClearBufferBit(&pLoad, 1);
        
        Gui_Printf(&pLoad,10, 2 ,0,0, "Load:");
        //Gui_DrawBmp(&pLoad,2,2,8,8,bmp);
        //maxl = 0;
        for(i = 0; i < 4; i ++ ) {   
            if( offset_y + i == select_id) Gui_Printf(&pLoad,2, 11 + 9 * i ,1,0, "File %bd",offset_y + i);
            else Gui_Printf(&pLoad,2, 11 + 9 * i ,0,0, "File %bd",offset_y + i);
            //maxl = (strlen(cmd_list1[offset_y +i]) > maxl ? strlen(cmd_list1[offset_y +i]) : maxl);
        }

        //strcpy(Str,cmd_list1[select_id]);
        //display_s = select_id
        //OLED_Printf(64,0,16,"%bd %bd",offset_y,select_id);
        Editor_delay(500);
        //Gui_DrawRectangleWired(&pCommand, 0, 0, maxl * 6 + 3, 46, 1);
        //width += (maxl - width) *0.8;
        Gui_Flush(&pLoad);
    }
    Prog_Id = select_id;
    FAT12_Fseek(&f,Prog_Id * 1024,0);
    FAT12_Fread(&f,(uint8_t*)Code,1024);
    Gui_FreePage(&pLoad);
    return 0;
}
uint8_t Save_Program(uint8_t  Code[][32]){
    uint8_t id, i,j,k; 
    uint8_t kl,kr,keynum;
    uint8_t select_id = 0;
    uint8_t offset_y = 0;
    uint8_t choice = 0; //yes 1 no 0;
    uint8_t code question[]={
            /*--  调入了一幅图像：C:\Users\nichengyan\Desktop\询问question.bmp  --*/
        /*--  宽度x高度=32x32  --*/
        0x00,0x00,0x00,0x00,0x00,0x00,0xC0,0xC0,0x30,0x30,0x30,0x30,0x0C,0x0C,0x8C,0xCC,
        0xCC,0x8C,0x0C,0x0C,0x30,0x30,0x30,0x30,0xC0,0xC0,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0xF0,0xF0,0x0F,0x0F,0x00,0x00,0x00,0x00,0x3C,0x3C,0x1F,0x0F,0x03,0x03,
        0xC3,0xC3,0xFF,0xFF,0x3C,0x3C,0x00,0x00,0x00,0x00,0x0F,0x0F,0xF0,0xF0,0x00,0x00,
        0x00,0x00,0x0F,0x0F,0xF0,0xF0,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xCF,0xCF,
        0xCF,0xCF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xF0,0xF0,0x0F,0x0F,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x03,0x03,0x0C,0x0C,0x0C,0x0C,0x30,0x30,0x31,0x33,
        0x33,0x31,0x30,0x30,0x0C,0x0C,0x0C,0x0C,0x03,0x03,0x00,0x00,0x00,0x00,0x00,0x00
    };
    Page pSave;
    Gui_CreatePage(&pSave, 48, 48);
    if(1){   
        Gui_ClearBufferBit(&pSave, 0);
        Gui_SetPosAbs(&pSave,40,8);
        Gui_ClearBufferBit(&pSave,1);
        Gui_Flush(&pSave);
        while(1){
            kl = Key_Scan() >> 8;
            kr = Key_Scan();
            // tick ++;
            if (kl != 0xff)
                keynum = kl;
            else if (kr != 0xff)
                keynum = kr;
            else
                keynum = 0xff;
            if (keynum == 0x3d )
                break;
            if (keynum == 0x45 ){
                //memset(Str,0,32);
                Gui_FreePage(&pSave);
                return 0xff;
            }      
            
            if (keynum == 0x44)
            {
                if(select_id > 0 ){
                    select_id --;    
                }           
            }            
            if (keynum == 0x49)
            {
                 if(select_id < 31 ){
                    select_id ++;  
                 }     
            }    

            if(select_id  >= offset_y + 3 ){
                offset_y = select_id - 3;
            }

            if(select_id < offset_y){
                offset_y = select_id; 
            }

            Gui_ClearBufferBit(&pSave, 1);
            Gui_Printf(&pSave,6, 2 ,0,0, "Save As");
            //Gui_DrawBmp(&pSave,2,2,8,8,bmp);
            //maxl = 0;
            for(i = 0; i < 4; i ++ ) {   
                if( offset_y + i == select_id) Gui_Printf(&pSave,2, 11 + 9 * i ,1,0, "File %bd",offset_y + i);
                else Gui_Printf(&pSave,2, 11 + 9 * i ,0,0, "File %bd",offset_y + i);
                //maxl = (strlen(cmd_list1[offset_y +i]) > maxl ? strlen(cmd_list1[offset_y +i]) : maxl);
            }
            //strcpy(Str,cmd_list1[select_id]);
            //display_s = select_id
            //OLED_Printf(64,0,16,"%bd %bd",offset_y,select_id);
            Editor_delay(500);
            //Gui_DrawRectangleWired(&pCommand, 0, 0, maxl * 6 + 3, 46, 1);
            //width += (maxl - width) *0.8;
            Gui_Flush(&pSave);
        }
        Prog_Id = select_id;
    }
    while (Key_Scan() != 0xffff);
    while(1){
        kl = Key_Scan() >> 8;
        kr = Key_Scan();
        // tick ++;
        if (kl != 0xff)
            keynum = kl;
        else if (kr != 0xff)
            keynum = kr;
        else
            keynum = 0xff;
        if (keynum == 0x3d )
            break;
        if(keynum == 0x48 || keynum == 0x4a){
            if(choice == 1) choice = 0;
            else choice = 1;
        }
        Gui_ClearBufferBit(&pSave, 0);
        Gui_SetPosAbs(&pSave,40,8);
        Gui_ClearBufferBit(&pSave,1);
        Gui_DrawBmp(&pSave,8,-1,32,32,question);
        Gui_Printf(&pSave,8,31, 0, 0 , "Save?");
        if(choice == 1){
            Gui_Printf(&pSave,6,39, 1, 0 , "yes");
            Gui_Printf(&pSave,30,39, 0, 0 ,"no");
        }else{
             Gui_Printf(&pSave,6,39, 0, 0 , "yes");
             Gui_Printf(&pSave,30,39, 1, 0 ,"no");
        }
        Editor_delay(500);
        Gui_Flush(&pSave);
    }
    while (Key_Scan() != 0xffff);
    if(choice){
        FAT12_Fseek(&f,Prog_Id * 1024,0);
        FAT12_Modify(&f,(uint8_t*)Code,1024);
        Gui_FreePage(&pSave);
        return 0;
    }else{
        Gui_FreePage(&pSave);
        return 0xff;
    }
}
void Select_Command(uint8_t *Str)
{
    Page pCommand;
    uint8_t kl, kr, keynum;
    uint8_t select_id = 0;
    int8_t offset_y = 0; //组内偏移量
    uint8_t maxl= 0; //最长的一个
    float width = 0;
    float k = 0.1f;
    uint8_t i,j = 0;
    uint8_t list_select = 0; //0 :指令 1:变量
    const uint8_t cmd_list1[][6] = {
        {"rv"},
        {"wv"},
        {"ad"},
        {"rv"},
        {"jz"},
        {"jnz"},
        {"cmp"},
        {"put"},
        {"get"},
        {"wait"},
    };
    const uint8_t cmd_list2[][6] = {
        {"clr"},
        {"fls"},
        {"dis"},
        {"dpt"},
        {"gpt"},
        {"tick"},
        {" "},
        {" "},
        {" "},
        {" "},
    };
    const uint8_t var_list[][6] = {
        {"A"},
        {"B"},
        {"C"},
        {"D"},
        {"E"},
        {"F"},
        {"X"},
        {"Y"},
        {"Z"},
        {","}
        //{"rv"},
    };
    Gui_CreatePage(&pCommand, 28, 48);
    Gui_ClearBufferBit(&pCommand, 0);
    for (i = 0; i < 16; i++)
    {
        Gui_ClearBufferBit(&pCommand, 0);
        Gui_DrawRectangleWired(&pCommand, -16 + i, 0, 26, 46, 1);
        Gui_Flush(&pCommand);
    }
    while (1)
    {
        kl = Key_Scan() >> 8;
        kr = Key_Scan();
        // tick ++;
        if (kl != 0xff)
            keynum = kl;
        else if (kr != 0xff)
            keynum = kr;
        else
            keynum = 0xff;
        if (keynum == 0x3d )
            break;
        if (keynum == 0x45 ){
            memset(Str,0,32);
            break;
        }
        if  (keynum == 0x4a && list_select < 2) list_select ++;
        if  (keynum == 0x48 && list_select > 0) list_select --;         
        
        if (keynum == 0x44)
        {
            if(select_id > 0 ){
                select_id --;    
            }           
        }            
        if (keynum == 0x49)
        {
             if(select_id < 9 ){
                select_id ++;  
             }     
        }    

        if(select_id  >= offset_y + 4 ){
            offset_y = select_id - 4;
        }

        if(select_id < offset_y){
            offset_y = select_id; 
        }

        Gui_ClearBufferBit(&pCommand, 0);
        maxl = 0;
        switch(list_select){
            case 0:
                for(i = 0; i < 5; i ++ ) {   
                    if( offset_y + i == select_id) Gui_ShowString(&pCommand,1,9 * i , cmd_list1[offset_y +i] ,0,0);
                    else Gui_ShowString(&pCommand,1,9 * i ,cmd_list1[offset_y +i] ,1,0);
                    maxl = (strlen(cmd_list1[offset_y +i]) > maxl ? strlen(cmd_list1[offset_y +i]) : maxl);
                }
                strcpy(Str,cmd_list1[select_id]);
                break;
            case 1:            
                for(i = 0; i < 5; i ++ ) {   
                    if( offset_y + i == select_id) Gui_ShowString(&pCommand,1,9 * i , cmd_list2[offset_y +i] ,0,0);
                    else Gui_ShowString(&pCommand,1,9 * i ,cmd_list2[offset_y +i] ,1,0);
                    maxl = (strlen(cmd_list2[offset_y +i]) > maxl ? strlen(cmd_list2[offset_y +i]) : maxl);
                }
                strcpy(Str,cmd_list2[select_id]);
                break;

            case 2:             
                for(i = 0; i < 5; i ++ ) {   
                    if( offset_y + i == select_id) Gui_ShowString(&pCommand,1,9 * i , var_list[offset_y +i] ,0,0);
                    else Gui_ShowString(&pCommand,1,9 * i ,var_list[offset_y +i] ,1,0);
                    maxl = (strlen(var_list[offset_y +i]) > maxl ? strlen(var_list[offset_y +i]) : maxl);
                }
                strcpy(Str,var_list[select_id]);
                break;
            default:
                break;
        }
        //display_s = select_id
        //OLED_Printf(64,0,16,"%bd %bd",offset_y,select_id);
        Editor_delay(500);
        Gui_DrawRectangleWired(&pCommand, 0, 0, maxl * 6 + 3, 46, 1);
        //width += (maxl - width) *0.8;
        Gui_Flush(&pCommand);
        
    }
    Gui_FreePage(&pCommand);
}
//========================================================================
// 函数: Editor_ShowString(Page *p, int sx, int sy, uchar buffer[], uchar op, uchar font， uchar err_pos)
// 描述: 显示字符串函数，把缓冲区的字符串打印到屏幕上，自动换行
// 参数: 与Gui.h里的Gui_ShowString相似，把原来指定颜色用的op改为了err_pos, 用于在错误的字符位置给予反色处理
// 返回: 无
// 版本: V2.0, 2025-11-28
//========================================================================
void Editor_ShowString(Page *p, int sx, int sy, uchar buffer[], uchar font, uchar err_pos)
{
    uchar i, nowx, nowy, dx, dy;
    nowx = sx;
    nowy = sy;
    if (font == 1)
    {
        dx = 8;
        dy = 16;
    }
    else if (font == 0)
    {
        dx = 6;
        dy = 8;
    }
    else
        return;
    for (i = 0; buffer[i] != '\0'; i++)
    {
        if (nowx > p->PSizex - dx)
        {
            nowx = 0;
            nowy += dy;
        }
        if (buffer[i] != '\n')
        {
            Gui_ShowChar(p, nowx, nowy, buffer[i], (i == err_pos ? 0 : 1), font);
            nowx += dx;
        }
        else
        {
            nowx = 0;
            nowy += dy;
        }
    }
    return;
}
// 延时函数
void Editor_delay(uint16_t t)
{
    uint16_t i;
    while (t--)
        for (i = 0; i < 600; i++)
            ;
}
//========================================================================
// 函数: Editor_MoveR(uint8_t *str,uint8_t len,uint8_t m,uint8_t n)
// 描述: //把字符串第m及其之后(m>=0)的元素往右边挪n位（n>=0）
// 参数: Str:目标字符串  len:长度   m:第几个（从0数起）     n:挪几位（从0数起）
// 返回: 无
// 版本: V2.0, 2024-8-2
//========================================================================
void Editor_MoveR(uint8_t *str, uint8_t len, uint8_t m, uint8_t n)
{ // 把字符串第m及其之后(m>=0)的元素往右边挪n位（n>=0）
    char i;
    if (m + n >= len)
        return;
    for (i = len - 1; i >= m; i--)
    {
        if (i >= m + n)
            str[i] = str[i - n];
        else
            str[i] = 0;
    }
}
//========================================================================
// 函数: Editor_MoveL(uint8_t *str,uint8_t len,uint8_t m,uint8_t n)
// 描述: //把字符串第m及其之后(m>=0)的元素往左边边挪n位（n>=0）
// 参数: Str:目标字符串  len:长度   m:第几个（从0数起）     n:挪几位（从0数起）
// 返回: 无
// 版本: V2.0, 2024-8-2
//========================================================================
void Editor_MoveL(uint8_t *str, uint8_t len, uint8_t m, uint8_t n)
{ // 把字符串第m及其之后(m>=0)的元素往左挪n位（n>=0）
    char i;
    if (m + n >= len)
        return;
    for (i = m; i <= len; i++)
    {
        if (i <= len - n)
            str[i] = str[i + n];
        else
            str[i] = 0;
    }
}



uint8_t Editor_Main(Page *pa, uint8_t Code[][32])
{
    //uint8_t Code[select_line][48];

    char pos = 0, offset_x = 0; // 当前光标在字符串的位置， 当前字符串向右的偏移量
    uint8_t file_res = 0;
    uint8_t select_line = 0; //被选中的行下标
    int8_t offset_y = 0; //一屏内的偏移量
    uint8_t show_line_num = 0; //显示行号
    uint8_t i,j;                  // 计数器变量
    uint8_t tick = 0;
    uint8_t curcnt = 0, curx = 0, cury = 0;       // 光标闪烁计数器，光标坐标(x,y)  注意：这里的坐标是指，在2*16字符选区内的坐标。  x取0或1，y取0~15
    uint8_t curflg = 0, alphaflg = 0, betaflg = 0; // 光标显示标志，alpha键标志,beta键标志
    uint8_t Lenth = 0;                            // 缓冲区内表示式长度
    uint8_t kl, kr, keynum;                       // 左半边键码,右半边键码，最终键码。    仲裁逻辑：当左右两边都有按键操作时，优先取右边的键码值.
    uint8_t TitleLenth;                           // 标题长度 0代表无标题
    uint8_t Input_Max_Lenth = 32;                 // 默认32个字符
    uint8_t Error_Pos = 0xff;                     // 错误出现的地方
    //uint8_t line_num = *lnum;
    uint8_t *command[6]; // 指令选择数组
    uint8_t cmd_length = 0; //指令长度
    Input_Max_Lenth = 32;
    Gui_ClearBufferBit(pa, 0);
    Gui_Flush(pa);


    for(i = 0; i < 32; i ++) Code[select_line][i] = 0;
    memcpy(Code[select_line],Code[select_line],32);
    pos = 0;
    offset_x = 0;
    Lenth = 0;
    // TitleLenth = strlen(Title);
    while (Key_Scan() != 0xffff)
        ;
    while (1)
    {
        // 按键扫描
        kl = Key_Scan() >> 8;
        kr = Key_Scan();
        // tick ++;
        if (kl != 0xff)
            keynum = kl;
        else if (kr != 0xff)
            keynum = kr;
        else
            keynum = 0xff;
        if (keynum == 0x3d)
            break;

        //memcpy(Code[select_line],Code[select_line],32);

        // 右半边按键处理
        /*-----------Right--------*/

       if (keynum - 0x30 + '0' >= '0' && keynum - 0x30 + '0' <= '9')
        {
            if (betaflg)
            { // beta标志为真：则按下1~6分别对应A~F
                if(keynum - 0x30 + '0' >= '1' && keynum - 0x30 + '0' <= '6'){
                    Lenth += 1;
                    Editor_MoveR(Code[select_line], Lenth, pos, 1);
                    Code[select_line][pos] = 'A' + keynum - 0x31;
                    pos += 1;
                }
                if(keynum - 0x30 + '0' == '7'){
                    Lenth += 6;
                    Editor_MoveR(Code[select_line], Lenth, pos, 6);
                    Code[select_line][pos] = 's';
                    Code[select_line][pos + 1] = 'i';
                    Code[select_line][pos + 2] = 'n';
                    Code[select_line][pos + 3] = 'h';
                    Code[select_line][pos + 4] = '(';
                    Code[select_line][pos + 5] = ')';
                    pos += 5;
 
                }
                if (keynum - 0x30 + '0' == '8')
                {
                    Lenth += 6;
                    Editor_MoveR(Code[select_line], Lenth, pos, 6);
                    Code[select_line][pos] = 'c';
                    Code[select_line][pos + 1] = 'o';
                    Code[select_line][pos + 2] = 's';
                    Code[select_line][pos + 3] = 'h';
                    Code[select_line][pos + 4] = '(';
                    Code[select_line][pos + 5] = ')';
                    pos += 5;
                }
                if (keynum - 0x30 + '0' == '9')
                {
                    Lenth += 6;
                    Editor_MoveR(Code[select_line], Lenth, pos, 6);
                    Code[select_line][pos] = 't';
                    Code[select_line][pos + 1] = 'a';
                    Code[select_line][pos + 2] = 'n';
                    Code[select_line][pos + 3] = 'h';
                    Code[select_line][pos + 4] = '(';
                    Code[select_line][pos + 5] = ')';
                    pos += 5;
                }
                alphaflg = 0;
                betaflg = 0;
            }
            else if (alphaflg)
            { // alpha标志为真，则按下1 2 3 4 5 6分别对应 x y z u v w
                if (keynum - 0x30 + '0' >= '1' && keynum - 0x30 + '0' <= '6')
                {
                    Lenth += 1;
                    Editor_MoveR(Code[select_line], Lenth, pos, 1);
                    if (keynum - 0x30 + '0' == '1')
                        Code[select_line][pos] = 'x';
                    if (keynum - 0x30 + '0' == '2')
                        Code[select_line][pos] = 'y';
                    if (keynum - 0x30 + '0' == '3')
                        Code[select_line][pos] = 'z';
                    if (keynum - 0x30 + '0' == '4')
                        Code[select_line][pos] = 'u';
                    if (keynum - 0x30 + '0' == '5')
                        Code[select_line][pos] = 'v';
                    if (keynum - 0x30 + '0' == '6')
                        Code[select_line][pos] = 'w';
                    pos += 1;
                    alphaflg = 0;
                    betaflg = 0;
                }
                else if (keynum - 0x30 + '0' == '7')
                { // alpha标志为真，则按下7对应rand()
                    Lenth += 6;
                    Editor_MoveR(Code[select_line], Lenth, pos, 6);
                    Code[select_line][pos] = 'r';
                    Code[select_line][pos + 1] = 'a';
                    Code[select_line][pos + 2] = 'n';
                    Code[select_line][pos + 3] = 'd';
                    Code[select_line][pos + 4] = '(';
                    Code[select_line][pos + 5] = ')';
                    pos += 5;
                    alphaflg = 0;
                    betaflg = 0;
                }
                else if (keynum - 0x30 + '0' == '8')
                { // alpha标志为真，则按下8对应 random(,)
                    Lenth += 9;
                    Editor_MoveR(Code[select_line], Lenth, pos, 9);
                    Code[select_line][pos] = 'r';
                    Code[select_line][pos + 1] = 'a';
                    Code[select_line][pos + 2] = 'n';
                    Code[select_line][pos + 3] = 'd';
                    Code[select_line][pos + 4] = 'o';
                    Code[select_line][pos + 5] = 'm';
                    Code[select_line][pos + 6] = '(';
                    Code[select_line][pos + 7] = ',';
                    Code[select_line][pos + 8] = ')';
                    pos += 7;
                    alphaflg = 0;
                    betaflg = 0;
                }
                else if (keynum - 0x30 + '0' == '9')
                { // alpha标志为真，则按下8对应 random(,)
                    Lenth += 1;
                    Editor_MoveR(Code[select_line], Lenth, pos, 1);
                    Code[select_line][pos] = ',';
                    pos += 1;
                    alphaflg = 0;
                    betaflg = 0;
                }
            }
            else
            { // 没有任何标志的情况下,按键0~9就对应数字0~9
                Lenth++;
                Editor_MoveR(Code[select_line], Lenth, pos, 1);
                Code[select_line][pos] = keynum - 0x30 + '0';
                pos++;
                alphaflg = 0;
                betaflg = 0;
            }
        }
        if (keynum == 0x6e)
        { // 小数点
            Lenth++;
            Editor_MoveR(Code[select_line], Lenth, pos, 1);
            Code[select_line][pos] = '.';
            pos++;
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x6f)
        { // 除
            Lenth++;
            Editor_MoveR(Code[select_line], Lenth, pos, 1);
            Code[select_line][pos] = '/';
            pos++;
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x6a)
        { // 乘
            Lenth++;
            Editor_MoveR(Code[select_line], Lenth, pos, 1);
            Code[select_line][pos] = '*';
            pos++;
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x6d)
        { // 减
            Lenth++;
            Editor_MoveR(Code[select_line], Lenth, pos, 1);
            Code[select_line][pos] = '-';
            pos++;
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x6b)
        { // 加
            Lenth++;
            Editor_MoveR(Code[select_line], Lenth, pos, 1);
            Code[select_line][pos] = '+';
            pos++;
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x47)  //Save
        { // 右括号
            if(alphaflg){
                //pos = 0;
                //Lenth = 0;
                //select_line = 0;
                //offset_x = 0;
                file_res = Save_Program(Code);
                alphaflg = 0;
                betaflg = 0;
            }else{
                Lenth++;
                Editor_MoveR(Code[select_line], Lenth, pos, 1);
                Code[select_line][pos] = ')';
                pos++;
                alphaflg = 0;
                betaflg = 0;
            }
        }
        if (keynum == 0x46)  //Load
        { // 左括号
            if(alphaflg){   
                file_res = Load_Program(Code);
                if(file_res == 0){
                    pos = 0;
                    select_line = 0;
                    Lenth = strlen(Code[select_line]);
                    offset_x = 0;                
                }
                alphaflg = 0;
                betaflg = 0;
            }else{
                Lenth++;
                Editor_MoveR(Code[select_line], Lenth, pos, 1);
                Code[select_line][pos] = '(';
                pos++;
                alphaflg = 0;
                betaflg = 0;
            }
        }
        if (keynum == 0x4b)
        { // 绝对值abs
            if(betaflg){
                Lenth += 5;
                Editor_MoveR(Code[select_line], Lenth, pos, 5);
                Code[select_line][pos] = 'a';
                Code[select_line][pos + 1] = 'b';
                Code[select_line][pos + 2] = 's';
                Code[select_line][pos + 3] = '(';
                Code[select_line][pos + 4] = ')';
                pos += 4;
            }
            else{
                Select_Command(command);
                cmd_length = strlen(command);

                Lenth += cmd_length;
                Editor_MoveR(Code[select_line], Lenth, pos, cmd_length);
                memcpy(&Code[select_line][pos],command, cmd_length);
                pos += cmd_length ;
            }
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x55)
        { // 删除
            if (pos > 0 && Lenth > 0)
            {
                Editor_MoveL(Code[select_line], Lenth, pos - 1, 1);
                Lenth--;
                pos--;
                alphaflg = 0;
                betaflg = 0;
            }
        }

        // 左边按键处理
        /*-----------Left--------*/
        if (keynum == 0x41)
        { // alpha键
            if (!alphaflg)
                alphaflg = 0xff;
            else
                alphaflg = 0;
            betaflg = 0;
        }
       if (keynum == 0x58)
        {  //beta键
            if (!betaflg)
                betaflg = 0xff;
            else
                betaflg = 0;
            alphaflg = 0;
        }
        if (keynum == 0x51)
        { // 度数d
            Lenth += 1;
            Editor_MoveR(Code[select_line], Lenth, pos, 1);
            Code[select_line][pos] = 'd';
            pos += 1;
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x43)
        { // π或者e
            if (alphaflg)
            {
                Lenth += 2;
                Editor_MoveR(Code[select_line], Lenth, pos, 2);
                Code[select_line][pos] = 'P';
                Code[select_line][pos + 1] = 'i';
                pos += 2;
            }
            else
            {
                Lenth += 1;
                Editor_MoveR(Code[select_line], Lenth, pos, 1);
                Code[select_line][pos] = 'e';
                pos += 1;
            }
            alphaflg = 0;
            betaflg = 0;
        }
        
        // 下面的数学表达式输入就不再赘述，方法实现同上，数学函数名看代码就能知道
        if (keynum == 0x4c)
        {
            if (alphaflg)
            {
                Lenth += 6;
                Editor_MoveR(Code[select_line], Lenth, pos, 6);
                Code[select_line][pos] = '^';
                Code[select_line][pos + 1] = '(';
                Code[select_line][pos + 2] = '1';
                Code[select_line][pos + 3] = '/';
                Code[select_line][pos + 4] = '2';
                Code[select_line][pos + 5] = ')';
                pos += 6;
            }
            else
            {
                Lenth += 2;
                Editor_MoveR(Code[select_line], Lenth, pos, 2);
                Code[select_line][pos] = '^';
                Code[select_line][pos + 1] = '2';
                pos += 2;
            }
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x4d)
        {
            if (alphaflg)
            {
                Lenth += 5;
                Editor_MoveR(Code[select_line], Lenth, pos, 5);
                Code[select_line][pos] = '^';
                Code[select_line][pos + 1] = '(';
                Code[select_line][pos + 2] = '1';
                Code[select_line][pos + 3] = '/';
                Code[select_line][pos + 4] = ')';
                pos += 4;
            }
            else
            {
                Lenth += 3;
                Editor_MoveR(Code[select_line], Lenth, pos, 3);
                Code[select_line][pos] = '^';
                Code[select_line][pos + 1] = '(';
                Code[select_line][pos + 2] = ')';
                pos += 2;
            }
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x4e)
        {
            if (alphaflg)
            {
                Lenth += 6;
                Editor_MoveR(Code[select_line], Lenth, pos, 6);
                Code[select_line][pos] = 'f';
                Code[select_line][pos + 1] = 'a';
                Code[select_line][pos + 2] = 'c';
                Code[select_line][pos + 3] = 't';
                Code[select_line][pos + 4] = '(';
                Code[select_line][pos + 5] = ')';
                pos += 5;
            }
            else
            {
                Lenth += 5;
                Editor_MoveR(Code[select_line], Lenth, pos, 5);
                Code[select_line][pos] = '^';
                Code[select_line][pos + 1] = '(';
                Code[select_line][pos + 2] = '-';
                Code[select_line][pos + 3] = '1';
                Code[select_line][pos + 4] = ')';
                pos += 5;
            }
            alphaflg = 0;
            betaflg = 0;
        }

        if (keynum == 0x52)
        {
            if (alphaflg)
            {
                Lenth += 6;
                Editor_MoveR(Code[select_line], Lenth, pos, 6);
                Code[select_line][pos] = 'a';
                Code[select_line][pos + 1] = 's';
                Code[select_line][pos + 2] = 'i';
                Code[select_line][pos + 3] = 'n';
                Code[select_line][pos + 4] = '(';
                Code[select_line][pos + 5] = ')';
                pos += 5;
            }
            else
            {
                Lenth += 5;
                Editor_MoveR(Code[select_line], Lenth, pos, 5);
                Code[select_line][pos] = 's';
                Code[select_line][pos + 1] = 'i';
                Code[select_line][pos + 2] = 'n';
                Code[select_line][pos + 3] = '(';
                Code[select_line][pos + 4] = ')';
                pos += 4;
            }
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x53)
        {
            if (alphaflg)
            {
                Lenth += 6;
                Editor_MoveR(Code[select_line], Lenth, pos, 6);
                Code[select_line][pos] = 'a';
                Code[select_line][pos + 1] = 'c';
                Code[select_line][pos + 2] = 'o';
                Code[select_line][pos + 3] = 's';
                Code[select_line][pos + 4] = '(';
                Code[select_line][pos + 5] = ')';
                pos += 5;
            }
            else
            {
                Lenth += 5;
                Editor_MoveR(Code[select_line], Lenth, pos, 5);
                Code[select_line][pos] = 'c';
                Code[select_line][pos + 1] = 'o';
                Code[select_line][pos + 2] = 's';
                Code[select_line][pos + 3] = '(';
                Code[select_line][pos + 4] = ')';
                pos += 4;
            }
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x54)
        {
            if (alphaflg)
            {
                Lenth += 6;
                Editor_MoveR(Code[select_line], Lenth, pos, 6);
                Code[select_line][pos] = 'a';
                Code[select_line][pos + 1] = 't';
                Code[select_line][pos + 2] = 'a';
                Code[select_line][pos + 3] = 'n';
                Code[select_line][pos + 4] = '(';
                Code[select_line][pos + 5] = ')';
                pos += 5;
            }
            else
            {
                Lenth += 5;
                Editor_MoveR(Code[select_line], Lenth, pos, 5);
                Code[select_line][pos] = 't';
                Code[select_line][pos + 1] = 'a';
                Code[select_line][pos + 2] = 'n';
                Code[select_line][pos + 3] = '(';
                Code[select_line][pos + 4] = ')';
                pos += 4;
            }
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x4f)
        {
            if (alphaflg)
            {
                Lenth += 4;
                Editor_MoveR(Code[select_line], Lenth, pos, 4);
                Code[select_line][pos] = 'l';
                Code[select_line][pos + 1] = 'n';
                Code[select_line][pos + 2] = '(';
                Code[select_line][pos + 3] = ')';
                pos += 3;
            }
            else
            {
                Lenth += 2;
                Editor_MoveR(Code[select_line], Lenth, pos, 2);
                Code[select_line][pos] = 'e';
                Code[select_line][pos + 1] = '^';
                pos += 2;
            }
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x50)
        {
            if (alphaflg)
            {
                Lenth += 4;
                Editor_MoveR(Code[select_line], Lenth, pos, 5);
                Code[select_line][pos] = 'l';
                Code[select_line][pos + 1] = 'g';
                Code[select_line][pos + 2] = '(';
                Code[select_line][pos + 3] = ')';
                pos += 3;
            }
            else
            {
                Lenth += 3;
                Editor_MoveR(Code[select_line], Lenth, pos, 3);
                Code[select_line][pos] = '1';
                Code[select_line][pos + 1] = '0';
                Code[select_line][pos + 2] = '^';
                pos += 3;
            }
            alphaflg = 0;
            betaflg = 0;
        }

        if (keynum == 0x57)
        {
            Lenth += 3;
            Editor_MoveR(Code[select_line], Lenth, pos, 3);
            Code[select_line][pos] = 'A';
            Code[select_line][pos + 1] = 'n';
            Code[select_line][pos + 2] = 's';
            pos += 3;
            alphaflg = 0;
            betaflg = 0;
        }
        if (keynum == 0x45)
        {
            // Quit = 1;
            return 0;
        } // 退出
        if (keynum == 0x56)
        {
            Lenth = 0;
            pos = 0;
        }
        /*---------------字符串和光标位置逻辑处理---------------------*/

        if (Lenth >= Input_Max_Lenth)
        {
            Lenth = Input_Max_Lenth - 1;
        }

        if (keynum == 0x48)
        {
            pos--;
        }
        if (keynum == 0x4a)
        {
            pos++;
        }
        if (pos < 0)
        {
            pos = Lenth;
        }
        if (pos > Lenth)
        {
            pos = 0;
            offset_x = 0;
        }
        if (pos > Line_Length)
        {
            offset_x = pos - Line_Length;
        }
        else
        {
            offset_x = 0;
        }
        if (keynum == 0x44)
        {
            if(select_line > 0 ){
                select_line --;    
                Lenth = strlen(Code[select_line]);
                pos = 0;
                offset_x = 0;
            }           
        }            
        if (keynum == 0x49)
        {
             if(select_line < 32 ){
                select_line ++; 
                Lenth = strlen(Code[select_line]);
                pos = 0;
                offset_x = 0;
             }     
        }    
        if(select_line >= offset_y + 4 ){
            offset_y = select_line - 4;
        }

        if(select_line < offset_y){
            offset_y = select_line; 
        }

        for (i = Lenth; i < 32; i++)
            Code[select_line][i] = '\0'; // 清空有效长度之后的缓冲内容

        Gui_ClearBufferBit(pa, 0);
        Gui_DrawRectangleWired(pa, 0, 0, 120, 48, 1);
        Gui_DrawRectangleRound(pa, 120, 0, 8, 48, 4, 1);
        Gui_DrawCircleFilled(pa,124,4 + 1.3 * select_line,4,1 );
        //cury = line_num;
        //OLED_Printf(0,2,16,"%bd %bd %bd", offset_x,pos,Lenth);
        for(i = 0; i < 5; i ++ ) {   
            if( offset_y + i == select_line){
                //Gui_ShowString(pa,1,9 * i , Code[offset_y +i] ,0,0);
                curx = pos - offset_x;
                cury = i;
                for (j = 0; j < (Lenth >= Line_Length ? Line_Length : Lenth); j++)
                    Gui_ShowChar(pa,  1 + j * 6, cury * 8 + 1, Code[select_line][j + offset_x], 0, 0);
                if (curflg)
                    Gui_DrawLine(pa,  2 + curx * 6, cury * 8 + 1,  2 + curx * 6 , cury * 8 + 1 + 8, 1);
                else
                    Gui_DrawLine(pa,  2 + curx * 6 , cury * 8 + 1, 2 + curx * 6 , cury * 8 + 1 + 8, 0);
            }else{
                cury = i;
                for (j = 0; j < (strlen(Code[i + offset_y]) >= Line_Length ? Line_Length : strlen(Code[i + offset_y])); j++)
                    Gui_ShowChar(pa, 1 + j * 6, cury * 8 + 1, Code[i + offset_y][j], 1, 0);
            }
            //else Gui_ShowString(pa,1,9 * i ,Code[offset_y +i] ,1,0);
            //maxl = (strlen(cmd_list[offset_y +i]) > maxl ? strlen(cmd_list[offset_y +i]) : maxl);
        }
        
        if (alphaflg || betaflg){
            if(alphaflg) Gui_ShowString(pa, 24, 56, "alpha", 1, 0);
            if (betaflg) Gui_ShowString(pa, 80, 56, "beta", 1, 0);
        }else{
            if(Prog_Id == 0xff) Gui_Printf(pa,16, 56 ,1,0, "No Active File");
            else Gui_Printf(pa,16, 56 ,1,0, "Current File:%bu", Prog_Id);
        }
        Gui_Flush(pa);             // 刷屏
        Editor_delay(500);
        if (curcnt > 3)
        {
            curflg = !curflg;
            curcnt = 0;
        } // 循环4次闪一下
        curcnt++;
    }
    return Lenth; // 传回表达式长度
}

