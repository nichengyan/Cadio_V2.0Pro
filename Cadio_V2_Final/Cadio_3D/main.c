#include "_STC8X_.h"
#include "oled.h"
#include "W25QXX.h"
#include <math.h>
#include "intrins.h"
#include "Graphic.h"
#define Main_Fosc 40000000ul
#define WINDOW_WIDTH 128*2     /* 放大6倍便于观看 */
#define WINDOW_HEIGHT 64*2      /* 放大6倍便于观看 */
#define LOGICAL_WIDTH 128       /* 逻辑分辨率 */
#define LOGICAL_HEIGHT 64       /* 逻辑分辨率 */
#define FIXED_SCALE 256         /* 定点数缩放因子：256 = 2^8 */
/* 使用16位定点数：高8位整数，低8位小数 */
typedef int fixed_t;  /* 16位有符号定点数 */

/* 定点数转换宏 */
#define INT_TO_FIXED(x) ((fixed_t)((x) * FIXED_SCALE))
#define FIXED_TO_INT(x) ((fixed_t)((x) / FIXED_SCALE))
#define FIXED_MUL(a, b) ((fixed_t)(((long)(a) * (long)(b)) / FIXED_SCALE))
#define FIXED_DIV(a, b) ((fixed_t)(((long)(a) * FIXED_SCALE) / (b)))


sbit Wifi = P2 ^ 0;
sbit Audio = P2 ^ 1;
sbit laser = P1 ^ 3;
sbit light = P1 ^ 7;




/* 顶点结构体 */
typedef struct Vertex
{
    fixed_t x;
    fixed_t y;
    fixed_t z;
};

/* 边结构体 */
typedef struct Edge
{
    int v1;
    int v2;
};


/* 面结构体 */
typedef struct Face
{
    int vertexCount;
    int vertices[4];
    uint8_t color;
};

/* 正弦余弦查找表（0-359度，精度0.01弧度） */
fixed_t sin_table[360];
fixed_t cos_table[360];

/* 函数声明 */
//void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);
void initTrigTables(void);
fixed_t fixed_sin(int angle);
fixed_t fixed_cos(int angle);
void rotatePoint(int angleX, int angleY, int angleZ, fixed_t* x, fixed_t* y, fixed_t* z);
void drawLine(int x1, int y1, int x2, int y2);
void drawFace(struct Face face, struct Vertex* screenVertices);

#ifndef cube

#define VERTICE_COUNT 8
#define EDGE_COUNT 12
#define FACE_COUNT 6

struct Vertex ObjVertices[VERTICE_COUNT] = {
    {INT_TO_FIXED(-30), INT_TO_FIXED(-30), INT_TO_FIXED(-30)},  
    {INT_TO_FIXED( 30), INT_TO_FIXED(-30), INT_TO_FIXED(-30)},  
    {INT_TO_FIXED( 30), INT_TO_FIXED( 30), INT_TO_FIXED(-30)},  
    {INT_TO_FIXED(-30), INT_TO_FIXED( 30), INT_TO_FIXED(-30)},  
    {INT_TO_FIXED(-30), INT_TO_FIXED(-30), INT_TO_FIXED( 30)}, 
    {INT_TO_FIXED( 30), INT_TO_FIXED(-30), INT_TO_FIXED( 30)}, 
    {INT_TO_FIXED( 30), INT_TO_FIXED( 30), INT_TO_FIXED( 30)},  
    {INT_TO_FIXED(-30), INT_TO_FIXED( 30), INT_TO_FIXED( 30)}   
};


struct Edge ObjEdges[EDGE_COUNT] = {
    {0, 1},  
    {1, 2}, 
    {2, 3}, 
    {3, 0},  
    {4, 5}, 
    {5, 6}, 
    {6, 7}, 
    {7, 4},  
    {0, 4}, 
    {1, 5},  
    {2, 6}, 
    {3, 7} 
};

