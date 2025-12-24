// 05 環狀鏈結串列測試（JavaScript）/ Tests for circular linked list (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const {  // Import code under test.  
    CircularLinkedList,  // Circular linked list implementation.  
    buildOrderedList,  // Helper that builds [0..n-1].  
} = require("./circularLinkedListDemo.js");  // Load module relative to this file.  

test("pushFront/pushBack are O(1) (hops=0)", () => {  // push operations should not traverse.  
    const lst = new CircularLinkedList();  // Start with empty list.  
    assert.equal(lst.pushBack(1).hops, 0);  // pushBack cost should be 0.  
    assert.deepEqual(lst.toArray(), [1]);  // Validate ordering.  
    assert.equal(lst.pushFront(0).hops, 0);  // pushFront cost should be 0.  
    assert.deepEqual(lst.toArray(), [0, 1]);  // Validate ordering.  
});  // End test.  

test("popFront is O(1) (hops=0)", () => {  // popFront should not traverse.  
    const lst = buildOrderedList(3);  // Build [0,1,2].  
    const r = lst.popFront();  // Pop head.  
    assert.equal(r.value, 0);  // Removed value should be 0.  
    assert.equal(r.cost.hops, 0);  // popFront cost should be 0.  
    assert.deepEqual(lst.toArray(), [1, 2]);  // Validate ordering.  
});  // End test.  

test("popBack cost is max(0, n-2)", () => {  // popBack requires finding predecessor in singly circular list.  
    for (const n of [1, 2, 3, 5, 10]) {  // Multiple sizes.  
        const lst = buildOrderedList(n);  // Build [0..n-1].  
        const r = lst.popBack();  // Pop tail.  
        assert.equal(r.value, n - 1);  // Removed value should be last.  
        const expected = n <= 1 ? 0 : n - 2;  // Predecessor search traverses n-2 links.  
        assert.equal(r.cost.hops, expected);  // Validate cost.  
        assert.deepEqual(lst.toArray(), Array.from({ length: n - 1 }, (_, i) => i));  // Validate ordering.  
    }  // Close loop.  
});  // End test.  

test("getWithCost(i) hops equals i", () => {  // getWithCost should traverse i links from head.  
    const lst = buildOrderedList(10);  // Build [0..9].  
    for (const i of [0, 1, 5, 9]) {  // Sample indices.  
        const r = lst.getWithCost(i);  // Fetch value + cost.  
        assert.equal(r.value, i);  // Ordered list value should equal index.  
        assert.equal(r.cost.hops, i);  // Hops should equal index.  
    }  // Close loop.  
});  // End test.  

test("rotate(k) rotates left by k with hops=k%size", () => {  // rotate should move tail forward.  
    const size = 4;  // Use small fixed size.  
    const cases = [  // Provide test cases.  
        { steps: 0, expected: [0, 1, 2, 3] },  // No rotation.  
        { steps: 1, expected: [1, 2, 3, 0] },  // Rotate by 1.  
        { steps: 3, expected: [3, 0, 1, 2] },  // Rotate by 3.  
        { steps: 4, expected: [0, 1, 2, 3] },  // Full cycle rotation.  
        { steps: 7, expected: [3, 0, 1, 2] },  // 7%4==3.  
    ];  // Close cases.  

    for (const c of cases) {  // Iterate cases.  
        const lst = buildOrderedList(size);  // Build base list.  
        const cost = lst.rotate(c.steps);  // Rotate by steps.  
        assert.equal(cost.hops, c.steps % size);  // Validate hop count.  
        assert.deepEqual(lst.toArray(), c.expected);  // Validate rotated ordering.  
    }  // Close loop.  
});  // End test.  

test("indexOfWithCost hops", () => {  // indexOfWithCost should count traversals up to match or size.  
    const lst = buildOrderedList(5);  // Build [0..4].  
    const f0 = lst.indexOfWithCost(0);  // Find head value.  
    assert.equal(f0.index, 0);  // Index should be 0.  
    assert.equal(f0.cost.hops, 0);  // Finding head needs 0 hops.  
    const f3 = lst.indexOfWithCost(3);  // Find value at index 3.  
    assert.equal(f3.index, 3);  // Index should be 3.  
    assert.equal(f3.cost.hops, 3);  // Finding index 3 needs 3 hops.  
    const fn = lst.indexOfWithCost(999);  // Search missing.  
    assert.equal(fn.index, -1);  // Not found sentinel.  
    assert.equal(fn.cost.hops, 5);  // Not found traverses size hops.  
});  // End test.  

test("invalid indices and inputs throw", () => {  // Methods should validate indices and inputs.  
    const lst = buildOrderedList(3);  // Build [0..2].  
    assert.throws(() => lst.getWithCost(3), RangeError);  // Invalid get.  
    assert.throws(() => lst.rotate(-1), RangeError);  // Invalid rotate.  
    const empty = new CircularLinkedList();  // Create empty list.  
    assert.throws(() => empty.popFront(), RangeError);  // Invalid pop.  
    assert.throws(() => empty.popBack(), RangeError);  // Invalid pop.  
});  // End test.  

