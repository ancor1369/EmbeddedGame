/*
 * KYOD_Task.c
 *
 *  Created on: Nov. 17, 2019
 *      Author: andres
 */

#include <commonData.h>
#include <KYPD_Task.h>
#include "../Drivers/uart.h"

void KeyPadTask(void *pvParameters)
{
	pressedKey *key = (pressedKey*)pvParameters;
	unsigned int col[4]={4,3,2,1};
	unsigned int row[4]={8,7,6,5};

	int  keyTable[4][4]={{0, 1, 2, 3},    //col 1
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
			Chip_UART_Send(UART_SELECTION, &result, 1);
			kk = -1;
		}
		vTaskDelay(configTICK_RATE_HZ/8);
	}
}
