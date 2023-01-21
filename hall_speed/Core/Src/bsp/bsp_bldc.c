/**
  ******************************************************************************
  * �ļ�����: bsp_GeneralTIM.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2020-07-21
  * ��    ��: TIM�Ƚ������������
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "bsp_bldc.h" 
#include "bsp_hall.h"

/* ˽�����Ͷ��� --------------------------------------------------------------*/

/* ˽�к궨�� ----------------------------------------------------------------*/

/* ˽�б��� ------------------------------------------------------------------*/
TIM_HandleTypeDef htimx_BLDCM;

/* ��չ���� ------------------------------------------------------------------*/
MotorSta_Typedef Motor_State = MOTOR_DISABLE; // ���ʹ��״̬
MotorDir_Typedef Motor_Dir   = MOTOR_DIR_CCW;  // ������� ,˳ʱ��
float PWM_Duty   = 0.25f;        // 25%ռ�ձ�

/* ˽�к���ԭ�� --------------------------------------------------------------*/

/* ������ --------------------------------------------------------------------*/

/**
  * ��������: ͨ�ö�ʱ��Ӳ����ʼ������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void YS_TIM_Base_MspInit(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* ��ʱ��ͨ���������Ŷ˿�ʱ��ʹ�� */
  BLDCMOTOR_TIM_GPIO_RCC_CLK_ENABLE();
  
  /* ��ʱ��ͨ��1��������IO��ʼ�� */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH1_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = BLDCMOTOR_TIM_GPIO_AF;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH1_PORT, &GPIO_InitStruct);
  
  /* ��ʱ��ͨ��2��������IO��ʼ�� */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH2_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = BLDCMOTOR_TIM_GPIO_AF;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH2_PORT, &GPIO_InitStruct);
  
  /* ��ʱ��ͨ��3��������IO��ʼ�� */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH3_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = BLDCMOTOR_TIM_GPIO_AF;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3_PORT, &GPIO_InitStruct);
  
  /* ��ʱ������ͨ��1��������IO��ʼ�� */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH1N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3N_PORT, &GPIO_InitStruct);
  
  /* ��ʱ������ͨ��1��������IO��ʼ�� */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH2N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3N_PORT, &GPIO_InitStruct);
  
  /* ��ʱ������ͨ��1��������IO��ʼ�� */
  GPIO_InitStruct.Pin       = BLDCMOTOR_TIM_CH3N_PIN;
  HAL_GPIO_Init(BLDCMOTOR_TIM_CH3N_PORT, &GPIO_InitStruct);
  
  /* shortdown�������ų�ʼ�� */
  BLDCMOTOR_SHORTDOWN_RCC_CLK_ENABLE();
  BLDCMOTOR_ENABLE(); // ����������Ϊ��Ч��ƽ,��PWM�ܿ��Ƶ��
  GPIO_InitStruct.Pin       = BLDCMOTOR_SHORTDOWN_PIN;
  GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = 0;
  GPIO_InitStruct.Pull      = GPIO_NOPULL;
  HAL_GPIO_Init(BLDCMOTOR_SHORTDOWN_PORT, &GPIO_InitStruct);
  
  BLDCMOTOR_ENABLE(); // ����������Ϊ��Ч��ƽ,ʹPWM�ܿ��Ƶ��
}

/**
  * ��������: �߼���ʱ����ʼ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ��
  */
