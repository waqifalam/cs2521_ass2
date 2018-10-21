#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct NodeRep *Node;
struct NodeRep {
    int urlID;
    Node next;
};

Node newNode(int urlID);
Node newList(void);
Node appendList(Node head, int urlID);
void showList(Node head);
int nNodes(Node head);
int NodeinList(Node node, int urlID);

int main(int argc, char const *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: searchPagerank.exe word1 word 2 ..\n");
    }

    int urlID = -1;
    char textbuffer[1024];
    FILE *urlStream = fopen("invertedIndex.txt", "r");
    Node head = newList();

    for (size_t i = 1; i < argc; i++) {
        while (fscanf(urlStream, "%s", textbuffer) && strcmp(argv[i], textbuffer) != 0);

        while (fscanf(urlStream, "%s", textbuffer) == 1 && strstr(textbuffer, "url") != NULL) {
            if (textbuffer[3] < '0' || textbuffer[3] > '9') break;
            urlID = atoi(&textbuffer[3]);
            head = appendList(head, urlID);
        }
        rewind(urlStream);
    }

    fclose(urlStream);
    FILE *rankSteam = fopen("pagerankList.txt", "r");
    int nDisplayed = 0;

    while (fgets(textbuffer, 1024, rankSteam) != NULL && nDisplayed < 30) {
        urlID = atoi(&textbuffer[3]);
        if (NodeinList(head, urlID)) {
            printf("url%d\n", urlID);
            nDisplayed++;
        }
    }

    fclose(rankSteam);
    return 0;
}

Node newNode(int urlID) {
    Node new = malloc(sizeof(struct NodeRep));
    new->urlID = urlID;
    new->next = NULL;
    return new;
}

Node newList(void) {
    return NULL;
}

Node appendList(Node head, int urlID) {
    if (head == NULL) return newNode(urlID);

    Node curr = NULL;
    for (curr = head; curr != NULL; curr = curr->next) {
        if (curr->urlID == urlID) return head; // ignore repeats
        if (curr->next == NULL) break;
    }

    curr->next = newNode(urlID);
    return head;
}

void showList(Node head) {
    Node curr = NULL;
    for (curr = head; curr != NULL; curr = curr->next) printf("url%d ", curr->urlID);
    printf("\n");
}

int nNodes(Node head) {
    if (!head) return 0;
    return 1 + nNodes(head->next);
}

int NodeinList(Node node, int urlID) {
    if (node == NULL) return 0;
    if (node->urlID == urlID) return 1;
    return NodeinList(node->next, urlID);
}
