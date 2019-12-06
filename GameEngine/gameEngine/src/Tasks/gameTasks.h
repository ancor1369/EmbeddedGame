/*
 * gameInit.h
 *
 *  Created on: Nov. 23, 2019
 *      Author: Andres Rogelio Cordoba
 *		Email: ancor1369@gmail.com
 */

#ifndef GAMETASKS_H_
#define GAMETASKS_H_
#include "FreeRTOS.h"
#include "task.h"
#include "gameTasks.h"
#include "commonData.h"
#include "KYPD.h"
#include "KYPD_Task.h"
#include "uart.h"

/*
 * Name: GameInit
 * Parameters: a pvParameter to send initialization variables to the task
 * Description: Creates the initial state of the game making sure that everythin
 * is in place as needed to start a new game.
 */
void vGameInit();
/*
 * Name: vTaskCollisions
 * Parameters: a pvParameter to send initialization variables to the task
 * Description: Constantly verifies what is the current state of the
 * characters on the screen and makes sure that
 */
void vTaskCollisions(void *pvParameters);

#endif /* GAMETASKS_H_ */
