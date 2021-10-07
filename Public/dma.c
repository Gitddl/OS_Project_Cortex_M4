#include "dma.h"
#include "stm32f4xx_dma.h"

/*******************************************************************************
* �� �� ��         : DMAx_Init
* ��������		   : DMA��ʼ������
* ��    ��         : DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
					 chx:DMAͨ��ѡ��,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
					 par:�����ַ
					 mar:�洢����ַ
					 ndtr:���ݴ�����
* ��    ��         : ��
*******************************************************************************/ 
void DMAx_Init(DMA_Stream_TypeDef *DMA_Streamx, DMA_InitTypeDef *DMA_InitStructure)
{
	
	if((u32)DMA_Streamx>(u32)DMA2)//�õ���ǰstream������DMA2����DMA1
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2ʱ��ʹ��
	}
	else 
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1ʱ��ʹ�� 
	}
	DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//�ȴ�DMA������ 

	DMA_Init(DMA_Streamx, DMA_InitStructure);//��ʼ��DMA Stream
	
}

/*******************************************************************************
* �� �� ��         : DMAx_Enable
* ��������		   : ����һ��DMA����
* ��    ��         : DMA_Streamx:DMA������,DMA1_Stream0~7/DMA2_Stream0~7
					 ndtr:���ݴ�����
* ��    ��         : ��
*******************************************************************************/ 
void DMAx_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
 	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
	DMA_Cmd(DMA_Streamx, DISABLE);                      //�ر�DMA���� 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE);	//ȷ��DMA���Ա�����  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //���ݴ�����  
 
	DMA_Cmd(DMA_Streamx, ENABLE);						//����DMA���� 
}
