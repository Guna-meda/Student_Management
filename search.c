#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#include "book.h"
#include "datastructures/trie.h"
#include "datastructures/graph.h"
#include "member.h"

void autoComplete(Library *lib) {
    char prefix[64];
    printf("Enter title prefix: "); scanf("%s", prefix);

    void **books = NULL;
    int cnt = 0;
    trie_prefix_search(lib->titleTrie, prefix, &books, &cnt);

    if (cnt == 0) {
        printf("No titles found.\n");
    } else {
        printf("--- %d matches ---\n", cnt);
        for (int i = 0; i < cnt; ++i) {
            Book *b = (Book*)books[i];
            printf("%s (ISBN:%s) – %d copies\n", b->title, b->isbn, b->copies);
        }
    }
    free(books);
}

/* ------------------------------------------------------------------ */
/* Recommendation – very small “co-borrow” graph */
void recommendBooks(Library *lib) {
    char id[64];
    printf("Enter Member ID: "); scanf("%s", id);
    MemberExt *m = hm_get(lib->memberMap, id);
    if (!m) { printf("Member not found.\n"); return; }

    /* collect all ISBNs the member currently borrows */
    char borrowed[32][20];
    int n = 0;
    for (int i = 0; i < m->borrowedCount && n < 32; ++i)
        strcpy(borrowed[n++], m->borrowed[i].isbn);

    printf("--- Recommendations for %s ---\n", id);
    int printed = 0;
    for (int i = 0; i < n; ++i) {
        GraphNode *gn = graph_get(lib->recommendGraph, borrowed[i]);
        if (!gn) continue;
        for (int j = 0; j < gn->adjCount; ++j) {
            const char *otherIsbn = gn->adj[j];
            Book *b = avl_search(lib->bookRoot, otherIsbn);
            if (b && b->copies > 0) {
                printf("%s – %s (%d copies)\n", b->title, b->isbn, b->copies);
                ++printed;
                if (printed >= 5) goto done;
            }
        }
    }
done:
    if (!printed) printf("No recommendations available.\n");
}