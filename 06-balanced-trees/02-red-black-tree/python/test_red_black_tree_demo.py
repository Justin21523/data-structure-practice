"""紅黑樹（Red-Black Tree, LLRB）測試 / Red-black tree tests.  # Bilingual module header.

本檔案用 Python 內建 `unittest` 驗證：  # Traditional Chinese overview.
- insert 之後仍滿足紅黑樹（LLRB）不變量  # Verify insertion invariants.
- delete 之後仍維持排序與平衡性質  # Verify deletion invariants.
- validate() 同時檢查 BST ordering + 黑高一致 + 無連續紅 + 無右紅（LLRB）  # Explain validation coverage.
"""  # End of module docstring.

import unittest  # Use Python's built-in unittest so tests run without extra dependencies.  

from red_black_tree_demo import RedBlackTree  # Import the implementation under test.  


class RedBlackTreeDemoTests(unittest.TestCase):  # Group red-black tree tests using unittest.TestCase.  
    def test_empty_tree(self) -> None:  # Empty tree should follow our edge-based height definition.  
        t = RedBlackTree.from_values([])  # Build an empty tree.  
        self.assertTrue(t.is_empty())  # Validate emptiness.  
        self.assertEqual(-1, t.height())  # Validate height definition for empty tree.  
        self.assertEqual([], t.inorder())  # Validate inorder for empty tree.  
        self.assertIsNone(t.min())  # Validate min for empty tree.  
        self.assertIsNone(t.max())  # Validate max for empty tree.  
        self.assertTrue(t.validate())  # Empty tree should be valid.  
        self.assertFalse(t.contains(1))  # contains should be false on empty.  
        self.assertFalse(t.delete(1))  # delete should be false when key is missing.  

    def test_insert_rejects_duplicates(self) -> None:  # Insert should reject duplicates by policy.  
        t = RedBlackTree.from_values([])  # Start from an empty tree.  
        self.assertTrue(t.insert(5))  # First insert should succeed.  
        self.assertFalse(t.insert(5))  # Duplicate insert should be rejected.  
        self.assertTrue(t.contains(5))  # contains should find inserted key.  
        self.assertEqual([5], t.inorder())  # inorder should contain one key.  
        self.assertTrue(t.validate())  # Tree should remain valid after duplicate attempt.  

    def test_sorted_inorder_and_height_bound_for_ascending_inserts(self) -> None:  # Ascending inserts should still keep height logarithmic.  
        values = list(range(1, 16))  # Insert a strictly increasing sequence (worst case for plain BST).  
        t = RedBlackTree.from_values(values)  # Build tree by inserting in order.  
        self.assertEqual(values, t.inorder())  # inorder must stay sorted.  
        self.assertTrue(t.validate())  # Tree should satisfy red-black invariants.  
        self.assertLessEqual(t.height(), 7)  # Red-black height bound for n=15 is <= 7 edges (2*log2(n+1)-1).  

    def test_delete_cases_and_root_delete(self) -> None:  # Delete should keep inorder sorted and invariants valid.  
        values = [8, 3, 10, 1, 6, 14, 4, 7, 13]  # Classic insert order from many textbooks.  
        t = RedBlackTree.from_values(values)  # Build tree by inserting in order.  
        self.assertTrue(t.validate())  # Sanity-check initial validity.  

        self.assertFalse(t.delete(999))  # Deleting a missing key should return False.  
        self.assertEqual([1, 3, 4, 6, 7, 8, 10, 13, 14], t.inorder())  # inorder should be unchanged after missing delete.  
        self.assertTrue(t.validate())  # Tree should remain valid after missing delete.  

        self.assertTrue(t.delete(7))  # Delete a leaf node.  
        self.assertFalse(t.contains(7))  # Deleted key should be missing.  
        self.assertEqual([1, 3, 4, 6, 8, 10, 13, 14], t.inorder())  # inorder should reflect deletion.  
        self.assertTrue(t.validate())  # Tree should remain valid after deletion.  

        self.assertTrue(t.delete(14))  # Delete a one-child node in this data set.  
        self.assertFalse(t.contains(14))  # Deleted key should be missing.  
        self.assertEqual([1, 3, 4, 6, 8, 10, 13], t.inorder())  # inorder should reflect deletion.  
        self.assertTrue(t.validate())  # Tree should remain valid after deletion.  

        self.assertTrue(t.delete(3))  # Delete a two-child node.  
        self.assertFalse(t.contains(3))  # Deleted key should be missing.  
        self.assertEqual([1, 4, 6, 8, 10, 13], t.inorder())  # inorder should reflect deletion.  
        self.assertTrue(t.validate())  # Tree should remain valid after deletion.  

        t2 = RedBlackTree.from_values(values)  # Build a fresh tree for root deletion test.  
        self.assertTrue(t2.delete(8))  # Delete the original root (8).  
        self.assertFalse(t2.contains(8))  # Deleted key should be missing.  
        self.assertEqual([1, 3, 4, 6, 7, 10, 13, 14], t2.inorder())  # inorder should remain sorted without 8.  
        self.assertTrue(t2.validate())  # Tree should remain valid after root deletion.  


if __name__ == "__main__":  # Allow running this file directly.  
    unittest.main(verbosity=2)  # Run tests with verbose output.  

