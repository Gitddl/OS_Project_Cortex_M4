#ifndef _dma_H
#define _dma_H

#include "sys.h"


void DMAx_Init(DMA_Stream_TypeDef *DMA_Streamx, DMA_InitTypeDef *DMA_InitStructure);//����DMAx_CHx
void DMAx_Enable(DMA_Stream_TypeDef *DMA_Streamx,u16 ndtr);	//ʹ��һ��DMA����

#endif
