/*
 * gameTask.c
 *
 *  Created on: Nov. 23, 2019
 *      Author: Andres Rogelio Cordoba
 *		Email: ancor1369@gmail.com
 */

#define sWidt 128
#define sHight 64


#include "gameTasks.h"
#include "node.h"


node_t *pHeadChara = NULL;

//Allocating constants in stack
unsigned int charID = 0;
const dimension dimTank =
{
	.height = 24,
	.width = 24
};
const dimension dimAlien =
{
	.height = 16,
	.width = 16
};


void vGameInit()
{
	//Create a Tank, this one is going to be the head of the
	//Linked list that will contain those
	character tankOne;

	coordinate c ={
	    		.x = 10,
				.y = 10,
				.x_offset = 10 -(dimTank.height/2),
				.y_offset = 10 - (dimTank.width/2)
	    };

	tankOne.objectID = 0;
	tankOne.characterID = Tank;
    tankOne.dimensions = dimTank;
    tankOne.go_Position = c;


	pHeadChara = createHead(tankOne);

	coordinate b ={
		    		.x = sWidt - 10,
					.y = sHight - 10,
					.x_offset = (sWidt - 10) -(dimAlien.height/2),
					.y_offset = (sHight - 10) - (dimAlien.width/2)
		    };

	character alien;

	alien.objectID = 1;
	alien.characterID = Small_Allien;
	alien.dimensions = dimAlien;
	alien.go_Position = b;


	addNode(pHeadChara, alien);
}

void vTaskCollisions(void * pvParameters)
{
	vGameInit();

	while(1)
	{
		/*TODO: Create the physics engine inside this loop.
		 * Make sure every speed is calculated here
		 * and that all the coordinates to be
		 * sent on the serial interface are calculated here.
		 *
		 * Different tasks are created and destroyed inside this one
		*/
		vTaskDelay(configTICK_RATE_HZ/100);
	}
}
