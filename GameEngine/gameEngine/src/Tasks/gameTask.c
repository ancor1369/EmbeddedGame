/*
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
bool gameOver;

xTaskHandle handle = NULL;

uint8_t numAliens = 0;
uint8_t numMissiles = 0;

char keyPressed = 'N'; // Variable taking care of what is the key pressed by the user. N means "None"

//Allocating constants in stack
unsigned int charID = 0;
const dimension dimTank =
{
	.height = 16,
	.width = 16
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
	Board_LED_Set(1, false);
	Board_LED_Set(2, false);
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
	coordinate speed=
	{
			.x=1,
			.y=1
	};
	tankOne.objectID = 0;
	tankOne.characterID = Tank;
    tankOne.dimensions = dimTank;
    tankOne.go_Position = c;
    tankOne.go_Speed = speed;
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
	coordinate s={
			.x=40,
			.y=3
	};

	character alien;
	alien.objectID = numAliens;
	alien.characterID = Small_Allien;
	alien.dimensions = dimAlien;
	alien.go_Position = b;
	alien.go_Speed = s;
	alien.visible = true;
	//data structure gets initialized and ready to be processed
	pHeadAlien = createHead(alien);
	pHeadAlien->handle = NULL;

	coordinate a = {
			.x = 10,
			.y = -5,
			.x_offset= 5,
			.y_offset = 5
	};

	character missile;
	missile.objectID = numMissiles;
	missile.characterID = Bullet;
	missile.go_Position = a;
	missile.go_Speed = bulletSpeed;
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

	xTaskCreate(vTaskSendData, (signed char *) "TaskSendData",
							configMINIMAL_STACK_SIZE, NULL,(tskIDLE_PRIORITY + 1UL),
							(xTaskHandle *) NULL);

	xTaskCreate(vPlayerTask, (signed char *) "PlayerTask",
					configMINIMAL_STACK_SIZE, NULL,(tskIDLE_PRIORITY + 1UL),
					&pHeadPlayer->handle);

//	xTaskCreate(vMissileTask,(signed char *)"missileTask",
//							configMINIMAL_STACK_SIZE,0, (1UL),
//							&pHeadMissile->handle);

	xTaskCreate(vAlientTask,(signed char *)"alienTansk",
							configMINIMAL_STACK_SIZE,0, (1UL),
							&pHeadAlien->handle);

	//Higher priority task to check collisions.
	xTaskCreate(vTaskCollision,"Collisions",configMINIMAL_STACK_SIZE,NULL,2,NULL);



	vTaskStartScheduler();
}


void vCreateMissile()
{

	if(numMissiles == 0)
	{
		numMissiles += 1;
		coordinate m =
		{
			.x = pHeadPlayer->character.go_Position.x + dimTank.width +2,
			.y = pHeadPlayer->character.go_Position.y + dimTank.height/4
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
		//add the node to the linked list
		go_character *pMisssile = addNode(pHeadMissile, missile);
		char msg[BuferSize] = "Create: ";
		sprintf(msg,"MissileTask:%d",pMisssile->character.objectID);
		xTaskCreate(vMissileTask,(signed char *)msg,configMINIMAL_STACK_SIZE,(void*)numMissiles,1,&pMisssile->handle);
	}
	else
	{
		go_character *missile = getNode(pHeadMissile, 1);
		missile->character.go_Position.x = pHeadPlayer->character.go_Position.x;
		missile->character.go_Position.y = pHeadPlayer->character.go_Position.y+dimTank.height/4;
	}
}


int iCalculateDistance(go_character *a, go_character * b)
{
	return abs(a->character.go_Position.x - b->character.go_Position.x) + abs(a->character.go_Position.y - b->character.go_Position.y);
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
				pHeadPlayer->character.go_Position.y -= pHeadPlayer->character.go_Speed.y;
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
				pHeadPlayer->character.go_Position.x -= pHeadPlayer->character.go_Speed.x;
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
				pHeadPlayer->character.go_Position.x += pHeadPlayer->character.go_Speed.x;
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
				pHeadPlayer->character.go_Position.y += pHeadPlayer->character.go_Speed.y;
				if(pHeadPlayer->character.go_Position.y >= sHight)
				{
					pHeadPlayer->character.go_Position.y = 0;
				}
				keyPressed = 'N';
			}
				break;
			case '5':
			{

				vCreateMissile();
				keyPressed = 'N';
			}
				break;
			case 'B':
			{
				pHeadPlayer->character.go_Speed.y += 4;
				keyPressed = 'N';
			}
			break;
			case 'C':
			{
				pHeadPlayer->character.go_Speed.y -= 2;
				if(pHeadPlayer->character.go_Speed.y < 0)
				{
					pHeadPlayer->character.go_Speed.y = 1;
				}
				keyPressed = 'N';
			}
			break;
			case '0':
			{
				pHeadPlayer->character.go_Speed.x -= 2;
				if(pHeadPlayer->character.go_Speed.x < 0)
				{
					pHeadPlayer->character.go_Speed.x = 1;
				}
				keyPressed = 'N';
			}
			break;
			case 'F':
			{
				pHeadPlayer->character.go_Speed.x += 2;
				keyPressed = 'N';
			}
			break;
			default:
				break;
		}
		vTaskDelay(configTICK_RATE_HZ/10);
	}
}


void vAlientTask(void *pvParameters)
{
	uint8_t *alienNumber = (uint8_t*)pvParameters;
	go_character *alien = getNode(pHeadAlien, (uint8_t)alienNumber);

	srand((unsigned int)alienNumber);
	uint8_t counter = 0;

	while(1)
	{
		counter++;
		if(counter == 20)
		{
			alien->character.go_Speed.x = (rand()%10-5);
			alien->character.go_Speed.y = (rand()%10-5);
			counter = 0;
		}
		alien->character.go_Position.x += alien->character.go_Speed.x;
		alien->character.go_Position.y += alien->character.go_Speed.y;
		  if(alien->character.go_Position.x >= sWidth)
		  {
			  alien->character.go_Position.x = 0;
		  }
		  if(alien->character.go_Position.x <= 0)
		  {
			  alien->character.go_Position.x = sWidth;
		  }
		  if(alien->character.go_Position.y >= sHight)
		  {
			  alien->character.go_Position.y = 0;
		  }
		  if(alien->character.go_Position.y <= 0)
		  {
			  alien->character.go_Position.y = sHight;
		  }
		vTaskDelay(configTICK_RATE_HZ/10);
	}
}

static void vMissileTask(void * pvParameters)
{
	uint8_t *missileNumber = (uint8_t*)pvParameters;
	go_character *missile = getNode(pHeadMissile, (uint8_t)missileNumber);
	while(1)
	{
	  missile->character.go_Position.x += missile->character.go_Speed.x;
	  if(missile->character.go_Position.x >= sWidth)
	  {
		  missile->character.go_Position.x = 0;
	  }
	  vTaskDelay(configTICK_RATE_HZ/10);
	}
}
void vTaskCollision()
{
	go_character *pw = NULL;
	int distance = 0;
	while(1)
	{
		if(gameOver ==false)
		{
			if(numMissiles == 1)
			{
				pw = getNode(pHeadMissile, 1);
				distance = iCalculateDistance(pHeadPlayer, pw);
				if(distance <= 5)
				{
					//Deleta player task
					vTaskDelete(pHeadPlayer->handle);
					gameOver = true;
				}
				distance = iCalculateDistance(pHeadAlien, pw);
				if(distance <= 5)
				{
					vTaskDelete(pHeadAlien->handle);
					gameOver = true;
				}

			}
			distance = iCalculateDistance(pHeadPlayer, pHeadAlien);
			if(distance <= 5)
			{
				//Stop game
				vTaskDelete(pHeadPlayer->handle);
				gameOver = true;
			}
		}
		vTaskDelay(configTICK_RATE_HZ/20);
	}
}
void vTaskSendData(void * pvParameters)
{
	//this task takes all the linked lists containing the players and computes the
	//relationships between them, so it is possible to set some of them to disappear as consequence
	//of the collision between objects.
	char commit[] = "Z";
	char devider[] = "\r\n";
	char buffer[BuferSize];
	go_character *ptrCht = NULL;
	go_character *pw = NULL; //working pointer


	while(1)
	{
		if(numMissiles == 0)
		{
			sprintf(buffer,"%d,%d,%d,%d",0,2,-15,1);
			vSendMessage(&buffer,sizeof(buffer));
			vSendMessage(commit, sizeof(commit));
			vSendMessage(devider, sizeof(devider));
		}

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


		if(numMissiles > 0)
		{
			ptrCht = getNode(pHeadMissile, 1);
			while(1)
			{

				//-------------------------------------------------------------------
				//This functionality is supposed to be done in the corresponding task
				//however the task refused completely to run correctly as I expected
//				  ptrCht->character.go_Position.x += ptrCht->character.go_Speed.x;
//				  if(ptrCht->character.go_Position.x >= sWidth)
//				  {
//					  ptrCht->character.go_Position.x = 0;
//				  }
				 ///Section that is supposed to run in a task
				  //------------------------------------------------------------------

				for(uint8_t i = 0;i<BuferSize;i++)
				{
					buffer[i]=' ';
				}
				sprintf(buffer,"%d,%d,%d,%d",ptrCht->character.objectID,ptrCht->character.characterID,
						ptrCht->character.go_Position.x,ptrCht->character.go_Position.y);
				vSendMessage(&buffer,sizeof(buffer));
				vSendMessage(commit, sizeof(commit));
				vSendMessage(devider, sizeof(devider));
				if(ptrCht->pNext == NULL)
				{
					break;
				}
				pw = ptrCht->pNext;
				ptrCht = pw;
			}
		}
		//Run all the linked list and make sure it sends all the coordinates of all the characters
		ptrCht = pHeadAlien;
		while(1)
		{
			for(uint8_t i = 0;i<BuferSize;i++)
			{
				buffer[i]=' ';
			}
			sprintf(buffer,"%d,%d,%d,%d",ptrCht->character.objectID,ptrCht->character.characterID,ptrCht->character.go_Position.x,ptrCht->character.go_Position.y);
			vSendMessage(&buffer,sizeof(buffer));
			vSendMessage(commit, sizeof(commit));
			vSendMessage(devider, sizeof(devider));
			if(ptrCht->pNext == NULL)
			{
				break;
			}
			ptrCht = ptrCht->pNext;
		}

		vTaskDelay(configTICK_RATE_HZ/10);
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
		vTaskDelay(configTICK_RATE_HZ/10);
	}
}
