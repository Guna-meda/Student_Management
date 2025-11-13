#ifndef UTILS_H
#define UTILS_H

#include "datastructures/avl.h"

/* utils.h – add after bookRoot */
#include "datastructures/hashmap.h"
#include "datastructures/trie.h"
#include "datastructures/graph.h"
#include "datastructures/priorityqueue.h"

typedef struct {
    AVLNode       *bookRoot;
    HashMap       *memberMap;
    TrieNode      *titleTrie;
    Graph         *recommendGraph;
    PriorityQueue *reservationPQ;
} Library;

void initLibrary(Library *lib);

#endif
