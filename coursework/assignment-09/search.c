#define DEBUG 0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

#include "trie.h"

// 역색인 헤더 정보에 대한 구조체
typedef struct {
	int		index;	// starting position in posting.idx
	int		df;		// document frequency
} tHEADER;

////////////////////////////////////////////////////////////////////////////////
// 헤더 정보가 저장된 파일(예) "header.idx")을 읽어 메모리에 저장한다.
// 헤더 구조체 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
tHEADER *load_header( char *filename){
	tHEADER *header = NULL;
	
	FILE *fp = NULL;
	size_t ret_fr;
	header = (tHEADER *)malloc(sizeof(tHEADER) * 26512);
	
	if(header){
		for(int i = 0; i < 26512; i++){
			header[i].index = 0;
			header[i].df = 0;
		}
		
		fp = fopen(filename, "rb");
		if(fp == NULL){
			free(header);
			header = NULL;
			printf("Error: cannot open file: header.idx\n");
		}
		else{
			ret_fr = fread(header, sizeof(tHEADER), 26512, fp);
			fclose(fp);
		}
	}
	
	return header;
}

// 포스팅 리스트가 저장된 파일(예) "posting.idx")를 읽어 메모리에 저장한다.
// 포스팅 리스트(int arrary) 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
int *load_posting( char *filename){
	int *posting = NULL;
	
	FILE *fp = NULL;
	size_t ret_fr;
	posting = (int *)malloc(sizeof(int) * 450000);
	for(int i = 0; i < 450000; i++){
		posting[i] = 0;
	}
	
	fp = fopen(filename, "rb");
	if(fp == NULL){
		free(posting);
		posting = NULL;
		printf("Error: cannot open file: posting.idx\n");
	}
	else{
		ret_fr = fread(posting, sizeof(int), 450000, fp);
		fclose(fp);
	}
	
	return posting;
};

// 단일 텀(single term)을 검색하여 문서를 찾는다.
// 문서 집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 검색된 문서 수는 newnumdocs에 저장한다.
int *getDocuments( tHEADER *header, int *posting, TRIE *trie, char *term, int *numdocs){
	int *docs = NULL;
	*numdocs = 0;
	
	int found;
	int trieidx;
	int postid;
	int df;
	
	found = trieSearch(trie, term, &trieidx);
	if(found){
		postid = header[trieidx].index;
		df = header[trieidx].df;

		docs = (int *)malloc(sizeof(int) * df);
		for(int i = 0; i < df; i++){
			docs[i] = posting[postid + i];
		}
		*numdocs = df;
	}
	
	return docs;
}

// 문서 집합을 화면에 출력한다.
void showDocuments( int *docs, int numdocs){
	for(int i = 0; i < numdocs; i++){
		printf(" %d", docs[i]);
	}
	
	return;
}

// 두 문서 집합의 교집합을 구한다.
// 교집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 교집합의 문서 수는 newnumdocs에 저장한다.
int *intersectDocuments( int *docs, int numdocs, int *docs2, int numdocs2, int *newnumdocs){
	int *newdocs = NULL;
	
	int *pdocs = docs;
	int *pdocs2 = docs2;
	*newnumdocs = 0;
	
	if(numdocs != 0 && numdocs2 != 0){
		while(1){
			if(pdocs - 1 == &docs[numdocs - 1] || pdocs2 - 1 == &docs2[numdocs2 - 1])
				break;
			
			if(*pdocs < *pdocs2)
				pdocs += 1;
			else if(*pdocs > *pdocs2)
				pdocs2 += 1;
			else{
				newdocs = realloc(newdocs, sizeof(int) * (*newnumdocs + 1));
				newdocs[*newnumdocs] = *pdocs;
				pdocs += 1;
				pdocs2 += 1;
				*newnumdocs += 1;
			}
		}
	}
	
	if(numdocs != 0)
		free(docs);
	if(numdocs2 != 0)
		free(docs2);
	
	return newdocs;
}

// 두 문서 집합의 합집합을 구한다.
// 합집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 합집합의 문서 수는 newnumdocs에 저장한다.
int *unionDocuments( int *docs, int numdocs, int *docs2, int numdocs2, int *newnumdocs){
	int *newdocs = NULL;
	*newnumdocs = 0;
	
	int *pdocs = docs;
	int *pdocs2 = docs2;
	
	if(numdocs == 0 || numdocs2 == 0){
		if(numdocs != 0){
			newdocs = docs;
			*newnumdocs = numdocs;
		}
		else if(numdocs2 != 0){
			newdocs = docs2;
			*newnumdocs = numdocs2;
		}
	}
	else{
		while(1){
			if(pdocs - 1 == &docs[numdocs - 1] && pdocs2 - 1 == &docs2[numdocs2 - 1]){
				free(docs);
				free(docs2);
				break;
			}
			else if(pdocs - 1 != &docs[numdocs - 1] && pdocs2 - 1 == &docs2[numdocs2 - 1]){
				while(1){
					if(pdocs - 1 == &docs[numdocs - 1])
						break;
					newdocs = realloc(newdocs, sizeof(int) * (*newnumdocs + 1));
					newdocs[*newnumdocs] = *pdocs;
					pdocs += 1;
					*newnumdocs += 1;
				}
				free(docs);
				free(docs2);
				break;
			}
			else if(pdocs - 1 == &docs[numdocs - 1] && pdocs2 - 1 != &docs2[numdocs2 - 1]){
				while(1){
					if(pdocs2 - 1 == &docs2[numdocs2 - 1])
						break;						
					newdocs = realloc(newdocs, sizeof(int) * (*newnumdocs + 1));
					newdocs[*newnumdocs] = *pdocs2;
					pdocs2 += 1;
					*newnumdocs += 1;
				}
				free(docs);
				free(docs2);
				break;
			}
			
			if(*pdocs < *pdocs2){
				newdocs = realloc(newdocs, sizeof(int) * (*newnumdocs + 1));
				newdocs[*newnumdocs] = *pdocs;
				pdocs += 1;
			}
			else if(*pdocs > *pdocs2){
				newdocs = realloc(newdocs, sizeof(int) * (*newnumdocs + 1));
				newdocs[*newnumdocs] = *pdocs2;
				pdocs2 += 1;
			}
			else{
				newdocs = realloc(newdocs, sizeof(int) * (*newnumdocs + 1));
				newdocs[*newnumdocs] = *pdocs;
				pdocs += 1;
				pdocs2 += 1;
			}
			*newnumdocs += 1;
		}
	}
	
	return newdocs;
}

