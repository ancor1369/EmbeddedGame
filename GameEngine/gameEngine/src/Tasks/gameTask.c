l/*
 * gameTask.c
 *
 *  Created on: Nov. 23, 2019
 *      Author: Andres Rogelio Cordoba
 *		Email: ancor1369@gmail.com
 */

#include "gameTasks.h"

//go_character variables that are used
//to obtain and manage all the characters
//that will be playing on the screen

go_character *pHeadPlayer = NULL;  //Head for the linked list representing the player
go_character *pHeadMissile = NULL; //Head for the linked list representing the Missiles
go_character *pHeadAlien = NULL;  //Head for the linked list representing the Aliens

xTaskHandle handle = NULL;

uint8_t numAliens = 0;
uint8_t numMissiles = 0;

char keyPressed = 'N'; // Variable taking care of what is the key pressed by the user. N means "None"

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
const dimension dimBullet =
{
	.height = 12,
	.width = 12
};

const dimension dimBigAllien =
{
		.height = 12,
		.width = 12
};

const coordinate bulletSpeed =
{
		.x = 3,
		.y = 0
};

/* LED1 toggle thread
 *
 * This tasks continues to be here to honor
 * the first days when we started  understanding
 * how a task is created :-)
 */
static void vLEDTask1(void *pvParameters) {
	bool LedState = false;
	while (1) {
		Board_LED_Set(0, LedState);
		LedState = (bool) !LedState;
		vTaskDelay(configTICK_RATE_HZ / 6);
	}
}

