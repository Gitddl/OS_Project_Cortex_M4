/*******************************************************************************
*                 
*                 		       普中科技
--------------------------------------------------------------------------------
* 实 验 名		 : LED闪烁（使用位带操作）
* 实验说明       : 
* 连接方式       : 
* 注    意		 : 位带操作程序在system.h内
*******************************************************************************/

/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stm32f4xx.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "task.h"

/*******************************************************************************
* 函 数 名         : delay
* 函数功能		   : 延时函数，通过while循环占用CPU，达到延时功能
* 输    入         : i
* 输    出         : 无
*******************************************************************************/
void delay(u32 i)
{
	while(i--);
}

#define START_TASK_PRIO		1
#define START_STK_SIZE		128
TaskHandle_t StartTask_Handler;
void start_task(void *pvParmeters);

#define LED1_TASK_PRIO		2
#define LED1_STK_SIZE		128
TaskHandle_t LED1Task_Handler;
void led1_task(void *pvParmeters);

/*******************************************************************************
* 函 数 名         : main
* 函数功能		   : 主函数
* 输    入         : 无
* 输    出         : 无
*******************************************************************************/
int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	delay_init(168);
	LED_Init();
	
	xTaskCreate((TaskFunction_t)	start_task,
				(const char*)		"start_task",
				(uint16_t)			START_STK_SIZE,
				(void*)				NULL,
				(UBaseType_t)		START_TASK_PRIO,
				(TaskHandle_t*)		&StartTask_Handler);
	vTaskStartScheduler();
	while(1);
}

void start_task(void *pvParmeters)
{
	taskENTER_CRITICAL();
	xTaskCreate(led1_task,
				"led1_task",
				LED1_STK_SIZE,
				NULL,
				LED1_TASK_PRIO,
				&LED1Task_Handler);
	
	vTaskDelete(StartTask_Handler);
	
	taskEXIT_CRITICAL();
}

void led1_task(void *pvParmeters)
{
	while(1)
	{
		led1 = !led1;
		vTaskDelay(500);
	}
}
