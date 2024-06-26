/**************/
/*   list.c   */
/**************/


#include <stdio.h>
#include <stdlib.h>
#include "list.h"


//////////////////////////////////////////
// Init
// Aim:		create new list
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
//////////////////////////////////////////
BOOL L_init(LIST* pList)
{
	if ( pList == NULL ) 	
		return False;	// no list to initialize

	pList->head.next = NULL;
	return True;
}


/////////////////////////////////////////////////////////////////
// Insert
// Aim:		add new node
// Input:	pointer to the node BEFORE the place for the new one
//			a value to be stored in the new node
// Output:	pointer to the new node
/////////////////////////////////////////////////////////////////
NODE* L_insert(NODE* pNode, DATA Value)
{
	NODE* tmp;

	if ( !pNode ) 
		return NULL;

	tmp = (NODE*)malloc(sizeof(NODE));	// new node

	if ( tmp != NULL )  {
		tmp->key = Value;
		tmp->next = pNode->next;
		pNode->next = tmp;
	}
	return tmp;
}


//////////////////////////////////////////////////////////////
// Delete
// Aim:		erase node
// Input:	pointer to the node BEFORE the node to be deleted 
// Output:	TRUE if succeeded
//////////////////////////////////////////////////////////////
BOOL L_delete(NODE* pNode, void (*freeData)(void*))
{
	NODE* tmp;
	if (!pNode)
		return False;
	tmp = pNode->next;
	if(!tmp)
		return False;
	
	pNode->next = tmp->next;
	if(freeData != NULL)
	{
		freeData(tmp->key);
	}
	free(tmp);
	return True;
}


/////////////////////////////////////////////////////////
// Find
// Aim:		search for a value
// Input:	pointer to the node to start with 
//			a value to be found
// Output:	pointer to the node containing the Value
/////////////////////////////////////////////////////////
const NODE* L_find(const NODE* pNode, DATA Value, int (*compare)(const void*, const void*))
{
	const NODE* tmp = pNode;

	while (tmp != NULL)
	{
		if(compare != NULL)
		{
			if(compare(tmp->key, Value))
				return tmp;
		}
		else if (tmp->key == Value)
			return tmp;
		tmp = tmp->next;
	}

	return NULL;
}


////////////////////////////////////////////////
// Free (additional function)
// Aim:		free the list memory
// Input:	pointer to the list structure
// Output:	TRUE if succeeded
////////////////////////////////////////////////
BOOL L_free(LIST* pList, void (*freeData)(void*))
{
	NODE *tmp;
	BOOL cont = True;
	if ( !pList ) 
		return False;

	tmp = &(pList->head);
	while (cont)
		cont = L_delete(tmp, freeData);
		
	return True;
}


////////////////////////////////////////////////
// Print (additional function)
// Aim:		print the list content (assume the DATA is int)
// Input:	pointer to the list structure
// Output:	a number of the printed elements
////////////////////////////////////////////////
int L_print(const LIST* pList, void (*printData)(const void*))
{
	NODE	*tmp;
	int		c = 0;

	if ( !pList ) 
		return False;

	printf("\n");
	tmp = pList->head.next;
	while(tmp != NULL)
	{
		//printf(" %d ---> ", tmp->key);
		printf("%d. ", c + 1);
		printData(tmp->key);
		c++;
		tmp = tmp->next;
	}
	printf("\n");
	return c;
}

int L_size(const LIST* pList)
{
	NODE* tmp;
	int	count = 0;

	if (!pList)
		return False;

	tmp = pList->head.next;
	while (tmp != NULL)
	{
		count++;
		tmp = tmp->next;
	}
	return count;
}

NODE* L_getAt(const LIST* pList, int index)
{
	NODE* tmp;
	int	count = 0;

	if (!pList)
		return NULL;

	tmp = pList->head.next;
	while (tmp != NULL)
	{
		if (count == index)
			return tmp;
		count++;
		tmp = tmp->next;
	}
	return NULL;
}
