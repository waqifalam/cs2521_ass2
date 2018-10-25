#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "BSTree.h"

#define TRUE  1
#define FALSE 0

static int AlphabeticalOrder(char *x, char *y);
static void appendList(BSTree t, int urlID);
static void freeurlList(urlList head);

static urlList newurlList(int urlID) {
    urlList new = malloc(sizeof(struct ListNodeRep));
    new->urlID = urlID;
    new->next = NULL;
    return new;
}

static BSTNode newBSTNode(char *c, int urlID) {
    BSTNode new = malloc(sizeof(struct BSTNodeRep));
    new->left = new->right = NULL;
    new->word = malloc((strlen(c) + 1) * sizeof(char)); // NULL char
    strcpy(new->word, c);
    new->head = newurlList(urlID);
    return new;
}

BSTree newBSTree(void) {
    return NULL;
}

BSTree addBSTNode(BSTree t, char *c, int urlID) {
    if (t == NULL) {
        return newBSTNode(c, urlID);
    } else if (AlphabeticalOrder(c, t->word)) {
        t->left = addBSTNode(t->left, c, urlID);
    } else if (AlphabeticalOrder(t->word, c)) {
        t->right = addBSTNode(t->right, c, urlID);
    } else {
        appendList(t, urlID); // exact same word
    }

    return t;
}

static int AlphabeticalOrder(char *x, char *y) {
    if (*x == *y && *x == '\0') return FALSE; // exact same word
    if (*x == *y) return AlphabeticalOrder(x + sizeof(char), y + sizeof(char)); // next character
    if (*x < *y) return TRUE;
    return FALSE;
}

// Appends list similar to insertion sort
static void appendList(BSTree t, int urlID) {
    assert(t->head != NULL);
    urlList curr = NULL;
    for (curr = t->head; curr != NULL; curr = curr->next) {
        if (urlID == curr->urlID) return; // same ID
    }

    // urlID lower than original lowest
    if (urlID < t->head->urlID) {
        urlList tmp = t->head;
        t->head = newurlList(urlID);
        t->head->next = tmp;
        return;
    }

    curr = t->head;
    urlList prev = curr;
    curr = curr->next;

    while (curr != NULL) {
        if (urlID < curr->urlID) {
            // insert in order
            prev->next = newurlList(urlID);
            prev->next->next = curr;
            return;
        }
        prev = prev->next;
        curr = curr->next;
    }

    prev->next = newurlList(urlID); // insert at last
}

static void showBSTNode(BSTNode t) {
    printf("%s: ", t->word);
    urlList curr = NULL;
    for (curr = t->head; curr != NULL; curr = curr->next) printf("%d ", curr->urlID);
    printf("\n");
}

// show infix order
void showBSTree(BSTree t) {
    if (t == NULL) return;
    showBSTree(t->left);
    showBSTNode(t);
    showBSTree(t->right);
}

static void outputBSTNode(BSTNode t, FILE *f) {
    fprintf(f, "%s  ", t->word);
    for (urlList curr = t->head; curr != NULL; curr = curr->next) fprintf(f, "url%d ", curr->urlID);
    fprintf(f, "\n");
}

// output infix order to stream f
void outputBSTree(BSTree t, FILE *f) {
    if (t == NULL) return;
    outputBSTree(t->left, f);
    outputBSTNode(t, f);
    outputBSTree(t->right, f);
}

void freeBSTree(BSTree t) {
    if (t == NULL) return;
    freeBSTree(t->left);
    freeBSTree(t->right);
    free(t->word);
    freeurlList(t->head);
    free(t);
}

static void freeurlList(urlList head) {
    if (head == NULL) return;
    urlList curr = head;
    for (urlList tmp = curr->next; tmp != NULL; tmp = tmp->next) {
        free(curr);
        curr = tmp;
    }
    free(curr);
}
