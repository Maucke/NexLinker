#include "oled.h"
#include "main.h"
#include "gpio.h"
#include "stdlib.h"

uint8_t *OLED_GRAM;

uint16_t ConvertColor(uint8_t red,uint8_t green,uint8_t blue)
{
	return red<<11|green<<6|blue;
}

uint16_t RandomColor(void){
	
	uint8_t red,green,blue;
	red = rand();
	__ASM("NOP");
	green = rand();
	__ASM("NOP");
	blue = rand();
	return (red<<11|green<<5|blue);
}

void Write_Command(uint8_t cmd)  {
  
  OLED_CS(GPIO_PIN_RESET);
  
#if  INTERFACE_4WIRE_SPI
  
  OLED_DC(GPIO_PIN_RESET);
  
  while(HAL_SPI_Transmit(&hspi1,&cmd,0x01,0x10) != HAL_OK);
  
  OLED_DC(GPIO_PIN_SET);
  
#elif INTERFACE_3WIRE_SPI
  
  uint8_t i;
	uint16_t hwData = 0;
	
  hwData = (uint16_t)cmd & ~0x0100;

	for(i = 0; i < 9; i ++) {
		OLED_SCK(GPIO_PIN_RESET);
    if(hwData & 0x0100) {
      OLED_DIN(GPIO_PIN_SET);
		}
    else  {
      OLED_DIN(GPIO_PIN_RESET);
		}
    OLED_SCK(GPIO_PIN_SET);
		hwData <<= 1;
	}

  
#endif
  
  OLED_CS(GPIO_PIN_SET);
}


void Write_Data(uint8_t dat) {
  
  OLED_CS(GPIO_PIN_RESET);
  OLED_DC(GPIO_PIN_SET);
  
  while(HAL_SPI_Transmit(&hspi1,&dat,0x01,0x10) != HAL_OK);
  
  OLED_DC(GPIO_PIN_RESET);
  OLED_CS(GPIO_PIN_SET);
  
}

void Write_Datas(uint8_t* dat_p, long length) {

  OLED_CS(GPIO_PIN_RESET);
  OLED_DC(GPIO_PIN_SET);
// while(HAL_SPI_Transmit(&hspi1,dat_p,length,0xffff) != HAL_OK);
	HAL_SPI_Transmit_DMA(&hspi1,dat_p,length);
//  OLED_DC(GPIO_PIN_RESET);
//  OLED_CS(GPIO_PIN_SET);
}

void SCR_reg(int idx, int value)
{
  Write_Command(idx);
  Write_Data(value);
}

void RAM_Address(void)  {
  OLED_CS(GPIO_PIN_SET);
  // draw region
  SCR_reg(0x17,0);
  SCR_reg(0x18,0x9f);
  SCR_reg(0x19,0);
  SCR_reg(0x1a,0x7f);
  
  // start position
//  SCR_reg(0x20,0);
//  SCR_reg(0x21,0);
}

void Refrash_Screen(void)  {
    
  RAM_Address();
  Write_Command(0x22);
	Write_Datas(OLED_GRAM,2*SCR_WIDTH*SCR_HEIGHT);//RAM data clear
}
  

void Clear_Screen(uint16_t color)  {
  
  int i,j;
  for(i=0;i<SCR_HEIGHT;i++)  {
    for(j=0;j<SCR_WIDTH;j++)  {
      OLED_GRAM[2*j+i*SCR_WIDTH*2] = color;
      OLED_GRAM[2*j+1+i*SCR_WIDTH*2] = color;
    }
  }
}

void Draw_Pixel(long x, long y,uint16_t color)
{
  // Bounds check.
  if ((x >= SCR_WIDTH) || (y >= SCR_HEIGHT)) return;
  if ((x < 0) || (y < 0)) return;

		OLED_GRAM[2*x+y*SCR_WIDTH*2] = (uint8_t)(color >> 8);
		OLED_GRAM[2*x+1+y*SCR_WIDTH*2] = (uint8_t)(color & 0x00ff);
}
  
  
void Device_Init(uint8_t *gram) {

	OLED_GRAM = gram;
  OLED_CS(GPIO_PIN_RESET);

  OLED_RST(GPIO_PIN_RESET);
  HAL_Delay(40);
  OLED_RST(GPIO_PIN_SET);
  HAL_Delay(20);
    
  // display off, analog reset
  SCR_reg(0x04, 0x01);
  HAL_Delay(1);
  // normal mode
  SCR_reg(0x04, 0x00);
  HAL_Delay(1);
  // display off
  SCR_reg(0x06, 0x00);
  // turn on internal oscillator using external resistor
  SCR_reg(0x02, 0x01);
  // 90 hz frame rate, divider 0
  SCR_reg(0x03, 0x30);
  // duty cycle 127
  SCR_reg(0x28, 0x7f);
  // start on line 0
  SCR_reg(0x29, 0x00);
  // rgb_if
  SCR_reg(0x14, 0x31);
  // memory write mode
  SCR_reg(0x16, 0x66);

  // driving current r g b (uA)
  SCR_reg(0x10, 0x45);
  SCR_reg(0x11, 0x34);
  SCR_reg(0x12, 0x33);

  // precharge time r g b
  SCR_reg(0x08, 0x04);
  SCR_reg(0x09, 0x05);
  SCR_reg(0x0a, 0x05);

  // precharge current r g b (uA)
  SCR_reg(0x0b, 0x9d);
  SCR_reg(0x0c, 0x8c);
  SCR_reg(0x0d, 0x57);

  SCR_reg(0x80, 0x00);

  // mode set
  SCR_reg(0x13, 0x00);
  
  Clear_Screen(0);
	Refrash_Screen();
	
  HAL_Delay(100);
  
  SCR_reg(0x06, 0x01);
}

// Draw a horizontal line ignoring any screen rotation.
void Draw_FastHLine(int16_t x, int16_t y, int16_t length,uint16_t color) {
  // Bounds check
		int16_t x0=x;
    do
    {
        Draw_Pixel(x, y,color);   // 逐点显示，描出垂直线
        x++;
    }
    while(x0+length>=x);
}
  // Draw a vertical line ignoring any screen rotation.
void Draw_FastVLine(int16_t x, int16_t y, int16_t length,uint16_t color)  {
  // Bounds check
		int16_t y0=y;
    do
    {
        Draw_Pixel(x, y,color);   // 逐点显示，描出垂直线
        y++;
    }
    while(y0+length>=y);
}

void Display_bbmp(int x,int y,int w,int h,const uint8_t *ch,uint16_t color) {
  
	uint8_t i,j,k;
	for(k=0;k<(h/8+1);k++)
		for(i=0;i<8;i++)
		{
			if(k*8+i>=h)
				return;
			for(j=0;j<w;j++)
					if(ch[k*w+j]&(1<<i))
						Draw_Pixel(x+j,y+k*8+i,color);
		}
}
  
void Display_bmp(int x,int y,int w,int h,const uint8_t *ch) {
  int Temp;
  int i,j;
  for(i=y;i<y+h;i++)  {
    for(j=x;j<x+w;j++)  {
			Temp = (i-y)*w*2+2*(j-x);
			if((ch[Temp+1]<<8)|ch[Temp])
				Draw_Pixel(j,i,(ch[Temp+1]<<8)|ch[Temp]); 
    }
  }
} 
