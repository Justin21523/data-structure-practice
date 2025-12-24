/** 01 漸進符號示範測試（Java）/ Tests for asymptotic demo (Java). */  // Bilingual file header for tests.
public final class AsymptoticDemoTest {  // Declare a tiny test runner without external libraries.
    private AsymptoticDemoTest() {  // Prevent instantiation because this class is a test harness.
        // No instances.  // Explain why the constructor is private.
    }  // Close the constructor block.

    private static void assertEquals(long expected, long actual, String message) {  // Assert helper for numeric equality.
        if (expected != actual) {  // Fail when the values differ.
            throw new AssertionError(message + " (expected=" + expected + ", actual=" + actual + ")");  // Report a clear mismatch message.
        }  // Close the assertion block.
    }  // End of assertEquals.

    private static void assertThrows(Class<? extends Throwable> expectedType, Runnable action, String message) {  // Assert helper for exception behavior.
        try {  // Execute the action and ensure it throws.
            action.run();  // Run the action that is expected to fail.
        } catch (Throwable ex) {  // Capture any thrown exception so we can verify its type.
            if (expectedType.isInstance(ex)) {  // Accept when the thrown type matches the expected type.
                return;  // Return early because the test passed.
            }  // Close the type-check block.
            throw new AssertionError(message + " (wrong exception type: " + ex.getClass().getName() + ")");  // Fail if the exception type is wrong.
        }  // End of catch block.
        throw new AssertionError(message + " (no exception was thrown)");  // Fail if the action did not throw at all.
    }  // End of assertThrows.

    private static void testConstantOpsIsConstant() {  // Verify that the O(1) counter is independent of n.
        assertEquals(AsymptoticDemo.countConstantOps(0), AsymptoticDemo.countConstantOps(10), "O(1) should be constant");  // Compare different inputs.
        assertEquals(3, AsymptoticDemo.countConstantOps(1), "This demo uses exactly 3 operations");  // Confirm the chosen constant.
    }  // End of testConstantOpsIsConstant.

    private static void testLog2OpsMatchesHalvingCount() {  // Verify that the halving loop counts floor(log2(n)).
        assertEquals(0, AsymptoticDemo.countLog2Ops(1), "log2 ops for n=1 should be 0");  // No halving for n=1.
        assertEquals(1, AsymptoticDemo.countLog2Ops(2), "log2 ops for n=2 should be 1");  // 2 -> 1.
        assertEquals(3, AsymptoticDemo.countLog2Ops(8), "log2 ops for n=8 should be 3");  // 8 -> 4 -> 2 -> 1.
    }  // End of testLog2OpsMatchesHalvingCount.

    private static void testLog2OpsRejectsNonPositive() {  // Verify that invalid inputs are rejected for the log demo.
        assertThrows(IllegalArgumentException.class, () -> AsymptoticDemo.countLog2Ops(0), "log2 ops should reject n=0");  // log2(0) is not accepted here.
        assertThrows(IllegalArgumentException.class, () -> AsymptoticDemo.countLog2Ops(-1), "log2 ops should reject negative n");  // Negative sizes are invalid.
    }  // End of testLog2OpsRejectsNonPositive.

    private static void testLinearOpsEqualsN() {  // Verify that the linear counter returns exactly n operations.
        assertEquals(0, AsymptoticDemo.countLinearOps(0), "linear ops for n=0 should be 0");  // 0 iterations.
        assertEquals(5, AsymptoticDemo.countLinearOps(5), "linear ops for n=5 should be 5");  // 5 iterations.
    }  // End of testLinearOpsEqualsN.

    private static void testQuadraticOpsEqualsNSquared() {  // Verify that the quadratic counter returns n^2 operations.
        assertEquals(0, AsymptoticDemo.countQuadraticOps(0), "quadratic ops for n=0 should be 0");  // 0 * 0.
        assertEquals(16, AsymptoticDemo.countQuadraticOps(4), "quadratic ops for n=4 should be 16");  // 4 * 4.
    }  // End of testQuadraticOpsEqualsNSquared.

    private static void testNLogNOpsEqualsNTimesLog2N() {  // Verify that the n log n counter matches n * floor(log2(n)) for powers of two.
        assertEquals(0, AsymptoticDemo.countNLog2NOps(0), "n log n ops for n=0 should be 0");  // Boundary case definition.
        assertEquals(24, AsymptoticDemo.countNLog2NOps(8), "n log n ops for n=8 should be 24");  // 8 * log2(8) = 8 * 3.
    }  // End of testNLogNOpsEqualsNTimesLog2N.

    public static void main(String[] args) {  // Execute all tests as a simple CLI program.
        testConstantOpsIsConstant();  // Run the O(1) test case.
        testLog2OpsMatchesHalvingCount();  // Run the O(log n) test case.
        testLog2OpsRejectsNonPositive();  // Run invalid-input tests for the log demo.
        testLinearOpsEqualsN();  // Run the O(n) test case.
        testQuadraticOpsEqualsNSquared();  // Run the O(n^2) test case.
        testNLogNOpsEqualsNTimesLog2N();  // Run the O(n log n) test case.
        System.out.println("All tests PASSED.");  // Print a success message when no assertions fail.
    }  // End of test runner main.
}  // End of AsymptoticDemoTest class.

