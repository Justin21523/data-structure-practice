/* 02 二元搜尋樹示範（C）/ Binary Search Tree (BST) demo (C). */  // Bilingual header for this unit's public API.
#ifndef BST_H  // Header guard to prevent multiple inclusion.
#define BST_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* BST 節點 / BST node. */  // Bilingual type purpose.
typedef struct bst_node {  // Node stores key and pointers to left/right children.
    int key;  // Node key.
    struct bst_node* left;  // Left child pointer (NULL means no child).
    struct bst_node* right;  // Right child pointer (NULL means no child).
} bst_node;  // End of bst_node.

/* BST（只存 root）/ BST wrapper. */  // Bilingual type purpose.
typedef struct bst_tree {  // Represent a BST by its root pointer.
    bst_node* root;  // Root pointer (NULL means empty tree).
} bst_tree;  // End of bst_tree.

/* 動態 int 陣列（給 inorder 回傳用）/ Dynamic int array for inorder traversal. */  // Bilingual type purpose.
typedef struct bst_int_array {  // Own a heap-allocated int buffer plus its length.
    int* data;  // Heap buffer (NULL when len is 0).
    int len;  // Number of valid ints in data.
} bst_int_array;  // End of bst_int_array.

void bst_init(bst_tree* tree);  // Initialize an empty BST (root=NULL).
void bst_destroy(bst_tree* tree);  // Free all nodes in the BST and reset root to NULL.

bool bst_contains(const bst_tree* tree, int key);  // Return true if key exists in the BST.
bool bst_insert(bst_tree* tree, int key);  // Insert key; return false on duplicate or allocation failure.
bool bst_delete(bst_tree* tree, int key);  // Delete key; return true if a node was removed.

bool bst_min(const bst_tree* tree, int* out_key);  // Write minimum key into out_key; return false if empty.
bool bst_max(const bst_tree* tree, int* out_key);  // Write maximum key into out_key; return false if empty.

int bst_size(const bst_tree* tree);  // Return number of nodes (O(n)).
int bst_height(const bst_tree* tree);  // Return height measured in edges (empty=-1, leaf=0).

void bst_int_array_destroy(bst_int_array* arr);  // Free traversal arrays allocated by this unit and reset to empty.
bool bst_inorder(const bst_tree* tree, bst_int_array* out);  // Return inorder traversal (sorted keys) in out; allocates out->data.

#endif  // BST_H  // End of header guard.

