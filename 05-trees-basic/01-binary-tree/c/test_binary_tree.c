// 01 二元樹測試（C）/ Tests for binary tree (C).  // Bilingual file header.

#include "binary_tree.h"  // Include the API under test.

#include <stdio.h>  // Provide printf for status output.
#include <stdlib.h>  // Provide exit for early termination on failure.

static void assert_true(bool condition, const char* message) {  // Minimal assertion helper.
    if (!condition) {  // Fail when condition is false.
        printf("FAIL: %s\n", message);  // Print failure message.
        exit(1);  // Exit with failure.
    }  // Close failure branch.
    printf("PASS: %s\n", message);  // Print pass status.
}  // Close assert_true().

static void assert_int_equals(int expected, int actual, const char* message) {  // Minimal int equality assertion helper.
    if (expected != actual) {  // Fail when values differ.
        printf("FAIL: %s (expected=%d, actual=%d)\n", message, expected, actual);  // Print mismatch.
        exit(1);  // Exit with failure.
    }  // Close failure branch.
    printf("PASS: %s\n", message);  // Print pass status.
}  // Close assert_int_equals().

static void assert_int_array_equals(const int* expected, int expected_len, const bt_int_array* actual, const char* message) {  // Minimal int-array equality helper.
    if (actual->len != expected_len) {  // Fail when lengths differ.
        printf("FAIL: %s (expected_len=%d, actual_len=%d)\n", message, expected_len, actual->len);  // Print mismatch.
        exit(1);  // Exit with failure.
    }  // Close length check.
    for (int i = 0; i < expected_len; i++) {  // Compare each element.
        if (actual->data[i] != expected[i]) {  // Fail on first mismatch.
            printf("FAIL: %s (index=%d, expected=%d, actual=%d)\n", message, i, expected[i], actual->data[i]);  // Print mismatch.
            exit(1);  // Exit with failure.
        }  // Close mismatch branch.
    }  // Close loop.
    printf("PASS: %s\n", message);  // Print pass status.
}  // Close assert_int_array_equals().

static void test_empty_tree_from_empty_array(void) {  // Empty input should create an empty tree.
    binary_tree t;  // Create tree struct.
    assert_true(binary_tree_from_level_order(&t, NULL, 0), "from_level_order(empty) should succeed");  // Build empty tree.
    assert_true(t.root == NULL, "root should be NULL for empty tree");  // Validate emptiness.
    assert_int_equals(0, binary_tree_size(&t), "size should be 0 for empty");  // Validate size.
    assert_int_equals(-1, binary_tree_height(&t), "height should be -1 for empty");  // Validate height definition.
    assert_int_equals(0, binary_tree_count_leaves(&t), "leaves should be 0 for empty");  // Validate leaves.
    bt_int_array a;  // Output traversal buffer.
    assert_true(binary_tree_preorder(&t, &a), "preorder should succeed");  // Compute preorder.
    assert_int_array_equals(NULL, 0, &a, "preorder should be empty");  // Validate preorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_inorder(&t, &a), "inorder should succeed");  // Compute inorder.
    assert_int_array_equals(NULL, 0, &a, "inorder should be empty");  // Validate inorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_postorder(&t, &a), "postorder should succeed");  // Compute postorder.
    assert_int_array_equals(NULL, 0, &a, "postorder should be empty");  // Validate postorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_level_order(&t, &a), "level_order should succeed");  // Compute level-order.
    assert_int_array_equals(NULL, 0, &a, "level_order should be empty");  // Validate BFS.
    bt_int_array_destroy(&a);  // Free buffer.
    binary_tree_destroy(&t);  // Destroy tree (no-op for empty).
}  // Close test_empty_tree_from_empty_array().

static void test_single_node_tree(void) {  // Single value should create a one-node tree.
    bt_optional_int values[] = {BT_SOME(1)};  // One-node level-order input.
    binary_tree t;  // Create tree struct.
    assert_true(binary_tree_from_level_order(&t, values, 1), "from_level_order([1]) should succeed");  // Build tree.
    assert_true(t.root != NULL, "root should not be NULL");  // Validate non-empty.
    assert_int_equals(1, binary_tree_size(&t), "size should be 1");  // Validate size.
    assert_int_equals(0, binary_tree_height(&t), "height should be 0 for leaf");  // Validate height.
    assert_int_equals(1, binary_tree_count_leaves(&t), "leaves should be 1");  // Validate leaves.
    bt_int_array a;  // Output traversal buffer.
    assert_true(binary_tree_preorder(&t, &a), "preorder should succeed");  // Compute preorder.
    assert_int_array_equals((int[]){1}, 1, &a, "preorder should be [1]");  // Validate preorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_inorder(&t, &a), "inorder should succeed");  // Compute inorder.
    assert_int_array_equals((int[]){1}, 1, &a, "inorder should be [1]");  // Validate inorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_postorder(&t, &a), "postorder should succeed");  // Compute postorder.
    assert_int_array_equals((int[]){1}, 1, &a, "postorder should be [1]");  // Validate postorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_level_order(&t, &a), "level_order should succeed");  // Compute level-order.
    assert_int_array_equals((int[]){1}, 1, &a, "level_order should be [1]");  // Validate BFS.
    bt_int_array_destroy(&a);  // Free buffer.
    binary_tree_destroy(&t);  // Free tree nodes.
}  // Close test_single_node_tree().

