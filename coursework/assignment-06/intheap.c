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

HEAP *heapCreate( int capacity){
   HEAP *heap = (HEAP *)malloc(sizeof(HEAP));
   
   if(heap){
      heap -> heapArr = (int *)malloc(sizeof(int) * capacity);
	  for(int i = 0; i < capacity; i++){
		  heap -> heapArr[i] = 0;
	  }
      heap -> last = -1;
      heap -> capacity = capacity;
   }
   
   return heap;
};

static void _reheapUp( HEAP *heap, int index){
   int nodeIdx = index;
   int parentIdx;
   int temp;
   
   if(nodeIdx != 0){
      parentIdx = (index - 1) / 2;
      if((heap -> heapArr[nodeIdx]) > (heap -> heapArr[parentIdx])){
         temp = heap -> heapArr[nodeIdx];
         heap -> heapArr[nodeIdx] = heap -> heapArr[parentIdx];
         heap -> heapArr[parentIdx] = temp;
         _reheapUp(heap, parentIdx);
      }
   }
   
   return;
};

int heapInsert( HEAP *heap, int data){
   int success;

   if(((heap -> last) + 1) >= (heap -> capacity)) // stack overflow
      success = 0;
   else{
      heap -> heapArr[(heap -> last) + 1] = data;
      _reheapUp(heap, (heap -> last) + 1);
      (heap -> last)++;
      success = 1;
   }

   return success;
};

static void _reheapDown( HEAP *heap, int index){
	int nodeIdx = index;
	int child1Idx;
	int child2Idx;
	int temp;
	
	//root인 경우 처리?
	
	child1Idx = nodeIdx * 2 + 1; // left child
	child2Idx = nodeIdx * 2 + 2; // right child
	
	if(child1Idx <= 20 && child2Idx < 20){
		if(heap -> heapArr[nodeIdx] < heap -> heapArr[child1Idx] ||
		heap -> heapArr[nodeIdx] < heap -> heapArr[child2Idx]){
			if(heap -> heapArr[child1Idx] > heap -> heapArr[child2Idx]){
				temp = heap -> heapArr[nodeIdx];
				heap -> heapArr[nodeIdx] = heap -> heapArr[child1Idx];
				heap -> heapArr[child1Idx] = temp;
				_reheapDown(heap, child1Idx);
			}
			else{
				temp = heap -> heapArr[nodeIdx];
				heap -> heapArr[nodeIdx] = heap -> heapArr[child2Idx];
				heap -> heapArr[child2Idx] = temp;
				_reheapDown(heap, child2Idx);
			}
		}
	}
	
	return;
};

int heapDelete( HEAP *heap, int* data){
	int success;
	
	if(heap -> last < 0) // stack underflow
		success = 0;
	else{
		*data = heap -> heapArr[0];
		heap -> heapArr[0] = heap -> heapArr[heap -> last];
		_reheapDown(heap, 0);
		heap -> heapArr[heap -> last] = 0;
		(heap -> last)--;
		success = 1;
	}
	
	return success;
};

void heapPrint( HEAP *heap){
   for(int i = 0; i <= heap -> last; i++){
      printf("     %d", heap -> heapArr[i]);
   }
   printf("\n");
};

void heapDestroy( HEAP *heap){
	free(heap -> heapArr);
	free(heap);
	
	return;
};

int main(void)
{
	HEAP *heap;
	int data;
	
	heap = heapCreate(MAX_ELEM);
	
	srand( time(NULL));
	
	for (int i = 1; i < MAX_ELEM; i++)
	{
		data = rand() % MAX_ELEM * 3 + 1; // 1 ~ MAX_ELEM*3 random number
				
		fprintf( stdout, "Inserting %d: ", data);
		
		// insert function call
		heapInsert( heap, data);
		
		heapPrint( heap);
 	}

	while (heap->last >= 0)
	{
		// delete function call
		heapDelete( heap, &data);

		fprintf( stdout, "Deleting %d: ", data);

		heapPrint( heap);
 	}
	
	heapDestroy( heap);
	
	return 0;
}