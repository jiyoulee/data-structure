#include <stdio.h>
#include <stdlib.h> // malloc, rand
#include <time.h> // time
#include <assert.h>

#define MAX_ELEM	20

typedef struct
{
	int *heapArr;
	int	last;
	int	capacity;
} HEAP;

HEAP *heapCreate( int capacity);
void heapDestroy( HEAP *heap);
int heapInsert( HEAP *heap, int data);
static void _reheapUp( HEAP *heap, int index);
int heapDelete( HEAP *heap, int* data);
static void _reheapDown( HEAP *heap, int index);
void heapPrint( HEAP *heap);

int main(void)
{
	HEAP *heap;
	int data;
	
	//heap = heapCreate(MAX_ELEM);
	
	srand( time(NULL));
	
	for (int i = 1; i < MAX_ELEM; i++)
	{
		data = rand() % MAX_ELEM * 3 + 1; // 1 ~ MAX_ELEM*3 random number
				
		fprintf( stdout, "Inserting %d: ", data);
		
		// insert function call
		//heapInsert( heap, data);
		
		//heapPrint( heap);
 	}

	//while (heap->last >= 0)
	{
		// delete function call
		//heapDelete( heap, &data);

		//fprintf( stdout, "Deleting %d: ", data);

		//heapPrint( heap);
 	}
	
	//heapDestroy( heap);
	
	return 0;
}
