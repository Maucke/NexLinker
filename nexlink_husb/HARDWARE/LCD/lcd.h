#ifndef _LCD_H_
#define _LCD_H_

#include "main.h"

//#define USE_HORIZONTAL 0		 //设置横屏或者竖屏显示 0或1为竖屏 2或3为横屏
#define X_MAX 240				 //LCD排线朝下时的X和Y最大像素点
#define Y_MAX 280

#if USE_HORIZONTAL == 0 || USE_HORIZONTAL == 1
#define LCD_W X_MAX
#define LCD_H Y_MAX

#else
#define LCD_W Y_MAX
#define LCD_H X_MAX
#endif

/**********************************DMA相关**************************************************/

#define LCD_SPI_TX_DMA DMA2_Stream3				//DMA1通道

/**********************************DMA相关**************************************************/

/**********************************普通IO相关**************************************************/
#define LCD_DC_GPIO GPIOA
#define LCD_DC_PIN GPIO_PIN_8

#define LCD_CS_GPIO GPIOA
#define LCD_CS_PIN GPIO_PIN_4

#define LCD_RST_GPIO GPIOC
#define LCD_RST_PIN GPIO_PIN_9

#define PIN_OUT(PORT, PIN, STATUS) (PORT)->BSRR = (STATUS) ? (PIN) : (uint32_t)(PIN) << 16

#define LCD_DC_OUT(STATUS) PIN_OUT(LCD_DC_GPIO, LCD_DC_PIN, STATUS)
#define LCD_CS_OUT(STATUS) PIN_OUT(LCD_CS_GPIO, LCD_CS_PIN, STATUS)
#define LCD_RST_OUT(STATUS) PIN_OUT(LCD_RST_GPIO, LCD_RST_PIN, STATUS)

typedef enum
{
	DMA_MEMINC_ENABLE = 0,
	DMA_MEMINC_DISABLE
} DMA_MEMINC_STATE;

void LCD_WR_DATA8(uint8_t dat);													   //写入一个字节
void LCD_WR_DATA(uint16_t dat);													   //写入两个字节
void LCD_WR_REG(uint8_t dat);													   //写入一个指令
void LCD_DMA_Transfer16Bit(uint8_t *pData, uint16_t size, DMA_MEMINC_STATE state); //启动DMA连续发送单个16bit数据
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);		   //设置坐标函数
void LCD_Init(void);															   //LCD初始化

void LCD_Fill(uint16_t xsta,uint16_t ysta,uint16_t xend,uint16_t yend,uint16_t color);
void LCD_Color_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t *buf);
uint16_t LCD_ReadScanLine(void);

void LCD_SetRotation(uint8_t dir);

#endif
