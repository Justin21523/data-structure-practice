// 03 樹走訪示範（C）/ Tree traversal demo (C).  // Bilingual file header.

#include "tree_traversal.h"  // Include the traversal API for this unit.

#include <stdio.h>  // Provide printf for demo output.

static void print_int_array(const char* label, const tt_int_array* arr) {  // Print an int array in [a, b, c] form.
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
    tt_optional_int values[] = {TT_SOME(1), TT_SOME(2), TT_SOME(3), TT_SOME(4), TT_SOME(5)};  // Sample tree used across this repo for traversal examples.
    int n = (int)(sizeof(values) / sizeof(values[0]));  // Compute array length.

    tt_tree t;  // Create tree struct.
    if (!tt_tree_from_level_order(&t, values, n)) {  // Build tree from level-order input.
        printf("Failed to build tree (allocation failure).\n");  // Print error message.
        return 1;  // Exit failure.
    }  // Close build check.

    printf("=== Tree Traversal Demo (C) ===\n");  // Print header.
    printf("level_order input: [1, 2, 3, 4, 5]\n");  // Print input (fixed for this demo).

    tt_int_array a;  // Reusable output array for traversals.

    tt_preorder_recursive(&t, &a);  // Compute recursive preorder.
    print_int_array("preorder_recursive", &a);  // Print recursive preorder.
    tt_int_array_destroy(&a);  // Free buffer.

    tt_preorder_iterative(&t, &a);  // Compute iterative preorder.
    print_int_array("preorder_iterative", &a);  // Print iterative preorder.
    tt_int_array_destroy(&a);  // Free buffer.

    tt_inorder_recursive(&t, &a);  // Compute recursive inorder.
    print_int_array("inorder_recursive", &a);  // Print recursive inorder.
    tt_int_array_destroy(&a);  // Free buffer.

    tt_inorder_iterative(&t, &a);  // Compute iterative inorder.
    print_int_array("inorder_iterative", &a);  // Print iterative inorder.
    tt_int_array_destroy(&a);  // Free buffer.

    tt_postorder_recursive(&t, &a);  // Compute recursive postorder.
    print_int_array("postorder_recursive", &a);  // Print recursive postorder.
    tt_int_array_destroy(&a);  // Free buffer.

    tt_postorder_iterative(&t, &a);  // Compute iterative postorder.
    print_int_array("postorder_iterative", &a);  // Print iterative postorder.
    tt_int_array_destroy(&a);  // Free buffer.

    tt_level_order(&t, &a);  // Compute level-order BFS.
    print_int_array("level_order", &a);  // Print level-order BFS.
    tt_int_array_destroy(&a);  // Free buffer.

    tt_tree_destroy(&t);  // Free tree nodes.
    return 0;  // Exit success.
}  // Close main().

