#include "trie.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static TrieNode* new_node(char c) {
    TrieNode *n = calloc(1, sizeof(TrieNode));
    n->ch = c;
    return n;
}

/* lower-case only for simplicity */
TrieNode* trie_insert(TrieNode *root, const char *word, void *payload) {
    if (!root) root = new_node('\0');
    TrieNode *cur = root;
    for (const char *p = word; *p; ++p) {
        char c = tolower(*p);
        int idx = c - 'a';
        if (idx < 0 || idx > 25) continue;   /* ignore non-letter */
        if (!cur->child[idx]) cur->child[idx] = new_node(c);
        cur = cur->child[idx];
    }
    cur->isEnd = 1;
    cur->payload = payload;
    return root;
}

/* collect every word that starts with prefix */
static void collect(TrieNode *node, char *buf, int pos,
                    void ***out, int *cnt, int *cap) {
    if (!node) return;
    if (node->isEnd) {
        buf[pos] = '\0';
        if (*cnt == *cap) {
            *cap = *cap ? *cap*2 : 8;
            *out = realloc(*out, *cap * sizeof(void*));
        }
        (*out)[(*cnt)++] = node->payload;
    }
    for (int i = 0; i < 26; ++i)
        if (node->child[i]) {
            buf[pos] = node->child[i]->ch;
            collect(node->child[i], buf, pos+1, out, cnt, cap);
        }
}

void* trie_prefix_search(TrieNode *root, const char *prefix,
                         void ***results, int *count) {
    *results = NULL; *count = 0;
    if (!root) return NULL;

    TrieNode *cur = root;
    for (const char *p = prefix; *p; ++p) {
        int idx = tolower(*p) - 'a';
        if (idx < 0 || idx > 25 || !cur->child[idx]) return NULL;
        cur = cur->child[idx];
    }
    /* now cur is the node after the prefix */
    char buf[256];
    int cap = 0;
    collect(cur, buf, 0, results, count, &cap);
    return cur;
}

static void trie_free_rec(TrieNode *n, void (*free_payload)(void*)) {
    if (!n) return;
    for (int i = 0; i < 26; ++i) trie_free_rec(n->child[i], free_payload);
    if (n->isEnd && free_payload) free_payload(n->payload);
    free(n);
}

void trie_free(TrieNode *root, void (*free_payload)(void*)) {
    trie_free_rec(root, free_payload);
}