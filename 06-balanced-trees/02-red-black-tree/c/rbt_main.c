// 02 Red-Black Tree 示範（C, LLRB）/ Red-black tree demo (C, LLRB).  // Bilingual file header.

#include "rbt.h"  // Include the RBT API for this unit.

#include <stdio.h>  // Provide printf for demo output.

static void print_int_array(const char* label, const rbt_int_array* arr) {  // Print an int array in [a, b, c] form.
    printf("%s=[", label);  // Print label and opening bracket.
    for (int i = 0; i < arr->len; i++) {  // Print each value.
        if (i > 0) {  // Print separator after the first entry.
            printf(", ");  // Print comma+space.
        }  // Close separator branch.
        printf("%d", arr->data[i]);  // Print one integer.
    }  // Close loop.
    printf("]\n");  // Print closing bracket and newline.
}  // Close print_int_array().

int main(void) {  // CLI entry point for the demo.
    int values[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic textbook insert order.
    int n = (int)(sizeof(values) / sizeof(values[0]));  // Compute array length.

    rbt_tree t;  // Create tree struct.
    rbt_init(&t);  // Initialize empty tree.
    for (int i = 0; i < n; i++) {  // Insert values in order.
        rbt_insert(&t, values[i]);  // Insert one key (duplicates are rejected by policy).
    }  // Close loop.

    printf("=== Red-Black Tree Demo (C, LLRB) ===\n");  // Print header.
    printf("insert order: [8, 3, 10, 1, 6, 14, 4, 7, 13]\n");  // Print insertion order (fixed for this demo).

    int size = rbt_size(&t);  // Compute node count.
    int height = rbt_height(&t);  // Compute height in edges.
    int min_key = 0;  // Store minimum key.
    int max_key = 0;  // Store maximum key.
    bool has_min = rbt_min(&t, &min_key);  // Compute minimum key.
    bool has_max = rbt_max(&t, &max_key);  // Compute maximum key.
    bool is_valid = rbt_validate(&t);  // Validate invariants.

    printf("size=%d, height=%d, ", size, height);  // Print size/height prefix.
    if (has_min) {  // Print minimum when present.
        printf("min=%d, ", min_key);  // Print minimum key.
    } else {  // Otherwise print null marker.
        printf("min=null, ");  // Print null for missing min.
    }  // Close min branch.
    if (has_max) {  // Print maximum when present.
        printf("max=%d, ", max_key);  // Print maximum key.
    } else {  // Otherwise print null marker.
        printf("max=null, ");  // Print null for missing max.
    }  // Close max branch.
    printf("valid=%s\n", is_valid ? "true" : "false");  // Print validation status.

    printf("contains(6)=%s, contains(99)=%s\n", rbt_contains(&t, 6) ? "true" : "false", rbt_contains(&t, 99) ? "true" : "false");  // Print search examples.

    rbt_int_array inorder;  // Store inorder traversal.
    if (!rbt_inorder(&t, &inorder)) {  // Compute inorder traversal.
        printf("Failed to compute inorder (allocation failure).\n");  // Print error message.
        rbt_destroy(&t);  // Free tree nodes.
        return 1;  // Exit failure.
    }  // Close allocation check.
    print_int_array("inorder(sorted)", &inorder);  // Print inorder traversal.
    rbt_int_array_destroy(&inorder);  // Free inorder buffer.

    rbt_delete(&t, 7);  // Delete a leaf (7).
    rbt_delete(&t, 14);  // Delete a one-child node in this data set.
    rbt_delete(&t, 3);  // Delete a two-child node (3) and rebalance.

    if (!rbt_inorder(&t, &inorder)) {  // Compute inorder after deletions.
        printf("Failed to compute inorder after deletions (allocation failure).\n");  // Print error message.
        rbt_destroy(&t);  // Free tree nodes.
        return 1;  // Exit failure.
    }  // Close allocation check.
    print_int_array("after deletes (7,14,3) inorder", &inorder);  // Print inorder after deletions.
    rbt_int_array_destroy(&inorder);  // Free inorder buffer.
    printf("valid=%s\n", rbt_validate(&t) ? "true" : "false");  // Print validation status after deletions.

    rbt_destroy(&t);  // Free all nodes in the tree.
    return 0;  // Exit success.
}  // Close main().

