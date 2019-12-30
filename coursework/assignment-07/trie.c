#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h> // isupper, tolower

#define MAX_DEGREE	27 // 'a' ~ 'z' and EOW
#define EOW			'$' // end of word

// used in the following functions: trieInsert, trieSearch, triePrefixList
#define getIndex(x)		(((x) == EOW) ? MAX_DEGREE-1 : ((x) - 'a'))

// TRIE type definition
typedef struct trieNode {
	char 			*entry;
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
		trie -> entry = NULL;
		for(int i = 0; i < MAX_DEGREE; i++){
			trie -> subtrees[i] = NULL;
		}
	}
	
	return trie;
}

/* Inserts new entry into the trie // malloc control missing
	return	1 success
			0 failure
*/
// 주의! 엔트리를 중복 삽입하지 않도록 체크해야 함
// 대소문자를 소문자로 통일하여 삽입
// 영문자 외 문자를 포함하는 문자열은 삽입하지 않음
int trieInsert( TRIE *root, char *str){
	int success = -1;
	
	int invalid = 0;
	int wordLength = strlen(str);
	char *word = NULL;
	int wordIdx;
	TRIE *pLoc = NULL;
	TRIE *pNew = NULL;
	
	for(int i = 0; i < wordLength; i++){
		if(isupper(str[i]))
			wordIdx = getIndex(tolower(str[i]));
		else
			wordIdx = getIndex(str[i]);
		
		if(wordIdx < 0 || wordIdx > 27){
			invalid = 1;
			break;
		}
	}
	
	if(!invalid){
		word = (char *)malloc(sizeof(char) * wordLength + 1);
		
		if(!word){
			success = 0;
		}
		else{
			for(int i = 0; i < wordLength; i++){
				if(isupper(str[i]))
					word[i] = tolower(str[i]);
				else
					word[i] = str[i];
			}
			word[wordLength] = '\0';
			
			pLoc = root;
			for(int i = 0; i < wordLength; i++){
				wordIdx = getIndex(word[i]);
				if(pLoc -> subtrees[wordIdx] == NULL){
					pNew = trieCreateNode();
					pLoc -> subtrees[wordIdx] = pNew;
					pNew = NULL;
				}
				pLoc = pLoc -> subtrees[wordIdx];
			}
		
			if(pLoc -> entry != NULL){
				pLoc = NULL;
				free(word);
				success = 0;
			}
			else{
				pLoc -> entry = word;
				pLoc = NULL;
				success = 1;
			}
		}
	}
	
	return success;
}

/* makes permuterms for given str // malloc control missing
	ex) "abc" -> "abc$", "bc$a", "c$ab", "$abc"
	return	number of permuterms
*/
int make_permuterms( char *str, char *permuterms[]){
	int num_p;
	
	int wordLength = strlen(str);
	char *base_p = NULL;
	char *word_p = NULL;
	
	base_p = (char *)malloc(sizeof(char) * (wordLength + 2));
	for(int i = 0; i < wordLength; i++){
		if(isupper(str[i]))
			base_p[i] = tolower(str[i]);
		else
			base_p[i] = str[i];
	}
	base_p[wordLength] = '$';
	base_p[wordLength + 1] = '\0';
	permuterms[0] = base_p;
	base_p = NULL;
	
	for(int i = 0; i < wordLength; i++){
		word_p = (char *)malloc(sizeof(char) * (wordLength + 2));
		for(int k = 0; k < wordLength + 1; k++){
			word_p[k] = permuterms[i][k + 1];
		}
		word_p[wordLength] = permuterms[i][0];
		word_p[wordLength + 1] = '\0';
		
		permuterms[i + 1] = word_p;
		word_p = NULL;
		
		num_p = i + 2;
	}
	
	return num_p;
};

/* recycles memory for permuterms
*/
void clear_permuterms( char *permuterms[], int size){
	for(int i = 0; i < size; i++){
		free(permuterms[i]);
	}
}

/* Retrieve trie for the requested key
	return	1 key found
			0 key not found
*/
int trieSearch( TRIE *root, char *str){
	int found = -1;
	
	int invalid = 0;
	int wordLength = strlen(str);
	int wordIdx;
	TRIE *pLoc = root;
	
	for(int i = 0; i < wordLength; i++){
		if(isupper(str[i])){
			str[i] = tolower(str[i]);
			wordIdx = getIndex(tolower(str[i]));
		}
		else
			wordIdx = getIndex(str[i]);
		
		if(wordIdx < 0 || wordIdx > 27){
			invalid = 1;
			break;
		}
	}
	
	if(invalid)
		found = 0;
	else{
		for(int i = 0; i < wordLength; i++){
			if(isupper(str[i]))
				wordIdx = getIndex(tolower(str[i]));
			else
				wordIdx = getIndex(str[i]);
		
			if(pLoc -> subtrees[wordIdx] == NULL){
				found = 0;
				break;
			}
			else
				pLoc = pLoc -> subtrees[wordIdx];
		}
		
		if(found != 0){
			if(pLoc -> entry == NULL)
				found = 0;
			else
				found = 1;
		}
	}
	
	return found;
}

