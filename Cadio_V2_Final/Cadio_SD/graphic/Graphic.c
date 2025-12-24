#include "Graphic.h"
#include <stdarg.h>
#include <stdio.h>
uchar Graphic_Font = 0;
uchar xdata Graphic_FrameBuffer[1024];
void Graphic_SetFont(uchar f)
{
    Graphic_Font = f;
}
void Graphic_Swap_int16(int *a, int *b)
{
    int tmp = 0;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
void Graphic_DrawPoint(char x, char y, uchar op)
{
    if (x >= MaxWidth || x < 0 || y >= MaxHeight || y < 0)
        return;
    if (op)
        Graphic_FrameBuffer[(y / 8) * 128 + x] |= (0x01 << (uchar)(y % 8));
    else
        Graphic_FrameBuffer[(y / 8) * 128 + x] &= (~(0x01 << (uchar)(y % 8)));
    return;
}
uint8_t Graphic_GetPoint(char x, char y)
{
    if (x >= MaxWidth || x < 0 || y >= MaxHeight || y < 0)
        return 0;
    if (Graphic_FrameBuffer[(y / 8) * 128 + x] & (0x01 << (uchar)(y % 8)))
        return 1;
    else
        return 0;
}
void Graphic_DrawLine(char x_1, char y_1, char x_2, char y_2, uchar op)
{
    double k, i, step = 0;
    ;
    if (x_1 == x_2 && y_1 != y_2)
    {
        if (y_1 > y_2)
            Graphic_Swap_int16(&y_1, &y_2);
        for (i = y_1; i <= y_2; i++)
            Graphic_DrawPoint(x_1, i, op);
    }
    if (x_1 != x_2 && y_1 == y_2)
    {
        if (x_1 > x_2)
            Graphic_Swap_int16(&x_1, &x_2);
        for (i = x_1; i <= x_2; i++)
            Graphic_DrawPoint(i, y_1, op);
    }
    if (x_1 != x_2 && y_1 != y_2)
    {
        k = (float)(y_1 - y_2) / (float)(x_1 - x_2);
        if (abs(k) <= 1)
        {
            if (x_1 > x_2)
                Graphic_Swap_int16(&x_1, &x_2);
            for (i = x_1; i <= x_2; i++)
                Graphic_DrawPoint(i, y_1 + k * (float)(i - x_1), op);
        }
        else
        {
            if (y_1 > y_2)
                Graphic_Swap_int16(&x_1, &x_2);
            for (i = y_1; i <= y_2; i++)
                Graphic_DrawPoint(x_1 + (float)(i - y_1) / k, i, op);
        }
    }
}
void Graphic_DrawCircleWired(char x, char y, uchar r, uchar op)
{
    int xx, yy;
    int i, j;
    float dis, tmp = 1;
    xx = x, yy = y;
    tmp *= r;
    tmp *= r;
    for (i = x - r; i <= x + r; i++)
    {
        for (j = y - r; j < y + r; j++)
        {
            dis = sqrt((i - xx) * (i - xx) + (j - yy) * (j - yy));
            if (fabs(dis - tmp) < 1)
                Graphic_DrawPoint(i, j, op);
        }
    }
    return;
}
void Graphic_DrawCircleFilled(char x, char y, uchar r, uchar op)
{
    int xx, yy;
    int i, j, dis2, tmp = 1;
    xx = x, yy = y;
    tmp *= r;
    tmp *= r;
    for (i = x - r; i <= x + r; i++)
    {
        for (j = y - r; j < y + r; j++)
        {
            dis2 = (i - xx) * (i - xx) + (j - yy) * (j - yy);
            if (dis2 < tmp)
                Graphic_DrawPoint(i, j, op);
        }
    }
    return;
}
void Graphic_ShowChar(char sx, char sy, uchar c, uchar op)
{
    int x, y, nowx, nowy;
    if (Graphic_Font == 1)
    {
        for (y = 0; y < 16; y++)
        {
            for (x = 0; x < 8; x++)
            {
                nowx = sx + x;
                nowy = sy + y;
                if (Asc1608[c - 32][(uchar)((y) / 8) * 8 + x] & (0x01 << (uchar)((y) % 8)))
                {
                    Graphic_DrawPoint(nowx, nowy, op);
                }
                else
                {
                    Graphic_DrawPoint(nowx, nowy, !op);
                }
            }
        }
    }
    else
    {
        for (y = 0; y < 8; y++)
        {
            for (x = 0; x < 6; x++)
            {
                nowx = sx + x;
                nowy = sy + y;
                if (Asc0806[c - 32][(uchar)((y) / 8) * 6 + x] & (0x01 << (uchar)((y) % 8)))
                {
                    Graphic_DrawPoint(nowx, nowy, op);
                }
                else
                {
                    Graphic_DrawPoint(nowx, nowy, !op);
                }
            }
        }
    }
}
void Graphic_ShowString(int sx, int sy, uchar buffer[], uchar op)
{
    uchar i, dx;
    if (Graphic_Font == 1)
        dx = 8;
    else if (Graphic_Font == 0)
        dx = 6;
    else
        return;
    for (i = 0; buffer[i] != '\0'; i++)
        Graphic_ShowChar(sx + i * dx, sy, buffer[i], op);
    return;
}

void Graphic_Printf(int sx, int sy, uchar op, const char *str, ...)
{
    uint8_t buffer[64];
    va_list args;
    va_start(args, str);
    vsprintf(buffer, str, args);
    va_end(args);
    Graphic_ShowString(sx, sy, buffer, op);
}

void Graphic_ShowNum(char sx, char sy, ulong num, uchar lenth, uchar mode, uchar op)
{
    uchar i, flg = 0, tmp = 0, buffer[15], nowx, dx;
    if (Graphic_Font == 1)
        dx = 8;
    else if (Graphic_Font == 0)
        dx = 6;
    else
        return;
    nowx = sx;
    for (i = 0; i < lenth; i++)
    {
        buffer[i] = num % 10;
        num /= 10;
    }
    for (i = lenth; i > 0; i--)
    {
        tmp = buffer[i - 1];
        if (mode == 0)
        {
            Graphic_ShowChar(sx + i * dx, sy, tmp + 48, op);
            nowx += dx;
        }
        else
        {
            if (tmp != 0 && flg == 0)
                flg = 1;
            if (flg == 0 && tmp == 0)
                continue;
            if (flg == 1)
            {
                Graphic_ShowChar(sx + i * dx, sy, tmp + 48, op);
                nowx += dx;
            }
        }
    }
    return;
}

void Graphic_DrawBmp(uchar sx, uchar sy, uchar sizex, uchar sizey, uchar BMP[])
{
    uint x, y, nowx, nowy;
    uint pixel;
    for (y = 0; y < sizey; y++)
    {
        for (x = 0; x < sizex; x++)
        {
            nowy = sy + y;
            nowx = sx + x;
            if (1)
            {
                if (BMP[(uchar)(y / 8) * sizex + x] & (0x01 << (uchar)(y % 8)))
                    pixel = 0x00;
                else
                    pixel = 0xff;
                Graphic_DrawPoint(nowx, nowy, pixel);
            }
        }
    }
    return;
}
void Graphic_ClearBufferBit(uchar op)
{
    uint i;
    for (i = 0; i < 1024; i++)
        Graphic_FrameBuffer[i] = op ? 0xff : 0;
    return;
}
void Graphic_Flush()
{
    OLED_DrawBMP(0, 0, 128, 64, Graphic_FrameBuffer);
}