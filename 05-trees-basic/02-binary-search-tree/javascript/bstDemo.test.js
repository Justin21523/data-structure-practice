// 02 二元搜尋樹測試（JavaScript）/ Tests for BST (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const {  // Import code under test.  
    BinarySearchTree,  // BST implementation.  
} = require("./bstDemo.js");  // Load module relative to this file.  

test("empty tree properties", () => {  // Empty tree should follow our edge-based height definition.  
    const t = BinarySearchTree.fromValues([]);  // Build an empty BST.  
    assert.equal(t.isEmpty(), true);  // Validate emptiness.  
    assert.equal(t.size(), 0);  // Validate size.  
    assert.equal(t.height(), -1);  // Validate height definition.  
    assert.equal(t.min(), null);  // Validate min for empty.  
    assert.equal(t.max(), null);  // Validate max for empty.  
    assert.deepEqual(t.inorder(), []);  // Validate inorder for empty.  
    assert.equal(t.contains(1), false);  // Validate contains on empty.  
    assert.equal(t.delete(1), false);  // Validate delete missing on empty.  
});  // End test.  

test("insert, contains, and duplicates", () => {  // Insert should reject duplicates and contains should reflect membership.  
    const t = BinarySearchTree.fromValues([]);  // Start from an empty tree.  
    assert.equal(t.insert(5), true);  // First insert should succeed.  
    assert.equal(t.contains(5), true);  // contains should find inserted key.  
    assert.equal(t.insert(5), false);  // Duplicate insert should be rejected.  
    assert.equal(t.size(), 1);  // Size should remain 1 after duplicate.  
    assert.equal(t.contains(99), false);  // contains should be false for missing key.  
});  // End test.  

test("inorder sorted and min/max", () => {  // Inorder traversal should be sorted for a valid BST.  
    const values = [8, 3, 10, 1, 6, 14, 4, 7, 13];  // Classic BST insert order.  
    const t = BinarySearchTree.fromValues(values);  // Build BST by inserting in order.  
    assert.equal(t.size(), 9);  // Validate size.  
    assert.equal(t.height(), 3);  // Validate height.  
    assert.equal(t.min(), 1);  // Validate min.  
    assert.equal(t.max(), 14);  // Validate max.  
    assert.deepEqual(t.inorder(), [1, 3, 4, 6, 7, 8, 10, 13, 14]);  // Validate sorted inorder.  
});  // End test.  

test("delete leaf, one-child, and two-children cases", () => {  // Delete should handle all three structural cases.  
    const values = [8, 3, 10, 1, 6, 14, 4, 7, 13];  // Build a tree with multiple delete cases.  
    const t = BinarySearchTree.fromValues(values);  // Build BST by insertion.  

    assert.equal(t.delete(7), true);  // Delete leaf node (7).  
    assert.equal(t.contains(7), false);  // Deleted key should be missing.  
    assert.deepEqual(t.inorder(), [1, 3, 4, 6, 8, 10, 13, 14]);  // Inorder should remain sorted.  

    assert.equal(t.delete(14), true);  // Delete one-child node (14 has left child 13).  
    assert.equal(t.contains(14), false);  // Deleted key should be missing.  
    assert.deepEqual(t.inorder(), [1, 3, 4, 6, 8, 10, 13]);  // Inorder should remain sorted.  

    assert.equal(t.delete(3), true);  // Delete two-child node (3) using successor replacement.  
    assert.equal(t.contains(3), false);  // Deleted key should be missing.  
    assert.deepEqual(t.inorder(), [1, 4, 6, 8, 10, 13]);  // Inorder should remain sorted after successor replacement.  
    assert.equal(t.height(), 2);  // Height should decrease after deletions.  
    assert.equal(t.min(), 1);  // Minimum remains 1.  
    assert.equal(t.max(), 13);  // Maximum becomes 13 after deleting 14.  
});  // End test.  

test("delete root with two children", () => {  // Deleting the root with two children should work and keep BST order.  
    const values = [8, 3, 10, 1, 6, 14, 4, 7, 13];  // Root (8) has two children.  
    const t = BinarySearchTree.fromValues(values);  // Build BST by insertion.  
    assert.equal(t.delete(8), true);  // Delete root key (8).  
    assert.equal(t.contains(8), false);  // Deleted key should be missing.  
    assert.equal(t.size(), 8);  // Size should decrease by 1.  
    assert.deepEqual(t.inorder(), [1, 3, 4, 6, 7, 10, 13, 14]);  // Inorder should remain sorted without 8.  
    assert.equal(t.min(), 1);  // Minimum stays 1.  
    assert.equal(t.max(), 14);  // Maximum stays 14.  
});  // End test.  

