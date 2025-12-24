// 04 雙向鏈結串列測試（JavaScript）/ Tests for doubly linked list (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const {  // Import code under test.  
    DoublyLinkedList,  // Doubly linked list implementation.  
    buildOrderedList,  // Helper that builds [0..n-1].  
} = require("./doublyLinkedListDemo.js");  // Load module relative to this file.  

function expectedGetHops(size, index) {  // Compute expected hops when traversing from the nearest end.  
    return Math.min(index, size - 1 - index);  // Nearest-end traversal cost formula.  
}  // End expectedGetHops.  

test("push/pop at both ends are O(1) (hops=0)", () => {  // push/pop should not traverse when head+tail exist.  
    for (const n of [0, 1, 2, 5]) {  // Multiple sizes including boundary cases.  
        const lst = buildOrderedList(n);  // Build [0..n-1].  
        assert.equal(lst.pushBack(999).hops, 0);  // pushBack should be O(1).  
        assert.equal(lst.pushFront(888).hops, 0);  // pushFront should be O(1).  
        assert.equal(lst.popBack().cost.hops, 0);  // popBack should be O(1).  
        assert.equal(lst.popFront().cost.hops, 0);  // popFront should be O(1).  
    }  // Close loop.  
});  // End test.  

test("getWithCost(i) hops is min(i, size-1-i)", () => {  // getWithCost should use nearest-end traversal.  
    const size = 16;  // Fixed size for coverage.  
    const lst = buildOrderedList(size);  // Build [0..15].  
    for (const i of [0, 1, 2, 4, 8, 15]) {  // Sample indices.  
        const r = lst.getWithCost(i);  // Fetch value + cost.  
        assert.equal(r.value, i);  // Ordered list value should equal index.  
        assert.equal(r.cost.hops, expectedGetHops(size, i));  // Validate hops formula.  
    }  // Close loop.  
});  // End test.  

test("insertAt cost and ordering", () => {  // insertAt should splice correctly and report traversal hops.  
    const a = buildOrderedList(6);  // Build [0..5].  
    const c0 = a.insertAt(0, 99);  // Insert at head.  
    assert.equal(c0.hops, 0);  // Head insertion needs no traversal.  
    assert.deepEqual(a.toArray(), [99, 0, 1, 2, 3, 4, 5]);  // Validate ordering.  

    const b = buildOrderedList(6);  // Build [0..5] again.  
    const c3 = b.insertAt(3, 77);  // Insert at index 3.  
    assert.equal(c3.hops, Math.min(3, 6 - 1 - 3));  // Validate hops.  
    assert.deepEqual(b.toArray(), [0, 1, 2, 77, 3, 4, 5]);  // Validate ordering.  

    const c = buildOrderedList(6);  // Build [0..5] again.  
    const ce = c.insertAt(6, 55);  // Insert at tail (index==size).  
    assert.equal(ce.hops, 0);  // Tail insertion needs no traversal.  
    assert.deepEqual(c.toArray(), [0, 1, 2, 3, 4, 5, 55]);  // Validate ordering.  
});  // End test.  

test("removeAt cost and ordering", () => {  // removeAt should bypass correctly and report traversal hops.  
    const a = buildOrderedList(6);  // Build [0..5].  
    const r0 = a.removeAt(0);  // Remove head.  
    assert.equal(r0.value, 0);  // Removed value should be 0.  
    assert.equal(r0.cost.hops, 0);  // Head removal needs no traversal.  
    assert.deepEqual(a.toArray(), [1, 2, 3, 4, 5]);  // Validate ordering.  

    const b = buildOrderedList(6);  // Build [0..5] again.  
    const r5 = b.removeAt(5);  // Remove tail.  
    assert.equal(r5.value, 5);  // Removed value should be 5.  
    assert.equal(r5.cost.hops, 0);  // Tail removal needs no traversal.  
    assert.deepEqual(b.toArray(), [0, 1, 2, 3, 4]);  // Validate ordering.  

    const c = buildOrderedList(6);  // Build [0..5] again.  
    const r3 = c.removeAt(3);  // Remove value 3.  
    assert.equal(r3.value, 3);  // Removed value should be 3.  
    assert.equal(r3.cost.hops, Math.min(3, 6 - 1 - 3));  // Validate hops.  
    assert.deepEqual(c.toArray(), [0, 1, 2, 4, 5]);  // Validate ordering.  
});  // End test.  

test("indexOfWithCost hops", () => {  // indexOfWithCost should count forward traversals.  
    const lst = buildOrderedList(5);  // Build [0..4].  
    const f0 = lst.indexOfWithCost(0);  // Find head value.  
    assert.equal(f0.index, 0);  // Index should be 0.  
    assert.equal(f0.cost.hops, 0);  // Finding head needs 0 hops.  
    const f3 = lst.indexOfWithCost(3);  // Find value at index 3.  
    assert.equal(f3.index, 3);  // Index should be 3.  
    assert.equal(f3.cost.hops, 3);  // Finding index 3 needs 3 hops.  
    const fn = lst.indexOfWithCost(999);  // Search for missing value.  
    assert.equal(fn.index, -1);  // Not found sentinel.  
    assert.equal(fn.cost.hops, 5);  // Not found traverses n hops.  
});  // End test.  

test("invalid indices and empty pops throw", () => {  // Methods should validate indices and empty operations.  
    const lst = buildOrderedList(3);  // Build [0..2].  
    assert.throws(() => lst.getWithCost(3), RangeError);  // Invalid get.  
    assert.throws(() => lst.insertAt(4, 0), RangeError);  // Invalid insert.  
    assert.throws(() => lst.removeAt(3), RangeError);  // Invalid remove.  
    const empty = new DoublyLinkedList();  // Create empty list.  
    assert.throws(() => empty.popBack(), RangeError);  // Invalid pop.  
    assert.throws(() => empty.popFront(), RangeError);  // Invalid pop.  
});  // End test.  

