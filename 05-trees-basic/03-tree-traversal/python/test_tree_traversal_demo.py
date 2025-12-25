"""樹走訪（Tree Traversal）測試 / Tree traversal tests.  # Bilingual module header.

本檔案用 Python 內建 `unittest` 驗證：  # Traditional Chinese overview.
- DFS 三種走訪：preorder / inorder / postorder  # Test DFS orders.
- 每種 DFS 的 recursive 與 iterative 版本結果一致  # Test recursive vs iterative equivalence.
- BFS 的 level-order 走訪結果  # Test BFS order.
"""  # End of module docstring.

import unittest  # Use Python's built-in unittest so tests run without extra dependencies.  

from tree_traversal_demo import BinaryTree  # Import the implementation under test.  


class TreeTraversalDemoTests(unittest.TestCase):  # Group traversal tests using unittest.TestCase.  
    def test_empty_tree(self) -> None:  # Empty tree should produce empty traversals for all variants.  
        t = BinaryTree.from_level_order([])  # Build an empty tree.  
        self.assertEqual(t.preorder_recursive(), [])  # Recursive preorder should be empty.  
        self.assertEqual(t.preorder_iterative(), [])  # Iterative preorder should be empty.  
        self.assertEqual(t.inorder_recursive(), [])  # Recursive inorder should be empty.  
        self.assertEqual(t.inorder_iterative(), [])  # Iterative inorder should be empty.  
        self.assertEqual(t.postorder_recursive(), [])  # Recursive postorder should be empty.  
        self.assertEqual(t.postorder_iterative(), [])  # Iterative postorder should be empty.  
        self.assertEqual(t.level_order(), [])  # Level-order should be empty.  

    def test_sample_tree_traversals_match_expected(self) -> None:  # Sample tree should match known traversal sequences.  
        t = BinaryTree.from_level_order([1, 2, 3, 4, 5])  # Build sample tree.  
        expected_pre = [1, 2, 4, 5, 3]  # Expected preorder traversal.  
        expected_in = [4, 2, 5, 1, 3]  # Expected inorder traversal.  
        expected_post = [4, 5, 2, 3, 1]  # Expected postorder traversal.  
        expected_level = [1, 2, 3, 4, 5]  # Expected level-order traversal.  

        self.assertEqual(t.preorder_recursive(), expected_pre)  # Recursive preorder should match.  
        self.assertEqual(t.preorder_iterative(), expected_pre)  # Iterative preorder should match.  
        self.assertEqual(t.inorder_recursive(), expected_in)  # Recursive inorder should match.  
        self.assertEqual(t.inorder_iterative(), expected_in)  # Iterative inorder should match.  
        self.assertEqual(t.postorder_recursive(), expected_post)  # Recursive postorder should match.  
        self.assertEqual(t.postorder_iterative(), expected_post)  # Iterative postorder should match.  
        self.assertEqual(t.level_order(), expected_level)  # Level-order should match.  

    def test_tree_with_holes_traversals_match_expected(self) -> None:  # Trees with None holes should still traverse correctly.  
        t = BinaryTree.from_level_order([1, 2, 3, None, 5, None, 7])  # Build tree with missing children.  
        expected_pre = [1, 2, 5, 3, 7]  # Expected preorder traversal.  
        expected_in = [2, 5, 1, 3, 7]  # Expected inorder traversal.  
        expected_post = [5, 2, 7, 3, 1]  # Expected postorder traversal.  
        expected_level = [1, 2, 3, 5, 7]  # Expected level-order traversal.  

        self.assertEqual(t.preorder_recursive(), expected_pre)  # Recursive preorder should match.  
        self.assertEqual(t.preorder_iterative(), expected_pre)  # Iterative preorder should match.  
        self.assertEqual(t.inorder_recursive(), expected_in)  # Recursive inorder should match.  
        self.assertEqual(t.inorder_iterative(), expected_in)  # Iterative inorder should match.  
        self.assertEqual(t.postorder_recursive(), expected_post)  # Recursive postorder should match.  
        self.assertEqual(t.postorder_iterative(), expected_post)  # Iterative postorder should match.  
        self.assertEqual(t.level_order(), expected_level)  # Level-order should match.  


if __name__ == "__main__":  # Allow running this file directly.  
    unittest.main(verbosity=2)  # Run tests with verbose output.  

