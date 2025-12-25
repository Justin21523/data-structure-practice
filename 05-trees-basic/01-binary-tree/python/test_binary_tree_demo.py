"""二元樹（Binary Tree）測試 / Binary tree tests.  # Bilingual module header.

本檔案用 Python 內建 `unittest` 驗證：  # Traditional Chinese overview.
- fromLevelOrder 建樹的正確性（含 None 缺洞）  # Test tree construction.
- size / height / count_leaves 的結果  # Test structural properties.
- preorder / inorder / postorder / level_order 的走訪順序  # Test traversals.
"""  # End of module docstring.

import unittest  # Use Python's built-in unittest so tests run without extra dependencies.  

from binary_tree_demo import BinaryTree  # Import the implementation under test.  


class BinaryTreeDemoTests(unittest.TestCase):  # Group binary tree tests using unittest.TestCase.  
    def test_empty_tree(self) -> None:  # Empty tree should have size 0 and height -1.  
        t = BinaryTree.from_level_order([])  # Build an empty tree.  
        self.assertTrue(t.is_empty())  # Empty tree should report empty.  
        self.assertEqual(t.size(), 0)  # Size of empty tree is 0.  
        self.assertEqual(t.height(), -1)  # Height of empty tree is -1 by our edges definition.  
        self.assertEqual(t.count_leaves(), 0)  # Empty tree has 0 leaves.  
        self.assertEqual(t.preorder(), [])  # Traversals of empty tree are empty.  
        self.assertEqual(t.inorder(), [])  # Traversals of empty tree are empty.  
        self.assertEqual(t.postorder(), [])  # Traversals of empty tree are empty.  
        self.assertEqual(t.level_order(), [])  # Traversals of empty tree are empty.  

    def test_single_node_tree(self) -> None:  # Single node tree should have height 0 and 1 leaf.  
        t = BinaryTree.from_level_order([1])  # Build a single-node tree.  
        self.assertFalse(t.is_empty())  # Tree should not be empty.  
        self.assertEqual(t.size(), 1)  # Size should be 1.  
        self.assertEqual(t.height(), 0)  # Height of a leaf is 0 edges.  
        self.assertEqual(t.count_leaves(), 1)  # Single node is a leaf.  
        self.assertEqual(t.preorder(), [1])  # Preorder should be [1].  
        self.assertEqual(t.inorder(), [1])  # Inorder should be [1].  
        self.assertEqual(t.postorder(), [1])  # Postorder should be [1].  
        self.assertEqual(t.level_order(), [1])  # Level-order should be [1].  

    def test_sample_tree_traversals_and_properties(self) -> None:  # Sample tree should match known traversals.  
        # Tree structure from level order [1,2,3,4,5]:  # Comment the intended shape for clarity.  
        #     1  # Root.  
        #    / \  # Two children.  
        #   2   3  # Second level.  
        #  / \  # Children of 2.  
        # 4   5  # Leaves.  
        t = BinaryTree.from_level_order([1, 2, 3, 4, 5])  # Build sample tree.  
        self.assertEqual(t.size(), 5)  # Tree should have 5 nodes.  
        self.assertEqual(t.height(), 2)  # Height is 2 edges (1->2->4).  
        self.assertEqual(t.count_leaves(), 3)  # Leaves are 4, 5, and 3.  
        self.assertEqual(t.preorder(), [1, 2, 4, 5, 3])  # Preorder: root, left, right.  
        self.assertEqual(t.inorder(), [4, 2, 5, 1, 3])  # Inorder: left, root, right.  
        self.assertEqual(t.postorder(), [4, 5, 2, 3, 1])  # Postorder: left, right, root.  
        self.assertEqual(t.level_order(), [1, 2, 3, 4, 5])  # Level-order: BFS by levels.  

    def test_tree_with_holes(self) -> None:  # None holes should be handled correctly by from_level_order.  
        # Level order [1,2,3,None,5,None,7] builds:  # Describe intended shape.  
        #     1  # Root.  
        #    / \  # Two children.  
        #   2   3  # Second level.  
        #    \   \  # Missing left children.  
        #     5   7  # Leaves.  
        t = BinaryTree.from_level_order([1, 2, 3, None, 5, None, 7])  # Build tree with missing children.  
        self.assertEqual(t.size(), 5)  # Nodes are 1,2,3,5,7.  
        self.assertEqual(t.height(), 2)  # Longest path has 2 edges.  
        self.assertEqual(t.count_leaves(), 2)  # Leaves are 5 and 7.  
        self.assertEqual(t.preorder(), [1, 2, 5, 3, 7])  # Validate preorder.  
        self.assertEqual(t.inorder(), [2, 5, 1, 3, 7])  # Validate inorder.  
        self.assertEqual(t.postorder(), [5, 2, 7, 3, 1])  # Validate postorder.  
        self.assertEqual(t.level_order(), [1, 2, 3, 5, 7])  # Validate level-order BFS.  

    def test_root_is_none_builds_empty(self) -> None:  # When values[0] is None, the tree should be empty.  
        t = BinaryTree.from_level_order([None, 1, 2])  # Root is missing, so tree is empty.  
        self.assertTrue(t.is_empty())  # Tree should report empty.  
        self.assertEqual(t.size(), 0)  # Size should be 0 because root is None.  
        self.assertEqual(t.height(), -1)  # Height should be -1 for empty tree.  


if __name__ == "__main__":  # Allow running this file directly.  
    unittest.main(verbosity=2)  # Run tests with verbose output.  

