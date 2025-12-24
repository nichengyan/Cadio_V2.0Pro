#include "set.h"

static uint8_t ColorReverse_Flg = 0;
static uint8_t SkipAnimation_Flg = 0;
static uint8_t AutoContinue_Flg = 0;
static uint8_t ErrorDetection_Flg = 1;
static uint8_t LongExpression_Flg = 0;

void Set_Menu(Page *pa)
{
    int posx, posy, desx, desy;
    float vx, vy;
    uint8_t option = 1, keynum;
    uint8_t setting_table[7];
    uint8_t lenth_table[7] = {0, 13, 14, 14, 15, 15, 15};
    posx = desx = lenth_table[1] * 6 + 2;
    posy = desy = 8 + option * 8;
    setting_table[1] = ColorReverse_Flg;
    setting_table[2] = SkipAnimation_Flg;
    setting_table[3] = AutoContinue_Flg;
    setting_table[4] = ErrorDetection_Flg;
    setting_table[5] = LongExpression_Flg;
    while (1)
    {
        Gui_ShowString(pa, 0, 0, "Setting:", 1, 1);
        keynum = Key_Scan() & 0x00ff;
        if (keynum == 0x3d)
            break;
        if (keynum == 0x44 && option > 1)
            option--;
        if (keynum == 0x49 && option < 5)
            option++;
        if (keynum == 0x48 || keynum == 0x4a)
            setting_table[option] = !setting_table[option];
        Gui_ShowString(pa, 0, 0, "Settings", 1, 1);
        if (setting_table[1])
        {
            Gui_ShowString(pa, 6, 16, "1:Color Reverse T", 1, 0);
            ColorReverse_Flg = 1;
            OLED_ColorTurn(1);
        }
        else
        {
            Gui_ShowString(pa, 6, 16, "1:Color Reverse F", 1, 0);
            ColorReverse_Flg = 0;
            OLED_ColorTurn(0);
        }
        if (setting_table[2])
        {
            Gui_ShowString(pa, 6, 24, "2:Skip Animation T", 1, 0);
            SkipAnimation_Flg = 1;
        }
        else
        {
            Gui_ShowString(pa, 6, 24, "2:Skip Animation F", 1, 0);
            SkipAnimation_Flg = 0;
        }
        if (setting_table[3])
        {
            Gui_ShowString(pa, 6, 32, "3:Auto Continue T", 1, 0);
            AutoContinue_Flg = 1;
        }
        else
        {
            Gui_ShowString(pa, 6, 32, "3:Auto Continue F", 1, 0);
            AutoContinue_Flg = 0;
        }
        if (setting_table[4])
        {
            Gui_ShowString(pa, 6, 40, "4:Error Detection T", 1, 0);
            ErrorDetection_Flg = 1;
        }
        else
        {
            Gui_ShowString(pa, 6, 40, "4:Error Detection F", 1, 0);
            ErrorDetection_Flg = 0;
        }
        if (setting_table[5])
        {
            Gui_ShowString(pa, 6, 48, "5:Long Expression T", 1, 0);
            LongExpression_Flg = 1;
        }
        else
        {
            Gui_ShowString(pa, 6, 48, "5:Long Expression F", 1, 0);
            LongExpression_Flg = 0;
        }
        desy = 8 + option * 8;
        desx = lenth_table[option] * 6 + 2;
        vy = (desy - posy) * 0.7;
        vx = (desx - posx) * 0.7;
        posy += vy;
        posx += vx;
        Gui_DrawRectangleWired(pa, 18, posy, posx, 8, 1);
        Gui_Flush(pa);
        Gui_ClearBufferBit(pa, 0);
    }
    while (Key_Scan() != 0xffff)
        ;
}
uint8_t Set_ColorReverse_Flg() { return ColorReverse_Flg; }
uint8_t Set_SkipAnimation_Flg() { return SkipAnimation_Flg; }
uint8_t Set_AutoContinue_Flg() { return  AutoContinue_Flg; }
uint8_t Set_ErrorDetection_Flg() { return ErrorDetection_Flg; }
uint8_t Set_LongExpression_Flg() { return LongExpression_Flg; }

// 因为实用性原因，开机密码也就算了。。。（大家可以自己弄一个）
// 因为懒得搞参数保存了，所有参数会掉电丢失！！
// 可以在8M SPIFlash 内置的FAT12里建一个文件专门来保存参数，但由于FAT12写入出了点锅，就先不弄了
// （不往STC自带的EEPROM里存是有深层原因的，因为所有FLASH空间都已经为FAT12_Loader规划好了）
