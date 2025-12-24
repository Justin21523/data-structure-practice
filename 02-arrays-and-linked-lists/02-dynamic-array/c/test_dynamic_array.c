/* 02 動態陣列測試（C）/ Tests for dynamic array (C). */  // Bilingual test file header.
#include "dynamic_array.h"  // Include the API under test.

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

static int is_power_of_two(int x) {  // Helper for capacity invariant.
    return x > 0 && ((x & (x - 1)) == 0);  // Standard bit trick.
}  // End is_power_of_two.

typedef struct append_summary {  // Summarize m appends for tests.
    int m;  // Number of operations.
    int final_size;  // Final size.
    int final_capacity;  // Final capacity.
    long long total_copies;  // Total copies.
    long long total_actual_cost;  // Total cost (writes + copies).
} append_summary;  // End summary struct.

static bool simulate_appends(int m, append_summary* out_summary) {  // Simulate m appends and fill summary.
    if (!out_summary) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (m < 0) {  // Reject invalid counts.
        return false;  // Signal invalid input.
    }  // Close validation.

    dynamic_array a;  // Fresh array.
    if (!dynamic_array_init(&a)) {  // Initialize.
        return false;  // Signal allocation failure.
    }  // Close init failure.

    long long total_actual = 0;  // Accumulate total cost.
    for (int i = 0; i < m; i++) {  // Perform m appends.
        dynamic_array_cost cost;  // Per-op cost.
        if (!dynamic_array_append(&a, i, &cost)) {  // Append deterministic value.
            dynamic_array_destroy(&a);  // Clean up.
            return false;  // Signal failure.
        }  // Close failure check.
        total_actual += 1 + (long long)cost.copied;  // Add write + copy cost.
    }  // Close loop.

    out_summary->m = m;  // Store m.
    out_summary->final_size = a.size;  // Store size.
    out_summary->final_capacity = a.capacity;  // Store capacity.
    out_summary->total_copies = a.total_copies;  // Store copies.
    out_summary->total_actual_cost = total_actual;  // Store total cost.
    dynamic_array_destroy(&a);  // Clean up.
    return true;  // Signal success.
}  // End simulate_appends.

static void test_capacity_invariant(void) {  // Capacity should be power-of-two and >= size.
    int ms[] = {0, 1, 2, 3, 4, 5, 8, 9, 16, 33, 100};  // Mix sizes.
    size_t ms_count = sizeof(ms) / sizeof(ms[0]);  // Array length.
    for (size_t i = 0; i < ms_count; i++) {  // Iterate test cases.
        append_summary s;  // Summary struct.
        assert_true(simulate_appends(ms[i], &s), "simulate_appends should succeed");  // Ensure simulation succeeds.
        assert_eq_ll(ms[i], s.final_size, "final size should equal m");  // Validate size.
        assert_true(is_power_of_two(s.final_capacity), "capacity should be power of two");  // Validate capacity form.
        assert_true(s.final_capacity >= (ms[i] > 0 ? ms[i] : 1), "capacity should be >= max(1,m)");  // Validate bound.
    }  // Close loop.
}  // End test_capacity_invariant.

static void test_total_copies_equals_final_capacity_minus_one(void) {  // Copies sum to cap-1 under doubling.
    int ms[] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33};  // Cover boundaries.
    size_t ms_count = sizeof(ms) / sizeof(ms[0]);  // Array length.
    for (size_t i = 0; i < ms_count; i++) {  // Iterate test cases.
        append_summary s;  // Summary struct.
        assert_true(simulate_appends(ms[i], &s), "simulate_appends should succeed");  // Ensure simulation succeeds.
        assert_eq_ll((long long)(s.final_capacity - 1), s.total_copies, "total copies should equal finalCapacity - 1");  // Validate copy sum.
    }  // Close loop.
}  // End test_total_copies_equals_final_capacity_minus_one.

static void test_aggregate_total_cost_is_linear(void) {  // Total actual cost should be <= 3m.
    int ms[] = {0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256};  // Multiple sizes.
    size_t ms_count = sizeof(ms) / sizeof(ms[0]);  // Array length.
    for (size_t i = 0; i < ms_count; i++) {  // Iterate test cases.
        append_summary s;  // Summary struct.
        assert_true(simulate_appends(ms[i], &s), "simulate_appends should succeed");  // Ensure simulation succeeds.
        assert_true(s.total_actual_cost <= (long long)3 * (long long)s.m, "total actual cost should be <= 3m");  // Validate bound.
    }  // Close loop.
}  // End test_aggregate_total_cost_is_linear.

