// 01 AVL Tree 測試（C）/ Tests for AVL tree (C).  // Bilingual file header.

#include "avl.h"  // Include the API under test.

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

static void assert_int_array_equals(const int* expected, int expected_len, const avl_int_array* actual, const char* message) {  // Minimal int-array equality helper.
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

static void assert_less_equal(int upper_bound, int actual, const char* message) {  // Minimal <= assertion helper.
    if (actual > upper_bound) {  // Fail when value is larger than bound.
        printf("FAIL: %s (upperBound=%d, actual=%d)\n", message, upper_bound, actual);  // Print mismatch.
        exit(1);  // Exit with failure.
    }  // Close failure branch.
    printf("PASS: %s\n", message);  // Print pass status.
}  // Close assert_less_equal().

static void insert_values(avl_tree* t, const int* values, int n) {  // Helper to build an AVL tree by inserting values in order.
    for (int i = 0; i < n; i++) {  // Insert each value.
        avl_insert(t, values[i]);  // Insert one key (duplicates are rejected by policy).
    }  // Close loop.
}  // Close insert_values().

static void test_empty_tree_properties(void) {  // Empty tree should follow our edge-based height definition.
    avl_tree t;  // Create AVL tree struct.
    avl_init(&t);  // Initialize empty tree.
    assert_true(t.root == NULL, "root should be NULL for empty tree");  // Validate emptiness.
    assert_int_equals(0, avl_size(&t), "size should be 0 for empty");  // Validate size.
    assert_int_equals(-1, avl_height(&t), "height should be -1 for empty");  // Validate height definition.
    int out = 0;  // Output slot for min/max.
    assert_true(!avl_min(&t, &out), "min should fail on empty");  // Validate min for empty.
    assert_true(!avl_max(&t, &out), "max should fail on empty");  // Validate max for empty.
    assert_true(!avl_contains(&t, 1), "contains should be false on empty");  // Validate contains.
    assert_true(!avl_delete(&t, 1), "delete should be false when key is missing");  // Validate delete missing.
    assert_true(avl_validate(&t), "validate should be true on empty");  // Empty tree is valid.
    avl_int_array inorder;  // Output traversal buffer.
    assert_true(avl_inorder(&t, &inorder), "inorder should succeed on empty");  // Compute inorder.
    assert_int_array_equals(NULL, 0, &inorder, "inorder should be empty");  // Validate inorder.
    avl_int_array_destroy(&inorder);  // Free buffer.
    avl_destroy(&t);  // Destroy tree (no-op for empty).
}  // Close test_empty_tree_properties().

static void test_insert_rejects_duplicates(void) {  // Insert should reject duplicates by policy.
    avl_tree t;  // Create AVL tree struct.
    avl_init(&t);  // Initialize empty tree.
    assert_true(avl_insert(&t, 5), "first insert should succeed");  // Insert 5.
    assert_true(avl_contains(&t, 5), "contains should find inserted key");  // Validate contains.
    assert_true(!avl_insert(&t, 5), "duplicate insert should be rejected");  // Duplicate insert.
    assert_int_equals(1, avl_size(&t), "size should remain 1 after duplicate");  // Validate size.
    assert_true(avl_validate(&t), "tree should remain valid after duplicate attempt");  // Validate invariants.
    avl_destroy(&t);  // Free nodes.
}  // Close test_insert_rejects_duplicates().

static void test_rotation_patterns(void) {  // Classic 3-key patterns should produce a perfectly balanced tree.
    int sequences[][3] = {  // Define insert sequences for each rotation pattern.
        {3, 2, 1},  // LL case.
        {1, 2, 3},  // RR case.
        {3, 1, 2},  // LR case.
        {1, 3, 2},  // RL case.
    };  // Close sequences table.

    for (int i = 0; i < 4; i++) {  // Check each scenario.
        avl_tree t;  // Create AVL tree struct.
        avl_init(&t);  // Initialize empty tree.
        insert_values(&t, sequences[i], 3);  // Insert 3 keys to trigger balancing.
        assert_true(avl_validate(&t), "tree should be valid after inserts");  // Validate AVL invariants.
        avl_int_array inorder;  // Output traversal buffer.
        assert_true(avl_inorder(&t, &inorder), "inorder should succeed for 3-key tree");  // Compute inorder.
        assert_int_array_equals((int[]){1, 2, 3}, 3, &inorder, "inorder should be sorted for 3-key tree");  // Validate inorder.
        avl_int_array_destroy(&inorder);  // Free buffer.
        assert_int_equals(1, avl_height(&t), "height should be 1 for 3 nodes");  // Height should be 1 in edges.
        avl_destroy(&t);  // Free nodes.
    }  // Close loop.
}  // Close test_rotation_patterns().

static void test_inorder_sorted_and_height_bound(void) {  // AVL should keep height small while preserving sorted inorder.
    int values[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic insert order from many textbooks.
    avl_tree t;  // Create AVL tree struct.
    avl_init(&t);  // Initialize empty tree.
    insert_values(&t, values, 9);  // Build AVL tree by insertion.
    assert_int_equals(9, avl_size(&t), "size should be 9");  // Validate size.
    assert_true(avl_validate(&t), "tree should satisfy AVL invariants");  // Validate invariants.
    assert_less_equal(3, avl_height(&t), "for 9 nodes, AVL height (edges) must be <= 3");  // Bound check.
    int min_key = 0;  // Output slot for min.
    int max_key = 0;  // Output slot for max.
    assert_true(avl_min(&t, &min_key), "min should succeed");  // Compute min.
    assert_true(avl_max(&t, &max_key), "max should succeed");  // Compute max.
    assert_int_equals(1, min_key, "min should be 1");  // Validate min.
    assert_int_equals(14, max_key, "max should be 14");  // Validate max.
    avl_int_array inorder;  // Output traversal buffer.
    assert_true(avl_inorder(&t, &inorder), "inorder should succeed");  // Compute inorder.
    assert_int_array_equals((int[]){1, 3, 4, 6, 7, 8, 10, 13, 14}, 9, &inorder, "inorder should be sorted");  // Validate inorder.
    avl_int_array_destroy(&inorder);  // Free buffer.
    avl_destroy(&t);  // Free nodes.
}  // Close test_inorder_sorted_and_height_bound().

static void test_delete_cases_and_root_delete(void) {  // Delete should rebalance and keep inorder sorted.
    int values[] = {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Build a tree with multiple delete cases.
    avl_tree t;  // Create AVL tree struct.
    avl_init(&t);  // Initialize empty tree.
    insert_values(&t, values, 9);  // Build AVL tree by insertion.
    assert_true(avl_validate(&t), "initial tree should be valid");  // Sanity-check initial validity.

    assert_true(avl_delete(&t, 7), "delete leaf (7) should succeed");  // Delete a leaf node.
    assert_true(!avl_contains(&t, 7), "deleted key (7) should be missing");  // Validate deletion.
    avl_int_array inorder;  // Output traversal buffer.
    assert_true(avl_inorder(&t, &inorder), "inorder should succeed after deleting 7");  // Compute inorder.
    assert_int_array_equals((int[]){1, 3, 4, 6, 8, 10, 13, 14}, 8, &inorder, "inorder after deleting 7 should match");  // Validate inorder.
    avl_int_array_destroy(&inorder);  // Free buffer.
    assert_true(avl_validate(&t), "tree should remain valid after deleting 7");  // Validate invariants.

    assert_true(avl_delete(&t, 14), "delete one-child node (14) should succeed");  // Delete one-child node.
    assert_true(!avl_contains(&t, 14), "deleted key (14) should be missing");  // Validate deletion.
    assert_true(avl_inorder(&t, &inorder), "inorder should succeed after deleting 14");  // Compute inorder.
    assert_int_array_equals((int[]){1, 3, 4, 6, 8, 10, 13}, 7, &inorder, "inorder after deleting 14 should match");  // Validate inorder.
    avl_int_array_destroy(&inorder);  // Free buffer.
    assert_true(avl_validate(&t), "tree should remain valid after deleting 14");  // Validate invariants.

    assert_true(avl_delete(&t, 3), "delete two-child node (3) should succeed");  // Delete two-child node.
    assert_true(!avl_contains(&t, 3), "deleted key (3) should be missing");  // Validate deletion.
    assert_true(avl_inorder(&t, &inorder), "inorder should succeed after deleting 3");  // Compute inorder.
    assert_int_array_equals((int[]){1, 4, 6, 8, 10, 13}, 6, &inorder, "inorder after deleting 3 should match");  // Validate inorder.
    avl_int_array_destroy(&inorder);  // Free buffer.
    assert_true(avl_validate(&t), "tree should remain valid after deleting 3");  // Validate invariants.
    assert_less_equal(2, avl_height(&t), "with 6 nodes, AVL height (edges) must be <= 2");  // Bound check.

    avl_destroy(&t);  // Free nodes.

    avl_tree t2;  // Create another tree for root deletion test.
    avl_init(&t2);  // Initialize empty tree.
    insert_values(&t2, values, 9);  // Build AVL tree by insertion.
    assert_true(avl_delete(&t2, 8), "delete root (8) should succeed");  // Delete the original root.
    assert_true(!avl_contains(&t2, 8), "deleted root key (8) should be missing");  // Validate deletion.
    assert_true(avl_inorder(&t2, &inorder), "inorder should succeed after deleting root");  // Compute inorder.
    assert_int_array_equals((int[]){1, 3, 4, 6, 7, 10, 13, 14}, 8, &inorder, "inorder should remain sorted without 8");  // Validate inorder.
    avl_int_array_destroy(&inorder);  // Free buffer.
    assert_true(avl_validate(&t2), "tree should remain valid after root deletion");  // Validate invariants.
    avl_destroy(&t2);  // Free nodes.
}  // Close test_delete_cases_and_root_delete().

int main(void) {  // Run all tests and print status.
    printf("=== AVL Tests (C) ===\n");  // Print header.
    test_empty_tree_properties();  // Run empty tree tests.
    test_insert_rejects_duplicates();  // Run duplicate insert tests.
    test_rotation_patterns();  // Run rotation-pattern tests.
    test_inorder_sorted_and_height_bound();  // Run inorder/height bound tests.
    test_delete_cases_and_root_delete();  // Run delete tests.
    printf("All tests PASSED.\n");  // Print success message.
    return 0;  // Exit success.
}  // Close main().