// 立方体的6个面（包含颜色） *
struct Face ObjFaces[FACE_COUNT] = {
    {4, {0, 1, 2, 3}, 1},     
    {4, {5, 4, 7, 6}, 4},     
    {4, {4, 0, 3, 7}, 7},    
    {4, {1, 5, 6, 2}, 11},  
    {4, {3, 2, 6, 7}, 13},   
    {4, {4, 5, 1, 0}, 15}    
};




#else
#define VERTICE_COUNT 20    /* 4的倍数，保证每个面4顶点 */
#define EDGE_COUNT 34       /* R标边数 */
#define FACE_COUNT 17       /* R标面数（每个面4顶点） */

struct Vertex ObjVertices[VERTICE_COUNT] = {
    /* 前面 - 分为四个矩形区域 */
    /* 竖线矩形1 */
    {INT_TO_FIXED(-35), INT_TO_FIXED(-40), INT_TO_FIXED(-10)},  /* 0: 左下外 */
    {INT_TO_FIXED(-25), INT_TO_FIXED(-40), INT_TO_FIXED(-10)},  /* 1: 左下内 */
    {INT_TO_FIXED(-25), INT_TO_FIXED( 20), INT_TO_FIXED(-10)},  /* 2: 左中内 */
    {INT_TO_FIXED(-35), INT_TO_FIXED( 20), INT_TO_FIXED(-10)},  /* 3: 左中外 */
    
    /* 竖线矩形2 */
    {INT_TO_FIXED(-35), INT_TO_FIXED( 20), INT_TO_FIXED(-10)},  /* 4: 左中外（与3相同） */
    {INT_TO_FIXED(-25), INT_TO_FIXED( 20), INT_TO_FIXED(-10)},  /* 5: 左中内（与2相同） */
    {INT_TO_FIXED(-25), INT_TO_FIXED( 40), INT_TO_FIXED(-10)},  /* 6: 左上内 */
    {INT_TO_FIXED(-35), INT_TO_FIXED( 40), INT_TO_FIXED(-10)},  /* 7: 左上外 */
    
    /* 环形矩形 */
    {INT_TO_FIXED(-25), INT_TO_FIXED( 20), INT_TO_FIXED(-10)},  /* 8: 左中（与2/5相同） */
    {INT_TO_FIXED( 15), INT_TO_FIXED( 20), INT_TO_FIXED(-10)},  /* 9: 右下外 */
    {INT_TO_FIXED( 15), INT_TO_FIXED( 40), INT_TO_FIXED(-10)},  /* 10: 右上外 */
    {INT_TO_FIXED(-25), INT_TO_FIXED( 40), INT_TO_FIXED(-10)},  /* 11: 左上内（与6相同） */
    
    /* 斜线矩形（近似为平行四边形） */
    {INT_TO_FIXED(-25), INT_TO_FIXED(  0), INT_TO_FIXED(-10)},  /* 12: 斜线起点上 */
    {INT_TO_FIXED(-15), INT_TO_FIXED(-40), INT_TO_FIXED(-10)},  /* 13: 斜线左下 */
    {INT_TO_FIXED( 25), INT_TO_FIXED(-40), INT_TO_FIXED(-10)},  /* 14: 斜线右下 */
    {INT_TO_FIXED( 15), INT_TO_FIXED(  0), INT_TO_FIXED(-10)},  /* 15: 斜线起点右 */
    
    /* 背面对应点（每个矩形对应一个背面点） */
    {INT_TO_FIXED(-35), INT_TO_FIXED(-40), INT_TO_FIXED(10)},   /* 16: 0的背面 */
    {INT_TO_FIXED( 25), INT_TO_FIXED(-40), INT_TO_FIXED(10)},   /* 17: 14的背面 */
    {INT_TO_FIXED(-35), INT_TO_FIXED( 40), INT_TO_FIXED(10)},   /* 18: 7的背面 */
    {INT_TO_FIXED( 15), INT_TO_FIXED( 40), INT_TO_FIXED(10)},   /* 19: 10的背面 */
};

