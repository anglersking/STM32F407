

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//
//  �� �� ��   : main.c
//  �� �� ��   : v2.0
//  ��������   : 2018-1111
//  ����޸�   : 
//  ��������   : 0.96��ISP LCD 4�ӿ���ʾ����(STM32ϵ��)
/******************************************************************************
//������������STM32F103C8
//              GND   ��Դ��
//              VCC   ��5V��3.3v��Դ
//              SCL   ��PA5��SCL��
//              SDA   ��PA7��SDA��
//              RES   ��PB0
//              DC    ��PB1
//              CS    ��PA4//����ֱ�ӽӵ�
//              BLK    ��P10 BLK�������ղ��ӣ�����Ҫ�رձ����ʱ��BLK�õ͵�ƽ
*******************************************************************************/
// �޸���ʷ   :
// ��    ��   : 
// �޸�����   : 
//******************************************************************************/
#ifndef __LCD_DIRIVER_H__
#define __LCD_DIRIVER_H__
#include "main.h"
#include "spi.h"
#include "gpio.h"
#define RED  	0xf800
#define GREEN	0x07e0
#define BLUE 	0x001f
#define WHITE	0xffff
#define BLACK	0x0000
#define YELLOW  0xFFE0
#define GRAY0   0xEF7D   	//��ɫ0 3165 00110 001011 00101
#define GRAY1   0x8410      	//��ɫ1      00000 000000 00000
#define GRAY2   0x4208      	//��ɫ2  1111111111011111




#define LCD_CTRLA   	  	GPIOA		//����TFT���ݶ˿�
#define LCD_CTRLB   	  	GPIOB		//����TFT���ݶ˿�



#define LCD_SCL        	GPIO_PIN_5	//PB13--->>TFT --SCL/SCK
#define LCD_SDA        	GPIO_PIN_7	//PB15 MOSI--->>TFT --SDA/DIN
#define LCD_CS     	GPIO_PIN_10	//PB10--->>TFT --RST

#define LCD_BL       	GPIO_PIN_12  //MCU_PB9--->>TFT --BL
#define LCD_DC         	GPIO_PIN_11	//PC12--->>TFT --RS/DC
#define LCD_RST     	GPIO_PIN_12	//PB10--->>TFT --RST

//#define LCD_CS_SET(x) LCD_CTRL->ODR=(LCD_CTRL->ODR&~LCD_CS)|(x ? LCD_CS:0)

//Һ�����ƿ���1�������궨��
#define	LCD_SCL_SET  	  HAL_GPIO_WritePin(GPIOA, LCD_SCL,GPIO_PIN_SET)    
#define	LCD_SDA_SET  	  HAL_GPIO_WritePin(GPIOA, LCD_SDA,GPIO_PIN_SET)    

    
#define	LCD_CS_SET  	 HAL_GPIO_WritePin(GPIOC, LCD_CS,GPIO_PIN_SET)   
#define	LCD_DC_SET  	 HAL_GPIO_WritePin(GPIOC, LCD_DC,GPIO_PIN_SET) 
#define	LCD_BL_SET  	 HAL_GPIO_WritePin(GPIOC, LCD_BL,GPIO_PIN_SET) 
#define	LCD_RST_SET  	 HAL_GPIO_WritePin(GPIOC, LCD_RST,GPIO_PIN_SET) 
//Һ�����ƿ���0�������궨��
#define	LCD_SCL_CLR  	 HAL_GPIO_WritePin(GPIOA, LCD_SCL,GPIO_PIN_RESET) 
#define	LCD_SDA_CLR  	 HAL_GPIO_WritePin(GPIOA, LCD_SDA,GPIO_PIN_RESET) 
#define	LCD_CS_CLR  	 HAL_GPIO_WritePin(GPIOC, LCD_CS,GPIO_PIN_RESET)
    
#define	LCD_RST_CLR  	 HAL_GPIO_WritePin(GPIOC, LCD_RST,GPIO_PIN_RESET)
#define	LCD_DC_CLR  	 HAL_GPIO_WritePin(GPIOC, LCD_DC,GPIO_PIN_RESET)
#define	LCD_BL_CLR  	 HAL_GPIO_WritePin(GPIOC, LCD_BL,GPIO_PIN_RESET)



void LCD_GPIO_Init(void);
void Lcd_WriteIndex(uint8_t Index);
void Lcd_WriteData(uint8_t Data);
void Lcd_WriteReg(uint8_t Index,uint8_t Data);
uint16_t Lcd_ReadReg(uint8_t LCD_Reg);
void Lcd_Reset(void);
void Lcd_Init(void);
void Lcd_Clear(uint16_t Color);
void Lcd_SetXY(uint16_t x,uint16_t y);
void Gui_DrawPoint(uint16_t x,uint16_t y,uint16_t Data);
unsigned int Lcd_ReadPoint(uint16_t x,uint16_t y);
void Lcd_SetRegion(uint16_t x_start,uint16_t y_start,uint16_t x_end,uint16_t y_end);
void LCD_WriteData_16Bit(uint16_t Data);

#endif /*__ GPIO_H__ */
