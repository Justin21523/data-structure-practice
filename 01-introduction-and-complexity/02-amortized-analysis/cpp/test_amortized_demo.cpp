// 02 攤銷分析測試（C++）/ Tests for amortized analysis (C++).  // Bilingual test file header.
#include "AmortizedDemo.hpp"  // Include the API under test.

#include <iostream>  // Use std::cout for human-readable test output.
#include <stdexcept>  // Use std::runtime_error for test failures.

static void assertTrue(bool condition, const std::string& message) {  // Minimal boolean assertion helper.
    if (!condition) {  // Fail when condition is false.
        throw std::runtime_error("FAIL: " + message);  // Throw to signal test failure.
    }  // Close failure branch.
}  // End of assertTrue.

static void assertEqLL(long long expected, long long actual, const std::string& message) {  // Minimal long long equality assertion.
    if (expected != actual) {  // Fail when values differ.
        throw std::runtime_error("FAIL: " + message + " (expected=" + std::to_string(expected) + ", actual=" + std::to_string(actual) + ")");  // Throw mismatch.
    }  // Close failure branch.
}  // End of assertEqLL.

static void testPotentialStartsAtZero() {  // Φ(0,1) should be 0.
    assertEqLL(0, amortized::potential(0, 1), "potential(0,1) should be 0");  // Validate offset.
}  // End of testPotentialStartsAtZero.

static void testCapacityInvariants() {  // Capacity should be power-of-two and >= max(1,m).
    int ms[] = {0, 1, 2, 3, 4, 5, 6, 8, 16, 33, 100};  // Mix powers-of-two and non-powers-of-two.
    for (int m : ms) {  // Run each test case.
        amortized::SimulationSummary s = amortized::simulateAppends(m);  // Simulate m appends.
        assertEqLL(m, s.finalSize, "final size should equal m");  // Validate size.
        assertTrue(amortized::isPowerOfTwo(s.finalCapacity), "capacity should be power of two");  // Validate capacity form.
        assertTrue(s.finalCapacity >= std::max(1, m), "capacity should be >= max(1,m)");  // Validate capacity bound.
    }  // Close loop.
}  // End of testCapacityInvariants.

static void testTotalCopiesEqualsFinalCapacityMinusOne() {  // Copies sum to cap-1 under doubling.
    int ms[] = {0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33};  // Cover boundaries around resizes.
    for (int m : ms) {  // Run each test case.
        amortized::SimulationSummary s = amortized::simulateAppends(m);  // Simulate m appends.
        assertEqLL(static_cast<long long>(s.finalCapacity - 1), s.totalCopies, "total copies should equal finalCapacity - 1");  // Validate copy sum.
    }  // Close loop.
}  // End of testTotalCopiesEqualsFinalCapacityMinusOne.

static void testAggregateTotalCostIsLinear() {  // Total actual cost should be <= 3m.
    int ms[] = {0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256};  // Multiple sizes for robustness.
    for (int m : ms) {  // Run each test case.
        amortized::SimulationSummary s = amortized::simulateAppends(m);  // Simulate m appends.
        assertTrue(s.totalActualCost <= static_cast<long long>(3) * static_cast<long long>(m), "total actual cost should be <= 3m");  // Aggregate bound.
    }  // Close loop.
}  // End of testAggregateTotalCostIsLinear.

static void testAccountingAndPotentialPerStep() {  // Bank non-negative and amortized cost 3 per step.
    amortized::AmortizedDynamicArray table;  // Create a fresh table.
    for (int i = 0; i < 256; i++) {  // Run enough operations to hit multiple resizes.
        amortized::AppendStep step = table.append(i);  // Append one value and capture step.
        assertTrue(step.bankAfter >= 0, "bank should never be negative");  // Validate accounting invariant.
        assertEqLL(3, step.amortizedCost, "amortized cost should be 3 each step");  // Validate potential-method result.
    }  // Close loop.
}  // End of testAccountingAndPotentialPerStep.

int main() {  // Run all tests and print a short status.
    try {  // Catch exceptions to turn failures into non-zero exit codes.
        std::cout << "=== Amortized Analysis Demo Tests (C++) ===\n";  // Print header.
        testPotentialStartsAtZero();  // Run Φ offset test.
        testCapacityInvariants();  // Run capacity invariant tests.
        testTotalCopiesEqualsFinalCapacityMinusOne();  // Run copy-sum tests.
        testAggregateTotalCostIsLinear();  // Run aggregate bound tests.
        testAccountingAndPotentialPerStep();  // Run per-step invariants.
        std::cout << "All tests PASSED.\n";  // Report success.
        return 0;  // Exit success.
    } catch (const std::exception& ex) {  // Report failure message.
        std::cerr << ex.what() << "\n";  // Print error to stderr.
        return 1;  // Exit failure.
    }  // Close catch.
}  // End of main.

