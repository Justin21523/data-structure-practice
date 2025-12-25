// 01 二元樹示範（C）/ Binary tree demo (C).  // Bilingual file header.

#include "binary_tree.h"  // Include the binary tree API for this unit.

#include <stdio.h>  // Provide printf for demo output.

static void print_level_order_input(const bt_optional_int* values, int n) {  // Print level-order input array with nulls.
    printf("level_order input: [");  // Print opening bracket.
    for (int i = 0; i < n; i++) {  // Print each entry.
        if (i > 0) {  // Print separator after the first entry.
            printf(", ");  // Print comma+space.
        }  // Close separator branch.
        if (values[i].is_present) {  // Print value when present.
            printf("%d", values[i].value);  // Print integer value.
        } else {  // Otherwise print null marker.
            printf("null");  // Print null for missing node.
        }  // Close present/null branch.
    }  // Close loop.
    printf("]\n");  // Print closing bracket and newline.
}  // Close print_level_order_input().

static void print_int_array(const char* label, const bt_int_array* arr) {  // Print an int array in [a, b, c] form.
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
    bt_optional_int values[] = {BT_SOME(1), BT_SOME(2), BT_SOME(3), BT_SOME(4), BT_SOME(5)};  // Sample tree: 1 with children 2 and 3; 2 has 4 and 5.
    int n = (int)(sizeof(values) / sizeof(values[0]));  // Compute array length.

    binary_tree t;  // Create tree struct.
    if (!binary_tree_from_level_order(&t, values, n)) {  // Build tree from level-order input.
        printf("Failed to build tree (allocation failure).\n");  // Print error message.
        return 1;  // Exit failure.
    }  // Close build check.

    printf("=== Binary Tree Demo (C) ===\n");  // Print header.
    print_level_order_input(values, n);  // Print input representation.

    int size = binary_tree_size(&t);  // Compute node count.
    int height = binary_tree_height(&t);  // Compute height in edges.
    int leaves = binary_tree_count_leaves(&t);  // Compute leaf count.
    printf("size=%d, height=%d, leaves=%d\n", size, height, leaves);  // Print key properties.

    bt_int_array pre;  // Store preorder traversal.
    bt_int_array in;  // Store inorder traversal.
    bt_int_array post;  // Store postorder traversal.
    bt_int_array level;  // Store level-order traversal.

    if (!binary_tree_preorder(&t, &pre) || !binary_tree_inorder(&t, &in) || !binary_tree_postorder(&t, &post) || !binary_tree_level_order(&t, &level)) {  // Build traversal arrays.
        printf("Failed to compute traversals (allocation failure).\n");  // Print error message.
        binary_tree_destroy(&t);  // Free tree nodes.
        return 1;  // Exit failure.
    }  // Close traversal allocation check.

    print_int_array("preorder", &pre);  // Print preorder traversal.
    print_int_array("inorder", &in);  // Print inorder traversal.
    print_int_array("postorder", &post);  // Print postorder traversal.
    print_int_array("level_order", &level);  // Print level-order traversal.

    bt_int_array_destroy(&pre);  // Free preorder buffer.
    bt_int_array_destroy(&in);  // Free inorder buffer.
    bt_int_array_destroy(&post);  // Free postorder buffer.
    bt_int_array_destroy(&level);  // Free level-order buffer.
    binary_tree_destroy(&t);  // Free all nodes in the tree.
    return 0;  // Exit success.
}  // Close main().

