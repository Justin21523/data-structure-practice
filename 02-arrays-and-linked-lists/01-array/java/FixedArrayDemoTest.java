// 01 靜態陣列測試（Java）/ Tests for static array (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals for comparing expected arrays.  

public final class FixedArrayDemoTest {  // Minimal test runner without JUnit/Maven.  
    private FixedArrayDemoTest() {  // Prevent instantiation.  
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
        } catch (Throwable t) {  // Catch any throwable so we can verify type.  
            if (expectedType.isInstance(t)) {  // Accept expected exception type.  
                return;  // Test passed.  
            }  // Close type check.  
            throw new AssertionError("FAIL: " + message + " (threw " + t.getClass().getName() + ")");  // Wrong type.  
        }  // Close catch.  
        throw new AssertionError("FAIL: " + message + " (no exception thrown)");  // Fail if nothing thrown.  
    }  // End assertThrows.  

    private static void testAppendAndGet() {  // append should place values in order and return moved=0.  
        FixedArrayDemo.FixedArray a = new FixedArrayDemo.FixedArray(3);  // Create small fixed array.  
        assertEquals(0, a.append(10), "append moved should be 0");  // Append at end moves 0 elements.  
        assertEquals(0, a.append(20), "append moved should be 0");  // Append at end moves 0 elements.  
        assertEquals(10, a.get(0), "get(0) should be 10");  // Validate first element.  
        assertEquals(20, a.get(1), "get(1) should be 20");  // Validate second element.  
        assertArrayEquals(new int[] {10, 20}, a.toArray(), "toArray should match inserted values");  // Validate toArray.  
    }  // End testAppendAndGet.  

    private static void testSetUpdatesValue() {  // set should update in-place.  
        FixedArrayDemo.FixedArray a = new FixedArrayDemo.FixedArray(2);  // Create array.  
        a.append(1);  // Fill index 0.  
        a.append(2);  // Fill index 1.  
        a.set(1, 99);  // Update value at index 1.  
        assertArrayEquals(new int[] {1, 99}, a.toArray(), "set should update element");  // Validate content.  
    }  // End testSetUpdatesValue.  

    private static void testInsertAtShiftsRightAndCountsMoves() {  // insertAt should shift correctly and count moves.  
        FixedArrayDemo.FixedArray a = new FixedArrayDemo.FixedArray(5);  // Create capacity for insertion.  
        a.append(1);  // [1]  
        a.append(2);  // [1,2]  
        a.append(3);  // [1,2,3]  
        int moved = a.insertAt(1, 99);  // Insert at index 1; should move 2 elements.  
        assertEquals(2, moved, "insertAt moved should equal size-index");  // Validate moved count.  
        assertArrayEquals(new int[] {1, 99, 2, 3}, a.toArray(), "insertAt should shift right");  // Validate ordering.  
    }  // End testInsertAtShiftsRightAndCountsMoves.  

    private static void testRemoveAtShiftsLeftAndCountsMoves() {  // removeAt should shift correctly and count moves.  
        FixedArrayDemo.FixedArray a = new FixedArrayDemo.FixedArray(4);  // Create array.  
        a.append(10);  // Fill.  
        a.append(20);  // Fill.  
        a.append(30);  // Fill.  
        a.append(40);  // Fill.  
        FixedArrayDemo.RemoveResult result = a.removeAt(1);  // Remove 20; should move 2 elements.  
        assertEquals(20, result.value, "removed value should be returned");  // Validate removed value.  
        assertEquals(2, result.moved, "removeAt moved should equal size-index-1");  // Validate moved count.  
        assertArrayEquals(new int[] {10, 30, 40}, a.toArray(), "removeAt should shift left");  // Validate ordering.  
    }  // End testRemoveAtShiftsLeftAndCountsMoves.  

    private static void testIndexOf() {  // indexOf should find first match or return -1.  
        FixedArrayDemo.FixedArray a = new FixedArrayDemo.FixedArray(5);  // Create array.  
        a.append(7);  // [7]  
        a.append(8);  // [7,8]  
        a.append(7);  // [7,8,7]  
        assertEquals(0, a.indexOf(7), "indexOf should return first occurrence");  // Validate first match.  
        assertEquals(1, a.indexOf(8), "indexOf should return correct index");  // Validate.  
        assertEquals(-1, a.indexOf(999), "indexOf should return -1 when not found");  // Validate not found.  
    }  // End testIndexOf.  

    private static void testOverflowRaises() {  // append should fail when array is full.  
        FixedArrayDemo.FixedArray a = new FixedArrayDemo.FixedArray(1);  // Capacity 1.  
        a.append(1);  // Fill.  
        assertThrows(IllegalStateException.class, () -> a.append(2), "append should throw when full");  // Expect overflow.  
    }  // End testOverflowRaises.  

    private static void testInvalidIndicesRaise() {  // Methods should validate indices.  
        FixedArrayDemo.FixedArray a = new FixedArrayDemo.FixedArray(2);  // Create array.  
        a.append(1);  // Size is 1.  
        assertThrows(IndexOutOfBoundsException.class, () -> a.get(1), "get should throw on invalid index");  // Invalid access.  
        assertThrows(IndexOutOfBoundsException.class, () -> a.set(-1, 0), "set should throw on invalid index");  // Invalid access.  
        assertThrows(IndexOutOfBoundsException.class, () -> a.insertAt(2, 0), "insertAt should throw when index>size");  // Invalid insert.  
        assertThrows(IndexOutOfBoundsException.class, () -> a.removeAt(1), "removeAt should throw on invalid index");  // Invalid remove.  
    }  // End testInvalidIndicesRaise.  

    private static void testSimulationFormulas() {  // Simulations should match known formulas.  
        assertEquals(0, FixedArrayDemo.simulateInsertMoves(0, 0), "insert into empty moves 0");  // n=0 boundary.  
        assertEquals(4, FixedArrayDemo.simulateInsertMoves(4, 0), "insert at head moves n");  // head insert.  
        assertEquals(0, FixedArrayDemo.simulateInsertMoves(4, 4), "insert at tail moves 0");  // tail insert.  
        assertEquals(3, FixedArrayDemo.simulateRemoveMoves(4, 0), "remove at head moves n-1");  // head remove.  
        assertEquals(0, FixedArrayDemo.simulateRemoveMoves(4, 3), "remove at tail moves 0");  // tail remove.  
    }  // End testSimulationFormulas.  

    public static void main(String[] args) {  // Run all tests and print a short status.  
        testAppendAndGet();  // Run append/get test.  
        testSetUpdatesValue();  // Run set test.  
        testInsertAtShiftsRightAndCountsMoves();  // Run insert test.  
        testRemoveAtShiftsLeftAndCountsMoves();  // Run remove test.  
        testIndexOf();  // Run indexOf test.  
        testOverflowRaises();  // Run overflow test.  
        testInvalidIndicesRaise();  // Run index validation tests.  
        testSimulationFormulas();  // Run formula checks.  
        System.out.println("All tests PASSED.");  // Print success message.  
    }  // End main.  
}  // End FixedArrayDemoTest.  

