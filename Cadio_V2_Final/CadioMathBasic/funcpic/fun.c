#include "fun.h"
#include "cal.h"
#include <stdlib.h>

code uint8_t Arrow[2][8] = {
    /*--  调入了一幅图像：这是您新建的图像  --*/
    /*--  宽度x高度=8x8  --*/
    {0xFE, 0xF9, 0xE1, 0x83, 0xE3, 0xD7, 0xB7, 0x7F}, // 正常颜色

    /*--  调入了一幅图像：这是您新建的图像  --*/
    /*--  宽度x高度=8x8  --*/
    {0x01, 0x06, 0x1E, 0x7C, 0x1C, 0x28, 0x48, 0x80} // 黑白反色
};
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
void Draw_Axis(Page *pa, int ox, int oy, float alphax, float alphay)
{
    float fi;
    Gui_DrawLine(pa, 0, oy, 127, oy, 1);
    Gui_DrawLine(pa, ox, 0, ox, 63, 1);
    Gui_Printf(pa, 0, 0, 1, 0, "dx:%.2f", alphax * 12.8);
    Gui_Printf(pa, 0, 9, 1, 0, "dy:%.2f", alphay * 6.4);
    for (fi = 0; fi < 128; fi += 12.8)
        Gui_DrawPoint(pa, fi, oy - 1, 1);
    for (fi = 0; fi < 64; fi += 6.4)
        Gui_DrawPoint(pa, ox - 1, fi, 1);
}
void Draw_Cur(Page *pa, int ox, int oy, float alphax, float alphay)
{
    Page pCur, pBak; // 光标用小页面,备份页面，免得光标破坏原来的页面
    uint8_t keynum, i, j, curposflg = 0;
    int curx = 80, cury = 48;
    Gui_CreatePage(&pCur, 8, 8);
    Gui_ClearBufferBit(&pCur, 0);
    Gui_CreatePage(&pBak, 8, 8);
    Gui_ClearBufferBit(&pBak, 0);
    while (1)
    {
        keynum = Key_Scan() & 0x00ff;
        if (Key_Scan() >> 8 == 0x45)
            break;
        if (keynum == 0x44 && cury > 0)
        {
            cury--;
        }
        if (keynum == 0x49 && cury < 63)
        {
            cury++;
        }
        if (keynum == 0x48 && curx > 0)
        {
            curx--;
        }
        if (keynum == 0x4a && curx < 127)
        {
            curx++;
        }
        if (keynum == 0x3d)
            curposflg = !curposflg;
        for (i = 0; i < 8; i++)
        {
            for (j = 0; j < 8; j++)
            {
                Gui_DrawPoint(&pBak, i, j, Gui_GetPoint(pa, i + curx, j + cury));
            }
        }
        if (curposflg)
        {
            Gui_Printf(pa, 80, 47, 1, 0, "X:%.2f", (curx - ox) * alphax);
            Gui_Printf(pa, 80, 55, 1, 0, "Y:%.2f", (oy - cury) * alphay);
        }
        else
        {
            Gui_ShowString(pa, 80, 47, "        ", 1, 0);
            Gui_ShowString(pa, 80, 55, "        ", 1, 0);
        }
        Gui_DrawBmp(&pCur, 0, 0, 8, 8, Arrow[0]);
        Gui_SetPosAbs(&pCur, curx, cury);
        Gui_CopyPage(&pCur, pa, 1);
        Gui_Flush(pa);
        Gui_DrawBmp(&pCur, 0, 0, 8, 8, Arrow[1]);
        Gui_SetPosAbs(&pCur, curx, cury);
        Gui_CopyPage(&pCur, pa, 2);
        Gui_SetPosAbs(&pBak, curx, cury);
        Gui_CopyPage(&pBak, pa, 1);
    }
    Gui_FreePage(&pCur);
    Gui_FreePage(&pBak);
    return;
}
// 延时函数
void Fun_delay(uint16_t t)
{
    uint16_t i;
    while (t--)
        for (i = 0; i < 600; i++)
            ;
}
//========================================================================
// 函数: Fun_Select(Page *pa)
// 描述: 函数绘图模式选择界面
// 参数: Page *pa 目标页面
// 返回:
// 版本: V2.0, 2024-8-12
//========================================================================
uint8_t Fun_Select(Page *pa)
{
    float sy, sx;                    // sx:框框右边界横坐标 sy:框框纵坐标
    float dy, dx;                    // 纵向单次增量，横向单次增量 (都可以理解为速度)
    uint8_t table1[] = {11, 13, 12}; // 每个条目字符个数-2
    uint8_t table2[] = {13, 12, 13}; // 每个条目字符个数-2
    uint8_t option = 0, keynum;      // 选项0:Invalid  1:单元函数 2:隐函数3:参数方程
    uint8_t nextpage = 0;            // 标识着换到下一页，因为一页显示不下4个条目
    Gui_ClearBufferBit(pa, 0);
    Gui_Flush(pa);
    option = 1;
    sx = 0;
    dx = 0;
    sy = 16;
    dy = 0;
    nextpage = 0;
    sx = table1[0] * 8;
    while (Key_Scan() != 0xffff)
        ;
    while (1)
    {
        keynum = Key_Scan() & 0x00ff;
        if (Key_Scan() >> 8 == 0x45)
            return 0;
        if (keynum == 0x44)
            option--;
        if (keynum == 0x49)
            option++;
        if (keynum == 0x3d)
            return option;
        if (keynum == 0x31)
            return 1;
        if (keynum == 0x32)
            return 2;
        if (keynum == 0x33)
            return 3;
        if (keynum == 0x34)
            return 4;
        if (option < 1)
            option = 4;
        if (option > 4)
            option = 1;
        if (option == 4)
            nextpage = 1;
        else
            nextpage = 0;
        if (nextpage == 0)
        {
            Gui_ShowString(pa, 0, 0, "Mode Select:", 1, 1);
            Gui_ShowString(pa, 0, 16, "1:Func Uanary", 1, 1);   // 单元函数
            Gui_ShowString(pa, 0, 32, "2:Func Implicit", 1, 1); // 隐函数
            Gui_ShowString(pa, 0, 48, "3:Func Dynamic", 1, 1);  // 参数方程
            Gui_DrawRectangleWired(pa, 16, sy, sx, 16, 1);      // 绘制框框
            dy = (option * 16 - sy) * 0.7;
            dx = (table1[option - 1] * 8 - sx) * 0.7;
            // 一阶常系数线性  运动模型
        }
        else
        {
            Gui_ShowString(pa, 0, 0, "Mode Select:", 1, 1);
            Gui_ShowString(pa, 0, 16, "2:Func Implicit", 1, 1); // 隐函数
            Gui_ShowString(pa, 0, 32, "3:Func Dynamic", 1, 1);  // 动态函数
            Gui_ShowString(pa, 0, 48, "4:Parm Equation", 1, 1); // 参数方程
            Gui_DrawRectangleWired(pa, 16, sy, sx, 16, 1);      // 绘制框框
            dy = ((option - 1) * 16 - sy) * 0.7;
            dx = (table2[option - 2] * 8 - sx) * 0.7;
            // 小动画
        }
        sy += dy;
        sx += dx;
        Gui_Flush(pa);
        Gui_ClearBufferBit(pa, 0);
    }
}
//========================================================================
// 函数: Fun_Unary(Page *pa)
// 描述: 函数绘图模式----单元函数
// 参数: Page *pa 目标页面
// 返回: 无
// 版本: V2.0, 2024-8-12
//========================================================================
void Fun_Unary(Page *pa)
{
    uint8_t keynum = 0, kl = 0, kr = 0;               // 键码  左键码  右键码
    uint8_t shiftflg = 0, backflg = 0, curposflg = 0; // shift标志  back标志（用于放置连按不小心退出）光标坐标显示标志，用于指示当前坐标
    uint8_t Expr[6][MaxLenth];                        // 9组表达式
    uint8_t Lenth[6];                                 // 9组表达式长度
    uint8_t Status[6];                                // 9组表达式状态   0:隐藏 1:绘制
    float parax[7];                                   // 参数标
    uint8_t op = 0;                                   // 选项op
    int i, j;                                         // 整数计数器变量
    float fi;                                         // 浮点计数器变量
    float alphax = 1, alphay = 1, ox = 64, oy = 32;   // 一个像素对应多少单位长度
    float dx = alphax = 1;                            // 迭代精度是多少个单位长度
    float rx, resy;                                   // rx:自变量值。 resy:因变量值，是把rx代入算出来的纵坐标原始值.
    float  now_y,prv_y;                                 // 当前纵坐标值和上一个纵坐标值，用于估算连续性
    float step_y= 0;                                    //  竖直方向要额外补偿的点纵坐标迭代器，用于让函数图像连续。

                                                      //  机理：当检测到当前点和上一点数值差距大于1，
                                                      //        会自动在竖直方向上插入若干点使得图像连续

    int curx = 64, cury = 32;                         // 当前光标位置
    uint8_t Expr_Max_Lenth = 32;                      // 默认32个字符
    if (Set_LongExpression_Flg())
        Expr_Max_Lenth = 84; // 如果使用长模式，就是84个字符
    else
        Expr_Max_Lenth = 32;
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < Expr_Max_Lenth; j++)
        {
            Expr[i][j] = 0;
        }
        Lenth[i] = 0;
        Status[i] = 0;
    }
    // 表达式组初始化
    while (Key_Scan() != 0xffff)
        ;
    while (1)
    {
        // 读取左右键码并仲裁
        keynum = Key();
        if (keynum == 0x45 && backflg == 0)
            break;
        if (keynum == 0x44)
        {
            oy -= 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x49)
        {
            oy += 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x4a)
        {
            ox += 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x48)
        {
            ox -= 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x41)
            shiftflg = 1;
        if (shiftflg == 0)
        {
            if (keynum == 0x31)
            {
                alphax *= 2;
                backflg = 0;
            }
            if (keynum == 0x32)
            {
                alphax /= 2;
                backflg = 0;
            }
            if (keynum == 0x34)
            {
                alphay *= 2;
                backflg = 0;
            }
            if (keynum == 0x35)
            {
                alphay /= 2;
                backflg = 0;
            }
            if (keynum == 0x37)
            {
                dx *= 2;
                backflg = 0;
            }
            if (keynum == 0x38)
            {
                dx /= 2;
                backflg = 0;
            }
        }
        else
        {
            if (keynum >= 0x31 && keynum <= 0x36)
            {
                while (Key_Scan() != 0xffff)
                    ;
                Lenth[keynum - 0x31] = Cal_Input(pa, Expr[keynum - 0x31], "Func_Unary:");
                Status[keynum - 0x31] = 1;
                shiftflg = 0;
                backflg = 0;
            }
            if (keynum == 0x46)
            {
                Gui_ShowString(pa, 96, 0, "     ", 1, 0);
                curx = 80;
                cury = 24;
                Draw_Cur(pa, ox, oy, alphax, alphay);
                shiftflg = 0;
                backflg = 0;
            }
            while (Key_Scan() != 0xffff)
                ;
        }
        Gui_ClearBufferBit(pa, 0);
        Draw_Axis(pa, ox, oy, alphax, alphay);
        if (shiftflg)
            Gui_ShowString(pa, 96, 0, "shift", 1, 0);
        for (j = 0; j < 6; j++)
        {
            if (!Status[j])
                continue;
            FastCal_Init(Expr[j], Lenth[j], 0);
            now_y = 0;
            prv_y= 0;
            for (fi = 0; fi < 127; fi += dx)
            {
                rx = (fi - ox) * alphax;
                // parax[0]=rx;parax[1]=rx;parax[2]=rx;
                // parax[3]=rx;
                parax[4] = rx;
                parax[5] = rx;
                parax[6] = rx;
                resy = FastCal_Work(parax, 0);
                Cal_UpdateAns(resy);
                now_y = resy / alphay;
                if(Set_AutoContinue_Flg()){ //是否开启连续性补偿
                    if(fi > 0.01f){  //float比大小,让他至少第一个点不进行连续性检测
                        if(now_y >= prv_y) {for(step_y = prv_y; step_y <= now_y && oy - step_y >= 0 && oy - step_y < 64 ;step_y++) Gui_DrawPoint(pa, fi, oy - step_y, 1);}
                        if(now_y <  prv_y) {for(step_y = now_y; step_y <= prv_y && oy - step_y >= 0 && oy - step_y < 64 ;step_y++) Gui_DrawPoint(pa, fi, oy - step_y, 1);}
                    }
                }else{
                    Gui_DrawPoint(pa, fi, oy - now_y, 1);
                }
                prv_y = now_y;
            }
        }
        Gui_Flush(pa);
    }
}
//========================================================================
// 函数: Fun_Implicit(Page *pa)
// 描述: 函数绘图模式----XY的隐函数
// 参数: Page *pa 目标页面
// 返回: 无
// 版本: V2.0, 2024-8-12
//========================================================================
void Fun_Implicit(Page *pa)
{
    uint8_t keynum = 0, kl = 0, kr = 0;                                         // 键码  左键码  右键码
    uint8_t shiftflg = 0, backflg = 0, curposflg = 0, drawflg = 0, quitflg = 0; // #shift标志   #back标志（用于放置连按不小心退出）  #光标坐标显示标志，用于指示当前坐标    #启动绘制标志  #函数绘制过程中退出标志
    uint8_t Expr[MaxLenth];                                                     // 表达式
    uint8_t Lenth;                                                              // 表达式长度
    uint8_t Status;                                                             // 表达式状态   0:隐藏 1:绘制
    float parax[7];                                                             // 参数表
    uint8_t op = 0;                                                             // 选项op
    int i;                                                                      // 整数计数器变量
    float fi, fj;                                                               // 浮点计数器变量
    float alphax = 1, alphay = 1, ox = 64, oy = 32;                             // 一个像素对应多少单位长度
    float dx = alphax = 1, dy = 1;                                              // 迭代精度是多少个单位长度
    float rx, ry, res;                                                          // 代入计算的横坐标真实值，结果;
    int RandomCal_Cnt = 0, RandomCal_Total = 0;                                 // 随机迭代计数器（每迭代256次执行一次刷屏）,  总共迭代次数
    float Acc = 0.05;                                                           // 准确性，函数值小于此值才会画出对应的点     精度控制界面里的选项值(1~3)  dx dy Acc
    uint8_t Acc_Set = 0;                                                        // 准确性选项
    float posy, posx, vy, vx, desy, desx;                                       // pos当前位置，v速度，des目标位置  用于选择页面的动画
    int curx = 64, cury = 32;                                                   // 当前光标位置
    uint8_t Len_Table[] = {64, 64, 80};
    uint8_t Expr_Max_Lenth = 32; // 默认32个字符
    if (Set_LongExpression_Flg())
        Expr_Max_Lenth = 84; // 如果使用长模式，就是96个字符
    else
        Expr_Max_Lenth = 32;
    for (i = 0; i < Expr_Max_Lenth; i++)
        Expr[i] = 0;
    Lenth = 0;
    Status = 0;
    // 表达式组初始化

    Gui_ClearBufferBit(pa, 0);
    Gui_Flush(pa);
    // 光标小页面初始化
    alphax = 0.05;
    alphay = 0.05;
    while (Key_Scan() != 0xffff)
        ;
    while (1)
    {
        keynum = Key();
        if (keynum == 0x45 && backflg == 0)
            break;
        if (keynum == 0x44)
        {
            oy -= 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x49)
        {
            oy += 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x4a)
        {
            ox += 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x48)
        {
            ox -= 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x41)
            shiftflg = 1;

        if (shiftflg == 0)
        {
            if (keynum == 0x31)
            {
                alphax *= 2;
                backflg = 0;
            }
            if (keynum == 0x32)
            {
                alphax /= 2;
                backflg = 0;
            }
            if (keynum == 0x34)
            {
                alphay *= 2;
                backflg = 0;
            }
            if (keynum == 0x35)
            {
                alphay /= 2;
                backflg = 0;
            }
        }
        else
        {
            if (keynum == 0x31)
            {
                while (Key_Scan() != 0xffff)
                    ;
                Lenth = Cal_Input(pa, Expr, "Func Implicit:");
                Status = 1;
                shiftflg = 0;
                backflg = 0;
                drawflg = 1;
            }
            if (keynum == 0x4b)
            {
                Acc_Set = 1;
                posy = Acc_Set * 16;
                posx = desx = Len_Table[Acc_Set - 1] + 4;
                desy = Acc_Set * 16;
                desx = Len_Table[Acc_Set - 1] + 4;
                while (Key_Scan() != 0xffff)
                    ;
                Gui_ClearBufferBit(pa, 0);
                Gui_Flush(pa);
                while (1)
                {
                    Gui_ShowString(pa, 0, 0, "Accuracy Config", 1, 1);
                    keynum = Key_Scan() & 0x00ff;
                    if (keynum == 0x3d)
                        break;
                    if (keynum == 0x44 && Acc_Set > 1)
                        Acc_Set--;
                    if (keynum == 0x49 && Acc_Set < 3)
                        Acc_Set++;
                    if (keynum == 0x48)
                    {
                        if (Acc_Set == 1)
                            dx -= 0.05;
                        if (Acc_Set == 2)
                            dy -= 0.05;
                        if (Acc_Set == 3)
                            Acc -= 0.01;
                    }
                    if (keynum == 0x4a)
                    {
                        if (Acc_Set == 1)
                            dx += 0.01;
                        if (Acc_Set == 2)
                            dy += 0.01;
                        if (Acc_Set == 3)
                            Acc += 0.005;
                    }
                    if (dx < 0.2)
                        dx = 0.2;
                    if (dx > 2)
                        dx = 2;
                    if (dy < 0.2)
                        dy = 0.2;
                    if (dy > 2)
                        dy = 2;
                    if (Acc < 0.005)
                        Acc = 0x005;
                    if (Acc > 0.1)
                        Acc = 0.1;
                    Gui_Printf(pa, 16, 16, 1, 1, "dx: %.2f", dx);
                    Gui_Printf(pa, 16, 32, 1, 1, "dy: %.2f", dy);
                    Gui_Printf(pa, 16, 48, 1, 1, "Acc: %.3f", Acc);
                    desy = Acc_Set * 16;
                    desx = desx = Len_Table[Acc_Set - 1] + 4;
                    vy = (desy - posy) * 0.7;
                    vx = (desx - posx) * 0.7;
                    posy += vy;
                    posx += vx;
                    Gui_DrawRectangleWired(pa, 12, posy, posx, 16, 1);
                    Gui_Flush(pa);
                    Gui_ClearBufferBit(pa, 0);
                }
            }
            while (Key_Scan() != 0xffff)
                ;
        }
        // while(Key_Scan()!=0xffff);
        Gui_ClearBufferBit(pa, 0);
        Draw_Axis(pa, ox, oy, alphax, alphay);
        if (shiftflg)
            Gui_ShowString(pa, 96, 0, "shift", 1, 0);
        Gui_Flush(pa);
        if (drawflg)
        {
            if (Lenth == 0)
                continue;
            FastCal_Init(Expr, Lenth, 0);
            quitflg = 0;
            if (1)
            {
                // 正常计算
                for (fi = 0; fi < 128; fi += dx)
                {
                    if (quitflg)
                        break;
                    for (fj = 0; fj < 64; fj += dy)
                    {
                        rx = (fi - ox) * alphax;
                        ry = (oy - fj) * alphay;
                        parax[4] = rx;
                        parax[5] = ry;
                        res = FastCal_Work(parax, 0);
                        if (fabs(res) < Acc && Get_ErrorCode() == 0)
                            Gui_DrawPoint(pa, fi, fj, 1);
                    }
                    if (Key_Scan() != 0xffff || Get_ErrorCode())
                    {
                        quitflg = 1;
                        break;
                    }
                    Gui_Printf(pa, 104, 0, 1, 0, "%.0f%%", fi * 0.79);
                    Gui_Flush(pa);
                }
                while (Key_Scan() != 0xffff)
                    ;
            }
            curx = 80;
            cury = 48;
            Draw_Cur(pa, ox, oy, alphax, alphay);
            drawflg = 0;
            shiftflg = 0;
            quitflg = 0;
            while (Key_Scan() != 0xffff)
                ;
        }
    }
}
void Fun_Dynamic(Page *pa)
{
    Page pPara;                                       // 光标用小页面,备份页面，免得光标破坏原来的页面
    uint8_t keynum = 0, kl = 0, kr = 0;               // 键码  左键码  右键码
    uint8_t shiftflg = 0, backflg = 0, curposflg = 0; // #shift标志  #back标志（用于放置连按不小心退出）  #光标坐标显示标志，用于指示当前坐标
    uint8_t paraflg = 0;                              // 参数标志 0:invalid   1:u 2:v 3:w
    uint8_t Expr[6][MaxLenth];                        // 表达式
    uint8_t Lenth[6];                                 // 9组表达式长度
    uint8_t Status[6];                                // 9组表达式状态   0:隐藏 1:绘制
    float parax[7];                                   // 参数表
    float dpara = 0.1;                                // 参数增量
    uint8_t op = 0;                                   // 选项op
    int i, j;                                         // 整数计数器变量
    float fi;                                         // 浮点计数器变量
    float alphax = 1, alphay = 1, ox = 64, oy = 32;   // 一个像素对应多少单位长度
    float dx = alphax = 1;                            // 迭代精度是多少个单位长度
    float rx, resy;                                   // 代入计算的横坐标真实值，结果纵坐标;
    float  now_y,prv_y;                                 // 当前纵坐标值和上一个纵坐标值，用于估算连续性
    float step_y= 0;                                    //  竖直方向要额外补偿的点纵坐标迭代器，用于让函数图像连续。

                                                      //  机理：当检测到当前点和上一点数值差距大于1，
                                                      //        会自动在竖直方向上插入若干点使得图像连续
    int curx = 64, cury = 32;                         // 当前光标位置
    

    uint8_t Expr_Max_Lenth = 32; // 默认32个字符
    if (Set_LongExpression_Flg())
        Expr_Max_Lenth = 84; // 如果使用长模式，就是96个字符
    else
        Expr_Max_Lenth = 32;
    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < Expr_Max_Lenth; j++)
        {
            Expr[i][j] = 0;
        }
        Lenth[i] = 0;
        Status[i] = 0;
    }
    for (i = 0; i < 7; i++)
        parax[i] = 1;
    // 表达式组初始化
    Gui_CreatePage(&pPara, 56, 16);
    Gui_ClearBufferBit(&pPara, 0);
    // 光标小页面初始化
    while (Key_Scan() != 0xffff)
        ;
    while (1)
    {
        // 读取左右键码并仲裁
        keynum = Key();
        if (keynum == 0x45 && backflg == 0)
            break;
        if (paraflg == 0)
        {
            if (keynum == 0x44)
            {
                oy -= 5;
                shiftflg = 0;
                backflg = 0;
            }
            if (keynum == 0x49)
            {
                oy += 5;
                shiftflg = 0;
                backflg = 0;
            }
            if (keynum == 0x4a)
            {
                ox += 5;
                shiftflg = 0;
                backflg = 0;
            }
            if (keynum == 0x48)
            {
                ox -= 5;
                shiftflg = 0;
                backflg = 0;
            }
            if (keynum == 0x41)
                shiftflg = !shiftflg;
        }
        else
        {
            Gui_ShowChar(&pPara, 0, 0, paraflg + 'u' - 1, 1, 0);
            Gui_ShowChar(&pPara, 6, 0, ':', 1, 0);
            Gui_Printf(&pPara, 12, 0, 1, 0, "%.2f", parax[paraflg]);
            Gui_ShowChar(&pPara, 0, 8, 'd', 1, 0);
            Gui_ShowChar(&pPara, 6, 8, paraflg + 'u' - 1, 1, 0);
            Gui_ShowChar(&pPara, 12, 8, ':', 1, 0);
            Gui_Printf(&pPara, 18, 8, 1, 0, "%.2f", dpara);
            if (keynum == 0x44)
            {
                dpara *= 2;
                shiftflg = 0;
                backflg = 0;
            }
            if (keynum == 0x49)
            {
                dpara /= 2;
                shiftflg = 0;
                backflg = 0;
            }
            if (keynum == 0x4a)
            {
                parax[paraflg] += dpara;
                shiftflg = 0;
                backflg = 0;
            }
            if (keynum == 0x48)
            {
                parax[paraflg] -= dpara;
                shiftflg = 0;
                backflg = 0;
            }
            if (keynum == 0x34)
            {
                paraflg = 1;
            }
            if (keynum == 0x35)
            {
                paraflg = 2;
            }
            if (keynum == 0x36)
            {
                paraflg = 3;
            }
            if (keynum == 0x3d)
                paraflg = 0;
            shiftflg = 0;
        }
        if (shiftflg == 0 && paraflg == 0)
        {
            if (keynum == 0x31)
            {
                alphax *= 2;
                backflg = 0;
            }
            if (keynum == 0x32)
            {
                alphax /= 2;
                backflg = 0;
            }
            if (keynum == 0x34)
            {
                alphay *= 2;
                backflg = 0;
            }
            if (keynum == 0x35)
            {
                alphay /= 2;
                backflg = 0;
            }
            if (keynum == 0x37)
            {
                dx *= 2;
                backflg = 0;
            }
            if (keynum == 0x38)
            {
                dx /= 2;
                backflg = 0;
            }
        }
        else
        {
            if (keynum >= 0x34 && keynum <= 0x36)
                paraflg = keynum - 0x33;
            if (keynum >= 0x31 && keynum <= 0x33)
            {
                while (Key_Scan() != 0xffff)
                    ;
                Lenth[keynum - 0x31] = Cal_Input(pa, Expr[keynum - 0x31], "Func_Dynamic:");
                Status[keynum - 0x31] = 1;
                shiftflg = 0;
                backflg = 0;
            }
            if (keynum == 0x46)
            {
                Gui_ShowString(pa, 96, 0, "     ", 1, 0);
                curx = 80;
                cury = 24;
                Draw_Cur(pa, ox, oy, alphax, alphay);
                shiftflg = 0;
                backflg = 1;
                while (Key_Scan() != 0xffff)
                    ;
            }
        }
        Gui_ClearBufferBit(pa, 0);
        Draw_Axis(pa, ox, oy, alphax, alphay);
        if (shiftflg)
            Gui_ShowString(pa, 96, 0, "shift", 1, 0);
        if (paraflg)
        {
            Gui_SetPosRel(pa, &pPara, 72, 48);
            Gui_CopyPage(&pPara, pa, 1);
        }
        for (j = 0; j < 3; j++)
        {
            if (!Status[j])
                continue;
            FastCal_Init(Expr[j], Lenth[j], 0);
            for (fi = 0; fi < 127; fi += dx)
            {
                rx = (fi - ox) * alphax;
                // parax[0]=rx;parax[1]=rx;parax[2]=rx;
                // parax[3]=rx;
                parax[4] = rx;
                parax[5] = rx;
                parax[6] = rx;
                resy = FastCal_Work(parax, 0);
                Cal_UpdateAns(resy);
                now_y = resy / alphay;
                if(Set_AutoContinue_Flg()){ //是否开启连续性补偿
                    if(fi > 0.01f){  //float比大小,让他至少第一个点不进行连续性检测
                        if(now_y >= prv_y) {for(step_y = prv_y; step_y <= now_y && oy - step_y >= 0 && oy - step_y < 64 ;step_y++) Gui_DrawPoint(pa, fi, oy - step_y, 1);}
                        if(now_y <  prv_y) {for(step_y = now_y; step_y <= prv_y && oy - step_y >= 0 && oy - step_y < 64 ;step_y++) Gui_DrawPoint(pa, fi, oy - step_y, 1);}
                    }
                }else{
                    Gui_DrawPoint(pa, fi, oy - now_y, 1);
                }
                prv_y = now_y;
            }
        }
        Gui_Flush(pa);
    }
    Gui_FreePage(&pPara);
}
void Fun_ParaEqu(Page *pa)
{
    Page pPara;
    uint8_t keynum = 0, kl = 0, kr = 0;               // 键码  左键码  右键码
    uint8_t shiftflg = 0, backflg = 0, curposflg = 0; // #shift标志  #back标志（用于放置连按不小心退出）  #光标坐标显示标志，用于指示当前坐标
    uint8_t paraflg = 0;                              // 参数标志 0:invalid   1:u 2:v 3:w
    uint8_t Exprx[MaxLenth], Expry[MaxLenth];         // x,y表达式
    uint8_t Lenthx, Lenthy;                           // x,y表达式长度
    uint8_t Statusx, Statusy;                         // 9组表达式状态   0:隐藏 1:绘制
    float parax[7];                                   // 参数表
    uint8_t op = 0;                                   // 选项op
    int i;                                            // 整数计数器变量
    float fi;                                         // 浮点计数器变量
    float alphax = 1, alphay = 1, ox = 64, oy = 32;   // 一个像素对应多少单位长度
    float resx, resy;                                 // 代入计算的横坐标真实值，结果纵坐标;
    float rx, ry;                                     // 实际在屏幕上的坐标(rx,ry)
    float Len_Table[] = {110, 110, 110};
    float pIter_Set = 0, lPara = -3.142, rPara = 3.142, dPara = 0.05; // 参数迭代器设置选项，参数迭代器左端点，参数迭代器右端点,参数迭代器步长
    float posx, posy, desx, desy, vx, vy;
    int curx = 64, cury = 32;    // 当前光标位
    uint8_t Expr_Max_Lenth = 32; // 默认32个字符
    if (Set_LongExpression_Flg())
        Expr_Max_Lenth = 84; // 如果使用长模式，就是96个字符
    else
        Expr_Max_Lenth = 32;
    for (i = 0; i < Expr_Max_Lenth; i++)
    {
        Exprx[i] = 0;
        Expry[i] = 0;
    }
    for (i = 0; i < 7; i++)
        parax[i] = 1;
    Lenthx = 0;
    Lenthy = 0;
    Statusx = 0;
    Statusy = 0;
    // 表达式组初始化
    Gui_CreatePage(&pPara, 56, 16);
    Gui_ClearBufferBit(&pPara, 0);
    // 光标小页面初始化
    while (Key_Scan() != 0xffff)
        ;
    while (1)
    {
        // 读取左右键码并仲裁
        keynum = Key();
        if (keynum == 0x45 && backflg == 0)
            break;
        if (keynum == 0x44)
        {
            oy -= 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x49)
        {
            oy += 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x4a)
        {
            ox += 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x48)
        {
            ox -= 5;
            shiftflg = 0;
            backflg = 0;
        }
        if (keynum == 0x41)
            shiftflg = !shiftflg;
        if (shiftflg == 0)
        {
            if (keynum == 0x31)
            {
                alphax *= 2;
                backflg = 0;
            }
            if (keynum == 0x32)
            {
                alphax /= 2;
                backflg = 0;
            }
            if (keynum == 0x34)
            {
                alphay *= 2;
                backflg = 0;
            }
            if (keynum == 0x35)
            {
                alphay /= 2;
                backflg = 0;
            }
        }
        else
        {
            if (keynum == 0x31 || keynum == 0x32)
            {
                while (Key_Scan() != 0xffff)
                    ;
                if (keynum == 0x31)
                {
                    Lenthx = Cal_Input(pa, Exprx, "Expression X:");
                    Statusx = 1;
                    shiftflg = 0;
                    backflg = 0;
                }
                else
                {
                    Lenthy = Cal_Input(pa, Expry, "Expression Y:");
                    Statusy = 1;
                    shiftflg = 0;
                    backflg = 0;
                }
                alphax = 1;
                alphay = 1;
                ox = 64;
                oy = 32;
            }
            if (keynum == 0x46)
            {
                curx = 80;
                cury = 48;
                Draw_Cur(pa, ox, oy, alphax, alphay);
                while (Key_Scan() != 0xffff)
                    ;
                shiftflg = 0;
                backflg = 1;
            }
            if (keynum == 0x4b)
            {
                pIter_Set = 1;
                posy = pIter_Set * 16;
                posx = Len_Table[(uint8_t)pIter_Set - 1] + 4;
                desy = pIter_Set * 16;
                desx = Len_Table[(uint8_t)pIter_Set - 1] + 4;
                while (Key_Scan() != 0xffff)
                    ;
                Gui_ClearBufferBit(pa, 0);
                Gui_Flush(pa);
                while (1)
                {
                    Gui_ShowString(pa, 0, 0, "pIterator Config", 1, 1);
                    keynum = Key_Scan() & 0x00ff;
                    if (keynum == 0x3d)
                        break;
                    if (keynum == 0x44 && pIter_Set > 1)
                        pIter_Set--;
                    if (keynum == 0x49 && pIter_Set < 3)
                        pIter_Set++;
                    if (keynum == 0x48)
                    {
                        if (pIter_Set == 1)
                            lPara -= 10 * dPara;
                        if (pIter_Set == 2)
                            rPara -= 10 * dPara;
                        if (pIter_Set == 3)
                            dPara -= 0.01;
                    }
                    if (keynum == 0x4a)
                    {
                        if (pIter_Set == 1)
                            lPara += 10 * dPara;
                        if (pIter_Set == 2)
                            rPara += 10 * dPara;
                        if (pIter_Set == 3)
                            dPara += 0.05;
                    }
                    if (dPara < 0.01)
                        dPara = 0.01;
                    if (lPara >= rPara)
                        lPara = rPara - 10 * dPara;
                    Gui_Printf(pa, 16, 16, 1, 1, "lPara: %.3f", lPara);
                    Gui_Printf(pa, 16, 32, 1, 1, "rPara: %.3f", rPara);
                    Gui_Printf(pa, 16, 48, 1, 1, "dPara: %.3f", dPara);
                    desy = pIter_Set * 16;
                    desx = Len_Table[(uint8_t)pIter_Set - 1] + 4;
                    vy = (desy - posy) * 0.7;
                    vx = (desx - posx) * 0.7;
                    posy += vy;
                    posx += vx;
                    Gui_DrawRectangleWired(pa, 12, posy, posx, 16, 1);
                    Gui_Flush(pa);
                    Gui_ClearBufferBit(pa, 0);
                }
                while (Key_Scan() != 0xffff)
                    ;
                shiftflg = 0;
            }
        }
        Gui_ClearBufferBit(pa, 0);
        Draw_Axis(pa, ox, oy, alphax, alphay);
        if (shiftflg)
            Gui_ShowString(pa, 96, 0, "shift", 1, 0);
        if (paraflg)
        {
            Gui_SetPosRel(pa, &pPara, 72, 48);
            Gui_CopyPage(&pPara, pa, 1);
        }
        FastCal_Init(Exprx, Lenthx, 0);
        FastCal_Init(Expry, Lenthy, 1);
        for (fi = lPara; fi < rPara; fi += dPara)
        {
            // parax[0]=rx;parax[1]=rx;parax[2]=rx;
            // parax[3]=rx;
            parax[1] = fi;
            parax[2] = fi;
            parax[3] = fi;
            resx = FastCal_Work(parax, 0) / alphax;
            resy = FastCal_Work(parax, 1) / alphay;
            rx = (resx + ox);
            ry = (oy - resy);
            Gui_DrawPoint(pa, rx, ry, 1);
        }

        Gui_Flush(pa);
    }
    Gui_FreePage(&pPara);
}