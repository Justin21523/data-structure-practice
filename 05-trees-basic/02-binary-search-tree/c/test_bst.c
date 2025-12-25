// 02 二元搜尋樹測試（C）/ Tests for BST (C).  // Bilingual file header.

#include "bst.h"  // Include the API under test.

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

static void assert_int_array_equals(const int* expected, int expected_len, const bst_int_array* actual, const char* message) {  // Minimal int-array equality helper.
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

static void insert_values(bst_tree* t, const int* values, int n) {  // Helper to build a BST by inserting values in order.
    for (int i = 0; i < n; i++) {  // Insert each value.
        bst_insert(t, values[i]);  // Insert one key (duplicates are ignored by policy).
    }  // Close loop.
}  // Close insert_values().

static void test_empty_tree_properties(void) {  // Empty tree should follow our edge-based height definition.
    bst_tree t;  // Create BST struct.
    bst_init(&t);  // Initialize empty BST.
    assert_true(t.root == NULL, "root should be NULL for empty tree");  // Validate emptiness.
    assert_int_equals(0, bst_size(&t), "size should be 0 for empty");  // Validate size.
    assert_int_equals(-1, bst_height(&t), "height should be -1 for empty");  // Validate height definition.
    int out = 0;  // Output slot for min/max.
    assert_true(!bst_min(&t, &out), "min should fail on empty");  // Validate min for empty.
    assert_true(!bst_max(&t, &out), "max should fail on empty");  // Validate max for empty.
    assert_true(!bst_contains(&t, 1), "contains should be false on empty");  // Validate contains.
    assert_true(!bst_delete(&t, 1), "delete should be false when key is missing");  // Validate delete missing.
    bst_int_array inorder;  // Output traversal buffer.
    assert_true(bst_inorder(&t, &inorder), "inorder should succeed on empty");  // Compute inorder.
    assert_int_array_equals(NULL, 0, &inorder, "inorder should be empty");  // Validate inorder.
    bst_int_array_destroy(&inorder);  // Free buffer.
    bst_destroy(&t);  // Destroy tree (no-op for empty).
}  // Close test_empty_tree_properties().

static void test_insert_contains_and_duplicates(void) {  // Insert should reject duplicates and contains should reflect membership.
    bst_tree t;  // Create BST struct.
    bst_init(&t);  // Initialize empty BST.
    assert_true(bst_insert(&t, 5), "first insert should succeed");  // Insert 5.
    assert_true(bst_contains(&t, 5), "contains should find inserted key");  // Validate contains.
    assert_true(!bst_insert(&t, 5), "duplicate insert should be rejected");  // Duplicate insert.
    assert_int_equals(1, bst_size(&t), "size should remain 1 after duplicate");  // Validate size.
    assert_true(!bst_contains(&t, 99), "contains should be false for missing key");  // Validate missing.
    bst_destroy(&t);  // Free nodes.
}  // Close test_insert_contains_and_duplicates().

static void test_inorder_sorted_and_min_max(void) {  // Inorder traversal should be sorted for a valid BST.
    int values[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic BST insert order.
    bst_tree t;  // Create BST struct.
    bst_init(&t);  // Initialize empty BST.
    insert_values(&t, values, 9);  // Build BST by insertion.
    assert_int_equals(9, bst_size(&t), "size should be 9");  // Validate size.
    assert_int_equals(3, bst_height(&t), "height should be 3");  // Validate height.
    int min_key = 0;  // Output slot for min.
    int max_key = 0;  // Output slot for max.
    assert_true(bst_min(&t, &min_key), "min should succeed on non-empty");  // Compute min.
    assert_true(bst_max(&t, &max_key), "max should succeed on non-empty");  // Compute max.
    assert_int_equals(1, min_key, "min should be 1");  // Validate min.
    assert_int_equals(14, max_key, "max should be 14");  // Validate max.
    bst_int_array inorder;  // Output traversal buffer.
    assert_true(bst_inorder(&t, &inorder), "inorder should succeed");  // Compute inorder.
    assert_int_array_equals((int[]){1, 3, 4, 6, 7, 8, 10, 13, 14}, 9, &inorder, "inorder should be sorted");  // Validate inorder.
    bst_int_array_destroy(&inorder);  // Free buffer.
    bst_destroy(&t);  // Free nodes.
}  // Close test_inorder_sorted_and_min_max().

static void test_delete_leaf_one_child_two_children(void) {  // Delete should handle all three structural cases.
    int values[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Build a tree with multiple delete cases.
    bst_tree t;  // Create BST struct.
    bst_init(&t);  // Initialize empty BST.
    insert_values(&t, values, 9);  // Build BST by insertion.

    assert_true(bst_delete(&t, 7), "delete leaf (7) should succeed");  // Delete leaf node.
    assert_true(!bst_contains(&t, 7), "deleted key (7) should be missing");  // Validate deletion.
    bst_int_array inorder;  // Output traversal buffer.
    assert_true(bst_inorder(&t, &inorder), "inorder should succeed after deleting 7");  // Compute inorder.
    assert_int_array_equals((int[]){1, 3, 4, 6, 8, 10, 13, 14}, 8, &inorder, "inorder after deleting 7 should match");  // Validate inorder.
    bst_int_array_destroy(&inorder);  // Free buffer.

    assert_true(bst_delete(&t, 14), "delete one-child node (14) should succeed");  // Delete one-child node.
    assert_true(!bst_contains(&t, 14), "deleted key (14) should be missing");  // Validate deletion.
    assert_true(bst_inorder(&t, &inorder), "inorder should succeed after deleting 14");  // Compute inorder.
    assert_int_array_equals((int[]){1, 3, 4, 6, 8, 10, 13}, 7, &inorder, "inorder after deleting 14 should match");  // Validate inorder.
    bst_int_array_destroy(&inorder);  // Free buffer.

    assert_true(bst_delete(&t, 3), "delete two-child node (3) should succeed");  // Delete two-child node.
    assert_true(!bst_contains(&t, 3), "deleted key (3) should be missing");  // Validate deletion.
    assert_true(bst_inorder(&t, &inorder), "inorder should succeed after deleting 3");  // Compute inorder.
    assert_int_array_equals((int[]){1, 4, 6, 8, 10, 13}, 6, &inorder, "inorder after deleting 3 should match");  // Validate inorder.
    bst_int_array_destroy(&inorder);  // Free buffer.
    assert_int_equals(2, bst_height(&t), "height should be 2 after deletions");  // Validate height change.
    int min_key = 0;  // Output slot for min.
    int max_key = 0;  // Output slot for max.
    assert_true(bst_min(&t, &min_key), "min should succeed after deletions");  // Compute min.
    assert_true(bst_max(&t, &max_key), "max should succeed after deletions");  // Compute max.
    assert_int_equals(1, min_key, "min should remain 1");  // Validate min.
    assert_int_equals(13, max_key, "max should become 13");  // Validate max.

    bst_destroy(&t);  // Free nodes.
}  // Close test_delete_leaf_one_child_two_children().

static void test_delete_root_two_children(void) {  // Deleting the root with two children should work and keep BST order.
    int values[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Root (8) has two children.
    bst_tree t;  // Create BST struct.
    bst_init(&t);  // Initialize empty BST.
    insert_values(&t, values, 9);  // Build BST by insertion.
    assert_true(bst_delete(&t, 8), "delete root (8) should succeed");  // Delete root.
    assert_true(!bst_contains(&t, 8), "deleted root key (8) should be missing");  // Validate deletion.
    assert_int_equals(8, bst_size(&t), "size should be 8 after deleting root");  // Validate size.
    bst_int_array inorder;  // Output traversal buffer.
    assert_true(bst_inorder(&t, &inorder), "inorder should succeed after deleting root");  // Compute inorder.
    assert_int_array_equals((int[]){1, 3, 4, 6, 7, 10, 13, 14}, 8, &inorder, "inorder should remain sorted without 8");  // Validate inorder.
    bst_int_array_destroy(&inorder);  // Free buffer.
    int min_key = 0;  // Output slot for min.
    int max_key = 0;  // Output slot for max.
    assert_true(bst_min(&t, &min_key), "min should succeed");  // Compute min.
    assert_true(bst_max(&t, &max_key), "max should succeed");  // Compute max.
    assert_int_equals(1, min_key, "min should stay 1");  // Validate min.
    assert_int_equals(14, max_key, "max should stay 14");  // Validate max.
    bst_destroy(&t);  // Free nodes.
}  // Close test_delete_root_two_children().

int main(void) {  // Run all tests and print status.
    printf("=== BST Tests (C) ===\n");  // Print header.
    test_empty_tree_properties();  // Run empty tree tests.
    test_insert_contains_and_duplicates();  // Run insert/contains/duplicate tests.
    test_inorder_sorted_and_min_max();  // Run inorder/min/max tests.
    test_delete_leaf_one_child_two_children();  // Run delete case tests.
    test_delete_root_two_children();  // Run root deletion test.
    printf("All tests PASSED.\n");  // Print success message.
    return 0;  // Exit success.
}  // Close main().

