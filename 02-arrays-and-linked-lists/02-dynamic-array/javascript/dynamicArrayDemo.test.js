// 02 動態陣列測試（JavaScript）/ Tests for dynamic array (JavaScript).  // Bilingual test header.

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const demo = require("./dynamicArrayDemo");  // Import module under test.  

test("capacity should be power-of-two and >= size", () => {  // Validate doubling-capacity invariant.  
    for (const m of [0, 1, 2, 3, 4, 5, 8, 9, 16, 33, 100]) {  // Mix sizes.  
        const s = demo.simulateAppends(m);  // Simulate m appends.  
        assert.equal(s.finalSize, m);  // Size should equal m.  
        assert.equal(demo.isPowerOfTwo(s.finalCapacity), true);  // Capacity should be power-of-two.  
        assert.ok(s.finalCapacity >= Math.max(1, m));  // Capacity should be sufficient.  
    }  // Close loop.  
});  // End test.  

test("total copies should equal finalCapacity - 1", () => {  // Validate deterministic copy sum for doubling.  
    for (const m of [0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33]) {  // Cover boundaries.  
        const s = demo.simulateAppends(m);  // Simulate m appends.  
        assert.equal(s.totalCopies, s.finalCapacity - 1);  // Copies sum to cap-1.  
    }  // Close loop.  
});  // End test.  

test("aggregate totalActualCost <= 3m", () => {  // Validate amortized O(1) bound for append.  
    for (const m of [0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256]) {  // Multiple sizes.  
        const s = demo.simulateAppends(m);  // Simulate m appends.  
        assert.ok(s.totalActualCost <= 3 * m);  // Total cost grows linearly.  
    }  // Close loop.  
});  // End test.  

test("insertAt should shift right and keep ordering", () => {  // Validate insert shifting behavior.  
    const a = new demo.DynamicArray();  // Create empty array.  
    a.append(1);  // [1]  
    a.append(2);  // [1,2]  
    a.append(3);  // [1,2,3]  
    const cost = a.insertAt(1, 99);  // Insert at index 1.  
    assert.equal(cost.moved, 2);  // Moving right should shift 2 elements.  
    assert.deepEqual(a.toArray(), [1, 99, 2, 3]);  // Validate ordering.  
});  // End test.  

test("removeAt should shift left and keep ordering", () => {  // Validate remove shifting behavior.  
    const a = new demo.DynamicArray();  // Create empty array.  
    [10, 20, 30, 40].forEach((v) => a.append(v));  // Fill array.  
    const result = a.removeAt(1);  // Remove 20.  
    assert.equal(result.value, 20);  // Validate removed value.  
    assert.equal(result.cost.moved, 2);  // Removing at index 1 moves 2 elements.  
    assert.deepEqual(a.toArray(), [10, 30, 40]);  // Validate ordering.  
});  // End test.  

test("invalid indices should throw", () => {  // Validate index checks.  
    const a = new demo.DynamicArray();  // Create empty array.  
    a.append(1);  // Size is 1.  
    assert.throws(() => a.get(1));  // get should throw on invalid index.  
    assert.throws(() => a.set(-1, 0));  // set should throw on invalid index.  
    assert.throws(() => a.insertAt(2, 0));  // insertAt should throw when index>size.  
    assert.throws(() => a.removeAt(1));  // removeAt should throw on invalid index.  
});  // End test.  

