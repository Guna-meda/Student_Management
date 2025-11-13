#ifndef UTILS_H
#define UTILS_H

#include "datastructures/avl.h"

/* central context passed around to avoid globals */
typedef struct {
    AVLNode *bookRoot;   /* AVL tree root for books (key = isbn) */
    /* later add:
       HashMap *memberMap;
       Trie *titleTrie;
       Graph *recommendationGraph;
       PriorityQueue *reservationPQ;
    */
} Library;

void initLibrary(Library *lib);

#endif
