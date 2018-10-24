#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef struct NodeRep *Node;
struct NodeRep {
    int urlID;
    Node next;
};

typedef struct LListRep *LList;
struct LListRep {
	int size;
	Node head;
};

void permutation(int *str, int start, int end, int size, int uniqueURL[], LList url[], int nList);
void swap(int* str, int start, int end);
Node newNode(int urlID);
LList newLList(void);
void appendList(LList list, int urlID);
void showList(LList list);
int NodeinList(LList list, int urlID);
int calculate_union_size(LList url[], int uniqueURL[], int nList, int maxSize);

int main(int argc, char const *argv[]) {
	// Handle error
	if (argc < 2) {
		fprintf(stderr, "Usage: ./scaledFootrule files...\n");
		return EXIT_FAILURE;
	}

	char buffer[64];
	LList url[argc - 1];

	for (int i = 1; i < argc; i++) {
		FILE *f = fopen(argv[i], "r");
		url[i - 1] = newLList();
		while (fscanf(f, "%s", buffer) == 1) {
			int urlID = atoi(&buffer[3]);
			appendList(url[i - 1], urlID);
		}
		fclose(f);
	}

	int maxSize = 0;
	for (int i = 0; i < argc - 1; i++) maxSize = maxSize + url[i]->size;

  int uniqueURL[maxSize];
	int union_size = calculate_union_size(url, uniqueURL, argc - 1, maxSize);
  int position[union_size];
    for (int i = 0; i < union_size; i++) position[i] = i + 1;
    permutation(position, 0, union_size - 1, union_size, uniqueURL, url, argc - 1);

    return 0;
}

void permutation(int *str, int start, int end, int size, int uniqueURL[], LList url[], int nList);
{
	if (start == end) {
		for (int i = 0; i < size; i++) printf("%d", str[i]);
    printf("\n");

    double weight = 0;
    for (int i = 0; i < nList; i++) {
      double j = 0;
      for (Node curr = url[i]->head; curr != NULL; curr = curr->next) {
        weight = weight + fabs(j / url[i]->size - findIndex(curr));
        j++
      }
    }
	} else {
		for (int count = start; count <= end; count++) {
    		swap(str, start, count);
          	permutation(str, start + 1, end, size, uniqueURL, url, nList);
          	swap(str, start, count);
      	}
  	}
}

void swap(int* str, int start, int end)
{
	int *a = str + start;
	int *b = str + end;
    int temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

Node newNode(int urlID) {
    Node new = malloc(sizeof(struct NodeRep));
    new->urlID = urlID;
    new->next = NULL;
    return new;
}

LList newLList(void) {
	LList new = malloc(sizeof(struct LListRep));
	new->size = 0;
	new->head = NULL;
	return new;
}

void appendList(LList list, int urlID) {
	if (list->head == NULL) {
		list->head = newNode(urlID);
		list->size = 1;
		return;
	}

	Node curr = NULL;
    for (curr = list->head; curr != NULL; curr = curr->next) {
        if (curr->urlID == urlID) return; // ignore repeats
        if (curr->next == NULL) break;
    }

	curr->next = newNode(urlID);
	list->size = list->size + 1;
}

void showList(LList list) {
    for (Node curr = list->head; curr != NULL; curr = curr->next) printf("url%d ", curr->urlID);
    printf("\n");
}

int NodeinList(LList list, int urlID) {
	for (Node curr = list->head; curr != NULL; curr = curr->next) {
		if (curr->urlID == urlID) return 1;
	}
	return 0;
}

int calculate_union_size(LList url[], int uniqueURL[], int nList, int maxSize) {
	int x = 0;
	int repeat_found = 0;
	for (int i = 0; i < nList; i++) {
		for (Node curr = url[i]->head; curr != NULL; curr = curr->next) {
			for (int j = 0; j < x; j++) {
				if (uniqueURL[j] == curr->urlID) repeat_found = 1;
			}
			if (!repeat_found) uniqueURL[x++] = curr->urlID;
			repeat_found = 0;
		}
	}

	return x;
}
