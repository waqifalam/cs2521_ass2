#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 30

void permutation(int *a, int start, int end, int size);
void swap(int* str, int start, int end);

int main(int argc, char* argv[]){
	// Handle error
	if (argc < 2){
		fprintf(stderr, "Usage: ./scaledFootrule files...\n");
		return EXIT_FAILURE;
	}

	int str[MAX] = {0};
	int size = 0;

	for (int i = 0; i < argc-1; i++){
		char* filename;
		strcpy(filename, argv[i+1]);
		FILE* f = fopen(filename, "r");\
		char *string = malloc(MAX*sizeof(char));
		while (fscanf(f, "%s", string) == 1){
			string = string+3;
			int num = atoi(string);
			int flag = 0;
			for (int j = 0; j < size; j++){
				if (str[j] == num){
					flag = 1;
				}
			}
			if (flag == 0){
				str[size] = num;
				size++;
			}
		}
	}
    permutation(str, 0, size-1, size); 
    return 0;
}

void permutation(int *str, int start, int end, int size) 
{ 
   int count;
   if (start == end) {
    	for (int i = 0; i < size; i++){
    		printf("%d", str[i]);
    	}
    	printf("\n");
   }else
   { 
       for (count = start; count <= end; count++) 
       { 
          swap(str, start, count); 
          permutation(str, start+1, end, size); 
          swap(str,start,count);
       } 
   } 
} 

void swap(int* str, int start, int end) 
{
	int *a = str+start;
	int *b = str+end;
    int temp; 
    temp = *a; 
    *a = *b; 
    *b = temp; 
} 
