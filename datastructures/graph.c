#include "graph.h"
#include <stdlib.h>
#include <string.h>

Graph* graph_create(void) {
    Graph *g = malloc(sizeof(Graph));
    g->nodes = hm_create(128);
    return g;
}

static GraphNode* node_create(const char *isbn) {
    GraphNode *n = calloc(1, sizeof(GraphNode));
    strncpy(n->isbn, isbn, sizeof(n->isbn)-1);
    return n;
}

static void node_add_adj(GraphNode *n, const char *isbn) {
    if (n->adjCount == n->adjCap) {
        n->adjCap = n->adjCap ? n->adjCap*2 : 4;
        n->adj = realloc(n->adj, n->adjCap * sizeof(char*));
    }
    n->adj[n->adjCount] = malloc(strlen(isbn) + 1);
    strcpy(n->adj[n->adjCount], isbn);
    ++n->adjCount;
}

void graph_add_edge(Graph *g, const char *isbn1, const char *isbn2) {
    GraphNode *n1 = hm_get(g->nodes, isbn1);
    if (!n1) { n1 = node_create(isbn1); hm_put(g->nodes, isbn1, n1); }
    node_add_adj(n1, isbn2);

    GraphNode *n2 = hm_get(g->nodes, isbn2);
    if (!n2) { n2 = node_create(isbn2); hm_put(g->nodes, isbn2, n2); }
    node_add_adj(n2, isbn1);
}

GraphNode* graph_get(Graph *g, const char *isbn) {
    return hm_get(g->nodes, isbn);
}

static void free_node(void *p) {
    GraphNode *n = p;
    for (int i = 0; i < n->adjCount; ++i) free(n->adj[i]);
    free(n->adj);
    free(n);
}

void graph_destroy(Graph *g) {
    hm_destroy(g->nodes, free_node);
    free(g);
}