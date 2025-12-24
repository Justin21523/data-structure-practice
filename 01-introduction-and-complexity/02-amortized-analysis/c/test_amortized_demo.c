/* 02 攤銷分析測試（C）/ Tests for amortized analysis (C). */  // Bilingual test file header.
#include "amortized_demo.h"  // Include the API under test.

#include <stdio.h>  // Use printf for human-readable test output.

static int tests_failed = 0;  // Track how many assertions failed.

static void assert_true(int condition, const char* message) {  // Minimal boolean assertion helper.
    if (!condition) {  // Fail when condition is false.
        tests_failed += 1;  // Record one failure.
        printf("FAIL: %s\\n", message);  // Print a clear failure message.
    } else {  // Pass branch for nicer output.
        printf("PASS: %s\\n", message);  // Print pass marker.
    }  // Close branch.
}  // End of assert_true.

static void assert_eq_ll(long long expected, long long actual, const char* message) {  // Minimal long long equality assertion.
    if (expected != actual) {  // Fail when values differ.
        tests_failed += 1;  // Record one failure.
        printf("FAIL: %s (expected=%lld, actual=%lld)\\n", message, expected, actual);  // Print mismatch.
    } else {  // Pass branch for nicer output.
        printf("PASS: %s\\n", message);  // Print pass marker.
    }  // Close branch.
}  // End of assert_eq_ll.

static int is_power_of_two(int x) {  // Local helper for capacity validation.
    return x > 0 && ((x & (x - 1)) == 0);  // Standard power-of-two predicate.
}  // End of is_power_of_two.

static void test_potential_starts_at_zero(void) {  // Φ(0,1) should be 0 with our chosen offset.
    assert_eq_ll(0, amortized_potential(0, 1), "potential(0,1) should be 0");  // Validate Φ offset.
}  // End of test_potential_starts_at_zero.

static void test_capacity_invariants(void) {  // Capacity should be power-of-two and >= max(1,m).
    int ms[] = {0, 1, 2, 3, 4, 5, 6, 8, 16, 33, 100};  // Mix powers-of-two and non-powers-of-two.
    size_t ms_count = sizeof(ms) / sizeof(ms[0]);  // Compute array length.
    for (size_t i = 0; i < ms_count; i++) {  // Iterate test cases.
        int m = ms[i];  // Read m.
        amortized_summary s;  // Summary output.
        assert_true(amortized_simulate_appends(m, &s), "simulate_appends should succeed");  // Ensure simulation succeeds.
        assert_eq_ll(m, s.final_size, "final size should equal m");  // Validate size.
        assert_true(is_power_of_two(s.final_capacity), "capacity should be power of two");  // Validate power-of-two capacity.
        assert_true(s.final_capacity >= (m > 0 ? m : 1), "capacity should be >= max(1,m)");  // Validate capacity bound.
    }  // Close loop.
}  // End of test_capacity_invariants.

static void test_total_copies_equals_final_capacity_minus_one(void) {  // Copies sum to cap-1 under doubling.
    int ms[] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33};  // Cover boundaries around resizes.
    size_t ms_count = sizeof(ms) / sizeof(ms[0]);  // Compute array length.
    for (size_t i = 0; i < ms_count; i++) {  // Iterate test cases.
        int m = ms[i];  // Read m.
        amortized_summary s;  // Summary output.
        assert_true(amortized_simulate_appends(m, &s), "simulate_appends should succeed");  // Ensure simulation succeeds.
        assert_eq_ll((long long)(s.final_capacity - 1), s.total_copies, "total copies should equal finalCapacity - 1");  // Validate copy sum.
    }  // Close loop.
}  // End of test_total_copies_equals_final_capacity_minus_one.

static void test_aggregate_total_cost_is_linear(void) {  // Total actual cost should be <= 3m.
    int ms[] = {0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256};  // Multiple sizes for robustness.
    size_t ms_count = sizeof(ms) / sizeof(ms[0]);  // Compute array length.
    for (size_t i = 0; i < ms_count; i++) {  // Iterate test cases.
        int m = ms[i];  // Read m.
        amortized_summary s;  // Summary output.
        assert_true(amortized_simulate_appends(m, &s), "simulate_appends should succeed");  // Ensure simulation succeeds.
        assert_true(s.total_actual_cost <= (long long)3 * (long long)m, "total actual cost should be <= 3m");  // Aggregate method bound.
    }  // Close loop.
}  // End of test_aggregate_total_cost_is_linear.

static void test_accounting_and_potential_per_step(void) {  // Bank non-negative and amortized cost 3 per step.
    amortized_table table;  // Create a table for step-by-step testing.
    assert_true(amortized_table_init(&table), "table_init should succeed");  // Ensure initialization succeeds.
    for (int i = 0; i < 256; i++) {  // Run enough operations to hit multiple resizes.
        amortized_step step;  // Step record for this append.
        assert_true(amortized_table_append(&table, i, &step), "table_append should succeed");  // Ensure append succeeds.
        assert_true(step.bank_after >= 0, "bank should never be negative");  // Validate accounting invariant.
        assert_eq_ll(3, step.amortized_cost, "amortized cost should be 3 each step");  // Validate potential method result.
    }  // Close loop.
    amortized_table_destroy(&table);  // Clean up resources.
}  // End of test_accounting_and_potential_per_step.

int main(void) {  // Run all tests and return non-zero on failure.
    printf("=== Amortized Analysis Demo Tests (C) ===\\n");  // Print header.

    test_potential_starts_at_zero();  // Run Φ offset test.
    test_capacity_invariants();  // Run capacity invariant tests.
    test_total_copies_equals_final_capacity_minus_one();  // Run copy-sum tests.
    test_aggregate_total_cost_is_linear();  // Run aggregate bound tests.
    test_accounting_and_potential_per_step();  // Run per-step invariants.

    printf("========================================\\n");  // Print separator.
    if (tests_failed == 0) {  // Determine pass/fail from recorded failures.
        printf("All tests PASSED.\\n");  // Report success.
        return 0;  // Exit success.
    }  // Close pass branch.

    printf("Tests FAILED: %d\\n", tests_failed);  // Report failure count.
    return 1;  // Exit failure.
}  // End of test runner.

