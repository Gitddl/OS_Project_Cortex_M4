#include "dma.h"
#include "stm32f4xx_dma.h"

/*******************************************************************************
* 函 数 名         : DMAx_Init
* 函数功能		   : DMA初始化函数
* 输    入         : DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
					 chx:DMA通道选择,@ref DMA_channel DMA_Channel_0~DMA_Channel_7
					 par:外设地址
					 mar:存储器地址
					 ndtr:数据传输量
* 输    出         : 无
*******************************************************************************/ 
void DMAx_Init(DMA_Stream_TypeDef *DMA_Streamx, DMA_InitTypeDef *DMA_InitStructure)
{
	
	if((u32)DMA_Streamx>(u32)DMA2)//得到当前stream是属于DMA2还是DMA1
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA2,ENABLE);//DMA2时钟使能
	}
	else 
	{
		RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_DMA1,ENABLE);//DMA1时钟使能 
	}
	DMA_DeInit(DMA_Streamx);
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE){}//等待DMA可配置 

	DMA_Init(DMA_Streamx, DMA_InitStructure);//初始化DMA Stream
	
}

/*******************************************************************************
* 函 数 名         : DMAx_Enable
* 函数功能		   : 开启一次DMA传输
* 输    入         : DMA_Streamx:DMA数据流,DMA1_Stream0~7/DMA2_Stream0~7
					 ndtr:数据传输量
* 输    出         : 无
*******************************************************************************/ 
void DMAx_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr)
{
 
 	DMA_ClearFlag(DMA2_Stream7,DMA_FLAG_TCIF7);
	DMA_Cmd(DMA_Streamx, DISABLE);                      //关闭DMA传输 
	
	while (DMA_GetCmdStatus(DMA_Streamx) != DISABLE);	//确保DMA可以被设置  
		
	DMA_SetCurrDataCounter(DMA_Streamx,ndtr);          //数据传输量  
 
	DMA_Cmd(DMA_Streamx, ENABLE);						//开启DMA传输 
}
