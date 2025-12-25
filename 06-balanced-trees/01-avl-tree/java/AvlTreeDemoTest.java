// 01 AVL Tree 測試（Java）/ Tests for AVL tree (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals / Arrays.toString for comparisons and messages.  

public final class AvlTreeDemoTest {  // Minimal test runner without JUnit/Maven.  
    private AvlTreeDemoTest() {  // Prevent instantiation.  
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
        AvlTreeDemo.AvlTree t = AvlTreeDemo.AvlTree.fromValues(new int[] {});  // Build an empty AVL tree.  
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
        AvlTreeDemo.AvlTree t = AvlTreeDemo.AvlTree.fromValues(new int[] {});  // Start from empty tree.  
        assertTrue(t.insert(5), "first insert should succeed");  // Insert 5.  
        assertTrue(!t.insert(5), "duplicate insert should be rejected");  // Duplicate insert.  
        assertTrue(t.contains(5), "contains should find inserted key");  // Validate contains.  
        assertArrayEquals(new int[] {5}, t.inorder(), "inorder should contain one key");  // Validate inorder.  
        assertTrue(t.validate(), "tree should remain valid after duplicate attempt");  // Validate invariants.  
    }  // End testInsertRejectsDuplicates.  

    private static void testRotationsLLRRLRRL() {  // Classic 3-key patterns should produce a perfectly balanced tree.  
        int[][] sequences = new int[][] {  // Define insert sequences for each rotation pattern.  
            new int[] {3, 2, 1},  // LL case.  
            new int[] {1, 2, 3},  // RR case.  
            new int[] {3, 1, 2},  // LR case.  
            new int[] {1, 3, 2},  // RL case.  
        };  // Close sequences array.  

        for (int[] seq : sequences) {  // Check each scenario.  
            AvlTreeDemo.AvlTree t = AvlTreeDemo.AvlTree.fromValues(seq);  // Build AVL tree from sequence.  
            assertTrue(t.validate(), "tree should be valid after inserts");  // Validate AVL invariants.  
            assertArrayEquals(new int[] {1, 2, 3}, t.inorder(), "inorder should be sorted");  // inorder must stay sorted.  
            assertEquals(1, t.height(), "height should be 1 for 3 nodes");  // Height should be 1 in edges.  
        }  // Close loop.  
    }  // End testRotationsLLRRLRRL.  

    private static void testInorderSortedAndHeightBound() {  // AVL should keep height small while preserving sorted inorder.  
        int[] values = new int[] {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic insert order from many textbooks.  
        AvlTreeDemo.AvlTree t = AvlTreeDemo.AvlTree.fromValues(values);  // Build AVL tree by inserting in order.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 7, 8, 10, 13, 14}, t.inorder(), "inorder should be sorted");  // Validate inorder.  
        assertEquals(1, t.min(), "min should be 1");  // Validate min.  
        assertEquals(14, t.max(), "max should be 14");  // Validate max.  
        assertTrue(t.validate(), "tree should satisfy AVL invariants");  // Validate invariants.  
        assertLessEqual(3, t.height(), "for 9 nodes, AVL height (edges) must be <= 3");  // Bound check.  
    }  // End testInorderSortedAndHeightBound.  

    private static void testDeleteCasesAndRootDelete() {  // Delete should rebalance and keep inorder sorted.  
        int[] values = new int[] {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Build a tree with multiple delete cases.  
        AvlTreeDemo.AvlTree t = AvlTreeDemo.AvlTree.fromValues(values);  // Build AVL tree.  
        assertTrue(t.validate(), "initial tree should be valid");  // Sanity-check initial validity.  

        assertTrue(t.delete(7), "delete leaf (7) should succeed");  // Delete a leaf node.  
        assertTrue(!t.contains(7), "deleted key (7) should be missing");  // Validate deletion.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 8, 10, 13, 14}, t.inorder(), "inorder after deleting 7 should match");  // Validate inorder.  
        assertTrue(t.validate(), "tree should remain valid after deleting 7");  // Validate invariants.  

        assertTrue(t.delete(14), "delete one-child node (14) should succeed");  // Delete one-child node.  
        assertTrue(!t.contains(14), "deleted key (14) should be missing");  // Validate deletion.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 14 should match");  // Validate inorder.  
        assertTrue(t.validate(), "tree should remain valid after deleting 14");  // Validate invariants.  

        assertTrue(t.delete(3), "delete two-child node (3) should succeed");  // Delete two-child node.  
        assertTrue(!t.contains(3), "deleted key (3) should be missing");  // Validate deletion.  
        assertArrayEquals(new int[] {1, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 3 should match");  // Validate inorder.  
        assertTrue(t.validate(), "tree should remain valid after deleting 3");  // Validate invariants.  
        assertLessEqual(2, t.height(), "with 6 nodes, AVL height (edges) must be <= 2");  // Bound check.  

        AvlTreeDemo.AvlTree t2 = AvlTreeDemo.AvlTree.fromValues(values);  // Build a fresh tree for root deletion test.  
        assertTrue(t2.delete(8), "delete root (8) should succeed");  // Delete the original root.  
        assertTrue(!t2.contains(8), "deleted root key (8) should be missing");  // Validate deletion.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 7, 10, 13, 14}, t2.inorder(), "inorder should remain sorted without 8");  // Validate inorder.  
        assertTrue(t2.validate(), "tree should remain valid after root deletion");  // Validate invariants.  
    }  // End testDeleteCasesAndRootDelete.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testEmptyTreeProperties();  // Run empty tree tests.  
        testInsertRejectsDuplicates();  // Run duplicate insert tests.  
        testRotationsLLRRLRRL();  // Run rotation-pattern tests.  
        testInorderSortedAndHeightBound();  // Run inorder/height bound tests.  
        testDeleteCasesAndRootDelete();  // Run delete tests.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End AvlTreeDemoTest.  

