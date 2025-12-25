"""二元搜尋樹（BST）測試 / Binary Search Tree (BST) tests.  # Bilingual module header.

本檔案用 Python 內建 `unittest` 驗證：  # Traditional Chinese overview.
- insert / contains / delete 的正確性（含重複 key 策略）  # Test core operations.
- inorder 走訪會是遞增序列（驗證 BST ordering property）  # Test inorder sorted property.
- size / height / min / max 的結果（含空樹定義）  # Test basic properties.
"""  # End of module docstring.

import unittest  # Use Python's built-in unittest so tests run without extra dependencies.  

from bst_demo import BinarySearchTree  # Import the implementation under test.  


class BstDemoTests(unittest.TestCase):  # Group BST tests using unittest.TestCase.  
    def test_empty_tree_properties(self) -> None:  # Empty tree should follow our edge-based height definition.  
        t = BinarySearchTree.from_values([])  # Build an empty BST.  
        self.assertTrue(t.is_empty())  # Empty BST should report empty.  
        self.assertEqual(t.size(), 0)  # Size of empty tree is 0.  
        self.assertEqual(t.height(), -1)  # Height of empty tree is -1 by our edges definition.  
        self.assertEqual(t.min(), None)  # Empty tree has no minimum.  
        self.assertEqual(t.max(), None)  # Empty tree has no maximum.  
        self.assertEqual(t.inorder(), [])  # Inorder traversal of empty tree is empty.  
        self.assertFalse(t.contains(1))  # contains should return False for any key.  
        self.assertFalse(t.delete(1))  # delete should return False when key is missing.  

    def test_insert_contains_and_duplicates(self) -> None:  # Insert should reject duplicates and contains should reflect membership.  
        t = BinarySearchTree.from_values([])  # Start from an empty tree.  
        self.assertTrue(t.insert(5))  # First insert should succeed.  
        self.assertTrue(t.contains(5))  # contains should find inserted key.  
        self.assertFalse(t.insert(5))  # Duplicate insert should be rejected by policy.  
        self.assertEqual(t.size(), 1)  # Size should remain 1 after duplicate insert attempt.  
        self.assertFalse(t.contains(99))  # contains should return False for missing key.  

    def test_inorder_sorted_and_min_max(self) -> None:  # Inorder traversal should be sorted for a valid BST.  
        values = [8, 3, 10, 1, 6, 14, 4, 7, 13]  # Classic BST insert order used in many textbooks.  
        t = BinarySearchTree.from_values(values)  # Build BST by inserting in order.  
        self.assertEqual(t.size(), 9)  # Tree should have 9 unique nodes.  
        self.assertEqual(t.height(), 3)  # Height should be 3 edges for this shape.  
        self.assertEqual(t.min(), 1)  # Minimum should be 1 (leftmost).  
        self.assertEqual(t.max(), 14)  # Maximum should be 14 (rightmost).  
        self.assertEqual(t.inorder(), [1, 3, 4, 6, 7, 8, 10, 13, 14])  # Inorder should be sorted.  

    def test_delete_leaf_one_child_two_children(self) -> None:  # Delete should handle all three structural cases.  
        values = [8, 3, 10, 1, 6, 14, 4, 7, 13]  # Build a tree with a leaf, a one-child node, and a two-child node.  
        t = BinarySearchTree.from_values(values)  # Build BST by insertion.  

        self.assertTrue(t.delete(7))  # Delete leaf node (7).  
        self.assertFalse(t.contains(7))  # Deleted key should be missing.  
        self.assertEqual(t.inorder(), [1, 3, 4, 6, 8, 10, 13, 14])  # Inorder should remain sorted.  

        self.assertTrue(t.delete(14))  # Delete one-child node (14 has left child 13).  
        self.assertFalse(t.contains(14))  # Deleted key should be missing.  
        self.assertEqual(t.inorder(), [1, 3, 4, 6, 8, 10, 13])  # Inorder should remain sorted.  

        self.assertTrue(t.delete(3))  # Delete two-child node (3) using inorder successor replacement.  
        self.assertFalse(t.contains(3))  # Deleted key should be missing.  
        self.assertEqual(t.inorder(), [1, 4, 6, 8, 10, 13])  # Inorder should remain sorted after successor replacement.  
        self.assertEqual(t.height(), 2)  # Height should decrease after deletions reshape the tree.  
        self.assertEqual(t.min(), 1)  # Minimum remains 1.  
        self.assertEqual(t.max(), 13)  # Maximum becomes 13 after deleting 14.  

    def test_delete_root_two_children(self) -> None:  # Deleting the root with two children should work and keep BST order.  
        values = [8, 3, 10, 1, 6, 14, 4, 7, 13]  # Classic tree where root (8) has two children.  
        t = BinarySearchTree.from_values(values)  # Build BST by insertion.  
        self.assertTrue(t.delete(8))  # Delete root key (8).  
        self.assertFalse(t.contains(8))  # Deleted key should be missing.  
        self.assertEqual(t.size(), 8)  # Size should decrease by 1.  
        self.assertEqual(t.inorder(), [1, 3, 4, 6, 7, 10, 13, 14])  # Inorder should remain sorted without 8.  
        self.assertEqual(t.min(), 1)  # Minimum stays 1.  
        self.assertEqual(t.max(), 14)  # Maximum stays 14.  


if __name__ == "__main__":  # Allow running this file directly.  
    unittest.main(verbosity=2)  # Run tests with verbose output.  

