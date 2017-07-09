#include "stepmotor.h"

#define YUZHI 99		//tim3��������ֵ

u8 flag1=0;  				//��flagΪ0��Ϊ��תģʽ����flagΪ1��Ϊ�Ʋ�ģʽ
u32 counter1=0; 
u32 counter2=0; 
u32 jishi1=1000;		//ÿ���Ӹ� 1M/(YUZHI+1)/jishi1 ������
u32 jishi2=1000;
u32 maichongshu1=0;
u32 maichongshu2=0;

void TIM3_IRQHandler(void)		//	  //TIM3����������ж���Ӧ����������pwm��
{
	TIM_ClearITPendingBit(TIM3,TIM_IT_Update);	//	 //	 ���TIM3����ж���Ӧ������־λ
	if(flag1==0){	
		if(counter1==0){
			GPIO_SetBits(GPIOA,STEP_1);
			counter1++;
		}
		else{
			GPIO_ResetBits(GPIOA,STEP_1);
			counter1++;
			if(counter1>=jishi1){
				counter1=0;
			}
		}
	}
	else if(maichongshu1>0){
		if(counter1==0){
			GPIO_SetBits(GPIOA,STEP_1);
			counter1++;
			maichongshu1--;
		}
		else{
			GPIO_ResetBits(GPIOA,STEP_1);
			counter1++;
			if(counter1>=jishi1){
				counter1=0;
			}
		}		
	}
	else {};
}	



void setPulsePerSecond1(u32 pulse){
	//pulseΪÿ���������
	flag1=0;	
	if(pulse==0){
		disable1();	
	}	
	else{
		jishi1=1000000/(YUZHI+1)/pulse;
		enable1();	
	}
}

void setPulseNumber1(u32 pulse,u32 number){
	//pulseΪÿ���������,numberΪ��������������
	flag1=1;	
	if(pulse==0){
		disable1();	
	}	
	else{
		jishi1=1000000/(YUZHI+1)/pulse;
		maichongshu1=number;
		enable1();	
	}
}

void setSteps1(u32 pulse,u32 number){
	setPulseNumber1(pulse,number);
	while(maichongshu1!=0){
		delay_nms(10);
	}
}

//���÷ֱ��ʣ�ֵԽ��һ������ת��ԽС
void setResolution1(u8 resolution){
	switch(resolution){
		case FULL_STEP:
			GPIO_ResetBits(GPIOA,MS1_1|MS2_1|MS3_1);
			break;
		case HALF_STEP:
			GPIO_ResetBits(GPIOA,MS2_1|MS3_1);
			GPIO_SetBits(GPIOA,MS1_1);
			break;
		case QUARTER_STEP:
			GPIO_ResetBits(GPIOA,MS1_1|MS3_1);
			GPIO_SetBits(GPIOA,MS2_1);
			break;
		case EIGHTH_STEP:
			GPIO_ResetBits(GPIOA,MS3_1);
			GPIO_SetBits(GPIOA,MS1_1|MS2_1);
			break;
		case SIXTEENTH_STEP:
			GPIO_SetBits(GPIOA,MS1_1|MS2_1|MS3_1);
			break;
		default:
			GPIO_ResetBits(GPIOA,MS1_1|MS2_1|MS3_1);
			break;
	}
}

void disable1(void){
		GPIO_SetBits(GPIOA,ENABLE_1);
}

void enable1(void){
		GPIO_ResetBits(GPIOA,ENABLE_1);	
}

void setDirection1(u8 dir){
	if(dir){
		GPIO_ResetBits(GPIOA,DIR_1);
	}
	else{
		GPIO_SetBits(GPIOA,DIR_1);
	}
}


void tim3()							  //����TIM3Ϊ������ʱ��ģʽ ��Լ10us����һ�Σ�����Ƶ��Լ100kHz
{
  	TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;	  //�����ʽΪTIM_TimeBaseInitTypeDef�Ľṹ�������ΪTIM_TimeBaseStructure  

  	TIM_TimeBaseStructure. TIM_Period =YUZHI;		  //���ü�����ֵΪ9������ʱ���Զ����㣬�������ж�
	  TIM_TimeBaseStructure.TIM_Prescaler =71;	   //	 ʱ��Ԥ��Ƶֵ�����Զ���
  	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;	 // ʱ�ӷ�Ƶ����
  	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;	 // ������ʽΪ���ϼ���

    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);		 //	 ��ʼ��tim3
    TIM_ClearITPendingBit(TIM3,TIM_IT_Update); //���TIM3����жϱ�־
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //  ʹ��TIM3����������ж�
    TIM_Cmd(TIM3,ENABLE);					  //		   ʹ��TIM3
}

void nvic()									//�����ж����ȼ�
{	 
    NVIC_InitTypeDef NVIC_InitStructure;  //	 //	  ����һ���ȼ�����

 	  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1); //Ҫ��ͬһ��Group
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn; //TIM3	��������ж�
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//	������ȼ�Ϊ1������һ����ͬ����ϣ���ж��໥��϶Է�
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1; 	//	��Ӧ���ȼ�1��������һ�����������ж�ͬʱ��ʱ����һ����ִ��
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
}

void RCC_Configuration(void)				 //ʹ���κ�һ������ʱ����ؿ�������Ӧ��ʱ��
{
  
   RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	  //ʹ��APB2���������ʱ�ӣ�����GPIOC, ���ܸ���ʱ��AFIO�ȣ�
                                                                              //�������������裬����̼����ֲ������
	
   RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ��APB1���������ʱ�ӣ���ʱ��tim3��4��������������ֲ�

                
}


void GPIO_Configuration(void)			 //ʹ��ĳio���������ʱ������ض����ʼ������
{
   GPIO_InitTypeDef GPIO_InitStructure;   //�����ʽΪGPIO_InitTypeDef�Ľṹ�������ΪGPIO_InitStructure  
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;	   //����IO�ڹ���ģʽΪ	����������н�ǿ�����������
   GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	   //����IO����ߵ��������Ϊ50M
	 GPIO_InitStructure.GPIO_Pin = ENABLE_1|MS1_1|MS2_1|MS3_1|STEP_1|DIR_1;	 //���ñ�ѡ�еĹܽţ�|��ʾͬʱ��ѡ��
   GPIO_Init(GPIOA, &GPIO_InitStructure);		  //��ʼ��GPIOC����ӦIO��Ϊ�������ã�����led���
   
	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);	//ʧ��STM32 JTAG��д���ܣ�ֻ����SWDģʽ��д����ų�PA15��PB�в���IO��
}

void Stepmotor_Configuration(void){

  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_AFIO, ENABLE);	  //ʹ��APB2���������ʱ�ӣ�����GPIOC, ���ܸ���ʱ��AFIO�ȣ�
                                                                              //�������������裬����̼����ֲ������
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE); //ʹ��APB1���������ʱ�ӣ���ʱ��tim3��4��������������ֲ�
  
	RCC_Configuration();															   		
  GPIO_Configuration();							
  nvic(); 
	tim3();
	    
}


void delay_nus(u32 n)		//72Mʱ���£�Լ��ʱus
{
  u8 i;
  while(n--)
  {
    i=7;
    while(i--);
  }
}

void delay_nms(u32 n)	  //72Mʱ���£�Լ��ʱms
{
    while(n--)
      delay_nus(1000);
}


