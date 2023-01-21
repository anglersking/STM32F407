/**
  ******************************************************************************
  * 文件名程: main.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2019-09-03
  * 功    能: HALL传感器接口-API
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-H7Multi使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */

#ifndef __BSP_HALL_H_
#define __BSP_HALL_H_

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/

/* 宏定义 --------------------------------------------------------------------*/

/**************** 定时器霍尔接口TIM参数定义，TIMx CH1,CH2,CH3 *****************/

/* 注意:更换 "霍尔接口定时器"的时候需要修改 "PWM输出定时器(TIM1/TIM8)" 的触发源  */

#define HALL_TIMx                        TIM5
#define HALL_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM5_CLK_ENABLE()

#define HALL_TIM_GPIO_AF                 GPIO_AF2_TIM5
#define HALL_TIM_GPIO_RCC_CLK_ENABLE()   __HAL_RCC_GPIOH_CLK_ENABLE()

#define HALL_TIM_CH1_PORT                GPIOA          // CH1的引脚
#define HALL_TIM_CH1_PIN                 GPIO_PIN_0
#define HALL_TIM_CH1                     TIM_CHANNEL_1

#define HALL_TIM_CH2_PORT                GPIOA         // CH2的引脚
#define HALL_TIM_CH2_PIN                 GPIO_PIN_1
#define HALL_TIM_CH2                     TIM_CHANNEL_2

#define HALL_TIM_CH3_PORT                GPIOA          // CH3的引脚
#define HALL_TIM_CH3_PIN                 GPIO_PIN_2
#define HALL_TIM_CH3                     TIM_CHANNEL_3

#define HALL_TIM_IRQn                    TIM5_IRQn
#define HALL_TIM_IRQHanler               TIM5_IRQHandler


// 定义定时器预分频，定时器实际时钟频率为：84MHz/（HALL_TIMx_PRESCALER+1）
#define HALL_TIM_PRESCALER               83  // 实际时钟频率为：1MHz

#define HALL_TIM_FREQ                (84e6/(HALL_TIM_PRESCALER+1)) // 定时器计数频率

// 定义定时器周期，当定时器开始计数到HALL_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define HALL_TIM_PERIOD                  0xFFFF //

#define POLE_PAIRES                      4 // 无刷电机4对极
#define PPR                             (POLE_PAIRES*2*3) // (4*2*3) 脉冲每转,

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_hall;
extern __IO uint32_t RT_hallPhase; // 霍尔信号
extern __IO uint32_t RT_hallcomp;  // 霍尔捕获计数值
extern __IO uint32_t RT_hallcnt ; // 霍尔计数值

/* 函数声明 ------------------------------------------------------------------*/
void HALLSensor_TIMx_Init(void);
int32_t HALL_GetPhase(void);

#endif /* __BSP_HALL_H_ */
/********** (C) COPYRIGHT 2019-2030 硬石嵌入式开发团队 *******END OF FILE******/
