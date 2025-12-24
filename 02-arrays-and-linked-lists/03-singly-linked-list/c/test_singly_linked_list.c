/* 03 單向鏈結串列測試（C）/ Tests for singly linked list (C). */  // Bilingual test file header.
#include "singly_linked_list.h"  // Include the API under test.

#include <stdio.h>  // Use printf for human-readable test output.

static int tests_failed = 0;  // Track how many assertions failed.

static void assert_true(int condition, const char* message) {  // Minimal boolean assertion helper.
    if (!condition) {  // Fail when condition is false.
        tests_failed += 1;  // Record one failure.
        printf("FAIL: %s\n", message);  // Print failure message.
    } else {  // Pass branch.
        printf("PASS: %s\n", message);  // Print pass marker.
    }  // Close branch.
}  // End assert_true.

static void assert_eq_ll(long long expected, long long actual, const char* message) {  // Minimal long long equality assertion helper.
    if (expected != actual) {  // Fail when values differ.
        tests_failed += 1;  // Record one failure.
        printf("FAIL: %s (expected=%lld, actual=%lld)\n", message, expected, actual);  // Print mismatch.
    } else {  // Pass branch.
        printf("PASS: %s\n", message);  // Print pass marker.
    }  // Close branch.
}  // End assert_eq_ll.

static bool build_ordered_list(int n, singly_linked_list* out_list) {  // Build list containing [0..n-1] in order.
    if (!out_list) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (n < 0) {  // Reject invalid sizes.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (!singly_linked_list_init(out_list)) {  // Initialize list.
        return false;  // Signal failure.
    }  // Close init failure.
    for (int v = n - 1; v >= 0; v--) {  // Push front in reverse.
        singly_list_cost cost;  // Per-op cost (ignored).
        if (!singly_linked_list_push_front(out_list, v, &cost)) {  // Insert node at head.
            singly_linked_list_destroy(out_list);  // Clean up on failure.
            return false;  // Signal failure.
        }  // Close failure check.
    }  // Close loop.
    return true;  // Signal success.
}  // End build_ordered_list.

static void test_push_front_hops_is_zero(void) {  // push_front should never traverse next pointers.
    singly_linked_list lst;  // Local list.
    assert_true(singly_linked_list_init(&lst), "init should succeed");  // Initialize.
    int values[] = {1, 2, 3, 4};  // Values to insert.
    for (size_t i = 0; i < sizeof(values) / sizeof(values[0]); i++) {  // Insert each value.
        singly_list_cost cost;  // Cost record.
        assert_true(singly_linked_list_push_front(&lst, values[i], &cost), "push_front should succeed");  // Insert at head.
        assert_eq_ll(0, cost.hops, "push_front hops should be 0");  // Validate cost.
    }  // Close loop.
    singly_linked_list_destroy(&lst);  // Clean up.
}  // End test_push_front_hops_is_zero.

static void test_push_back_hops_equals_size_minus_one(void) {  // push_back should traverse to tail without a tail pointer.
    int ns[] = {0, 1, 2, 3, 5, 10};  // Multiple sizes.
    for (size_t i = 0; i < sizeof(ns) / sizeof(ns[0]); i++) {  // Iterate test cases.
        int n = ns[i];  // Read n.
        singly_linked_list lst;  // Local list.
        assert_true(build_ordered_list(n, &lst), "build_ordered_list should succeed");  // Build [0..n-1].
        singly_list_cost cost;  // Cost record.
        assert_true(singly_linked_list_push_back(&lst, 999, &cost), "push_back should succeed");  // Append sentinel.
        int expected = (n == 0) ? 0 : (n - 1);  // Expected hops.
        assert_eq_ll(expected, cost.hops, "push_back hops should equal max(0,n-1)");  // Validate cost.
        assert_eq_ll(n + 1, lst.size, "size should increase by one");  // Validate size update.
        int buffer[32];  // Output buffer for array copy.
        assert_true(singly_linked_list_to_array(&lst, buffer, 32), "to_array should succeed");  // Copy list to array.
        assert_eq_ll(999, buffer[lst.size - 1], "push_back should append to tail");  // Validate tail value.
        singly_linked_list_destroy(&lst);  // Clean up.
    }  // Close loop.
}  // End test_push_back_hops_equals_size_minus_one.

