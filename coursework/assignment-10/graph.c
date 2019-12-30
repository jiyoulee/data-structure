#include <stdio.h>
#include <stdlib.h>

typedef struct graphHead{
	int 				count;
	struct graphVertex *firstVertex;
} Graph;

typedef struct graphVertex{
	struct graphVertex *nextVertex;
	int 				data;
	int 				degree;
	int 				processed;
	struct graphEdge   *firstEdge;
} Vertex;

typedef struct graphEdge{
	struct graphVertex *dstVertex;
	struct graphEdge   *nextEdge;
} Edge;

Graph * createGraph(void){
	Graph *newGraph = (Graph *)malloc(sizeof(Graph));
	
	if(newGraph){
		newGraph -> count = 0;
		newGraph -> firstVertex = NULL;
	}
	
	return newGraph;
}

Vertex * createVertex(int data){
	Vertex *newVertex = (Vertex *)malloc(sizeof(Vertex));
	
	if(newVertex){
		newVertex -> nextVertex = NULL;
		newVertex -> data = data;
		newVertex -> degree = 0;
		newVertex -> processed = 0;
		newVertex -> firstEdge = NULL;
	}
	
	return newVertex;
}

void insertVertex(Graph *graph, int data){
	Vertex *newVertex = createVertex(data);
	
	Vertex *root = graph -> firstVertex;
	Vertex *prevVertex = NULL;
	
	if(!(graph -> firstVertex))
		graph -> firstVertex = newVertex;
	else{
		while(root){
			if(root -> data < data){
				prevVertex = root;
				root = root -> nextVertex;
			}
		}
		
		if(!prevVertex){
			newVertex -> nextVertex = graph -> firstVertex -> nextVertex;
			graph -> firstVertex = newVertex;
		}
		else{
			newVertex -> nextVertex = prevVertex -> nextVertex;
			prevVertex -> nextVertex = newVertex;
		}
	}
	
	graph -> count += 1;
	
	return;
}

Edge * createEdge(void){
	Edge *newEdge = (Edge *)malloc(sizeof(Edge));
	
	if(newEdge){
		newEdge -> dstVertex = NULL;
		newEdge -> nextEdge = NULL;
	}
	
	return newEdge;
}

void addEdge(Graph *graph, int src, int dst){
	Edge *newEdge = createEdge();
	Vertex *srcVertex = NULL;
	Vertex *dstVertex = NULL;
	Vertex *rootVertex = NULL;
	Edge *rootEdge = NULL;
	Edge *prevEdge = NULL;
	
	rootVertex = graph -> firstVertex;
	while(1){
		if(rootVertex -> data == src){
			srcVertex = rootVertex;
			break;
		}
		
		rootVertex = rootVertex -> nextVertex;
	}
	
	rootVertex = graph -> firstVertex;
	while(1){
		if(rootVertex -> data == dst){
			dstVertex = rootVertex;
			break;
		}
		
		rootVertex = rootVertex -> nextVertex;
	}
	
	newEdge -> dstVertex = dstVertex;
	if(!(srcVertex -> firstEdge))
		srcVertex -> firstEdge = newEdge;
	else{
		rootEdge = srcVertex -> firstEdge;
		while(rootEdge != NULL && rootEdge -> dstVertex -> data < dst){
				prevEdge = rootEdge;
				rootEdge = rootEdge -> nextEdge;
		}
		
		if(!prevEdge){
			newEdge -> nextEdge = srcVertex -> firstEdge;
			srcVertex -> firstEdge = newEdge;
		}
		else{
			newEdge -> nextEdge = prevEdge -> nextEdge;
			prevEdge -> nextEdge = newEdge;
		}
	}
	
	srcVertex -> degree += 1;
	
	return;
}

