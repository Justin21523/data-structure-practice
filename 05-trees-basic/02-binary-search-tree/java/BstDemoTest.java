// 02 二元搜尋樹測試（Java）/ Tests for BST (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals / Arrays.toString for comparisons and messages.  

public final class BstDemoTest {  // Minimal test runner without JUnit/Maven.  
    private BstDemoTest() {  // Prevent instantiation.  
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

    private static void testEmptyTreeProperties() {  // Empty tree should follow our edge-based height definition.  
        BstDemo.BinarySearchTree t = BstDemo.BinarySearchTree.fromValues(new int[] {});  // Build an empty BST.  
        assertTrue(t.isEmpty(), "tree should be empty");  // Validate emptiness.  
        assertEquals(0, t.size(), "size should be 0");  // Validate size.  
        assertEquals(-1, t.height(), "height should be -1 for empty");  // Validate height definition.  
        assertNull(t.min(), "min should be null for empty");  // Validate min.  
        assertNull(t.max(), "max should be null for empty");  // Validate max.  
        assertArrayEquals(new int[] {}, t.inorder(), "inorder should be empty");  // Validate inorder.  
        assertTrue(!t.contains(1), "contains should be false for any key");  // Validate contains.  
        assertTrue(!t.delete(1), "delete should be false when key is missing");  // Validate delete missing.  
    }  // End testEmptyTreeProperties.  

    private static void testInsertContainsAndDuplicates() {  // Insert should reject duplicates and contains should reflect membership.  
        BstDemo.BinarySearchTree t = BstDemo.BinarySearchTree.fromValues(new int[] {});  // Start from an empty tree.  
        assertTrue(t.insert(5), "first insert should succeed");  // Insert 5.  
        assertTrue(t.contains(5), "contains should find inserted key");  // Validate contains.  
        assertTrue(!t.insert(5), "duplicate insert should be rejected");  // Duplicate insert.  
        assertEquals(1, t.size(), "size should remain 1 after duplicate");  // Validate size.  
        assertTrue(!t.contains(99), "contains should be false for missing key");  // Validate missing.  
    }  // End testInsertContainsAndDuplicates.  

    private static void testInorderSortedAndMinMax() {  // Inorder traversal should be sorted for a valid BST.  
        int[] values = new int[] {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Classic BST insert order.  
        BstDemo.BinarySearchTree t = BstDemo.BinarySearchTree.fromValues(values);  // Build BST.  
        assertEquals(9, t.size(), "size should be 9");  // Validate size.  
        assertEquals(3, t.height(), "height should be 3");  // Validate height.  
        assertEquals(1, t.min(), "min should be 1");  // Validate min.  
        assertEquals(14, t.max(), "max should be 14");  // Validate max.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 7, 8, 10, 13, 14}, t.inorder(), "inorder should be sorted");  // Validate inorder.  
    }  // End testInorderSortedAndMinMax.  

    private static void testDeleteLeafOneChildTwoChildren() {  // Delete should handle all three structural cases.  
        int[] values = new int[] {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Build a tree with multiple delete cases.  
        BstDemo.BinarySearchTree t = BstDemo.BinarySearchTree.fromValues(values);  // Build BST.  

        assertTrue(t.delete(7), "delete leaf (7) should succeed");  // Delete leaf.  
        assertTrue(!t.contains(7), "deleted key (7) should be missing");  // Validate deletion.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 8, 10, 13, 14}, t.inorder(), "inorder after deleting 7 should match");  // Validate inorder.  

        assertTrue(t.delete(14), "delete one-child node (14) should succeed");  // Delete one-child node.  
        assertTrue(!t.contains(14), "deleted key (14) should be missing");  // Validate deletion.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 14 should match");  // Validate inorder.  

        assertTrue(t.delete(3), "delete two-child node (3) should succeed");  // Delete two-child node.  
        assertTrue(!t.contains(3), "deleted key (3) should be missing");  // Validate deletion.  
        assertArrayEquals(new int[] {1, 4, 6, 8, 10, 13}, t.inorder(), "inorder after deleting 3 should match");  // Validate inorder.  
        assertEquals(2, t.height(), "height should be 2 after deletions");  // Validate height change.  
        assertEquals(1, t.min(), "min should remain 1");  // Validate min.  
        assertEquals(13, t.max(), "max should become 13");  // Validate max after deleting 14.  
    }  // End testDeleteLeafOneChildTwoChildren.  

    private static void testDeleteRootTwoChildren() {  // Deleting the root with two children should work and keep BST order.  
        int[] values = new int[] {8, 3, 10, 1, 6, 14, 4, 7, 13};  // Root (8) has two children.  
        BstDemo.BinarySearchTree t = BstDemo.BinarySearchTree.fromValues(values);  // Build BST.  
        assertTrue(t.delete(8), "delete root (8) should succeed");  // Delete root.  
        assertTrue(!t.contains(8), "deleted root key (8) should be missing");  // Validate deletion.  
        assertEquals(8, t.size(), "size should be 8 after deleting root");  // Validate size.  
        assertArrayEquals(new int[] {1, 3, 4, 6, 7, 10, 13, 14}, t.inorder(), "inorder should remain sorted without 8");  // Validate inorder.  
        assertEquals(1, t.min(), "min should stay 1");  // Validate min.  
        assertEquals(14, t.max(), "max should stay 14");  // Validate max.  
    }  // End testDeleteRootTwoChildren.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testEmptyTreeProperties();  // Run empty tree tests.  
        testInsertContainsAndDuplicates();  // Run insert/contains/duplicate tests.  
        testInorderSortedAndMinMax();  // Run inorder/min/max tests.  
        testDeleteLeafOneChildTwoChildren();  // Run delete case tests.  
        testDeleteRootTwoChildren();  // Run root deletion test.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End BstDemoTest.  

