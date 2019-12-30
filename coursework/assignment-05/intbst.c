#include <stdlib.h> // malloc, atoi, rand
#include <stdio.h>
#include <ctype.h> // isdigit
#include <assert.h> // assert
#include <time.h> // time

////////////////////////////////////////////////////////////////////////////////
// TREE type definition
typedef struct node
{
	int			data;
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
TREE *BST_Create( void)
{
	TREE *pTree = (TREE *)malloc(sizeof(TREE));
	
	if(pTree)
		pTree -> root = NULL;
	
	return pTree;
};

static NODE *_makeNode( int data)
{
	NODE *pNew = (NODE *)malloc(sizeof(NODE));
	
	if(pNew)
	{
		pNew -> data = data;
		pNew -> left = NULL;
		pNew -> right = NULL;
	}
	
	return pNew;
};

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int BST_Insert( TREE *pTree, int data)
{
	int successful;

	NODE *root = pTree -> root;
	NODE *pNew = _makeNode(data);
	
	while(1)
	{
		if(!pNew)
		{
			successful = 0;
			break;
		}
		else
		{
			if(pTree -> root == NULL)
			{
				pTree -> root = pNew;
				successful = 1;
				break;
			}
			else
			{
				if(data >= root -> data)
				{
					if(root -> right == NULL)
					{
						root -> right = pNew;
						successful = 1;
						break;
					}
					else
						root = root -> right;
				}
				else
				{
					if(root -> left == NULL)
					{
						root -> left = pNew;
						successful = 1;
						break;
					}
					else
						root = root -> left;
				}
			}
		}
	}
	
	return successful;
};

/* internal traversal function
*/
static void _traverse( NODE *root)
{
	if(root == NULL)
		return;
	else
	{
		_traverse(root -> left);
		printf("%d ", root -> data);
		_traverse(root -> right);
	}
};

/* prints tree using inorder traversal
*/
void BST_Traverse( TREE *pTree)
{
	_traverse(pTree -> root);
	
	return;
};

static void _infix_print( NODE *root, int level)
{
	if(root == NULL)
		return;
	else
	{
		_infix_print(root -> right, level + 1);
		for(int i = 0; i < level; i++)
		{
			printf("\t");
		}
		printf("%d\n", root -> data);
		_infix_print(root -> left, level + 1);
	}
};

/* Print tree using inorder right-to-left traversal
*/
void printTree( TREE *pTree)
{
	_infix_print(pTree -> root, 0);
};

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
NODE *_retrieve( NODE *root, int key)
{
	NODE *pLoc = NULL;
	
	while(1)
	{
		if(root == NULL)
			break;
		else
		{
			if(key == root -> data)
			{
				pLoc = root;
				break;
			}
			else
			{
				if(key < root -> data)
					root = root -> left;
				else
					root = root -> right;
			}
		}
		
	}
	
	return pLoc;
};

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
int *BST_Retrieve( TREE *pTree, int key)
{
	int *data = NULL;
	
	NODE *pLoc = _retrieve(pTree -> root, key);
	
	if(pLoc)
		data = &(pLoc -> data);
	
	return data;
};

/* internal function
	success is 1 if deleted; 0 if not
	return	pointer to root
*/
NODE *_delete( NODE *root, int dltKey, int *success)
{
	NODE *pLoc = NULL;
	NODE *pDel = NULL;
	
	pLoc = root;
	
	while(1){
		if(!pLoc) {
			*success = 0;
			break;
		}
		else if(dltKey == pLoc -> data){
			if(pLoc -> left == NULL && pLoc -> right == NULL){
				pDel = pLoc;
				pLoc = root;
				while(1){
					if(pLoc -> left){
						if(pLoc -> left -> data == dltKey){
							pLoc -> left = NULL;
							break;
						}
					}
					
					if(pLoc -> right){
						if(pLoc -> right -> data == dltKey){
							pLoc -> right = NULL;
							break;
						}
					}
					
					if(dltKey < pLoc -> data)
						pLoc = pLoc -> left;
					else
						pLoc = pLoc -> right;
				}
				free(pDel);
				*success = 1;
				break;
			}
			else{
				*success = 0;
				break;
			}
		}
		else {
			if(dltKey < pLoc -> data)
				pLoc = pLoc -> left;
			else if (dltKey >= pLoc -> data)
				pLoc = pLoc -> right;
		}
	}
	
	pDel == NULL;
	
	return pLoc;
};

/* Deletes a node with dltKey from the tree
	return	1 success
			0 not found
*/
int BST_Delete( TREE *pTree, int dltKey)
{
	int successful;
	
	NODE *pLoc = NULL;
	NODE *pDel = NULL;
	int *data;
	int dataNew;
	int success;
	
	pLoc = _delete(pTree -> root, dltKey, &success);
	
	if(success == 1)
		successful = 1;
	else{
		if(pLoc == NULL){
			successful = 0;
		}
		else if(pLoc -> right == NULL){
			pDel = pLoc -> left;
			
			data = BST_Retrieve(pTree, pLoc -> data);
			*data = pDel -> data;
			
			pLoc -> left = pDel -> left;
			pLoc -> right = pDel -> right;
			
			pDel -> left = NULL;
			pDel -> right = NULL;
			free(pDel);
			
			successful = 1;
		}
		else if(pLoc -> left == NULL){
			pDel = pLoc -> right;
			
			data = BST_Retrieve(pTree, pLoc -> data);
			*data = pDel -> data;
			
			pLoc -> left = pDel -> left;
			pLoc -> right = pDel -> right;
			
			pDel -> left = NULL;
			pDel -> right = NULL;
			free(pDel);
			
			successful = 1;
		}
		else{
			pDel = pLoc -> right;
			
			while(pDel -> left != NULL){
				pDel = pDel -> left;
			}
			dataNew = pDel -> data;
			
			if(pLoc -> data = pDel -> data){
				(pLoc -> data) -= 1;
				successful = BST_Delete(pTree, pDel -> data);
				(pLoc -> data) += 1;
			}
			else
				successful = BST_Delete(pTree, pDel -> data);
			
			data = BST_Retrieve(pTree, pLoc -> data);
			*data = dataNew;
		}
	}
	
	pLoc == NULL;
	pDel == NULL;
	
	return successful;
};

static void _destroy( NODE *root)
{
	if(root == NULL)
		return;
	else{
		_destroy(root -> left);
		_destroy(root -> right);
		free(root);
	}
};

/* Deletes all data in tree and recycles memory
	return	NULL head pointer
*/
TREE *BST_Destroy( TREE *pTree)
{
	fprintf(stdout, "begin destroy");
	_destroy(pTree -> root);
	free(pTree);
	
	pTree == NULL;
	
	return pTree;
};

////////////////////////////////////////////////////////////////////////////////

int main( int argc, char **argv)
{
	TREE *tree;
	int data;
	
	// creates a null tree
	tree = BST_Create();
	
	if (!tree)
	{
		printf( "Cannot create tree\n");
		return 100;
	}

	fprintf( stdout, "Inserting: ");
	
	srand( time(NULL));
	for (int i = 1; i < 20; i++)
	{
		data = rand() % 100 + 1; // 1 ~ 100 random number
		
		fprintf( stdout, "%d ", data);
		
		// insert funtion call
		BST_Insert( tree, data);
 	}
	fprintf( stdout, "\n");
			
	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	BST_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	int ret;
	do
	{
		fprintf( stdout, "Input a number to delete: "); 
		int num;
		ret = scanf( "%d", &num);
		if (ret != 1) break;
		
		ret = BST_Delete( tree, num);
		if (!ret) fprintf( stdout, "%d not found\n", num);
		
		// print tree with right-to-left infix traversal
		fprintf( stdout, "Tree representation:\n");
		printTree(tree);
		
	} while(1);
	
	BST_Destroy( tree);

	return 0;
}
