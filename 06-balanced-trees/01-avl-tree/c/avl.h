/* 01 AVL Tree 示範（C）/ AVL tree demo (C). */  // Bilingual header for this unit's public API.
#ifndef AVL_H  // Header guard to prevent multiple inclusion.
#define AVL_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* AVL 節點 / AVL node. */  // Bilingual type purpose.
typedef struct avl_node {  // Node stores key, cached height, and pointers to left/right children.
    int key;  // Node key.
    struct avl_node* left;  // Left child pointer (NULL means no child).
    struct avl_node* right;  // Right child pointer (NULL means no child).
    int height;  // Cached height in edges (leaf=0 by definition).
} avl_node;  // End of avl_node.

/* AVL Tree（只存 root）/ AVL tree wrapper. */  // Bilingual type purpose.
typedef struct avl_tree {  // Represent an AVL tree by its root pointer.
    avl_node* root;  // Root pointer (NULL means empty tree).
} avl_tree;  // End of avl_tree.

/* 動態 int 陣列（給 inorder 回傳用）/ Dynamic int array for inorder traversal. */  // Bilingual type purpose.
typedef struct avl_int_array {  // Own a heap-allocated int buffer plus its length.
    int* data;  // Heap buffer (NULL when len is 0).
    int len;  // Number of valid ints in data.
} avl_int_array;  // End of avl_int_array.

void avl_init(avl_tree* tree);  // Initialize an empty AVL tree (root=NULL).
void avl_destroy(avl_tree* tree);  // Free all nodes in the AVL tree and reset root to NULL.

bool avl_contains(const avl_tree* tree, int key);  // Return true if key exists in the tree.
bool avl_insert(avl_tree* tree, int key);  // Insert key; return false on duplicate or allocation failure.
bool avl_delete(avl_tree* tree, int key);  // Delete key; return true if a node was removed.

bool avl_min(const avl_tree* tree, int* out_key);  // Write minimum key into out_key; return false if empty.
bool avl_max(const avl_tree* tree, int* out_key);  // Write maximum key into out_key; return false if empty.

int avl_size(const avl_tree* tree);  // Return number of nodes (O(n)).
int avl_height(const avl_tree* tree);  // Return height measured in edges (empty=-1, leaf=0).

void avl_int_array_destroy(avl_int_array* arr);  // Free traversal arrays allocated by this unit and reset to empty.
bool avl_inorder(const avl_tree* tree, avl_int_array* out);  // Return inorder traversal (sorted keys) in out; allocates out->data.

bool avl_validate(const avl_tree* tree);  // Validate BST ordering + AVL balance + cached height correctness.

#endif  // AVL_H  // End of header guard.

