/*
 * node.c
 *
 *  Created on: Nov. 23, 2019
 *      Author: Andres Rogelio Cordoba
 *		Email: ancor1369@gmail.com
 */

#include <node.h>
#include <stdlib.h>

void addNode(node_t* pHead, character data)
{
    //Create new node
    node_t *pNode = createNode();
    pNode->character = data;
    
    //Find the attatchement point for the new node
    //to be inserted
    
    node_t *pw; //working pointer
    pw = pHead;    
    while(pw->pNext != NULL)
    {
        pw = pw->pNext;
    }    
    pw->pNext = pNode;
    return;
}

node_t* createHead(character data)
{
    //creates the head of the whole
    node_t *pHead = createNode();
    pHead->character = data;
}

node_t* createNode(void)
{
    //Creates a dinamic space of memory to hold the new node
    node_t* pNew = (node_t*)pvPortMalloc(sizeof(node_t));
    pNew->pNext = NULL;
    return pNew;
}

void deleteNode(node_t* pHead, character target)
{
    node_t* pw= pHead; //Working pointer
    node_t* ppw=pHead; //Working parent's pointer
    
    while((pw->character.objectID != target.objectID))
    {
        ppw=pw;
        pw=pw->pNext;
    }    
    if((pw->pNext)!= NULL)    
    {
        node_t *pSave = pw->pNext;
        vPortFree((void*)pw);
        ppw->pNext = pSave;
    }
    else
    {
        printf("Node not found");
    }
}

void printNodes(node_t *pHead)
{
//    while( pHead->pNext != NULL)
//    {
//        printf("%d\n",pHead->data.id);
//        printf("%s\n",pHead->data.message);
//        pHead = pHead->pNext;
//    }
//    if(pHead->pNext == NULL)
//    {
//        printf("%d\n",pHead->data.id);
//        printf("%s\n",pHead->data.message);
//    }
//    return;
}

node_t* getNode(node_t* pHead, character target)
{
    node_t* pw; //Worker pointer
    pw = pHead;
    
    while(pw->character.objectID != target.objectID)
    {
        pw = pHead->pNext;
    }
    //Making sure the selected target is the one we are seeking
    if(pw->character.objectID == target.objectID)
    {
        return pw;
    }
    //It it is not, just return a null pointer.
    else
    {
        return NULL;
    }
}


