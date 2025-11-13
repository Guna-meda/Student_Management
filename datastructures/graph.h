#ifndef GRAPH_H
#define GRAPH_H

/* graph depends on HashMap for node storage */
#include "hashmap.h"

typedef struct {
    char isbn[20];
    char **adj;
    int adjCount;
    int adjCap;
} GraphNode;

typedef struct {
    HashMap *nodes;   /* key = isbn */
} Graph;

Graph*   graph_create(void);
void     graph_add_edge(Graph *g, const char *isbn1, const char *isbn2);
GraphNode* graph_get(Graph *g, const char *isbn);
void     graph_destroy(Graph *g);

#endif