struct Edge ObjEdges[EDGE_COUNT] = {
    /* 前面矩形边 */
    {0, 1}, {1, 2}, {2, 3}, {3, 0},   /* 矩形1 */
    {4, 5}, {5, 6}, {6, 7}, {7, 4},   /* 矩形2 */
    {8, 9}, {9, 10}, {10, 11}, {11, 8}, /* 矩形3 */
    {12, 13}, {13, 14}, {14, 15}, {15, 12}, /* 矩形4 */
    
    /* 连接前面矩形的边（形成R形状） */
    {2, 8},   /* 竖线连接环形 */
    {6, 11},  /* 竖线上连接环形上 */
    {9, 15},  /* 环形右连接斜线 */
    
    /* 连接前后面的垂直边 */
    {0, 16},  /* 左下外 */
    {14, 17}, /* 斜线右下 */
    {7, 18},  /* 左上外 */
    {10, 19}, /* 环形右上 */
    
    /* 背面矩形边 */
    {16, 17}, /* 背面底部 */
    {17, 19}, /* 背面右下到右上 */
    {19, 18}, /* 背面上边 */
    {18, 16}, /* 背面左边 */
    
    /* 额外的结构边 */
    {13, 16}, /* 斜线左下到背面 */
    {1, 16},  /* 左下内到背面 */
    {6, 18},  /* 左上内到背面 */
    {10, 19}, /* 环形右上到背面（重复但需要） */
};

/* 所有面都是4顶点，保证共面 */
struct Face ObjFaces[FACE_COUNT] = {
    /* 前面4个矩形 */
    {4, {0, 1, 2, 3}, 1},      /* 竖线下半部 */
    {4, {4, 5, 6, 7}, 1},      /* 竖线上半部 */
    {4, {8, 9, 10, 11}, 1},    /* 环形部分 */
    {4, {12, 13, 14, 15}, 1},  /* 斜线部分 */
    
    /* 对应的背面4个矩形 */
    {4, {16, 1, 2, 18}, 2},    /* 背面竖线下 */
    {4, {16, 2, 6, 18}, 2},    /* 背面竖线上 */
    {4, {18, 6, 10, 19}, 2},   /* 背面环形 */
    {4, {16, 13, 14, 17}, 2},  /* 背面斜线 */
    
    /* 侧面（连接前后） */
    {4, {0, 3, 7, 16}, 3},     /* 左侧面 */
    {4, {3, 2, 6, 7}, 4},      /* 竖线前面中间（侧面效果） */
    {4, {7, 6, 11, 10}, 5},    /* 上侧面 */
    {4, {10, 9, 15, 19}, 6},   /* 右上侧面 */
    {4, {9, 8, 12, 15}, 7},    /* 右下侧面 */
    {4, {0, 1, 13, 16}, 8},    /* 左下侧面 */
    {4, {1, 2, 8, 13}, 9},     /* 中间侧面 */
    {4, {13, 8, 12, 13}, 10},  /* 斜线起点侧面 */
    {4, {14, 15, 19, 17}, 11} /* 斜线终点侧面 */
};
#endif
// 绘制单个像素 
const int DITHER_MATRIX[4][4] = {
    { 0,  8,  2, 10},
    {12,  4, 14,  6},
    { 3, 11,  1,  9},
    {15,  7, 13,  5}
};
void drawDitherPixel(int x, int y, int gray)
{
    if(x <=0 || x >=128 ||y <= 0||y >=64) return;
    /* 抖动决策：如果灰度值大于阈值，则点亮像素 */
    if (gray > DITHER_MATRIX[y % 4][x % 4])
        Graphic_DrawPoint(x, y, 1);
}

