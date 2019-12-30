#include <stdlib.h> // malloc, rand
#include <stdio.h>
#include <time.h> // time

#define MAX_ELEM 20
#define max(x, y)	(((x) > (y)) ? (x) : (y))

////////////////////////////////////////////////////////////////////////////////
// AVL_TREE type definition
typedef struct node
{
	int			data;
	struct node	*left;
	struct node	*right;
	int			height;
} NODE;

typedef struct
{
	NODE	*root;
	int		count;  // number of nodes
} AVL_TREE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a AVL_TREE head node and returns its address to caller
	return	head node pointer
			NULL if overflow
*/
AVL_TREE *AVL_Create( void){
	AVL_TREE *tree = (AVL_TREE *)malloc(sizeof(AVL_TREE));
	
	if(tree){
		tree -> root = NULL;
		tree -> count = 0;
	}
	
	return tree;
}

static NODE *_makeNode( int data){
	NODE *pNew = (NODE *)malloc(sizeof(NODE));
	
	if(pNew){
		pNew -> data = data;
		pNew -> left = NULL;
		pNew -> right = NULL;
		pNew -> height = 1;
	}
	
	return pNew;
}

/* internal function
	return	height of the (sub)tree from the node (root)
*/
static int getHeight( NODE *root){
	int treeHeight;
	
	if(root == NULL)
		treeHeight = 0;
	else
		treeHeight = root -> height;
	
	return treeHeight;
}

/* internal function
	Exchanges pointers to rotate the tree to the left
	updates heights of the nodes
	return	new root
*/
static NODE *rotateLeft( NODE *root){
	NODE *rootNew = NULL;
	
	NODE *parent = root;
	NODE *child = root -> right;
	
	parent -> right = child -> left;
	child -> left = parent;
	
	if(parent -> left)
		parent -> left -> height = max(getHeight(parent -> left -> left), getHeight(parent -> left -> right)) + 1;
	if(parent -> right)
		parent -> right -> height = max(getHeight(parent -> right -> left), getHeight(parent -> right -> right)) + 1;
	parent -> height = max(getHeight(parent -> left), getHeight(parent -> right)) + 1;
	child -> height = max(getHeight(child -> left), getHeight(child -> right)) + 1;
	
	rootNew = child;
	
	return rootNew;
}

/* internal function
	Exchanges pointers to rotate the tree to the right
	updates heights of the nodes
	return	new root
*/
static NODE *rotateRight( NODE *root){
	NODE *rootNew = NULL;
	
	NODE *parent = root;
	NODE *child = root -> left;
	
	parent -> left = child -> right;
	child -> right = parent;
	
	if(parent -> left)
		parent -> left -> height = max(getHeight(parent -> left -> left), getHeight(parent -> left -> right)) + 1;
	if(parent -> right)
		parent -> right -> height = max(getHeight(parent -> right -> left), getHeight(parent -> right -> right)) + 1;
	parent -> height = max(getHeight(parent -> left), getHeight(parent -> right)) + 1;
	child -> height = max(getHeight(child -> left), getHeight(child -> right)) + 1;
	
	rootNew = child;
	
	return rootNew;
}

/* internal function
	This function uses recursion to insert the new data into a leaf node
	return	pointer to new root
*/
static NODE *_insert( NODE *root, NODE *newPtr){
	NODE *rootNew = NULL;
	
	NODE *rootSub = NULL;
	
	if(root == NULL)
		rootNew = newPtr;
	else{
		if(newPtr -> data < root -> data && root -> left == NULL){
			root -> left = newPtr;
			root -> height = max(getHeight(root -> left), getHeight(root -> right)) + 1;
			rootNew = root;
		}
		else if(newPtr -> data >= root -> data && root -> right == NULL){
			root -> right = newPtr;
			root -> height = max(getHeight(root -> left), getHeight(root -> right)) + 1;
			rootNew = root;
		}
		else{
			if(newPtr -> data < root -> data){
				rootNew = _insert(root -> left, newPtr);
				
				if((root -> left) != rootNew)
					root -> left = rootNew;
				root -> height = max(getHeight(root -> left), getHeight(root -> right)) + 1;
				
				if((getHeight(root -> left) - getHeight(root -> right)) == 2){
					if((getHeight(root -> left -> left) - getHeight(root -> left -> right)) == 1)
						rootNew = rotateRight(root);
					else if((getHeight(root -> left -> left) - getHeight(root -> left -> right)) == -1){
						rootSub = rotateLeft(rootNew);
						root -> left = rootSub;
						root -> height = max(getHeight(root -> left), getHeight(root -> right)) + 1;
						rootNew = rotateRight(root);
					}
				}
				else
					rootNew = root;
			}
			else{
				rootNew = _insert(root -> right, newPtr);
				
				if((root -> right) != rootNew)
					root -> right = rootNew;
				root -> height = max(getHeight(root -> left), getHeight(root -> right)) + 1;
			
				if((getHeight(root -> left) - getHeight(root -> right)) == -2){
					if((getHeight(root -> right -> left) - getHeight(root -> right -> right)) == -1)
						rootNew = rotateLeft(root);
					else if((getHeight(root -> right -> left) - getHeight(root -> right -> right)) == 1){
						rootSub = rotateRight(rootNew);
						root -> right = rootSub;
						root -> height = max(getHeight(root -> left), getHeight(root -> right)) + 1;
						rootNew = rotateLeft(root);
					}
				}
				else
					rootNew = root;
			}
		}
	}

	return rootNew;
}

