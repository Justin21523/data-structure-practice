// 02 攤銷分析測試（Java）/ Tests for amortized analysis (Java).  // Bilingual file header.

import java.util.List;  // Use List for iterating over steps in tests.  

public final class AmortizedDemoTest {  // A minimal test runner (no JUnit/Maven required).  
    private AmortizedDemoTest() {  // Prevent instantiation.  
    }  // Close constructor.  

    private static void assertTrue(boolean condition, String message) {  // Minimal assertion helper for booleans.  
        if (!condition) {  // Fail when condition is false.  
            throw new AssertionError("FAIL: " + message);  // Throw to indicate test failure.  
        }  // Close failure branch.  
    }  // End of assertTrue.  

    private static void assertEquals(long expected, long actual, String message) {  // Minimal assertion helper for longs.  
        if (expected != actual) {  // Fail when values differ.  
            throw new AssertionError("FAIL: " + message + " (expected=" + expected + ", actual=" + actual + ")");  // Throw with mismatch.  
        }  // Close failure branch.  
    }  // End of assertEquals.  

    private static void assertThrows(Class<? extends Throwable> expectedType, Runnable action, String message) {  // Assert that an action throws.  
        try {  // Execute and expect a throw.  
            action.run();  // Run the action.  
        } catch (Throwable t) {  // Catch any throwable so we can validate type.  
            if (expectedType.isInstance(t)) {  // Accept the expected type.  
                return;  // Return early because the test passed.  
            }  // Close type-check branch.  
            throw new AssertionError("FAIL: " + message + " (threw " + t.getClass().getName() + ")");  // Wrong type.  
        }  // Close catch.  
        throw new AssertionError("FAIL: " + message + " (no exception thrown)");  // Fail if nothing was thrown.  
    }  // End of assertThrows.  

    private static boolean isPowerOfTwo(int x) {  // Local helper for capacity validation.  
        return x > 0 && (x & (x - 1)) == 0;  // Standard power-of-two predicate.  
    }  // End of isPowerOfTwo.  

    private static void testPotentialStartsAtZero() {  // Φ(0,1) should be 0 with our chosen offset.  
        assertEquals(0, AmortizedDemo.potential(0, 1), "potential(0,1) should be 0");  // Validate Φ offset.  
    }  // End of testPotentialStartsAtZero.  

    private static void testCapacityIsPowerOfTwoAndGeSize() {  // Validate capacity invariant after simulation.  
        int[] ms = new int[] {0, 1, 2, 3, 4, 5, 6, 8, 16, 33, 100};  // Mix powers-of-two and non-powers-of-two.  
        for (int m : ms) {  // Run each scenario.  
            AmortizedDemo.SimulationSummary s = AmortizedDemo.simulateAppends(m);  // Simulate m appends.  
            assertEquals(m, s.finalSize, "final size should equal m");  // Validate size.  
            assertTrue(isPowerOfTwo(s.finalCapacity), "capacity should be power of two");  // Validate capacity form.  
            assertTrue(s.finalCapacity >= Math.max(1, m), "capacity should be >= max(1,m)");  // Validate capacity bound.  
        }  // Close loop.  
    }  // End of testCapacityIsPowerOfTwoAndGeSize.  

    private static void testTotalCopiesEqualsFinalCapacityMinusOne() {  // Validate the deterministic copy-sum property.  
        int[] ms = new int[] {0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33};  // Cover boundaries around resizes.  
        for (int m : ms) {  // Run each scenario.  
            AmortizedDemo.SimulationSummary s = AmortizedDemo.simulateAppends(m);  // Simulate m appends.  
            assertEquals((long) (s.finalCapacity - 1), s.totalCopies, "total copies should equal finalCapacity - 1");  // Copies sum to cap-1.  
        }  // Close loop.  
    }  // End of testTotalCopiesEqualsFinalCapacityMinusOne.  

    private static void testAggregateTotalCostIsLinear() {  // Validate aggregate bound totalActualCost <= 3m.  
        int[] ms = new int[] {0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256};  // Multiple sizes to catch off-by-one errors.  
        for (int m : ms) {  // Run each scenario.  
            AmortizedDemo.SimulationSummary s = AmortizedDemo.simulateAppends(m);  // Simulate m appends.  
            assertTrue(s.totalActualCost <= (long) 3 * (long) m, "total actual cost should be <= 3m");  // Aggregate method result.  
        }  // Close loop.  
    }  // End of testAggregateTotalCostIsLinear.  

    private static void testAccountingBankNeverNegative() {  // Validate accounting method credit invariant.  
        AmortizedDemo.AmortizedDynamicArray table = new AmortizedDemo.AmortizedDynamicArray();  // Start with empty table.  
        for (int i = 0; i < 256; i++) {  // Run enough operations to hit multiple resizes.  
            AmortizedDemo.AppendStep step = table.append(i);  // Append one value and capture step info.  
            assertTrue(step.bankAfter >= 0, "bank should never be negative");  // Ensure credit stays non-negative.  
        }  // Close loop.  
    }  // End of testAccountingBankNeverNegative.  

    private static void testPotentialMethodAmortizedCostIsThree() {  // Validate potential method amortized cost per op.  
        AmortizedDemo.AmortizedDynamicArray table = new AmortizedDemo.AmortizedDynamicArray();  // Start with empty table.  
        List<AmortizedDemo.AppendStep> steps = table.getSteps();  // Access the internal step list for inspection.  
        for (int i = 0; i < 256; i++) {  // Run enough operations to include many resizes.  
            table.append(i);  // Append one value.  
            AmortizedDemo.AppendStep step = steps.get(steps.size() - 1);  // Read the most recent step record.  
            assertEquals(3, step.amortizedCost, "amortized cost should be 3 for each append");  // Validate constant amortized cost.  
        }  // Close loop.  
    }  // End of testPotentialMethodAmortizedCostIsThree.  

    private static void testPotentialRejectsInvalidCapacity() {  // Validate input validation on potential().  
        assertThrows(IllegalArgumentException.class, () -> AmortizedDemo.potential(0, 0), "potential should reject capacity=0");  // Reject invalid capacity.  
    }  // End of testPotentialRejectsInvalidCapacity.  

    public static void main(String[] args) {  // Execute all tests and print a short status.  
        testPotentialStartsAtZero();  // Run Φ offset test.  
        testPotentialRejectsInvalidCapacity();  // Run invalid-input test.  
        testCapacityIsPowerOfTwoAndGeSize();  // Run capacity invariant test.  
        testTotalCopiesEqualsFinalCapacityMinusOne();  // Run copy-sum test.  
        testAggregateTotalCostIsLinear();  // Run aggregate bound test.  
        testAccountingBankNeverNegative();  // Run accounting method invariant test.  
        testPotentialMethodAmortizedCostIsThree();  // Run potential method per-step test.  
        System.out.println("All tests PASSED.");  // Print success when no AssertionError was thrown.  
    }  // End of main.  
}  // End of AmortizedDemoTest.  

