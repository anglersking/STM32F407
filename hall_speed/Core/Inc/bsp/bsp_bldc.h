/**
  ******************************************************************************
  * �ļ�����: bsp_GeneralTIM.h 
  * ��    ��: ӲʯǶ��ʽ�����Ŷ�
  * ��    ��: V1.0
  * ��д����: 2020-07-21
  * ��    ��: TIM-API
  ******************************************************************************
  * ˵����
  * ����������Ӳʯstm32������YS-F4Proʹ�á�
  * 
  * �Ա���
  * ��̳��http://www.ing10bbs.com
  * ��Ȩ��ӲʯǶ��ʽ�����Ŷ����У��������á�
  ******************************************************************************
  */
#ifndef __BLDCMOTOR_TIM_H__
#define __BLDCMOTOR_TIM_H__

/* ����ͷ�ļ� ----------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* ���Ͷ��� ------------------------------------------------------------------*/
/* ��������� */
typedef enum 
{
  MOTOR_DIR_CW = 0, // ˳ʱ��ת��
  MOTOR_DIR_CCW     // ��ʱ��ת��
}MotorDir_Typedef;  // ������

/* ���ʹ�ܶ��� */
typedef enum 
{
  MOTOR_ENABLE  = 0,
  MOTOR_DISABLE
}MotorSta_Typedef;

/* �궨�� --------------------------------------------------------------------*/
#define USE_PMSMMOTOR
/******************** ͨ�ö�ʱ��TIM�������壬TIM8 CH1,CH2,CH3 *****************/

#define BLDCMOTOR_TIMx                        TIM8
#define BLDCMOTOR_TIM_RCC_CLK_ENABLE()        __HAL_RCC_TIM8_CLK_ENABLE()

#define BLDCMOTOR_TIM_GPIO_AF                 GPIO_AF3_TIM8
#define BLDCMOTOR_TIM_GPIO_RCC_CLK_ENABLE()   {__HAL_RCC_GPIOI_CLK_ENABLE();\
                                              __HAL_RCC_GPIOH_CLK_ENABLE();}

#define BLDCMOTOR_TIM_CH1_PORT                GPIOI          // TIM8_CH1������
#define BLDCMOTOR_TIM_CH1_PIN                 GPIO_PIN_5
#define BLDCMOTOR_TIM_CH1                     TIM_CHANNEL_1

#define BLDCMOTOR_TIM_CH2_PORT                GPIOI          // TIM8_CH2������
#define BLDCMOTOR_TIM_CH2_PIN                 GPIO_PIN_6
#define BLDCMOTOR_TIM_CH2                     TIM_CHANNEL_2

#define BLDCMOTOR_TIM_CH3_PORT                GPIOI          // TIM8_CH3������
#define BLDCMOTOR_TIM_CH3_PIN                 GPIO_PIN_7
#define BLDCMOTOR_TIM_CH3                     TIM_CHANNEL_3

/* ��������ͨ�� */
#define BLDCMOTOR_TIM_CH1N_PORT               GPIOH          // TIM8_CH1N������
#define BLDCMOTOR_TIM_CH1N_PIN                GPIO_PIN_13

#define BLDCMOTOR_TIM_CH2N_PORT               GPIOH          // TIM8_CH2N������
#define BLDCMOTOR_TIM_CH2N_PIN                GPIO_PIN_14

#define BLDCMOTOR_TIM_CH3N_PORT               GPIOH          // TIM8_CH3N������
#define BLDCMOTOR_TIM_CH3N_PIN                GPIO_PIN_15

/* short down ���� */
#define BLDCMOTOR_SHORTDOWN_RCC_CLK_ENABLE()  __HAL_RCC_GPIOH_CLK_ENABLE()
#define BLDCMOTOR_SHORTDOWN_PORT              GPIOH
#define BLDCMOTOR_SHORTDOWN_PIN               GPIO_PIN_9

#define BLDCMOTOR_ENABLE()                   HAL_GPIO_WritePin(BLDCMOTOR_SHORTDOWN_PORT,BLDCMOTOR_SHORTDOWN_PIN,GPIO_PIN_SET)
#define BLDCMOTOR_DISABLE()                  HAL_GPIO_WritePin(BLDCMOTOR_SHORTDOWN_PORT,BLDCMOTOR_SHORTDOWN_PIN,GPIO_PIN_RESET)

/** ����Դѡ��,TIM1��Ϊ�Ӷ�ʱ��,�������ӵ�TIM3,TIM4��TRGO,������ʱ������com�¼�
  * TIM5   TIM_TS_ITR3
  * ѡ��TIM2��ʱ��,����ԴΪTIR1��ѡ��TIM4�򴥷�Դ��TIM_TS_ITR2
  */
#define BLDMOTOR_TIM_TS                       TIM_TS_ITR3 // ��ʱ������Դ,ITR3  TIM5->TIM8

// ���嶨ʱ��Ԥ��Ƶ����ʱ��ʵ��ʱ��Ƶ��Ϊ��168MHz/��BLDCMOTOR_TIMx_PRESCALER+1��
#define BLDCMOTOR_TIM_PRESCALER            0  // ʵ��ʱ��Ƶ��Ϊ��168MHz

// ���嶨ʱ�����ڣ�����ʱ����ʼ������BLDCMOTOR_TIMx_PERIODֵ�Ǹ��¶�ʱ�������ɶ�Ӧ�¼����ж�
#define BLDCMOTOR_TIM_PERIOD               4200  // ��ʱ������Ƶ��Ϊ��168MHz/(8400)=20KHz����50us��ʱ����

// ��ʱ��ͨ��1��ʼռ�ձȣ�ʵ�ʲ�û�����壬�ڴ�����ʵʱ�޸���
#define BLDCMOTOR_TIM_CH1_PULSE            0   // ��ʼռ�ձ�

/* ��չ���� ------------------------------------------------------------------*/
extern TIM_HandleTypeDef htimx_BLDCM;
extern MotorSta_Typedef Motor_State ; // ���ʹ��״̬
extern MotorDir_Typedef Motor_Dir  ;  // ������� 

/* �������� ------------------------------------------------------------------*/
void BLDCMOTOR_TIMx_Init(void);
void BLDCMotor_Start(void);
void BLDCM_Inertia_brake(void);
void BLDCMotor_braking_LowerShort(void);
void BLDCMotor_unbraking_LS(void);

void BLDCMotor_SetSpeed(float speed);

void BLDCMotor_PhaseCtrl(int32_t HALLPhase );

#endif	/* __BLDCMOTOR_TIM_H__ */
/************** (C) COPYRIGHT 2019-2030 ӲʯǶ��ʽ�����Ŷ� *****END OF FILE****/