void vGameInit()
{
	/*
	 * Create initial characters on the screen.
	 * this ones are initializing the game
	 */

	character tankOne;
	coordinate c ={
	    		.x = 10,
				.y = 10,
				.x_offset = 5,
				.y_offset = 5
	    };

	tankOne.objectID = 0;
	tankOne.characterID = Tank;
    tankOne.dimensions = dimTank;
    tankOne.go_Position = c;
    tankOne.visible = true;
    //data structure gets initialized and ready to be processed
	pHeadPlayer = createHead(tankOne);
	pHeadPlayer->handle = NULL;

	coordinate b ={
		    		.x = sWidth - 10,
					.y = sHight - 10,
					.x_offset = (sWidth - 10) -(dimAlien.height/2),
					.y_offset = (sHight - 10) - (dimAlien.width/2)
		    };

	character alien;
	alien.objectID = numAliens;
	alien.characterID = Small_Allien;
	alien.dimensions = dimAlien;
	alien.go_Position = b;
	alien.visible = true;
	//data structure gets initialized and ready to be processed
	pHeadAlien = createHead(alien);
	pHeadAlien->handle = NULL;

	coordinate a = {
			.x = 10,
			.y = 10,
			.x_offset= 5,
			.y_offset = 5
	};

	coordinate aa =
	{
		.x=3,
		.y=0
	};

	character missile;
	missile.objectID = numMissiles;
	missile.characterID = Bullet;
	missile.go_Position = a;
	missile.go_Speed = aa;
	missile.visible = false;
	//data structure gets initialized and ready to be processed
	pHeadMissile = createHead(missile);
	pHeadMissile->handle = NULL;

	/*
	 * Create the tasks that are needed to
	 * make the system to work
	 */

	xTaskCreate(vLEDTask1, (signed char *) "vTaskLed1",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	xTaskCreate(vTaskUART,(signed char *) "CommunicationTask",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	xTaskCreate(vKeyPadTask,(signed char *) "CommunicationTask",
					configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
					(xTaskHandle *) NULL);

	xTaskCreate(vTaskCollisions, (signed char *) "TaskCollisions",
							configMINIMAL_STACK_SIZE, NULL,(tskIDLE_PRIORITY + 3UL),
							(xTaskHandle *) NULL);

	xTaskCreate(vPlayerTask, (signed char *) "PlayerTask",
					configMINIMAL_STACK_SIZE, NULL,(tskIDLE_PRIORITY + 3UL),
					&pHeadPlayer->handle);


	//Start the scheduler
	vTaskStartScheduler();
}


void vPlayerTask(void *pvParameters)
{
	while(1)
	{
		switch(keyPressed)
		{
			//Up
			case '2':
			{
				pHeadPlayer->character.go_Position.y -= 1;
				if(pHeadPlayer->character.go_Position.y <= 0)
				{
					pHeadPlayer->character.go_Position.y = sHight;
				}
				keyPressed = 'N';
			}
			break;
			//Left
			case '4':
			{
				pHeadPlayer->character.go_Position.x -= 1;
				if(pHeadPlayer->character.go_Position.x <= 0)
				{
					pHeadPlayer->character.go_Position.x = sWidth;
				}
				keyPressed = 'N';
			}
			break;
			//Right
			case '6':
			{
				pHeadPlayer->character.go_Position.x += 1;
				if(pHeadPlayer->character.go_Position.x >= sWidth)
				{
					pHeadPlayer->character.go_Position.x = 0;
				}
				keyPressed = 'N';
			}
				break;
			//Down
			case '8':
			{
				pHeadPlayer->character.go_Position.y += 1;
				if(pHeadPlayer->character.go_Position.y >= sHight)
				{
					pHeadPlayer->character.go_Position.y = 0;
				}
				keyPressed = 'N';
			}
				break;
			case 'A':
			{
				/* Start all the necessary duties to make this happen.
				 * 1. Find out what is the current position for the tank
				 * 2. Set the coordinates for the missile
				 * 3. Get the handle for the new task
				 * 4. Initiate the new task for missiles so they go on their own
				 */
				numMissiles += 1;
				coordinate m =
				{
						.x = pHeadPlayer->character.go_Position.x,
						.y = pHeadPlayer->character.go_Position.y
				};
				character missile =
				{
					.objectID = numMissiles,
					.characterID = Bullet,
					.go_Position = m,
					.go_Speed = bulletSpeed,
					.dimensions = dimBullet,
					.visible = true
				};

				pHeadMissile->character.go_Position.x = pHeadPlayer->character.go_Position.x;
				pHeadMissile->character.go_Position.y = pHeadPlayer->character.go_Position.y;
				//add the node to the linked list
				go_character *pMisssile = addNode(pHeadMissile, missile);

				xTaskCreate(vMissileTask,(signed char *)"missileTask",
						configMINIMAL_STACK_SIZE,&numMissiles, (tskIDLE_PRIORITY + 1UL),
						//&pHeadMissile->handle);
						&pMisssile->handle);
				keyPressed = 'N';
			}
				break;
			case 'B':
			{
				go_character *pMisssile = getNode(pHeadMissile, 1);
				vTaskDelete(pMisssile->handle);
				keyPressed = 'N';
			}
			break;
			default:
				break;
		}
		vTaskDelay(configTICK_RATE_HZ/20);
	}
}


void vMissileTask(void * pvParameters)
{
	/*This task needs to get the missile that was assigned to the task and
	 * will update all its characteristics,speed
	 */

	//uint8_t missileNumber = (uint8_t*)pvParameters;
	go_character *missile = NULL;

	uint8_t missileNumber = 0;
	missile = pHeadMissile;
	//Advance the missile when it is running
    //missile = getNode(pHeadMissile, missileNumber);
	while(1)
	{
	  missile->character.go_Position.x += 1;

	  if(missile->character.go_Position.x >= sWidth)
	  {
		  missile->character.go_Position.x = 0;
	  }
	  vTaskDelay(configTICK_RATE_HZ/10);
	}
}

void vTaskCollisions(void * pvParameters)
{
	//this task takes all the linked lists containing the players and computes the
	//relationships between them, so it is possible to set some of them to disappear as consequence
	//of the collision between objects.

//	pHeadAlien;
//	pHeadMissile;
//	pHeadPlayer;
	char commit[] = "Z";
	char devider[] = "\r\n";
	char buffer[BuferSize];
	go_character *ptrCht;
	while(1)
	{
		//At the end of the day this is what needs to be done with all thepHeadMissile->character.go_Position.x = pHeadPlayer->character.go_Position.x;
		//Characters, which is to extract all their coordinates and use them
		//to send the whole message with a set of screen updates for the
		//Arduino.
		for(uint8_t i = 0;i<BuferSize;i++)
		{
			buffer[i]=' ';
		}
		//Simulate the package construction and then send the message through the interface
		sprintf(buffer,"%d,%d,%d,%d",1,pHeadPlayer->character.characterID,pHeadPlayer->character.go_Position.x,pHeadPlayer->character.go_Position.y);
		vSendMessage(&buffer,sizeof(buffer));
		vSendMessage(commit, sizeof(commit));
		vSendMessage(devider, sizeof(devider));

		bool cont = true;
		ptrCht = pHeadMissile;
		//while(cont)
		{
			//if(numMissiles == 1)
			{

				for(uint8_t i = 0;i<BuferSize;i++)
				{
					buffer[i]=' ';
				}
				sprintf(buffer,"%d,%d,%d,%d",1,ptrCht->character.characterID,ptrCht->character.go_Position.x,ptrCht->character.go_Position.y);
				vSendMessage(&buffer,sizeof(buffer));
				vSendMessage(commit, sizeof(commit));
				vSendMessage(devider, sizeof(devider));
//				if(ptrCht->pNext == NULL)
//				{
//					cont = false;
//				}
//				ptrCht = ptrCht->pNext;
			}
		}


		//changes done to take the whole thinking up and running.

		//Run all the linked list and make sure it sends all the coordinates of all the characters

//		bool cont = true;
//		ptrCht = pHeadAlien;
//		while(cont)
//		{
//			for(uint8_t i = 0;i<BuferSize;i++)
//			{
//				buffer[i]=' ';
//			}
//			sprintf(buffer,"%d,%d,%d,%d",1,ptrCht->character.characterID,ptrCht->character.go_Position.x,ptrCht->character.go_Position.y);
//			vSendMessage(&buffer,sizeof(buffer));
//			vSendMessage(commit, sizeof(commit));
//			vSendMessage(devider, sizeof(devider));
//			ptrCht = ptrCht->pNext;
//			if(ptrCht->pNext == NULL)
//			{
//				cont = false;
//			}
//		}


		/*TODO: Create the physics engine inside this loop.
		 * Make sure every speed is calculated here
		 * and that all the coordinates to be
		 * sent on the serial interface are calculated hmissileNumberere.
		 *
		 * Different tasks are created and destroyed inside this one
		*/

		//This vTaskDelay defines how often the information is sent to the slave
		vTaskDelay(configTICK_RATE_HZ/20);
	}
}


/*
 * Driver tasks
 */

void vKeyPadTask(void *pvParameters)
{
	unsigned int col[4]={4,3,2,1};
	unsigned int row[4]={8,7,6,5};

	int keyTable[4][4]={{0, 1, 2, 3},    //col 1
	                     {4, 5, 6, 7},    //col 2
	                     {8, 9, 10, 11},    //col 3
	                     {12, 13, 14, 15}};   //col 4

	char Letter [16]={'D','E','F','0','C','9','8','7','B','6','5','4','A','3','2','1'};

	setPins(row,col);
	setKeyMap(keyTable);
	begin();
	int kk = -1;
	while(1)
	{
		kk = getKey();
		if(kk != -1)
		{
			char result = Letter[kk];
			keyPressed = result;
			kk = -1;
		}
		vTaskDelay(configTICK_RATE_HZ/30);
	}
}
