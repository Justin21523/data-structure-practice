/* 04 雙向鏈結串列測試（C）/ Tests for doubly linked list (C). */  // Bilingual test file header.
#include "doubly_linked_list.h"  // Include the API under test.

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

static int expected_get_hops(int size, int index) {  // Compute expected hops when traversing from the nearest end.
    int a = index;  // Hops from head (index steps).
    int b = size - 1 - index;  // Hops from tail (size-1-index steps).
    return a < b ? a : b;  // Return min(a,b).
}  // End expected_get_hops.

static bool build_ordered_list(int n, doubly_linked_list* out_list) {  // Build list containing [0..n-1] in order.
    if (!out_list) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (n < 0) {  // Reject invalid sizes.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (!doubly_linked_list_init(out_list)) {  // Initialize list.
        return false;  // Signal failure.
    }  // Close init failure.
    for (int v = 0; v < n; v++) {  // Append in increasing order.
        dll_cost cost;  // Per-op cost (ignored).
        if (!doubly_linked_list_push_back(out_list, v, &cost)) {  // Append one value.
            doubly_linked_list_destroy(out_list);  // Clean up on failure.
            return false;  // Signal failure.
        }  // Close failure check.
    }  // Close loop.
    return true;  // Signal success.
}  // End build_ordered_list.

static void test_push_pop_ends_are_o1(void) {  // push/pop at both ends should have 0 hops with head+tail.
    int ns[] = {0, 1, 2, 5};  // Multiple sizes.
    for (size_t i = 0; i < sizeof(ns) / sizeof(ns[0]); i++) {  // Iterate test cases.
        int n = ns[i];  // Read n.
        doubly_linked_list lst;  // Local list.
        assert_true(build_ordered_list(n, &lst), "build_ordered_list should succeed");  // Build [0..n-1].

        dll_cost c1;  // pushBack cost.
        assert_true(doubly_linked_list_push_back(&lst, 999, &c1), "push_back should succeed");  // Insert at tail.
        assert_eq_ll(0, c1.hops, "push_back hops should be 0");  // Validate cost.

        dll_cost c2;  // pushFront cost.
        assert_true(doubly_linked_list_push_front(&lst, 888, &c2), "push_front should succeed");  // Insert at head.
        assert_eq_ll(0, c2.hops, "push_front hops should be 0");  // Validate cost.

        dll_remove_result r1;  // popBack result.
        assert_true(doubly_linked_list_pop_back(&lst, &r1), "pop_back should succeed");  // Remove tail.
        assert_eq_ll(0, r1.cost.hops, "pop_back hops should be 0");  // Validate cost.

        dll_remove_result r2;  // popFront result.
        assert_true(doubly_linked_list_pop_front(&lst, &r2), "pop_front should succeed");  // Remove head.
        assert_eq_ll(0, r2.cost.hops, "pop_front hops should be 0");  // Validate cost.

        doubly_linked_list_destroy(&lst);  // Clean up.
    }  // Close loop.
}  // End test_push_pop_ends_are_o1.

static void test_get_with_cost_uses_nearest_end(void) {  // get(i) hops should be min(i, size-1-i).
    doubly_linked_list lst;  // Local list.
    assert_true(build_ordered_list(16, &lst), "build_ordered_list should succeed");  // Build [0..15].
    int indices[] = {0, 1, 2, 4, 8, 15};  // Sample indices.
    for (size_t i = 0; i < sizeof(indices) / sizeof(indices[0]); i++) {  // Iterate indices.
        int idx = indices[i];  // Read index.
        dll_get_result r;  // Get result.
        assert_true(doubly_linked_list_get(&lst, idx, &r), "get should succeed");  // Fetch.
        assert_eq_ll(idx, r.value, "ordered list value should equal index");  // Validate value.
        assert_eq_ll(expected_get_hops(16, idx), r.cost.hops, "get hops should match nearest-end formula");  // Validate hops.
    }  // Close loop.
    doubly_linked_list_destroy(&lst);  // Clean up.
}  // End test_get_with_cost_uses_nearest_end.