static void test_get_hops_equals_index(void) {  // get should traverse index links.
    singly_linked_list lst;  // Local list.
    assert_true(build_ordered_list(10, &lst), "build_ordered_list should succeed");  // Build [0..9].
    int indices[] = {0, 1, 5, 9};  // Sample indices.
    for (size_t i = 0; i < sizeof(indices) / sizeof(indices[0]); i++) {  // Iterate indices.
        int idx = indices[i];  // Read index.
        singly_list_get_result r;  // Result record.
        assert_true(singly_linked_list_get(&lst, idx, &r), "get should succeed");  // Fetch.
        assert_eq_ll(idx, r.value, "value should equal index for ordered list");  // Validate value.
        assert_eq_ll(idx, r.cost.hops, "get hops should equal index");  // Validate cost.
    }  // Close loop.
    singly_linked_list_destroy(&lst);  // Clean up.
}  // End test_get_hops_equals_index.

static void test_insert_at_cost_and_ordering(void) {  // insert_at should splice correctly and report hops.
    singly_linked_list a;  // List A.
    assert_true(build_ordered_list(5, &a), "build_ordered_list should succeed");  // Build [0..4].
    singly_list_cost c0;  // Cost record.
    assert_true(singly_linked_list_insert_at(&a, 0, 99, &c0), "insert_at(0) should succeed");  // Insert at head.
    assert_eq_ll(0, c0.hops, "insert_at(0) hops should be 0");  // Validate cost.
    int buffer_a[32];  // Output buffer.
    assert_true(singly_linked_list_to_array(&a, buffer_a, 32), "to_array should succeed");  // Copy list.
    int expected_a[] = {99, 0, 1, 2, 3, 4};  // Expected ordering.
    for (int i = 0; i < a.size; i++) {  // Compare arrays.
        assert_eq_ll(expected_a[i], buffer_a[i], "insert_at(0) ordering should match");  // Validate element.
    }  // Close loop.
    singly_linked_list_destroy(&a);  // Clean up.

    singly_linked_list b;  // List B.
    assert_true(build_ordered_list(5, &b), "build_ordered_list should succeed");  // Build [0..4].
    singly_list_cost c3;  // Cost record.
    assert_true(singly_linked_list_insert_at(&b, 3, 77, &c3), "insert_at(3) should succeed");  // Insert at index 3.
    assert_eq_ll(2, c3.hops, "insert_at(3) hops should be 2");  // Validate cost.
    int buffer_b[32];  // Output buffer.
    assert_true(singly_linked_list_to_array(&b, buffer_b, 32), "to_array should succeed");  // Copy list.
    int expected_b[] = {0, 1, 2, 77, 3, 4};  // Expected ordering.
    for (int i = 0; i < b.size; i++) {  // Compare arrays.
        assert_eq_ll(expected_b[i], buffer_b[i], "insert_at(3) ordering should match");  // Validate element.
    }  // Close loop.
    singly_linked_list_destroy(&b);  // Clean up.
}  // End test_insert_at_cost_and_ordering.

static void test_remove_at_cost_and_ordering(void) {  // remove_at should bypass correctly and report hops.
    singly_linked_list a;  // List A.
    assert_true(build_ordered_list(5, &a), "build_ordered_list should succeed");  // Build [0..4].
    singly_list_remove_result r0;  // Remove result.
    assert_true(singly_linked_list_remove_at(&a, 0, &r0), "remove_at(0) should succeed");  // Remove head.
    assert_eq_ll(0, r0.value, "remove_at(0) should remove value 0");  // Validate removed value.
    assert_eq_ll(0, r0.cost.hops, "remove_at(0) hops should be 0");  // Validate cost.
    int buffer_a[32];  // Output buffer.
    assert_true(singly_linked_list_to_array(&a, buffer_a, 32), "to_array should succeed");  // Copy list.
    int expected_a[] = {1, 2, 3, 4};  // Expected ordering.
    for (int i = 0; i < a.size; i++) {  // Compare arrays.
        assert_eq_ll(expected_a[i], buffer_a[i], "remove_at(0) ordering should match");  // Validate element.
    }  // Close loop.
    singly_linked_list_destroy(&a);  // Clean up.

    singly_linked_list b;  // List B.
    assert_true(build_ordered_list(5, &b), "build_ordered_list should succeed");  // Build [0..4].
    singly_list_remove_result r3;  // Remove result.
    assert_true(singly_linked_list_remove_at(&b, 3, &r3), "remove_at(3) should succeed");  // Remove index 3 (value 3).
    assert_eq_ll(3, r3.value, "remove_at(3) should remove value 3");  // Validate removed value.
    assert_eq_ll(2, r3.cost.hops, "remove_at(3) hops should be 2");  // Validate cost.
    int buffer_b[32];  // Output buffer.
    assert_true(singly_linked_list_to_array(&b, buffer_b, 32), "to_array should succeed");  // Copy list.
    int expected_b[] = {0, 1, 2, 4};  // Expected ordering.
    for (int i = 0; i < b.size; i++) {  // Compare arrays.
        assert_eq_ll(expected_b[i], buffer_b[i], "remove_at(3) ordering should match");  // Validate element.
    }  // Close loop.
    singly_linked_list_destroy(&b);  // Clean up.
}  // End test_remove_at_cost_and_ordering.

