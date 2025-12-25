// 03 樹走訪測試（C）/ Tests for tree traversal (C).  // Bilingual file header.

#include "tree_traversal.h"  // Include the API under test.

#include <stdio.h>  // Provide printf for status output.
#include <stdlib.h>  // Provide exit for early termination on failure.

static void assert_true(bool condition, const char* message) {  // Minimal assertion helper.
    if (!condition) {  // Fail when condition is false.
        printf("FAIL: %s\n", message);  // Print failure message.
        exit(1);  // Exit with failure.
    }  // Close failure branch.
    printf("PASS: %s\n", message);  // Print pass status.
}  // Close assert_true().

static void assert_int_array_equals(const int* expected, int expected_len, const tt_int_array* actual, const char* message) {  // Minimal int-array equality helper.
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

static void test_empty_tree(void) {  // Empty tree should produce empty traversals for all variants.
    tt_tree t;  // Create tree struct.
    assert_true(tt_tree_from_level_order(&t, NULL, 0), "from_level_order(empty) should succeed");  // Build empty tree.

    tt_int_array a;  // Output traversal buffer.
    assert_true(tt_preorder_recursive(&t, &a), "preorder_recursive should succeed");  // Compute recursive preorder.
    assert_int_array_equals(NULL, 0, &a, "preorder_recursive should be empty");  // Validate recursive preorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_preorder_iterative(&t, &a), "preorder_iterative should succeed");  // Compute iterative preorder.
    assert_int_array_equals(NULL, 0, &a, "preorder_iterative should be empty");  // Validate iterative preorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_inorder_recursive(&t, &a), "inorder_recursive should succeed");  // Compute recursive inorder.
    assert_int_array_equals(NULL, 0, &a, "inorder_recursive should be empty");  // Validate recursive inorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_inorder_iterative(&t, &a), "inorder_iterative should succeed");  // Compute iterative inorder.
    assert_int_array_equals(NULL, 0, &a, "inorder_iterative should be empty");  // Validate iterative inorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_postorder_recursive(&t, &a), "postorder_recursive should succeed");  // Compute recursive postorder.
    assert_int_array_equals(NULL, 0, &a, "postorder_recursive should be empty");  // Validate recursive postorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_postorder_iterative(&t, &a), "postorder_iterative should succeed");  // Compute iterative postorder.
    assert_int_array_equals(NULL, 0, &a, "postorder_iterative should be empty");  // Validate iterative postorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_level_order(&t, &a), "level_order should succeed");  // Compute BFS level-order.
    assert_int_array_equals(NULL, 0, &a, "level_order should be empty");  // Validate level-order.
    tt_int_array_destroy(&a);  // Free buffer.

    tt_tree_destroy(&t);  // Destroy tree (no-op for empty).
}  // Close test_empty_tree().

static void test_sample_tree_matches_expected(void) {  // Sample tree should match known traversal sequences.
    tt_optional_int values[] = {TT_SOME(1), TT_SOME(2), TT_SOME(3), TT_SOME(4), TT_SOME(5)};  // Sample tree used across this repo.
    tt_tree t;  // Create tree struct.
    assert_true(tt_tree_from_level_order(&t, values, 5), "from_level_order(sample) should succeed");  // Build tree.

    int expected_pre[] = {1, 2, 4, 5, 3};  // Expected preorder traversal.
    int expected_in[] = {4, 2, 5, 1, 3};  // Expected inorder traversal.
    int expected_post[] = {4, 5, 2, 3, 1};  // Expected postorder traversal.
    int expected_level[] = {1, 2, 3, 4, 5};  // Expected level-order traversal.

    tt_int_array a;  // Output traversal buffer.
    assert_true(tt_preorder_recursive(&t, &a), "preorder_recursive should succeed");  // Compute recursive preorder.
    assert_int_array_equals(expected_pre, 5, &a, "preorder_recursive should match");  // Validate recursive preorder.
    tt_int_array_destroy(&a);  // Free buffer.
    assert_true(tt_preorder_iterative(&t, &a), "preorder_iterative should succeed");  // Compute iterative preorder.
    assert_int_array_equals(expected_pre, 5, &a, "preorder_iterative should match");  // Validate iterative preorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_inorder_recursive(&t, &a), "inorder_recursive should succeed");  // Compute recursive inorder.
    assert_int_array_equals(expected_in, 5, &a, "inorder_recursive should match");  // Validate recursive inorder.
    tt_int_array_destroy(&a);  // Free buffer.
    assert_true(tt_inorder_iterative(&t, &a), "inorder_iterative should succeed");  // Compute iterative inorder.
    assert_int_array_equals(expected_in, 5, &a, "inorder_iterative should match");  // Validate iterative inorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_postorder_recursive(&t, &a), "postorder_recursive should succeed");  // Compute recursive postorder.
    assert_int_array_equals(expected_post, 5, &a, "postorder_recursive should match");  // Validate recursive postorder.
    tt_int_array_destroy(&a);  // Free buffer.
    assert_true(tt_postorder_iterative(&t, &a), "postorder_iterative should succeed");  // Compute iterative postorder.
    assert_int_array_equals(expected_post, 5, &a, "postorder_iterative should match");  // Validate iterative postorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_level_order(&t, &a), "level_order should succeed");  // Compute BFS level-order.
    assert_int_array_equals(expected_level, 5, &a, "level_order should match");  // Validate level-order.
    tt_int_array_destroy(&a);  // Free buffer.

    tt_tree_destroy(&t);  // Free tree nodes.
}  // Close test_sample_tree_matches_expected().

static void test_tree_with_holes_matches_expected(void) {  // Trees with null holes should still traverse correctly.
    tt_optional_int values[] = {TT_SOME(1), TT_SOME(2), TT_SOME(3), TT_NONE, TT_SOME(5), TT_NONE, TT_SOME(7)};  // Tree with missing children.
    tt_tree t;  // Create tree struct.
    assert_true(tt_tree_from_level_order(&t, values, 7), "from_level_order(holes) should succeed");  // Build tree.

    int expected_pre[] = {1, 2, 5, 3, 7};  // Expected preorder traversal.
    int expected_in[] = {2, 5, 1, 3, 7};  // Expected inorder traversal.
    int expected_post[] = {5, 2, 7, 3, 1};  // Expected postorder traversal.
    int expected_level[] = {1, 2, 3, 5, 7};  // Expected level-order traversal.

    tt_int_array a;  // Output traversal buffer.
    assert_true(tt_preorder_recursive(&t, &a), "preorder_recursive (holes) should succeed");  // Compute recursive preorder.
    assert_int_array_equals(expected_pre, 5, &a, "preorder_recursive (holes) should match");  // Validate preorder.
    tt_int_array_destroy(&a);  // Free buffer.
    assert_true(tt_preorder_iterative(&t, &a), "preorder_iterative (holes) should succeed");  // Compute iterative preorder.
    assert_int_array_equals(expected_pre, 5, &a, "preorder_iterative (holes) should match");  // Validate preorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_inorder_recursive(&t, &a), "inorder_recursive (holes) should succeed");  // Compute recursive inorder.
    assert_int_array_equals(expected_in, 5, &a, "inorder_recursive (holes) should match");  // Validate inorder.
    tt_int_array_destroy(&a);  // Free buffer.
    assert_true(tt_inorder_iterative(&t, &a), "inorder_iterative (holes) should succeed");  // Compute iterative inorder.
    assert_int_array_equals(expected_in, 5, &a, "inorder_iterative (holes) should match");  // Validate inorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_postorder_recursive(&t, &a), "postorder_recursive (holes) should succeed");  // Compute recursive postorder.
    assert_int_array_equals(expected_post, 5, &a, "postorder_recursive (holes) should match");  // Validate postorder.
    tt_int_array_destroy(&a);  // Free buffer.
    assert_true(tt_postorder_iterative(&t, &a), "postorder_iterative (holes) should succeed");  // Compute iterative postorder.
    assert_int_array_equals(expected_post, 5, &a, "postorder_iterative (holes) should match");  // Validate postorder.
    tt_int_array_destroy(&a);  // Free buffer.

    assert_true(tt_level_order(&t, &a), "level_order (holes) should succeed");  // Compute BFS level-order.
    assert_int_array_equals(expected_level, 5, &a, "level_order (holes) should match");  // Validate level-order.
    tt_int_array_destroy(&a);  // Free buffer.

    tt_tree_destroy(&t);  // Free tree nodes.
}  // Close test_tree_with_holes_matches_expected().

int main(void) {  // Run all tests and print status.
    printf("=== TreeTraversal Tests (C) ===\n");  // Print header.
    test_empty_tree();  // Run empty tree tests.
    test_sample_tree_matches_expected();  // Run sample tree tests.
    test_tree_with_holes_matches_expected();  // Run hole-handling tests.
    printf("All tests PASSED.\n");  // Print success message.
    return 0;  // Exit success.
}  // Close main().

