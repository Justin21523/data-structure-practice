// 02 動態陣列測試（Java）/ Tests for dynamic array (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals for comparing expected arrays.  

public final class DynamicArrayDemoTest {  // Minimal test runner without JUnit/Maven.  
    private DynamicArrayDemoTest() {  // Prevent instantiation.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    private static void assertTrue(boolean condition, String message) {  // Minimal boolean assertion helper.  
        if (!condition) {  // Fail when condition is false.  
            throw new AssertionError("FAIL: " + message);  // Throw to signal test failure.  
        }  // Close failure branch.  
    }  // End assertTrue.  

    private static void assertEquals(long expected, long actual, String message) {  // Minimal long equality assertion helper.  
        if (expected != actual) {  // Fail when values differ.  
            throw new AssertionError("FAIL: " + message + " (expected=" + expected + ", actual=" + actual + ")");  // Throw mismatch.  
        }  // Close failure branch.  
    }  // End assertEquals.  

    private static void assertArrayEquals(int[] expected, int[] actual, String message) {  // Minimal array equality helper.  
        if (!Arrays.equals(expected, actual)) {  // Fail when arrays differ.  
            throw new AssertionError("FAIL: " + message + " (expected=" + Arrays.toString(expected) + ", actual=" + Arrays.toString(actual) + ")");  // Throw mismatch.  
        }  // Close failure branch.  
    }  // End assertArrayEquals.  

    private static void assertThrows(Class<? extends Throwable> expectedType, Runnable action, String message) {  // Assert that an action throws.  
        try {  // Execute action and expect it to throw.  
            action.run();  // Run action.  
        } catch (Throwable t) {  // Catch any throwable.  
            if (expectedType.isInstance(t)) {  // Accept expected type.  
                return;  // Test passed.  
            }  // Close type check.  
            throw new AssertionError("FAIL: " + message + " (threw " + t.getClass().getName() + ")");  // Wrong type.  
        }  // Close catch.  
        throw new AssertionError("FAIL: " + message + " (no exception thrown)");  // Fail if nothing thrown.  
    }  // End assertThrows.  

    private static boolean isPowerOfTwo(int x) {  // Helper for capacity invariant.  
        return x > 0 && (x & (x - 1)) == 0;  // Standard bit trick.  
    }  // End isPowerOfTwo.  

    private static void testCapacityDoublingInvariant() {  // Capacity should be power-of-two and >= size.  
        int[] ms = new int[] {0, 1, 2, 3, 4, 5, 8, 9, 16, 33, 100};  // Mix sizes.  
        for (int m : ms) {  // Iterate test cases.  
            DynamicArrayDemo.AppendSummary s = DynamicArrayDemo.simulateAppends(m);  // Simulate m appends.  
            assertEquals(m, s.finalSize, "final size should equal m");  // Validate size.  
            assertTrue(isPowerOfTwo(s.finalCapacity), "capacity should be power of two");  // Validate capacity form.  
            assertTrue(s.finalCapacity >= Math.max(1, m), "capacity should be >= max(1,m)");  // Validate bound.  
        }  // Close loop.  
    }  // End testCapacityDoublingInvariant.  

    private static void testTotalCopiesEqualsFinalCapacityMinusOne() {  // Copies sum to cap-1 under doubling.  
        int[] ms = new int[] {0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33};  // Cover boundaries.  
        for (int m : ms) {  // Iterate test cases.  
            DynamicArrayDemo.AppendSummary s = DynamicArrayDemo.simulateAppends(m);  // Simulate m appends.  
            assertEquals((long) (s.finalCapacity - 1), s.totalCopies, "total copies should equal finalCapacity - 1");  // Validate copy sum.  
        }  // Close loop.  
    }  // End testTotalCopiesEqualsFinalCapacityMinusOne.  

    private static void testAggregateTotalCostIsLinear() {  // Total actual cost should be <= 3m.  
        int[] ms = new int[] {0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256};  // Multiple sizes.  
        for (int m : ms) {  // Iterate test cases.  
            DynamicArrayDemo.AppendSummary s = DynamicArrayDemo.simulateAppends(m);  // Simulate m appends.  
            assertTrue(s.totalActualCost <= (long) 3 * (long) m, "total actual cost should be <= 3m");  // Validate bound.  
        }  // Close loop.  
    }  // End testAggregateTotalCostIsLinear.  

    private static void testInsertAtShiftsRight() {  // insertAt should shift right and keep ordering.  
        DynamicArrayDemo.DynamicArray a = new DynamicArrayDemo.DynamicArray();  // Create empty array.  
        a.append(1);  // [1]  
        a.append(2);  // [1,2]  
        a.append(3);  // [1,2,3]  
        DynamicArrayDemo.OperationCost cost = a.insertAt(1, 99);  // Insert at index 1.  
        assertEquals(2, cost.moved, "insertAt moved should equal size-index");  // Validate shift count.  
        assertArrayEquals(new int[] {1, 99, 2, 3}, a.toArray(), "insertAt should shift right");  // Validate ordering.  
    }  // End testInsertAtShiftsRight.  

    private static void testRemoveAtShiftsLeft() {  // removeAt should shift left and keep ordering.  
        DynamicArrayDemo.DynamicArray a = new DynamicArrayDemo.DynamicArray();  // Create empty array.  
        a.append(10);  // Fill.  
        a.append(20);  // Fill.  
        a.append(30);  // Fill.  
        a.append(40);  // Fill.  
        DynamicArrayDemo.RemoveResult r = a.removeAt(1);  // Remove 20.  
        assertEquals(20, r.value, "removeAt should return removed value");  // Validate removed value.  
        assertEquals(2, r.cost.moved, "removeAt moved should equal size-index-1");  // Validate shift count.  
        assertArrayEquals(new int[] {10, 30, 40}, a.toArray(), "removeAt should shift left");  // Validate ordering.  
    }  // End testRemoveAtShiftsLeft.  

    private static void testInvalidIndicesThrow() {  // Methods should validate indices.  
        DynamicArrayDemo.DynamicArray a = new DynamicArrayDemo.DynamicArray();  // Create empty array.  
        a.append(1);  // Size is 1.  
        assertThrows(IndexOutOfBoundsException.class, () -> a.get(1), "get should throw on invalid index");  // Invalid get.  
        assertThrows(IndexOutOfBoundsException.class, () -> a.set(-1, 0), "set should throw on invalid index");  // Invalid set.  
        assertThrows(IndexOutOfBoundsException.class, () -> a.insertAt(2, 0), "insertAt should throw when index>size");  // Invalid insert.  
        assertThrows(IndexOutOfBoundsException.class, () -> a.removeAt(1), "removeAt should throw on invalid index");  // Invalid remove.  
    }  // End testInvalidIndicesThrow.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testCapacityDoublingInvariant();  // Run capacity invariant test.  
        testTotalCopiesEqualsFinalCapacityMinusOne();  // Run copy-sum test.  
        testAggregateTotalCostIsLinear();  // Run aggregate bound test.  
        testInsertAtShiftsRight();  // Run insert shift test.  
        testRemoveAtShiftsLeft();  // Run remove shift test.  
        testInvalidIndicesThrow();  // Run index validation tests.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End DynamicArrayDemoTest.  

