#include "cal.h"
#include "set.h"

static Mem Cal_Memory[MemSize];                    // 记忆数组
static uint8_t Cal_CntM = 0;                       // 记忆条数
static uint8_t Quit = 0;                           // 退出标志
static float Para_Init[7] = {0, 0, 0, 0, 0, 0, 0}; // 参数，目前用不到就先全设置为0



//========================================================================
// 函数: Cal_ShowString(Page *p, int sx, int sy, uchar buffer[], uchar op, uchar font， uchar err_pos)
// 描述: 显示字符串函数，把缓冲区的字符串打印到屏幕上，自动换行
// 参数: 与Gui.h里的Gui_ShowString相似，把原来指定颜色用的op改为了err_pos, 用于在错误的字符位置给予反色处理
// 返回: 无
// 版本: V2.0, 2025-11-28
//========================================================================
void Cal_ShowString(Page *p, int sx, int sy, uchar buffer[], uchar font,uchar err_pos)
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
            Gui_ShowChar(p, nowx, nowy, buffer[i], (i == err_pos ? 0:1), font);
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
void Cal_delay(uint16_t t)
{
    uint16_t i;
    while (t--)
        for (i = 0; i < 600; i++)
            ;
}
//========================================================================
// 函数: Cal_MoveR(uint8_t *str,uint8_t len,uint8_t m,uint8_t n)
// 描述: //把字符串第m及其之后(m>=0)的元素往右边挪n位（n>=0）
// 参数: Str:目标字符串  len:长度   m:第几个（从0数起）     n:挪几位（从0数起）
// 返回: 无
// 版本: V2.0, 2024-8-2
//========================================================================
void Cal_MoveR(uint8_t *str, uint8_t len, uint8_t m, uint8_t n)
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
// 函数: Cal_MoveL(uint8_t *str,uint8_t len,uint8_t m,uint8_t n)
// 描述: //把字符串第m及其之后(m>=0)的元素往左边边挪n位（n>=0）
// 参数: Str:目标字符串  len:长度   m:第几个（从0数起）     n:挪几位（从0数起）
// 返回: 无
// 版本: V2.0, 2024-8-2
//========================================================================
void Cal_MoveL(uint8_t *str, uint8_t len, uint8_t m, uint8_t n)
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

