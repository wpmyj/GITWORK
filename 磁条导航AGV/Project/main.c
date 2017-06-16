#include "hardware.h"
#include "rs485communicate.h"
#include "magserv.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "commtype.h"
#include "rs485config.h"

void speed_send_task(void *pvParameters);
void mag_sense_query_task(void *pvParameter);

TaskHandle_t    SpeedSendTaskHandle;//�ٶȷ���������
TaskHandle_t    MagSenseQueryHandle;//�Ŵ�������ѯ���

/*---------------------------------------------------------------------------------------------*/
int main(void)
{
	hardware_init();
	NVIC_configuration();
	/*�����ٶȷ�������*/
	xTaskCreate((TaskFunction_t)speed_send_task,
		(const char*)"SpeedSendTask",
		(u16)SPEEDSENDTASKSTK,
		(void*)NULL,
		(UBaseType_t)SPEEDSENDTASKPRIO,
		(TaskHandle_t *)&SpeedSendTaskHandle);

	/*�����Ŵ�������ѯ����*/
	xTaskCreate((TaskFunction_t)mag_sense_query_task,
		(const char*)"MagSenseQuery",
		(u16)MAGSENSEQUERYSTK,
		(void*)NULL,
		(UBaseType_t)MAGSENSEQUERYPRIO,
		(TaskHandle_t *)&MagSenseQueryHandle);
	vTaskStartScheduler();
}

/*---------------------------------------------------------------------------------------------*/
//�ٶȿ�������
void speed_send_task(void *pvParameters)
{
	pvParameters = (void *)pvParameters;
	while (1)
	{
		mag_to_speed();
		vTaskDelay(50);
	}
}
//�Ŵ�������ѯ����

/*---------------------------------------------------------------------------------------------*/
void mag_sense_query_task(void *pvParameters)
{
	static u8 TxBufferToQueryMagsense[7] = MAGQUERYMSG;  //�ŵ�����������ѯ����
	pvParameters = (void *)pvParameters;
	while (1)
	{
		usart_out(USART1, TxBufferToQueryMagsense, 7);
		vTaskDelay(200);
	}
}

