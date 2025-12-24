// 03 單向鏈結串列測試（JavaScript）/ Tests for singly linked list (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const {  // Import code under test.  
    SinglyLinkedList,  // Singly linked list implementation.  
    buildOrderedList,  // Helper that builds [0..n-1].  
} = require("./singlyLinkedListDemo.js");  // Load module relative to this file.  

test("pushFront hops is always 0", () => {  // pushFront should never traverse next pointers.  
    const lst = new SinglyLinkedList();  // Start with empty list.  
    for (const v of [1, 2, 3, 4]) {  // Insert a few values.  
        const cost = lst.pushFront(v);  // Insert at head.  
        assert.equal(cost.hops, 0);  // Validate cost.  
    }  // Close loop.  
});  // End test.  

test("pushBack hops equals max(0, n-1)", () => {  // pushBack should traverse to tail without a tail pointer.  
    for (const n of [0, 1, 2, 3, 5, 10]) {  // Multiple sizes including boundary cases.  
        const lst = buildOrderedList(n);  // Build [0..n-1].  
        const cost = lst.pushBack(999);  // Append sentinel.  
        const expected = n === 0 ? 0 : n - 1;  // Expected hops.  
        assert.equal(cost.hops, expected);  // Validate traversal cost.  
        assert.equal(lst.size, n + 1);  // Validate size update.  
        assert.equal(lst.toArray()[lst.size - 1], 999);  // Validate tail value.  
    }  // Close loop.  
});  // End test.  

test("getWithCost(i) returns value i with hops i on ordered list", () => {  // getWithCost should traverse i links.  
    const lst = buildOrderedList(10);  // Build [0..9].  
    for (const i of [0, 1, 5, 9]) {  // Sample indices.  
        const r = lst.getWithCost(i);  // Fetch value + cost.  
        assert.equal(r.value, i);  // Value should equal index in ordered list.  
        assert.equal(r.cost.hops, i);  // Hops should equal index.  
    }  // Close loop.  
});  // End test.  

test("insertAt cost and ordering", () => {  // insertAt should splice correctly and report hops.  
    const a = buildOrderedList(5);  // Build [0..4].  
    const c0 = a.insertAt(0, 99);  // Insert at head.  
    assert.equal(c0.hops, 0);  // Head insertion has zero hops.  
    assert.deepEqual(a.toArray(), [99, 0, 1, 2, 3, 4]);  // Validate ordering.  

    const b = buildOrderedList(5);  // Build [0..4] again.  
    const c3 = b.insertAt(3, 77);  // Insert at index 3.  
    assert.equal(c3.hops, 2);  // Predecessor is index 2 => 2 hops.  
    assert.deepEqual(b.toArray(), [0, 1, 2, 77, 3, 4]);  // Validate ordering.  
});  // End test.  

test("removeAt cost and ordering", () => {  // removeAt should bypass node correctly and report hops.  
    const a = buildOrderedList(5);  // Build [0..4].  
    const r0 = a.removeAt(0);  // Remove head.  
    assert.equal(r0.value, 0);  // Removed value should be 0.  
    assert.equal(r0.cost.hops, 0);  // Head removal has zero hops.  
    assert.deepEqual(a.toArray(), [1, 2, 3, 4]);  // Validate ordering.  

    const b = buildOrderedList(5);  // Build [0..4] again.  
    const r3 = b.removeAt(3);  // Remove value 3.  
    assert.equal(r3.value, 3);  // Removed value should be 3.  
    assert.equal(r3.cost.hops, 2);  // Predecessor is index 2 => 2 hops.  
    assert.deepEqual(b.toArray(), [0, 1, 2, 4]);  // Validate ordering.  
});  // End test.  

test("indexOfWithCost hops", () => {  // indexOfWithCost should count next traversals.  
    const lst = buildOrderedList(5);  // Build [0..4].  
    const f0 = lst.indexOfWithCost(0);  // Find head value.  
    assert.equal(f0.index, 0);  // Index should be 0.  
    assert.equal(f0.cost.hops, 0);  // Finding head needs 0 hops.  
    const f3 = lst.indexOfWithCost(3);  // Find value at index 3.  
    assert.equal(f3.index, 3);  // Index should be 3.  
    assert.equal(f3.cost.hops, 3);  // Reaching index 3 needs 3 hops.  
    const fn = lst.indexOfWithCost(999);  // Search for missing value.  
    assert.equal(fn.index, -1);  // Not found sentinel.  
    assert.equal(fn.cost.hops, 5);  // Not found should traverse n hops.  
});  // End test.  

test("invalid indices throw", () => {  // Methods should validate indices.  
    const lst = buildOrderedList(3);  // Build [0..2].  
    assert.throws(() => lst.getWithCost(-1), RangeError);  // Invalid get.  
    assert.throws(() => lst.getWithCost(3), RangeError);  // Invalid get.  
    assert.throws(() => lst.insertAt(4, 0), RangeError);  // Invalid insert.  
    assert.throws(() => lst.removeAt(3), RangeError);  // Invalid remove.  
});  // End test.  

