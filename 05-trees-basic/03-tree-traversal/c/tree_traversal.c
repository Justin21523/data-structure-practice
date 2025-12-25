// 03 樹走訪示範（C）/ Tree traversal demo (C).  // Bilingual file header.

#include "tree_traversal.h"  // Include the public API for this unit.

#include <stdlib.h>  // Provide malloc/free/calloc for heap allocation.

static tt_node* tt_node_create(int value) {  // Allocate and initialize a new node.
    tt_node* node = (tt_node*)malloc(sizeof(tt_node));  // Allocate node on heap.
    if (node == NULL) {  // Handle allocation failure.
        return NULL;  // Signal failure to caller.
    }  // Close allocation check.
    node->value = value;  // Store node value.
    node->left = NULL;  // Initialize left child as missing.
    node->right = NULL;  // Initialize right child as missing.
    return node;  // Return allocated node pointer.
}  // Close tt_node_create().

static void destroy_subtree(tt_node* node) {  // Recursively free a subtree.
    if (node == NULL) {  // Empty subtree: nothing to free.
        return;  // Stop recursion.
    }  // Close base case.
    destroy_subtree(node->left);  // Free left subtree first.
    destroy_subtree(node->right);  // Free right subtree next.
    free(node);  // Free this node.
}  // Close destroy_subtree().

bool tt_tree_from_level_order(tt_tree* out_tree, const tt_optional_int* values, int n) {  // Build a tree from level-order representation with nulls.
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

    tt_node** nodes = (tt_node**)calloc((size_t)n, sizeof(tt_node*));  // Allocate node-pointer array initialized to NULLs.
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

        nodes[i] = tt_node_create(values[i].value);  // Allocate node for this value.
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
        tt_node* node = nodes[i];  // Read node at index i.
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
}  // Close tt_tree_from_level_order().

