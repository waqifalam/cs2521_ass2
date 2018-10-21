#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "BSTree.h"

void convertLowerCase(char *s);
void NormaliseString(char *s);

int main(int argc, char const *argv[]) {
    char buffer[256];
    char url_buffer[256];
    int x = 0;
    FILE *f = fopen("collection.txt", "r");
    BSTree t = newBSTree();

    while (fscanf(f, "%s", buffer) == 1) {
        int urlID = atoi(&buffer[3]);
        strcat(buffer, ".txt");
        FILE *g = fopen(buffer, "r");
        while (fscanf(g, "%s", url_buffer) && strcmp("Section-2", url_buffer) != 0);

        while (fscanf(g, "%s", url_buffer) == 1 && strcmp("#end", url_buffer) != 0) {
            convertLowerCase(url_buffer);
            NormaliseString(url_buffer);
            t = addBSTNode(t, url_buffer, urlID);
        }

        fclose(g);
    }

    fclose(f);
    FILE *output = fopen("invertedIndex.txt", "w");
    outputBSTree(t, output);
    fclose(output);
    return 0;
}

void convertLowerCase(char *s) {
    int n = strlen(s);
    int i = 0;

    for (i = 0; i < n; i++) {
        if ('A' <= s[i] && s[i] <= 'Z') s[i] = s[i] - 'A' + 'a';
    }
}

void NormaliseString(char *s) {
    int n = strlen(s);
    int i = 0;

    for (i = 0; i < n; i++) {
        if (s[i] < 'a' || s[i] > 'z') {
            s[i] = '\0';
            return;
        }
    }
}
