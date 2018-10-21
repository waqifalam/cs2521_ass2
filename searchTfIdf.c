#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define MAX 30
#define MAXSTRING 2401

int wordFrequency(char *filename, char **array, int size);
void sortArray(char **array, int *frequency, int len, double *tfidf);
double idfOfWord(char *word, int total);
double tfOfWord(char *string, char *array);
void normaliseWord(char *ch);

int main(int argc, char *argv[]){
	// Handle error
	if (argc < 2){
		fprintf(stderr, "Usage: ./searchTfIdf words...\n");
		return EXIT_FAILURE;
	}

	int counter = 1;
	int words = 0;
	// Using 2D array
	char **array = NULL;
	while (counter < argc){
		normaliseWord(argv[counter]);
		array = realloc(array, (words+1)*sizeof(char*));
		array[words] = malloc(sizeof(char)*(strlen(argv[counter])+1));
		strncpy(array[words],argv[counter], strlen(argv[counter]));
		words++;
		counter++;
	}
	int totalWords = words;

	int URLs = 0;
    char **array1 = NULL;
    char string[MAXSTRING];
    FILE *filePointer = fopen("collection.txt", "r");
    while (fscanf(filePointer, "%s", string) != EOF) {
        array1 = realloc(array1, (URLs+1)*sizeof(char*));
        array1[URLs] = malloc(strlen(string)+1);
        strcpy(array1[URLs], string);
        URLs++;
    }

    int totalURLs = URLs;

    fclose(filePointer);

    int frequency[totalURLs]; 
    for (int count = 0; count < totalURLs; count++) {
        frequency[count] = 0;
    }

    double tfidfValues[totalURLs];
    for (int count = 0; count < totalURLs; count++) {
        tfidfValues[count] = 0;
    }

    int i,j,k;
    i = j = k = 0;

    while (i < totalURLs) {
        while (j < totalWords) {
            tfidfValues[i] = tfidfValues[i] + tfOfWord(array1[i], array[j])*idfOfWord(array[j], totalURLs);
            j++;
        }
        frequency[i] = wordFrequency(array1[i], array, totalWords);
        j = 0;
        i++;
    }

    sortArray(array1, frequency, totalURLs,tfidfValues);
    while (k < MAX && k < totalURLs) {
        if (frequency[k] != 0) {
            printf("%s  %.6f\n", array1[k], tfidfValues[k]);
        }
        k++;
    }

    free(array);
    free(array1);

    return EXIT_SUCCESS;
}


void normaliseWord(char *ch){
    int i;
    char *c;
    char *s;

    for (i = 0; i < strlen(ch); i++) {
        ch[i] = tolower(ch[i]);
    }
    s = c = ch;
    while (*s){
        *c = *s++;
        if (*c != '.' && *c != ',' && *c != '?' && *c != ';' && *c != ' '){
            c++;
        }
    }
    *c = '\0'; 
}

double tfOfWord(char *string, char *array){
    double count = 0;
    char filename[MAXSTRING];
    char extension[MAXSTRING];
    strcpy(filename, string);
    strcpy(extension, ".txt");
    char *currFile = strcat(filename, extension);
    FILE *file1 = fopen(currFile, "r");
    char currWord[MAXSTRING];
    while (fscanf(file1, "%s", currWord) != EOF){
        normaliseWord(currWord);
        if (strcmp(array, currWord) == 0){
            count = count + 1;
        }
    }

    fclose(file1);


    double totalCount = 0;
    int flag = 0;
    char nextWord[MAXSTRING];
    FILE *secondFile = fopen(currFile, "r");
    while (fscanf(secondFile, "%s", nextWord) != EOF){
        if (strcmp(nextWord, "#end") == 0){
            flag = 0;
        }
        if (flag == 1){
            totalCount = totalCount + 1;
        }
        if (strcmp(nextWord, "Section-2") == 0){
            flag = flag + 1;
        }
    }

    fclose(secondFile);

    if (totalCount > 0){
        double result = count/totalCount;
        return result;
    }
    return 0;
}

double idfOfWord(char *word, int total){
    int documentNum = 0;
    char str[MAXSTRING];
    char newLine[MAXSTRING];
    char **twodarray = NULL;
    FILE *filePointer = fopen("invertedIndex.txt", "r");
    while (fscanf(filePointer, "%s", str) != EOF){
        if (strcmp(word, str) == 0){
            while (fscanf(filePointer, "%99s%99[ \t\n]", str, newLine) == 2){
                twodarray = realloc(twodarray, (documentNum+1)*sizeof(char*));
                twodarray[documentNum] = malloc(strlen(str)+1);
                strcpy(twodarray[documentNum], str);
                documentNum++;
                if (strchr(newLine, '\n')){
                    break;
                }
            }
        }
    }

    free(twodarray);
    if (documentNum > 0){
        double idf = log10((double)total/documentNum); 
        return idf;
    }
    return 0;
}

void sortArray(char **array, int *frequency, int len, double *tfidf){
    int count1;
    int count2;
    int count3;
    double tmp;
    char *tmp2;

    for (count1 = 0; count1 < len; count1++){
        for (count2 = count1 + 1; count2 < len; count2++){
            if (frequency[count1] < frequency[count2]){
                count3 = frequency[count1];
                frequency[count1] = frequency[count2];
                frequency[count2] = count3;
                tmp = tfidf[count1];
                tfidf[count1] = tfidf[count2];
                tfidf[count2] = tmp;
                tmp2 = array[count1];
                array[count1] = array[count2];
                array[count2] = tmp2;
            }
            if (frequency[count1] == frequency[count2] && tfidf[count1] < tfidf[count2]){
                count3 = frequency[count1];
                frequency[count1] = frequency[count2];
                frequency[count2] = count3;
                tmp = tfidf[count1];
                tfidf[count1] = tfidf[count2];
                tfidf[count2] = tmp;
                tmp2 = array[count1];
                array[count1] = array[count2];
                array[count2] = tmp2;
            }
        }
    }
}

int wordFrequency(char *filename, char **array, int size){
    int count = 0;
    char filePointer[MAXSTRING];
    char extenstion[MAXSTRING];
    strcpy(filePointer, filename);
    strcpy(extenstion, ".txt");
    char *current = strcat(filePointer, extenstion);
    
    int count2 = 0;
    char s[MAXSTRING];
    int flag = 0;

    while (count2 < size) {
        FILE *filePointer2 = fopen(current, "r");
        while (fscanf(filePointer2, "%s", s) != EOF && flag != 1) {
            normaliseWord(s);
            if (strcmp(array[count2], s) == 0) {
                count++;
                flag = 1;
            }
        }
        flag = 0;
        fclose(filePointer2);
        count2++;
    }
    return count;
}