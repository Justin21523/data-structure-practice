/* 01 靜態陣列測試（C）/ Tests for static array (C). */  // Bilingual test file header.
#include "fixed_array.h"  // Include the API under test.

#include <stdio.h>  // Use printf for human-readable test output.

static int tests_failed = 0;  // Track how many assertions failed.

static void assert_true(int condition, const char* message) {  // Minimal boolean assertion helper.
    if (!condition) {  // Fail when condition is false.
        tests_failed += 1;  // Record one failure.
        printf("FAIL: %s\\n", message);  // Print failure message.
    } else {  // Pass branch.
        printf("PASS: %s\\n", message);  // Print pass marker.
    }  // Close branch.
}  // End of assert_true.

static void assert_eq_int(int expected, int actual, const char* message) {  // Minimal int equality assertion helper.
    if (expected != actual) {  // Fail when values differ.
        tests_failed += 1;  // Record one failure.
        printf("FAIL: %s (expected=%d, actual=%d)\\n", message, expected, actual);  // Print mismatch.
    } else {  // Pass branch.
        printf("PASS: %s\\n", message);  // Print pass marker.
    }  // Close branch.
}  // End of assert_eq_int.

static bool build_filled_array(int n, int extra_capacity, fixed_array* out_array) {  // Build array filled with 0..n-1 for tests.
    if (!out_array) {  // Validate output pointer.
        return false;  // Signal failure.
    }  // Close pointer check.
    if (n < 0 || extra_capacity < 0) {  // Reject invalid parameters.
        return false;  // Signal failure.
    }  // Close validation.
    if (!fixed_array_init(out_array, n + extra_capacity)) {  // Initialize backing buffer.
        return false;  // Signal allocation failure.
    }  // Close init check.
    for (int i = 0; i < n; i++) {  // Fill with deterministic values.
        int moved = 0;  // Append should move 0 elements.
        if (!fixed_array_append(out_array, i, &moved)) {  // Append one value.
            fixed_array_destroy(out_array);  // Clean up on failure.
            return false;  // Signal failure.
        }  // Close append failure branch.
    }  // Close loop.
    return true;  // Signal success.
}  // End build_filled_array.

static void test_append_get_to_array(void) {  // Basic correctness test for append/get.
    fixed_array a;  // Local array for testing.
    assert_true(fixed_array_init(&a, 3), "init should succeed");  // Initialize with capacity 3.
    int moved = -1;  // Placeholder for moved count output.
    assert_true(fixed_array_append(&a, 10, &moved), "append should succeed");  // Append 10.
    assert_eq_int(0, moved, "append moved should be 0");  // Append should move 0.
    assert_true(fixed_array_append(&a, 20, &moved), "append should succeed");  // Append 20.
    assert_eq_int(0, moved, "append moved should be 0");  // Append should move 0.
    int v0 = 0;  // Read value at index 0.
    int v1 = 0;  // Read value at index 1.
    assert_true(fixed_array_get(&a, 0, &v0), "get(0) should succeed");  // Read index 0.
    assert_true(fixed_array_get(&a, 1, &v1), "get(1) should succeed");  // Read index 1.
    assert_eq_int(10, v0, "get(0) should be 10");  // Validate value.
    assert_eq_int(20, v1, "get(1) should be 20");  // Validate value.
    fixed_array_destroy(&a);  // Clean up resources.
}  // End test_append_get_to_array.

static void test_set_updates_value(void) {  // set should update value in-place.
    fixed_array a;  // Local array.
    assert_true(fixed_array_init(&a, 2), "init should succeed");  // Initialize.
    int moved = 0;  // Moved output.
    fixed_array_append(&a, 1, &moved);  // Append 1.
    fixed_array_append(&a, 2, &moved);  // Append 2.
    assert_true(fixed_array_set(&a, 1, 99), "set should succeed");  // Update index 1.
    int v = 0;  // Temp for get.
    assert_true(fixed_array_get(&a, 1, &v), "get should succeed");  // Read updated index.
    assert_eq_int(99, v, "set should update value");  // Validate updated value.
    fixed_array_destroy(&a);  // Clean up.
}  // End test_set_updates_value.

static void test_insert_at_shifts_right_and_counts_moves(void) {  // insertAt should shift right and count moves.
    fixed_array a;  // Local array.
    assert_true(fixed_array_init(&a, 5), "init should succeed");  // Capacity for insertion.
    int moved = 0;  // Moved output.
    fixed_array_append(&a, 1, &moved);  // [1]
    fixed_array_append(&a, 2, &moved);  // [1,2]
    fixed_array_append(&a, 3, &moved);  // [1,2,3]
    assert_true(fixed_array_insert_at(&a, 1, 99, &moved), "insert_at should succeed");  // Insert at index 1.
    assert_eq_int(2, moved, "insert_at moved should equal size-index");  // Validate move count (3-1=2).
    int v0 = 0;  // Read index 0.
    int v1 = 0;  // Read index 1.
    int v2 = 0;  // Read index 2.
    int v3 = 0;  // Read index 3.
    fixed_array_get(&a, 0, &v0);  // Read.
    fixed_array_get(&a, 1, &v1);  // Read.
    fixed_array_get(&a, 2, &v2);  // Read.
    fixed_array_get(&a, 3, &v3);  // Read.
    assert_eq_int(1, v0, "after insert, index 0 should be 1");  // Validate.
    assert_eq_int(99, v1, "after insert, index 1 should be 99");  // Validate.
    assert_eq_int(2, v2, "after insert, index 2 should be 2");  // Validate.
    assert_eq_int(3, v3, "after insert, index 3 should be 3");  // Validate.
    fixed_array_destroy(&a);  // Clean up.
}  // End test_insert_at_shifts_right_and_counts_moves.

