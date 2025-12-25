// 01 AVL Tree 示範（C）/ AVL tree demo (C).  // Bilingual file header.

#include "avl.h"  // Include the public API for this unit.

#include <stdlib.h>  // Provide malloc/free for heap allocation.

static avl_node* avl_node_create(int key) {  // Allocate and initialize a new AVL node.
    avl_node* node = (avl_node*)malloc(sizeof(avl_node));  // Allocate node on heap.
    if (node == NULL) {  // Handle allocation failure.
        return NULL;  // Signal failure to caller.
    }  // Close allocation check.
    node->key = key;  // Store key.
    node->left = NULL;  // Initialize left child as missing.
    node->right = NULL;  // Initialize right child as missing.
    node->height = 0;  // Leaf height is 0 when height is measured in edges.
    return node;  // Return allocated node pointer.
}  // Close avl_node_create().

static void destroy_subtree(avl_node* node) {  // Recursively free a subtree.
    if (node == NULL) {  // Empty subtree: nothing to free.
        return;  // Stop recursion.
    }  // Close base case.
    destroy_subtree(node->left);  // Free left subtree first.
    destroy_subtree(node->right);  // Free right subtree next.
    free(node);  // Free this node.
}  // Close destroy_subtree().

void avl_init(avl_tree* tree) {  // Initialize an empty AVL tree.
    if (tree == NULL) {  // Allow NULL pointer for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    tree->root = NULL;  // Represent empty tree with NULL root.
}  // Close avl_init().

void avl_destroy(avl_tree* tree) {  // Free all nodes in the AVL tree.
    if (tree == NULL) {  // Allow NULL pointer for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    destroy_subtree(tree->root);  // Recursively free nodes reachable from root.
    tree->root = NULL;  // Reset root to represent an empty tree.
}  // Close avl_destroy().

bool avl_contains(const avl_tree* tree, int key) {  // Return true if key exists in the tree.
    const avl_node* node = (tree == NULL) ? NULL : tree->root;  // Start searching from root (or treat NULL tree as empty).
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
}  // Close avl_contains().

static int h(const avl_node* node) {  // Return cached height for node, or -1 for empty subtree.
    return (node == NULL) ? -1 : node->height;  // Apply repo-wide height convention.
}  // Close h().

static void update_height(avl_node* node) {  // Recompute node->height from children heights.
    int left_h = h(node->left);  // Read left subtree height.
    int right_h = h(node->right);  // Read right subtree height.
    node->height = 1 + (left_h > right_h ? left_h : right_h);  // Height in edges is 1 + max(child heights).
}  // Close update_height().

static int balance_factor(const avl_node* node) {  // Compute balance factor = height(left) - height(right).
    return h(node->left) - h(node->right);  // Positive means left-heavy; negative means right-heavy.
}  // Close balance_factor().

static avl_node* rotate_right(avl_node* y) {  // Right rotation to fix a left-heavy imbalance.
    avl_node* x = y->left;  // Take left child as pivot.
    avl_node* t2 = x->right;  // Keep pivot's right subtree (will become y's left subtree).

    x->right = y;  // Put y as the right child of x.
    y->left = t2;  // Attach T2 as y's left child.

    update_height(y);  // Update y first because its children changed.
    update_height(x);  // Update x after y because x now points to y.
    return x;  // Return new subtree root.
}  // Close rotate_right().

static avl_node* rotate_left(avl_node* x) {  // Left rotation to fix a right-heavy imbalance.
    avl_node* y = x->right;  // Take right child as pivot.
    avl_node* t2 = y->left;  // Keep pivot's left subtree (will become x's right subtree).

    y->left = x;  // Put x as the left child of y.
    x->right = t2;  // Attach T2 as x's right child.

    update_height(x);  // Update x first because its children changed.
    update_height(y);  // Update y after x because y now points to x.
    return y;  // Return new subtree root.
}  // Close rotate_left().

static avl_node* rebalance(avl_node* node) {  // Rebalance node if it violates AVL balance constraints.
    int balance = balance_factor(node);  // Compute current node's balance factor.

    if (balance > 1) {  // Left-heavy subtree; need either LL or LR rotation(s).
        if (balance_factor(node->left) >= 0) {  // LL case: left child is not right-heavy.
            return rotate_right(node);  // Single right rotation fixes LL.
        }  // Close LL branch.
        node->left = rotate_left(node->left);  // LR case step 1: rotate left on left child.
        return rotate_right(node);  // LR case step 2: rotate right on current node.
    }  // Close left-heavy branch.

    if (balance < -1) {  // Right-heavy subtree; need either RR or RL rotation(s).
        if (balance_factor(node->right) <= 0) {  // RR case: right child is not left-heavy.
            return rotate_left(node);  // Single left rotation fixes RR.
        }  // Close RR branch.
        node->right = rotate_right(node->right);  // RL case step 1: rotate right on right child.
        return rotate_left(node);  // RL case step 2: rotate left on current node.
    }  // Close right-heavy branch.

    return node;  // Already balanced; return node unchanged.
}  // Close rebalance().

typedef struct avl_insert_result {  // Return (newRoot, inserted) for recursive inserts.
    avl_node* new_root;  // The new subtree root after insertion.
    bool inserted;  // Whether a node was inserted.
} avl_insert_result;  // End of avl_insert_result.

static avl_insert_result insert_subtree(avl_node* node, int key) {  // Insert into subtree and rebalance.
    if (node == NULL) {  // Empty spot: create a new leaf node.
        avl_node* created = avl_node_create(key);  // Allocate a new node.
        if (created == NULL) {  // Handle allocation failure.
            return (avl_insert_result){NULL, false};  // Signal "not inserted" (tree shape unchanged).
        }  // Close allocation check.
        return (avl_insert_result){created, true};  // Return new node and inserted=true.
    }  // Close base case.

    bool inserted = false;  // Track insertion flag from recursion.
    if (key < node->key) {  // Insert into left subtree when key is smaller.
        avl_insert_result r = insert_subtree(node->left, key);  // Recurse into left child slot.
        node->left = r.new_root;  // Re-attach possibly-rotated left subtree.
        inserted = r.inserted;  // Propagate inserted flag.
    } else if (key > node->key) {  // Insert into right subtree when key is larger.
        avl_insert_result r = insert_subtree(node->right, key);  // Recurse into right child slot.
        node->right = r.new_root;  // Re-attach possibly-rotated right subtree.
        inserted = r.inserted;  // Propagate inserted flag.
    } else {  // key == node->key means duplicate key.
        return (avl_insert_result){node, false};  // Reject duplicates by policy (no changes).
    }  // Close compare branch.

    update_height(node);  // Update cached height after insertion below.
    node = rebalance(node);  // Rebalance on the way back up.
    return (avl_insert_result){node, inserted};  // Return possibly new root and insertion flag.
}  // Close insert_subtree().

bool avl_insert(avl_tree* tree, int key) {  // Insert key; return false on duplicate or allocation failure.
    if (tree == NULL) {  // Validate input pointer.
        return false;  // Signal failure.
    }  // Close validation.
    avl_insert_result r = insert_subtree(tree->root, key);  // Insert into root subtree.
    tree->root = r.new_root;  // Update root pointer (rotations may change root).
    return r.inserted;  // Return whether insertion happened.
}  // Close avl_insert().

static avl_node* min_node(avl_node* node) {  // Return the leftmost (minimum-key) node in a non-empty subtree.
    avl_node* cur = node;  // Start at given node.
    while (cur->left != NULL) {  // Keep going left until we cannot.
        cur = cur->left;  // Move to left child.
    }  // Close loop.
    return cur;  // Leftmost node has the minimum key.
}  // Close min_node().

typedef struct avl_delete_result {  // Return (newRoot, removed) for recursive deletes.
    avl_node* new_root;  // The new subtree root after deletion.
    bool removed;  // Whether a node was removed.
} avl_delete_result;  // End of avl_delete_result.

static avl_delete_result delete_subtree(avl_node* node, int key) {  // Delete from subtree and rebalance.
    if (node == NULL) {  // Key not found in an empty subtree.
        return (avl_delete_result){NULL, false};  // Nothing removed.
    }  // Close base case.

    bool removed = false;  // Track whether a node was removed in this subtree.
    if (key < node->key) {  // Delete from left subtree when key is smaller.
        avl_delete_result r = delete_subtree(node->left, key);  // Recurse into left child.
        node->left = r.new_root;  // Re-attach possibly-rotated left subtree.
        removed = r.removed;  // Propagate removed flag.
    } else if (key > node->key) {  // Delete from right subtree when key is larger.
        avl_delete_result r = delete_subtree(node->right, key);  // Recurse into right child.
        node->right = r.new_root;  // Re-attach possibly-rotated right subtree.
        removed = r.removed;  // Propagate removed flag.
    } else {  // key == node->key means we found the node to delete.
        removed = true;  // Mark that we are removing a node.

        if (node->left == NULL && node->right == NULL) {  // Case 1: leaf node (0 children).
            free(node);  // Free node memory.
            return (avl_delete_result){NULL, true};  // Remove leaf by returning NULL.
        }  // Close leaf case.
        if (node->left == NULL) {  // Case 2a: only right child exists.
            avl_node* child = node->right;  // Store right child pointer.
            free(node);  // Free node memory.
            return (avl_delete_result){child, true};  // Replace node by its right child.
        }  // Close right-only case.
        if (node->right == NULL) {  // Case 2b: only left child exists.
            avl_node* child = node->left;  // Store left child pointer.
            free(node);  // Free node memory.
            return (avl_delete_result){child, true};  // Replace node by its left child.
        }  // Close left-only case.

        // Case 3: two children => replace with inorder successor (min of right subtree).  // Explain 2-child strategy.
        avl_node* successor = min_node(node->right);  // Find successor node from right subtree.
        node->key = successor->key;  // Copy successor key into current node (keeps tree shape).
        avl_delete_result rr = delete_subtree(node->right, successor->key);  // Delete successor node from right subtree.
        node->right = rr.new_root;  // Re-attach right subtree after successor removal.
    }  // Close match branch.

    update_height(node);  // Update cached height after deletion below.
    node = rebalance(node);  // Rebalance on the way back up.
    return (avl_delete_result){node, removed};  // Return possibly new root and removed flag.
}  // Close delete_subtree().

bool avl_delete(avl_tree* tree, int key) {  // Delete key; return true if a node was removed.
    if (tree == NULL) {  // Validate input pointer.
        return false;  // Signal failure.
    }  // Close validation.
    avl_delete_result r = delete_subtree(tree->root, key);  // Delete from root subtree.
    tree->root = r.new_root;  // Update root pointer (rotations may change root).
    return r.removed;  // Return whether deletion happened.
}  // Close avl_delete().

bool avl_min(const avl_tree* tree, int* out_key) {  // Write minimum key; return false if empty.
    if (out_key == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (tree == NULL || tree->root == NULL) {  // Empty tree has no minimum.
        return false;  // Report empty.
    }  // Close empty case.
    *out_key = min_node(tree->root)->key;  // Write leftmost key.
    return true;  // Report success.
}  // Close avl_min().

bool avl_max(const avl_tree* tree, int* out_key) {  // Write maximum key; return false if empty.
    if (out_key == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (tree == NULL || tree->root == NULL) {  // Empty tree has no maximum.
        return false;  // Report empty.
    }  // Close empty case.
    const avl_node* cur = tree->root;  // Start at root.
    while (cur->right != NULL) {  // Keep going right until we cannot.
        cur = cur->right;  // Move to right child.
    }  // Close loop.
    *out_key = cur->key;  // Write rightmost key.
    return true;  // Report success.
}  // Close avl_max().

static int size_of(const avl_node* node) {  // Recursive helper that counts nodes in a subtree.
    if (node == NULL) {  // Empty subtree contributes 0 nodes.
        return 0;  // Return 0 for empty.
    }  // Close base case.
    return 1 + size_of(node->left) + size_of(node->right);  // Count root + left + right.
}  // Close size_of().

int avl_size(const avl_tree* tree) {  // Return number of nodes in the AVL tree.
    if (tree == NULL) {  // Treat NULL tree pointer as empty.
        return 0;  // Return 0 for missing tree.
    }  // Close validation.
    return size_of(tree->root);  // Count from root.
}  // Close avl_size().

int avl_height(const avl_tree* tree) {  // Return height measured in edges (empty=-1, leaf=0).
    if (tree == NULL || tree->root == NULL) {  // Treat NULL tree pointer as empty.
        return -1;  // Empty height definition.
    }  // Close empty case.
    return tree->root->height;  // Use cached root height (maintained by AVL operations).
}  // Close avl_height().

void avl_int_array_destroy(avl_int_array* arr) {  // Free traversal array contents.
    if (arr == NULL) {  // Allow NULL for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    free(arr->data);  // Free heap buffer (safe when NULL).
    arr->data = NULL;  // Clear pointer to avoid dangling reference.
    arr->len = 0;  // Reset length.
}  // Close avl_int_array_destroy().

static bool alloc_out_array(const avl_tree* tree, avl_int_array* out) {  // Allocate out->data sized to tree size.
    if (out == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    out->data = NULL;  // Default to empty buffer.
    out->len = 0;  // Default to empty length.
    int n = avl_size(tree);  // Compute number of nodes to allocate for.
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

static void inorder_fill(const avl_node* node, int* out, int* io_index) {  // Fill inorder traversal into out using io_index cursor.
    if (node == NULL) {  // Empty subtree contributes nothing.
        return;  // Stop recursion.
    }  // Close base case.
    inorder_fill(node->left, out, io_index);  // Visit left subtree first.
    out[*io_index] = node->key;  // Visit root.
    *io_index += 1;  // Advance write cursor.
    inorder_fill(node->right, out, io_index);  // Visit right subtree.
}  // Close inorder_fill().

bool avl_inorder(const avl_tree* tree, avl_int_array* out) {  // Return inorder traversal as an allocated int array.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    int index = 0;  // Write cursor for traversal.
    if (tree != NULL) {  // Allow NULL tree pointer for convenience.
        inorder_fill(tree->root, out->data, &index);  // Fill traversal from root.
    }  // Close NULL-tree branch.
    return true;  // Report success.
}  // Close avl_inorder().

static bool validate_subtree(const avl_node* node, bool has_low, int low, bool has_high, int high, int* out_height) {  // Validate subtree and output computed height.
    if (node == NULL) {  // Empty subtree is valid and has height -1 by definition.
        *out_height = -1;  // Write computed height for empty subtree.
        return true;  // Report valid.
    }  // Close base case.

    if (has_low && node->key <= low) {  // Enforce strict lower bound (no duplicates).
        return false;  // Report invalid ordering.
    }  // Close lower-bound check.
    if (has_high && node->key >= high) {  // Enforce strict upper bound (no duplicates).
        return false;  // Report invalid ordering.
    }  // Close upper-bound check.

    int left_h = 0;  // Store computed left subtree height.
    int right_h = 0;  // Store computed right subtree height.
    if (!validate_subtree(node->left, has_low, low, true, node->key, &left_h)) {  // Validate left subtree with updated upper bound.
        return false;  // Propagate failure.
    }  // Close left validation.
    if (!validate_subtree(node->right, true, node->key, has_high, high, &right_h)) {  // Validate right subtree with updated lower bound.
        return false;  // Propagate failure.
    }  // Close right validation.

    int computed = 1 + (left_h > right_h ? left_h : right_h);  // Compute expected height from children heights.
    if (node->height != computed) {  // Cached height must match computed height.
        return false;  // Report height mismatch.
    }  // Close height check.

    int diff = left_h - right_h;  // Compute balance factor in terms of computed heights.
    if (diff < 0) {  // Normalize to absolute value without relying on abs for int.
        diff = -diff;  // Take absolute value.
    }  // Close absolute branch.
    if (diff > 1) {  // AVL balance constraint requires |bf| <= 1.
        return false;  // Report imbalance.
    }  // Close balance check.

    *out_height = computed;  // Write computed height for this subtree.
    return true;  // Report valid subtree.
}  // Close validate_subtree().

bool avl_validate(const avl_tree* tree) {  // Validate BST ordering + AVL balance + cached height correctness.
    if (tree == NULL || tree->root == NULL) {  // Treat NULL/empty tree as valid.
        return true;  // Empty tree satisfies invariants trivially.
    }  // Close empty case.
    int computed = 0;  // Store computed height output.
    return validate_subtree(tree->root, false, 0, false, 0, &computed);  // Validate with open bounds (has_low/has_high false).
}  // Close avl_validate().