static void test_insert_at_shifts_right(void) {  // insertAt should shift right and keep ordering.
    dynamic_array a;  // Local array.
    assert_true(dynamic_array_init(&a), "init should succeed");  // Initialize.
    dynamic_array_cost cost;  // Per-op cost.
    dynamic_array_append(&a, 1, &cost);  // [1]
    dynamic_array_append(&a, 2, &cost);  // [1,2]
    dynamic_array_append(&a, 3, &cost);  // [1,2,3]
    assert_true(dynamic_array_insert_at(&a, 1, 99, &cost), "insert_at should succeed");  // Insert at index 1.
    assert_eq_ll(2, cost.moved, "insert_at moved should equal size-index");  // Validate moved count.
    int v0 = 0;  // Read index 0.
    int v1 = 0;  // Read index 1.
    int v2 = 0;  // Read index 2.
    int v3 = 0;  // Read index 3.
    dynamic_array_get(&a, 0, &v0);  // Read.
    dynamic_array_get(&a, 1, &v1);  // Read.
    dynamic_array_get(&a, 2, &v2);  // Read.
    dynamic_array_get(&a, 3, &v3);  // Read.
    assert_eq_ll(1, v0, "after insert, index 0 should be 1");  // Validate.
    assert_eq_ll(99, v1, "after insert, index 1 should be 99");  // Validate.
    assert_eq_ll(2, v2, "after insert, index 2 should be 2");  // Validate.
    assert_eq_ll(3, v3, "after insert, index 3 should be 3");  // Validate.
    dynamic_array_destroy(&a);  // Clean up.
}  // End test_insert_at_shifts_right.

static void test_remove_at_shifts_left(void) {  // removeAt should shift left and keep ordering.
    dynamic_array a;  // Local array.
    assert_true(dynamic_array_init(&a), "init should succeed");  // Initialize.
    dynamic_array_cost cost;  // Per-op cost.
    dynamic_array_append(&a, 10, &cost);  // Fill.
    dynamic_array_append(&a, 20, &cost);  // Fill.
    dynamic_array_append(&a, 30, &cost);  // Fill.
    dynamic_array_append(&a, 40, &cost);  // Fill.
    dynamic_array_remove_result r;  // Remove result.
    assert_true(dynamic_array_remove_at(&a, 1, &r), "remove_at should succeed");  // Remove index 1.
    assert_eq_ll(20, r.value, "remove_at should return removed value");  // Validate removed value.
    assert_eq_ll(2, r.cost.moved, "remove_at moved should equal size-index-1");  // Validate moved count.
    int v0 = 0;  // Read index 0.
    int v1 = 0;  // Read index 1.
    int v2 = 0;  // Read index 2.
    dynamic_array_get(&a, 0, &v0);  // Read.
    dynamic_array_get(&a, 1, &v1);  // Read.
    dynamic_array_get(&a, 2, &v2);  // Read.
    assert_eq_ll(10, v0, "after remove, index 0 should be 10");  // Validate.
    assert_eq_ll(30, v1, "after remove, index 1 should be 30");  // Validate.
    assert_eq_ll(40, v2, "after remove, index 2 should be 40");  // Validate.
    dynamic_array_destroy(&a);  // Clean up.
}  // End test_remove_at_shifts_left.

static void test_invalid_indices_fail(void) {  // Methods should reject invalid indices.
    dynamic_array a;  // Local array.
    assert_true(dynamic_array_init(&a), "init should succeed");  // Initialize.
    dynamic_array_cost cost;  // Per-op cost.
    dynamic_array_append(&a, 1, &cost);  // Size is 1.
    int v = 0;  // Temp value.
    assert_true(!dynamic_array_get(&a, 1, &v), "get should fail on invalid index");  // Invalid get.
    assert_true(!dynamic_array_set(&a, -1, 0), "set should fail on invalid index");  // Invalid set.
    assert_true(!dynamic_array_insert_at(&a, 2, 0, &cost), "insert_at should fail when index>size");  // Invalid insert.
    dynamic_array_remove_result r;  // Temp remove result.
    assert_true(!dynamic_array_remove_at(&a, 1, &r), "remove_at should fail on invalid index");  // Invalid remove.
    dynamic_array_destroy(&a);  // Clean up.
}  // End test_invalid_indices_fail.

int main(void) {  // Run all tests and return non-zero on failure.
    printf("=== DynamicArray Tests (C) ===\n");  // Print header.
    test_capacity_invariant();  // Run capacity invariant test.
    test_total_copies_equals_final_capacity_minus_one();  // Run copy-sum test.
    test_aggregate_total_cost_is_linear();  // Run aggregate bound test.
    test_insert_at_shifts_right();  // Run insert shift test.
    test_remove_at_shifts_left();  // Run remove shift test.
    test_invalid_indices_fail();  // Run invalid index tests.
    printf("==============================\n");  // Print separator.
    if (tests_failed == 0) {  // Determine pass/fail.
        printf("All tests PASSED.\n");  // Report success.
        return 0;  // Exit success.
    }  // Close pass branch.
    printf("Tests FAILED: %d\n", tests_failed);  // Report failure count.
    return 1;  // Exit failure.
}  // End main.