static void test_insert_at_cost_and_ordering(void) {  // insert_at should splice correctly and report traversal hops.
    doubly_linked_list a;  // List A.
    assert_true(build_ordered_list(6, &a), "build_ordered_list should succeed");  // Build [0..5].
    dll_cost c0;  // Cost record.
    assert_true(doubly_linked_list_insert_at(&a, 0, 99, &c0), "insert_at(0) should succeed");  // Insert at head.
    assert_eq_ll(0, c0.hops, "insert_at(0) hops should be 0");  // Validate cost.
    int buffer_a[32];  // Output buffer.
    assert_true(doubly_linked_list_to_array(&a, buffer_a, 32), "to_array should succeed");  // Copy list.
    int expected_a[] = {99, 0, 1, 2, 3, 4, 5};  // Expected ordering.
    for (int i = 0; i < a.size; i++) {  // Compare arrays.
        assert_eq_ll(expected_a[i], buffer_a[i], "insert_at(0) ordering should match");  // Validate element.
    }  // Close loop.
    doubly_linked_list_destroy(&a);  // Clean up.

    doubly_linked_list b;  // List B.
    assert_true(build_ordered_list(6, &b), "build_ordered_list should succeed");  // Build [0..5].
    dll_cost c3;  // Cost record.
    assert_true(doubly_linked_list_insert_at(&b, 3, 77, &c3), "insert_at(3) should succeed");  // Insert at index 3.
    assert_eq_ll(expected_get_hops(6, 3), c3.hops, "insert_at(3) hops should match nearest-end formula");  // Validate cost.
    int buffer_b[32];  // Output buffer.
    assert_true(doubly_linked_list_to_array(&b, buffer_b, 32), "to_array should succeed");  // Copy list.
    int expected_b[] = {0, 1, 2, 77, 3, 4, 5};  // Expected ordering.
    for (int i = 0; i < b.size; i++) {  // Compare arrays.
        assert_eq_ll(expected_b[i], buffer_b[i], "insert_at(3) ordering should match");  // Validate element.
    }  // Close loop.
    doubly_linked_list_destroy(&b);  // Clean up.
}  // End test_insert_at_cost_and_ordering.

static void test_remove_at_cost_and_ordering(void) {  // remove_at should bypass correctly and report traversal hops.
    doubly_linked_list a;  // List A.
    assert_true(build_ordered_list(6, &a), "build_ordered_list should succeed");  // Build [0..5].
    dll_remove_result r0;  // Remove result.
    assert_true(doubly_linked_list_remove_at(&a, 0, &r0), "remove_at(0) should succeed");  // Remove head.
    assert_eq_ll(0, r0.value, "remove_at(0) should remove value 0");  // Validate value.
    assert_eq_ll(0, r0.cost.hops, "remove_at(0) hops should be 0");  // Validate cost.
    int buffer_a[32];  // Output buffer.
    assert_true(doubly_linked_list_to_array(&a, buffer_a, 32), "to_array should succeed");  // Copy list.
    int expected_a[] = {1, 2, 3, 4, 5};  // Expected ordering.
    for (int i = 0; i < a.size; i++) {  // Compare arrays.
        assert_eq_ll(expected_a[i], buffer_a[i], "remove_at(0) ordering should match");  // Validate element.
    }  // Close loop.
    doubly_linked_list_destroy(&a);  // Clean up.

    doubly_linked_list b;  // List B.
    assert_true(build_ordered_list(6, &b), "build_ordered_list should succeed");  // Build [0..5].
    dll_remove_result r5;  // Remove result.
    assert_true(doubly_linked_list_remove_at(&b, 5, &r5), "remove_at(tail) should succeed");  // Remove tail.
    assert_eq_ll(5, r5.value, "remove_at(tail) should remove value 5");  // Validate value.
    assert_eq_ll(0, r5.cost.hops, "remove_at(tail) hops should be 0");  // Validate cost.
    int buffer_b[32];  // Output buffer.
    assert_true(doubly_linked_list_to_array(&b, buffer_b, 32), "to_array should succeed");  // Copy list.
    int expected_b[] = {0, 1, 2, 3, 4};  // Expected ordering.
    for (int i = 0; i < b.size; i++) {  // Compare arrays.
        assert_eq_ll(expected_b[i], buffer_b[i], "remove_at(tail) ordering should match");  // Validate element.
    }  // Close loop.
    doubly_linked_list_destroy(&b);  // Clean up.
}  // End test_remove_at_cost_and_ordering.