void Cal_MemInsert(Mem mx)
{
    uint8_t i;
    if (Cal_CntM < MemSize)
    {
        Cal_Memory[Cal_CntM++] = mx;
        return;
    } // 如果没有满，就在后面插入
    for (i = 1; i < MemSize; i++)
        Cal_Memory[i - 1] = Cal_Memory[i]; // 如果满了，丢弃第一条记忆，整体相前挪一位腾出末位
    Cal_Memory[MemSize - 1] = mx;          // 新元素在末位插入
}
//========================================================================
// 函数: Input(Page *pa,uint8_t *Str)
// 描述: 绘制输入框，接受输入字符串，并对其进行编辑
// 参数: *pa:绘制的页面   TBuffer:接收到的字符串 *Title:标题字符串
// 返回: 无意义
// 版本: V2.0, 2024-8-2
//========================================================================
uint8_t Cal_Input(Page *pa, uint8_t *TBuffer, uint8_t *Title)
{
    char pos = 0, mempos = Cal_CntM - 1; // 光标位置（0~31），记忆位置（0~Cal_CntM-1）
    uint8_t i;                           // 计数器变量
    uint8_t tick = 0;
    uint8_t curcnt = 0, curx = 0, cury = 0;       // 光标闪烁计数器，光标坐标(x,y)  注意：这里的坐标是指，在2*16字符选区内的坐标。  x取0或1，y取0~15
    uint8_t curflg = 0, alpha_flg = 0, beta_flg = 0; // 光标显示标志，alpha键标志,beta键标志
    uint8_t Lenth = 0;                            // 缓冲区内表示式长度
    uint8_t kl, kr, keynum;                       // 左半边键码,右半边键码，最终键码。    仲裁逻辑：当左右两边都有按键操作时，优先取右边的键码值.
    uint8_t TitleLenth;                           // 标题长度 0代表无标题
    uint8_t Input_Max_Lenth = 32;                 // 默认32个字符
    uint8_t Error_Pos = 0xff;                     // 错误出现的地方
    if (Set_LongExpression_Flg())
        Input_Max_Lenth = 84; // 如果使用长模式，就是96个字符
    else
        Input_Max_Lenth = 32;
    Gui_ClearBufferBit(pa, 0);
    Gui_Flush(pa);
    Lenth = strlen(TBuffer); // 获取字符串长度
    TitleLenth = strlen(Title);
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
        // 右半边按键处理
        /*-----------Right--------*/
        if (keynum - 0x30 + '0' >= '0' && keynum - 0x30 + '0' <= '9')
        {
            if (beta_flg)
            { // 双曲函数beta标志为真：则按下1~6分别对应A~F
                if(keynum - 0x30 + '0' >= '1' && keynum - 0x30 + '0' <= '6'){
                    Lenth += 1;
                    Cal_MoveR(TBuffer, Lenth, pos, 1);
                    TBuffer[pos] = 'A' + keynum - 0x31;
                    pos += 1;
                }
                if(keynum - 0x30 + '0' == '7'){
                    Lenth += 6;
                    Cal_MoveR(TBuffer, Lenth, pos, 6);
                    TBuffer[pos] = 's';
                    TBuffer[pos + 1] = 'i';
                    TBuffer[pos + 2] = 'n';
                    TBuffer[pos + 3] = 'h';
                    TBuffer[pos + 4] = '(';
                    TBuffer[pos + 5] = ')';
                    pos += 5;
 
                }
                if (keynum - 0x30 + '0' == '8')
                {
                    Lenth += 6;
                    Cal_MoveR(TBuffer, Lenth, pos, 6);
                    TBuffer[pos] = 'c';
                    TBuffer[pos + 1] = 'o';
                    TBuffer[pos + 2] = 's';
                    TBuffer[pos + 3] = 'h';
                    TBuffer[pos + 4] = '(';
                    TBuffer[pos + 5] = ')';
                    pos += 5;
                }
                if (keynum - 0x30 + '0' == '9')
                {
                    Lenth += 6;
                    Cal_MoveR(TBuffer, Lenth, pos, 6);
                    TBuffer[pos] = 't';
                    TBuffer[pos + 1] = 'a';
                    TBuffer[pos + 2] = 'n';
                    TBuffer[pos + 3] = 'h';
                    TBuffer[pos + 4] = '(';
                    TBuffer[pos + 5] = ')';
                    pos += 5;
                }
                alpha_flg = 0;
                beta_flg = 0;
            }
            else if (alpha_flg)
            { // alpha标志为真，则按下1 2 3 4 5 6分别对应 x y z u v w
                if (keynum - 0x30 + '0' >= '1' && keynum - 0x30 + '0' <= '6')
                {
                    Lenth += 1;
                    Cal_MoveR(TBuffer, Lenth, pos, 1);
                    if (keynum - 0x30 + '0' == '1')
                        TBuffer[pos] = 'x';
                    if (keynum - 0x30 + '0' == '2')
                        TBuffer[pos] = 'y';
                    if (keynum - 0x30 + '0' == '3')
                        TBuffer[pos] = 'z';
                    if (keynum - 0x30 + '0' == '4')
                        TBuffer[pos] = 'u';
                    if (keynum - 0x30 + '0' == '5')
                        TBuffer[pos] = 'v';
                    if (keynum - 0x30 + '0' == '6')
                        TBuffer[pos] = 'w';
                    pos += 1;
                    alpha_flg = 0;
                    beta_flg = 0;
                }
                else if (keynum - 0x30 + '0' == '7')
                { // alpha标志为真，则按下7对应rand()
                    Lenth += 6;
                    Cal_MoveR(TBuffer, Lenth, pos, 6);
                    TBuffer[pos] = 'r';
                    TBuffer[pos + 1] = 'a';
                    TBuffer[pos + 2] = 'n';
                    TBuffer[pos + 3] = 'd';
                    TBuffer[pos + 4] = '(';
                    TBuffer[pos + 5] = ')';
                    pos += 5;
                    alpha_flg = 0;
                    beta_flg = 0;
                }
                else if (keynum - 0x30 + '0' == '8')
                { // alpha标志为真，则按下8对应 random(,)
                    Lenth += 9;
                    Cal_MoveR(TBuffer, Lenth, pos, 9);
                    TBuffer[pos] = 'r';
                    TBuffer[pos + 1] = 'a';
                    TBuffer[pos + 2] = 'n';
                    TBuffer[pos + 3] = 'd';
                    TBuffer[pos + 4] = 'o';
                    TBuffer[pos + 5] = 'm';
                    TBuffer[pos + 6] = '(';
                    TBuffer[pos + 7] = ',';
                    TBuffer[pos + 8] = ')';
                    pos += 7;
                    alpha_flg = 0;
                    beta_flg = 0;
                }
                else if (keynum - 0x30 + '0' == '9')
                { // alpha标志为真，则按下8对应 random(,)
                    Lenth += 1;
                    Cal_MoveR(TBuffer, Lenth, pos, 1);
                    TBuffer[pos] = ',';
                    pos += 1;
                    alpha_flg = 0;
                    beta_flg = 0;
                }
            }
            else
            { // 没有任何标志的情况下,按键0~9就对应数字0~9
                Lenth++;
                Cal_MoveR(TBuffer, Lenth, pos, 1);
                TBuffer[pos] = keynum - 0x30 + '0';
                pos++;
                alpha_flg = 0;
                beta_flg = 0;
            }
        }
        if (keynum == 0x6e)
        { // 小数点
            Lenth++;
            Cal_MoveR(TBuffer, Lenth, pos, 1);
            TBuffer[pos] = '.';
            pos++;
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x6f)
        { // 除
            Lenth++;
            Cal_MoveR(TBuffer, Lenth, pos, 1);
            TBuffer[pos] = '/';
            pos++;
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x6a)
        { // 乘
            Lenth++;
            Cal_MoveR(TBuffer, Lenth, pos, 1);
            TBuffer[pos] = '*';
            pos++;
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x6d)
        { // 减
            Lenth++;
            Cal_MoveR(TBuffer, Lenth, pos, 1);
            TBuffer[pos] = '-';
            pos++;
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x6b)
        { // 加
            Lenth++;
            Cal_MoveR(TBuffer, Lenth, pos, 1);
            TBuffer[pos] = '+';
            pos++;
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x47)
        { // 右括号
            Lenth++;
            Cal_MoveR(TBuffer, Lenth, pos, 1);
            TBuffer[pos] = ')';
            pos++;
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x46)
        { // 左括号
            Lenth++;
            Cal_MoveR(TBuffer, Lenth, pos, 1);
            TBuffer[pos] = '(';
            pos++;
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x4b)
        { // 绝对值abs
            Lenth += 5;
            Cal_MoveR(TBuffer, Lenth, pos, 5);
            TBuffer[pos] = 'a';
            TBuffer[pos + 1] = 'b';
            TBuffer[pos + 2] = 's';
            TBuffer[pos + 3] = '(';
            TBuffer[pos + 4] = ')';
            pos += 4;
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x55)
        { // 删除
            if (pos > 0 && Lenth > 0)
            {
                Cal_MoveL(TBuffer, Lenth, pos - 1, 1);
                Lenth--;
                pos--;
                alpha_flg = 0;
                beta_flg = 0;
            }
        }

        // 左边按键处理
        /*-----------Left--------*/
        if (keynum == 0x41)
        { // alpha键
            if (!alpha_flg)
                alpha_flg = 0xff;
            else
                alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x58)
        {
            if (!beta_flg)
                beta_flg = 0xff;
            else
                beta_flg = 0;
            alpha_flg = 0;
        }
        if (keynum == 0x51)
        { // 度数d

            Lenth += 1;
            Cal_MoveR(TBuffer, Lenth, pos, 1);
            TBuffer[pos] = 'd';
            pos += 1;
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x43)
        { // π或者e
            if (alpha_flg)
            {
                Lenth += 2;
                Cal_MoveR(TBuffer, Lenth, pos, 2);
                TBuffer[pos] = 'p';
                TBuffer[pos + 1] = 'i';
                pos += 2;
            }
            else
            {
                Lenth += 1;
                Cal_MoveR(TBuffer, Lenth, pos, 1);
                TBuffer[pos] = 'e';
                pos += 1;
            }
            alpha_flg = 0;
            beta_flg = 0;
        }
        // 下面的数学表达式输入就不再赘述，方法实现同上，数学函数名看代码就能知道
        if (keynum == 0x4c)
        {
            if (alpha_flg)
            {
                Lenth += 6;
                Cal_MoveR(TBuffer, Lenth, pos, 6);
                TBuffer[pos] = '^';
                TBuffer[pos + 1] = '(';
                TBuffer[pos + 2] = '1';
                TBuffer[pos + 3] = '/';
                TBuffer[pos + 4] = '2';
                TBuffer[pos + 5] = ')';
                pos += 6;
            }
            else
            {
                Lenth += 2;
                Cal_MoveR(TBuffer, Lenth, pos, 2);
                TBuffer[pos] = '^';
                TBuffer[pos + 1] = '2';
                pos += 2;
            }
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x4d)
        {
            if (alpha_flg)
            {
                Lenth += 5;
                Cal_MoveR(TBuffer, Lenth, pos, 5);
                TBuffer[pos] = '^';
                TBuffer[pos + 1] = '(';
                TBuffer[pos + 2] = '1';
                TBuffer[pos + 3] = '/';
                TBuffer[pos + 4] = ')';
                pos += 4;
            }
            else
            {
                Lenth += 3;
                Cal_MoveR(TBuffer, Lenth, pos, 3);
                TBuffer[pos] = '^';
                TBuffer[pos + 1] = '(';
                TBuffer[pos + 2] = ')';
                pos += 2;
            }
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x4e)
        {
            if (alpha_flg)
            {
                Lenth += 6;
                Cal_MoveR(TBuffer, Lenth, pos, 6);
                TBuffer[pos] = 'f';
                TBuffer[pos + 1] = 'a';
                TBuffer[pos + 2] = 'c';
                TBuffer[pos + 3] = 't';
                TBuffer[pos + 4] = '(';
                TBuffer[pos + 5] = ')';
                pos += 5;
            }
            else
            {
                Lenth += 5;
                Cal_MoveR(TBuffer, Lenth, pos, 5);
                TBuffer[pos] = '^';
                TBuffer[pos + 1] = '(';
                TBuffer[pos + 2] = '-';
                TBuffer[pos + 3] = '1';
                TBuffer[pos + 4] = ')';
                pos += 5;
            }
            alpha_flg = 0;
            beta_flg = 0;
        }

        if (keynum == 0x52)
        {
            if (alpha_flg)
            {
                Lenth += 6;
                Cal_MoveR(TBuffer, Lenth, pos, 6);
                TBuffer[pos] = 'a';
                TBuffer[pos + 1] = 's';
                TBuffer[pos + 2] = 'i';
                TBuffer[pos + 3] = 'n';
                TBuffer[pos + 4] = '(';
                TBuffer[pos + 5] = ')';
                pos += 5;
            }
            else
            {
                Lenth += 5;
                Cal_MoveR(TBuffer, Lenth, pos, 5);
                TBuffer[pos] = 's';
                TBuffer[pos + 1] = 'i';
                TBuffer[pos + 2] = 'n';
                TBuffer[pos + 3] = '(';
                TBuffer[pos + 4] = ')';
                pos += 4;
            }
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x53)
        {
            if (alpha_flg)
            {
                Lenth += 6;
                Cal_MoveR(TBuffer, Lenth, pos, 6);
                TBuffer[pos] = 'a';
                TBuffer[pos + 1] = 'c';
                TBuffer[pos + 2] = 'o';
                TBuffer[pos + 3] = 's';
                TBuffer[pos + 4] = '(';
                TBuffer[pos + 5] = ')';
                pos += 5;
            }
            else
            {
                Lenth += 5;
                Cal_MoveR(TBuffer, Lenth, pos, 5);
                TBuffer[pos] = 'c';
                TBuffer[pos + 1] = 'o';
                TBuffer[pos + 2] = 's';
                TBuffer[pos + 3] = '(';
                TBuffer[pos + 4] = ')';
                pos += 4;
            }
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x54)
        {
            if (alpha_flg)
            {
                Lenth += 6;
                Cal_MoveR(TBuffer, Lenth, pos, 6);
                TBuffer[pos] = 'a';
                TBuffer[pos + 1] = 't';
                TBuffer[pos + 2] = 'a';
                TBuffer[pos + 3] = 'n';
                TBuffer[pos + 4] = '(';
                TBuffer[pos + 5] = ')';
                pos += 5;
            }
            else
            {
                Lenth += 5;
                Cal_MoveR(TBuffer, Lenth, pos, 5);
                TBuffer[pos] = 't';
                TBuffer[pos + 1] = 'a';
                TBuffer[pos + 2] = 'n';
                TBuffer[pos + 3] = '(';
                TBuffer[pos + 4] = ')';
                pos += 4;
            }
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x4f)
        {
            if (alpha_flg)
            {
                Lenth += 4;
                Cal_MoveR(TBuffer, Lenth, pos, 4);
                TBuffer[pos] = 'l';
                TBuffer[pos + 1] = 'n';
                TBuffer[pos + 2] = '(';
                TBuffer[pos + 3] = ')';
                pos += 3;
            }
            else
            {
                Lenth += 2;
                Cal_MoveR(TBuffer, Lenth, pos, 2);
                TBuffer[pos] = 'e';
                TBuffer[pos + 1] = '^';
                pos += 2;
            }
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x50)
        {
            if (alpha_flg)
            {
                Lenth += 4;
                Cal_MoveR(TBuffer, Lenth, pos, 5);
                TBuffer[pos] = 'l';
                TBuffer[pos + 1] = 'g';
                TBuffer[pos + 2] = '(';
                TBuffer[pos + 3] = ')';
                pos += 3;
            }
            else
            {
                Lenth += 3;
                Cal_MoveR(TBuffer, Lenth, pos, 3);
                TBuffer[pos] = '1';
                TBuffer[pos + 1] = '0';
                TBuffer[pos + 2] = '^';
                pos += 3;
            }
            alpha_flg = 0;
            beta_flg = 0;
        }

        if (keynum == 0x57)
        {
            Lenth += 3;
            Cal_MoveR(TBuffer, Lenth, pos, 3);
            TBuffer[pos] = 'A';
            TBuffer[pos + 1] = 'n';
            TBuffer[pos + 2] = 's';
            pos += 3;
            alpha_flg = 0;
            beta_flg = 0;
        }
        if (keynum == 0x45)
        {
            Quit = 1;
            return 0;
        } // 退出
        if (keynum == 0x56)
        {
            Lenth = 0;
            pos = 0;
        }
        /*------------------------------------*/

        if (keynum == 0x48)
            pos--; //
        if (keynum == 0x4a)
            pos++;
        if (Lenth >= Input_Max_Lenth)
            Lenth = Input_Max_Lenth - 1;
        if (pos > Lenth)
            pos = 0;
        if (pos < 0)
            pos = Lenth;
        if (keynum == 0x44)
        {
            if (Cal_CntM == 0)
            { // 没有记忆就显示空白
                Lenth = 0;
                pos = 0;
                continue;
            }
            else
            { // 有记忆，就显示出来
                if (mempos > 0)
                    mempos--;
                Lenth = Cal_Memory[mempos].ExprLenth;
                pos = 0;
                for (i = 0; i < Lenth; i++)
                    TBuffer[i] = Cal_Memory[mempos].Expr[i];
            }
        }
        if (keynum == 0x49)
        { // 没有记忆就显示空白
            if (Cal_CntM == 0)
            {
                Lenth = 0;
                pos = 0;
                continue;
            }
            else
            { // 有记忆，就显示出来
                if (mempos + 1 < Cal_CntM)
                    mempos++;
                Lenth = Cal_Memory[mempos].ExprLenth;
                pos = 0;
                for (i = 0; i < Lenth; i++)
                    TBuffer[i] = Cal_Memory[mempos].Expr[i];
            }
        }
        for (i = Lenth; i < MaxLenth; i++)
            TBuffer[i] = '\0'; // 清空有效长度之后的缓冲内容
        if (Set_LongExpression_Flg())
        { // 启动长表达式(84个字符)
            curx = pos % 21;
            cury = pos / 21;
            
            Cal_ShowString(pa, 1, 1, TBuffer, 0, Error_Pos); // 显示缓冲区内容
            
            if (curflg)
                Gui_DrawLine(pa, curx * 6 + 1, cury * 8 + 1, curx * 6 + 1, cury * 8 + 1 + 8, 1);
            else
                Gui_DrawLine(pa, curx * 6 + 1, cury * 8 + 1, curx * 6 + 1, cury * 8 + 1 + 8, 0);
        }
        else
        { // 不启动
            curx = pos % 15;
            cury = pos / 15;

            Cal_ShowString(pa, 1, 1, TBuffer, 1, Error_Pos); // 显示缓冲区内容
            
            if (curflg)
                Gui_DrawLine(pa, curx * 8 + 1, cury * 16 + 1, curx * 8 + 1, cury * 16 + 1 + 16, 1);
            else
                Gui_DrawLine(pa, curx * 8 + 1, cury * 16 + 1, curx * 8 + 1, cury * 16 + 1 + 16, 0);
        }
        Gui_DrawRectangleWired(pa, 0, 0, 127, 32, 1); // 画一个输入框框
        if (Set_ErrorDetection_Flg())
        { // 启动自动查错模式
            if (alpha_flg)
                Gui_ShowString(pa, 56, 33, "alpha", 1, 0);
            else
                Gui_ShowString(pa, 56, 33, "     ", 1, 0);
            if (beta_flg)
                Gui_ShowString(pa, 104, 33, "beta", 1, 0);
            else
                Gui_ShowString(pa, 104, 33, "     ", 1, 0);
            if (TitleLenth)
                Gui_ShowString_OneLine(pa, (128 - TitleLenth * 8) / 2, 48, Title, 1, 1);
            Calculate_Expression(TBuffer, Lenth, Para_Init);
            if (Get_ErrorCode()){
                Gui_Printf(pa, 0, 33, 1, 0, "Error %bd", Get_ErrorCode());
                Error_Pos = Get_ErrorPos();
            }else{
                Error_Pos = 0xff;
                Gui_Printf(pa, 0, 33, 1, 0, "Input OK");
            }
        }
        else
        { // 不启动
            if (alpha_flg)
                Gui_ShowString(pa, 24, 33, "alpha", 1, 0);
            else
                Gui_ShowString(pa, 24, 33, "     ", 1, 0);
            if (beta_flg)
                Gui_ShowString(pa, 80, 33, "beta", 1, 0);
            else
                Gui_ShowString(pa, 80, 33, "     ", 1, 0);
            if (TitleLenth)
                Gui_ShowString_OneLine(pa, (128 - TitleLenth * 8) / 2, 48, Title, 1, 1);
            Calculate_Expression(TBuffer, Lenth, Para_Init);
        }
        Gui_Flush(pa);             // 刷屏
        Gui_ClearBufferBit(pa, 0); // 清屏
        Cal_delay(500);
        if (curcnt > 3)
        {
            curflg = !curflg;
            curcnt = 0;
        } // 循环4次闪一下
        curcnt++;
    }
    return Lenth; // 传回表达式长度
}
//========================================================================
// 函数: Cal_Work(Page *pa)
// 描述: 计算并显示结果
// 参数: *pa:绘制的页面
// 返回: 无意义
// 版本: V2.0, 2024-8-2
//========================================================================
uint8_t Cal_Work(Page *pa)
{
    Page pInput, pOutput;              // 承载输入区的页面，承载输出区的页面
    Mem Mnow;                          // 当前记忆条目
    uint8_t i;                         // 计数器变量
    float res;                         // 结果
    uint8_t expression[MaxLenth], Len; // 待计算的表达式，及其长度
    int sy;                            // 结果页面的纵坐标
    Gui_CreatePage(&pInput, 128, 48);
    Gui_ClearBufferBit(&pInput, 0);
    Gui_SetPosRel(pa, &pInput, 0, 0);
    Gui_CreatePage(&pOutput, 128, 16);
    Gui_ClearBufferBit(&pOutput, 0);
    Gui_SetPosRel(pa, &pOutput, 0, 48);
    while (Key_Scan() != 0xffff)
        ;
    while (1)
    {
        for (i = 0; i < MaxLenth; i++)
            expression[i] = 0; // 清空
        Gui_ClearBufferBit(&pInput, 0);
        Gui_ClearBufferBit(&pOutput, 0);
        Len = Cal_Input(&pInput, expression, ""); // 获取输入的字符串
        if (Quit)
        {
            Quit = 0;
            break;
        } // 退出计算模式
        Mnow.ExprLenth = Len;
        for (i = 0; i < Len; i++)
            Mnow.Expr[i] = expression[i];
        Cal_MemInsert(Mnow);                                    // 装入记忆库
        res = Calculate_Expression(expression, Len, Para_Init); // 计算
        if (Get_ErrorCode())
        {
            Gui_Printf(&pOutput, 0, 0, 1, 1, "Error: %bd", Get_ErrorCode());
        }
        else
        {
            Gui_Printf(&pOutput, 0, 0, 1, 1, "%g", res);
            Cal_UpdateAns(res);
        }
        sy = 64;
        // 简单动画，向上跳出的输出数字
        while (sy >= 48)
        {
            Gui_SetPosRel(pa, &pOutput, 0, sy);
            Gui_CopyPage(&pOutput, pa, 0);
            Gui_DrawRectangleWired(pa, 0, 0, 127, 31, 1);
            Gui_Flush(pa);
            sy -= 8;
        }
    }
    Gui_FreePage(&pInput);
    Gui_FreePage(&pOutput);
    return 0;
}