static void test_sample_tree_without_holes(void) {  // Sample tree should produce expected traversals.
    bt_optional_int values[] = {BT_SOME(1), BT_SOME(2), BT_SOME(3), BT_SOME(4), BT_SOME(5)};  // Sample level-order array.
    binary_tree t;  // Create tree struct.
    assert_true(binary_tree_from_level_order(&t, values, 5), "from_level_order(sample) should succeed");  // Build tree.
    assert_int_equals(5, binary_tree_size(&t), "size should be 5");  // Validate size.
    assert_int_equals(2, binary_tree_height(&t), "height should be 2 (edges)");  // Validate height.
    assert_int_equals(3, binary_tree_count_leaves(&t), "leaves should be 3");  // Validate leaves.
    bt_int_array a;  // Output traversal buffer.
    assert_true(binary_tree_preorder(&t, &a), "preorder should succeed");  // Compute preorder.
    assert_int_array_equals((int[]){1, 2, 4, 5, 3}, 5, &a, "preorder should match");  // Validate preorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_inorder(&t, &a), "inorder should succeed");  // Compute inorder.
    assert_int_array_equals((int[]){4, 2, 5, 1, 3}, 5, &a, "inorder should match");  // Validate inorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_postorder(&t, &a), "postorder should succeed");  // Compute postorder.
    assert_int_array_equals((int[]){4, 5, 2, 3, 1}, 5, &a, "postorder should match");  // Validate postorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_level_order(&t, &a), "level_order should succeed");  // Compute level-order.
    assert_int_array_equals((int[]){1, 2, 3, 4, 5}, 5, &a, "level_order should match");  // Validate BFS.
    bt_int_array_destroy(&a);  // Free buffer.
    binary_tree_destroy(&t);  // Free tree nodes.
}  // Close test_sample_tree_without_holes().

static void test_tree_with_holes(void) {  // Missing nodes (nulls) should be handled correctly.
    bt_optional_int values[] = {BT_SOME(1), BT_SOME(2), BT_SOME(3), BT_NONE, BT_SOME(5), BT_NONE, BT_SOME(7)};  // Sample with holes.
    binary_tree t;  // Create tree struct.
    assert_true(binary_tree_from_level_order(&t, values, 7), "from_level_order(holes) should succeed");  // Build tree.
    assert_int_equals(5, binary_tree_size(&t), "size should be 5");  // Validate size.
    assert_int_equals(2, binary_tree_height(&t), "height should be 2 (edges)");  // Validate height.
    assert_int_equals(2, binary_tree_count_leaves(&t), "leaves should be 2");  // Validate leaves.
    bt_int_array a;  // Output traversal buffer.
    assert_true(binary_tree_preorder(&t, &a), "preorder should succeed");  // Compute preorder.
    assert_int_array_equals((int[]){1, 2, 5, 3, 7}, 5, &a, "preorder should match");  // Validate preorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_inorder(&t, &a), "inorder should succeed");  // Compute inorder.
    assert_int_array_equals((int[]){2, 5, 1, 3, 7}, 5, &a, "inorder should match");  // Validate inorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_postorder(&t, &a), "postorder should succeed");  // Compute postorder.
    assert_int_array_equals((int[]){5, 2, 7, 3, 1}, 5, &a, "postorder should match");  // Validate postorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_level_order(&t, &a), "level_order should succeed");  // Compute level-order.
    assert_int_array_equals((int[]){1, 2, 3, 5, 7}, 5, &a, "level_order should match");  // Validate BFS.
    bt_int_array_destroy(&a);  // Free buffer.
    binary_tree_destroy(&t);  // Free tree nodes.
}  // Close test_tree_with_holes().

static void test_root_null_builds_empty_tree(void) {  // Null root should produce an empty tree even if later values exist.
    bt_optional_int values[] = {BT_NONE, BT_SOME(1), BT_SOME(2)};  // Root is missing.
    binary_tree t;  // Create tree struct.
    assert_true(binary_tree_from_level_order(&t, values, 3), "from_level_order([null,1,2]) should succeed");  // Build tree.
    assert_true(t.root == NULL, "tree should be empty when root is null");  // Validate emptiness.
    assert_int_equals(0, binary_tree_size(&t), "size should be 0");  // Validate size.
    assert_int_equals(-1, binary_tree_height(&t), "height should be -1");  // Validate height.
    assert_int_equals(0, binary_tree_count_leaves(&t), "leaves should be 0");  // Validate leaves.
    bt_int_array a;  // Output traversal buffer.
    assert_true(binary_tree_preorder(&t, &a), "preorder should succeed");  // Compute preorder.
    assert_int_array_equals(NULL, 0, &a, "preorder should be empty");  // Validate preorder.
    bt_int_array_destroy(&a);  // Free buffer.
    assert_true(binary_tree_level_order(&t, &a), "level_order should succeed");  // Compute BFS.
    assert_int_array_equals(NULL, 0, &a, "level_order should be empty");  // Validate BFS.
    bt_int_array_destroy(&a);  // Free buffer.
    binary_tree_destroy(&t);  // Destroy tree (no-op for empty).
}  // Close test_root_null_builds_empty_tree().

int main(void) {  // Run all tests and print status.
    printf("=== BinaryTree Tests (C) ===\n");  // Print header.
    test_empty_tree_from_empty_array();  // Run empty input test.
    test_single_node_tree();  // Run single node test.
    test_sample_tree_without_holes();  // Run sample tree test.
    test_tree_with_holes();  // Run hole-handling test.
    test_root_null_builds_empty_tree();  // Run null-root test.
    printf("All tests PASSED.\n");  // Print success message.
    return 0;  // Exit success.
}  // Close main().
