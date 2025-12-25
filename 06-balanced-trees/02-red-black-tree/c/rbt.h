/* 02 Red-Black Tree 示範（C, LLRB）/ Red-black tree demo (C, LLRB). */  // Bilingual header for this unit's public API.
#ifndef RBT_H  // Header guard to prevent multiple inclusion.
#define RBT_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for success/failure results.

/* RBT 節點（LLRB）/ LLRB node. */  // Bilingual type purpose.
typedef struct rbt_node {  // Node stores key, color, and pointers to left/right children.
    int key;  // Node key.
    struct rbt_node* left;  // Left child pointer (NULL means no child).
    struct rbt_node* right;  // Right child pointer (NULL means no child).
    bool red;  // Node color (true=RED, false=BLACK).
} rbt_node;  // End of rbt_node.

/* RBT（只存 root）/ Tree wrapper. */  // Bilingual type purpose.
typedef struct rbt_tree {  // Represent a tree by its root pointer.
    rbt_node* root;  // Root pointer (NULL means empty tree).
} rbt_tree;  // End of rbt_tree.

/* 動態 int 陣列（給 inorder 回傳用）/ Dynamic int array for inorder traversal. */  // Bilingual type purpose.
typedef struct rbt_int_array {  // Own a heap-allocated int buffer plus its length.
    int* data;  // Heap buffer (NULL when len is 0).
    int len;  // Number of valid ints in data.
} rbt_int_array;  // End of rbt_int_array.

void rbt_init(rbt_tree* tree);  // Initialize an empty tree (root=NULL).
void rbt_destroy(rbt_tree* tree);  // Free all nodes in the tree and reset root to NULL.

bool rbt_contains(const rbt_tree* tree, int key);  // Return true if key exists in the tree.
bool rbt_insert(rbt_tree* tree, int key);  // Insert key; return false on duplicate or allocation failure.
bool rbt_delete(rbt_tree* tree, int key);  // Delete key; return true if a node was removed.

bool rbt_min(const rbt_tree* tree, int* out_key);  // Write minimum key into out_key; return false if empty.
bool rbt_max(const rbt_tree* tree, int* out_key);  // Write maximum key into out_key; return false if empty.

int rbt_size(const rbt_tree* tree);  // Return number of nodes (O(n)).
int rbt_height(const rbt_tree* tree);  // Return height measured in edges (empty=-1, leaf=0).

void rbt_int_array_destroy(rbt_int_array* arr);  // Free traversal arrays allocated by this unit and reset to empty.
bool rbt_inorder(const rbt_tree* tree, rbt_int_array* out);  // Return inorder traversal (sorted keys) in out; allocates out->data.

bool rbt_validate(const rbt_tree* tree);  // Validate BST ordering + red-black properties (LLRB variant).

#endif  // RBT_H  // End of header guard.