static void test_index_of_with_cost_hops(void) {  // index_of should count next traversals.
    doubly_linked_list lst;  // Local list.
    assert_true(build_ordered_list(5, &lst), "build_ordered_list should succeed");  // Build [0..4].
    dll_find_result f0;  // Find result.
    assert_true(doubly_linked_list_index_of(&lst, 0, &f0), "index_of should succeed");  // Find 0.
    assert_eq_ll(0, f0.index, "index_of(0) should return 0");  // Validate index.
    assert_eq_ll(0, f0.cost.hops, "finding head needs 0 hops");  // Validate cost.
    dll_find_result f3;  // Find result.
    assert_true(doubly_linked_list_index_of(&lst, 3, &f3), "index_of should succeed");  // Find 3.
    assert_eq_ll(3, f3.index, "index_of(3) should return 3");  // Validate index.
    assert_eq_ll(3, f3.cost.hops, "finding index 3 needs 3 hops");  // Validate cost.
    dll_find_result fn;  // Find result.
    assert_true(doubly_linked_list_index_of(&lst, 999, &fn), "index_of should succeed");  // Search missing.
    assert_eq_ll(-1, fn.index, "index_of(missing) should return -1");  // Validate not found.
    assert_eq_ll(5, fn.cost.hops, "missing value should traverse n hops");  // Validate cost.
    doubly_linked_list_destroy(&lst);  // Clean up.
}  // End test_index_of_with_cost_hops.

static void test_invalid_indices_fail(void) {  // Methods should reject invalid indices and empty pops.
    doubly_linked_list lst;  // Local list.
    assert_true(build_ordered_list(3, &lst), "build_ordered_list should succeed");  // Build [0..2].
    dll_get_result gr;  // Get result.
    assert_true(!doubly_linked_list_get(&lst, 3, &gr), "get should fail on index==size");  // Invalid get.
    dll_cost ic;  // Insert cost.
    assert_true(!doubly_linked_list_insert_at(&lst, 4, 0, &ic), "insert_at should fail on index>size");  // Invalid insert.
    dll_remove_result rr;  // Remove result.
    assert_true(!doubly_linked_list_remove_at(&lst, 3, &rr), "remove_at should fail on index==size");  // Invalid remove.
    doubly_linked_list_destroy(&lst);  // Clean up.

    doubly_linked_list empty;  // Empty list.
    assert_true(doubly_linked_list_init(&empty), "init should succeed");  // Initialize empty list.
    assert_true(!doubly_linked_list_pop_back(&empty, &rr), "pop_back should fail on empty list");  // Invalid pop.
    assert_true(!doubly_linked_list_pop_front(&empty, &rr), "pop_front should fail on empty list");  // Invalid pop.
    doubly_linked_list_destroy(&empty);  // Clean up.
}  // End test_invalid_indices_fail.

int main(void) {  // Run all tests and return non-zero on failure.
    printf("=== DoublyLinkedList Tests (C) ===\n");  // Print header.
    test_push_pop_ends_are_o1();  // Run push/pop O(1) test.
    test_get_with_cost_uses_nearest_end();  // Run get cost test.
    test_insert_at_cost_and_ordering();  // Run insert test.
    test_remove_at_cost_and_ordering();  // Run remove test.
    test_index_of_with_cost_hops();  // Run indexOf test.
    test_invalid_indices_fail();  // Run invalid input tests.
    printf("==============================\n");  // Print separator.
    if (tests_failed == 0) {  // Determine pass/fail.
        printf("All tests PASSED.\n");  // Report success.
        return 0;  // Exit success.
    }  // Close pass branch.
    printf("Tests FAILED: %d\n", tests_failed);  // Report failure count.
    return 1;  // Exit failure.
}  // End main.

