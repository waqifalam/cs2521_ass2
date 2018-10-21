#ifndef BSTREE_H
#define BSTREE_H

typedef struct BSTNodeRep *BSTree;
typedef struct BSTNodeRep *BSTNode;
typedef struct ListNodeRep *urlList;

struct ListNodeRep {
    int urlID;
    urlList next;
};

struct BSTNodeRep {
    BSTree left;
    BSTree right;
    char *word;
    urlList head;
};

BSTree newBSTree(void);
BSTree addBSTNode(BSTree t, char *c, int urlID);
void showBSTree(BSTree t);
void outputBSTree(BSTree t, FILE *f);

#endif

// References:

// https://www.cse.unsw.edu.au/~cs2521/18s2/assignments/ass02/HowToImplement-Ass2-Part1.pdf
//
// ADT inspiration
