// 02 Red-Black Tree 測試（Java, LLRB）/ Tests for red-black tree (Java, LLRB).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals / Arrays.toString for comparisons and messages.  

public final class RedBlackTreeDemoTest {  // Minimal test runner without JUnit/Maven.  
    private RedBlackTreeDemoTest() {  // Prevent instantiation.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    private static void assertTrue(boolean condition, String message) {  // Minimal boolean assertion helper.  
        if (!condition) {  // Fail when condition is false.  
            throw new AssertionError("FAIL: " + message);  // Throw to signal test failure.  
        }  // Close failure branch.  
    }  // End assertTrue.  

    private static void assertEquals(int expected, int actual, String message) {  // Minimal int equality assertion helper.  
        if (expected != actual) {  // Fail when values differ.  
            throw new AssertionError("FAIL: " + message + " (expected=" + expected + ", actual=" + actual + ")");  // Throw mismatch.  
        }  // Close failure branch.  
    }  // End assertEquals(int).  

    private static void assertNull(Object actual, String message) {  // Minimal null assertion helper.  
        if (actual != null) {  // Fail when value is non-null.  
            throw new AssertionError("FAIL: " + message + " (actual=" + actual + ")");  // Throw mismatch.  
        }  // Close failure branch.  
    }  // End assertNull.  

    private static void assertArrayEquals(int[] expected, int[] actual, String message) {  // Minimal int[] equality helper.  
        if (!Arrays.equals(expected, actual)) {  // Fail when arrays differ.  
            throw new AssertionError("FAIL: " + message + " (expected=" + Arrays.toString(expected) + ", actual=" + Arrays.toString(actual) + ")");  // Throw mismatch.  
        }  // Close failure branch.  
    }  // End assertArrayEquals.  

    private static void assertLessEqual(int upperBound, int actual, String message) {  // Minimal <= assertion helper.  
        if (actual > upperBound) {  // Fail when value is larger than bound.  
            throw new AssertionError("FAIL: " + message + " (upperBound=" + upperBound + ", actual=" + actual + ")");  // Throw mismatch.  
        }  // Close failure branch.  
    }  // End assertLessEqual.  

    private static void testEmptyTreeProperties() {  // Empty tree should follow our edge-based height definition.  
        RedBlackTreeDemo.RedBlackTree t = RedBlackTreeDemo.RedBlackTree.fromValues(new int[] {});  // Build an empty tree.  
        assertTrue(t.isEmpty(), "tree should be empty");  // Validate emptiness.  
        assertEquals(-1, t.height(), "height should be -1 for empty");  // Validate height definition.  
        assertNull(t.min(), "min should be null for empty");  // Validate min.  
        assertNull(t.max(), "max should be null for empty");  // Validate max.  
        assertArrayEquals(new int[] {}, t.inorder(), "inorder should be empty");  // Validate inorder.  
        assertTrue(t.validate(), "empty tree should be valid");  // Empty tree is valid.  
        assertTrue(!t.contains(1), "contains should be false on empty");  // Validate contains.  
        assertTrue(!t.delete(1), "delete should be false when key is missing");  // Validate delete missing.  
    }  // End testEmptyTreeProperties.  

    private static void testInsertRejectsDuplicates() {  // Insert should reject duplicates by policy.  
        RedBlackTreeDemo.RedBlackTree t = RedBlackTreeDemo.RedBlackTree.fromValues(new int[] {});  // Start from empty tree.  
        assertTrue(t.insert(5), "first insert should succeed");  // Insert 5.  
        assertTrue(!t.insert(5), "duplicate insert should be rejected");  // Duplicate insert.  
        assertTrue(t.contains(5), "contains should find inserted key");  // Validate contains.  
        assertArrayEquals(new int[] {5}, t.inorder(), "inorder should contain one key");  // Validate inorder.  
        assertTrue(t.validate(), "tree should remain valid after duplicate attempt");  // Validate invariants.  
    }  // End testInsertRejectsDuplicates.  

