#include "TYPE.h"
#include "_STC8X_.h"
#include "tick.h"

static uint32_t tick_ms = 0;

void Timer0_Isr(void) interrupt 1
{
    tick_ms ++;
    // led1.Control(&led1);
}

void Timer0_Init(void) // 1毫秒@40.000MHz
{
    AUXR |= 0x80; // 定时器时钟1T模式
    TMOD &= 0xF0; // 设置定时器模式
    TL0 = 0xC0;   // 设置定时初始值
    TH0 = 0x63;   // 设置定时初始值
    TF0 = 0;      // 清除TF0标志
    TR0 = 1;      // 定时器0开始计时
    ET0 = 1;      // 使能定时器0中断   
}




void Tick_Init_1ms(){
    tick_ms = 0;
    Timer0_Init();
    
}
uint32_t Tick_Get(){
    return tick_ms;
}
/*

struct LED led1;
typedef struct LED
{
    uint8_t Status;
    uint8_t (*Get_Status)(struct LED *self);
    void (*Set_Status)(struct LED *self, uint8_t status);
    void (*Control)(struct LED *self);
};

uint8_t Get_Status(struct LED *self)
{
    return self->Status;
}
void Set_Status(struct LED *self, uint8_t status)
{
    self->Status = status;
}
void Control(struct LED *self)
{
    if (self->Status)
    {
        light = 1;
    }
    else
    {
        light = 0;
    }
}

void LED_Init(struct LED *self)
{
    self->Status = 0;
    self->Get_Status = Get_Status;
    self->Set_Status = Set_Status;
    self->Control = Control;
}
*/