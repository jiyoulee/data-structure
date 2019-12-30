#include <stdlib.h> // malloc
#include <stdio.h>
#include <ctype.h> // toupper
#include <string.h> // strcmp

#define QUIT   1
#define INSERT   2
#define DELETE   3
#define PRINT   4
#define SEARCH   5

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
   int         data;
   struct node   *link;
} NODE;

typedef struct
{
   int      count;
   NODE   *pos;
   NODE   *head;
   NODE   *rear;
} LIST;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a list head node and returns its address to caller
   return   head node pointer
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

/* returns   1 empty
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

/* internal search function
   searches list and passes back address of node
   containing target and its logical predecessor
   return   1 found
         0 not found
*/
static int _search( LIST *pList, NODE **pPre, NODE **pLoc, int argu)
{
   int found;
   
   *pPre = NULL;
   *pLoc = pList -> head;
   
   while(*pLoc != NULL && argu > (*pLoc) -> data)
   {
      *pPre = *pLoc;
      *pLoc = (*pLoc) -> link;
   }
   
   if(*pLoc == NULL)
      found = 0;
   else
      if(argu == (*pLoc) -> data)
         found = 1;
      else
         found = 0;
   
   return found;
};

/* internal insert function
   inserts data into a new node
   return   1 if successfulful
         0 if memory overflow
*/
static int _insert( LIST *pList, NODE *pPre, int dataIn)
{
   int successful;
   
   NODE *pNew = (NODE *)malloc(sizeof(NODE));
   pNew -> link = NULL;
   
   if(!pNew)
   {
      successful = 0;
   }
   else
   {      
      pNew -> data = dataIn;
      
      if(pPre == NULL)
      {
         pNew -> link = pList -> head;
         pList -> head = pNew;
      }
      else
      {
         pNew -> link = pPre -> link;
         pPre -> link = pNew;
      }
      
      pList -> count += 1;
	  
	  successful = 1;
   }
   
   return successful;
};

/* Inserts data into list
   return   -1 if overflow
         0 if successfulful
         1 if dupe key
*/
int addNode( LIST *pList, int dataIn)
{
	int successful;
	
	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	int found = _search(pList, &pPre, &pLoc, dataIn);
	int insert;
	
	if(found)
		successful = 1;
	else
	{
		insert = _insert(pList, pPre, dataIn);
		if(insert)
			successful = 0;
		else
			successful = -1;
	}
	
	return successful;
};

/* prints data from list
*/
void printList( LIST *pList)
{
	pList -> pos = pList -> head;
   
	while(pList -> pos != NULL)
	{
		printf("%d->", pList -> pos -> data);
		pList -> pos = pList -> pos -> link;
	}
	printf("NULL\n");
	
	pList -> pos = NULL;
   
	return;
};

/* interface to search function
   Argu   key being sought
   dataOut   contains found data
   return   1 successfulful
         0 not found
*/
int searchList( LIST *pList, int Argu, int *dataOut)
{
   NODE *pPre = NULL;
   NODE *pLoc = NULL;
   int found = _search(pList, &pPre, &pLoc, Argu);
   
   if(found)
      *dataOut = pLoc -> data;
   else
      dataOut = NULL;
   
   return found;
};

/* internal delete function
   deletes data from a list and saves the (deleted) data to dataOut
*/
static void _delete( LIST *pList, NODE *pPre, NODE *pLoc, int *dataOut)
{
   *dataOut = pLoc -> data;
   
   if(pPre == NULL)
   {
      pList -> head = pLoc -> link;
      pList -> pos = pLoc -> link;
   }
   else
      pPre -> link = pLoc -> link;
   
   pList -> count -= 1;
   
   free(pLoc);
   
   return;
};

/* Removes data from list
   return   0 not found
         1 deleted
*/
int removeNode( LIST *pList, int Key, int *dataOut)
{
	int successful;

	NODE *pPre = NULL;
	NODE *pLoc = NULL;
	int found = _search(pList, &pPre, &pLoc, Key);
	
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
   return   NULL head pointer
*/
LIST *destroyList( LIST *pList)
{  
	while(pList -> head != NULL)
	{
		pList -> pos = pList -> head;
		pList -> head = pList -> head -> link;
		
		free(pList -> pos);
	}
	free(pList);
   
   pList = NULL;
		
   return pList;
};

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
      case 'P':
         return PRINT;
      case 'I':
         return INSERT;
      case 'D':
         return DELETE;
      case 'S':
         return SEARCH;
   }
   return 0; // undefined action
}

////////////////////////////////////////////////////////////////////////////////
int main( void)
{
   int num;
   LIST *list;
   int data;
   
   // creates a null list
   list = createList();
   if (!list)
   {
      printf( "Cannot create list\n");
      return 100;
   }
   
   fprintf( stdout, "Select Q)uit, P)rint, I)nsert, D)elete, or S)earch: ");
   
   while(1)
   {
      int action = get_action();
      
      switch( action)
      {
         case QUIT:
            // destroy function call
            destroyList( list);
            return 0;
         
         case PRINT:
            // print function call
            printList( list);
            break;
            
         case INSERT:
            fprintf( stdout, "Enter a number to insert: ");
            fscanf( stdin, "%d", &num);
            
            // insert function call
            addNode( list, num);
            
            // print function call
            printList( list);
            break;
            
         case DELETE:
            fprintf( stdout, "Enter a number to delete: ");
            fscanf( stdin, "%d", &num);
            
            // delete function call
            removeNode( list, num, &data);
            
            // print function call
            printList( list);
            break;
         
         case SEARCH:
            fprintf( stdout, "Enter a number to retrieve: ");
            fscanf( stdin, "%d", &num);
            
            // search function call
            int found;
            found = searchList( list, num, &data);
            if (found) fprintf( stdout, "Found: %d\n", data);
            else fprintf( stdout, "Not found: %d\n", num);
            
            break;
      }
      if (action) fprintf( stdout, "Select Q)uit, P)rint, I)nsert, D)elete, or S)earch: ");
      
   }
   
   return 0;
}