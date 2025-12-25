// 03 樹走訪測試（JavaScript）/ Tests for tree traversal (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const {  // Import code under test.  
    BinaryTree,  // BinaryTree implementation.  
} = require("./treeTraversalDemo.js");  // Load module relative to this file.  

test("empty tree", () => {  // Empty tree should produce empty traversals for all variants.  
    const t = BinaryTree.fromLevelOrder([]);  // Build an empty tree.  
    assert.deepEqual(t.preorderRecursive(), []);  // Recursive preorder should be empty.  
    assert.deepEqual(t.preorderIterative(), []);  // Iterative preorder should be empty.  
    assert.deepEqual(t.inorderRecursive(), []);  // Recursive inorder should be empty.  
    assert.deepEqual(t.inorderIterative(), []);  // Iterative inorder should be empty.  
    assert.deepEqual(t.postorderRecursive(), []);  // Recursive postorder should be empty.  
    assert.deepEqual(t.postorderIterative(), []);  // Iterative postorder should be empty.  
    assert.deepEqual(t.levelOrder(), []);  // Level-order should be empty.  
});  // End test.  

test("sample tree matches expected sequences", () => {  // Sample tree should match known traversal sequences.  
    const t = BinaryTree.fromLevelOrder([1, 2, 3, 4, 5]);  // Build sample tree.  
    const expectedPre = [1, 2, 4, 5, 3];  // Expected preorder traversal.  
    const expectedIn = [4, 2, 5, 1, 3];  // Expected inorder traversal.  
    const expectedPost = [4, 5, 2, 3, 1];  // Expected postorder traversal.  
    const expectedLevel = [1, 2, 3, 4, 5];  // Expected level-order traversal.  

    assert.deepEqual(t.preorderRecursive(), expectedPre);  // Recursive preorder should match.  
    assert.deepEqual(t.preorderIterative(), expectedPre);  // Iterative preorder should match.  
    assert.deepEqual(t.inorderRecursive(), expectedIn);  // Recursive inorder should match.  
    assert.deepEqual(t.inorderIterative(), expectedIn);  // Iterative inorder should match.  
    assert.deepEqual(t.postorderRecursive(), expectedPost);  // Recursive postorder should match.  
    assert.deepEqual(t.postorderIterative(), expectedPost);  // Iterative postorder should match.  
    assert.deepEqual(t.levelOrder(), expectedLevel);  // Level-order should match.  
});  // End test.  

test("tree with holes matches expected sequences", () => {  // Trees with null holes should still traverse correctly.  
    const t = BinaryTree.fromLevelOrder([1, 2, 3, null, 5, null, 7]);  // Build tree with missing children.  
    const expectedPre = [1, 2, 5, 3, 7];  // Expected preorder traversal.  
    const expectedIn = [2, 5, 1, 3, 7];  // Expected inorder traversal.  
    const expectedPost = [5, 2, 7, 3, 1];  // Expected postorder traversal.  
    const expectedLevel = [1, 2, 3, 5, 7];  // Expected level-order traversal.  

    assert.deepEqual(t.preorderRecursive(), expectedPre);  // Recursive preorder should match.  
    assert.deepEqual(t.preorderIterative(), expectedPre);  // Iterative preorder should match.  
    assert.deepEqual(t.inorderRecursive(), expectedIn);  // Recursive inorder should match.  
    assert.deepEqual(t.inorderIterative(), expectedIn);  // Iterative inorder should match.  
    assert.deepEqual(t.postorderRecursive(), expectedPost);  // Recursive postorder should match.  
    assert.deepEqual(t.postorderIterative(), expectedPost);  // Iterative postorder should match.  
    assert.deepEqual(t.levelOrder(), expectedLevel);  // Level-order should match.  
});  // End test.  

