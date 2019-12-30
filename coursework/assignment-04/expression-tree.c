#include <stdlib.h> // malloc, atoi
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert

#define MAX_STACK_SIZE	50

////////////////////////////////////////////////////////////////////////////////
// LIST type definition
typedef struct node
{
	char		data;
	struct node	*left;
	struct node	*right;
} NODE;

typedef struct
{
	NODE	*root;
} TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a tree head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
TREE *createTree( void){
	TREE *pTree = (TREE *)malloc(sizeof(TREE));
	
	if(pTree)
	pTree -> root = NULL;
	
	return pTree;
}

/*  Allocates dynamic memory for a node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
static NODE *_makeNode( char ch){
	NODE *pNew = (NODE *)malloc(sizeof(NODE));
	
	if(pNew){
		pNew -> data = ch;
		pNew -> left = NULL;
		pNew -> right = NULL;
	}
	
	return pNew;
}

static void _destroy( NODE *root){
	if(isdigit(root -> data))
		free(root);
	else{
		_destroy(root -> left);
		_destroy(root -> right);
		free(root);
	}
	
	return;
};

/* converts postfix expression to binary tree
	return	1 success
			0 invalid postfix expression
*/
int postfix2tree( char *expr, TREE *pTree){
	int success = -2;
	
	NODE* stack[1024];
	NODE *pNew = NULL;
	int idxE = 0;
	int idxS = 0;
	int i = 0;
	
	for(int i = 0; i < 1024; i++){
		stack[i] = NULL;
	}
	
	while(expr[idxE] != '\0'){
		if(success == -1)
			break;

		switch(expr[idxE]){
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				pNew = _makeNode(expr[idxE]);
				stack[idxS] = pNew;
				pNew = NULL;
				idxE++;
				idxS++;
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				if(idxS < 2)
					success = -1;
				else{
					pNew = _makeNode(expr[idxE]);
					pNew -> right = stack[idxS - 1];
					stack[idxS -1] = NULL;
					pNew -> left = stack[idxS - 2];
					stack[idxS - 2] = NULL;
					stack[idxS -2] = pNew;
					pNew == NULL;
					idxE++;
					idxS--;
				}
				break;
			default:
				success = -1;
				break;
		}
	}
	
	if(success != -1){
		if(stack[0] != NULL && stack[1] == NULL){
			success = 1;
			
			pTree -> root = stack[0];
			stack[0] = NULL;
		}
		else{
			success = 0;
			
			while(stack[i] != NULL){
				_destroy(stack[i]);
				stack[i] = NULL;
				i++;
			}
			free(pTree);
		}
	}
	else{
		success = 0;
		
		while(stack[i] != NULL){
				_destroy(stack[i]);
				stack[i] = NULL;
				i++;
			}
		free(pTree);
	}
	
	return success;
}

/* internal traversal function
	an implementation of ALGORITHM 6-6
*/
static void _traverse( NODE *root){
	if(isdigit(root -> data))
		printf("%c", root -> data);
	else{
		printf("(");
		_traverse(root -> left);
		printf("%c", root -> data);
		_traverse(root -> right);
		printf(")");
	}
	
	return;
}

/* Print node in tree using inorder traversal
*/
void traverseTree( TREE *pTree);

/* internal traversal function
*/
static void _infix_print( NODE *root, int level){
	if(isdigit(root->data)){
		for(int i = 0; i < level; i++)
		{
			printf("\t");
		}
		printf("%c\n", root -> data);
	}
	else{
		_infix_print(root -> right, level + 1);
		for(int i = 0; i < level; i++)
		{
			printf("\t");
		}
		printf("%c\n", root -> data);
		_infix_print(root -> left, level + 1);
	}

	return;	
}

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree);

/* evaluate postfix expression
	return	value of expression
*/
float evalPostfix( char *expr){
	float val;
	
	float stack[1024];
	int idxE = 0;
	int idxS = 0;
	
	for(int i = 0; i < 1025; i++){
		stack[i] = 0;
	}
	
	while(expr[idxE] != '\0'){
		if(isdigit(expr[idxE])){
			if(isdigit(expr[idxE + 1]))
				stack[idxS] = atoi(&expr[idxE]) / 10;
			else
				stack[idxS] = atoi(&expr[idxE]);
			idxE++;
			idxS++;
		}
		else{
			switch(expr[idxE]){
				case '+':
				stack[idxS - 2] = stack[idxS - 2] + stack[idxS - 1];
				stack[idxS - 1] = 0;
				idxE++;
				idxS--;
				break;
				case '-':
				stack[idxS - 2] = stack[idxS - 2] - stack[idxS - 1];
				stack[idxS - 1] = 0;
				idxE++;
				idxS--;
				break;
				case '*':
				stack[idxS - 2] = stack[idxS - 2] * stack[idxS - 1];
				stack[idxS - 1] = 0;
				idxE++;
				idxS--;
				break;
				case '/':
				stack[idxS - 2] = stack[idxS - 2] / stack[idxS - 1];
				stack[idxS - 1] = 0;
				idxE++;
				idxS--;
				break;
			}
		}
	}
	
	val = stack[0];
	
	return val;
}

/* Deletes all data in tree and recycles memory
	return	NULL head pointer
*/
TREE *destroyTree( TREE *pTree);

////////////////////////////////////////////////////////////////////////////////
TREE *destroyTree( TREE *pTree)
{
	if (pTree)
	{
		_destroy( pTree->root);
	}
		
	free( pTree);
	
	return NULL;	
}

////////////////////////////////////////////////////////////////////////////////
void printTree( TREE *pTree)
{
	_infix_print(pTree->root, 0);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
void traverseTree( TREE *pTree)
{
	_traverse (pTree->root);
	
	return;
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	TREE *tree;
	char expr[1024];
	
	fprintf( stdout, "\nInput an expression (postfix): ");
	
	while (fscanf( stdin, "%s", expr) == 1)
	{
		// creates a null tree
		tree = createTree();
		
		if (!tree)
		{
			printf( "Cannot create tree\n");
			return 100;
		}
		
		// postfix expression -> expression tree
		int ret = postfix2tree( expr, tree);
		if (!ret)
		{
			fprintf( stdout, "invalid expression!\n");
			continue;
		}
		
		// expression tree -> infix expression
		fprintf( stdout, "\nInfix expression : ");
		traverseTree( tree);
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "\n\nTree representation:\n");
		printTree(tree);
		
		// evaluate postfix expression
		float val = evalPostfix( expr);
		fprintf( stdout, "\nValue = %f\n", val);
		
		// destroy tree
		destroyTree( tree);
		
		fprintf( stdout, "\nInput an expression (postfix): ");
	}
	return 0;
}