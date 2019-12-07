/************************************************************************/
/*																		*/
/*	KYPD.h	--	Declaration for KYPD library 	    				    */
/*																		*/
/************************************************************************/
/*	Author:		Michelle Yu												*/
/*	Copyright 2011, Digilent Inc.										*/
/************************************************************************/
/*  File Description:													*/
/*		This file declares functions for KYPD							*/
/* 		Adapted to C by Andres Rogelio Cordoba						    */
/*																		*/
/************************************************************************/
/*  Revision History:													*/
/*																		*/
/*	10/06/2011(MichelleY): created										*/
/*																		*/
/************************************************************************/
#if !defined(KYPD_H)
#define KYPD_H

#define KYPD_ROWNUM 4
#define KYPD_COLNUM 4


/* ------------------------------------------------------------ */
/*				Include File Definitions						*/
/* ------------------------------------------------------------ */

#include "../Drivers/KYPD.h"
#include "chip.h"
#include "board.h"

#include "FreeRTOS.h"
#include "task.h"

#define KYPD_GENERAL_PORT LPC_GPIO
#define KYPD_PORT LPC_GPIO2 			//Define the port needed for the target device
#define KYPD_PORT_NUM 2
/* ------------------------------------------------------------ */
/*					Procedure Declarations						*/
/* ------------------------------------------------------------ */



void begin(void);
void end(void);
void setPins(unsigned int* row, unsigned int* col);
void setKeyMap(int table[KYPD_COLNUM][KYPD_ROWNUM]);
int getKey(void);
int getKey_col(int colRow);
int getColRow(void);

unsigned int rowPins[KYPD_ROWNUM];
unsigned int colPins[KYPD_COLNUM];
int keyMap[KYPD_COLNUM][KYPD_ROWNUM];


#endif
