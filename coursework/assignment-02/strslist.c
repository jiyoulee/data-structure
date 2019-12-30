#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp

// User structure type definition
typedef struct 
{
	char	*token;
	int		freq;
} tTOKEN;

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	tTOKEN		*dataPtr;
	struct node	*link;
} NODE;

typedef struct
{
	int		count;
	NODE	*pos;
	NODE	*head;
	NODE	*rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a list head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
LIST *createList( void)
{
	LIST *pList = (LIST *)malloc(sizeof(LIST));
	
	if(pList)
	{
		pList -> head = NULL;
		pList -> pos = NULL;
		pList -> rear = NULL;
		pList -> count = 0;
	}
	
	return pList;
};

/* returns number of nodes in list
*/
int listCount( LIST *pList)
{
	return pList -> count;
};

/* returns	1 empty
			0 list has data
*/
int emptyList( LIST *pList)
{
	int empty;
	int count = listCount(pList);
	
	switch(count)
	{
		case 0:
		empty = 1;
		break;
		default:
		empty = 0;
		break;
	}
	
	return empty;
};

//int fullList( LIST *pList);

/* Allocates dynamic memory for a token structure, initialize fields(token, freq) and returns its address to caller
	return	token structure pointer
			NULL if overflow
*/
tTOKEN *createToken( char *str)
{
	tTOKEN *pToken = (tTOKEN *)malloc(sizeof(tTOKEN));
	
	if(pToken)
	{
		pToken -> token = strdup(str);
		pToken -> freq = 1;
	}
	
	return pToken;
};

/* internal search function
	searches list and passes back address of node
	containing target and its logical predecessor
	return	1 found
			0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, char *pArgu)
{
	int found;
	
	*pPre = NULL;
	*pLoc = pList -> head;
	
	while(*pLoc != NULL)
	{
		if(strcmp(pArgu, (*pLoc) -> dataPtr -> token) > 0)
		{
			*pPre = *pLoc;
			*pLoc = (*pLoc) -> link;
		}
		else
			break;
	}
	
	if(*pLoc == NULL)
		found = 0;
	else
		if(!strcmp(pArgu, (*pLoc) -> dataPtr -> token))
			found = 1;
		else
			found = 0;
		
	return found;
};

/* internal insert function
	inserts data into a new node
	return	1 if successful
			0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, tTOKEN *dataInPtr)
{
	int successful;
	
	NODE *pNew = (NODE *)malloc(sizeof(NODE));
	pNew -> dataPtr = NULL;
	pNew -> link = NULL;
	
	if(!pNew)
		successful = 0;
	else
	{	
		if(pPre == NULL)
		{
			pNew -> link = pList -> head;
			pList -> head = pNew;
			pNew -> dataPtr = dataInPtr;
		}
		else
		{
			pNew -> link = pPre -> link;
			pPre -> link = pNew;
			pNew -> dataPtr = dataInPtr;
		}
		
		pList -> count += 1;
		
		successful = 1;
	}
	
	return successful;
};

/* Inserts data into list
	return	-1 if overflow
			0 if successful
			1 if duplicated key
*/
int addNode( LIST *pList, tTOKEN *dataInPtr)
{
	int successful;
	
	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	int found = _search(pList, &pPre, &pLoc, (dataInPtr -> token));
	int insert;
	
	if(found)
	{
		pLoc -> dataPtr -> freq += 1;
		successful = 1;
	}
	else
	{
		insert = _insert(pList, pPre, dataInPtr);
		if(insert)
			successful = 0;
		else
			successful = -1;
	}
	
	return successful;
};

/* Deletes all data in token structure and recycles memory
	return	NULL head pointer
*/
tTOKEN *destroyToken( tTOKEN *pToken)
{
	free(pToken -> token);
	free(pToken);
	
	pToken = NULL;
	
	return pToken;
};

/* prints data from list
*/
void printList( LIST *pList)
{
	pList -> pos = pList -> head;
	
	while((pList -> pos) != NULL)
	{
		printf("%s\t%d\n", pList -> pos -> dataPtr -> token, pList -> pos -> dataPtr -> freq);
		pList -> pos = pList -> pos -> link;
	}
	
	pList -> pos = NULL;
	
	return;
};

/* Deletes all data in list and recycles memory
	return	NULL head pointer
*/
LIST *destroyList( LIST *pList)
{
	while(pList -> head != NULL)
	{
		pList -> pos = pList -> head;
		pList -> head = pList -> head -> link;
		
		free(pList -> pos -> dataPtr -> token);
		free(pList -> pos -> dataPtr);
		free(pList -> pos);
	}
	free(pList);
	
	pList = NULL;
	
	return pList;
};

/* Removes data from list
	return	0 not found
			1 deleted
*/
//int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut);

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
//int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut);

////////////////////////////////////////////////////////////////////////////////
int main( void)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;
	
	// creates a null list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	while(scanf( "%s", str) == 1)
	{	
		pToken = createToken( str);

		// insert function call
		ret = addNode( list, pToken);
		
		if (ret == 1) // duplicated 
			destroyToken( pToken);
	}
	// print function call
	printList( list);
	
	destroyList( list);
	
	return 0;
}