/* prints all entries in trie using preorder traversal
*/
void trieList( TRIE *root){
	int subtrees;
	
	if(root -> entry != NULL)
		printf("%s\n", root -> entry);
	
	for(int i = 0; i < MAX_DEGREE; i++){
		if(root -> subtrees[i] != NULL)
			subtrees = 1;
		else
			subtrees = 0;

		if(subtrees == 1)
			trieList(root -> subtrees[i]);
	}
	
	return;
}

/* prints all entries starting with str (as prefix) in trie
   ex) "abb" -> "abbas", "abbasid", "abbess", ...
	using trieList function
*/
void triePrefixList( TRIE *root, char *str){
	int wordIdx;
	int wordLength = strlen(str);
	TRIE *pLoc = root;
	int found;
	
	for(int i = 0; i < wordLength; i++){
		if(isupper(str[i]))
			wordIdx = getIndex(tolower(str[i]));
		else
			wordIdx = getIndex(str[i]);
		
		if(pLoc -> subtrees[wordIdx] != NULL){
			found = 1;
			pLoc = pLoc -> subtrees[wordIdx];
		}
		else{
			found = 0;
			break;
		}
	}
	
	if(found)
		trieList(pLoc);
	else
		printf("valid query format, but query was unable to be found!\n");
	
	return;
}

/* wildcard search
	ex) "ab*", "*ab", "a*b", "*ab*"
	using triePrefixList function
*/
void trieSearchWildcard( TRIE *root, char *str){
	int wordIdx;
	int starCount = 0;
	int starFront;
	int starRear;
	int wordLength = strlen(str);
	int rearIdx = strlen(str) - 1;
	char *strY = strchr(str, '*') + 1;
	int invalid = 0;
	
	char word[100];
	int strLength = strlen(str);
	for(int i = 0; i < strLength; i++){
		word[i] = str[i];
	}
	word[strLength] = '\0';
	char *strX = strtok(word, "*");
	
	char permuPrefix[100];
	for(int i = 0; i < 100; i++){
		permuPrefix[i] = '\0';
	}
	
	for(int i = 0; i < wordLength; i++){
		if(isupper(str[i]))
			wordIdx = getIndex(tolower(str[i]));
		else
			wordIdx = getIndex(str[i]);
		
		if(wordIdx < 0 || wordIdx > 27){
			if(wordIdx != getIndex('*')){
				invalid = 1;
				break;
			}
		}
	}
	
	if(!invalid){
		for(int i = 0; i < wordLength; i++){
			if(str[i] == '*')
				starCount++;
		}
		
		if(starCount != 1 && starCount != 2)
				invalid = 1;
		
		if(!invalid){
			if(str[0] == '*')
				starFront = 1;
			else
				starFront = 0;
			
			if(str[rearIdx] == '*')
				starRear = 1;
			else
				starRear = 0;
			
			if(starCount == 1){
				if(strLength == 1)
					strcat(permuPrefix, "$");
				else if(starFront == 1){
					strcat(permuPrefix, strX);
					strcat(permuPrefix, "$");
				}
				else if(starRear == 1){
					strcat(permuPrefix, "$");
					strcat(permuPrefix, strX);
				}
				else{
					strcat(permuPrefix, strY);
					strcat(permuPrefix, "$");
					strcat(permuPrefix, strX);
				}
			}
			else if(starCount == 2){
				if (starFront == 1 && starRear == 1){
					if(strLength == 2)
						invalid = 1;
					else
						strcat(permuPrefix, strX);
				}
				else
					invalid = 1;
			}
		}
	}
	
	if(!invalid)
		triePrefixList(root, permuPrefix);
	else
		printf("invalid query format!\n");
	
	return;
}

/* Deletes all data in trie and recycles memory
*/
void trieDestroy( TRIE *root){
	if(root == NULL)
		return;
	else{
		if(root -> entry != NULL)
			free(root -> entry);
		for(int i = 0; i < MAX_DEGREE; i++){
			trieDestroy(root -> subtrees[i]);
		}
		free(root);
	}
}

int main(int argc, char **argv)
{
	TRIE *trie;
	TRIE *permute_trie;
	int ret;
	char str[100];
	FILE *fp;
	char *permuterms[100];
	int num_p;
	
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
	
	trie = trieCreateNode(); // original trie
	permute_trie = trieCreateNode(); // trie for permuterm index
	
	printf( "Inserting to trie...\t");
	while (fscanf( fp, "%s", str) == 1) // words file
	{	
		ret = trieInsert( trie, str);
		
		if (ret)
		{
			num_p = make_permuterms( str, permuterms);
			
			for (int i = 0; i < num_p; i++)
				trieInsert( permute_trie, permuterms[i]);
			
			clear_permuterms( permuterms, num_p);
		}
	}
	
	printf( "[done]\n"); // Inserting to trie

	fclose( fp);
	
	printf( "\nQuery: ");
	while (fscanf( stdin, "%s", str) == 1)
	{
		if (strchr( str, '*')) // wildcard search term
		{
			trieSearchWildcard( permute_trie, str);
		}
		else // search term
		{
			ret = trieSearch( trie, str);
			printf( "[%s]%s found!\n", str, ret ? "": " not");
		}
		printf( "\nQuery: ");
	}

	trieDestroy( trie);
	trieDestroy( permute_trie);
	
	return 0;
}
