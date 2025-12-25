// 02 Red-Black Tree tests (JavaScript, LLRB) / 紅黑樹測試（JavaScript, LLRB）.  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const { RedBlackTree } = require("./redBlackTreeDemo.js");  // Import implementation under test.  

test("empty tree", () => {  // Empty tree should follow our edge-based height definition.  
    const t = RedBlackTree.fromValues([]);  // Build an empty tree.  
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
    const t = RedBlackTree.fromValues([]);  // Start from empty tree.  
    assert.equal(t.insert(5), true);  // First insert should succeed.  
    assert.equal(t.insert(5), false);  // Duplicate insert should be rejected.  
    assert.equal(t.contains(5), true);  // contains should find inserted key.  
    assert.deepEqual(t.inorder(), [5]);  // inorder should contain one key.  
    assert.equal(t.validate(), true);  // Tree should remain valid after duplicate attempt.  
});  // End test.  

test("ascending inserts keep height logarithmic", () => {  // Ascending inserts should not degenerate like plain BST.  
    const values = [];  // Build 1..15 input array.  
    for (let i = 1; i <= 15; i++) {  // Fill with increasing values.  
        values.push(i);  // Append i.  
    }  // Close fill loop.  
    const t = RedBlackTree.fromValues(values);  // Build tree by inserting in order.  
    assert.deepEqual(t.inorder(), values);  // inorder must stay sorted.  
    assert.equal(t.validate(), true);  // Tree should satisfy red-black invariants.  
    assert.ok(t.height() <= 7);  // For n=15, red-black height (edges) must be <= 7.  
});  // End test.  

test("delete cases and root delete", () => {  // Delete should keep inorder sorted and invariants valid.  
    const values = [8, 3, 10, 1, 6, 14, 4, 7, 13];  // Classic insert order from many textbooks.  
    const t = RedBlackTree.fromValues(values);  // Build tree by inserting in order.  
    assert.equal(t.validate(), true);  // Sanity-check initial validity.  

    assert.equal(t.delete(999), false);  // Deleting a missing key should return false.  
    assert.deepEqual(t.inorder(), [1, 3, 4, 6, 7, 8, 10, 13, 14]);  // inorder should be unchanged after missing delete.  
    assert.equal(t.validate(), true);  // Tree should remain valid after missing delete.  

    assert.equal(t.delete(7), true);  // Delete a leaf node.  
    assert.equal(t.contains(7), false);  // Deleted key should be missing.  
    assert.deepEqual(t.inorder(), [1, 3, 4, 6, 8, 10, 13, 14]);  // inorder should reflect deletion.  
    assert.equal(t.validate(), true);  // Tree should remain valid after deletion.  

    assert.equal(t.delete(14), true);  // Delete a one-child node in this data set.  
    assert.equal(t.contains(14), false);  // Deleted key should be missing.  
    assert.deepEqual(t.inorder(), [1, 3, 4, 6, 8, 10, 13]);  // inorder should reflect deletion.  
    assert.equal(t.validate(), true);  // Tree should remain valid after deletion.  

    assert.equal(t.delete(3), true);  // Delete a two-child node.  
    assert.equal(t.contains(3), false);  // Deleted key should be missing.  
    assert.deepEqual(t.inorder(), [1, 4, 6, 8, 10, 13]);  // inorder should reflect deletion.  
    assert.equal(t.validate(), true);  // Tree should remain valid after deletion.  

    const t2 = RedBlackTree.fromValues(values);  // Build a fresh tree for root deletion test.  
    assert.equal(t2.delete(8), true);  // Delete the original root (8).  
    assert.equal(t2.contains(8), false);  // Deleted key should be missing.  
    assert.deepEqual(t2.inorder(), [1, 3, 4, 6, 7, 10, 13, 14]);  // inorder should remain sorted without 8.  
    assert.equal(t2.validate(), true);  // Tree should remain valid after root deletion.  
});  // End test.  

