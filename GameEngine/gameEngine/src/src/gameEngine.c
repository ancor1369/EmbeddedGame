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

/* Sets up system hardware */
static void prvSetupHardware(void)
{
	SystemCoreClockUpdate();
	Board_Init();
	/* Initial LED0 state is off */
	Board_LED_Set(0, false);
	Board_LED_Set(1, false);
	Board_LED_Set(2, false);
}

/*
 * Name: main
 * Description: Entry point of the game engine,
 * From this point onwards, the systems will start taking care of the
 * various tasks that will be running.
 */
int main(void)
{
	prvSetupHardware();

	//Once the program gets into
	//this function, the game satarts to play
	vGameInit();

	/* Should never arrive here */
	return 1;
}