/* 绘制直线 */
void drawLine(int x1, int y1, int x2, int y2)
{
    int dx;
    int dy;
    int sx;
    int sy;
    int err;
    int e2;
    
    dx = abs(x2 - x1);
    dy = abs(y2 - y1);
    
    sx = (x1 < x2) ? 1 : -1;
    sy = (y1 < y2) ? 1 : -1;
    
    err = dx - dy;
    
    while (1)
    {
         Graphic_DrawPoint(x1, y1, 1);
        
        if (x1 == x2 && y1 == y2)
        {
            break;
        }
        
        e2 = 2 * err;
        
        if (e2 > -dy)
        {
            err -= dy;
            x1 += sx;
        }
        
        if (e2 < dx)
        {
            err += dx;
            y1 += sy;
        }
    }
}


void drawFace(struct Face face, struct Vertex* screenVertices)
{
    short i;
    short minY;
    short maxY;
    short x1, y1, x2, y2;
    short x;
    short t;
    short intersections[4];
    short intersectCount;
    short j, k;
    struct Vertex v1;
    struct Vertex v2;
    float a,b;
    /* 计算面的边界 */
    minY = LOGICAL_HEIGHT;
    maxY = 0;
    
    for (i = 0; i < face.vertexCount; i++)
    {
        short screenY = FIXED_TO_INT(screenVertices[face.vertices[i]].y);
        
        if (screenY < minY) minY = screenY;
        if (screenY > maxY) maxY = screenY;
    }
    
    if (minY < 0) minY = 0;
    if (maxY >= LOGICAL_HEIGHT) maxY = LOGICAL_HEIGHT - 1;
    
    /* 对每条扫描线进行填充 */
    for (i = minY; i <= maxY; i++)
    {
        intersectCount = 0;
        
        /* 计算多边形与当前扫描线的交点 */
        for (j = 0; j < face.vertexCount; j++)
        {
            short nextJ = (j + 1) % face.vertexCount;
            v1 = screenVertices[face.vertices[j]];
            v2 = screenVertices[face.vertices[nextJ]];
            
            x1 = FIXED_TO_INT(v1.x);
            y1 = FIXED_TO_INT(v1.y);
            x2 = FIXED_TO_INT(v2.x);
            y2 = FIXED_TO_INT(v2.y);
            
            /* 检查线段是否与扫描线相交 */
            if ((y1 <= i && y2 > i) || (y2 <= i && y1 > i))
            {
                /* 计算交点x坐标 */
                if (y2 != y1)
                {
                    t = ((i - y1) * (x2 - x1)) / (y2 - y1);
                    x = x1 + t;
                    
                    if (intersectCount < 4)
                    {
                        intersections[intersectCount] = x;
                        intersectCount++;
                    }
                }
            }
        }
        
        /* 对交点进行排序 */
        for (j = 0; j < intersectCount - 1; j++)
        {
            for (k = 0; k < intersectCount - j - 1; k++)
            {
                if (intersections[k] > intersections[k + 1])
                {
                    short temp = intersections[k];
                    intersections[k] = intersections[k + 1];
                    intersections[k + 1] = temp;
                }
            }
        }
        
        /* 在交点之间绘制水平线 */
        for (j = 0; j < intersectCount; j += 2)
        {
            if (j + 1 < intersectCount)
            {
                short startX = intersections[j];
                short endX = intersections[j + 1];
                
                if (startX < 0) startX = 0;
                if (endX >= LOGICAL_WIDTH) endX = LOGICAL_WIDTH - 1;
                
                for (x = startX; x <= endX; x++)
                {
                    drawDitherPixel(x, i, face.color);
                }
            }
        }
    }
}

/* 初始化三角函数查找表 */
void initTrigTables(void)
{
    int i;
    double angle;
    
    for (i = 0; i < 360; i++)
    {
        angle = i * 3.1415926535 / 180.0;  /* 转换为弧度 */
        sin_table[i] = INT_TO_FIXED(sin(angle));
        cos_table[i] = INT_TO_FIXED(cos(angle));
    }
}

/* 定点数正弦函数 */
fixed_t fixed_sin(int angle)
{
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;
    return sin_table[angle];
}

