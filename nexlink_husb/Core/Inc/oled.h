#ifndef __OLED_DRIVER_H
#define __OLED_DRIVER_H

#include "stm32f4xx_hal.h"
#include "spi.h"
#include "stdbool.h"

#define UT_RPM 0
#define UT_DEG 1
#define UT_MHZ 2
#define UT_PREC 3

#define CU_CPU 0
#define CU_GPU 1
#define CU_RAM 2

#define TP_RAD 0
#define TP_NVI 1

#define OCX SCR_WIDTH/2
#define OCY SCR_HEIGHT/2
#define PI 3.14159f
#define INTERFACE_4WIRE_SPI 1
#define SCR_WIDTH   160
#define SCR_HEIGHT  128

#define swap(a, b) { uint16_t t = a; a = b; b = t; }

#define OLED_RST(x)   HAL_GPIO_WritePin(OLED_RST_GPIO_Port, OLED_RST_Pin, x)
#define OLED_DC(x)    HAL_GPIO_WritePin(OLED_DC_GPIO_Port, OLED_DC_Pin, x)
#define OLED_CS(x)    HAL_GPIO_WritePin(OLED_CS_GPIO_Port, OLED_CS_Pin, x)

#if INTERFACE_3WIRE_SPI

#define OLED_SCK(x)   HAL_GPIO_WritePin(OLED_SCK_GPIO_Port, OLED_SCK_Pin, x)
#define OLED_DIN(x)   HAL_GPIO_WritePin(OLED_DIN_GPIO_Port, OLED_DIN_Pin, x)

#endif

#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

typedef unsigned short uint16_t;
typedef unsigned char  uint8_t;
typedef unsigned char  uint8;                   /* defined for unsigned 8-bits integer variable 	无符号8位整型变量  */
typedef signed   char  int8;                    /* defined for signed 8-bits integer variable		有符号8位整型变量  */
typedef unsigned short uint16;                  /* defined for unsigned 16-bits integer variable 	无符号16位整型变量 */
typedef signed   short int16;                   /* defined for signed 16-bits integer variable 		有符号16位整型变量 */
typedef unsigned int   uint32;                  /* defined for unsigned 32-bits integer variable 	无符号32位整型变量 */
typedef signed   int   int32;                   /* defined for signed 32-bits integer variable 		有符号32位整型变量 */
typedef float          fp32;                    /* single precision floating point variable (32bits) 单精度浮点数（32位长度） */
typedef double         fp64;                    /* double precision floating point variable (64bits) 双精度浮点数（64位长度） */

void SCR_reg(int idx, int value);
void Calc_Color(void);
void MOLED_Fill(uint16_t data);
uint16_t WheelP(uint16_t WheelPos);
uint16_t WheelPw(uint16_t WheelPos);

uint16_t ConvertColor(uint8_t red,uint8_t green,uint8_t blue);
void Device_Init(uint8_t *gram);
void Clear_Screen(uint16_t color);
void Refrash_Screen(void);
void Fill_Color(uint16_t color);
void Set_Coordinate(uint16_t x, uint16_t y);
uint16_t RandomColor(void);

void Write_text(uint8_t data1);
void Set_Address(uint8_t column, uint8_t row);
void Set_Direction(uint8_t dir);

void Set_Color(uint16_t color);
void Set_FillColor(uint16_t color);
void Display_bbmp(int x,int y,int w,int h,const uint8_t *ch,uint16_t color);

void Display_bmp(int x,int y,int w,int h,const uint8_t *ch);
void Invert(bool v);
void Draw_Pixel(long x, long y,uint16_t color);
void Write_Data(uint8_t dat);
void Write_Datas(uint8_t* dat_p, long length);

void Draw_FastHLine(int16_t x, int16_t y, int16_t length,uint16_t color);

void Draw_FastVLine(int16_t x, int16_t y, int16_t length,uint16_t color);
void Write_Command(uint8_t data1);

void Display_hbmp(int x,int y,int w,int h,const uint8_t *ch,uint16_t color,uint8_t bk);

void Set_DampColor(uint16_t color);

void OLED_SNF6x8(int x,int y,char *ch,uint16_t color);
void Clear_FpsCount(void);
void OLED_HFAny(int x,int y,int w,int h,uint8_t Num,const unsigned char *ch,uint16_t color);
void OLED_BFAny(int x,int y,int w,int h,uint8_t Num,const unsigned char *ch,uint16_t color,uint16_t hui);
void OLED_SBFAny(int x,int y,char *ch,int w,uint16_t color);
uint8_t Float2U8(float Input);
void RAM_Address(void);
void OLED_NF6x8(int x,int y,uint8_t Num,uint8_t Offset);
void OLED_F8x16(int x,int y,uint8_t Num);

#endif

