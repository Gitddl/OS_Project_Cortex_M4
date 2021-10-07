#include "led.h"
#include "./Public/sys.h"

/*******************************************************************************
* �� �� ��         : LED_Init
* ��������		   : LED��ʼ������
* ��    ��         : ��
* ��    ��         : ��
*******************************************************************************/

#define ON		0
#define OFF		1

void LED_Init()
{
	GPIO_InitTypeDef GPIO_InitStructure; //����ṹ�����
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF,ENABLE); //ʹ�ܶ˿�Fʱ��
	
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_OUT; //���ģʽ
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_9|GPIO_Pin_10;//�ܽ�����F9
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_100MHz;//�ٶ�Ϊ100M
	GPIO_InitStructure.GPIO_OType=GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;//����
	GPIO_Init(GPIOF,&GPIO_InitStructure); //��ʼ���ṹ��
	GPIO_SetBits(GPIOF,GPIO_Pin_9|GPIO_Pin_10);
}

void LED_ON()
{
	led1 = ON;  //��LED
}

void LED_OFF()
{
	led1 = OFF;
}

void LED_Tigger()
{
	led1 = !led1;
}

