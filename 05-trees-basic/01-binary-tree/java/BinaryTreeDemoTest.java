// 01 二元樹測試（Java）/ Tests for binary tree (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals / Arrays.toString for comparisons and messages.  

public final class BinaryTreeDemoTest {  // Minimal test runner without JUnit/Maven.  
    private BinaryTreeDemoTest() {  // Prevent instantiation.  
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
    }  // End assertEquals.  

    private static void assertArrayEquals(int[] expected, int[] actual, String message) {  // Minimal int[] equality helper.  
        if (!Arrays.equals(expected, actual)) {  // Fail when arrays differ.  
            throw new AssertionError("FAIL: " + message + " (expected=" + Arrays.toString(expected) + ", actual=" + Arrays.toString(actual) + ")");  // Throw mismatch.  
        }  // Close failure branch.  
    }  // End assertArrayEquals.  

    private static void testEmptyTreeFromEmptyArray() {  // Empty input should create an empty tree.  
        BinaryTreeDemo.BinaryTree t = BinaryTreeDemo.BinaryTree.fromLevelOrder(new Integer[] {});  // Build from empty array.  
        assertTrue(t.isEmpty(), "tree should be empty");  // Validate emptiness.  
        assertEquals(0, t.size(), "size should be 0");  // Validate size.  
        assertEquals(-1, t.height(), "height should be -1 for empty");  // Validate height definition.  
        assertEquals(0, t.countLeaves(), "leaves should be 0 for empty");  // Validate leaf count.  
        assertArrayEquals(new int[] {}, t.preorder(), "preorder should be empty");  // Validate preorder.  
        assertArrayEquals(new int[] {}, t.inorder(), "inorder should be empty");  // Validate inorder.  
        assertArrayEquals(new int[] {}, t.postorder(), "postorder should be empty");  // Validate postorder.  
        assertArrayEquals(new int[] {}, t.levelOrder(), "levelOrder should be empty");  // Validate level order.  
    }  // End testEmptyTreeFromEmptyArray.  

    private static void testSingleNodeTree() {  // A single value should create a one-node tree.  
        BinaryTreeDemo.BinaryTree t = BinaryTreeDemo.BinaryTree.fromLevelOrder(new Integer[] {1});  // Build from one item.  
        assertTrue(!t.isEmpty(), "tree should not be empty");  // Validate non-empty.  
        assertEquals(1, t.size(), "size should be 1");  // Validate size.  
        assertEquals(0, t.height(), "height should be 0 for leaf");  // Validate height definition.  
        assertEquals(1, t.countLeaves(), "leaves should be 1 for single node");  // Validate leaf count.  
        assertArrayEquals(new int[] {1}, t.preorder(), "preorder should be [1]");  // Validate preorder.  
        assertArrayEquals(new int[] {1}, t.inorder(), "inorder should be [1]");  // Validate inorder.  
        assertArrayEquals(new int[] {1}, t.postorder(), "postorder should be [1]");  // Validate postorder.  
        assertArrayEquals(new int[] {1}, t.levelOrder(), "levelOrder should be [1]");  // Validate level order.  
    }  // End testSingleNodeTree.  

    private static void testSampleTreeWithoutHoles() {  // A compact tree should produce expected traversals.  
        Integer[] values = new Integer[] {1, 2, 3, 4, 5};  // Sample level-order array.  
        BinaryTreeDemo.BinaryTree t = BinaryTreeDemo.BinaryTree.fromLevelOrder(values);  // Build tree.  
        assertEquals(5, t.size(), "size should be 5");  // Validate size.  
        assertEquals(2, t.height(), "height should be 2 (edges)");  // Validate height.  
        assertEquals(3, t.countLeaves(), "leaves should be 3");  // Validate leaves.  
        assertArrayEquals(new int[] {1, 2, 4, 5, 3}, t.preorder(), "preorder should match");  // Validate preorder.  
        assertArrayEquals(new int[] {4, 2, 5, 1, 3}, t.inorder(), "inorder should match");  // Validate inorder.  
        assertArrayEquals(new int[] {4, 5, 2, 3, 1}, t.postorder(), "postorder should match");  // Validate postorder.  
        assertArrayEquals(new int[] {1, 2, 3, 4, 5}, t.levelOrder(), "levelOrder should match");  // Validate BFS.  
    }  // End testSampleTreeWithoutHoles.  

    private static void testTreeWithHoles() {  // Missing nodes (nulls) should be handled correctly.  
        Integer[] values = new Integer[] {1, 2, 3, null, 5, null, 7};  // Sample with holes.  
        BinaryTreeDemo.BinaryTree t = BinaryTreeDemo.BinaryTree.fromLevelOrder(values);  // Build tree with holes.  
        assertEquals(5, t.size(), "size should be 5");  // Validate size.  
        assertEquals(2, t.height(), "height should be 2 (edges)");  // Validate height.  
        assertEquals(2, t.countLeaves(), "leaves should be 2");  // Validate leaves.  
        assertArrayEquals(new int[] {1, 2, 5, 3, 7}, t.preorder(), "preorder should match");  // Validate preorder.  
        assertArrayEquals(new int[] {2, 5, 1, 3, 7}, t.inorder(), "inorder should match");  // Validate inorder.  
        assertArrayEquals(new int[] {5, 2, 7, 3, 1}, t.postorder(), "postorder should match");  // Validate postorder.  
        assertArrayEquals(new int[] {1, 2, 3, 5, 7}, t.levelOrder(), "levelOrder should match");  // Validate BFS.  
    }  // End testTreeWithHoles.  

    private static void testRootNullBuildsEmptyTree() {  // A null root should produce an empty tree even if later values exist.  
        Integer[] values = new Integer[] {null, 1, 2};  // Root is missing.  
        BinaryTreeDemo.BinaryTree t = BinaryTreeDemo.BinaryTree.fromLevelOrder(values);  // Build tree.  
        assertTrue(t.isEmpty(), "tree should be empty when root is null");  // Validate emptiness.  
        assertEquals(0, t.size(), "size should be 0");  // Validate size.  
        assertEquals(-1, t.height(), "height should be -1 for empty");  // Validate height.  
        assertEquals(0, t.countLeaves(), "leaves should be 0 for empty");  // Validate leaves.  
        assertArrayEquals(new int[] {}, t.preorder(), "preorder should be empty");  // Validate preorder.  
        assertArrayEquals(new int[] {}, t.inorder(), "inorder should be empty");  // Validate inorder.  
        assertArrayEquals(new int[] {}, t.postorder(), "postorder should be empty");  // Validate postorder.  
        assertArrayEquals(new int[] {}, t.levelOrder(), "levelOrder should be empty");  // Validate BFS.  
    }  // End testRootNullBuildsEmptyTree.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testEmptyTreeFromEmptyArray();  // Run empty input test.  
        testSingleNodeTree();  // Run single node test.  
        testSampleTreeWithoutHoles();  // Run sample tree test.  
        testTreeWithHoles();  // Run hole-handling test.  
        testRootNullBuildsEmptyTree();  // Run null-root test.  
        System.out.println("All tests PASSED.");  // Print success line.  
    }  // End main.  
}  // End BinaryTreeDemoTest.  