void tt_tree_destroy(tt_tree* tree) {  // Free all nodes in the tree.
    if (tree == NULL) {  // Allow NULL pointer for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    destroy_subtree(tree->root);  // Recursively free nodes reachable from root.
    tree->root = NULL;  // Reset root to represent an empty tree.
}  // Close tt_tree_destroy().

static int size_of(const tt_node* node) {  // Recursive helper that counts nodes in a subtree.
    if (node == NULL) {  // Empty subtree contributes 0 nodes.
        return 0;  // Return 0 for empty.
    }  // Close base case.
    return 1 + size_of(node->left) + size_of(node->right);  // Count root + left + right.
}  // Close size_of().

int tt_tree_size(const tt_tree* tree) {  // Return number of nodes in the tree.
    if (tree == NULL) {  // Treat NULL tree pointer as empty.
        return 0;  // Return 0 for missing tree.
    }  // Close validation.
    return size_of(tree->root);  // Count from root.
}  // Close tt_tree_size().

void tt_int_array_destroy(tt_int_array* arr) {  // Free traversal array contents.
    if (arr == NULL) {  // Allow NULL for convenience.
        return;  // Nothing to do.
    }  // Close validation.
    free(arr->data);  // Free heap buffer (safe when NULL).
    arr->data = NULL;  // Clear pointer to avoid dangling reference.
    arr->len = 0;  // Reset length.
}  // Close tt_int_array_destroy().

static bool alloc_out_array(const tt_tree* tree, tt_int_array* out) {  // Allocate out->data sized to tree size.
    if (out == NULL) {  // Validate output pointer.
        return false;  // Signal invalid input.
    }  // Close validation.
    out->data = NULL;  // Default to empty buffer.
    out->len = 0;  // Default to empty length.
    int n = tt_tree_size(tree);  // Compute number of nodes to allocate for.
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

static void preorder_fill(const tt_node* node, int* out, int* io_index) {  // Fill preorder traversal into out using io_index cursor.
    if (node == NULL) {  // Empty subtree contributes nothing.
        return;  // Stop recursion.
    }  // Close base case.
    out[*io_index] = node->value;  // Visit root first.
    *io_index += 1;  // Advance write cursor.
    preorder_fill(node->left, out, io_index);  // Visit left subtree.
    preorder_fill(node->right, out, io_index);  // Visit right subtree.
}  // Close preorder_fill().

bool tt_preorder_recursive(const tt_tree* tree, tt_int_array* out) {  // Preorder via recursion.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    int index = 0;  // Write cursor for traversal.
    if (tree != NULL) {  // Allow NULL tree pointer for convenience.
        preorder_fill(tree->root, out->data, &index);  // Fill traversal from root.
    }  // Close NULL-tree branch.
    return true;  // Report success.
}  // Close tt_preorder_recursive().

bool tt_preorder_iterative(const tt_tree* tree, tt_int_array* out) {  // Preorder via explicit stack.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    if (out->len == 0) {  // Empty tree => empty traversal.
        return true;  // Report success.
    }  // Close empty branch.
    if (tree == NULL || tree->root == NULL) {  // Defensive check (should match out->len==0, but keep safe).
        tt_int_array_destroy(out);  // Free allocated buffer because traversal cannot proceed.
        return false;  // Signal inconsistent input.
    }  // Close validation.

    int n = out->len;  // Number of nodes in the tree.
    tt_node** stack = (tt_node**)malloc((size_t)n * sizeof(tt_node*));  // Allocate stack storage (each node pushed once).
    if (stack == NULL) {  // Handle allocation failure.
        tt_int_array_destroy(out);  // Free output buffer to avoid leaks.
        return false;  // Signal failure.
    }  // Close allocation check.

    int top = 0;  // Stack size / next push index.
    int out_index = 0;  // Write cursor for traversal output.
    stack[top] = tree->root;  // Push root.
    top += 1;  // Advance stack top.

    while (top > 0) {  // Process until stack is empty.
        top -= 1;  // Decrement top to pop.
        tt_node* node = stack[top];  // Pop top node.
        out->data[out_index] = node->value;  // Visit node when popped (preorder).
        out_index += 1;  // Advance output cursor.
        if (node->right != NULL) {  // Push right child first so left is processed first.
            stack[top] = node->right;  // Push right child.
            top += 1;  // Advance stack top.
        }  // Close right push.
        if (node->left != NULL) {  // Push left child last.
            stack[top] = node->left;  // Push left child.
            top += 1;  // Advance stack top.
        }  // Close left push.
    }  // Close loop.

    free(stack);  // Free stack storage.
    return true;  // Report success.
}  // Close tt_preorder_iterative().

static void inorder_fill(const tt_node* node, int* out, int* io_index) {  // Fill inorder traversal into out using io_index cursor.
    if (node == NULL) {  // Empty subtree contributes nothing.
        return;  // Stop recursion.
    }  // Close base case.
    inorder_fill(node->left, out, io_index);  // Visit left subtree first.
    out[*io_index] = node->value;  // Visit root.
    *io_index += 1;  // Advance write cursor.
    inorder_fill(node->right, out, io_index);  // Visit right subtree.
}  // Close inorder_fill().

bool tt_inorder_recursive(const tt_tree* tree, tt_int_array* out) {  // Inorder via recursion.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    int index = 0;  // Write cursor for traversal.
    if (tree != NULL) {  // Allow NULL tree pointer for convenience.
        inorder_fill(tree->root, out->data, &index);  // Fill traversal from root.
    }  // Close NULL-tree branch.
    return true;  // Report success.
}  // Close tt_inorder_recursive().

bool tt_inorder_iterative(const tt_tree* tree, tt_int_array* out) {  // Inorder via explicit stack.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    if (out->len == 0) {  // Empty tree => empty traversal.
        return true;  // Report success.
    }  // Close empty branch.
    if (tree == NULL || tree->root == NULL) {  // Defensive check (should match out->len==0, but keep safe).
        tt_int_array_destroy(out);  // Free allocated buffer because traversal cannot proceed.
        return false;  // Signal inconsistent input.
    }  // Close validation.

    int n = out->len;  // Number of nodes in the tree.
    tt_node** stack = (tt_node**)malloc((size_t)n * sizeof(tt_node*));  // Allocate stack storage (path length <= n).
    if (stack == NULL) {  // Handle allocation failure.
        tt_int_array_destroy(out);  // Free output buffer to avoid leaks.
        return false;  // Signal failure.
    }  // Close allocation check.

    int top = 0;  // Stack size / next push index.
    int out_index = 0;  // Write cursor for traversal output.
    tt_node* node = tree->root;  // Start at root.

    while (node != NULL || top > 0) {  // Continue while there is a node to visit or stack is non-empty.
        while (node != NULL) {  // Walk left as far as possible.
            stack[top] = node;  // Push current node.
            top += 1;  // Advance stack top.
            node = node->left;  // Move to left child.
        }  // Close left-walk loop.
        top -= 1;  // Pop one node.
        node = stack[top];  // Read popped node.
        out->data[out_index] = node->value;  // Visit node (inorder).
        out_index += 1;  // Advance output cursor.
        node = node->right;  // Move to right subtree next.
    }  // Close main loop.

    free(stack);  // Free stack storage.
    return true;  // Report success.
}  // Close tt_inorder_iterative().

