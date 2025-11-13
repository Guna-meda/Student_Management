#include "avl.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Use unique names to avoid conflicts with platform macros */
static int avl_max(int a, int b) { return (a > b) ? a : b; }
static int avl_height(AVLNode *n) { return n ? n->height : 0; }

static AVLNode* new_node(const char* key, void* data) {
    AVLNode* node = (AVLNode*)malloc(sizeof(AVLNode));
    if (!node) return NULL;
    strncpy(node->key, key, sizeof(node->key)-1);
    node->key[sizeof(node->key)-1] = '\0';
    node->data = data;
    node->left = node->right = NULL;
    node->height = 1;
    return node;
}

static AVLNode* right_rotate(AVLNode* y) {
    AVLNode* x = y->left;
    AVLNode* T2 = x->right;

    x->right = y;
    y->left = T2;

    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;
    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;

    return x;
}

static AVLNode* left_rotate(AVLNode* x) {
    AVLNode* y = x->right;
    AVLNode* T2 = y->left;

    y->left = x;
    x->right = T2;

    x->height = avl_max(avl_height(x->left), avl_height(x->right)) + 1;
    y->height = avl_max(avl_height(y->left), avl_height(y->right)) + 1;

    return y;
}

static int get_balance(AVLNode* n) {
    if (!n) return 0;
    return avl_height(n->left) - avl_height(n->right);
}

/* Insert key (string) with data pointer */
AVLNode* avl_insert(AVLNode* node, const char* key, void* data) {
    if (!node) return new_node(key, data);

    int cmp = strcmp(key, node->key);
    if (cmp < 0)
        node->left = avl_insert(node->left, key, data);
    else if (cmp > 0)
        node->right = avl_insert(node->right, key, data);
    else {
        /* duplicate key: update data pointer (caller responsible for freeing old if needed) */
        node->data = data;
        return node;
    }

    node->height = 1 + avl_max(avl_height(node->left), avl_height(node->right));
    int balance = get_balance(node);

    /* Left Left */
    if (balance > 1 && strcmp(key, node->left->key) < 0)
        return right_rotate(node);
    /* Right Right */
    if (balance < -1 && strcmp(key, node->right->key) > 0)
        return left_rotate(node);
    /* Left Right */
    if (balance > 1 && strcmp(key, node->left->key) > 0) {
        node->left = left_rotate(node->left);
        return right_rotate(node);
    }
    /* Right Left */
    if (balance < -1 && strcmp(key, node->right->key) < 0) {
        node->right = right_rotate(node->right);
        return left_rotate(node);
    }

    return node;
}

/* Helper to find node with minimum key in subtree */
static AVLNode* min_value_node(AVLNode* node) {
    AVLNode* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

AVLNode* avl_delete(AVLNode* root, const char* key) {
    if (!root) return root;

    int cmp = strcmp(key, root->key);
    if (cmp < 0)
        root->left = avl_delete(root->left, key);
    else if (cmp > 0)
        root->right = avl_delete(root->right, key);
    else {
        /* node with only one child or no child */
        if (!root->left || !root->right) {
            AVLNode* temp = root->left ? root->left : root->right;
            if (!temp) {
                /* no child */
                temp = root;
                root = NULL;
            } else {
                /* one child: copy contents */
                *root = *temp;
            }
            free(temp);
        } else {
            /* two children: get inorder successor */
            AVLNode* temp = min_value_node(root->right);
            strncpy(root->key, temp->key, sizeof(root->key)-1);
            root->key[sizeof(root->key)-1] = '\0';
            root->data = temp->data;
            root->right = avl_delete(root->right, temp->key);
        }
    }

    if (!root) return root;

    root->height = 1 + avl_max(avl_height(root->left), avl_height(root->right));
    int balance = get_balance(root);

    /* Left Left */
    if (balance > 1 && get_balance(root->left) >= 0)
        return right_rotate(root);
    /* Left Right */
    if (balance > 1 && get_balance(root->left) < 0) {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    /* Right Right */
    if (balance < -1 && get_balance(root->right) <= 0)
        return left_rotate(root);
    /* Right Left */
    if (balance < -1 && get_balance(root->right) > 0) {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

void* avl_search(AVLNode* root, const char* key) {
    AVLNode* cur = root;
    while (cur) {
        int cmp = strcmp(key, cur->key);
        if (cmp == 0) return cur->data;
        cur = (cmp < 0) ? cur->left : cur->right;
    }
    return NULL;
}

void avl_inorder(AVLNode* root, void (*visit)(AVLNode* node)) {
    if (!root) return;
    avl_inorder(root->left, visit);
    visit(root);
    avl_inorder(root->right, visit);
}
