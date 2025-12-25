// 02 Red-Black Tree 示範（C, LLRB）/ Red-black tree demo (C, LLRB).  // Bilingual file header.

#include "rbt.h"  // Include the public API for this unit.

#include <stdlib.h>  // Provide malloc/free for heap allocation.

static const bool RED = true;  // Represent red nodes/links with true for readability.  
static const bool BLACK = false;  // Represent black nodes/links with false for readability.  

static rbt_node* rbt_node_create(int key, bool red) {  // Allocate and initialize a new node.
    rbt_node* node = (rbt_node*)malloc(sizeof(rbt_node));  // Allocate node on heap.
    if (node == NULL) {  // Handle allocation failure.
        return NULL;  // Signal failure to caller.
    }  // Close allocation check.
    node->key = key;  // Store key.
    node->left = NULL;  // Initialize left child as missing.
    node->right = NULL;  // Initialize right child as missing.
    node->red = red;  // Store node color.
    return node;  // Return allocated node pointer.
}  // Close rbt_node_create().

static void destroy_subtree(rbt_node* node) {  // Recursively free a subtree.
    if (node == NULL) {  // Empty subtree: nothing to free.
        return;  // Stop recursion.
    }  // Close base case.
    destroy_subtree(node->left);  // Free left subtree first.
    destroy_subtree(node->right);  // Free right subtree next.
    free(node);  // Free this node.
}  // Close destroy_subtree().

