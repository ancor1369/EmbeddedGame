/*
 * node.h
 *
 *  Created on: Nov. 23, 2019
 *      Author: Andres Rogelio Cordoba
 *		Email: ancor1369@gmail.com
 */


/*Definition of the functions to be used in the code*/

#include <string.h>
#include "gameData.h"


struct node_struct
{
    character character;
    struct node_t *pNext;
};    
/*
 *  Name: node_t
 * Description:
 * This is the data structure to support a 
 * Linked list node. 
 */
typedef struct node_struct node_t;




/*
 * Name: CreateHead
 * data: infomation to be set as content of the head
 * Description:
 * Creates the first element to be used on the linked list.
 */
node_t* createHead(character data);
/*
 *  Name: addNodeE:
 *  Parameters:
 *  node_t* pHead, is a pointer to the head of the linked list
 *  data_struct  data, is the quantity of information of information required
 *  Description:
 *  This one is used to add one node once the 
 *  head exists, if the head has not been created, it is 
 *  better not to call this one. 
 * 
 */
void addNode(node_t* pHead, character data);
/*
 * Name: CreateNode
 * Parameters:
 * None
 * Description:
 * This method is used to allocate the needed
 * resources for a new node on the heap. The node 
 * is exclusively of the type node_t. It returns a 
 * pointer to the created node.
 */
node_t* createNode(void);
/*
 * Name: CreateHead 
 * Parameters: an data_struct variable
 * Description:
 * Creates the space in the heap for the first 
 * node to be hosted, it attatches the information
 * passed as parameter. this method is intended to 
 * be called once per linked list to be created.
 */
node_t* createHead(character data);

void printNodes(node_t *pHead);
/*
 * Name:deleteNode
 * Parameters:
 * node_t * pHead: Pointer to the head of the linked list
 * character targed: Node with the information of the character
 * to be eliminated from the linked list
 * Description: The method goes
 */
void deleteNode(node_t * pHead,character target);
/*
 * Name: getNode
 * Parameters: node_t pHead
 * Description:
 * Retrieves the pointer of the target data that is passed to the
 * function.
 */
node_t* getNode(node_t* pHead, character target);
    


