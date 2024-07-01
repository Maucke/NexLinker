#include "lcd.h"
#include "spi.h"
#include "stdbool.h"

#define delay HAL_Delay
__IO uint8_t lcd_direction = 0;

// LCD串行数据写入
static void LCD_Writ_Bus(uint8_t dat)
{
    HAL_SPI_Transmit(&hspi1, &dat, 1, 100);
}

// LCD写入8位数据
void LCD_WR_DATA8(uint8_t dat)
{
    LCD_CS_OUT(0);

    LCD_Writ_Bus(dat);
	
    LCD_CS_OUT(1);
}

// LCD写入16位数据
void LCD_WR_DATA(uint16_t dat)
{
    LCD_CS_OUT(0);
	
    LCD_Writ_Bus(dat >> 8);
    LCD_Writ_Bus(dat);
	
    LCD_CS_OUT(1);
}

// LCD写入命令
void LCD_WR_REG(uint8_t dat)
{
    LCD_CS_OUT(0);

    LCD_DC_OUT(0); // 写命令
    LCD_Writ_Bus(dat);
    LCD_DC_OUT(1); // 写数据

    LCD_CS_OUT(1);
}

// 启用SPI DMA连续发送单个16bit数据
void LCD_DMA_Transfer16Bit(uint8_t *pData, uint16_t size, DMA_MEMINC_STATE state)
{
	while (hspi1.State != HAL_SPI_STATE_READY)
			; // 等待SPI空闲
	// 清除 DMA 控制寄存器的相关设置
    LCD_SPI_TX_DMA->CR &= ~DMA_SxCR_MINC; 

//    // 设置 DMA 存储器和外设数据长度为半字(16bit)
//    LCD_SPI_TX_DMA->CR |= DMA_SxCR_MSIZE_0 | DMA_SxCR_PSIZE_0; 

    // 根据传入的状态设置是否使能存储器地址增量
    if (state == DMA_MEMINC_ENABLE)
        LCD_SPI_TX_DMA->CR |= DMA_SxCR_MINC; 

    HAL_SPI_Transmit_DMA(&hspi1, pData, size); // 启用DMA传输
}

/*
 *功能: 设置起始和结束地址
 *参数1: @x1,x2 - 设置列的起始和结束地址
 *参数1: @y1,y2 - 设置行的起始和结束地址
 */
void LCD_Address_Set(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
	while (hspi1.State != HAL_SPI_STATE_READY)
			; // 等待SPI空闲
    LCD_CS_OUT(1);
	if(lcd_direction==0)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+20);
		LCD_WR_DATA(y2+20);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(lcd_direction==1)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1);
		LCD_WR_DATA(x2);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1+20);
		LCD_WR_DATA(y2+20);
		LCD_WR_REG(0x2c);//储存器写
	}
	else if(lcd_direction==2)
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+20);
		LCD_WR_DATA(x2+20);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
	else
	{
		LCD_WR_REG(0x2a);//列地址设置
		LCD_WR_DATA(x1+20);
		LCD_WR_DATA(x2+20);
		LCD_WR_REG(0x2b);//行地址设置
		LCD_WR_DATA(y1);
		LCD_WR_DATA(y2);
		LCD_WR_REG(0x2c);//储存器写
	}
    LCD_CS_OUT(0);
}

void LCD_SetRotation(uint8_t dir)
{
	lcd_direction = dir;
	LCD_WR_REG(0x36);
	if(lcd_direction==0)LCD_WR_DATA8(0x08);
	else if(lcd_direction==1)LCD_WR_DATA8(0xC8);
	else if(lcd_direction==2)LCD_WR_DATA8(0x78);
	else LCD_WR_DATA8(0xA8);
}

