/* 03 樹走訪示範（C）/ Tree traversal demo (C). */  // Bilingual header for this unit's public API.
#ifndef TREE_TRAVERSAL_H  // Header guard to prevent multiple inclusion.
#define TREE_TRAVERSAL_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for optional values and success/failure results.

/* 可選整數（用來表達 null）/ Optional int (to represent null). */  // Bilingual type purpose.
typedef struct tt_optional_int {  // Represent an "int or null" value for level-order input.
    bool is_present;  // True when value is present; false means "null".
    int value;  // The stored value (ignored when is_present is false).
} tt_optional_int;  // End of tt_optional_int.

#define TT_SOME(x) ((tt_optional_int){true, (x)})  // Construct a present optional value.
#define TT_NONE ((tt_optional_int){false, 0})  // Construct a missing optional value (null).

/* 二元樹節點 / Binary tree node. */  // Bilingual type purpose.
typedef struct tt_node {  // Node stores value and pointers to left/right children.
    int value;  // Node value.
    struct tt_node* left;  // Left child pointer (NULL means no child).
    struct tt_node* right;  // Right child pointer (NULL means no child).
} tt_node;  // End of tt_node.

/* 二元樹（只存 root）/ Binary tree wrapper. */  // Bilingual type purpose.
typedef struct tt_tree {  // Represent a binary tree by its root pointer.
    tt_node* root;  // Root pointer (NULL means empty tree).
} tt_tree;  // End of tt_tree.

/* 動態 int 陣列（給 traversal 回傳用）/ Dynamic int array for traversals. */  // Bilingual type purpose.
typedef struct tt_int_array {  // Own a heap-allocated int buffer plus its length.
    int* data;  // Heap buffer (NULL when len is 0).
    int len;  // Number of valid ints in data.
} tt_int_array;  // End of tt_int_array.

bool tt_tree_from_level_order(tt_tree* out_tree, const tt_optional_int* values, int n);  // Build tree from level-order array; returns false on allocation failure/invalid input.
void tt_tree_destroy(tt_tree* tree);  // Free all nodes in the tree and reset root to NULL.

int tt_tree_size(const tt_tree* tree);  // Return number of nodes (O(n)); used for allocation sizing.

void tt_int_array_destroy(tt_int_array* arr);  // Free traversal arrays allocated by this unit and reset to empty.

bool tt_preorder_recursive(const tt_tree* tree, tt_int_array* out);  // Preorder (root,left,right) via recursion; allocates out->data.
bool tt_preorder_iterative(const tt_tree* tree, tt_int_array* out);  // Preorder via stack; allocates out->data.
bool tt_inorder_recursive(const tt_tree* tree, tt_int_array* out);  // Inorder (left,root,right) via recursion; allocates out->data.
bool tt_inorder_iterative(const tt_tree* tree, tt_int_array* out);  // Inorder via stack; allocates out->data.
bool tt_postorder_recursive(const tt_tree* tree, tt_int_array* out);  // Postorder (left,right,root) via recursion; allocates out->data.
bool tt_postorder_iterative(const tt_tree* tree, tt_int_array* out);  // Postorder via stack+visited flag; allocates out->data.
bool tt_level_order(const tt_tree* tree, tt_int_array* out);  // Level-order (BFS) via queue; allocates out->data.

#endif  // TREE_TRAVERSAL_H  // End of header guard.

