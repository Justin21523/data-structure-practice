/* 01 漸進符號示範測試（C）/ Tests for asymptotic demo (C). */  // Bilingual test file header.
#include "asymptotic_demo.h"  // Include the API under test.

#include <stdio.h>  // Use printf for human-readable test output.

static int tests_failed = 0;  // Track how many assertions failed so we can set an exit code.

static void assert_eq_ll(long long expected, long long actual, const char* message) {  // Minimal assert helper for long long values.
    if (expected != actual) {  // Fail when values differ.
        tests_failed += 1;  // Increment failure count so we can report at the end.
        printf("FAIL: %s (expected=%lld, actual=%lld)\\n", message, expected, actual);  // Print a clear mismatch message.
    } else {  // Pass branch for nicer output.
        printf("PASS: %s\\n", message);  // Print a pass marker for this assertion.
    }  // Close assertion branch.
}  // End of assert_eq_ll.

static void test_constant_ops_is_constant(void) {  // Verify that the O(1) counter is independent of n.
    assert_eq_ll(count_constant_ops(0), count_constant_ops(10), "O(1) should be constant");  // Compare different inputs.
    assert_eq_ll(3, count_constant_ops(1), "This demo uses exactly 3 operations");  // Confirm the chosen constant.
}  // End of test_constant_ops_is_constant.

static void test_log2_ops_matches_halving_count(void) {  // Verify that the halving loop count matches floor(log2(n)).
    assert_eq_ll(0, count_log2_ops(1), "log2 ops for n=1 should be 0");  // No halving for n=1.
    assert_eq_ll(1, count_log2_ops(2), "log2 ops for n=2 should be 1");  // 2 -> 1.
    assert_eq_ll(3, count_log2_ops(8), "log2 ops for n=8 should be 3");  // 8 -> 4 -> 2 -> 1.
}  // End of test_log2_ops_matches_halving_count.

static void test_log2_ops_rejects_non_positive(void) {  // Verify that invalid inputs return the error sentinel (-1).
    assert_eq_ll(-1, count_log2_ops(0), "log2 ops should reject n=0");  // log2(0) is invalid in this demo.
    assert_eq_ll(-1, count_log2_ops(-1), "log2 ops should reject negative n");  // Negative sizes are invalid.
}  // End of test_log2_ops_rejects_non_positive.

static void test_linear_ops_equals_n(void) {  // Verify that the linear counter returns exactly n operations.
    assert_eq_ll(0, count_linear_ops(0), "linear ops for n=0 should be 0");  // 0 iterations.
    assert_eq_ll(5, count_linear_ops(5), "linear ops for n=5 should be 5");  // 5 iterations.
}  // End of test_linear_ops_equals_n.

static void test_quadratic_ops_equals_n_squared(void) {  // Verify that the quadratic counter returns n^2 operations.
    assert_eq_ll(0, count_quadratic_ops(0), "quadratic ops for n=0 should be 0");  // 0 * 0.
    assert_eq_ll(16, count_quadratic_ops(4), "quadratic ops for n=4 should be 16");  // 4 * 4.
}  // End of test_quadratic_ops_equals_n_squared.

static void test_n_log_n_ops_equals_n_times_log2_n(void) {  // Verify that n log n counter matches the intended pattern for powers of two.
    assert_eq_ll(0, count_n_log2_n_ops(0), "n log n ops for n=0 should be 0");  // Boundary case definition.
    assert_eq_ll(24, count_n_log2_n_ops(8), "n log n ops for n=8 should be 24");  // 8 * log2(8) = 8 * 3.
}  // End of test_n_log_n_ops_equals_n_times_log2_n.

int main(void) {  // Run all tests and return a non-zero exit code on failure.
    printf("=== Asymptotic Demo Tests (C) ===\\n");  // Print a header so output is easy to scan.

    test_constant_ops_is_constant();  // Run the O(1) test case.
    test_log2_ops_matches_halving_count();  // Run the O(log n) test case.
    test_log2_ops_rejects_non_positive();  // Run invalid-input tests for the log demo.
    test_linear_ops_equals_n();  // Run the O(n) test case.
    test_quadratic_ops_equals_n_squared();  // Run the O(n^2) test case.
    test_n_log_n_ops_equals_n_times_log2_n();  // Run the O(n log n) test case.

    printf("================================\\n");  // Print a separator line.
    if (tests_failed == 0) {  // Decide pass/fail based on the number of recorded failures.
        printf("All tests PASSED.\\n");  // Report success when there are no failures.
        return 0;  // Exit with code 0 for success.
    }  // Close pass branch.

    printf("Tests FAILED: %d\\n", tests_failed);  // Report how many failures occurred.
    return 1;  // Exit with non-zero code to signal failure in CI/shell scripts.
}  // End of test runner main.
