#include "utils.h"
#include "member.h"
#include <stdlib.h>
#include <time.h>

static void free_avl_data(AVLNode *n) {
    if (n && n->data) free(n->data);
}

void initLibrary(Library *lib) {
    if (!lib) return;
    lib->bookRoot        = NULL;
    lib->memberMap       = hm_create(64);
    lib->titleTrie       = NULL;
    lib->recommendGraph  = graph_create();
    lib->reservationPQ   = pq_create();
}

/* optional cleanup (call before exit) */
void destroyLibrary(Library *lib) {
    /* free AVL payloads */
    /* (you would need a traversal that frees each Book*) */
    avl_inorder(lib->bookRoot, free_avl_data);
    /* then free the tree nodes themselves – write a post-order free if needed */

    hm_destroy(lib->memberMap, free_member_ext);
    trie_free(lib->titleTrie, NULL);
    graph_destroy(lib->recommendGraph);
    pq_destroy(lib->reservationPQ);
}