void dfsTraverse(Graph *graph){
	Vertex *vertexL = NULL;
	Vertex *vertexP = NULL;
	Edge *edgeL = NULL;
	Vertex *stack[120];
	int stackL = 0;
	int stackCount = 0;
	
	for(int i = 0; i < 120; i++){
		stack[i] = 0;
	}
	
	if(graph){
		vertexL = graph -> firstVertex;
		for(int i = 0; i < (graph -> count); i++){
			vertexL -> processed = 0;
			vertexL = vertexL -> nextVertex;
		}
		
		vertexL = graph -> firstVertex;
		for(int i = 0; i < (graph -> count); i++){
			if((vertexL -> processed) == 0){
				stack[stackL] = vertexL;
				vertexL -> processed = 1;
				stackL++;
				stackCount++;
			}
			
			while(stackCount){
				stackL--;
				vertexP = stack[stackL];
				stack[stackL] = 0;
				stackCount--;
				printf("%d ", vertexP -> data);
				vertexP -> processed = 2;
				edgeL = vertexP -> firstEdge;
				for(int i = 0; i < (vertexP -> degree); i++){
					if((edgeL -> dstVertex -> processed) == 0){
						stack[stackL] = edgeL -> dstVertex;
						edgeL -> dstVertex -> processed = 1;
						stackL++;
						stackCount++;
					}
					edgeL = edgeL -> nextEdge;
				}
			}
			
			vertexL = vertexL -> nextVertex;
		}
	}
	
	return;
}

void bfsTraverse(Graph *graph){
	Vertex *vertexL = NULL;
	Vertex *vertexP = NULL;
	Edge *edgeL = NULL;
	Vertex *queue[120];
	int queueL = 0;
	int queueCount = 0;
	
	for(int i = 0; i < 120; i++){
		queue[i] = 0;
	}
	
	if(graph){
		vertexL = graph -> firstVertex;
		for(int i = 0; i < (graph -> count); i++){
			vertexL -> processed = 0;
			vertexL = vertexL -> nextVertex;
		}
		
		vertexL = graph -> firstVertex;
		for(int i = 0; i < (graph -> count); i++){
			if((vertexL -> processed) == 0){
				queue[queueL] = vertexL;
				vertexL -> processed = 1;
				queueL++;
				queueCount++;
			}
			
			while(queueCount){
				vertexP = queue[queueL - queueCount];
				queue[queueL - queueCount] = 0;
				queueCount--;
				printf("%d ", vertexP -> data);
				vertexP -> processed = 2;
				edgeL = vertexP -> firstEdge;
				for(int i = 0; i < (vertexP -> degree); i++){
					if((edgeL -> dstVertex -> processed) == 0){
						queue[queueL] = edgeL -> dstVertex;
						edgeL -> dstVertex -> processed = 1;
						queueL++;
						queueCount++;
					}
					edgeL = edgeL -> nextEdge;
				}
			}
			
			vertexL = vertexL -> nextVertex;
		}
	}
	
	return;
}

static void destroyEdges(Edge *edge){
	if(edge == NULL)
		return;
	else{
		destroyEdges(edge -> nextEdge);
		free(edge);
	}
}

static void destroyVertices(Vertex *vertex){
	if(vertex == NULL)
		return;
	else{
		destroyVertices(vertex -> nextVertex);
		destroyEdges(vertex-> firstEdge);
		free(vertex);
	}
}

void destroyGraph(Graph *graph){
	destroyVertices(graph -> firstVertex);
	free(graph);
	
	return;
}

int main(int argc, char **argv)
{
	FILE *fp;
	Graph *graph;
	char trash[9];
	int vertexCount;
	int a;
	int b;
	Edge *rootEdge = NULL;
	
	if (argc != 2)
	{
		fprintf( stderr, "Usage: %s FILE\n", argv[0]);
		return 1;
	}
	
	fp = fopen( argv[1], "rt");
	if (fp == NULL)
	{
		fprintf( stderr, "File open error: %s\n", argv[1]);
		return 1;
	}
	
	graph = createGraph(); 
	
	fscanf(fp, "%s %d", trash, &vertexCount);
	for(int i = 1; i <= vertexCount; i++){
		insertVertex(graph, i);
	}
	
	fscanf(fp, "%s", trash);
	while(fscanf(fp, "%d %d", &a, &b) == 2){
		addEdge(graph, a, b);
		addEdge(graph, b, a);
	}
	
	fclose( fp);
	
	fprintf(stdout, "DFS : ");
	dfsTraverse(graph);
	fprintf(stdout, "\n");
	
	fprintf(stdout, "BFS : ");
	bfsTraverse(graph);
	fprintf(stdout, "\n");
	
	destroyGraph(graph);
	
	return 0;
}
