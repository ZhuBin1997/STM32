/*********************************************************************/
/****************     robogame2017 ����������     ********************/
/****************            �������             ********************/
/*********************************************************************/

#include "stm32f10x.h"

/**********   ����   ***********/
	 /******    GPIOA    ******/
#define ENABLE_1 	GPIO_Pin_0
#define MS1_1 		GPIO_Pin_6
#define MS2_1 		GPIO_Pin_2
#define MS3_1 		GPIO_Pin_3
#define STEP_1		GPIO_Pin_4
#define DIR_1			GPIO_Pin_5


/*********  ����ģʽ  **********/
#define FULL_STEP 			1
#define HALF_STEP		 		2
#define QUARTER_STEP 		4
#define EIGHTH_STEP 		8
#define SIXTEENTH_STEP 16


//һȦ200��
void Stepmotor_Configuration(void);					//��ʼ���������
void setPulsePerSecond1(u32 pulse);					//���ò������1 ÿ�����������������Ϊ��תģʽ
void setSteps1(u32 pulse,u32 number);				//���ò������1 ÿ��������� �� �����������������������Ϊ�Ʋ�ģʽ
void setResolution1(u8 resolution);					//���ò������1����ģʽ
void disable1(void);												//ͣ�ò������1
void enable1(void);													//ʹ�ܲ������1
void setDirection1(u8 dir);									//���ò������1ת��

void delay_nus(u32);          						  //72Mʱ���£�Լ��ʱus
void delay_nms(u32);         						    //72Mʱ���£�Լ��ʱms
