/*
 * gameData.h
 *
 *  Created on: Nov. 23, 2019
 *      Author: Andres Rogelio Cordoba
 *		Email: ancor1369@gmail.com
 */

#include "FreeRTOS.h"

enum enumCharacter
{
	Tank = 1,
	Bullet,
	Small_Allien,
	Big_Allien
};
/*
 * Name: characterNames
 * Description: Contains the identifications that
 * will be used on the program to identify all the characters
 * that will be moving around the screen.
 */
typedef enum enumCharacter characterNames;

struct go_dimension
{
	unsigned int width;
	unsigned int height;
};
/*
 * Name: dimension
 * Description: Provides information of the height
 * and width of each one of the characters on the screen
 */
typedef struct go_dimension dimension;

struct go_coordinate
{
	unsigned int x;
	unsigned int y;
	unsigned int x_offset;
	unsigned int y_offset;
};
/*
 *	Name: coordinate
 *	Description: Holds the current coordinates of an object.
 */
typedef struct go_coordinate coordinate;

struct go_gameCharacter
{
	size_t objectID;
	characterNames characterID;
	coordinate go_Position;
	coordinate go_Speed;
	dimension dimensions;
	struct go_gameCaracter *pNext; //Pointer to the next location of the character

};
/*
 * Name: character
 * Description: This structure contains all the
 * information needed to manage one character on
 * the game. All the details needed control their
 * behavior on the board are described here.
 *
 * Parameters:
 * objectID: size_t, Identificator of the character on the screen
 * characterID: enum of characterNames, identifies the character on the screen
 * coordinate: struct coordinate, holds the location of the character on the screen
 * dimension: struct dimension, describe the size of the character on the screen
 */
typedef struct go_gameCharacter character;




//RESERVED: * pNext: chraracter pointer, allows chaining the character with any other on the code


