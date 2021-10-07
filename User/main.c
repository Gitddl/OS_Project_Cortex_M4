
/* Scheduler includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

#include "stm32f4xx.h"
#include "sys.h"
#include "time.h"
#include "usart.h"
#include "dma.h"
#include "led.h"
#include "task.h"
#include "log.h"

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


int main()
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	time_systick_init(168);
	uart_ init(115200);
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
		UART_SendData("UART:LED Trigger\r\n",18);
		LOG("LOG:LED Trigger\r\n");
		led1 = !led1;
		vTaskDelay(500);
	}
}


/**/