/* 01 二元樹示範（C）/ Binary tree demo (C). */  // Bilingual header for this unit's public API.
#ifndef BINARY_TREE_H  // Header guard to prevent multiple inclusion.
#define BINARY_TREE_H  // Header guard definition.

#include <stdbool.h>  // Provide bool type for optional values and success/failure results.

/* 可選整數（用來表達 null）/ Optional int (to represent null). */  // Bilingual type purpose.
typedef struct bt_optional_int {  // Represent an "int or null" value for level-order input.
    bool is_present;  // True when value is present; false means "null".
    int value;  // The stored value (ignored when is_present is false).
} bt_optional_int;  // End of bt_optional_int.

#define BT_SOME(x) ((bt_optional_int){true, (x)})  // Construct a present optional value.
#define BT_NONE ((bt_optional_int){false, 0})  // Construct a missing optional value (null).

/* 二元樹節點 / Binary tree node. */  // Bilingual type purpose.
typedef struct bt_node {  // Node stores value and pointers to left/right children.
    int value;  // Node value.
    struct bt_node* left;  // Left child pointer (NULL means no child).
    struct bt_node* right;  // Right child pointer (NULL means no child).
} bt_node;  // End of bt_node.

/* 二元樹（只存 root）/ Binary tree wrapper. */  // Bilingual type purpose.
typedef struct binary_tree {  // Represent a binary tree by its root pointer.
    bt_node* root;  // Root pointer (NULL means empty tree).
} binary_tree;  // End of binary_tree.

/* 動態 int 陣列（給 traversal 回傳用）/ Dynamic int array for traversals. */  // Bilingual type purpose.
typedef struct bt_int_array {  // Own a heap-allocated int buffer plus its length.
    int* data;  // Heap buffer (NULL when len is 0).
    int len;  // Number of valid ints in data.
} bt_int_array;  // End of bt_int_array.

bool binary_tree_from_level_order(binary_tree* out_tree, const bt_optional_int* values, int n);  // Build tree from level-order array; returns false on allocation failure/invalid input.
void binary_tree_destroy(binary_tree* tree);  // Free all nodes in the tree and reset root to NULL.

int binary_tree_size(const binary_tree* tree);  // Return number of nodes (O(n)).
int binary_tree_height(const binary_tree* tree);  // Return height measured in edges (empty=-1, leaf=0).
int binary_tree_count_leaves(const binary_tree* tree);  // Return number of leaf nodes (O(n)).

void bt_int_array_destroy(bt_int_array* arr);  // Free traversal arrays allocated by this unit and reset to empty.

bool binary_tree_preorder(const binary_tree* tree, bt_int_array* out);  // Return preorder traversal (root,left,right) in out; allocates out->data.
bool binary_tree_inorder(const binary_tree* tree, bt_int_array* out);  // Return inorder traversal (left,root,right) in out; allocates out->data.
bool binary_tree_postorder(const binary_tree* tree, bt_int_array* out);  // Return postorder traversal (left,right,root) in out; allocates out->data.
bool binary_tree_level_order(const binary_tree* tree, bt_int_array* out);  // Return level-order traversal (BFS) in out; allocates out->data.

#endif  // BINARY_TREE_H  // End of header guard.

