/*
 * character.c
 *
 *  Created on: Nov. 23, 2019
 *      Author: Andres Rogelio Cordoba
 *		Email: ancor1369@gmail.com
 */

#include <llCharacter.h>
#include <stdlib.h>

go_character* addNode(go_character* pHead, character data)
{
    //Create new node
	go_character *pNode = createNode();
    pNode->character = data;
    
    //Find the attatchement point for the new node
    //to be inserted
    
    go_character *pw = NULL; //working pointer
    pw = pHead;    
    while(pw->pNext != NULL)
    {
        pw = pw->pNext;
    }    
    pw->pNext = pNode;
    return pNode;
}

go_character* createHead(character data)
{
    //creates the head of the whole linked list
	go_character *pHead = createNode();
    pHead->character = data;
    return pHead;
}

go_character* createNode(void)
{
    //Creates a dynamic space of memory to hold the new node
	go_character* pNew = (go_character*)pvPortMalloc(sizeof(go_character));
    pNew->pNext = NULL;
    return pNew;
}

void deleteNode(go_character* pHead, uint8_t target)
{
	go_character* pw= pHead; //Working pointer
	go_character* ppw=pHead; //Working parent's pointer
    
    while((pw->character.objectID != target))
    {
        ppw=pw;
        pw=pw->pNext;
    }    
    if((pw->pNext)!= NULL)    
    {
    	go_character *pSave = pw->pNext;
        vPortFree((void*)pw);
        ppw->pNext = pSave;
    }
    else
    {
        printf("Node not found\n");
    }
}

void printNodes(go_character *pHead)
{
}

go_character* getNode(go_character* pHead, uint8_t target)
{
	go_character* pw = NULL; //Worker pointer
    pw = pHead;
    
    while(pw->character.objectID != target)
    {
        pw = pHead->pNext;
    }
    //Making sure the selected target is the one we are seeking
    if(pw->character.objectID == target)
    {
        return pw;
    }
    //It it is not, just return a null pointer.
    else
    {
        return NULL;
    }
}


