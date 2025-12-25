// 02 二元搜尋樹示範（C）/ Binary Search Tree (BST) demo (C).  // Bilingual file header.

#include "bst.h"  // Include the BST API for this unit.

#include <stdio.h>  // Provide printf for demo output.

static void print_int_array(const char* label, const bst_int_array* arr) {  // Print an int array in [a, b, c] form.
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
    int values[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic BST insert order used in many textbooks.
    int n = (int)(sizeof(values) / sizeof(values[0]));  // Compute array length.

    bst_tree t;  // Create BST struct.
    bst_init(&t);  // Initialize empty BST.
    for (int i = 0; i < n; i++) {  // Insert values in order.
        bst_insert(&t, values[i]);  // Insert one key (duplicates are ignored by policy).
    }  // Close loop.

    printf("=== Binary Search Tree Demo (C) ===\n");  // Print header.
    printf("insert order: [8, 3, 10, 1, 6, 14, 4, 7, 13]\n");  // Print insertion order (fixed for this demo).

    int size = bst_size(&t);  // Compute node count.
    int height = bst_height(&t);  // Compute height in edges.
    int min_key = 0;  // Store minimum key.
    int max_key = 0;  // Store maximum key.
    bool has_min = bst_min(&t, &min_key);  // Compute minimum key.
    bool has_max = bst_max(&t, &max_key);  // Compute maximum key.
    printf("size=%d, height=%d, ", size, height);  // Print size/height prefix.
    if (has_min) {  // Print minimum when present.
        printf("min=%d, ", min_key);  // Print minimum key.
    } else {  // Otherwise print null marker.
        printf("min=null, ");  // Print null for missing min.
    }  // Close min branch.
    if (has_max) {  // Print maximum when present.
        printf("max=%d\n", max_key);  // Print maximum key.
    } else {  // Otherwise print null marker.
        printf("max=null\n");  // Print null for missing max.
    }  // Close max branch.
    printf("contains(6)=%s, contains(99)=%s\n", bst_contains(&t, 6) ? "true" : "false", bst_contains(&t, 99) ? "true" : "false");  // Print search examples.

    bst_int_array inorder;  // Store inorder traversal.
    if (!bst_inorder(&t, &inorder)) {  // Compute inorder traversal.
        printf("Failed to compute inorder (allocation failure).\n");  // Print error message.
        bst_destroy(&t);  // Free tree nodes.
        return 1;  // Exit failure.
    }  // Close allocation check.
    print_int_array("inorder(sorted)", &inorder);  // Print inorder traversal.
    bst_int_array_destroy(&inorder);  // Free inorder buffer.

    bst_delete(&t, 7);  // Delete a leaf (7).
    bst_delete(&t, 14);  // Delete a one-child node (14 has left child 13).
    bst_delete(&t, 3);  // Delete a two-child node (3) using successor replacement.
    if (!bst_inorder(&t, &inorder)) {  // Compute inorder after deletions.
        printf("Failed to compute inorder after deletions (allocation failure).\n");  // Print error message.
        bst_destroy(&t);  // Free tree nodes.
        return 1;  // Exit failure.
    }  // Close allocation check.
    print_int_array("after deletes (7,14,3) inorder", &inorder);  // Print inorder after deletions.
    bst_int_array_destroy(&inorder);  // Free inorder buffer.

    bst_destroy(&t);  // Free all nodes in the BST.
    return 0;  // Exit success.
}  // Close main().
