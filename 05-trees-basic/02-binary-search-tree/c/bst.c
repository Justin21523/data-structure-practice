// 02 二元搜尋樹示範（C）/ Binary Search Tree (BST) demo (C).  // Bilingual file header.

#include "bst.h"  // Include the public API for this unit.

#include <stdlib.h>  // Provide malloc/free for heap allocation.

static bst_node* bst_node_create(int key) {  // Allocate and initialize a new BST node.
    bst_node* node = (bst_node*)malloc(sizeof(bst_node));  // Allocate node on heap.
    if (node == NULL) {  // Handle allocation failure.
        return NULL;  // Signal failure to caller.
    }  // Close allocation check.
    node->key = key;  // Store key.
    node->left = NULL;  // Initialize left child as missing.
    node->right = NULL;  // Initialize right child as missing.
    return node;  // Return allocated node pointer.
}  // Close bst_node_create().

static void destroy_subtree(bst_node* node) {  // Recursively free a subtree.
    if (node == NULL) {  // Empty subtree: nothing to free.
        return;  // Stop recursion.
    }  // Close base case.
    destroy_subtree(node->left);  // Free left subtree first.
    destroy_subtree(node->right);  // Free right subtree next.
    free(node);  // Free this node.
}  // Close destroy_subtree().

void bst_init(bst_tree* tree) {  // Initialize an empty BST.
    if (tree == NULL) {  // Allow NULL pointer for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    tree->root = NULL;  // Represent empty tree with NULL root.
}  // Close bst_init().

void bst_destroy(bst_tree* tree) {  // Free all nodes in the BST.
    if (tree == NULL) {  // Allow NULL pointer for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    destroy_subtree(tree->root);  // Recursively free nodes reachable from root.
    tree->root = NULL;  // Reset root to represent an empty tree.
}  // Close bst_destroy().

bool bst_contains(const bst_tree* tree, int key) {  // Return true if key exists in the BST.
    const bst_node* node = (tree == NULL) ? NULL : tree->root;  // Start searching from root (or treat NULL tree as empty).
    while (node != NULL) {  // Walk down until we hit a missing child.
        if (key < node->key) {  // Go left when key is smaller.
            node = node->left;  // Move to left child.
        } else if (key > node->key) {  // Go right when key is larger.
            node = node->right;  // Move to right child.
        } else {  // key == node->key means we found it.
            return true;  // Report found.
        }  // Close compare branch.
    }  // Close loop.
    return false;  // Report not found when search falls off the tree.
}  // Close bst_contains().

bool bst_insert(bst_tree* tree, int key) {  // Insert key; return false on duplicate or allocation failure.
    if (tree == NULL) {  // Validate input pointer.
        return false;  // Signal failure.
    }  // Close validation.

    if (tree->root == NULL) {  // Empty tree case: inserted node becomes the root.
        tree->root = bst_node_create(key);  // Allocate root node.
        return tree->root != NULL;  // Report allocation success.
    }  // Close empty case.

    bst_node* parent = NULL;  // Track parent so we can attach the new node.
    bst_node* node = tree->root;  // Start from root.
    while (node != NULL) {  // Walk down until we find an empty spot.
        parent = node;  // Update parent before moving.
        if (key < node->key) {  // Go left when key is smaller.
            node = node->left;  // Move to left child.
        } else if (key > node->key) {  // Go right when key is larger.
            node = node->right;  // Move to right child.
        } else {  // key == node->key means duplicate key.
            return false;  // Reject duplicates by policy.
        }  // Close compare branch.
    }  // Close loop.

    bst_node* new_node = bst_node_create(key);  // Create the new leaf node.
    if (new_node == NULL) {  // Handle allocation failure.
        return false;  // Signal failure.
    }  // Close allocation check.

    if (key < parent->key) {  // Attach to the left side when key is smaller.
        parent->left = new_node;  // Link as left child.
    } else {  // Otherwise attach to the right side.
        parent->right = new_node;  // Link as right child.
    }  // Close attach branch.
    return true;  // Report inserted.
}  // Close bst_insert().

typedef struct bst_delete_result {  // Return (newRoot, removed) for recursive deletes.
    bst_node* new_root;  // The new subtree root after deletion.
    bool removed;  // Whether a node was removed.
} bst_delete_result;  // End of bst_delete_result.

static int min_key(const bst_node* node) {  // Return the minimum key in a non-empty subtree.
    const bst_node* cur = node;  // Start at given node.
    while (cur->left != NULL) {  // Keep going left until we cannot.
        cur = cur->left;  // Move to left child.
    }  // Close loop.
    return cur->key;  // Leftmost node has the minimum key.
}  // Close min_key().

static bst_delete_result delete_subtree(bst_node* node, int key) {  // Recursive delete that returns (newRoot, removed).
    if (node == NULL) {  // Key not found in an empty subtree.
        return (bst_delete_result){NULL, false};  // Nothing removed.
    }  // Close base case.

    if (key < node->key) {  // Delete from left subtree when key is smaller.
        bst_delete_result r = delete_subtree(node->left, key);  // Recurse into left child.
        node->left = r.new_root;  // Update left child pointer.
        return (bst_delete_result){node, r.removed};  // Return updated subtree.
    }  // Close left branch.
    if (key > node->key) {  // Delete from right subtree when key is larger.
        bst_delete_result r = delete_subtree(node->right, key);  // Recurse into right child.
        node->right = r.new_root;  // Update right child pointer.
        return (bst_delete_result){node, r.removed};  // Return updated subtree.
    }  // Close right branch.

    // Now key == node->key, so we found the node to delete.  // Explain match branch.
    if (node->left == NULL && node->right == NULL) {  // Case 1: leaf node (0 children).
        free(node);  // Free the leaf node.
        return (bst_delete_result){NULL, true};  // Remove leaf by returning NULL.
    }  // Close leaf case.
    if (node->left == NULL) {  // Case 2a: only right child exists.
        bst_node* child = node->right;  // Capture right child pointer.
        free(node);  // Free current node.
        return (bst_delete_result){child, true};  // Replace node by its right child.
    }  // Close right-only case.
    if (node->right == NULL) {  // Case 2b: only left child exists.
        bst_node* child = node->left;  // Capture left child pointer.
        free(node);  // Free current node.
        return (bst_delete_result){child, true};  // Replace node by its left child.
    }  // Close left-only case.

    // Case 3: two children => replace with inorder successor (min of right subtree).  // Explain 2-child strategy.
    int successor_key = min_key(node->right);  // Find successor key from right subtree.
    node->key = successor_key;  // Copy successor key into current node.
    bst_delete_result r = delete_subtree(node->right, successor_key);  // Delete successor node from right subtree.
    node->right = r.new_root;  // Update right child pointer after successor removal.
    return (bst_delete_result){node, true};  // Report successful deletion.
}  // Close delete_subtree().

bool bst_delete(bst_tree* tree, int key) {  // Delete key; return true if a node was removed.
    if (tree == NULL) {  // Validate input pointer.
        return false;  // Signal failure.
    }  // Close validation.
    bst_delete_result r = delete_subtree(tree->root, key);  // Delete from root.
    tree->root = r.new_root;  // Update root pointer (may change when deleting root).
    return r.removed;  // Return whether deletion occurred.
}  // Close bst_delete().

bool bst_min(const bst_tree* tree, int* out_key) {  // Write minimum key; return false if empty.
    if (out_key == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (tree == NULL || tree->root == NULL) {  // Empty tree has no minimum.
        return false;  // Report empty.
    }  // Close empty case.
    *out_key = min_key(tree->root);  // Write leftmost key.
    return true;  // Report success.
}  // Close bst_min().

bool bst_max(const bst_tree* tree, int* out_key) {  // Write maximum key; return false if empty.
    if (out_key == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (tree == NULL || tree->root == NULL) {  // Empty tree has no maximum.
        return false;  // Report empty.
    }  // Close empty case.
    const bst_node* cur = tree->root;  // Start at root.
    while (cur->right != NULL) {  // Keep going right until we cannot.
        cur = cur->right;  // Move to right child.
    }  // Close loop.
    *out_key = cur->key;  // Write rightmost key.
    return true;  // Report success.
}  // Close bst_max().

static int size_of(const bst_node* node) {  // Recursive helper that counts nodes in a subtree.
    if (node == NULL) {  // Empty subtree contributes 0 nodes.
        return 0;  // Return 0 for empty.
    }  // Close base case.
    return 1 + size_of(node->left) + size_of(node->right);  // Count root + left + right.
}  // Close size_of().

int bst_size(const bst_tree* tree) {  // Return number of nodes in the BST.
    if (tree == NULL) {  // Treat NULL tree pointer as empty.
        return 0;  // Return 0 for missing tree.
    }  // Close validation.
    return size_of(tree->root);  // Count from root.
}  // Close bst_size().

static int height_of(const bst_node* node) {  // Recursive helper that computes height in edges.
    if (node == NULL) {  // Empty subtree has height -1 (so leaf has height 0).
        return -1;  // Return -1 for empty.
    }  // Close base case.
    int left_h = height_of(node->left);  // Compute left subtree height.
    int right_h = height_of(node->right);  // Compute right subtree height.
    return 1 + (left_h > right_h ? left_h : right_h);  // Height is 1 + max(left,right).
}  // Close height_of().

int bst_height(const bst_tree* tree) {  // Return height measured in edges (empty=-1, leaf=0).
    if (tree == NULL) {  // Treat NULL tree pointer as empty.
        return -1;  // Empty height definition.
    }  // Close validation.
    return height_of(tree->root);  // Compute from root.
}  // Close bst_height().

void bst_int_array_destroy(bst_int_array* arr) {  // Free traversal array contents.
    if (arr == NULL) {  // Allow NULL for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    free(arr->data);  // Free heap buffer (safe when NULL).
    arr->data = NULL;  // Clear pointer to avoid dangling reference.
    arr->len = 0;  // Reset length.
}  // Close bst_int_array_destroy().

static bool alloc_out_array(const bst_tree* tree, bst_int_array* out) {  // Allocate out->data sized to tree size.
    if (out == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    out->data = NULL;  // Default to empty buffer.
    out->len = 0;  // Default to empty length.
    int n = bst_size(tree);  // Compute number of nodes to allocate for.
    out->len = n;  // Store expected length.
    if (n == 0) {  // Empty tree needs no allocation.
        return true;  // Report success with empty array.
    }  // Close empty branch.
    out->data = (int*)malloc((size_t)n * sizeof(int));  // Allocate output buffer.
    if (out->data == NULL) {  // Handle allocation failure.
        out->len = 0;  // Reset to empty so caller can safely destroy.
        return false;  // Signal failure.
    }  // Close allocation check.
    return true;  // Report success.
}  // Close alloc_out_array().

static void inorder_fill(const bst_node* node, int* out, int* io_index) {  // Fill inorder traversal into out using io_index cursor.
    if (node == NULL) {  // Empty subtree contributes nothing.
        return;  // Stop recursion.
    }  // Close base case.
    inorder_fill(node->left, out, io_index);  // Visit left subtree first.
    out[*io_index] = node->key;  // Visit root.
    *io_index += 1;  // Advance write cursor.
    inorder_fill(node->right, out, io_index);  // Visit right subtree.
}  // Close inorder_fill().

bool bst_inorder(const bst_tree* tree, bst_int_array* out) {  // Return inorder traversal as an allocated int array.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    int index = 0;  // Write cursor for traversal.
    if (tree != NULL) {  // Allow NULL tree pointer for convenience.
        inorder_fill(tree->root, out->data, &index);  // Fill traversal from root.
    }  // Close NULL-tree branch.
    return true;  // Report success.
}  // Close bst_inorder().