/* 定点数余弦函数 */
fixed_t fixed_cos(int angle)
{
    while (angle < 0) angle += 360;
    while (angle >= 360) angle -= 360;
    return cos_table[angle];
}

/* 定点数旋转矩阵函数 */
void rotatePoint(int angleX, int angleY, int angleZ, fixed_t* x, fixed_t* y, fixed_t* z)
{
    fixed_t tempX;
    fixed_t tempY;
    fixed_t tempZ;
    fixed_t sinX, cosX;
    fixed_t sinY, cosY;
    fixed_t sinZ, cosZ;
    
    /* 保存原始坐标 */
    tempX = *x;
    tempY = *y;
    tempZ = *z;
    
    /* 绕X轴旋转 */
    sinX = fixed_sin(angleX);
    cosX = fixed_cos(angleX);
    
    *y = FIXED_MUL(tempY, cosX) - FIXED_MUL(tempZ, sinX);
    *z = FIXED_MUL(tempY, sinX) + FIXED_MUL(tempZ, cosX);
    
    tempY = *y;
    tempZ = *z;
    
    /* 绕Y轴旋转 */
    sinY = fixed_sin(angleY);
    cosY = fixed_cos(angleY);
    
    *x = FIXED_MUL(tempX, cosY) + FIXED_MUL(tempZ, sinY);
    *z = -FIXED_MUL(tempX, sinY) + FIXED_MUL(tempZ, cosY);
    
    tempX = *x;
    tempZ = *z;
    
    /* 绕Z轴旋转 */
    sinZ = fixed_sin(angleZ);
    cosZ = fixed_cos(angleZ);
    
    *x = FIXED_MUL(tempX, cosZ) - FIXED_MUL(tempY, sinZ);
    *y = FIXED_MUL(tempX, sinZ) + FIXED_MUL(tempY, cosZ);
}

int t = 0, i = 0,j = 0, k = 0;
uint32_t ms =0,time_used,ta1,tb1;

