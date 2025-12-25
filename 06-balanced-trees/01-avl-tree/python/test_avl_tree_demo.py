"""AVL Tree（自平衡二元搜尋樹）測試 / AVL tree tests.  # Bilingual module header.

本檔案用 Python 內建 `unittest` 驗證：  # Traditional Chinese overview.
- 插入會自動平衡（LL / RR / LR / RL 旋轉）  # Verify rotations.
- inorder 走訪仍維持排序（BST ordering）  # Verify BST ordering.
- delete 後仍維持 AVL 的平衡與 height 正確性  # Verify deletion rebalancing.
"""  # End of module docstring.

import unittest  # Use Python's built-in unittest so tests run without extra dependencies.  

from avl_tree_demo import AvlTree  # Import the implementation under test.  


class AvlTreeDemoTests(unittest.TestCase):  # Group AVL tests using unittest.TestCase.  
    def test_empty_tree(self) -> None:  # Empty tree should follow our edge-based height definition.  
        t = AvlTree.from_values([])  # Build an empty AVL tree.  
        self.assertTrue(t.is_empty())  # Validate emptiness.  
        self.assertEqual(-1, t.height())  # Validate height definition for empty tree.  
        self.assertEqual([], t.inorder())  # Validate inorder for empty tree.  
        self.assertIsNone(t.min())  # Validate min for empty tree.  
        self.assertIsNone(t.max())  # Validate max for empty tree.  
        self.assertTrue(t.validate())  # Empty tree should be valid.  
        self.assertFalse(t.contains(1))  # contains should be false on empty.  
        self.assertFalse(t.delete(1))  # delete should be false when key is missing.  

    def test_insert_rejects_duplicates(self) -> None:  # Insert should reject duplicates by policy.  
        t = AvlTree.from_values([])  # Start from an empty tree.  
        self.assertTrue(t.insert(5))  # First insert should succeed.  
        self.assertFalse(t.insert(5))  # Duplicate insert should be rejected.  
        self.assertTrue(t.contains(5))  # contains should find inserted key.  
        self.assertEqual([5], t.inorder())  # inorder should contain one key.  
        self.assertTrue(t.validate())  # Tree should remain valid after duplicate attempt.  

    def test_rotations_ll_rr_lr_rl(self) -> None:  # Classic 3-key patterns should trigger each rotation case.  
        cases = [  # Define (name, insertSequence, expectedRootKey).  
            ("LL", [3, 2, 1], 2),  # Left-left case => single right rotation.  
            ("RR", [1, 2, 3], 2),  # Right-right case => single left rotation.  
            ("LR", [3, 1, 2], 2),  # Left-right case => left then right.  
            ("RL", [1, 3, 2], 2),  # Right-left case => right then left.  
        ]  # Close cases list.  

        for name, seq, expected_root in cases:  # Check each rotation scenario.  
            t = AvlTree.from_values([])  # Start from empty tree for each case.  
            for v in seq:  # Insert values to trigger rotation.  
                self.assertTrue(t.insert(v), f"{name} insert should succeed")  # Ensure inserts succeed.  
            self.assertTrue(t.validate(), f"{name} tree should be valid")  # Validate AVL invariants.  
            self.assertEqual([1, 2, 3], t.inorder(), f"{name} inorder should be sorted")  # inorder must stay sorted.  
            self.assertIsNotNone(t.root, f"{name} root should exist")  # Root must exist after inserts.  
            self.assertEqual(expected_root, t.root.key, f"{name} root key should match rotation result")  # Check root key.  
            self.assertEqual(1, t.height(), f"{name} height should be 1 for 3 nodes")  # AVL with 3 nodes should have height 1.  

    def test_inorder_sorted_and_height_bound(self) -> None:  # AVL should keep height small while preserving sorted inorder.  
        values = [8, 3, 10, 1, 6, 14, 4, 7, 13]  # Classic insert order from many textbooks.  
        t = AvlTree.from_values(values)  # Build AVL tree by inserting in order.  
        self.assertEqual([1, 3, 4, 6, 7, 8, 10, 13, 14], t.inorder())  # inorder should be sorted.  
        self.assertEqual(1, t.min())  # min should be 1.  
        self.assertEqual(14, t.max())  # max should be 14.  
        self.assertTrue(t.validate())  # Tree should satisfy AVL invariants.  
        self.assertLessEqual(t.height(), 3)  # For 9 nodes, AVL height (edges) must be <= 3.  

    def test_delete_leaf_one_child_two_children_and_root(self) -> None:  # Delete should rebalance and keep inorder sorted.  
        values = [8, 3, 10, 1, 6, 14, 4, 7, 13]  # Build a tree with multiple delete cases.  
        t = AvlTree.from_values(values)  # Build AVL tree.  
        self.assertTrue(t.validate())  # Sanity-check initial validity.  

        self.assertTrue(t.delete(7))  # Delete a leaf node.  
        self.assertFalse(t.contains(7))  # Deleted key should be missing.  
        self.assertEqual([1, 3, 4, 6, 8, 10, 13, 14], t.inorder())  # inorder should reflect deletion.  
        self.assertTrue(t.validate())  # Tree should remain valid after deletion.  

        self.assertTrue(t.delete(14))  # Delete a one-child node (14 has left child 13).  
        self.assertFalse(t.contains(14))  # Deleted key should be missing.  
        self.assertEqual([1, 3, 4, 6, 8, 10, 13], t.inorder())  # inorder should reflect deletion.  
        self.assertTrue(t.validate())  # Tree should remain valid after deletion.  

        self.assertTrue(t.delete(3))  # Delete a two-child node.  
        self.assertFalse(t.contains(3))  # Deleted key should be missing.  
        self.assertEqual([1, 4, 6, 8, 10, 13], t.inorder())  # inorder should reflect deletion.  
        self.assertTrue(t.validate())  # Tree should remain valid after deletion.  
        self.assertLessEqual(t.height(), 2)  # With 6 nodes, AVL height (edges) must be <= 2.  

        t2 = AvlTree.from_values(values)  # Build a fresh tree for root deletion test.  
        self.assertTrue(t2.delete(8))  # Delete the original root (8).  
        self.assertFalse(t2.contains(8))  # Deleted key should be missing.  
        self.assertEqual([1, 3, 4, 6, 7, 10, 13, 14], t2.inorder())  # inorder should remain sorted without 8.  
        self.assertTrue(t2.validate())  # Tree should remain valid after root deletion.  


if __name__ == "__main__":  # Allow running this file directly.  
    unittest.main(verbosity=2)  # Run tests with verbose output.  

