// 01 漸進符號示範測試（C++）/ Tests for asymptotic demo (C++).  // Bilingual test file header.
#include "AsymptoticDemo.hpp"  // Include the functions under test.

#include <cstdlib>  // Provide exit codes for the test runner.
#include <iostream>  // Provide std::cout for readable test output.
#include <stdexcept>  // Provide std::runtime_error for failing assertions.

static void assertEquals(long long expected, long long actual, const char* message) {  // Minimal assertion helper for this repo.
    if (expected != actual) {  // Fail when values differ.
        throw std::runtime_error(std::string(message) + " (expected=" + std::to_string(expected) + ", actual=" + std::to_string(actual) + ")");  // Provide a detailed error.
    }  // Close assertion block.
}  // End of assertEquals.

static void testConstantOpsIsConstant() {  // Verify that the O(1) counter is independent of n.
    assertEquals(asymptotic::countConstantOps(0), asymptotic::countConstantOps(10), "O(1) should be constant");  // Compare different inputs.
    assertEquals(3, asymptotic::countConstantOps(1), "This demo uses exactly 3 operations");  // Confirm the chosen constant.
}  // End testConstantOpsIsConstant.

static void testLog2OpsMatchesHalvingCount() {  // Verify that the halving loop count matches floor(log2(n)).
    assertEquals(0, asymptotic::countLog2Ops(1), "log2 ops for n=1 should be 0");  // No halving for n=1.
    assertEquals(1, asymptotic::countLog2Ops(2), "log2 ops for n=2 should be 1");  // 2 -> 1.
    assertEquals(3, asymptotic::countLog2Ops(8), "log2 ops for n=8 should be 3");  // 8 -> 4 -> 2 -> 1.
}  // End testLog2OpsMatchesHalvingCount.

static void testLog2OpsRejectsNonPositive() {  // Verify that invalid inputs throw for the log counter.
    bool threw = false;  // Track whether an exception was thrown for invalid input.
    try {  // Attempt to call the function with an invalid value.
        (void)asymptotic::countLog2Ops(0);  // Trigger validation for n < 1.
    } catch (const std::invalid_argument&) {  // Accept invalid_argument for this validation.
        threw = true;  // Record that the expected exception occurred.
    }  // End try/catch.
    if (!threw) {  // Fail when no exception is thrown.
        throw std::runtime_error("countLog2Ops should throw for n=0");  // Provide a clear failure message.
    }  // Close failure block.
}  // End testLog2OpsRejectsNonPositive.

static void testLinearOpsEqualsN() {  // Verify that the linear counter returns exactly n operations.
    assertEquals(0, asymptotic::countLinearOps(0), "linear ops for n=0 should be 0");  // 0 iterations.
    assertEquals(5, asymptotic::countLinearOps(5), "linear ops for n=5 should be 5");  // 5 iterations.
}  // End testLinearOpsEqualsN.

static void testQuadraticOpsEqualsNSquared() {  // Verify that the quadratic counter returns n^2 operations.
    assertEquals(0, asymptotic::countQuadraticOps(0), "quadratic ops for n=0 should be 0");  // 0 * 0.
    assertEquals(16, asymptotic::countQuadraticOps(4), "quadratic ops for n=4 should be 16");  // 4 * 4.
}  // End testQuadraticOpsEqualsNSquared.

static void testNLogNOpsEqualsNTimesLog2N() {  // Verify that n log n counter matches the intended model for powers of two.
    assertEquals(0, asymptotic::countNLog2NOps(0), "n log n ops for n=0 should be 0");  // Boundary case.
    assertEquals(24, asymptotic::countNLog2NOps(8), "n log n ops for n=8 should be 24");  // 8 * log2(8) = 24.
}  // End testNLogNOpsEqualsNTimesLog2N.

int main() {  // Run all tests and return a non-zero exit code on failure.
    try {  // Catch exceptions so we can print a single summary message.
        testConstantOpsIsConstant();  // Run the O(1) test case.
        testLog2OpsMatchesHalvingCount();  // Run the O(log n) test case.
        testLog2OpsRejectsNonPositive();  // Run invalid-input tests for the log demo.
        testLinearOpsEqualsN();  // Run the O(n) test case.
        testQuadraticOpsEqualsNSquared();  // Run the O(n^2) test case.
        testNLogNOpsEqualsNTimesLog2N();  // Run the O(n log n) test case.
        std::cout << "All tests PASSED.\n";  // Print a success message when everything passes.
        return 0;  // Exit with 0 for success.
    } catch (const std::exception& ex) {  // Report any failure with a readable message.
        std::cout << "Test FAILED: " << ex.what() << "\n";  // Print the failure reason.
        return 1;  // Exit with non-zero code to signal failure.
    }  // End try/catch.
}  // End of test runner main.

