

#include <string.h>

#include "sys.h"
#include "usart.h"	
#include "dma.h"

// 串口1中断服务程序
// 注意,读取USARTx->SR能避免莫名其妙的错误
u8  USART_RX_BUF[USART_RX_LEN]; // 接收缓冲,最大USART_REC_LEN个字节
u8  USART_TX_BUF[USART_TX_LEN]; // 接收缓冲,最大USART_REC_LEN个字节

// 初始化IO 串口1 
// bound:波特率
void uart_init(u32 bound)
{
   // GPIO端口设置
	GPIO_InitTypeDef 	GPIO_InitStructure;
	USART_InitTypeDef 	USART_InitStructure;
	NVIC_InitTypeDef 	NVIC_InitStructure;
	DMA_InitTypeDef  	DMA_InitStructure;
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA,ENABLE); 							// 使能GPIOA时钟
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);							// 使能USART1时钟
 
	// 串口1对应引脚复用映射
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource9,GPIO_AF_USART1); 						// GPIOA9复用为USART1
	GPIO_PinAFConfig(GPIOA,GPIO_PinSource10,GPIO_AF_USART1); 						// GPIOA10复用为USART1
	
	// USART1端口配置
  	GPIO_InitStructure.GPIO_Pin 					= GPIO_Pin_9 | GPIO_Pin_10; 	// GPIOA9与GPIOA10
	GPIO_InitStructure.GPIO_Mode 					= GPIO_Mode_AF;					// 复用功能
	GPIO_InitStructure.GPIO_Speed 					= GPIO_Speed_50MHz;				// 速度50MHz
	GPIO_InitStructure.GPIO_OType 					= GPIO_OType_PP; 				// 推挽复用输出
	GPIO_InitStructure.GPIO_PuPd 					= GPIO_PuPd_UP; 				// 上拉
	GPIO_Init(GPIOA,&GPIO_InitStructure); 											// 初始化PA9，PA10

   // USART1 初始化设置
	USART_InitStructure.USART_BaudRate 				= bound;						// 波特率设置
	USART_InitStructure.USART_WordLength 			= USART_WordLength_8b;			// 字长为8位数据格式
	USART_InitStructure.USART_StopBits 				= USART_StopBits_1;				// 一个停止位
	USART_InitStructure.USART_Parity 				= USART_Parity_No;				// 无奇偶校验位
	USART_InitStructure.USART_HardwareFlowControl 	= USART_HardwareFlowControl_None;// 无硬件数据流控制
	USART_InitStructure.USART_Mode 					= USART_Mode_Rx | USART_Mode_Tx;// 收发模式
  	USART_Init(USART1, &USART_InitStructure); 										// 初始化串口1

	
	/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel 					= DMA_Channel_4;  				// 通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr 		= (u32) &USART1->DR;			// DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr 			= (u32) USART_TX_BUF;			// DMA 存储器0地址
	DMA_InitStructure.DMA_DIR 						= DMA_DIR_MemoryToPeripheral;	// 存储器到外设模式
	DMA_InitStructure.DMA_BufferSize 				= USART_TX_LEN;					// 数据传输量 
	DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;	// 外设非增量模式
	DMA_InitStructure.DMA_MemoryInc 				= DMA_MemoryInc_Enable;			// 存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize 		= DMA_PeripheralDataSize_Byte;	// 外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;		// 存储器数据长度:8位
	DMA_InitStructure.DMA_Mode 						= DMA_Mode_Normal;				// 使用普通模式 
	DMA_InitStructure.DMA_Priority 					= DMA_Priority_Medium;			// 中等优先级
	DMA_InitStructure.DMA_FIFOMode 					= DMA_FIFOMode_Disable;         
	DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_Full;
	DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;		// 存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst 			= DMA_PeripheralBurst_Single;	// 外设突发单次传输

	DMAx_Init(DMA2_Stream7, &DMA_InitStructure);
 	USART_DMACmd(USART1,USART_DMAReq_Tx,ENABLE);

		/* 配置 DMA Stream */
	DMA_InitStructure.DMA_Channel 					= DMA_Channel_4;  				// 通道选择
	DMA_InitStructure.DMA_PeripheralBaseAddr 		= (u32) &USART1->DR;			// DMA外设地址
	DMA_InitStructure.DMA_Memory0BaseAddr 			= (u32) USART_RX_BUF;			// DMA 存储器0地址
	DMA_InitStructure.DMA_DIR 						= DMA_DIR_PeripheralToMemory;	// 外设到存储器模式
	DMA_InitStructure.DMA_BufferSize 				= USART_RX_LEN;					// 数据传输量 
	DMA_InitStructure.DMA_PeripheralInc 			= DMA_PeripheralInc_Disable;	// 外设非增量模式
	DMA_InitStructure.DMA_MemoryInc 				= DMA_MemoryInc_Enable;			// 存储器增量模式
	DMA_InitStructure.DMA_PeripheralDataSize 		= DMA_PeripheralDataSize_Byte;	// 外设数据长度:8位
	DMA_InitStructure.DMA_MemoryDataSize 			= DMA_MemoryDataSize_Byte;		// 存储器数据长度:8位
	DMA_InitStructure.DMA_Mode 						= DMA_Mode_Circular;			// 使用循环模式 
	DMA_InitStructure.DMA_Priority 					= DMA_Priority_Medium;			// 中等优先级
	DMA_InitStructure.DMA_FIFOMode 					= DMA_FIFOMode_Enable;         
	DMA_InitStructure.DMA_FIFOThreshold 			= DMA_FIFOThreshold_3QuartersFull;
	DMA_InitStructure.DMA_MemoryBurst 				= DMA_MemoryBurst_Single;		// 存储器突发单次传输
	DMA_InitStructure.DMA_PeripheralBurst 			= DMA_PeripheralBurst_Single;	// 外设突发单次传输

	DMAx_Init(DMA2_Stream2, &DMA_InitStructure);
 	USART_DMACmd(USART1,USART_DMAReq_Rx,ENABLE);


  	USART_Cmd(USART1, ENABLE);  													// 使能串口1 
	
	USART_ClearFlag(USART1, USART_FLAG_TC);
	
	USART_ITConfig(USART1, USART_IT_IDLE|USART_IT_RXNE, ENABLE);									// 开启相关中断

	// Usart1 NVIC 配置
  	NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;								// 串口1中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=3;							// 抢占优先级3
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =3;								// 子优先级3
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;									// IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);													// 根据指定的参数初始化VIC寄存器、
	
}


void USART1_IRQHandler(void)														// 串口1中断服务程序
{
	if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  							// 接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{	
		USART_ITConfig(USART1, USART_IT_RXNE, DISABLE);
		DMAx_Enable(DMA2_Stream2, USART_RX_LEN);
		USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);
		USART_ClearITPendingBit(USART1, USART_IT_RXNE);
	} 
	else if(USART_GetITStatus(USART1, USART_IT_IDLE) != RESET)  							// 接收中断(接收到的数据必须是0x0d 0x0a结尾)
	{	
		USART_ITConfig(USART1, USART_IT_IDLE, DISABLE);
		DMA_Cmd(DMA2_Stream2, DISABLE);                      //关闭DMA传输 
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

