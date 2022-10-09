#ifndef __OLED_H
#define __OLED_H 

#include "main.h"
#include "stdlib.h"	

//-----------------测试LED端口定义---------------- 

//#define LED_ON HAL_GPIO_WritePin(GPIOC,GPIO_Pin_12,GPIO_PIN_RESET);
//#define LED_ON HAL_GPIO_WritePin(GPIOC,GPIO_Pin_12,GPIO_PIN_RESET);

//-----------------OLED端口定义---------------- 

#define OLED_PORT GPIOB

#define OLED_SCL_Clr() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_RESET);//SCL
#define OLED_SCL_Set() HAL_GPIO_WritePin(GPIOB,GPIO_PIN_10,GPIO_PIN_SET);//SCL

#define OLED_SDA_Clr() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_RESET);//SDA
#define OLED_SDA_Set() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_15,GPIO_PIN_SET);//SDA

#define OLED_RES_Clr() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_RESET);//RES
#define OLED_RES_Set() HAL_GPIO_WritePin(GPIOE,GPIO_PIN_14,GPIO_PIN_SET);//RES

#define OLED_DC_Clr()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_RESET);//DC
#define OLED_DC_Set()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_13,GPIO_PIN_SET);//DC
 		     
#define OLED_CS_Clr()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_RESET);//CS
#define OLED_CS_Set()  HAL_GPIO_WritePin(GPIOE,GPIO_PIN_12,GPIO_PIN_SET);//CS


#define OLED_CMD  0	//写命令
#define OLED_DATA 1	//写数据

void OLED_ClearPoint(uint8_t x,uint8_t y);
void OLED_ColorTurn(uint8_t i);
void OLED_DisplayTurn(uint8_t i);
void OLED_WR_Byte(uint8_t dat,uint8_t mode);
void OLED_DisPlay_On(void);
void OLED_DisPlay_Off(void);
void OLED_Refresh(void);
void OLED_Clear(void);
void OLED_DrawPoint(uint8_t x,uint8_t y,uint8_t t);
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2,uint8_t mode);
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r);
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1,uint8_t mode);
void OLED_ShowChar6x8(uint8_t x,uint8_t y,uint8_t chr,uint8_t mode);
void OLED_ShowString(uint8_t x,uint8_t y,uint8_t *chr,uint8_t size1,uint8_t mode);
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1,uint8_t mode);
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1,uint8_t mode);
void OLED_ScrollDisplay(uint8_t num,uint8_t space,uint8_t mode);
void OLED_ShowPicture(uint8_t x,uint8_t y,uint8_t sizex,uint8_t sizey,uint8_t BMP[],uint8_t mode);
void OLED_Init(void);

#endif

