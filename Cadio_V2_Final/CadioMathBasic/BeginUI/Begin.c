#include "Begin.h"
//========================================================================
// 函数: Begin_Demo(Page *pa,uint8_t Plain)
// 描述: 在页面pa上绘制开始小动画或开机界面
// 参数: Plain:朴素模式，传入1则跳过动画
// 返回: Page *pa:目标页面
// 版本: V2.0, 2024-8-2
//========================================================================
void Begin_Demo(uint8_t Plain)
{
    Page Simplegui;          // 开机动画的页面
    uint8_t cnt = 20;        // 循环的次数
    float finalposy, dy, sy; // 最终平衡点的纵坐标 ,纵坐标单次增量（可以理解为速度）,当前纵坐标（可以理解为位移）
    Gui_CreatePage(&Simplegui, 128, 24);
    Gui_ClearBufferBit(&Simplegui, 0);
    Gui_SetPosAbs(&Simplegui, 4, 24);
    sy = 24;
    finalposy = 0;
    if (Plain)
        return;
    // 简单动画
    while (cnt--)
    {
        Gui_ClearBufferBit(&Simplegui, 0);
        // 分段函数模型
        if (finalposy < sy)
            dy = 100 / (finalposy - sy);
        else
            dy = (finalposy - sy) * 0.3;
        Gui_DrawBmp(&Simplegui, 0, sy, 120, 24, Cadio);
        sy += dy;
        // Gui_CopyPage(&Simplegui,pa,0);
        Gui_SetPosAbs(&Simplegui, 4, 24);
        Gui_Flush(&Simplegui);
    }
    Gui_FreePage(&Simplegui);
}
//========================================================================
// 函数: Begin_Select_Menu(Page *pa)
// 描述: 在页面pa上绘制功能选择页面
// 参数: Page *pa:目标页面
// 返回: 0:设置 1:函数 2:计算
// 版本: V2.0, 2024-8-2
//========================================================================
uint8_t Begin_Select_Menu(Page *pa)
{
    int sx, sy, dx, dy;
    int finalposx, finalposy;
    int sy1, sy2, dy1, dy2;
    int finalposy1, finalposy2;
    uint8_t option = 0, cnt = 5, keynum;
    finalposy = 48; // 英文的纵坐标平衡位置
    finalposx = 96; // 图片的横坐标平衡位置
    sx = 0;
    sy = 64;
    option = 2;
    while (1)
    {
        keynum = Key_Scan() & 0x00ff;
        if (keynum == 0x3d)
            break;
        if (keynum == 0x4a && option > 0)
        {
            option--;
            finalposx -= 48;
            sy = 64;
        }
        if (keynum == 0x48 && option < 2)
        {
            option++;
            finalposx += 48;
            sy = 64;
        }
        // if(keynum==0x3d) break;
        Gui_ClearBufferBit(pa, 0);
        Gui_DrawBmp(pa, sx - 48, 0, 32, 32, cal);
        Gui_DrawBmp(pa, sx, 0, 32, 32, fun);
        Gui_DrawBmp(pa, sx + 48, 0, 32, 32, set);
        dx = (finalposx - sx) * 0.5;
        dy = (finalposy - sy) * 0.4;
        if (option == 0)
            Gui_ShowString(pa, 32, sy, "Settings", 1, 1);
        if (option == 1)
            Gui_ShowString(pa, 32, sy, "Function", 1, 1);
        if (option == 2)
            Gui_ShowString(pa, 32, sy, "Calculate", 1, 1);
        Gui_DrawTriangleFilled(pa, 48, 48, 80, 48, 64, 36, 1);
        sx += dx;
        sy += dy;
        Gui_Flush(pa);
    }
    finalposy1 = 6;
    finalposy2 = 42;
    sy1 = 0;
    sy2 = 48;
    while (cnt--)
    {
        Gui_ClearBufferBit(pa, 0);
        if (option == 0)
        {
            Gui_DrawBmp(pa, 48, sy1, 32, 32, set);
            Gui_ShowString(pa, 32, sy2, "Settings", 1, 1);
        }
        if (option == 1)
        {
            Gui_DrawBmp(pa, 48, sy1, 32, 32, fun);
            Gui_ShowString(pa, 32, sy2, "Function", 1, 1);
        }
        if (option == 2)
        {
            Gui_DrawBmp(pa, 48, sy1, 32, 32, cal);
            Gui_ShowString(pa, 32, sy2, "Calculate", 1, 1);
        }
        dy1 = (finalposy1 - sy1) * 0.5;
        dy2 = (finalposy2 - sy2) * 0.4;
        sy1 += dy1;
        sy2 += dy2;
        Gui_Flush(pa);
    }
    Gui_ClearBufferBit(pa, 0);
    Gui_Flush(pa);
    return option + 1;
}