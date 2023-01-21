/**
  ******************************************************************************
  * 文件名程: bsp_GeneralTIM.c 
  * 作    者: 硬石嵌入式开发团队
  * 版    本: V1.0
  * 编写日期: 2020-07-21
  * 功    能: TIM比较输出驱动程序
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
#include "bsp_bldc.h" 
#include "bsp_hall.h"

/* 私有类型定义 --------------------------------------------------------------*/

/* 私有宏定义 ----------------------------------------------------------------*/

/* 私有变量 ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx_BLDCM;

/* 扩展变量 ------------------------------------------------------------------*/
MotorSta_Typedef Motor_State = MOTOR_DISABLE; // 电机使能状态
MotorDir_Typedef Motor_Dir   = MOTOR_DIR_CCW;  // 电机方向 ,顺时针
float PWM_Duty   = 0.25f;        // 25%占空比

/* 私有函数原形 --------------------------------------------------------------*/

/* 函数体 --------------------------------------------------------------------*/

/**
  * 函数功能: 通用定时器硬件初始化配置
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void YS_TIM_Base_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* 定时器通道功能引脚端口时钟使能 */
  BLDCMOTOR_TIM_GPIO_RCC_CLK_ENABLE();
  
  /* 定时器通道1功能引脚IO初始化 */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH1_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = BLDCMOTOR_TIM_GPIO_AF;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH1_PORT, &GPIO_InitStruct);
  
  /* 定时器通道2功能引脚IO初始化 */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH2_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = BLDCMOTOR_TIM_GPIO_AF;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH2_PORT, &GPIO_InitStruct);
  
  /* 定时器通道3功能引脚IO初始化 */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH3_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = BLDCMOTOR_TIM_GPIO_AF;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3_PORT, &GPIO_InitStruct);
  
  /* 定时器互补通道1功能引脚IO初始化 */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH1N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3N_PORT, &GPIO_InitStruct);
  
  /* 定时器互补通道1功能引脚IO初始化 */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH2N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3N_PORT, &GPIO_InitStruct);
  
  /* 定时器互补通道1功能引脚IO初始化 */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH3N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3N_PORT, &GPIO_InitStruct);
  
  /* shortdown控制引脚初始化 */
  BLDCMOTOR_SHORTDOWN_RCC_CLK_ENABLE();
  BLDCMOTOR_ENABLE(); // 将引脚设置为有效电平,是PWM能控制电机
  GPIO_InitStruct.Pin       = BLDCMOTOR_SHORTDOWN_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = 0;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(BLDCMOTOR_SHORTDOWN_PORT, &GPIO_InitStruct);
  
  BLDCMOTOR_ENABLE(); // 将引脚设置为有效电平,使PWM能控制电机
}

/**
  * 函数功能: 高级定时器初始化
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 无
  */
void BLDCMOTOR_TIMx_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* 通用定时器外设时钟使能 */
  BLDCMOTOR_TIM_RCC_CLK_ENABLE();
  /* 初始化板载设备，定时器通道引脚，使能时钟 */ 
  YS_TIM_Base_MspInit();
  
  /* 配置定时器基础计数功能 */
  htimx_BLDCM.Instance           = BLDCMOTOR_TIMx;          // 定时器TIM
  htimx_BLDCM.Init.Prescaler     = BLDCMOTOR_TIM_PRESCALER; // PSC设置预分频值
  htimx_BLDCM.Init.CounterMode   = TIM_COUNTERMODE_CENTERALIGNED1;// 中心对齐模式
  htimx_BLDCM.Init.Period        = BLDCMOTOR_TIM_PERIOD;    // 计数周期
  htimx_BLDCM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htimx_BLDCM);

    /* 死区和刹车配置 */
  sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_ENABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime         = 0x22; // 大约200ns的死区时间
  sBreakDeadTimeConfig.BreakState       = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htimx_BLDCM, &sBreakDeadTimeConfig);

  /* 配置比较输出为PWM1模式 */
  sConfigOC.OCMode       = TIM_OCMODE_PWM1;        // PWM1 模式 
  sConfigOC.Pulse        = BLDCMOTOR_TIM_CH1_PULSE;// CHx的输出脉冲宽度
  sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;    // 输出极性
  sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;   // 互补通道输出极性
  sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;  // 空闲状态极性
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET; // 互补空闲状态极性
  sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;     // 快速输出模式，禁止  
  
  /* 这里使用的是HAL_TIM_OC_ConfigChannel,而不是HAL_TIM_PWM_ConfigChannel
   * 如果使用HAL_TIM_PWM_ConfigChannel会使能比较器的预装载功能,也就是修改比较值
   * 之后不是立即生效,这将会造成换相之后的第一个脉冲脉宽不固定
   * 这两个函数实际的配置几乎完全一样,只是HAL_TIM_OC_ConfigChannel不会去配置
   * OCFastmode和比较器的预装载使能.
   * 
   */
  HAL_TIM_OC_ConfigChannel(&htimx_BLDCM, &sConfigOC, BLDCMOTOR_TIM_CH1);
  /* CH2 CH2N*/
  HAL_TIM_OC_ConfigChannel(&htimx_BLDCM, &sConfigOC, BLDCMOTOR_TIM_CH2);
  /* CH3 CH3N */
  HAL_TIM_OC_ConfigChannel(&htimx_BLDCM, &sConfigOC, BLDCMOTOR_TIM_CH3);
  
  /* 配置COM事件(commutation)
   * 由主定时器的TRGO触发TIM1的COM事件
   * 当触发COM事件的时候将会统一更新定时器通道输出的使能控制位,避免由于程序执行
   * 的延迟导致换相时刻下桥臂出现负窄脉冲.
   */
  HAL_TIMEx_ConfigCommutationEvent(&htimx_BLDCM, BLDMOTOR_TIM_TS, TIM_COMMUTATION_TRGI);
}


