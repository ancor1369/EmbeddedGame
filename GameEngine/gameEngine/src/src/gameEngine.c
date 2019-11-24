/*
 * node.c
 *
 *  Created on: Nov. 23, 2019
 *      Author: Andres Rogelio Cordoba
 *		Email: ancor1369@gmail.com
 */


#include "board.h"
#include "FreeRTOS.h"
#include "task.h"
#include "gameTasks.h"
#include "commonData.h"
#include "KYPD.h"
#include "KYPD_Task.h"
#include "uart.h"




pressedKey key;



/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();

	/* Initial LED0 state is off */
	Board_LED_Set(0, true);
	Board_LED_Set(1, true);
	Board_LED_Set(3, true);
}


/* LED1 toggle thread */
static void vLEDTask1(void *pvParameters) {
	bool LedState = false;

	while (1) {
		Board_LED_Set(0, LedState);
		LedState = (bool) !LedState;

		//About a 3Hz on/off toggle rate
		vTaskDelay(configTICK_RATE_HZ / 6);
	}
}

int main(void)
{
	prvSetupHardware();


	key.colRow = -1;
	key.key = -1;
	/* LED1 toggle thread */
	xTaskCreate(vLEDTask1, (signed char *) "vTaskLed1",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);


	xTaskCreate(vTaskUART,(signed char *) "CommunicationTask",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	xTaskCreate(KeyPadTask,(signed char *) "CommunicationTask",
					configMINIMAL_STACK_SIZE, &key, (tskIDLE_PRIORITY + 1UL),
					(xTaskHandle *) NULL);

	xTaskCreate(vTaskCollisions,(signed char *)"CollisionsTask",configMINIMAL_STACK_SIZE,
			NULL,(tskIDLE_PRIORITY + 5UL),(xTaskHandle*) NULL);

	/* Start the scheduler */
	vTaskStartScheduler();

	/* Should never arrive here */
	return 1;
}
