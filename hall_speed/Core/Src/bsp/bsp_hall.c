/**
  ******************************************************************************
  * 文件名程: main.c
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2020-07-21
  * 功    能: hall传感器接口基础驱动函数
  ******************************************************************************
  * 说明：
  * 本例程配套硬石stm32开发板YS-F4Pro使用。
  *
  * 淘宝：
  * 论坛：http://www.ing10bbs.com
  * 版权归硬石嵌入式开发团队所有，请勿商用。
  ******************************************************************************
  */

/* 包含头文件 ----------------------------------------------------------------*/
#include "bsp_hall.h"
#include "bsp_bldc.h"

/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx_hall;

__IO uint32_t RT_hallPhase = 0; // 霍尔信号相位 RT-> Real Time
uint32_t LS_hallPhase = 0; // 上一次的霍尔信号相位

__IO uint32_t RT_hallcomp = 0;  // 霍尔计数值
__IO uint32_t RT_hallcnt = 0;   // 霍尔计数值

MotorDir_Typedef RT_hallDir = MOTOR_DIR_CW; // 霍尔顺序得到的电机转动方向

/* 霍尔信号顺序,用于确定电机转动方向 
 * 下标是当前的霍尔序列,该下标对应的元素值是上一次的霍尔序列
 * 只要记录上一次的霍尔值,然后对比数据即可知道当前电机实际转动方向
 */
#ifndef USE_PMSMMOTOR 
const uint8_t HallDirCcw [7] = {0, 3, 6, 2, 5, 1, 4};   // 0 无意义
#else 
const uint8_t HallDirCcw [7] = {0, 5, 3, 1, 6, 4, 2};    // PMSM 的逆时针旋转序列
#endif

/* 扩展变量 ------------------------------------------------------------------*/

/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/


/**
  * 函数功能: 定时器HALL接口引脚初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void YS_TIM_HALL_Base_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  HALL_TIM_GPIO_RCC_CLK_ENABLE();
  GPIO_InitStruct.Pin       = HALL_TIM_CH1_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = HALL_TIM_GPIO_AF;
  GPIO_InitStruct.Pull      = GPIO_PULLUP;
  HAL_GPIO_Init(HALL_TIM_CH1_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin       = HALL_TIM_CH2_PIN;
  HAL_GPIO_Init(HALL_TIM_CH2_PORT, &GPIO_InitStruct);
  GPIO_InitStruct.Pin       = HALL_TIM_CH3_PIN;
  HAL_GPIO_Init(HALL_TIM_CH3_PORT, &GPIO_InitStruct);
  HAL_NVIC_SetPriority(HALL_TIM_IRQn, 1, 1);
  HAL_NVIC_EnableIRQ(HALL_TIM_IRQn);
}

/**
  * 函数功能: 定时器霍尔传感器接口初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void HALLSensor_TIMx_Init(void)
{
  TIM_HallSensor_InitTypeDef sHallConfig = {0};
  /* 通用定时器外设时钟使能 */
  HALL_TIM_RCC_CLK_ENABLE();
  /* 初始化板载设备，定时器通道引脚，使能时钟 */
  YS_TIM_HALL_Base_MspInit();
  //
  /* 配置定时器基础计数功能 */
  htimx_hall.Instance           = HALL_TIMx;          // 定时器TIM
  htimx_hall.Init.Prescaler     = HALL_TIM_PRESCALER; // PSC设置预分频值
  htimx_hall.Init.CounterMode   = TIM_COUNTERMODE_CENTERALIGNED1;// 中心对齐模式
  htimx_hall.Init.Period        = HALL_TIM_PERIOD;    // 计数周期
  htimx_hall.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  sHallConfig.Commutation_Delay = 0x06; // 延迟触发,7us,实际测试需要7us
  sHallConfig.IC1Filter         = 0x0F; // 输入滤波;
  sHallConfig.IC1Polarity       = TIM_ICPOLARITY_RISING;// 输入捕获极性//不起作用
  sHallConfig.IC1Prescaler      = TIM_ICPSC_DIV1;       // 输入捕获预分频
  HAL_TIMEx_HallSensor_Init(&htimx_hall, &sHallConfig);
}
/**
  * 函数功能: 读取霍尔引脚状态
  * 输入参数: 无
  * 返 回 值: 霍尔引脚状态
  * 说    明: 直接读取引脚的状态,数据字节的低三位分别对应UVW(HALL)的电平状态
  */
int32_t HALL_GetPhase()
{
  int32_t tmp = 0;
  tmp |= HAL_GPIO_ReadPin(HALL_TIM_CH1_PORT, HALL_TIM_CH1_PIN);//U(A)
  tmp <<= 1;
  tmp |= HAL_GPIO_ReadPin(HALL_TIM_CH2_PORT, HALL_TIM_CH2_PIN);//V(B)
  tmp <<= 1;
  tmp |= HAL_GPIO_ReadPin(HALL_TIM_CH3_PORT, HALL_TIM_CH3_PIN);//W(C)
  return (tmp & 0x0007); // 取低三位
}

/**
  * 函数功能: 霍尔传感器回调函数
  * 输入参数: @htim,霍尔传感器接口定时器
  * 返 回 值: 无
  * 说    明:
  */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
  int32_t RT_hallPhase = 0; // 霍尔信号
  RT_hallPhase = HALL_GetPhase();     // 获取霍尔引脚的相位
  /* 换相控制 */
  BLDCMotor_PhaseCtrl(RT_hallPhase);
  
  /* 获取两次信号的计数间隔 *//* 假定不会溢出 */
  RT_hallcomp += __HAL_TIM_GET_COMPARE(htim,HALL_TIM_CH1);
  RT_hallcnt++;
  /* 判断方向 */
  if(HallDirCcw[RT_hallPhase] == LS_hallPhase) // 序列与表中的一致
  {
    RT_hallDir = MOTOR_DIR_CCW; 
  }
  else
    RT_hallDir = MOTOR_DIR_CW;
  LS_hallPhase = RT_hallPhase; // 记录这一个的霍尔值
}

/********** (C) COPYRIGHT 2019-2030 硬石嵌入式开发团队 *******END OF FILE******/
