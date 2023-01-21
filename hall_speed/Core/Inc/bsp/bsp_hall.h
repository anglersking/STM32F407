/**
  ******************************************************************************
  * �ļ�����: main.c 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2019-09-03
  * ��    ��: HALL�������ӿ�-API
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-H7Multiʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */

#ifndef __BSP_HALL_H_
#define __BSP_HALL_H_

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/

/* �궨�� --------------------------------------------------------------------*/

/**************** ��ʱ�������ӿ�TIM�������壬TIMx CH1,CH2,CH3 *****************/

/* ע��:���� "�����ӿڶ�ʱ��"��ʱ����Ҫ�޸� "PWM�����ʱ��(TIM1/TIM8)" �Ĵ���Դ  */

#define HALL_TIMx                        TIM5
#define HALL_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM5_CLK_ENABLE()

#define HALL_TIM_GPIO_AF                 GPIO_AF2_TIM5
#define HALL_TIM_GPIO_RCC_CLK_ENABLE()   __HAL_RCC_GPIOH_CLK_ENABLE()

#define HALL_TIM_CH1_PORT                GPIOA          // CH1������
#define HALL_TIM_CH1_PIN                 GPIO_PIN_0
#define HALL_TIM_CH1                     TIM_CHANNEL_1

#define HALL_TIM_CH2_PORT                GPIOA         // CH2������
#define HALL_TIM_CH2_PIN                 GPIO_PIN_1
#define HALL_TIM_CH2                     TIM_CHANNEL_2

#define HALL_TIM_CH3_PORT                GPIOA          // CH3������
#define HALL_TIM_CH3_PIN                 GPIO_PIN_2
#define HALL_TIM_CH3                     TIM_CHANNEL_3

#define HALL_TIM_IRQn                    TIM5_IRQn
#define HALL_TIM_IRQHanler               TIM5_IRQHandler


// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��84MHz/��HALL_TIMx_PRESCALER+1��
#define HALL_TIM_PRESCALER               83  // ʵ��ʱ��Ƶ��Ϊ��1MHz

#define HALL_TIM_FREQ                (84e6/(HALL_TIM_PRESCALER+1)) // ��ʱ������Ƶ��

// ���嶨ʱ�����ڣ�����ʱ����ʼ������HALL_TIMx_PERIODֵ�Ǹ��¶�ʱ�������ɶ�Ӧ�¼����ж�
#define HALL_TIM_PERIOD                  0xFFFF //

#define POLE_PAIRES                      4 // ��ˢ���4�Լ�
#define PPR                             (POLE_PAIRES*2*3) // (4*2*3) ����ÿת,

/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_hall;
extern __IO uint32_t RT_hallPhase; // �����ź�
extern __IO uint32_t RT_hallcomp;  // �����������ֵ
extern __IO uint32_t RT_hallcnt ; // ��������ֵ

/* �������� ------------------------------------------------------------------*/
void HALLSensor_TIMx_Init(void);
int32_t HALL_GetPhase(void);

#endif /* __BSP_HALL_H_ */
/********** (C) COPYRIGHT 2019-2030 ӲʯǶ��ʽ�����Ŷ� *******END OF FILE******/