// LCD初始化
void LCD_Init(void)
{
	LCD_RST_OUT(0);
	HAL_Delay(200);
	LCD_RST_OUT(1);
	HAL_Delay(10); 

	LCD_WR_REG(0xfd);//private_access
	LCD_WR_DATA8(0x06);
	LCD_WR_DATA8(0x08);

	LCD_WR_REG(0x61);//add
	LCD_WR_DATA8(0x07);//
	LCD_WR_DATA8(0x04);//

	LCD_WR_REG(0x62);//bias setting
	LCD_WR_DATA8(0x00);//00
	LCD_WR_DATA8(0x44);//44
	LCD_WR_DATA8(0x45);//40  47

	LCD_WR_REG(0x63);//
	LCD_WR_DATA8(0x41);//
	LCD_WR_DATA8(0x07);//
	LCD_WR_DATA8(0x12);//
	LCD_WR_DATA8(0x12);//

	LCD_WR_REG(0x64);//
	LCD_WR_DATA8(0x37);//
	//VSP
	LCD_WR_REG(0x65);//Pump1=4.7MHz //PUMP1 VSP
	LCD_WR_DATA8(0x09);//D6-5:pump1_clk[1:0] clamp 28 2b
	LCD_WR_DATA8(0x10);//6.26
	LCD_WR_DATA8(0x21);
	//VSN
	LCD_WR_REG(0x66); //pump=2 AVCL
	LCD_WR_DATA8(0x09); //clamp 08 0b 09
	LCD_WR_DATA8(0x10); //10
	LCD_WR_DATA8(0x21);
	//add source_neg_time
	LCD_WR_REG(0x67);//pump_sel
	LCD_WR_DATA8(0x20);//21 20
	LCD_WR_DATA8(0x40);

	//gamma vap/van
	LCD_WR_REG(0x68);//gamma vap/van
	LCD_WR_DATA8(0x90);//
	LCD_WR_DATA8(0x4c);//
	LCD_WR_DATA8(0x7C);//VCOM  
	LCD_WR_DATA8(0x66);//

	LCD_WR_REG(0xb1);//frame rate
	LCD_WR_DATA8(0x0F);//0x0f fr_h[5:0] 0F
	LCD_WR_DATA8(0x02);//0x02 fr_v[4:0] 02
	LCD_WR_DATA8(0x01);//0x04 fr_div[2:0] 04

	LCD_WR_REG(0xB4);
	LCD_WR_DATA8(0x01); //01:1dot 00:column
	////porch
	LCD_WR_REG(0xB5);
	LCD_WR_DATA8(0x02);//0x02 vfp[6:0]
	LCD_WR_DATA8(0x02);//0x02 vbp[6:0]
	LCD_WR_DATA8(0x0a);//0x0A hfp[6:0]
	LCD_WR_DATA8(0x14);//0x14 hbp[6:0]

	LCD_WR_REG(0xB6);
	LCD_WR_DATA8(0x04);//
	LCD_WR_DATA8(0x01);//
	LCD_WR_DATA8(0x9f);//
	LCD_WR_DATA8(0x00);//
	LCD_WR_DATA8(0x02);//
	////gamme sel
	LCD_WR_REG(0xdf);//
	LCD_WR_DATA8(0x11);//gofc_gamma_en_sel=1
	////gamma_test1 A1#_wangly
	//3030b_gamma_new_
	//GAMMA---------------------------------/////////////

	//GAMMA---------------------------------/////////////
	LCD_WR_REG(0xE2);	
	LCD_WR_DATA8(0x13);//vrp0[5:0]	V0 13
	LCD_WR_DATA8(0x00);//vrp1[5:0]	V1 
	LCD_WR_DATA8(0x00);//vrp2[5:0]	V2 
	LCD_WR_DATA8(0x30);//vrp3[5:0]	V61 
	LCD_WR_DATA8(0x33);//vrp4[5:0]	V62 
	LCD_WR_DATA8(0x3f);//vrp5[5:0]	V63

	LCD_WR_REG(0xE5);	
	LCD_WR_DATA8(0x3f);//vrn0[5:0]	V63
	LCD_WR_DATA8(0x33);//vrn1[5:0]	V62	
	LCD_WR_DATA8(0x30);//vrn2[5:0]	V61 
	LCD_WR_DATA8(0x00);//vrn3[5:0]	V2 
	LCD_WR_DATA8(0x00);//vrn4[5:0]	V1 
	LCD_WR_DATA8(0x13);//vrn5[5:0]  V0 13

	LCD_WR_REG(0xE1);	
	LCD_WR_DATA8(0x00);//prp0[6:0]	V15
	LCD_WR_DATA8(0x57);//prp1[6:0]	V51 

	LCD_WR_REG(0xE4);	
	LCD_WR_DATA8(0x58);//prn0[6:0]	V51 
	LCD_WR_DATA8(0x00);//prn1[6:0]  V15

	LCD_WR_REG(0xE0);
	LCD_WR_DATA8(0x01);//pkp0[4:0]	V3 
	LCD_WR_DATA8(0x03);//pkp1[4:0]	V7  
	LCD_WR_DATA8(0x0d);//pkp2[4:0]	V21
	LCD_WR_DATA8(0x0e);//pkp3[4:0]	V29 
	LCD_WR_DATA8(0x0e);//pkp4[4:0]	V37 
	LCD_WR_DATA8(0x0c);//pkp5[4:0]	V45 
	LCD_WR_DATA8(0x15);//pkp6[4:0]	V56 
	LCD_WR_DATA8(0x19);//pkp7[4:0]	V60 

	LCD_WR_REG(0xE3);	
	LCD_WR_DATA8(0x1a);//pkn0[4:0]	V60 
	LCD_WR_DATA8(0x16);//pkn1[4:0]	V56 
	LCD_WR_DATA8(0x0C);//pkn2[4:0]	V45 
	LCD_WR_DATA8(0x0f);//pkn3[4:0]	V37 
	LCD_WR_DATA8(0x0e);//pkn4[4:0]	V29 
	LCD_WR_DATA8(0x0d);//pkn5[4:0]	V21 
	LCD_WR_DATA8(0x02);//pkn6[4:0]	V7  
	LCD_WR_DATA8(0x01);//pkn7[4:0]	V3 
	//GAMMA---------------------------------/////////////

	//source
	LCD_WR_REG(0xE6);
	LCD_WR_DATA8(0x00);
	LCD_WR_DATA8(0xff);//SC_EN_START[7:0] f0

	LCD_WR_REG(0xE7);
	LCD_WR_DATA8(0x01);//CS_START[3:0] 01
	LCD_WR_DATA8(0x04);//scdt_inv_sel cs_vp_en
	LCD_WR_DATA8(0x03);//CS1_WIDTH[7:0] 12
	LCD_WR_DATA8(0x03);//CS2_WIDTH[7:0] 12
	LCD_WR_DATA8(0x00);//PREC_START[7:0] 06
	LCD_WR_DATA8(0x12);//PREC_WIDTH[7:0] 12

	LCD_WR_REG(0xE8); //source
	LCD_WR_DATA8(0x00); //VCMP_OUT_EN 81-
	LCD_WR_DATA8(0x70); //chopper_sel[6:4]
	LCD_WR_DATA8(0x00); //gchopper_sel[6:4] 60
	////gate
	LCD_WR_REG(0xEc);
	LCD_WR_DATA8(0x52);//52

	LCD_WR_REG(0xF1);
	LCD_WR_DATA8(0x01);//te_pol tem_extend 00 01 03
	LCD_WR_DATA8(0x01);
	LCD_WR_DATA8(0x02);


	LCD_WR_REG(0xF6);
	LCD_WR_DATA8(0x09);
	LCD_WR_DATA8(0x10);
	LCD_WR_DATA8(0x00);//
	LCD_WR_DATA8(0x00);//40 3线2通道

	LCD_WR_REG(0xfd);
	LCD_WR_DATA8(0xfa);
	LCD_WR_DATA8(0xfc);

	LCD_WR_REG(0x3a);
	LCD_WR_DATA8(0x05);//

	LCD_WR_REG(0x35);
	LCD_WR_DATA8(0x00);

	LCD_WR_REG(0x36);
	if(lcd_direction==0)LCD_WR_DATA8(0x08);
	else if(lcd_direction==1)LCD_WR_DATA8(0xC8);
	else if(lcd_direction==2)LCD_WR_DATA8(0x78);
	else LCD_WR_DATA8(0xA8);


	LCD_WR_REG(0x21); 

	LCD_WR_REG(0x11); // exit sleep
	HAL_Delay(200);
	LCD_WR_REG(0x29); // display on
	HAL_Delay(10); 
}

