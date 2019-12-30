#define DEBUG 0

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>

// 토큰-문서 구조체
typedef struct {
	char	*token;	// 토큰
	int		docid;	// 문서번호(document ID)
} tTokenDoc;

typedef struct {
	int		index;	// starting position in posting.idx
	int		df;		// 문서 빈도(document frequency)
} tHEADER;

////////////////////////////////////////////////////////////////////////////////
// 입력 파일로부터 토큰-문서 구조체를 생성한다.
tTokenDoc *get_tokens(char *filename, int *num_tokens){
	tTokenDoc *tokens = NULL;
	
	FILE *fp = NULL;
	char str[5000];
	char *word = NULL;
	int alloccount = 1;
	int docid = 0;
	
	tokens = (tTokenDoc *)malloc(sizeof(tTokenDoc) * 10000);
	*num_tokens = 0;
	
	fp = fopen(filename, "rt");
	if(fp == NULL)
		printf("Usage: ./index FILE");
	
	while(fgets(str, 5000, fp) != NULL){
		docid++;
		
		word = strtok(str, " \n");
		while(word != NULL){
			if((*num_tokens) >= 10000 * alloccount){
				alloccount++;
				tokens = realloc(tokens, sizeof(tTokenDoc) * 10000 * alloccount);
			}
			
			tokens[*num_tokens].token = strdup(word);
			tokens[*num_tokens].docid = docid;
			
			(*num_tokens)++;
			
			word = strtok(NULL, " \n");
			
		}
	}
	
	fclose(fp);
	
	return tokens;
}

// qsort를 위한 비교함수 (첫번째 정렬 기준: 토큰 문자열, 두번째 정렬 기준: 문서 번호)
static int _compare(const void *n1, const void *n2){
	int result;
	
	tTokenDoc *t1 = (tTokenDoc *)n1;
	tTokenDoc *t2 = (tTokenDoc *)n2;
	int strcmpval;
	
	strcmpval = strcmp(t1 -> token, t2 -> token);
	
	if(strcmpval < 0)
		result = -1;
	else if(strcmpval > 0)
		result = 1;
	else{
		if((t1 -> docid) - (t2 -> docid) < 0)
			result = -1;
		else if((t1 -> docid) - (t2 -> docid) > 0)
			result = 1;
		else
			result = 0;
	}
	
	return result;
	
}

// 토큰 구조체로부터 역색인 파일을 생성한다.
void invertedIndex( tTokenDoc *tokens, int num_tokens, 
					char *dicfilename, char *headerfilename, char *postingfilename){
						FILE *fp = NULL;
						tHEADER *header = malloc(sizeof(tHEADER) * 30000);
						int *posting = malloc(sizeof(int) * 450000);
						char *token;
						tTokenDoc *t1 = NULL;
						tTokenDoc *t2 = NULL;
						int hidx = 0;
						int pidx = 0;
						int hnum;
						int pnum;
						
						fp = fopen(dicfilename, "wt");
						token = tokens[0].token;
						fprintf(fp, "%s\n", token);
						header[0].index = 0;
						header[0].df = 1;
						posting[0] = tokens[0].docid;
						
						for(int i = 1; i < num_tokens; i++){
							t1 = &tokens[i -1];
							t2 = &tokens[i];
							
							if(strcmp(t1 -> token, t2 -> token) == 0){
								if(((t1 -> docid) - (t2 -> docid)) != 0){
									pidx++;
									posting[pidx] = t2 -> docid;
									(header[hidx].df)++;
								}
							}
							else{
								hidx++;
								pidx++;
								token = t2 -> token;
								fprintf(fp, "%s\n", token);
								header[hidx].index = pidx;
								header[hidx].df = 1;
								posting[pidx] = t2 -> docid;
							}
						}
						
						hnum = hidx + 1;
						pnum = pidx + 1;
						fclose(fp);
						
						fp = fopen(headerfilename, "wb");
						fwrite( header, sizeof(tHEADER), hnum, fp);
						fclose(fp);
						
						fp = fopen(postingfilename, "wb");
						fwrite( posting, sizeof(int), pnum, fp);
						fclose(fp);
						
						free(header);
						free(posting);
						
						return;
					}

////////////////////////////////////////////////////////////////////////////////
static void print_tokens( tTokenDoc *tokens, int num_tokens)
{
	int i;
	
	for (i = 0; i < num_tokens; i++)
	{
		printf( "%s\t%d\n", tokens[i].token, tokens[i].docid);
	}
}

////////////////////////////////////////////////////////////////////////////////
void destroy_tokens( tTokenDoc *tokens, int num_tokens)
{
	int i;
	
	for (i = 0; i < num_tokens; i++)
	{
		free( tokens[i].token);
	}
	free( tokens);
}

////////////////////////////////////////////////////////////////////////////////
int main( int argc, char **argv)
{
	tTokenDoc *tokens;
	int num_tokens = 0;

	if (argc != 2)
	{
		printf( "Usage: %s FILE\n", argv[0]);
		return 2;
	}
	
	tokens = get_tokens( argv[1], &num_tokens);
	
	assert( tokens != NULL && num_tokens > 0);

#if DEBUG	
	print_tokens( tokens, num_tokens);
#endif

	// 정렬 (첫번째 정렬 기준: 토큰 문자열, 두번째 정렬 기준: 문서 번호)
	qsort( tokens, num_tokens, sizeof( tTokenDoc), _compare);
	
	invertedIndex( tokens, num_tokens, "dic.txt", "header.idx", "posting.idx");

#if DEBUG	
	print_tokens( tokens, num_tokens);
#endif	

	destroy_tokens( tokens, num_tokens);
	
	return 0;
}