/* Inserts new data into the tree
	return	1 success
			0 overflow
*/
int AVL_Insert( AVL_TREE *pTree, int data){
	int success;
	
	NODE *newPtr = _makeNode(data);
	
	if(!newPtr)
		success = 0;
	else{
		NODE *rootNew = _insert(pTree -> root, newPtr);
	
		if((pTree -> root) != rootNew){
			pTree -> root = rootNew;
			pTree -> root -> height = max(getHeight(pTree -> root -> left), getHeight(pTree -> root -> right)) + 1;
		}
		
		pTree -> count += 1;
		
		success = 1;
	}
	
	return success;
}

static void _traverse( NODE *root){
	if(root != NULL){
		_traverse(root -> left);
		printf("%d ", root -> data);
		_traverse(root -> right);
	}
	
	return;
}

/* Prints tree using inorder traversal
*/
void AVL_Traverse( AVL_TREE *pTree){
	_traverse(pTree -> root);
	
	return;
}

/* internal traversal function
*/
static void _infix_print( NODE *root, int level){
	if(root != NULL){
		_infix_print(root -> right, level + 1);
		for(int i = 0; i < level; i++)
			printf("\t");
		printf("%d\n", root -> data);
		_infix_print(root -> left, level + 1);
	}
	
	return;
}

/* Prints tree using inorder right-to-left traversal
*/
void printTree( AVL_TREE *pTree){
	_infix_print(pTree -> root, 0);
	
	return;
}

static void _destroy( NODE *root){
	if(root != NULL){
		_destroy(root -> left);
		_destroy(root -> right);
		free(root);
	}
	
	return;
}

/* Deletes all data in tree and recycles memory
	return	NULL head pointer
*/
AVL_TREE *AVL_Destroy( AVL_TREE *pTree){
	_destroy(pTree -> root);
	free(pTree);
	pTree = NULL;
	
	return pTree;
}

/* internal function
	Retrieve node containing the requested key
	return	address of the node containing the key
			NULL not found
*/
//static NODE *_retrieve( NODE *root, int key);

/* Retrieve tree for the node containing the requested key
	return	address of data of the node containing the key
			NULL not found
*/
//int *AVL_Retrieve( AVL_TREE *pTree, int key);

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	AVL_TREE *tree;
	int data;
	
	// creates a null tree
	tree = AVL_Create();
	
	if (!tree)
	{
		printf( "Cannot create tree\n");
		return 100;
	}

	fprintf( stdout, "Inserting: ");
	
	srand( time(NULL));
	for (int i = 0; i < MAX_ELEM; i++)
	{
		data = rand() % (MAX_ELEM * 3) + 1; // random number
//		data = i+1; // sequential number
		
		fprintf( stdout, "%d ", data);

		// insert function call
		AVL_Insert( tree, data);
	}
	fprintf( stdout, "\n");

	// inorder traversal
	fprintf( stdout, "Inorder traversal: ");
	AVL_Traverse( tree);
	fprintf( stdout, "\n");
	
	// print tree with right-to-left infix traversal
	fprintf( stdout, "Tree representation:\n");
	printTree(tree);
	
	fprintf( stdout, "Height of tree: %d\n", tree->root->height);
	fprintf( stdout, "# of nodes: %d\n", tree->count);
	
	AVL_Destroy( tree);

	return 0;
}
