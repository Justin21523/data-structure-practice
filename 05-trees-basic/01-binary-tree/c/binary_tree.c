// 01 二元樹示範（C）/ Binary tree demo (C).  // Bilingual file header.

#include "binary_tree.h"  // Include the public API for this unit.

#include <stdlib.h>  // Provide malloc/free/calloc for heap allocation.

static bt_node* bt_node_create(int value) {  // Allocate and initialize a new node.
    bt_node* node = (bt_node*)malloc(sizeof(bt_node));  // Allocate node on heap.
    if (node == NULL) {  // Handle allocation failure.
        return NULL;  // Signal failure to caller.
    }  // Close allocation check.
    node->value = value;  // Store node value.
    node->left = NULL;  // Initialize left child as missing.
    node->right = NULL;  // Initialize right child as missing.
    return node;  // Return allocated node pointer.
}  // Close bt_node_create().

static void destroy_subtree(bt_node* node) {  // Recursively free a subtree.
    if (node == NULL) {  // Empty subtree: nothing to free.
        return;  // Stop recursion.
    }  // Close base case.
    destroy_subtree(node->left);  // Free left subtree first.
    destroy_subtree(node->right);  // Free right subtree next.
    free(node);  // Free this node.
}  // Close destroy_subtree().

bool binary_tree_from_level_order(binary_tree* out_tree, const bt_optional_int* values, int n) {  // Build a tree from level-order representation with nulls.
    if (out_tree == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    out_tree->root = NULL;  // Default to empty tree on all paths.
    if (n < 0) {  // Reject negative length.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (n == 0) {  // Empty array builds an empty tree.
        return true;  // Report success with empty tree.
    }  // Close empty branch.
    if (values == NULL) {  // Non-empty input must provide a values pointer.
        return false;  // Signal invalid input.
    }  // Close validation.

    if (!values[0].is_present) {  // If the root is null, we define the tree as empty.
        return true;  // Return empty tree and ignore the rest to avoid unreachable allocations.
    }  // Close root-null branch.

    bt_node** nodes = (bt_node**)calloc((size_t)n, sizeof(bt_node*));  // Allocate node-pointer array initialized to NULLs.
    if (nodes == NULL) {  // Handle allocation failure.
        return false;  // Signal failure to caller.
    }  // Close allocation check.

    for (int i = 0; i < n; i++) {  // First pass: allocate only reachable nodes.
        if (!values[i].is_present) {  // Skip null entries.
            continue;  // Move to next index.
        }  // Close skip branch.
        if (i != 0) {  // For non-root nodes, ensure parent exists so node is reachable.
            int parent_index = (i - 1) / 2;  // Compute parent index in array representation.
            if (nodes[parent_index] == NULL) {  // Ignore nodes whose parent is missing (unreachable).
                continue;  // Do not allocate unreachable node.
            }  // Close reachability check.
        }  // Close non-root branch.

        nodes[i] = bt_node_create(values[i].value);  // Allocate node for this value.
        if (nodes[i] == NULL) {  // Handle allocation failure.
            for (int j = 0; j < n; j++) {  // Free all nodes allocated so far.
                free(nodes[j]);  // Free one node (safe when pointer is NULL).
                nodes[j] = NULL;  // Clear pointer for safety.
            }  // Close cleanup loop.
            free(nodes);  // Free the pointer array itself.
            return false;  // Signal failure to caller.
        }  // Close allocation failure branch.
    }  // Close allocation pass.

    for (int i = 0; i < n; i++) {  // Second pass: link children using indices.
        bt_node* node = nodes[i];  // Read node at index i.
        if (node == NULL) {  // Skip missing nodes.
            continue;  // Move to next index.
        }  // Close skip branch.
        int left_index = 2 * i + 1;  // Compute left child index.
        int right_index = 2 * i + 2;  // Compute right child index.
        if (left_index < n) {  // Link left child when index is in range.
            node->left = nodes[left_index];  // Assign left child (may be NULL).
        }  // Close left link.
        if (right_index < n) {  // Link right child when index is in range.
            node->right = nodes[right_index];  // Assign right child (may be NULL).
        }  // Close right link.
    }  // Close linking pass.

    out_tree->root = nodes[0];  // Store root pointer.
    free(nodes);  // Free temporary pointer array (nodes themselves are kept by the tree).
    return true;  // Report success.
}  // Close binary_tree_from_level_order().

void binary_tree_destroy(binary_tree* tree) {  // Free all nodes in the tree.
    if (tree == NULL) {  // Allow NULL pointer for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    destroy_subtree(tree->root);  // Recursively free nodes reachable from root.
    tree->root = NULL;  // Reset root to represent an empty tree.
}  // Close binary_tree_destroy().

static int size_of(const bt_node* node) {  // Recursive helper that counts nodes in a subtree.
    if (node == NULL) {  // Empty subtree contributes 0 nodes.
        return 0;  // Return 0 for empty.
    }  // Close base case.
    return 1 + size_of(node->left) + size_of(node->right);  // Count root + left + right.
}  // Close size_of().

int binary_tree_size(const binary_tree* tree) {  // Return number of nodes in the tree.
    if (tree == NULL) {  // Treat NULL tree pointer as empty.
        return 0;  // Return 0 for missing tree.
    }  // Close validation.
    return size_of(tree->root);  // Count from root.
}  // Close binary_tree_size().

static int height_of(const bt_node* node) {  // Recursive helper that computes height in edges.
    if (node == NULL) {  // Empty subtree has height -1 (so leaf has height 0).
        return -1;  // Return -1 for empty.
    }  // Close base case.
    int left_h = height_of(node->left);  // Compute left subtree height.
    int right_h = height_of(node->right);  // Compute right subtree height.
    return 1 + (left_h > right_h ? left_h : right_h);  // Height is 1 + max(left,right).
}  // Close height_of().

int binary_tree_height(const binary_tree* tree) {  // Return height measured in edges (empty=-1, leaf=0).
    if (tree == NULL) {  // Treat NULL tree pointer as empty.
        return -1;  // Empty height definition.
    }  // Close validation.
    return height_of(tree->root);  // Compute from root.
}  // Close binary_tree_height().

static int leaves_of(const bt_node* node) {  // Recursive helper that counts leaf nodes in a subtree.
    if (node == NULL) {  // Empty subtree contributes 0 leaves.
        return 0;  // Return 0 for empty.
    }  // Close base case.
    if (node->left == NULL && node->right == NULL) {  // Node with no children is a leaf.
        return 1;  // Count this leaf.
    }  // Close leaf case.
    return leaves_of(node->left) + leaves_of(node->right);  // Sum leaves from both subtrees.
}  // Close leaves_of().

int binary_tree_count_leaves(const binary_tree* tree) {  // Return number of leaf nodes.
    if (tree == NULL) {  // Treat NULL tree pointer as empty.
        return 0;  // Return 0 leaves.
    }  // Close validation.
    return leaves_of(tree->root);  // Count from root.
}  // Close binary_tree_count_leaves().

void bt_int_array_destroy(bt_int_array* arr) {  // Free traversal array contents.
    if (arr == NULL) {  // Allow NULL for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    free(arr->data);  // Free heap buffer (safe when NULL).
    arr->data = NULL;  // Clear pointer to avoid dangling reference.
    arr->len = 0;  // Reset length.
}  // Close bt_int_array_destroy().

static bool alloc_out_array(const binary_tree* tree, bt_int_array* out) {  // Allocate out->data sized to tree size.
    if (out == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    out->data = NULL;  // Default to empty buffer.
    out->len = 0;  // Default to empty length.
    int n = binary_tree_size(tree);  // Compute number of nodes to allocate for.
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

static void preorder_fill(const bt_node* node, int* out, int* io_index) {  // Fill preorder traversal into out using io_index cursor.
    if (node == NULL) {  // Empty subtree contributes nothing.
        return;  // Stop recursion.
    }  // Close base case.
    out[*io_index] = node->value;  // Visit root first.
    *io_index += 1;  // Advance write cursor.
    preorder_fill(node->left, out, io_index);  // Visit left subtree.
    preorder_fill(node->right, out, io_index);  // Visit right subtree.
}  // Close preorder_fill().

bool binary_tree_preorder(const binary_tree* tree, bt_int_array* out) {  // Return preorder traversal as an allocated int array.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    int index = 0;  // Write cursor for traversal.
    if (tree != NULL) {  // Allow NULL tree pointer for convenience.
        preorder_fill(tree->root, out->data, &index);  // Fill traversal from root.
    }  // Close NULL-tree branch.
    return true;  // Report success.
}  // Close binary_tree_preorder().

static void inorder_fill(const bt_node* node, int* out, int* io_index) {  // Fill inorder traversal into out using io_index cursor.
    if (node == NULL) {  // Empty subtree contributes nothing.
        return;  // Stop recursion.
    }  // Close base case.
    inorder_fill(node->left, out, io_index);  // Visit left subtree first.
    out[*io_index] = node->value;  // Visit root.
    *io_index += 1;  // Advance write cursor.
    inorder_fill(node->right, out, io_index);  // Visit right subtree.
}  // Close inorder_fill().

bool binary_tree_inorder(const binary_tree* tree, bt_int_array* out) {  // Return inorder traversal as an allocated int array.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    int index = 0;  // Write cursor for traversal.
    if (tree != NULL) {  // Allow NULL tree pointer for convenience.
        inorder_fill(tree->root, out->data, &index);  // Fill traversal from root.
    }  // Close NULL-tree branch.
    return true;  // Report success.
}  // Close binary_tree_inorder().

static void postorder_fill(const bt_node* node, int* out, int* io_index) {  // Fill postorder traversal into out using io_index cursor.
    if (node == NULL) {  // Empty subtree contributes nothing.
        return;  // Stop recursion.
    }  // Close base case.
    postorder_fill(node->left, out, io_index);  // Visit left subtree first.
    postorder_fill(node->right, out, io_index);  // Visit right subtree next.
    out[*io_index] = node->value;  // Visit root last.
    *io_index += 1;  // Advance write cursor.
}  // Close postorder_fill().

bool binary_tree_postorder(const binary_tree* tree, bt_int_array* out) {  // Return postorder traversal as an allocated int array.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    int index = 0;  // Write cursor for traversal.
    if (tree != NULL) {  // Allow NULL tree pointer for convenience.
        postorder_fill(tree->root, out->data, &index);  // Fill traversal from root.
    }  // Close NULL-tree branch.
    return true;  // Report success.
}  // Close binary_tree_postorder().

bool binary_tree_level_order(const binary_tree* tree, bt_int_array* out) {  // Return level-order traversal (BFS) as an allocated int array.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    if (out->len == 0) {  // Empty tree => empty traversal.
        return true;  // Report success.
    }  // Close empty branch.
    if (tree == NULL || tree->root == NULL) {  // Defensive check (should match out->len==0, but keep safe).
        bt_int_array_destroy(out);  // Free allocated buffer because traversal cannot proceed.
        return false;  // Signal inconsistent input.
    }  // Close validation.

    int n = out->len;  // Number of nodes to visit.
    const bt_node** queue = (const bt_node**)malloc((size_t)n * sizeof(const bt_node*));  // Allocate queue storage of size n.
    if (queue == NULL) {  // Handle allocation failure.
        bt_int_array_destroy(out);  // Free output buffer to avoid leaks.
        return false;  // Signal failure.
    }  // Close allocation check.

    int head = 0;  // Queue head index (logical dequeue position).
    int tail = 0;  // Queue tail index (next enqueue position).
    int out_index = 0;  // Write cursor for traversal output.

    queue[tail] = tree->root;  // Enqueue root node first.
    tail += 1;  // Advance tail after enqueue.

    while (head < tail) {  // Process queue until empty.
        const bt_node* node = queue[head];  // Read next node to process.
        head += 1;  // Advance head (logical dequeue).
        out->data[out_index] = node->value;  // Visit node in BFS order.
        out_index += 1;  // Advance output cursor.
        if (node->left != NULL) {  // Enqueue left child when present.
            queue[tail] = node->left;  // Append to queue tail.
            tail += 1;  // Advance tail.
        }  // Close left enqueue.
        if (node->right != NULL) {  // Enqueue right child when present.
            queue[tail] = node->right;  // Append to queue tail.
            tail += 1;  // Advance tail.
        }  // Close right enqueue.
    }  // Close BFS loop.

    free(queue);  // Free queue storage.
    return true;  // Report success.
}  // Close binary_tree_level_order().

