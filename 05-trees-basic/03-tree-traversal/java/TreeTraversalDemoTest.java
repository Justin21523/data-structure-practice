// 03 樹走訪測試（Java）/ Tests for tree traversal (Java).  // Bilingual file header.

import java.util.Arrays;  // Use Arrays.equals / Arrays.toString for comparisons and messages.  

public final class TreeTraversalDemoTest {  // Minimal test runner without JUnit/Maven.  
    private TreeTraversalDemoTest() {  // Prevent instantiation.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    private static void assertArrayEquals(int[] expected, int[] actual, String message) {  // Minimal array equality helper.  
        if (!Arrays.equals(expected, actual)) {  // Fail when arrays differ.  
            throw new AssertionError("FAIL: " + message + " (expected=" + Arrays.toString(expected) + ", actual=" + Arrays.toString(actual) + ")");  // Throw mismatch.  
        }  // Close failure branch.  
    }  // End assertArrayEquals.  

    private static void testEmptyTree() {  // Empty tree should produce empty traversals for all variants.  
        TreeTraversalDemo.BinaryTree t = TreeTraversalDemo.BinaryTree.fromLevelOrder(new Integer[] {});  // Build an empty tree.  
        assertArrayEquals(new int[] {}, t.preorderRecursive(), "preorderRecursive should be empty");  // Validate preorderRecursive.  
        assertArrayEquals(new int[] {}, t.preorderIterative(), "preorderIterative should be empty");  // Validate preorderIterative.  
        assertArrayEquals(new int[] {}, t.inorderRecursive(), "inorderRecursive should be empty");  // Validate inorderRecursive.  
        assertArrayEquals(new int[] {}, t.inorderIterative(), "inorderIterative should be empty");  // Validate inorderIterative.  
        assertArrayEquals(new int[] {}, t.postorderRecursive(), "postorderRecursive should be empty");  // Validate postorderRecursive.  
        assertArrayEquals(new int[] {}, t.postorderIterative(), "postorderIterative should be empty");  // Validate postorderIterative.  
        assertArrayEquals(new int[] {}, t.levelOrder(), "levelOrder should be empty");  // Validate levelOrder.  
    }  // End testEmptyTree.  

    private static void testSampleTreeMatchesExpected() {  // Sample tree should match known traversal sequences.  
        TreeTraversalDemo.BinaryTree t = TreeTraversalDemo.BinaryTree.fromLevelOrder(new Integer[] {1, 2, 3, 4, 5});  // Build sample tree.  
        int[] expectedPre = new int[] {1, 2, 4, 5, 3};  // Expected preorder traversal.  
        int[] expectedIn = new int[] {4, 2, 5, 1, 3};  // Expected inorder traversal.  
        int[] expectedPost = new int[] {4, 5, 2, 3, 1};  // Expected postorder traversal.  
        int[] expectedLevel = new int[] {1, 2, 3, 4, 5};  // Expected level-order traversal.  

        assertArrayEquals(expectedPre, t.preorderRecursive(), "preorderRecursive should match");  // Validate preorderRecursive.  
        assertArrayEquals(expectedPre, t.preorderIterative(), "preorderIterative should match");  // Validate preorderIterative.  
        assertArrayEquals(expectedIn, t.inorderRecursive(), "inorderRecursive should match");  // Validate inorderRecursive.  
        assertArrayEquals(expectedIn, t.inorderIterative(), "inorderIterative should match");  // Validate inorderIterative.  
        assertArrayEquals(expectedPost, t.postorderRecursive(), "postorderRecursive should match");  // Validate postorderRecursive.  
        assertArrayEquals(expectedPost, t.postorderIterative(), "postorderIterative should match");  // Validate postorderIterative.  
        assertArrayEquals(expectedLevel, t.levelOrder(), "levelOrder should match");  // Validate levelOrder.  
    }  // End testSampleTreeMatchesExpected.  

    private static void testTreeWithHolesMatchesExpected() {  // Trees with null holes should still traverse correctly.  
        TreeTraversalDemo.BinaryTree t = TreeTraversalDemo.BinaryTree.fromLevelOrder(new Integer[] {1, 2, 3, null, 5, null, 7});  // Build tree with missing children.  
        int[] expectedPre = new int[] {1, 2, 5, 3, 7};  // Expected preorder traversal.  
        int[] expectedIn = new int[] {2, 5, 1, 3, 7};  // Expected inorder traversal.  
        int[] expectedPost = new int[] {5, 2, 7, 3, 1};  // Expected postorder traversal.  
        int[] expectedLevel = new int[] {1, 2, 3, 5, 7};  // Expected level-order traversal.  

        assertArrayEquals(expectedPre, t.preorderRecursive(), "preorderRecursive (holes) should match");  // Validate preorderRecursive.  
        assertArrayEquals(expectedPre, t.preorderIterative(), "preorderIterative (holes) should match");  // Validate preorderIterative.  
        assertArrayEquals(expectedIn, t.inorderRecursive(), "inorderRecursive (holes) should match");  // Validate inorderRecursive.  
        assertArrayEquals(expectedIn, t.inorderIterative(), "inorderIterative (holes) should match");  // Validate inorderIterative.  
        assertArrayEquals(expectedPost, t.postorderRecursive(), "postorderRecursive (holes) should match");  // Validate postorderRecursive.  
        assertArrayEquals(expectedPost, t.postorderIterative(), "postorderIterative (holes) should match");  // Validate postorderIterative.  
        assertArrayEquals(expectedLevel, t.levelOrder(), "levelOrder (holes) should match");  // Validate levelOrder.  
    }  // End testTreeWithHolesMatchesExpected.  

    public static void main(String[] args) {  // Run all tests and print status.  
        testEmptyTree();  // Run empty tree tests.  
        testSampleTreeMatchesExpected();  // Run sample tree tests.  
        testTreeWithHolesMatchesExpected();  // Run hole-handling tests.  
        System.out.println("All tests PASSED.");  // Print success.  
    }  // End main.  
}  // End TreeTraversalDemoTest.  

