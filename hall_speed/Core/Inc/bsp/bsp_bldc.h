/**
  ******************************************************************************
  * 文件名程: bsp_GeneralTIM.h 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2020-07-21
  * 功    能: TIM-API
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F4Pro使用。
  * 
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */
#ifndef __BLDCMOTOR_TIM_H__
#define __BLDCMOTOR_TIM_H__

/* 包含头文件 ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* 类型定义 ------------------------------------------------------------------*/
/* 电机方向定义 */
typedef enum 
{
  MOTOR_DIR_CW = 0, // 顺时针转动
  MOTOR_DIR_CCW     // 逆时针转动
}MotorDir_Typedef;  // 方向定义

/* 电机使能定义 */
typedef enum 
{
  MOTOR_ENABLE  = 0,
  MOTOR_DISABLE
}MotorSta_Typedef;

/* 宏定义 --------------------------------------------------------------------*/
#define USE_PMSMMOTOR
/******************** 通用定时器TIM参数定义，TIM8 CH1,CH2,CH3 *****************/

#define BLDCMOTOR_TIMx                        TIM8
#define BLDCMOTOR_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM8_CLK_ENABLE()

#define BLDCMOTOR_TIM_GPIO_AF                 GPIO_AF3_TIM8
#define BLDCMOTOR_TIM_GPIO_RCC_CLK_ENABLE()   {__HAL_RCC_GPIOI_CLK_ENABLE();\
                                              __HAL_RCC_GPIOH_CLK_ENABLE();}

#define BLDCMOTOR_TIM_CH1_PORT                GPIOI          // TIM8_CH1的引脚
#define BLDCMOTOR_TIM_CH1_PIN                 GPIO_PIN_5
#define BLDCMOTOR_TIM_CH1                     TIM_CHANNEL_1

#define BLDCMOTOR_TIM_CH2_PORT                GPIOI          // TIM8_CH2的引脚
#define BLDCMOTOR_TIM_CH2_PIN                 GPIO_PIN_6
#define BLDCMOTOR_TIM_CH2                     TIM_CHANNEL_2

#define BLDCMOTOR_TIM_CH3_PORT                GPIOI          // TIM8_CH3的引脚
#define BLDCMOTOR_TIM_CH3_PIN                 GPIO_PIN_7
#define BLDCMOTOR_TIM_CH3                     TIM_CHANNEL_3

/* 三个互补通道 */
#define BLDCMOTOR_TIM_CH1N_PORT               GPIOH          // TIM8_CH1N的引脚
#define BLDCMOTOR_TIM_CH1N_PIN                GPIO_PIN_13

#define BLDCMOTOR_TIM_CH2N_PORT               GPIOH          // TIM8_CH2N的引脚
#define BLDCMOTOR_TIM_CH2N_PIN                GPIO_PIN_14

#define BLDCMOTOR_TIM_CH3N_PORT               GPIOH          // TIM8_CH3N的引脚
#define BLDCMOTOR_TIM_CH3N_PIN                GPIO_PIN_15

/* short down 控制 */
#define BLDCMOTOR_SHORTDOWN_RCC_CLK_ENABLE()  __HAL_RCC_GPIOH_CLK_ENABLE()
#define BLDCMOTOR_SHORTDOWN_PORT              GPIOH
#define BLDCMOTOR_SHORTDOWN_PIN               GPIO_PIN_9

#define BLDCMOTOR_ENABLE()                   HAL_GPIO_WritePin(BLDCMOTOR_SHORTDOWN_PORT,BLDCMOTOR_SHORTDOWN_PIN,GPIO_PIN_SET)
#define BLDCMOTOR_DISABLE()                  HAL_GPIO_WritePin(BLDCMOTOR_SHORTDOWN_PORT,BLDCMOTOR_SHORTDOWN_PIN,GPIO_PIN_RESET)

/** 触发源选择,TIM1作为从定时器,可以连接到TIM3,TIM4的TRGO,由主定时器触发com事件
  * TIM5   TIM_TS_ITR3
  * 选择TIM2的时候,触发源为TIR1，选择TIM4则触发源是TIM_TS_ITR2
  */
#define BLDMOTOR_TIM_TS                       TIM_TS_ITR3 // 定时器触发源,ITR3  TIM5->TIM8

// 定义定时器预分频，定时器实际时钟频率为：168MHz/（BLDCMOTOR_TIMx_PRESCALER+1）
#define BLDCMOTOR_TIM_PRESCALER            0  // 实际时钟频率为：168MHz

// 定义定时器周期，当定时器开始计数到BLDCMOTOR_TIMx_PERIOD值是更新定时器并生成对应事件和中断
#define BLDCMOTOR_TIM_PERIOD               4200  // 定时器更新频率为：168MHz/(8400)=20KHz，即50us定时周期

// 定时器通道1初始占空比，实际并没有意义，在代码中实时修改了
#define BLDCMOTOR_TIM_CH1_PULSE            0   // 初始占空比

/* 扩展变量 ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_BLDCM;
extern MotorSta_Typedef Motor_State ; // 电机使能状态
extern MotorDir_Typedef Motor_Dir  ;  // 电机方向 

/* 函数声明 ------------------------------------------------------------------*/
void BLDCMOTOR_TIMx_Init(void);
void BLDCMotor_Start(void);
void BLDCM_Inertia_brake(void);
void BLDCMotor_braking_LowerShort(void);
void BLDCMotor_unbraking_LS(void);

void BLDCMotor_SetSpeed(float speed);

void BLDCMotor_PhaseCtrl(int32_t HALLPhase );

#endif	/* __BLDCMOTOR_TIM_H__ */
/************** (C) COPYRIGHT 2019-2030 硬石嵌入式开发团队 *****END OF FILE****/