uint16_t LCD_ReadScanLine(void)
{
    while (hspi1.State != HAL_SPI_STATE_READY)
        ; // 等待SPI空闲

    __HAL_SPI_DISABLE(&hspi1);
    hspi1.Instance->CR1 &= ~(SPI_CR1_RXONLY | SPI_CR1_BIDIMODE);
    hspi1.Instance->CR1 |= SPI_CR1_BIDIMODE;
	
    hspi1.Instance->CR1 &= ~SPI_CR1_BR;
    hspi1.Instance->CR1 |= SPI_BAUDRATEPRESCALER_16;
    __HAL_SPI_ENABLE(&hspi1);
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_16;

    LCD_CS_OUT(0);
    LCD_DC_OUT(0);

    uint8_t cmd = 0x45;
    HAL_SPI_Transmit(&hspi1, &cmd, 1, 100);

    LCD_DC_OUT(1);

    __HAL_SPI_DISABLE(&hspi1);
    SPI_1LINE_RX(&hspi1);
    __HAL_SPI_ENABLE(&hspi1);

    uint8_t data[3] = {0};
    int i = 0;
    while(i < 2)
    {
        if (hspi1.Instance->SR & SPI_FLAG_RXNE)
            data[i++] = *(__IO uint8_t *)&hspi1.Instance->DR;
    }
    
    __DSB();
    __HAL_SPI_DISABLE(&hspi1);

    while ((hspi1.Instance->SR & SPI_FLAG_RXNE) != SPI_FLAG_RXNE);
    /* read the received data */
    data[2] = *(__IO uint8_t *)&hspi1.Instance->DR;
    while ((hspi1.Instance->SR & SPI_FLAG_BSY) == SPI_FLAG_BSY);

    LCD_CS_OUT(1);

    __HAL_SPI_DISABLE(&hspi1);
    hspi1.Instance->CR1 &= ~SPI_CR1_BR;
    hspi1.Instance->CR1 |= SPI_BAUDRATEPRESCALER_2;
    SPI_1LINE_TX(&hspi1);
    hspi1.Instance->CR1 &= ~(SPI_CR1_RXONLY | SPI_CR1_BIDIMODE);
    __HAL_SPI_ENABLE(&hspi1);
    hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;

    return data[1] << 1 | !!data[2];
}

