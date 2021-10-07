

#include <string.h>

#include "sys.h"
#include "usart.h"	
#include "dma.h"

// ����1�жϷ������
// ע��,��ȡUSARTx->SR�ܱ���Ī������Ĵ���
u8  USART_RX_BUF[USART_RX_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�
u8  USART_TX_BUF[USART_TX_LEN]; // ���ջ���,���USART_REC_LEN���ֽ�

// ��ʼ��IO ����1 
// bound:������
void uart_init(u32 bound)
{
   // GPIO�˿�����
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	DMA_InitTypeDef  	DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 							// ʹ��GPIOAʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);							// ʹ��USART1ʱ��
 
	// ����1��Ӧ���Ÿ���ӳ��
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 						// GPIOA9����ΪUSART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 						// GPIOA10����ΪUSART1
	
	// USART1�˿�����
  	GPIO_InitStructure.GPIO_Pin 					= GPIO_Pin_9 | GPIO_Pin_10; 	// GPIOA9��GPIOA10
	GPIO_InitStructure.GPIO_Mode 					= GPIO_Mode_AF;					// ���ù���
	GPIO_InitStructure.GPIO_Speed 					= GPIO_Speed_50MHz;				// �ٶ�50MHz
	GPIO_InitStructure.GPIO_OType 					= GPIO_OType_PP; 				// ���츴�����
	GPIO_InitStructure.GPIO_PuPd 					= GPIO_PuPd_UP; 				// ����
	GPIO_Init(GPIOA,&GPIO_InitStructure); 											// ��ʼ��PA9��PA10

   // USART1 ��ʼ������
	USART_InitStructure.USART_BaudRate 				= bound;						// ����������
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;			// �ֳ�Ϊ8λ���ݸ�ʽ
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;				// һ��ֹͣλ
	USART_InitStructure.USART_Parity 				= USART_Parity_No;				// ����żУ��λ
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;// ��Ӳ������������
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;// �շ�ģʽ
  	USART_Init(USART1, &USART_InitStructure); 										// ��ʼ������1

	
	/* ���� DMA Stream */
	DMA_InitStructure.DMA_Channel 					= DMA_Channel_4;  				// ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr 		= (u32) &USART1->DR;			// DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr 			= (u32) USART_TX_BUF;			// DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR 						= DMA_DIR_MemoryToPeripheral;	// �洢��������ģʽ
	DMA_InitStructure.DMA_BufferSize 				= USART_TX_LEN;					// ���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;	// ���������ģʽ
	DMA_InitStructure.DMA_MemoryInc 				= DMA_MemoryInc_Enable;			// �洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize 		= DMA_PeripheralDataSize_Byte;	// �������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;		// �洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode 						= DMA_Mode_Normal;				// ʹ����ͨģʽ 
	DMA_InitStructure.DMA_Priority 					= DMA_Priority_Medium;			// �е����ȼ�
	DMA_InitStructure.DMA_FIFOMode 					= DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;		// �洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst 			= DMA_PeripheralBurst_Single;	// ����ͻ�����δ���

	DMAx_Init(DMA2_Stream7, &DMA_InitStructure);
 	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);

		/* ���� DMA Stream */
	DMA_InitStructure.DMA_Channel 					= DMA_Channel_4;  				// ͨ��ѡ��
	DMA_InitStructure.DMA_PeripheralBaseAddr 		= (u32) &USART1->DR;			// DMA�����ַ
	DMA_InitStructure.DMA_Memory0BaseAddr 			= (u32) USART_RX_BUF;			// DMA �洢��0��ַ
	DMA_InitStructure.DMA_DIR 						= DMA_DIR_PeripheralToMemory;	// ���赽�洢��ģʽ
	DMA_InitStructure.DMA_BufferSize 				= USART_RX_LEN;					// ���ݴ����� 
	DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;	// ���������ģʽ
	DMA_InitStructure.DMA_MemoryInc 				= DMA_MemoryInc_Enable;			// �洢������ģʽ
	DMA_InitStructure.DMA_PeripheralDataSize 		= DMA_PeripheralDataSize_Byte;	// �������ݳ���:8λ
	DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;		// �洢�����ݳ���:8λ
	DMA_InitStructure.DMA_Mode 						= DMA_Mode_Circular;			// ʹ��ѭ��ģʽ 
	DMA_InitStructure.DMA_Priority 					= DMA_Priority_Medium;			// �е����ȼ�
	DMA_InitStructure.DMA_FIFOMode 					= DMA_FIFOMode_Enable;         
	DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_3QuartersFull;
	DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;		// �洢��ͻ�����δ���
	DMA_InitStructure.DMA_PeripheralBurst 			= DMA_PeripheralBurst_Single;	// ����ͻ�����δ���

	DMAx_Init(DMA2_Stream2, &DMA_InitStructure);
 	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);


  	USART_Cmd(USART1, ENABLE);  													// ʹ�ܴ���1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART1, USART_IT_IDLE|USART_IT_RXNE, ENABLE);									// ��������ж�

	// Usart1 NVIC ����
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								// ����1�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;							// ��ռ���ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;								// �����ȼ�3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									// IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);													// ����ָ���Ĳ�����ʼ��VIC�Ĵ�����
	
}


void USART1_IRQHandler(void)														// ����1�жϷ������
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  							// �����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{	
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		DMAx_Enable(DMA2_Stream2, USART_RX_LEN);
		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	} 
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  							// �����ж�(���յ������ݱ�����0x0d 0x0a��β)
	{	
		USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
		DMA_Cmd(DMA2_Stream2, DISABLE);                      //�ر�DMA���� 
		USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);
		USART_ClearITPendingBit(USART1, USART_IT_IDLE);
	} 
} 
 
void UART_SendData(const u8 *data_buf, u32 data_len)
{
	if ((data_buf != NULL) && (data_len < USART_TX_LEN) && (data_len > 0))
	{
		memcpy(USART_TX_BUF, data_buf, data_len);
		DMAx_Enable(DMA2_Stream7, data_len);
	}
	else
	{
		// reserve;
	}
}

