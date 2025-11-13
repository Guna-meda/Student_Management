#ifndef AVL_H
#define AVL_H

typedef struct AVLNode {
    char key[64];
    void *data;
    struct AVLNode *left, *right;
    int height;
} AVLNode;

// Use consistent names
AVLNode* avl_insert(AVLNode* root, const char* key, void* data);
AVLNode* avl_delete(AVLNode* root, const char* key);
void* avl_search(AVLNode* root, const char* key);
void avl_inorder(AVLNode* root, void (*visit)(AVLNode* node));

#endif