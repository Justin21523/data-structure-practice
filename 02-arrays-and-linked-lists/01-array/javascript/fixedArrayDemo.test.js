// 01 靜態陣列測試（JavaScript）/ Tests for static array (JavaScript).  // Bilingual test header.

const test = require("node:test");  // Use Node's built-in test runner (no external deps).  
const assert = require("node:assert/strict");  // Use strict assertions for clear failures.  

const demo = require("./fixedArrayDemo");  // Import the demo module under test.  

test("append/get/toArray should work", () => {  // Basic correctness test for append/get.  
    const a = new demo.FixedArray(3);  // Create a small fixed array.  
    assert.equal(a.append(10), 0);  // Append at end moves 0 elements.  
    assert.equal(a.append(20), 0);  // Append at end moves 0 elements.  
    assert.equal(a.get(0), 10);  // Validate first element.  
    assert.equal(a.get(1), 20);  // Validate second element.  
    assert.deepEqual(a.toArray(), [10, 20]);  // Validate ordering.  
});  // End test.  

test("set should update value", () => {  // set() should update an existing index.  
    const a = new demo.FixedArray(2);  // Create array.  
    a.append(1);  // Fill index 0.  
    a.append(2);  // Fill index 1.  
    a.set(1, 99);  // Update index 1.  
    assert.deepEqual(a.toArray(), [1, 99]);  // Validate updated list.  
});  // End test.  

test("insertAt should shift right and return moved count", () => {  // Insert test for shifting.  
    const a = new demo.FixedArray(5);  // Create capacity for insertion.  
    a.append(1);  // [1]  
    a.append(2);  // [1,2]  
    a.append(3);  // [1,2,3]  
    const moved = a.insertAt(1, 99);  // Insert at index 1; should move 2 elements.  
    assert.equal(moved, 2);  // Validate moved count = size_before - index.  
    assert.deepEqual(a.toArray(), [1, 99, 2, 3]);  // Validate ordering.  
});  // End test.  

test("removeAt should shift left and return moved count", () => {  // Remove test for shifting.  
    const a = new demo.FixedArray(4);  // Create array.  
    [10, 20, 30, 40].forEach((v) => a.append(v));  // Fill array fully.  
    const result = a.removeAt(1);  // Remove 20; should move 2 elements.  
    assert.equal(result.value, 20);  // Validate removed value.  
    assert.equal(result.moved, 2);  // Validate moved count = size_before - index - 1.  
    assert.deepEqual(a.toArray(), [10, 30, 40]);  // Validate ordering after removal.  
});  // End test.  

test("indexOf should find first match or -1", () => {  // Linear search behavior.  
    const a = new demo.FixedArray(5);  // Create array.  
    [7, 8, 7].forEach((v) => a.append(v));  // Add duplicates.  
    assert.equal(a.indexOf(7), 0);  // First 7 at index 0.  
    assert.equal(a.indexOf(8), 1);  // 8 at index 1.  
    assert.equal(a.indexOf(999), -1);  // Missing returns -1.  
});  // End test.  

test("overflow and invalid indices should throw", () => {  // Validate error handling.  
    const a = new demo.FixedArray(1);  // Capacity 1.  
    a.append(1);  // Fill.  
    assert.throws(() => a.append(2));  // Append should throw when full.  
    assert.throws(() => a.get(1));  // get should throw on invalid index.  
    assert.throws(() => a.set(-1, 0));  // set should throw on invalid index.  
    assert.throws(() => a.insertAt(2, 0));  // insertAt should throw when index>size.  
    assert.throws(() => a.removeAt(1));  // removeAt should throw on invalid index.  
});  // End test.  

test("simulation formulas should match", () => {  // Validate deterministic move formulas.  
    assert.equal(demo.simulateInsertMoves(0, 0), 0);  // Insert into empty moves 0.  
    assert.equal(demo.simulateInsertMoves(4, 0), 4);  // Insert at head moves n.  
    assert.equal(demo.simulateInsertMoves(4, 4), 0);  // Insert at tail moves 0.  
    assert.equal(demo.simulateRemoveMoves(4, 0), 3);  // Remove at head moves n-1.  
    assert.equal(demo.simulateRemoveMoves(4, 3), 0);  // Remove at tail moves 0.  
});  // End test.  
