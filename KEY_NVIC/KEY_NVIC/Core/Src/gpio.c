/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    gpio.c
  * @brief   This file provides code for the configuration
  *          of all used GPIO pins.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "gpio.h"

/* USER CODE BEGIN 0 */
#include "usart.h"
/* USER CODE END 0 */

/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/** Configure pins as
        * Analog
        * Input
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pins : PE3 PE4 */
  GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pin : PA6 */
  GPIO_InitStruct.Pin = GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI3_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(EXTI3_IRQn);

  HAL_NVIC_SetPriority(EXTI4_IRQn, 1, 2);
  HAL_NVIC_EnableIRQ(EXTI4_IRQn);

}

/* USER CODE BEGIN 2 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	HAL_UART_Transmit(&huart1,(uint8_t *)"NVIC_IIN", 8, 100);
	if(GPIO_Pin==GPIO_PIN_4)
	 {
			 HAL_Delay(20);
			 if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_4)==GPIO_PIN_RESET)
			 {
				HAL_UART_Transmit(&huart1,(uint8_t *)"NVIC_4IN", 8, 100);
				
				GPIOA->BSRR=(1<<22);//?? ????HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
				HAL_Delay(1000);
			
			}
		
			 HAL_UART_Transmit(&huart1,(uint8_t *)"NVIC_over", 9, 100);
			 GPIOA->BSRR=(1<<6);//??  ????HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10,GPIO_PIN_SET);
		}
	 
		if(GPIO_Pin==GPIO_PIN_3)
	 {
			 HAL_Delay(20);
			 if (HAL_GPIO_ReadPin(GPIOE, GPIO_PIN_3)==GPIO_PIN_RESET)
			 {
				HAL_UART_Transmit(&huart1,(uint8_t *)"NVIC_3IN", 8, 100);
				
				GPIOA->BSRR=(1<<22);//?? ????HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6,GPIO_PIN_RESET);
				HAL_Delay(1000);
			
			}
		
			 HAL_UART_Transmit(&huart1,(uint8_t *)"NVIC_over", 9, 100);
			 GPIOA->BSRR=(1<<6);//??  ????HAL_GPIO_WritePin(GPIOA,GPIO_PIN_6|GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10,GPIO_PIN_SET);
		}
}
/* USER CODE END 2 */
