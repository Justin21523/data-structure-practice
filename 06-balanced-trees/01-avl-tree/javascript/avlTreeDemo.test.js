// 01 AVL Tree tests (JavaScript) / AVL Tree 測試（JavaScript）.  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const { AvlTree } = require("./avlTreeDemo.js");  // Import implementation under test.  

test("empty tree", () => {  // Empty tree should follow our edge-based height definition.  
    const t = AvlTree.fromValues([]);  // Build an empty AVL tree.  
    assert.equal(t.isEmpty(), true);  // Validate emptiness.  
    assert.equal(t.height(), -1);  // Validate height definition for empty tree.  
    assert.deepEqual(t.inorder(), []);  // Validate inorder for empty tree.  
    assert.equal(t.min(), null);  // Validate min for empty tree.  
    assert.equal(t.max(), null);  // Validate max for empty tree.  
    assert.equal(t.validate(), true);  // Empty tree should be valid.  
    assert.equal(t.contains(1), false);  // contains should be false on empty.  
    assert.equal(t.delete(1), false);  // delete should be false when key is missing.  
});  // End test.  

test("insert rejects duplicates", () => {  // Insert should reject duplicates by policy.  
    const t = AvlTree.fromValues([]);  // Start from empty tree.  
    assert.equal(t.insert(5), true);  // First insert should succeed.  
    assert.equal(t.insert(5), false);  // Duplicate insert should be rejected.  
    assert.equal(t.contains(5), true);  // contains should find inserted key.  
    assert.deepEqual(t.inorder(), [5]);  // inorder should contain one key.  
    assert.equal(t.validate(), true);  // Tree should remain valid after duplicate attempt.  
});  // End test.  

test("LL/RR/LR/RL 3-key patterns stay balanced", () => {  // Classic patterns should produce a perfectly balanced tree.  
    const sequences = [  // Define insert sequences for each rotation pattern.  
        [3, 2, 1],  // LL case.  
        [1, 2, 3],  // RR case.  
        [3, 1, 2],  // LR case.  
        [1, 3, 2],  // RL case.  
    ];  // Close sequences.  

    for (const seq of sequences) {  // Check each scenario.  
        const t = AvlTree.fromValues(seq);  // Build AVL tree from sequence.  
        assert.equal(t.validate(), true);  // Validate AVL invariants.  
        assert.deepEqual(t.inorder(), [1, 2, 3]);  // inorder must stay sorted.  
        assert.equal(t.height(), 1);  // Height should be 1 in edges for 3 nodes.  
    }  // Close loop.  
});  // End test.  

test("inorder sorted and height bound", () => {  // AVL should keep height small while preserving sorted inorder.  
    const values = [8, 3, 10, 1, 6, 14, 4, 7, 13];  // Classic insert order from many textbooks.  
    const t = AvlTree.fromValues(values);  // Build AVL tree by inserting in order.  
    assert.deepEqual(t.inorder(), [1, 3, 4, 6, 7, 8, 10, 13, 14]);  // inorder should be sorted.  
    assert.equal(t.min(), 1);  // min should be 1.  
    assert.equal(t.max(), 14);  // max should be 14.  
    assert.equal(t.validate(), true);  // Tree should satisfy AVL invariants.  
    assert.ok(t.height() <= 3);  // For 9 nodes, AVL height (edges) must be <= 3.  
});  // End test.  

test("delete cases and root delete", () => {  // Delete should rebalance and keep inorder sorted.  
    const values = [8, 3, 10, 1, 6, 14, 4, 7, 13];  // Build a tree with multiple delete cases.  
    const t = AvlTree.fromValues(values);  // Build AVL tree.  
    assert.equal(t.validate(), true);  // Sanity-check initial validity.  

    assert.equal(t.delete(7), true);  // Delete a leaf node.  
    assert.equal(t.contains(7), false);  // Deleted key should be missing.  
    assert.deepEqual(t.inorder(), [1, 3, 4, 6, 8, 10, 13, 14]);  // inorder should reflect deletion.  
    assert.equal(t.validate(), true);  // Tree should remain valid after deletion.  

    assert.equal(t.delete(14), true);  // Delete a one-child node.  
    assert.equal(t.contains(14), false);  // Deleted key should be missing.  
    assert.deepEqual(t.inorder(), [1, 3, 4, 6, 8, 10, 13]);  // inorder should reflect deletion.  
    assert.equal(t.validate(), true);  // Tree should remain valid after deletion.  

    assert.equal(t.delete(3), true);  // Delete a two-child node.  
    assert.equal(t.contains(3), false);  // Deleted key should be missing.  
    assert.deepEqual(t.inorder(), [1, 4, 6, 8, 10, 13]);  // inorder should reflect deletion.  
    assert.equal(t.validate(), true);  // Tree should remain valid after deletion.  
    assert.ok(t.height() <= 2);  // With 6 nodes, AVL height (edges) must be <= 2.  

    const t2 = AvlTree.fromValues(values);  // Build a fresh tree for root deletion test.  
    assert.equal(t2.delete(8), true);  // Delete the original root (8).  
    assert.equal(t2.contains(8), false);  // Deleted key should be missing.  
    assert.deepEqual(t2.inorder(), [1, 3, 4, 6, 7, 10, 13, 14]);  // inorder should remain sorted without 8.  
    assert.equal(t2.validate(), true);  // Tree should remain valid after root deletion.  
});  // End test.  

