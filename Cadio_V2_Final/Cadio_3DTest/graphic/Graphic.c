#include "Graphic.h"
void Graphic_Swap_int16(int *a, int *b)
{
    int tmp = 0;
    tmp = *a;
    *a = *b;
    *b = tmp;
}
uchar *Graphic_GetBuffer()
{
    return Graphic_FrameBuffer;
}
void Graphic_DrawPoint(int x, int y, uchar op)
{
    if (x >= MaxWidth || x < 0 || y >= MaxHeight || y < 0)
        return;
    if (op)
        Graphic_FrameBuffer[(y / 8) * 128 + x] |= (0x01 << (uint)(y % 8));
    else
        Graphic_FrameBuffer[(y / 8) * 128 + x] &= (~(0x01 << (uint)(y % 8)));
    return;
}
void Graphic_DrawLine(int x_1, int y_1, int x_2, int y_2, uchar op)
{
    double k, i, step = 0;
    ;
    if (x_1 == x_2 && y_1 != y_2)
    {
        if (y_1 > y_2)
            Graphic_Swap_int16(&y_1, &y_2);
        for (i = y_1; i <= y_2; i++)
            Graphic_DrawPoint(x_1, i, op);
        return;
    }
    if (x_1 != x_2 && y_1 == y_2)
    {
        if (x_1 > x_2)
            Graphic_Swap_int16(&x_1, &x_2);
        for (i = x_1; i <= x_2; i++)
            Graphic_DrawPoint(i, y_1, op);
        return;
    }
    if (x_1 != x_2 && y_1 != y_2)
    {
        k = (float)(y_1 - y_2) / (float)(x_1 - x_2);
        if (abs(k) <= 1)
        {
            if (x_1 > x_2)
            {
                Graphic_Swap_int16(&x_1, &x_2);
                Graphic_Swap_int16(&y_1, &y_2);
            }
            for (i = x_1; i <= x_2; i++)
                Graphic_DrawPoint(i, y_1 + k * (float)(i - x_1), op);
        }
        else
        {
            if (y_1 > y_2)
            {
                Graphic_Swap_int16(&x_1, &x_2);
                Graphic_Swap_int16(&y_1, &y_2);
            }
            for (i = y_1; i <= y_2; i++)
                Graphic_DrawPoint(x_1 + (float)(i - y_1) / k, i, op);
        }
        return;
    }
}
void Graphic_DrawCircleWired(int x, int y, uint r, uchar op)
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
void Graphic_DrawCircleFilled(int x, int y, uint r, uchar op)
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