uint8_t duration;
uint8_t flag = 0;
void PWM_config(void)
{
	P_SW2 |= 0x80;		//SFR enable   
	PWMB_PSCR = 0x0000;	// 预分频寄存器, 分频 Fck_cnt = Fck_psc/(PSCR[15:0}+1), 边沿对齐PWM频率 = SYSclk/((PSCR+1)*(AAR+1)), 中央对齐PWM频率 = SYSclk/((PSCR+1)*(AAR+1)*2).
	PWMB_DTR  = 00;		// 死区时间配置,   n=0~127: DTR= n T,   0x80 ~(0x80+n), n=0~63: DTR=(64+n)*2T,  
						// 对PWMB没有意义  0xc0 ~(0xc0+n), n=0~31: DTR=(32+n)*8T,   0xE0 ~(0xE0+n), n=0~31: DTR=(32+n)*16T,
	PWMB_ARR = 256 * 8;		// 自动重装载寄存器, 控制PWM周期

	PWMB_CCMR2 = 0x68;	// 通道模式配置, PWM模式1, 预装载允许
	PWMB_CCR2L = 0;	
	PWMB_CCER1 = 0x10;	// 配置通道输出使能和极性 PWM6 PWM5
    
	PWMB_BKR = 0x80;	// 主输出使能 相当于总开关
	PWMB_IER = 0x00;	// 使能中断
	PWMB_CR1 = 0x81;	// 使能计数器, 允许自动重装载寄存器缓冲, 边沿对齐模式, 向上计数, bit7=1:写自动重装载寄存器缓冲(本周期不会被打扰), =0:直接写自动重装载寄存器
	

	PWMB_ENO  = 0x04;	//IO输出PWM允许, bit6: ENO8P,  bit4: ENO7P,  bit2: ENO6P,  bit0: ENO5P
	PWMB_PS   = 0x04;	//选择IO, 4项从高到低(从左到右)对应PWM8 PWM7 PWM6 PWM5  
}
#define TONES_END 0x8000
uint16_t   sound[] = {
	0x0151,0x0007,0x0000,0x0015,0x018d,0x0007,0x0000,0x0007,0x014b,0x0007,0x0136,0x0007,0x0146,0x0007,0x0169,0x0007,0x019e,0x0007,0x007f,0x0007,0x0185,
	0x0007,0x0228,0x0007,0x026d,0x0007,0x02fc,0x0007,0x02e0,0x0007,0x01fd,0x0007,0x0219,0x0007,0x033c,0x0007,0x00e7,0x0007,0x0281,0x0007,0x026d,
	0x000e,0x052d,0x000e,0x04da,0x000e,0x011c,0x0007,0x0387,0x0007,0x0360,0x0007,0x033c,0x0007,0x0387,0x0007,0x02ad,0x000e,0x02c6,0x0007,0x010c,
	0x000e,0x02e0,0x0007,0x02c6,0x0007,0x0296,0x000e,0x0281,0x0007,0x02ad,0x0007,0x02c6,0x0007,0x02e0,0x0007,0x00e1,0x0007,0x031b,0x0007,0x0248,
	0x0007,0x0219,0x0007,0x01f1,0x0007,0x01d9,0x000e,0x01f1,0x0007,0x020b,0x0007,0x02ad,0x0007,0x00d1,0x0007,0x0248,0x0007,0x0238,0x0007,0x0219,
	0x0007,0x0228,0x000e,0x020b,0x0007,0x01e5,0x0007,0x01d9,0x0007, TONES_END
};
uint16_t *sound_array;
int now_freq  = 30;
int now_dura  = 30 ;
uint8_t status = 0,ptr = 0;
void Tones(uint16_t *s){
    sound_array = s;
    ptr = 0;
    status = 1;
    now_freq = 0;
    now_dura = 0;
   // ptr +=2;
}
void Beep(uint16_t f,uint16_t du){
    flag = 1;
    duration = du;
    PWMB_PSCR = Main_Fosc/PWMB_ARR/(f+ 1) ;
    
}
void Buzzer_Poll(){
    if(status){
        if(now_freq == TONES_END || now_dura==TONES_END){
            status = 0;
            ptr = 0;
            now_dura = 0;
            PWMB_CCR2L = 0;
            return;
        }else{
            PWMB_CCR2L = 32;
            PWMB_PSCR = Main_Fosc/PWMB_ARR/(now_freq+ 1) ;
            if(now_dura > 0){
                now_dura--;
                return;
            }else{        
                now_freq = sound_array[ptr];
                now_dura = sound_array[ptr + 1] * 5;
                ptr += 2;
                return;
            }
        }
    }
}


void Timer1_Isr(void) interrupt 3
{
    //Buzzer_Poll();
    ms++;
}

void Timer1_Init(void)		//1毫秒@40.000MHz
{
	AUXR |= 0x40;			//定时器时钟1T模式
	TMOD &= 0x0F;			//设置定时器模式
	TL1 = 0xC0;				//设置定时初始值
	TH1 = 0x63;				//设置定时初始值
	TF1 = 0;				//清除TF1标志
	TR1 = 1;				//定时器1开始计时
	ET1 = 1;				//使能定时器1中断
}

