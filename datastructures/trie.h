#ifndef TRIE_H
#define TRIE_H

typedef struct TrieNode {
    char ch;
    struct TrieNode *child[26];
    int isEnd;               /* 1 if a full word ends here */
    void *payload;           /* Book* for title search */
} TrieNode;

TrieNode* trie_insert(TrieNode *root, const char *word, void *payload);
void*     trie_prefix_search(TrieNode *root, const char *prefix, void ***results, int *count);
void      trie_free(TrieNode *root, void (*free_payload)(void*));

#endif