static void postorder_fill(const tt_node* node, int* out, int* io_index) {  // Fill postorder traversal into out using io_index cursor.
    if (node == NULL) {  // Empty subtree contributes nothing.
        return;  // Stop recursion.
    }  // Close base case.
    postorder_fill(node->left, out, io_index);  // Visit left subtree first.
    postorder_fill(node->right, out, io_index);  // Visit right subtree next.
    out[*io_index] = node->value;  // Visit root last.
    *io_index += 1;  // Advance write cursor.
}  // Close postorder_fill().

bool tt_postorder_recursive(const tt_tree* tree, tt_int_array* out) {  // Postorder via recursion.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    int index = 0;  // Write cursor for traversal.
    if (tree != NULL) {  // Allow NULL tree pointer for convenience.
        postorder_fill(tree->root, out->data, &index);  // Fill traversal from root.
    }  // Close NULL-tree branch.
    return true;  // Report success.
}  // Close tt_postorder_recursive().

typedef struct tt_frame {  // Stack frame for postorder iterative traversal.
    tt_node* node;  // Node pointer for this frame.
    bool visited;  // True means children were already processed.
} tt_frame;  // End of tt_frame.

bool tt_postorder_iterative(const tt_tree* tree, tt_int_array* out) {  // Postorder via stack + visited flag.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    if (out->len == 0) {  // Empty tree => empty traversal.
        return true;  // Report success.
    }  // Close empty branch.
    if (tree == NULL || tree->root == NULL) {  // Defensive check (should match out->len==0, but keep safe).
        tt_int_array_destroy(out);  // Free allocated buffer because traversal cannot proceed.
        return false;  // Signal inconsistent input.
    }  // Close validation.

    int n = out->len;  // Number of nodes in the tree.
    tt_frame* stack = (tt_frame*)malloc((size_t)(2 * n) * sizeof(tt_frame));  // Allocate stack storage (each node may appear twice).
    if (stack == NULL) {  // Handle allocation failure.
        tt_int_array_destroy(out);  // Free output buffer to avoid leaks.
        return false;  // Signal failure.
    }  // Close allocation check.

    int top = 0;  // Stack size / next push index.
    int out_index = 0;  // Write cursor for traversal output.
    stack[top] = (tt_frame){tree->root, false};  // Push root frame (not visited).
    top += 1;  // Advance stack top.

    while (top > 0) {  // Process until stack is empty.
        top -= 1;  // Pop one frame.
        tt_frame f = stack[top];  // Read popped frame.
        if (f.visited) {  // When visited==true, children were already processed.
            out->data[out_index] = f.node->value;  // Visit node last (postorder).
            out_index += 1;  // Advance output cursor.
            continue;  // Move to next frame.
        }  // Close visited branch.
        stack[top] = (tt_frame){f.node, true};  // Re-push node marked as visited (to output after children).
        top += 1;  // Advance stack top.
        if (f.node->right != NULL) {  // Push right child first so left is processed first.
            stack[top] = (tt_frame){f.node->right, false};  // Process right subtree.
            top += 1;  // Advance stack top.
        }  // Close right push.
        if (f.node->left != NULL) {  // Push left child last.
            stack[top] = (tt_frame){f.node->left, false};  // Process left subtree first.
            top += 1;  // Advance stack top.
        }  // Close left push.
    }  // Close loop.

    free(stack);  // Free stack storage.
    return true;  // Report success.
}  // Close tt_postorder_iterative().

bool tt_level_order(const tt_tree* tree, tt_int_array* out) {  // Level-order traversal (BFS) via queue.
    if (!alloc_out_array(tree, out)) {  // Allocate output buffer sized to tree size.
        return false;  // Signal allocation failure.
    }  // Close allocation branch.
    if (out->len == 0) {  // Empty tree => empty traversal.
        return true;  // Report success.
    }  // Close empty branch.
    if (tree == NULL || tree->root == NULL) {  // Defensive check (should match out->len==0, but keep safe).
        tt_int_array_destroy(out);  // Free allocated buffer because traversal cannot proceed.
        return false;  // Signal inconsistent input.
    }  // Close validation.

    int n = out->len;  // Number of nodes to visit.
    tt_node** queue = (tt_node**)malloc((size_t)n * sizeof(tt_node*));  // Allocate queue storage of size n.
    if (queue == NULL) {  // Handle allocation failure.
        tt_int_array_destroy(out);  // Free output buffer to avoid leaks.
        return false;  // Signal failure.
    }  // Close allocation check.

    int head = 0;  // Queue head index (logical dequeue position).
    int tail = 0;  // Queue tail index (next enqueue position).
    int out_index = 0;  // Write cursor for traversal output.

    queue[tail] = tree->root;  // Enqueue root node first.
    tail += 1;  // Advance tail after enqueue.

    while (head < tail) {  // Process queue until empty.
        tt_node* node = queue[head];  // Read next node to process.
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
}  // Close tt_level_order().