static void test_index_of_with_cost_hops(void) {  // index_of should count next traversals.
    singly_linked_list lst;  // Local list.
    assert_true(build_ordered_list(5, &lst), "build_ordered_list should succeed");  // Build [0..4].

    singly_list_find_result f0;  // Find result.
    assert_true(singly_linked_list_index_of(&lst, 0, &f0), "index_of should succeed");  // Find 0.
    assert_eq_ll(0, f0.index, "index_of(0) should return 0");  // Validate index.
    assert_eq_ll(0, f0.cost.hops, "finding head needs 0 hops");  // Validate cost.

    singly_list_find_result f3;  // Find result.
    assert_true(singly_linked_list_index_of(&lst, 3, &f3), "index_of should succeed");  // Find 3.
    assert_eq_ll(3, f3.index, "index_of(3) should return 3");  // Validate index.
    assert_eq_ll(3, f3.cost.hops, "finding index 3 needs 3 hops");  // Validate cost.

    singly_list_find_result fn;  // Find result.
    assert_true(singly_linked_list_index_of(&lst, 999, &fn), "index_of should succeed");  // Search missing.
    assert_eq_ll(-1, fn.index, "index_of(missing) should return -1");  // Validate not found.
    assert_eq_ll(5, fn.cost.hops, "missing value should traverse n hops");  // Validate cost.

    singly_linked_list_destroy(&lst);  // Clean up.
}  // End test_index_of_with_cost_hops.

static void test_invalid_indices_fail(void) {  // Methods should reject invalid indices.
    singly_linked_list lst;  // Local list.
    assert_true(build_ordered_list(3, &lst), "build_ordered_list should succeed");  // Build [0..2].
    singly_list_get_result gr;  // Get result.
    assert_true(!singly_linked_list_get(&lst, -1, &gr), "get should fail on negative index");  // Invalid get.
    assert_true(!singly_linked_list_get(&lst, 3, &gr), "get should fail on index==size");  // Invalid get.
    singly_list_cost ic;  // Insert cost.
    assert_true(!singly_linked_list_insert_at(&lst, 4, 0, &ic), "insert_at should fail on index>size");  // Invalid insert.
    singly_list_remove_result rr;  // Remove result.
    assert_true(!singly_linked_list_remove_at(&lst, 3, &rr), "remove_at should fail on index==size");  // Invalid remove.
    singly_linked_list_destroy(&lst);  // Clean up.
}  // End test_invalid_indices_fail.

int main(void) {  // Run all tests and return non-zero on failure.
    printf("=== SinglyLinkedList Tests (C) ===\n");  // Print header.
    test_push_front_hops_is_zero();  // Run pushFront cost test.
    test_push_back_hops_equals_size_minus_one();  // Run pushBack cost test.
    test_get_hops_equals_index();  // Run get cost test.
    test_insert_at_cost_and_ordering();  // Run insert cost + ordering test.
    test_remove_at_cost_and_ordering();  // Run remove cost + ordering test.
    test_index_of_with_cost_hops();  // Run find cost test.
    test_invalid_indices_fail();  // Run invalid index tests.
    printf("==============================\n");  // Print separator.
    if (tests_failed == 0) {  // Determine pass/fail.
        printf("All tests PASSED.\n");  // Report success.
        return 0;  // Exit success.
    }  // Close pass branch.
    printf("Tests FAILED: %d\n", tests_failed);  // Report failure count.
    return 1;  // Exit failure.
}  // End main.