void rbt_init(rbt_tree* tree) {  // Initialize an empty tree.
    if (tree == NULL) {  // Allow NULL pointer for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    tree->root = NULL;  // Represent empty tree with NULL root.
}  // Close rbt_init().

void rbt_destroy(rbt_tree* tree) {  // Free all nodes in the tree.
    if (tree == NULL) {  // Allow NULL pointer for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    destroy_subtree(tree->root);  // Recursively free nodes reachable from root.
    tree->root = NULL;  // Reset root to represent an empty tree.
}  // Close rbt_destroy().

bool rbt_contains(const rbt_tree* tree, int key) {  // Return true if key exists in the tree.
    const rbt_node* node = (tree == NULL) ? NULL : tree->root;  // Start searching from root (or treat NULL tree as empty).
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
}  // Close rbt_contains().

static bool is_red(const rbt_node* node) {  // Return true when node is red; NULL is treated as black.
    return node != NULL && node->red == RED;  // Null links are black by definition.
}  // Close is_red().

static rbt_node* rotate_left(rbt_node* h) {  // Rotate left to eliminate a right-leaning red link.
    rbt_node* x = h->right;  // Take right child as pivot (must exist).
    h->right = x->left;  // Move pivot's left subtree to h's right slot.
    x->left = h;  // Put h as pivot's left child.
    x->red = h->red;  // Preserve original color on the new root.
    h->red = RED;  // Mark the moved-down node as red.
    return x;  // Return new subtree root.
}  // Close rotate_left().

static rbt_node* rotate_right(rbt_node* h) {  // Rotate right to fix two consecutive left-leaning red links.
    rbt_node* x = h->left;  // Take left child as pivot (must exist).
    h->left = x->right;  // Move pivot's right subtree to h's left slot.
    x->right = h;  // Put h as pivot's right child.
    x->red = h->red;  // Preserve original color on the new root.
    h->red = RED;  // Mark the moved-down node as red.
    return x;  // Return new subtree root.
}  // Close rotate_right().

static void flip_colors(rbt_node* h) {  // Flip colors to split or merge 4-nodes (LLRB <-> 2-3 tree).
    h->red = !h->red;  // Toggle parent color.
    if (h->left != NULL) {  // Guard for safety (children should exist in normal LLRB usage).
        h->left->red = !h->left->red;  // Toggle left child color.
    }  // Close left guard.
    if (h->right != NULL) {  // Guard for safety (children should exist in normal LLRB usage).
        h->right->red = !h->right->red;  // Toggle right child color.
    }  // Close right guard.
}  // Close flip_colors().

static rbt_node* fix_up(rbt_node* h) {  // Restore LLRB invariants on the way back up after insert/delete.
    if (is_red(h->right) && !is_red(h->left)) {  // Fix right-leaning red link.
        h = rotate_left(h);  // Rotate left to make red link lean left.
    }  // Close right-red fix.
    if (is_red(h->left) && is_red(h->left->left)) {  // Fix two consecutive left reds.
        h = rotate_right(h);  // Rotate right to balance the 4-node shape.
    }  // Close left-left fix.
    if (is_red(h->left) && is_red(h->right)) {  // Split 4-node when both children are red.
        flip_colors(h);  // Flip colors to push red up one level.
    }  // Close split case.
    return h;  // Return possibly rotated/flipped subtree root.
}  // Close fix_up().

static rbt_node* move_red_left(rbt_node* h) {  // Ensure h->left is red when we need to delete from the left side.
    flip_colors(h);  // Push red down so we can borrow from the right side.
    if (h->right != NULL && is_red(h->right->left)) {  // Borrow from right-left red link when available.
        h->right = rotate_right(h->right);  // Rotate right on right child to create a right-leaning red link.
        h = rotate_left(h);  // Rotate left on h to move red link to the left side.
        flip_colors(h);  // Re-flip to restore colors after rotations.
    }  // Close borrow case.
    return h;  // Return adjusted subtree root.
}  // Close move_red_left().

static rbt_node* move_red_right(rbt_node* h) {  // Ensure h->right is red when we need to delete from the right side.
    flip_colors(h);  // Push red down so we can borrow from the left side.
    if (h->left != NULL && is_red(h->left->left)) {  // Borrow from left-left red link when available.
        h = rotate_right(h);  // Rotate right to move red link to the right side.
        flip_colors(h);  // Re-flip to restore colors after rotation.
    }  // Close borrow case.
    return h;  // Return adjusted subtree root.
}  // Close move_red_right().

typedef struct rbt_insert_result {  // Return (newRoot, inserted) for recursive inserts.
    rbt_node* new_root;  // The new subtree root after insertion.
    bool inserted;  // Whether a node was inserted.
} rbt_insert_result;  // End of rbt_insert_result.

static rbt_insert_result insert_subtree(rbt_node* h, int key) {  // Insert key into subtree and rebalance.
    if (h == NULL) {  // Empty spot: create a new red leaf node.
        rbt_node* created = rbt_node_create(key, RED);  // Allocate a new node.
        if (created == NULL) {  // Handle allocation failure.
            return (rbt_insert_result){NULL, false};  // Signal "not inserted" (tree shape unchanged).
        }  // Close allocation check.
        return (rbt_insert_result){created, true};  // Return new node and inserted=true.
    }  // Close base case.

    bool inserted = false;  // Track insertion flag from recursion.
    if (key < h->key) {  // Insert into left subtree when key is smaller.
        rbt_insert_result r = insert_subtree(h->left, key);  // Recurse into left child slot.
        h->left = r.new_root;  // Re-attach possibly rebalanced left subtree.
        inserted = r.inserted;  // Propagate inserted flag.
    } else if (key > h->key) {  // Insert into right subtree when key is larger.
        rbt_insert_result r = insert_subtree(h->right, key);  // Recurse into right child slot.
        h->right = r.new_root;  // Re-attach possibly rebalanced right subtree.
        inserted = r.inserted;  // Propagate inserted flag.
    } else {  // key == h->key means duplicate key.
        return (rbt_insert_result){h, false};  // Reject duplicates by policy (no changes).
    }  // Close compare branch.

    h = fix_up(h);  // Restore invariants after insertion below.
    return (rbt_insert_result){h, inserted};  // Return possibly rebalanced subtree root and insertion flag.
}  // Close insert_subtree().

bool rbt_insert(rbt_tree* tree, int key) {  // Insert key; return false on duplicate or allocation failure.
    if (tree == NULL) {  // Validate input pointer.
        return false;  // Signal failure.
    }  // Close validation.
    rbt_insert_result r = insert_subtree(tree->root, key);  // Insert into root subtree.
    tree->root = r.new_root;  // Update root pointer.
    if (tree->root != NULL) {  // Guard for empty tree.
        tree->root->red = BLACK;  // Root must always be black.
    }  // Close root-color guard.
    return r.inserted;  // Return whether insertion happened.
}  // Close rbt_insert().

static rbt_node* min_node(rbt_node* node) {  // Return the leftmost (minimum-key) node in a non-empty subtree.
    rbt_node* cur = node;  // Start at given node.
    while (cur->left != NULL) {  // Keep going left until we cannot.
        cur = cur->left;  // Move to left child.
    }  // Close loop.
    return cur;  // Leftmost node has the minimum key.
}  // Close min_node().

static rbt_node* delete_min(rbt_node* h) {  // Delete the minimum node in this subtree and return new subtree root.
    if (h->left == NULL) {  // Reached the minimum node (no left child).
        free(h);  // Free the minimum node.
        return NULL;  // Delete this node by returning NULL.
    }  // Close base case.

    if (!is_red(h->left) && !is_red(h->left->left)) {  // Ensure we do not descend into a 2-node.
        h = move_red_left(h);  // Move a red link to the left side.
    }  // Close move case.

    h->left = delete_min(h->left);  // Recurse into left subtree.
    return fix_up(h);  // Fix up invariants on the way back up.
}  // Close delete_min().

static rbt_node* delete_subtree(rbt_node* h, int key) {  // Delete key from subtree (assumes key exists).
    if (key < h->key) {  // Key is in the left subtree.
        if (h->left != NULL) {  // Only recurse when left subtree exists.
            if (!is_red(h->left) && !is_red(h->left->left)) {  // Avoid descending into a 2-node.
                h = move_red_left(h);  // Move a red link to the left side.
            }  // Close move case.
            h->left = delete_subtree(h->left, key);  // Recurse into left subtree.
        }  // Close left guard.
    } else {  // Key is in the current node or right subtree.
        if (is_red(h->left)) {  // Rotate right to maintain delete preconditions.
            h = rotate_right(h);  // Rotate right to make sure we can move red right when needed.
        }  // Close rotate-right pre-step.

        if (key == h->key && h->right == NULL) {  // Case: match and no right child => can delete this node.
            free(h);  // Free this node.
            return NULL;  // Delete node by returning NULL.
        }  // Close leaf-right-null case.

        if (h->right != NULL) {  // Only work on right side when it exists.
            if (!is_red(h->right) && !is_red(h->right->left)) {  // Avoid descending into a 2-node.
                h = move_red_right(h);  // Move a red link to the right side.
            }  // Close move case.
            if (key == h->key) {  // Match case with a right subtree => replace with successor.
                rbt_node* successor = min_node(h->right);  // Find successor node from right subtree.
                h->key = successor->key;  // Copy successor key into current node.
                h->right = delete_min(h->right);  // Delete successor node from right subtree.
            } else {  // Otherwise delete in the right subtree.
                h->right = delete_subtree(h->right, key);  // Recurse into right subtree.
            }  // Close match/descend branch.
        }  // Close right guard.
    }  // Close key-side branch.

    return fix_up(h);  // Fix up invariants on the way back up.
}  // Close delete_subtree().

bool rbt_delete(rbt_tree* tree, int key) {  // Delete key; return true when a node was removed.
    if (tree == NULL || tree->root == NULL) {  // Empty tree cannot delete anything.
        return false;  // Report not removed.
    }  // Close empty case.
    if (!rbt_contains(tree, key)) {  // Avoid running LLRB delete when key is missing.
        return false;  // Report not removed.
    }  // Close missing-key guard.

    if (!is_red(tree->root->left) && !is_red(tree->root->right)) {  // Detect 2-node root situation.
        tree->root->red = RED;  // Temporarily color root red to simplify deletion logic.
    }  // Close pre-step.

    tree->root = delete_subtree(tree->root, key);  // Delete key from root subtree (guaranteed to exist).
    if (tree->root != NULL) {  // Restore invariant when tree is non-empty.
        tree->root->red = BLACK;  // Root must be black.
    }  // Close root-color guard.
    return true;  // Report removed.
}  // Close rbt_delete().

bool rbt_min(const rbt_tree* tree, int* out_key) {  // Write minimum key; return false if empty.
    if (out_key == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (tree == NULL || tree->root == NULL) {  // Empty tree has no minimum.
        return false;  // Report empty.
    }  // Close empty case.
    *out_key = min_node(tree->root)->key;  // Write leftmost key.
    return true;  // Report success.
}  // Close rbt_min().

bool rbt_max(const rbt_tree* tree, int* out_key) {  // Write maximum key; return false if empty.
    if (out_key == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (tree == NULL || tree->root == NULL) {  // Empty tree has no maximum.
        return false;  // Report empty.
    }  // Close empty case.
    const rbt_node* cur = tree->root;  // Start at root.
    while (cur->right != NULL) {  // Keep going right until we cannot.
        cur = cur->right;  // Move to right child.
    }  // Close loop.
    *out_key = cur->key;  // Write rightmost key.
    return true;  // Report success.
}  // Close rbt_max().

static int size_of(const rbt_node* node) {  // Recursive helper that counts nodes in a subtree.
    if (node == NULL) {  // Empty subtree contributes 0 nodes.
        return 0;  // Return 0 for empty.
    }  // Close base case.
    return 1 + size_of(node->left) + size_of(node->right);  // Count root + left + right.
}  // Close size_of().

int rbt_size(const rbt_tree* tree) {  // Return number of nodes in the tree.
    if (tree == NULL) {  // Treat NULL tree pointer as empty.
        return 0;  // Return 0 for missing tree.
    }  // Close validation.
    return size_of(tree->root);  // Count from root.
}  // Close rbt_size().

static int height_of(const rbt_node* node) {  // Recursive helper that computes height in edges.
    if (node == NULL) {  // Empty subtree has height -1.
        return -1;  // Return -1 for empty.
    }  // Close base case.
    int left_h = height_of(node->left);  // Compute left subtree height.
    int right_h = height_of(node->right);  // Compute right subtree height.
    return 1 + (left_h > right_h ? left_h : right_h);  // Height is 1 + max(left,right).
}  // Close height_of().

int rbt_height(const rbt_tree* tree) {  // Return height measured in edges (empty=-1, leaf=0).
    if (tree == NULL) {  // Treat NULL tree pointer as empty.
        return -1;  // Empty height definition.
    }  // Close validation.
    return height_of(tree->root);  // Compute from root.
}  // Close rbt_height().

void rbt_int_array_destroy(rbt_int_array* arr) {  // Free traversal array contents.
    if (arr == NULL) {  // Allow NULL for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    free(arr->data);  // Free heap buffer (safe when NULL).
    arr->data = NULL;  // Clear pointer to avoid dangling reference.
    arr->len = 0;  // Reset length.
}  // Close rbt_int_array_destroy().

static bool alloc_out_array(const rbt_tree* tree, rbt_int_array* out) {  // Allocate out->data sized to tree size.
    if (out == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    out->data = NULL;  // Default to empty buffer.
    out->len = 0;  // Default to empty length.
    int n = rbt_size(tree);  // Compute number of nodes to allocate for.
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

static void inorder_fill(const rbt_node* node, int* out, int* io_index) {  // Fill inorder traversal into out using io_index cursor.
    if (node == NULL) {  // Empty subtree contributes nothing.
        return;  // Stop recursion.
    }  // Close base case.
    inorder_fill(node->left, out, io_index);  // Visit left subtree first.
    out[*io_index] = node->key;  // Visit root.
    *io_index += 1;  // Advance write cursor.
    inorder_fill(node->right, out, io_index);  // Visit right subtree.
}  // Close inorder_fill().

bool rbt_inorder(const rbt_tree* tree, rbt_int_array* out) {  // Return inorder traversal as an allocated int array.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    int index = 0;  // Write cursor for traversal.
    if (tree != NULL) {  // Allow NULL tree pointer for convenience.
        inorder_fill(tree->root, out->data, &index);  // Fill traversal from root.
    }  // Close NULL-tree branch.
    return true;  // Report success.
}  // Close rbt_inorder().

static bool validate_subtree(const rbt_node* node, bool has_low, int low, bool has_high, int high, int* out_black_height) {  // Validate subtree and output black-height.
    if (node == NULL) {  // Null links are black and contribute a base black-height of 1.
        *out_black_height = 1;  // Treat NULL as a black leaf sentinel.
        return true;  // Report valid.
    }  // Close base case.

    if (has_low && node->key <= low) {  // Enforce strict lower bound (no duplicates).
        return false;  // Report invalid ordering.
    }  // Close lower-bound check.
    if (has_high && node->key >= high) {  // Enforce strict upper bound (no duplicates).
        return false;  // Report invalid ordering.
    }  // Close upper-bound check.

    if (is_red(node->right)) {  // LLRB rule: red links must lean left (no red right child).
        return false;  // Report invalid right-leaning red link.
    }  // Close right-red check.

    if (is_red(node) && (is_red(node->left) || is_red(node->right))) {  // RB rule: no two consecutive reds.
        return false;  // Report invalid consecutive red links.
    }  // Close consecutive-red check.

    int left_bh = 0;  // Store computed left subtree black-height.
    int right_bh = 0;  // Store computed right subtree black-height.
    if (!validate_subtree(node->left, has_low, low, true, node->key, &left_bh)) {  // Validate left subtree with updated upper bound.
        return false;  // Propagate failure.
    }  // Close left validation.
    if (!validate_subtree(node->right, true, node->key, has_high, high, &right_bh)) {  // Validate right subtree with updated lower bound.
        return false;  // Propagate failure.
    }  // Close right validation.

    if (left_bh != right_bh) {  // RB rule: all root-to-leaf paths have same number of black nodes.
        return false;  // Report black-height mismatch.
    }  // Close black-height check.

    int black_here = node->red ? 0 : 1;  // Count black nodes; red nodes do not add to black-height.
    *out_black_height = left_bh + black_here;  // Write computed black-height for this subtree.
    return true;  // Report valid subtree.
}  // Close validate_subtree().

bool rbt_validate(const rbt_tree* tree) {  // Validate BST ordering + red-black properties (LLRB variant).
    if (tree == NULL || tree->root == NULL) {  // Treat NULL/empty tree as valid.
        return true;  // Empty tree satisfies invariants trivially.
    }  // Close empty case.
    if (tree->root->red) {  // Root must be black.
        return false;  // Report invalid root color.
    }  // Close root-color check.
    int black_height = 0;  // Store black-height output.
    return validate_subtree(tree->root, false, 0, false, 0, &black_height);  // Validate with open bounds.
}  // Close rbt_validate().