static char *rtrim( char *str);
static char *ltrim( char *str);
static char *trim( char *str);

// 질의(query)를 검색하여 문서를 찾는다.
// 질의는 단일 텀 또는 하나 이상의 불린 연산자('&' 또는 '|')를 포함한 질의가 될 수 있다.
// 문서 집합을 위한 메모리를 할당하고 그 주소를 반환
// 실패시 NULL을 반환
// 검색된 문서 수는 newnumdocs에 저장한다.
int *searchDocuments( tHEADER *header, int *posting, TRIE *trie, char *query, int *numdocs){
	int *docs = NULL;
	*numdocs = 0;
	
	int qlength = strlen(query);
	char newquery[100] = {'\0', };
	char term[100] = {'\0', };
	char *parser = newquery;
	char *pterm = term;
	int operation = -1;
	int *tdocs = NULL;
	int numtdocs;
	int *nqdocs = NULL;
	int numnqdocs;
	
	if(qlength != 0){
		strcpy(newquery, query);
		for(int i = 0; i < qlength; i++){
			parser += 1;
		}
		if(*(parser - 1) == '\n')
			*(parser - 1) = '\0';
		while(1){
			if(*parser == '&'){
				operation = 1;
				break;
			}
			else if(*parser == '|'){
				operation = 2;
				break;
			}
			else if(parser == &newquery[0]){
				operation = 0;
				break;
			}
			
			parser -= 1;
		}
		switch(operation){
			case 1:
			strcpy(term, parser + 1);
			while(*parser != '\0'){
				*parser = '\0';
				parser += 1;
			}
			tdocs = getDocuments(header, posting, trie, trim(term), &numtdocs);
			nqdocs = searchDocuments(header, posting, trie, trim(newquery), &numnqdocs);
			docs = intersectDocuments(tdocs, numtdocs, nqdocs, numnqdocs, numdocs);
			break;
			case 2:
			strcpy(term, parser + 1);
			while(*parser != '\0'){
				*parser = '\0';
				parser += 1;
			}
			tdocs = getDocuments(header, posting, trie, trim(term), &numtdocs);
			nqdocs = searchDocuments(header, posting, trie, trim(newquery), &numnqdocs);
			docs = unionDocuments(tdocs, numtdocs, nqdocs, numnqdocs, numdocs);
			break;
			case 0:
			strcpy(term, parser);
			while(*parser != '\0'){
				*parser = '\0';
				parser += 1;
			}
			docs = getDocuments(header, posting, trie, trim(term), numdocs);
			break;
			default:
			break;
		}
	}
	
	return docs;
}

////////////////////////////////////////////////////////////////////////////////
static char *rtrim( char *str)
{
	char *p = str + strlen( str) - 1;
	
	while (p >= str)
	{
		if (*p == '\n' || *p == ' ' || *p == '\t') *p = 0;
		else return str;
		p--;
	}
}

static char *ltrim( char *str)
{
	char *p = str;
	
	while (*p)
	{
		if (*p == '\n' || *p == ' ' || *p == '\t') p++;
		else 
		{
			return p;
		}
	}
	return NULL;
}

static char *trim( char *str)
{
	if( str == NULL || *str == 0) return str;
	
	return rtrim(ltrim(str));
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	tHEADER *header;
	int *posting;
	TRIE *trie;
	char query[100];
	int index;
	
	header = load_header( "header.idx");
	if (header == NULL) return 1;
	
	posting = load_posting( "posting.idx");
	if (posting == NULL) return 1;
		
	trie = dic2trie( "dic.txt");
	
	printf( "\nQuery: ");
	while (fgets( query, 100, stdin) != NULL)
	{
		int numdocs;
		int *docs = searchDocuments( header, posting, trie, query, &numdocs);
		
		if (docs == NULL) printf( "not found!\n");
		else 
		{
			showDocuments( docs, numdocs);
			free( docs);
		}
		printf( "\nQuery: ");
	}
	
	free( header);
	free( posting);
	trieDestroy( trie);
	
	return 0;
}