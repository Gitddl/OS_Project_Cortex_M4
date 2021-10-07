#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////
//V1.3修改说明 
//支持适应不同频率下的串口波特率设置.
//加入了对printf的支持
//增加了串口接收命令功能.
//修正了printf第一个字符丢失的bug
//V1.4修改说明
//1,修改串口初始化IO的bug
//2,修改了USART_RX_STA,使得串口最大接收字节数为2的14次方
//3,增加了USART_REC_LEN,用于定义串口最大允许接收的字节数(不大于2的14次方)
//4,修改了EN_USART1_RX的使能方式
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_RX_LEN  			1024  	//定义最大接收缓冲区
#define USART_TX_LEN  			1024  	//定义最大发送缓冲区

extern u8  USART_RX_BUF[USART_RX_LEN]; //接收缓冲,最大USART_REC_LEN个字节
extern u8  USART_TX_BUF[USART_TX_LEN]; //接收缓冲,最大USART_REC_LEN个字节

void uart_init(u32 bound);
void UART_SendData(const u8 *data_buf, u32 data_len);

#endif