/**
  * 函数功能: 无刷电机相位控制
  * 输入参数: @HALLPhase 霍尔信号相位
  * 返 回 值: 无
  * 说    明: 控制定时器输出PWM波形换相,定义定时器输出通道CH1为A相(U) 
  *           CH2为B相(V)，CH3为C相(W),配置下一次霍尔换相的时候的波形
  */
void BLDCMotor_PhaseCtrl(int32_t HALLPhase )
{
#ifndef USE_PMSMMOTOR  
  /* 顺时针的霍尔顺序与逆时针的顺序是关于7互补,所以可以使用7减逆时针的顺序得到
   * 顺时针的霍尔顺序,这里使用异或代替减法
   */
  if(MOTOR_DIR_CW == Motor_Dir)
    HALLPhase = 0x07 ^ HALLPhase;// 将低三位异或 111b ^ 010b -> 101b
#else  
  /* PMSM的旋转顺序跟BLDC刚好是相反的,设定为MOTOR_DIR_CCW的时候BLDC是逆时针旋转
   * PMSM是顺时针旋转,如果需要设定为MOTOR_DIR_CCW的时候PMSM为逆时针旋转,则使用
   * 下面语句代替上面的语句
   */
  if(MOTOR_DIR_CCW == Motor_Dir)
    HALLPhase = 0x07 ^ HALLPhase;// 将低三位异或 111b ^ 010b -> 101b
#endif
  switch(HALLPhase)
  {
    /* 定义电机的U(A),V(B),W(C)三相分别对应是CH1,CH2,CH3;
     *  A+,A-分别表示CH1控制的上,下桥臂导通
     */
    case 5: //B+  A-
    {
      /*  Channe3 configuration */ 
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
    
      /*  Channe2 configuration  */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM, BLDCMOTOR_TIM_CH2,BLDCMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);  
      
      /*  Channe1 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM, BLDCMOTOR_TIM_CH1,BLDCMOTOR_TIM_PERIOD +1);
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
      HAL_TIMEx_PWMN_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
    }
    break;
    
    case 4:// C+ A-
    {
      /*  Channe2 configuration */ 
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
 
      /*  Channe3 configuration  */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM, BLDCMOTOR_TIM_CH3,BLDCMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);  
      
      /*  Channe1 configuration  */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM,BLDCMOTOR_TIM_CH1,BLDCMOTOR_TIM_PERIOD +1);
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
      HAL_TIMEx_PWMN_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
    }
      break;
    
    case 6://C+ B-
    {
      /*  Channe1 configuration  */ 
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
    
      /*  Channe3 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM,BLDCMOTOR_TIM_CH3,BLDCMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);  

      /*  Channe2 configuration  */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM,BLDCMOTOR_TIM_CH2,BLDCMOTOR_TIM_PERIOD +1);
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
      HAL_TIMEx_PWMN_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
    }
      break;

    case 2: // A+ B-
    {
      /*  Channe3 configuration */       
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
    
      /*  Channe1 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM,BLDCMOTOR_TIM_CH1,BLDCMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);  
      
      /*  Channe2 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM,BLDCMOTOR_TIM_CH2,BLDCMOTOR_TIM_PERIOD +1);
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
      HAL_TIMEx_PWMN_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
    }
      break;
    
    
    case 3:// A+ C-
    {
      /*  Channe2 configuration */ 
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);

      /*  Channe1 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM,BLDCMOTOR_TIM_CH1,BLDCMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);  
      
      /*  Channe3 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM,BLDCMOTOR_TIM_CH3,BLDCMOTOR_TIM_PERIOD +1);    
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
      HAL_TIMEx_PWMN_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
    }
      break;
    case 1: // B+ C-
    {
      /*  Channe1 configuration */ 
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
    
      /*  Channe2 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM,BLDCMOTOR_TIM_CH2, BLDCMOTOR_TIM_PERIOD * PWM_Duty);
      HAL_TIM_PWM_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
      HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);  
      
      /*  Channe3 configuration */
      __HAL_TIM_SET_COMPARE(&htimx_BLDCM, BLDCMOTOR_TIM_CH3, BLDCMOTOR_TIM_PERIOD +1);
      HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
      HAL_TIMEx_PWMN_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
    }
    break;
  }
}
/**
  * 函数功能: 启动电机
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 启动无刷电机
  */
