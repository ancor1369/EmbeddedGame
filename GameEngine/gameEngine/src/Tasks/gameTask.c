/*
 * gameTask.c
 *
 *  Created on: Nov. 23, 2019
 *      Author: Andres Rogelio Cordoba
 *		Email: ancor1369@gmail.com
 */

#define sWidth 128
#define sHight 64
#define BuferSize 12

#include "gameTasks.h"

//Global variable that will be responsible to
//locate the character that is needed in the game, this one
//in particular represents the tank that the player can
//manage.

go_character *pHeadChara = NULL;
character tankOne;
char keyPressed = 'N';

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
	//Create a Tank, this one is going to be the head of the
	//Linked list that will contain those


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
    //tankOne.taskHandle = NULL;

	pHeadChara = createHead(tankOne);

//	coordinate b ={
//		    		.x = sWidthhh - 10,
//					.y = sHight - 10,
//					.x_offset = (sWidthhh - 10) -(dimAlien.height/2),
//					.y_offset = (sHight - 10) - (dimAlien.width/2)
//		    };
//
//	character alien;
//
//	alien.objectID = 1;
//	alien.characterID = Small_Allien;
//	alien.dimensions = dimAlien;
//	alien.go_Position = b;
//
//	addNode(pHeadChara, alien);
}


void vTaskRunGame(void *pvParameters)
{
	vGameInit();

	pressedKey key;
	key.colRow = -1;
	key.key = -1;
	/* LED1 toggle thread */
	xTaskCreate(vLEDTask1, (signed char *) "vTaskLed1",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	xTaskCreate(vTaskUART,(signed char *) "CommunicationTask",
				configMINIMAL_STACK_SIZE, NULL, (tskIDLE_PRIORITY + 1UL),
				(xTaskHandle *) NULL);

	xTaskCreate(vKeyPadTask,(signed char *) "CommunicationTask",
					configMINIMAL_STACK_SIZE, &key, (tskIDLE_PRIORITY + 1UL),
					(xTaskHandle *) NULL);

	xTaskCreate(vPlayerTask, (signed char *) "PlayerTask",
					configMINIMAL_STACK_SIZE, NULL,(tskIDLE_PRIORITY + 3UL),
					(xTaskHandle *) NULL);

	xTaskCreate(vTaskCollisions, (signed char *) "TaskCollisions",
						configMINIMAL_STACK_SIZE, NULL,(tskIDLE_PRIORITY + 3UL),
						(xTaskHandle *) NULL);



	char buff[] = "Tick\r\n";

	while(1)
	{
		vSendMessage(&buff,sizeof(buff));
		vTaskDelay(configTICK_RATE_HZ*2);
	}
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
				pHeadChara->character.go_Position.y -= 1;
				if(pHeadChara->character.go_Position.y <= 0)
				{
					pHeadChara->character.go_Position.y = sHight;
				}
				keyPressed = 'N';
			}
			break;
			//Left
			case '4':
			{
				pHeadChara->character.go_Position.x -= 1;
				if(pHeadChara->character.go_Position.x <= 0)
				{
					pHeadChara->character.go_Position.x = sWidth;
				}
				keyPressed = 'N';
			}
			break;
			//Right
			case '6':
			{
				pHeadChara->character.go_Position.x += 1;
				if(pHeadChara->character.go_Position.x >= sWidth)
				{
					pHeadChara->character.go_Position.x = 0;
				}
				keyPressed = 'N';
			}
				break;
			//Down
			case '8':
			{
				pHeadChara->character.go_Position.y += 1;
				if(pHeadChara->character.go_Position.y >= sHight)
				{
					pHeadChara->character.go_Position.y = 0;
				}
				keyPressed = 'N';
			}
				break;
			//Fire!, Yes Fire!!
			case 'A':
			{
				//TODO: Create a new task
			}
				break;
			default:
				break;
		}
		vTaskDelay(configTICK_RATE_HZ/20);
	}
}

void vTaskCollisions(void * pvParameters)
{
	char buffer[BuferSize];
	char commit[] = "Z";
	char devider[] = "\r\n";
	while(1)
	{
		for(uint8_t i = 0;i<BuferSize;i++)
		{
			buffer[i]=' ';
		}
		//Simulate the package construction and then send the message through the interface
		sprintf(buffer,"%d,%d,%d,%d",1,1,pHeadChara->character.go_Position.x,pHeadChara->character.go_Position.y);
		//It will send only the data so far
		vSendMessage(&buffer,sizeof(buffer));
		vSendMessage(commit, sizeof(commit));
		vSendMessage(devider, sizeof(devider));
		/*TODO: Create the physics engine inside this loop.
		 * Make sure every speed is calculated here
		 * and that all the coordinates to be
		 * sent on the serial interface are calculated here.
		 *
		 * Different tasks are created and destroyed inside this one
		*/
		vTaskDelay(configTICK_RATE_HZ/50);
	}
}

void vKeyPadTask(void *pvParameters)
{
	//pressedKey *key = (pressedKey*)pvParameters;
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
			//Chip_UART_Send(UART_SELECTION, &result, 1);
			kk = -1;
		}
		vTaskDelay(configTICK_RATE_HZ/30);
	}
}