    private static void testAscendingInsertsHeightBound() {  // Ascending inserts should still keep height logarithmic.  
        int[] values = new int[15];  // Allocate 1..15 input array.  
        for (int i = 0; i < 15; i++) {  // Fill array with increasing values.  
            values[i] = i + 1;  // Store (i+1).  
        }  // Close fill loop.  
        RedBlackTreeDemo.RedBlackTree t = RedBlackTreeDemo.RedBlackTree.fromValues(values);  // Build tree by inserting in order.  
        assertArrayEquals(values, t.inorder(), "inorder should be sorted for ascending inserts");  // inorder must stay sorted.  
        assertTrue(t.validate(), "tree should satisfy red-black invariants");  // Validate invariants.  
        assertLessEqual(7, t.height(), "for n=15, red-black height (edges) must be <= 7");  // Height bound check.  
    }  // End testAscendingInsertsHeightBound.  

    private static void testDeleteCasesAndRootDelete() {  // Delete should keep inorder sorted and invariants valid.  
        int[] values = new int[] {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic insert order from many textbooks.  
        RedBlackTreeDemo.RedBlackTree t = RedBlackTreeDemo.RedBlackTree.fromValues(values);  // Build tree by inserting in order.  
        assertTrue(t.validate(), "initial tree should be valid");  // Sanity-check initial validity.  

        assertTrue(!t.delete(999), "delete missing key should return false");  // Deleting missing key should fail.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 7, 8, 10, 13, 14}, t.inorder(), "inorder should be unchanged after missing delete");  // inorder should be unchanged.  
        assertTrue(t.validate(), "tree should remain valid after missing delete");  // Tree should remain valid.  

        assertTrue(t.delete(7), "delete leaf (7) should succeed");  // Delete a leaf node.  
        assertTrue(!t.contains(7), "deleted key (7) should be missing");  // Validate deletion.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 8, 10, 13, 14}, t.inorder(), "inorder after deleting 7 should match");  // Validate inorder.  
        assertTrue(t.validate(), "tree should remain valid after deleting 7");  // Validate invariants.  

        assertTrue(t.delete(14), "delete one-child node (14) should succeed");  // Delete a one-child node in this data set.  
        assertTrue(!t.contains(14), "deleted key (14) should be missing");  // Validate deletion.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 14 should match");  // Validate inorder.  
        assertTrue(t.validate(), "tree should remain valid after deleting 14");  // Validate invariants.  

        assertTrue(t.delete(3), "delete two-child node (3) should succeed");  // Delete a two-child node.  
        assertTrue(!t.contains(3), "deleted key (3) should be missing");  // Validate deletion.  
        assertArrayEquals(new int[] {1, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 3 should match");  // Validate inorder.  
        assertTrue(t.validate(), "tree should remain valid after deleting 3");  // Validate invariants.  

        RedBlackTreeDemo.RedBlackTree t2 = RedBlackTreeDemo.RedBlackTree.fromValues(values);  // Build a fresh tree for root deletion test.  
        assertTrue(t2.delete(8), "delete root (8) should succeed");  // Delete the original root.  
        assertTrue(!t2.contains(8), "deleted root key (8) should be missing");  // Validate deletion.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 7, 10, 13, 14}, t2.inorder(), "inorder should remain sorted without 8");  // Validate inorder.  
        assertTrue(t2.validate(), "tree should remain valid after root deletion");  // Validate invariants.  
    }  // End testDeleteCasesAndRootDelete.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testEmptyTreeProperties();  // Run empty tree tests.  
        testInsertRejectsDuplicates();  // Run duplicate insert tests.  
        testAscendingInsertsHeightBound();  // Run height bound test.  
        testDeleteCasesAndRootDelete();  // Run delete tests.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End RedBlackTreeDemoTest.  