void  BLDCMotor_Start()
{
  int32_t hallPhase = 0;

  BLDCMOTOR_ENABLE();// 使能允许输出PWM
  
  /* 下桥臂导通,给自举电容充电 */
  BLDCMotor_braking_LowerShort();
  HAL_Delay(9); //充电时间, 大概值 不需要准确
  
  /* 启动HALL传感器接口中断 */
  __HAL_TIM_CLEAR_FLAG(&htimx_hall,TIM_FLAG_CC1);
  HAL_TIMEx_HallSensor_Start_IT(&htimx_hall);
  
  hallPhase = HALL_GetPhase(); // 获取霍尔信号相位

  /* 配置当前霍尔信号对应的PWM相位 */
  BLDCMotor_PhaseCtrl(hallPhase);  // 配置输出PWM
  HAL_TIM_GenerateEvent(&htimx_BLDCM, TIM_EVENTSOURCE_COM); // 软件生成COM事件
  __HAL_TIM_CLEAR_FLAG(&htimx_BLDCM, TIM_FLAG_COM);

  Motor_State = MOTOR_ENABLE;
}
/**
  * 函数功能: 惯性刹车
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 自由停机,就是直接切断输出,依靠惯性是电机停下来
  */
void BLDCM_Inertia_brake()
{
  BLDCMOTOR_DISABLE(); // 使用驱动芯片的shutdown引脚切断输出
  Motor_State = MOTOR_DISABLE;
}
/**
  * 函数功能: 刹车制动
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 使用下桥臂使电机短路,属于能耗制动的一种方式
  */
void BLDCMotor_braking_LowerShort()
{
  /* 先禁止中断,防止刹车过程触发COM事件重新输出 */
  HAL_TIMEx_HallSensor_Stop_IT(&htimx_hall);

  /**
    * 直接关闭MOE,使下桥臂输出高电平
    * 直接控制MOS管下桥臂导通,上桥臂关闭
    */
  /* 下桥臂导通 */
  __HAL_TIM_SET_COMPARE(&htimx_BLDCM, BLDCMOTOR_TIM_CH1, 0);// 如果需要导通上桥臂
  __HAL_TIM_SET_COMPARE(&htimx_BLDCM, BLDCMOTOR_TIM_CH2, 0);// 占空比设置为100%
  __HAL_TIM_SET_COMPARE(&htimx_BLDCM, BLDCMOTOR_TIM_CH3, 0);// 即可
  HAL_TIM_PWM_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
  HAL_TIM_PWM_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
  HAL_TIM_PWM_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
  HAL_TIMEx_PWMN_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
  HAL_TIMEx_PWMN_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
  HAL_TIMEx_PWMN_Start(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
  HAL_TIM_GenerateEvent(&htimx_BLDCM, TIM_EVENTSOURCE_COM);  
  __HAL_TIM_CLEAR_FLAG(&htimx_BLDCM, TIM_FLAG_COM);

  Motor_State = MOTOR_DISABLE;
}
/**
  * 函数功能: 取消刹车制动
  * 输入参数: 无
  * 返 回 值: 无
  * 说    明: 刹车的时候使用下桥臂导通的方式,确认停下来之后取消刹车制动模式
  */
void BLDCMotor_unbraking_LS()
{
  /* 关闭输出 */
  HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
  HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
  HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
  HAL_TIM_GenerateEvent(&htimx_BLDCM, TIM_EVENTSOURCE_COM);  
}
/**
  * 函数功能: 设置电机转速
  * 输入参数: @speed PWM的占空比
  * 返 回 值: 无
  * 说    明: 无
  */
void BLDCMotor_SetSpeed(float speed)
{

  if(speed > 1.0f)
  {
    speed = 1.0f;
  }
  else if(speed < 0.0f)
  {
    speed = 0.0f;
  }
  PWM_Duty = speed;
}
/************** (C) COPYRIGHT 2019-2030 硬石嵌入式开发团队 *****END OF FILE****/
