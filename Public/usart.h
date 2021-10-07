#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "stm32f4xx_conf.h"
#include "sys.h" 
//////////////////////////////////////////////////////////////////////////////////
//V1.3�޸�˵�� 
//֧����Ӧ��ͬƵ���µĴ��ڲ���������.
//�����˶�printf��֧��
//�����˴��ڽ��������.
//������printf��һ���ַ���ʧ��bug
//V1.4�޸�˵��
//1,�޸Ĵ��ڳ�ʼ��IO��bug
//2,�޸���USART_RX_STA,ʹ�ô����������ֽ���Ϊ2��14�η�
//3,������USART_REC_LEN,���ڶ��崮�����������յ��ֽ���(������2��14�η�)
//4,�޸���EN_USART1_RX��ʹ�ܷ�ʽ
////////////////////////////////////////////////////////////////////////////////// 	
#define USART_RX_LEN  			1024  	//���������ջ�����
#define USART_TX_LEN  			1024  	//��������ͻ�����

extern u8  USART_RX_BUF[USART_RX_LEN]; //���ջ���,���USART_REC_LEN���ֽ�
extern u8  USART_TX_BUF[USART_TX_LEN]; //���ջ���,���USART_REC_LEN���ֽ�

void uart_init(u32 bound);
void UART_SendData(const u8 *data_buf, u32 data_len);

#endif