void BLDCMOTOR_TIMx_Init(void)
{
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* ͨ�ö�ʱ������ʱ��ʹ�� */
  BLDCMOTOR_TIM_RCC_CLK_ENABLE();
  /* ��ʼ�������豸����ʱ��ͨ�����ţ�ʹ��ʱ�� */ 
  YS_TIM_Base_MspInit();
  
  /* ���ö�ʱ�������������� */
  htimx_BLDCM.Instance           = BLDCMOTOR_TIMx;          // ��ʱ��TIM
  htimx_BLDCM.Init.Prescaler     = BLDCMOTOR_TIM_PRESCALER; // PSC����Ԥ��Ƶֵ
  htimx_BLDCM.Init.CounterMode   = TIM_COUNTERMODE_CENTERALIGNED1;// ���Ķ���ģʽ
  htimx_BLDCM.Init.Period        = BLDCMOTOR_TIM_PERIOD;    // ��������
  htimx_BLDCM.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  HAL_TIM_Base_Init(&htimx_BLDCM);

    /* ������ɲ������ */
  sBreakDeadTimeConfig.OffStateRunMode  = TIM_OSSR_ENABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_ENABLE;
  sBreakDeadTimeConfig.LockLevel        = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime         = 0x22; // ��Լ200ns������ʱ��
  sBreakDeadTimeConfig.BreakState       = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity    = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.AutomaticOutput  = TIM_AUTOMATICOUTPUT_DISABLE;
  HAL_TIMEx_ConfigBreakDeadTime(&htimx_BLDCM, &sBreakDeadTimeConfig);

  /* ���ñȽ����ΪPWM1ģʽ */
  sConfigOC.OCMode       = TIM_OCMODE_PWM1;        // PWM1 ģʽ 
  sConfigOC.Pulse        = BLDCMOTOR_TIM_CH1_PULSE;// CHx�����������
  sConfigOC.OCPolarity   = TIM_OCPOLARITY_HIGH;    // �������
  sConfigOC.OCNPolarity  = TIM_OCNPOLARITY_HIGH;   // ����ͨ���������
  sConfigOC.OCIdleState  = TIM_OCIDLESTATE_RESET;  // ����״̬����
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET; // ��������״̬����
  sConfigOC.OCFastMode   = TIM_OCFAST_DISABLE;     // �������ģʽ����ֹ  
  
  /* ����ʹ�õ���HAL_TIM_OC_ConfigChannel,������HAL_TIM_PWM_ConfigChannel
   * ���ʹ��HAL_TIM_PWM_ConfigChannel��ʹ�ܱȽ�����Ԥװ�ع���,Ҳ�����޸ıȽ�ֵ
   * ֮����������Ч,�⽫����ɻ���֮��ĵ�һ�����������̶�
   * ����������ʵ�ʵ����ü�����ȫһ��,ֻ��HAL_TIM_OC_ConfigChannel����ȥ����
   * OCFastmode�ͱȽ�����Ԥװ��ʹ��.
   * 
   */
  HAL_TIM_OC_ConfigChannel(&htimx_BLDCM, &sConfigOC, BLDCMOTOR_TIM_CH1);
  /* CH2 CH2N*/
  HAL_TIM_OC_ConfigChannel(&htimx_BLDCM, &sConfigOC, BLDCMOTOR_TIM_CH2);
  /* CH3 CH3N */
  HAL_TIM_OC_ConfigChannel(&htimx_BLDCM, &sConfigOC, BLDCMOTOR_TIM_CH3);
  
  /* ����COM�¼�(commutation)
   * ������ʱ����TRGO����TIM1��COM�¼�
   * ������COM�¼���ʱ�򽫻�ͳһ���¶�ʱ��ͨ�������ʹ�ܿ���λ,�������ڳ���ִ��
   * ���ӳٵ��»���ʱ�����ű۳��ָ�խ����.
   */
  HAL_TIMEx_ConfigCommutationEvent(&htimx_BLDCM, BLDMOTOR_TIM_TS, TIM_COMMUTATION_TRGI);
}


/**
  * ��������: ��ˢ�����λ����
  * �������: @HALLPhase �����ź���λ
  * �� �� ֵ: ��
  * ˵    ��: ���ƶ�ʱ�����PWM���λ���,���嶨ʱ�����ͨ��CH1ΪA��(U) 
  *           CH2ΪB��(V)��CH3ΪC��(W),������һ�λ��������ʱ��Ĳ���
  */