void LCD_Fill(uint16_t xsta, uint16_t ysta, uint16_t xend, uint16_t yend, uint16_t color)
{
    while (hspi1.State != HAL_SPI_STATE_READY)
        ; // 等待SPI空闲

    static uint16_t color1[1];
    uint16_t num;
    color1[0] = color;
    num = (xend - xsta) * (yend - ysta);
    LCD_Address_Set(xsta, ysta, xend - 1, yend - 1); // 设置显示范围
    LCD_CS_OUT(0);

    LCD_DMA_Transfer16Bit((uint8_t *)color1, num*2, DMA_MEMINC_DISABLE); // 启用DMA发送

    // 其余部分见HAL_SPI_TxCpltCallback()函数
}


// 把指定区域的显示缓冲区写入屏幕
void LCD_Color_Fill(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t *buf)
{
    while (hspi1.State != HAL_SPI_STATE_READY)
        ; // 等待SPI空闲
		
    long num;
    num = (x1 - x0) * (y1 - y0)*2;
    LCD_Address_Set(x0, y0, x1 - 1, y1 - 1);
    LCD_CS_OUT(0);
		
    LCD_DMA_Transfer16Bit((uint8_t *)buf, num, DMA_MEMINC_ENABLE); // 启用DMA发送

    // 其余部分见HAL_SPI_TxCpltCallback()函数
}

// SPI传输完成回调函数
// 此函数会在DMA SPITX传输完成后被调用
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef *hspi)
{
//	LCD_CS_OUT(1);
}
   