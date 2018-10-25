#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#define FALSE -100
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

    //Fill up array with words from the command line
	int counter = 1;
	int words = 0;
	char **array = NULL;
	while (counter < argc){
		normaliseWord(argv[counter]);
		array = realloc(array, (words+1)*sizeof(char*));
		array[words] = malloc(strlen(argv[counter])+1);
		strcpy(array[words],argv[counter]);
		words++;
		counter++;
	}
    int totalWords = words;

    //Fill up array1 with urls from collection.txt
	int URLs = 0;
    char **array1 = NULL;
    char string[MAXSTRING];
    FILE *filePointer = fopen("collection.txt", "r");
    if (filePointer == NULL){
        fprintf(stderr, "collection.txt not found!\n");
        int count;
        for (count =0; count < totalWords; count++){
            free(array[count]);
        }
        free(array);
		return EXIT_FAILURE;
    }
    while (fscanf(filePointer, "%s", string) != EOF) {
        array1 = realloc(array1, (URLs+1)*sizeof(char*));
        array1[URLs] = malloc(strlen(string)+1);
        strcpy(array1[URLs], string);
        URLs++;
    }

    int totalURLs = URLs;

    fclose(filePointer);

    // Declare frequency array to hold frequency for words
    int frequency[totalURLs];
    int count;
    for (count = 0; count < totalURLs; count++) {
        frequency[count] = 0;
    }

    // Declare tfidsValues array to hold tfidf for words
    double tfidfValues[totalURLs];
    for (count = 0; count < totalURLs; count++) {
        tfidfValues[count] = 0;
    }

    int i,j,k;
    i = j = k = 0;

    // iterate through urls and words
    // break if the url in collection.txt is not found
    // when found, calculate the tfidf and store in the array
    while (i < totalURLs) {
        while (j < totalWords) {
            if (tfOfWord(array1[i], array[j]) == FALSE || idfOfWord(array[j], totalURLs) == FALSE){
                for (count =0; count < totalWords; count++){
                    free(array[count]);
                }
                for (count =0; count < totalURLs; count++){
                    free(array1[count]);
                }
                free(array);
                free(array1);
                return EXIT_FAILURE;
            }
            tfidfValues[i] = tfidfValues[i] + tfOfWord(array1[i], array[j]) * idfOfWord(array[j], totalURLs);
            j++;
        }
        frequency[i] = wordFrequency(array1[i], array, totalWords);
        j = 0;
        i++;
    }

    // Sort the array for displaying
    sortArray(array1, frequency, totalURLs,tfidfValues);

    // Print every url with their tfidf values
    while (k < MAX && k < totalURLs) {
        if (frequency[k] != 0) {
            printf("%s  %.6f\n", array1[k], tfidfValues[k]);
        }
        k++;
    }

    // Free all the arrays used
    for (count =0; count < totalWords; count++){
        free(array[count]);
    }
    for (count =0; count < totalURLs; count++){
        free(array1[count]);
    }
    free(array);
    free(array1);

    return EXIT_SUCCESS;
}


void normaliseWord(char *ch){
    int i;
    char *c;
    char *s;

    // turn all the character in the word to uppercase
    for (i = 0; i < strlen(ch); i++) {
        ch[i] = tolower(ch[i]);
    }

    // if a character that is alphanumeric is found
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

    // Variable filename with the name of the file
    char filename[MAXSTRING];
    char extension[MAXSTRING];
    strcpy(filename, string);
    strcpy(extension, ".txt");
    char *currFile = strcat(filename, extension);
    FILE *file1 = fopen(currFile, "r");

    // currWord is a string that reads in from file
    char currWord[MAXSTRING];
    int c = 0;
    for (c = 0;c < MAXSTRING; c++){
        currWord[c] = '\0';
    }

    // if file is not found
    if (file1 == NULL){
        fprintf(stderr, "Could not find file %s\n", currFile);
		return FALSE;
    }

    // iterate through the file
    // if the word is found, increment count
    while (fscanf(file1, "%s", currWord) != EOF){
        normaliseWord(currWord);
        if (strncmp(array, currWord, sizeof(currWord)) == 0){
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
    // function calculates the idf of the word
    int documentNum = 0;
    char str[MAXSTRING];
    char newLine[MAXSTRING];
    char **twodarray = NULL;
    // open up the file invertedIndex.txt
    FILE *filePointer = fopen("invertedIndex.txt", "r");
    // if file not found return error
    if (filePointer == NULL){
        fprintf(stderr, "Could not find file invertedIndex.txt\n");
		return FALSE;
    }
    // read through the file
    // copy from the invertedIndex.txt into array
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
    int p;
    for (p = 0; p < documentNum; p++){
        free(twodarray[p]);
    }
    free(twodarray);
    //calculate idf
    if (documentNum > 0){
        double idf = log10((double)total/documentNum); 
        return idf;
    }
    return 0;
}

void sortArray(char **array, int *frequency, int len, double *tfidf){
    // function swaps and sorts array for printing in order
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
    // opens up the file
    // reads through the content
    // calculates the frequency of each word in the file
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