void BLDCMotor_PhaseCtrl(int32_t HALLPhase )
{
#ifndef USE_PMSMMOTOR  
  /* ˳ʱ��Ļ���˳������ʱ���˳���ǹ���7����,���Կ���ʹ��7����ʱ���˳��õ�
   * ˳ʱ��Ļ���˳��,����ʹ�����������
   */
  if(MOTOR_DIR_CW == Motor_Dir)
    HALLPhase = 0x07 ^ HALLPhase;// ������λ��� 111b ^ 010b -> 101b
#else  
  /* PMSM����ת˳���BLDC�պ����෴��,�趨ΪMOTOR_DIR_CCW��ʱ��BLDC����ʱ����ת
   * PMSM��˳ʱ����ת,�����Ҫ�趨ΪMOTOR_DIR_CCW��ʱ��PMSMΪ��ʱ����ת,��ʹ��
   * ������������������
   */
  if(MOTOR_DIR_CCW == Motor_Dir)
    HALLPhase = 0x07 ^ HALLPhase;// ������λ��� 111b ^ 010b -> 101b
#endif
  switch(HALLPhase)
  {
    /* ��������U(A),V(B),W(C)����ֱ��Ӧ��CH1,CH2,CH3;
     *  A+,A-�ֱ��ʾCH1���Ƶ���,���ű۵�ͨ
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
  * ��������: �������
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ������ˢ���
  */
void  BLDCMotor_Start()
{
  int32_t hallPhase = 0;

  BLDCMOTOR_ENABLE();// ʹ���������PWM
  
  /* ���ű۵�ͨ,���Ծٵ��ݳ�� */
  BLDCMotor_braking_LowerShort();
  HAL_Delay(9); //���ʱ��, ���ֵ ����Ҫ׼ȷ
  
  /* ����HALL�������ӿ��ж� */
  __HAL_TIM_CLEAR_FLAG(&htimx_hall,TIM_FLAG_CC1);
  HAL_TIMEx_HallSensor_Start_IT(&htimx_hall);
  
  hallPhase = HALL_GetPhase(); // ��ȡ�����ź���λ

  /* ���õ�ǰ�����źŶ�Ӧ��PWM��λ */
  BLDCMotor_PhaseCtrl(hallPhase);  // �������PWM
  HAL_TIM_GenerateEvent(&htimx_BLDCM, TIM_EVENTSOURCE_COM); // �������COM�¼�
  __HAL_TIM_CLEAR_FLAG(&htimx_BLDCM, TIM_FLAG_COM);

  Motor_State = MOTOR_ENABLE;
}
/**
  * ��������: ����ɲ��
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ����ͣ��,����ֱ���ж����,���������ǵ��ͣ����
  */
void BLDCM_Inertia_brake()
{
  BLDCMOTOR_DISABLE(); // ʹ������оƬ��shutdown�����ж����
  Motor_State = MOTOR_DISABLE;
}
/**
  * ��������: ɲ���ƶ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ʹ�����ű�ʹ�����·,�����ܺ��ƶ���һ�ַ�ʽ
  */
void BLDCMotor_braking_LowerShort()
{
  /* �Ƚ�ֹ�ж�,��ֹɲ�����̴���COM�¼�������� */
  HAL_TIMEx_HallSensor_Stop_IT(&htimx_hall);

  /**
    * ֱ�ӹر�MOE,ʹ���ű�����ߵ�ƽ
    * ֱ�ӿ���MOS�����ű۵�ͨ,���ű۹ر�
    */
  /* ���ű۵�ͨ */
  __HAL_TIM_SET_COMPARE(&htimx_BLDCM, BLDCMOTOR_TIM_CH1, 0);// �����Ҫ��ͨ���ű�
  __HAL_TIM_SET_COMPARE(&htimx_BLDCM, BLDCMOTOR_TIM_CH2, 0);// ռ�ձ�����Ϊ100%
  __HAL_TIM_SET_COMPARE(&htimx_BLDCM, BLDCMOTOR_TIM_CH3, 0);// ����
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
  * ��������: ȡ��ɲ���ƶ�
  * �������: ��
  * �� �� ֵ: ��
  * ˵    ��: ɲ����ʱ��ʹ�����ű۵�ͨ�ķ�ʽ,ȷ��ͣ����֮��ȡ��ɲ���ƶ�ģʽ
  */
void BLDCMotor_unbraking_LS()
{
  /* �ر���� */
  HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
  HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
  HAL_TIM_PWM_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH1);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH2);
  HAL_TIMEx_PWMN_Stop(&htimx_BLDCM, BLDCMOTOR_TIM_CH3);
  HAL_TIM_GenerateEvent(&htimx_BLDCM, TIM_EVENTSOURCE_COM);  
}
/**
  * ��������: ���õ��ת��
  * �������: @speed PWM��ռ�ձ�
  * �� �� ֵ: ��
  * ˵    ��: ��
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
/************** (C) COPYRIGHT 2019-2030 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/