static void test_remove_at_shifts_left_and_counts_moves(void) {  // removeAt should shift left and count moves.
    fixed_array a;  // Local array.
    assert_true(fixed_array_init(&a, 4), "init should succeed");  // Capacity 4.
    int moved = 0;  // Moved output.
    fixed_array_append(&a, 10, &moved);  // Fill.
    fixed_array_append(&a, 20, &moved);  // Fill.
    fixed_array_append(&a, 30, &moved);  // Fill.
    fixed_array_append(&a, 40, &moved);  // Fill.
    fixed_array_remove_result result;  // Remove result.
    assert_true(fixed_array_remove_at(&a, 1, &result), "remove_at should succeed");  // Remove index 1.
    assert_eq_int(20, result.value, "removed value should be returned");  // Validate removed value.
    assert_eq_int(2, result.moved, "remove_at moved should equal size-index-1");  // Validate moved count (4-1-1=2).
    int v0 = 0;  // Read index 0.
    int v1 = 0;  // Read index 1.
    int v2 = 0;  // Read index 2.
    fixed_array_get(&a, 0, &v0);  // Read.
    fixed_array_get(&a, 1, &v1);  // Read.
    fixed_array_get(&a, 2, &v2);  // Read.
    assert_eq_int(10, v0, "after remove, index 0 should be 10");  // Validate.
    assert_eq_int(30, v1, "after remove, index 1 should be 30");  // Validate.
    assert_eq_int(40, v2, "after remove, index 2 should be 40");  // Validate.
    fixed_array_destroy(&a);  // Clean up.
}  // End test_remove_at_shifts_left_and_counts_moves.

static void test_index_of(void) {  // index_of should find first match or -1.
    fixed_array a;  // Local array.
    assert_true(fixed_array_init(&a, 5), "init should succeed");  // Initialize.
    int moved = 0;  // Moved output.
    fixed_array_append(&a, 7, &moved);  // [7]
    fixed_array_append(&a, 8, &moved);  // [7,8]
    fixed_array_append(&a, 7, &moved);  // [7,8,7]
    assert_eq_int(0, fixed_array_index_of(&a, 7), "index_of should return first occurrence");  // Validate first match.
    assert_eq_int(1, fixed_array_index_of(&a, 8), "index_of should return correct index");  // Validate.
    assert_eq_int(-1, fixed_array_index_of(&a, 999), "index_of should return -1 when not found");  // Validate not found.
    fixed_array_destroy(&a);  // Clean up.
}  // End test_index_of.

static void test_overflow_and_invalid_indices(void) {  // Validate failure behavior for full array and bad indices.
    fixed_array a;  // Local array.
    assert_true(fixed_array_init(&a, 1), "init should succeed");  // Capacity 1.
    int moved = 0;  // Moved output.
    assert_true(fixed_array_append(&a, 1, &moved), "append should succeed");  // Fill array.
    assert_true(!fixed_array_append(&a, 2, &moved), "append should fail when full");  // Overflow should fail.
    int v = 0;  // Temp for get.
    assert_true(!fixed_array_get(&a, 1, &v), "get should fail on invalid index");  // Invalid get.
    assert_true(!fixed_array_set(&a, -1, 0), "set should fail on invalid index");  // Invalid set.
    assert_true(!fixed_array_insert_at(&a, 2, 0, &moved), "insert_at should fail when index>size");  // Invalid insert.
    fixed_array_remove_result rr;  // Temp remove result.
    assert_true(!fixed_array_remove_at(&a, 1, &rr), "remove_at should fail on invalid index");  // Invalid remove.
    fixed_array_destroy(&a);  // Clean up.
}  // End test_overflow_and_invalid_indices.

static void test_simulation_formulas(void) {  // Validate deterministic move formulas using the real operations.
    fixed_array a;  // Local array for insertion simulation.
    assert_true(build_filled_array(4, 1, &a), "build_filled_array should succeed");  // Build size 4 with extra capacity.
    int moved = 0;  // Moved output.
    assert_true(fixed_array_insert_at(&a, 0, 999, &moved), "insert_at should succeed");  // Insert at head.
    assert_eq_int(4, moved, "insert at head should move n elements");  // Validate formula.
    fixed_array_destroy(&a);  // Clean up.

    assert_true(build_filled_array(4, 0, &a), "build_filled_array should succeed");  // Build size 4.
    fixed_array_remove_result r;  // Remove result.
    assert_true(fixed_array_remove_at(&a, 0, &r), "remove_at should succeed");  // Remove at head.
    assert_eq_int(3, r.moved, "remove at head should move n-1 elements");  // Validate formula.
    fixed_array_destroy(&a);  // Clean up.
}  // End test_simulation_formulas.

int main(void) {  // Run all tests and return non-zero on failure.
    printf("=== FixedArray Tests (C) ===\\n");  // Print header.
    test_append_get_to_array();  // Run append/get test.
    test_set_updates_value();  // Run set test.
    test_insert_at_shifts_right_and_counts_moves();  // Run insert test.
    test_remove_at_shifts_left_and_counts_moves();  // Run remove test.
    test_index_of();  // Run index_of test.
    test_overflow_and_invalid_indices();  // Run overflow/invalid tests.
    test_simulation_formulas();  // Run formula checks.
    printf("============================\\n");  // Print separator.
    if (tests_failed == 0) {  // Determine pass/fail.
        printf("All tests PASSED.\\n");  // Report success.
        return 0;  // Exit success.
    }  // Close pass branch.
    printf("Tests FAILED: %d\\n", tests_failed);  // Report failure count.
    return 1;  // Exit failure.
}  // End main.

