#include <stdlib.h> // malloc
#include <stdio.h>
#include <string.h> // strdup, strcmp
#include <ctype.h> // toupper

#define QUIT			1
#define FORWARD_PRINT	2
#define BACKWARD_PRINT	3
#define DELETE			4

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
	struct node	*llink;
	struct node	*rlink;
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
	tTOKEN *dataPtr = (tTOKEN *)malloc(sizeof(tTOKEN));
	
	if(dataPtr)
	{
		dataPtr -> token = strdup(str);
		dataPtr -> freq = 1;
	}
	
	return dataPtr;
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
	
	while((*pLoc) != NULL)
	{
		if(strcmp(pArgu, ((*pLoc) -> dataPtr -> token)) > 0)
		{
			*pPre = *pLoc;
			*pLoc = ((*pLoc) -> rlink);
		}
		else
			break;
	}
	
	if((*pLoc) == NULL)
		found = 0;
	else
	{
		if(!strcmp(pArgu, ((*pLoc) -> dataPtr -> token)))
			found = 1;
		else
			found = 0;
	}
	
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
	
	int empty = emptyList(pList);
	NODE *pNew = (NODE *)malloc(sizeof(NODE));
	pNew -> dataPtr = NULL;
	pNew -> llink = NULL;
	pNew -> rlink = NULL;
	
	if(!pNew)
		successful = 0;
	else
	{
		pNew -> dataPtr = dataInPtr;
		
		if(pPre == NULL)
		{
			if(empty)
				pList -> rear = pNew;
			else
			{
			pNew -> rlink = pList -> head;
			pNew -> rlink -> llink = pNew;
			}
			pList -> head = pNew;
		}
		else
		{
			if((pPre -> rlink) == NULL)
				pList -> rear = pNew;
			else
			{
				pNew -> rlink = pPre -> rlink;
				pNew -> rlink -> llink = pNew;
			}
			pNew -> llink = pPre;
			pNew -> llink -> rlink = pNew;
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
	int found = _search(pList, &pPre, &pLoc, dataInPtr -> token);
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

/* prints data from list (forward)
*/
void printList( LIST *pList)
{
	pList -> pos = pList -> head;
	
	while(pList -> pos != NULL)
	{
		printf("%s\t%d\n", pList -> pos -> dataPtr -> token, pList -> pos -> dataPtr -> freq);
		pList -> pos = pList -> pos -> rlink;
	}
	
	pList -> pos = NULL;
	
	return;
};

/* prints data from list (backward)
*/
void printListR( LIST *pList)
{
	pList -> pos = pList -> rear;
	
	while(pList -> pos != NULL)
	{
		printf("%s\t%d\n", pList -> pos -> dataPtr -> token, pList -> pos -> dataPtr -> freq);
		pList -> pos = pList -> pos -> llink;
	}
	
	pList -> pos = NULL;
		
	return;
};

/* internal delete function
	deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, tTOKEN **dataOutPtr)
{
	*dataOutPtr = pLoc -> dataPtr;
	
	if(pPre == NULL) // first node
	{
		pList -> head = pLoc  -> rlink;
		pLoc -> rlink -> llink = NULL;
		pLoc -> rlink = NULL;
	}
	else
	{
		if(pLoc -> rlink == NULL) // last node
		{
			pList -> rear = pLoc -> llink;
			pLoc -> llink -> rlink = NULL;
			pLoc -> llink = NULL;
		}
		else
		{
			pLoc -> rlink -> llink = pLoc -> llink;
			pLoc -> llink -> rlink = pLoc -> rlink;
			pLoc -> rlink = NULL;
			pLoc -> llink = NULL;
		}
	}
	
	pList -> count -= 1;
	
	free(pLoc);
	
	return;
};

/* Removes data from list
	return	0 not found
			1 deleted
*/
int removeNode( LIST *pList, char *keyPtr, tTOKEN **dataOut)
{
	int successful;
	
	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	int found = _search(pList, &pPre, &pLoc, keyPtr);
	
	if(found)
	{
		_delete(pList, pPre, pLoc, dataOut);
		successful = 1;
	}
	else
		successful = 0;
	
	return successful;
};

/* Deletes all data in list and recycles memory
	return	NULL head pointer
*/
LIST *destroyList( LIST *pList)
{
	while(pList -> head != NULL)
	{
		pList -> pos = pList -> head;
		pList -> head = pList -> head -> rlink;
		
		free(pList -> pos -> dataPtr -> token);
		free(pList -> pos -> dataPtr);
		free(pList -> pos);
	}
	free(pList);
	
	pList = NULL;
	
	return pList;
};

/* interface to search function
	Argu	key being sought
	dataOut	contains found data
	return	1 successful
			0 not found
*/
//int searchList( LIST *pList, char *pArgu, tTOKEN **pDataOut);

/* gets user's input
*/
int get_action()
{
	char ch;
	scanf( "%c", &ch);
	ch = toupper( ch);
	switch( ch)
	{
		case 'Q':
			return QUIT;
		case 'F':
			return FORWARD_PRINT;
		case 'B':
			return BACKWARD_PRINT;
		case 'D':
			return DELETE;
	}
	return 0; // undefined action
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	LIST *list;
	char str[1024];
	tTOKEN *pToken;
	int ret;
	FILE *fp;
	
	if (argc != 2)
	{
		fprintf( stderr, "usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (!fp)
	{
		fprintf( stderr, "Error: cannot open file [%s]\n", argv[1]);
		return 2;
	}
	
	// creates a null list
	list = createList();
	if (!list)
	{
		printf( "Cannot create list\n");
		return 100;
	}
	
	while(fscanf( fp, "%s", str) == 1)
	{
		pToken = createToken( str);
		
		// insert function call
		ret = addNode( list, pToken);
		
		if (ret == 1) // duplicated 
			destroyToken( pToken);
	}
	
	fclose( fp);
	
	fprintf( stdout, "Select Q)uit, F)orward print, B)ackward print, D)elete: ");
	
	while (1)
	{
		int action = get_action();
		
		switch( action)
		{
			case QUIT:
				destroyList( list);
				return 0;
			
			case FORWARD_PRINT:
				printList( list);
				break;
			
			case BACKWARD_PRINT:
				printListR( list);
				break;
			
			case DELETE:
				fprintf( stdout, "Input a string to delete: ");
				fscanf( stdin, "%s", str);
				int ret = removeNode( list, str, &pToken);
				if (ret)
				{
					fprintf( stdout, "%s deleted\n", pToken->token);
					destroyToken( pToken);
				}
				else fprintf( stdout, "%s not found\n", str);
				break;
			}
		
		if (action) fprintf( stdout, "Select Q)uit, F)orward print, B)ackward print, D)elete: ");
	}
	return 0;
}
