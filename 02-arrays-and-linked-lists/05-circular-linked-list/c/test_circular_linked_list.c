/* 05 環狀鏈結串列測試（C）/ Tests for circular linked list (C). */  // Bilingual test file header.
#include "circular_linked_list.h"  // Include the API under test.

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

static bool build_ordered_list(int n, circular_linked_list* out_list) {  // Build list containing [0..n-1] in order.
    if (!out_list) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (n < 0) {  // Reject invalid sizes.
        return false;  // Signal invalid input.
    }  // Close validation.
    if (!circular_linked_list_init(out_list)) {  // Initialize list.
        return false;  // Signal failure.
    }  // Close init failure.
    for (int v = 0; v < n; v++) {  // Append in increasing order.
        cll_cost cost;  // Per-op cost (ignored).
        if (!circular_linked_list_push_back(out_list, v, &cost)) {  // Append one value.
            circular_linked_list_destroy(out_list);  // Clean up on failure.
            return false;  // Signal failure.
        }  // Close failure check.
    }  // Close loop.
    return true;  // Signal success.
}  // End build_ordered_list.

static void test_push_costs_are_zero(void) {  // push_front/push_back should be O(1) with tail pointer.
    circular_linked_list lst;  // Local list.
    assert_true(circular_linked_list_init(&lst), "init should succeed");  // Initialize empty list.
    cll_cost c1;  // pushBack cost.
    assert_true(circular_linked_list_push_back(&lst, 1, &c1), "push_back should succeed");  // pushBack.
    assert_eq_ll(0, c1.hops, "push_back hops should be 0");  // Validate cost.
    cll_cost c2;  // pushFront cost.
    assert_true(circular_linked_list_push_front(&lst, 0, &c2), "push_front should succeed");  // pushFront.
    assert_eq_ll(0, c2.hops, "push_front hops should be 0");  // Validate cost.
    int buffer[8];  // Snapshot buffer.
    assert_true(circular_linked_list_to_array(&lst, buffer, 8), "to_array should succeed");  // Copy to array.
    assert_eq_ll(0, buffer[0], "after pushFront, head should be 0");  // Validate head.
    assert_eq_ll(1, buffer[1], "after pushBack, second should be 1");  // Validate second.
    circular_linked_list_destroy(&lst);  // Clean up.
}  // End test_push_costs_are_zero.

static void test_pop_front_cost_is_zero(void) {  // pop_front should be O(1).
    circular_linked_list lst;  // Local list.
    assert_true(build_ordered_list(3, &lst), "build_ordered_list should succeed");  // Build [0,1,2].
    cll_remove_result r;  // Remove result.
    assert_true(circular_linked_list_pop_front(&lst, &r), "pop_front should succeed");  // Pop head.
    assert_eq_ll(0, r.value, "pop_front should remove value 0");  // Validate removed value.
    assert_eq_ll(0, r.cost.hops, "pop_front hops should be 0");  // Validate cost.
    int buffer[8];  // Snapshot buffer.
    circular_linked_list_to_array(&lst, buffer, 8);  // Copy to array.
    assert_eq_ll(1, buffer[0], "after pop_front, new head should be 1");  // Validate ordering.
    assert_eq_ll(2, buffer[1], "after pop_front, second should be 2");  // Validate ordering.
    circular_linked_list_destroy(&lst);  // Clean up.
}  // End test_pop_front_cost_is_zero.

static void test_pop_back_cost_is_n_minus_2(void) {  // pop_back requires finding predecessor in singly circular list.
    int ns[] = {1, 2, 3, 5, 10};  // Multiple sizes.
    for (size_t i = 0; i < sizeof(ns) / sizeof(ns[0]); i++) {  // Iterate sizes.
        int n = ns[i];  // Read n.
        circular_linked_list lst;  // Local list.
        assert_true(build_ordered_list(n, &lst), "build_ordered_list should succeed");  // Build [0..n-1].
        cll_remove_result r;  // Remove result.
        assert_true(circular_linked_list_pop_back(&lst, &r), "pop_back should succeed");  // Pop tail.
        assert_eq_ll(n - 1, r.value, "pop_back should remove last value");  // Validate removed value.
        int expected_hops = (n <= 1) ? 0 : (n - 2);  // Predecessor search traverses n-2 links.
        assert_eq_ll(expected_hops, r.cost.hops, "pop_back hops should equal max(0,n-2)");  // Validate cost.
        int buffer[32];  // Snapshot buffer.
        circular_linked_list_to_array(&lst, buffer, 32);  // Copy to array.
        for (int j = 0; j < n - 1; j++) {  // Validate remaining values.
            assert_eq_ll(j, buffer[j], "after pop_back, list should remain 0..n-2");  // Validate element.
        }  // Close loop.
        circular_linked_list_destroy(&lst);  // Clean up.
    }  // Close loop.
}  // End test_pop_back_cost_is_n_minus_2.

static void test_get_with_cost_hops_equals_index(void) {  // get(i) should traverse i links from head.
    circular_linked_list lst;  // Local list.
    assert_true(build_ordered_list(10, &lst), "build_ordered_list should succeed");  // Build [0..9].
    int indices[] = {0, 1, 5, 9};  // Sample indices.
    for (size_t i = 0; i < sizeof(indices) / sizeof(indices[0]); i++) {  // Iterate indices.
        int idx = indices[i];  // Read index.
        cll_get_result r;  // Get result.
        assert_true(circular_linked_list_get(&lst, idx, &r), "get should succeed");  // Fetch.
        assert_eq_ll(idx, r.value, "ordered list value should equal index");  // Validate value.
        assert_eq_ll(idx, r.cost.hops, "get hops should equal index");  // Validate cost.
    }  // Close loop.
    circular_linked_list_destroy(&lst);  // Clean up.
}  // End test_get_with_cost_hops_equals_index.

