#ifndef AVL_H
#define AVL_H

typedef struct AVLNode {
    char key[20];
    void *data;
    struct AVLNode *left, *right;
    int height;
} AVLNode;

AVLNode* insertNode(AVLNode* root, char* key, void* data);
AVLNode* deleteNode(AVLNode* root, char* key);
void* searchNode(AVLNode* root, char* key);
void inorderTraversal(AVLNode* root);

#endif
