
#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW

// TRIE type definition
typedef struct trieNode {
	int 			index; // 0, 1, 2, ...
	struct trieNode	*subtrees[MAX_DEGREE];
} TRIE;

////////////////////////////////////////////////////////////////////////////////
// Prototype declarations

/* Allocates dynamic memory for a trie node and returns its address to caller
	return	node pointer
			NULL if overflow
*/
TRIE *trieCreateNode(void){
	TRIE *trie = (TRIE *)malloc(sizeof(TRIE));
	
	if(trie){
		trie -> index = -1;
		for(int i = 0; i < MAX_DEGREE; i++){
			trie -> subtrees[i] = NULL;
		}
	}
	
	return trie;
}

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root){
	if(root == NULL)
		return;
	else{
		for(int i = 0; i < MAX_DEGREE; i++){
			trieDestroy(root -> subtrees[i]);
		}
		free(root);
	}
}

/* Inserts new entry into the trie
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str, int dic_index){
	int success = -1;
	
	int wordLength = strlen(str);
	char *word = NULL;
	int wordIdx;
	TRIE *pLoc = NULL;
	TRIE *pNew = NULL;
	
	pLoc = root;
	for(int i = 0; i < wordLength; i++){
		wordIdx = str[i] - 'a';
		if(pLoc -> subtrees[wordIdx] == NULL){
			pNew = trieCreateNode();
			pLoc -> subtrees[wordIdx] = pNew;
			pNew = NULL;
		}
		pLoc = pLoc -> subtrees[wordIdx];
	}
	
	if(pLoc -> index != -1){
		pLoc = NULL;
		success = 0;
	}
	else{
		pLoc -> index = dic_index;
		pLoc = NULL;
		success = 1;
	}
	
	return success;
};

/* makes a trie for given dictionary file
	return	trie head node pointer
			NULL failure
*/ 
TRIE *dic2trie( char *dicfile){
	TRIE *trie = NULL;;
	
	FILE *fp = NULL;
	char str[100];
	int index = 0;
	int success;
	
	fp = fopen(dicfile, "rt");
	if(fp == NULL)
		printf("Error: cannot open file: dic.txt\n");
	else{
		printf("Inserting to trie...");
		trie = trieCreateNode();
		
		if(trie){
			while(fscanf( fp, "%s", str) == 1){
				success = trieInsert(trie, str, index);
				if(!success){
					trieDestroy(trie);
					trie = NULL;
					break;
				}
				index++;
			}
			fclose(fp);
		}
		printf("\t[done]\n");
	}
	
	return trie;
}

/* Retrieve trie for the requested key
	return	index in dictionary (trie) if key found
			-1 key not found
*/
int trieSearch( TRIE *root, char *str, int *trieidx){
	int found = -1;
	
	int invalid = 0;
	int wordLength = strlen(str);
	int wordIdx;
	TRIE *pLoc = root;

	for(int i = 0; i < wordLength; i++){
			wordIdx = str[i] - 'a';
		if(wordIdx < 0 || wordIdx > 26){
			invalid = 1;
			break;
		}
	}
	
	if(invalid){
		found = 0;
	}
	else{
		for(int i = 0; i < wordLength; i++){
			wordIdx = str[i] - 'a';
			if(pLoc -> subtrees[wordIdx] == NULL){
				found = 0;
				break;
			}
			else
				pLoc = pLoc -> subtrees[wordIdx];
			}
			
		if(found != 0){
			if(pLoc -> index == -1)
				found = 0;
			else{
				*trieidx = pLoc -> index;
				found = 1;
			}
		}
	}

	return found;
}

/* prints all entries in trie using preorder traversal
*/
//void trieList( TRIE *root);

/* makes permuterms for given str
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
//int make_permuterms( char *str, char *permuterms[]);

/* makes a permuterm trie for given dictionary file
	return	trie head node pointer
			NULL failure
*/ 
//TRIE *dic2permute_trie( char *dicfile);

/* recycles memory for permuterms
*/
//void clear_permuterms( char *permuterms[], int size);

/* prints all entries starting with str (as prefix) in trie
   ex) "abb" -> "abbas", "abbasid", "abbess", ...
	using trieList function
*/
//void triePrefixList( TRIE *root, char *str);

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	using triePrefixList function
*/
//void trieSearchWildcard( TRIE *root, char *str);