void main()
{ 
    uint16_t ms_cnt;
    uint8_t quit;
    int angleX;
    int angleY;
    int angleZ;
    int i;
    struct Vertex transformedVertices[128];
    struct Vertex screenVertices[128];
    float faceDepths[128];
    int faceOrder[128];
    int j, k;
    int faceIndex,v1Index,v2Index;
    float tempDepth;
    int tempOrder;
    fixed_t avgDepth;
   // P_SW2 |= 0x80;
    P0M0 = 0x00;
    P0M1 = 0x00;
    P1M1 = 0xf7;
    P1M0 = 0x00; // 设置为高阻抗
    P2M0 = 0x00;
    P2M1 = 0x00;
    P3M0 = 0x00;
    P3M1 = 0x00;
    P5M0 = 0x00;
    P5M1 = 0x00;
    Audio = 1;
    Wifi = 0;
    laser = 0;
    light = 0;
    OLED_Init();
    OLED_Display_On();
    OLED_ColorTurn(0);
    OLED_Clear();
    Graphic_ClearBufferBit(0);
    PWM_config();
    

    Timer1_Init();
    EA = 1;
    

    //Beep(500,1000);
    /* 初始化三角函数表 */
    initTrigTables();
    
    /* 初始化旋转角度 */
    angleX = 0;
    angleY = 0;
    angleZ = 0;
    
    /* 主循环标志 */
    quit = 0;
    
    while (!quit)
    {
        ta1 = ms;
        
        /* 更新旋转角度 */
        angleX = (angleX + 2) % 360;
        angleY = (angleY + 4) % 360;
        angleZ = (angleZ + 6) % 360;
        
        Graphic_ClearBufferBit(0);
        /* 旋转所有顶点 */
        for (i = 0; i < VERTICE_COUNT; i++)
        {
            fixed_t x = ObjVertices[i].x;
            fixed_t y = ObjVertices[i].y;
            fixed_t z = ObjVertices[i].z;
            
            /* 应用旋转 */
            rotatePoint(angleX, angleY, angleZ, &x, &y, &z);
            
            transformedVertices[i].x = x;
            transformedVertices[i].y = y;
            transformedVertices[i].z = z;
            
            /* 转换为屏幕坐标（正交投影，缩放1.5倍，居中） */
            screenVertices[i].x = x / 2 + INT_TO_FIXED(LOGICAL_WIDTH / 2);
            screenVertices[i].y = y / 2 + INT_TO_FIXED(LOGICAL_HEIGHT / 2);
            screenVertices[i].z = z;
        }
        
        /* 计算每个面的深度（平均z值）用于排序 */
        for (i = 0; i < FACE_COUNT; i++)
        {
            avgDepth = 0;
            
            for (j = 0; j < ObjFaces[i].vertexCount; j++)
            {
                avgDepth += transformedVertices[ObjFaces[i].vertices[j]].z;
            }
            
            faceDepths[i] = (float)avgDepth / (ObjFaces[i].vertexCount * FIXED_SCALE);
            faceOrder[i] = i;
        }
        
        /* 按深度排序面（从后往前绘制） */
        for (j = 0; j < FACE_COUNT - 1; j++)
        {
            for (k = 0; k < FACE_COUNT - j - 1; k++)
            {
                if (faceDepths[k] < faceDepths[k + 1])
                {
                    /* 交换深度 */
                    tempDepth = faceDepths[k];
                    faceDepths[k] = faceDepths[k + 1];
                    faceDepths[k + 1] = tempDepth;
                    
                    /* 交换面顺序 */
                    tempOrder = faceOrder[k];
                    faceOrder[k] = faceOrder[k + 1];
                    faceOrder[k + 1] = tempOrder;
                }
            }
        }
        
        /* 绘制面（从后往前） */
        for (i = 0; i < FACE_COUNT; i++)
        {
            faceIndex = faceOrder[i];
            drawFace(ObjFaces[faceIndex], screenVertices);
        }
        
        /* 绘制立方体的白色边线 */
        for (i = 0; i < EDGE_COUNT; i++)
        {
            v1Index = ObjEdges[i].v1;
            v2Index = ObjEdges[i].v2;
            
            drawLine(
                FIXED_TO_INT(screenVertices[v1Index].x),
                FIXED_TO_INT(screenVertices[v1Index].y),
                FIXED_TO_INT(screenVertices[v2Index].x),
                FIXED_TO_INT(screenVertices[v2Index].y)
            );

        }
        
        Graphic_Printf(0,0,"%.1f", 1000.0f/ ms_cnt);
        Graphic_Flush();
        tb1 = ms;
        ms_cnt = tb1 - ta1;
    }

    while (1)
        ;
    return;
}