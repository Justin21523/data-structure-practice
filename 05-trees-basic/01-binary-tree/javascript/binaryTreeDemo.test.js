// 01 二元樹測試（JavaScript）/ Tests for binary tree (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const {  // Import code under test.  
    BinaryTree,  // BinaryTree implementation.  
} = require("./binaryTreeDemo.js");  // Load module relative to this file.  

test("empty tree from empty array", () => {  // Empty input should create an empty tree.  
    const t = BinaryTree.fromLevelOrder([]);  // Build from empty array.  
    assert.equal(t.isEmpty(), true);  // Validate emptiness.  
    assert.equal(t.size(), 0);  // Validate size.  
    assert.equal(t.height(), -1);  // Validate height definition.  
    assert.equal(t.countLeaves(), 0);  // Validate leaf count.  
    assert.deepEqual(t.preorder(), []);  // Validate preorder.  
    assert.deepEqual(t.inorder(), []);  // Validate inorder.  
    assert.deepEqual(t.postorder(), []);  // Validate postorder.  
    assert.deepEqual(t.levelOrder(), []);  // Validate level order.  
});  // End test.  

test("single node tree", () => {  // A single value should create a one-node tree.  
    const t = BinaryTree.fromLevelOrder([1]);  // Build from one item.  
    assert.equal(t.isEmpty(), false);  // Validate non-empty.  
    assert.equal(t.size(), 1);  // Validate size.  
    assert.equal(t.height(), 0);  // Validate height definition.  
    assert.equal(t.countLeaves(), 1);  // Validate leaf count.  
    assert.deepEqual(t.preorder(), [1]);  // Validate preorder.  
    assert.deepEqual(t.inorder(), [1]);  // Validate inorder.  
    assert.deepEqual(t.postorder(), [1]);  // Validate postorder.  
    assert.deepEqual(t.levelOrder(), [1]);  // Validate BFS.  
});  // End test.  

test("sample tree without holes", () => {  // A compact tree should produce expected traversals.  
    const values = [1, 2, 3, 4, 5];  // Sample level-order array.  
    const t = BinaryTree.fromLevelOrder(values);  // Build tree.  
    assert.equal(t.size(), 5);  // Validate size.  
    assert.equal(t.height(), 2);  // Validate height (edges).  
    assert.equal(t.countLeaves(), 3);  // Validate leaves.  
    assert.deepEqual(t.preorder(), [1, 2, 4, 5, 3]);  // Validate preorder.  
    assert.deepEqual(t.inorder(), [4, 2, 5, 1, 3]);  // Validate inorder.  
    assert.deepEqual(t.postorder(), [4, 5, 2, 3, 1]);  // Validate postorder.  
    assert.deepEqual(t.levelOrder(), [1, 2, 3, 4, 5]);  // Validate BFS.  
});  // End test.  

test("tree with holes (nulls)", () => {  // Missing nodes (nulls) should be handled correctly.  
    const values = [1, 2, 3, null, 5, null, 7];  // Sample with holes.  
    const t = BinaryTree.fromLevelOrder(values);  // Build tree with holes.  
    assert.equal(t.size(), 5);  // Validate size.  
    assert.equal(t.height(), 2);  // Validate height (edges).  
    assert.equal(t.countLeaves(), 2);  // Validate leaves.  
    assert.deepEqual(t.preorder(), [1, 2, 5, 3, 7]);  // Validate preorder.  
    assert.deepEqual(t.inorder(), [2, 5, 1, 3, 7]);  // Validate inorder.  
    assert.deepEqual(t.postorder(), [5, 2, 7, 3, 1]);  // Validate postorder.  
    assert.deepEqual(t.levelOrder(), [1, 2, 3, 5, 7]);  // Validate BFS.  
});  // End test.  

test("root null builds empty tree", () => {  // A null root should produce an empty tree even if later values exist.  
    const values = [null, 1, 2];  // Root is missing.  
    const t = BinaryTree.fromLevelOrder(values);  // Build tree.  
    assert.equal(t.isEmpty(), true);  // Validate emptiness.  
    assert.equal(t.size(), 0);  // Validate size.  
    assert.equal(t.height(), -1);  // Validate height.  
    assert.equal(t.countLeaves(), 0);  // Validate leaves.  
    assert.deepEqual(t.preorder(), []);  // Validate preorder.  
    assert.deepEqual(t.inorder(), []);  // Validate inorder.  
    assert.deepEqual(t.postorder(), []);  // Validate postorder.  
    assert.deepEqual(t.levelOrder(), []);  // Validate BFS.  
});  // End test.  

