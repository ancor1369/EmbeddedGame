/*
 * gameInit.h
 *
 *  Created on: Nov. 23, 2019
 *      Author: Andres Rogelio Cordoba
 *		Email: ancor1369@gmail.com
 */

#ifndef GAMETASKS_H_
#define GAMETASKS_H_
#include <llCharacter.h>
#include <stdio.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "KYPD.h"
#include "uart.h"
#include <time.h>

#define sWidth 128
#define sHight 64
#define BuferSize 12



/*
 * Name: GameInit
 * Parameters: Void
 * Description: Creates the initial state of the game making sure that everythin
 * is in place as needed to start a new game.
 */
void vGameInit();

/*
 * Name: vTaskrunGame
 * Parameters: pvParameters that is not going to carry any usefull information for
 * this task.
 * Description: Initialized the needed data structures to host the game and
 * Additionally creates the tasks that will be done in all the game.
 */
void vTaskRunGame(void *pvParameters);

/*
 * Name: vTaskSendData
 * Parameters: a pvParameter to send initialization variables to the task
 * Description: Constantly verifies what is the current state of the
 * characters on the screen and makes sure that
 */

void vTaskSendData(void *pvParameters);


/*
 * Name:vKeyPadTask
 * Description: This task handles the user inputs
 * by getting the keys that are pressed and making sure
 * everything is in place to make the correct implementation.
 * This is an approach that runs in a polling mode. This task
 * deletes any task that needs to disapear. Objects in the
 * linked lists are to be deleted on their corresponding
 * processing task.
 */

void vKeyPadTask(void *pvParameters);

/*
 * Name: vPlayerTask
 * Parameters: pvParameters is an object that holds the information necessary
 * for the task to run correctly.
 * Description:Minds about where the tank is located at all times, updates
 * the coordinates of the player, everything by taking into account the
 * the user input.
 */

void vPlayerTask(void *pvParameters);

/*
 * Name: vAlientTask
 * Parameters: pvParameters: it is optional to be used
 * Description: Processes all the alliens in the game, providing them
 * with all the personality characteristics they will hold so that the
 * game gains some interesting insights, It will delete the aliens from the
 * linked list as those are ordered to be deleted by the collisions task.
 */
void vAlientTask(void *pvParameters);

/*
 * Name:vMissileTans
 * Parameters: pvParameters: At this point it is optional to be used
 * Description: This task constantly takes care of the different positions and
 * Behavior the aliens will exhibit on the screen.
 */

static void vMissileTask(void *pvParameters);

/*
 *Name: vCreateMissile
 *Parameters: none
 *Description: This function set a new Missile task to run, if there is one of those
 *tasks running, it will just relaunch the missile from the new location.
 */

void vCreateMissile();

/*
 * Name:iCalculateDistance
 * Parameters: Takes two pointers to two different objects
 * Description: Calculates distance between two objects on the screen
 */

int iCalculateDistance(go_character *a, go_character * b);


void vTaskCollision();
#endif /* GAMETASKS_H_ */