static void test_rotate_changes_order_and_cost(void) {  // rotate(k) should rotate left by k with hops=k%size.
    int size = 4;  // Fixed size.
    int steps_list[] = {0, 1, 3, 4, 7};  // Steps list.
    int expected_heads[] = {0, 1, 3, 0, 3};  // Expected head values after rotation.
    for (size_t i = 0; i < sizeof(steps_list) / sizeof(steps_list[0]); i++) {  // Iterate cases.
        circular_linked_list lst;  // Local list.
        assert_true(build_ordered_list(size, &lst), "build_ordered_list should succeed");  // Build [0,1,2,3].
        cll_cost cost;  // Cost record.
        assert_true(circular_linked_list_rotate(&lst, steps_list[i], &cost), "rotate should succeed");  // Rotate.
        assert_eq_ll(steps_list[i] % size, cost.hops, "rotate hops should equal steps%size");  // Validate cost.
        int buffer[8];  // Snapshot buffer.
        circular_linked_list_to_array(&lst, buffer, 8);  // Copy to array.
        assert_eq_ll(expected_heads[i], buffer[0], "rotated head should match expected");  // Validate head value.
        circular_linked_list_destroy(&lst);  // Clean up.
    }  // Close loop.
}  // End test_rotate_changes_order_and_cost.

static void test_index_of_with_cost_hops(void) {  // index_of should count traversals up to match or size.
    circular_linked_list lst;  // Local list.
    assert_true(build_ordered_list(5, &lst), "build_ordered_list should succeed");  // Build [0..4].
    cll_find_result f0;  // Find result.
    assert_true(circular_linked_list_index_of(&lst, 0, &f0), "index_of should succeed");  // Find 0.
    assert_eq_ll(0, f0.index, "index_of(0) should return 0");  // Validate index.
    assert_eq_ll(0, f0.cost.hops, "finding head needs 0 hops");  // Validate cost.
    cll_find_result f3;  // Find result.
    assert_true(circular_linked_list_index_of(&lst, 3, &f3), "index_of should succeed");  // Find 3.
    assert_eq_ll(3, f3.index, "index_of(3) should return 3");  // Validate index.
    assert_eq_ll(3, f3.cost.hops, "finding index 3 needs 3 hops");  // Validate cost.
    cll_find_result fn;  // Find result.
    assert_true(circular_linked_list_index_of(&lst, 999, &fn), "index_of should succeed");  // Search missing.
    assert_eq_ll(-1, fn.index, "index_of(missing) should return -1");  // Validate not found.
    assert_eq_ll(5, fn.cost.hops, "missing value should traverse size hops");  // Validate cost.
    circular_linked_list_destroy(&lst);  // Clean up.
}  // End test_index_of_with_cost_hops.

static void test_invalid_indices_and_inputs_fail(void) {  // Methods should reject invalid indices and inputs.
    circular_linked_list lst;  // Local list.
    assert_true(build_ordered_list(3, &lst), "build_ordered_list should succeed");  // Build [0..2].
    cll_get_result gr;  // Get result.
    assert_true(!circular_linked_list_get(&lst, 3, &gr), "get should fail on index==size");  // Invalid get.
    cll_cost rc;  // Rotate cost.
    assert_true(!circular_linked_list_rotate(&lst, -1, &rc), "rotate should fail on negative steps");  // Invalid rotate.
    circular_linked_list_destroy(&lst);  // Clean up.

    circular_linked_list empty;  // Empty list.
    assert_true(circular_linked_list_init(&empty), "init should succeed");  // Initialize empty list.
    cll_remove_result rr;  // Remove result.
    assert_true(!circular_linked_list_pop_front(&empty, &rr), "pop_front should fail on empty list");  // Invalid pop.
    assert_true(!circular_linked_list_pop_back(&empty, &rr), "pop_back should fail on empty list");  // Invalid pop.
    circular_linked_list_destroy(&empty);  // Clean up.
}  // End test_invalid_indices_and_inputs_fail.

int main(void) {  // Run all tests and return non-zero on failure.
    printf("=== CircularLinkedList Tests (C) ===\n");  // Print header.
    test_push_costs_are_zero();  // Run push tests.
    test_pop_front_cost_is_zero();  // Run popFront test.
    test_pop_back_cost_is_n_minus_2();  // Run popBack test.
    test_get_with_cost_hops_equals_index();  // Run get cost test.
    test_rotate_changes_order_and_cost();  // Run rotate test.
    test_index_of_with_cost_hops();  // Run indexOf test.
    test_invalid_indices_and_inputs_fail();  // Run invalid input tests.
    printf("==============================\n");  // Print separator.
    if (tests_failed == 0) {  // Determine pass/fail.
        printf("All tests PASSED.\n");  // Report success.
        return 0;  // Exit success.
    }  // Close pass branch.
    printf("Tests FAILED: %d\n", tests_failed);  // Report failure count.
    return 1;  // Exit failure.
}  // End main.

