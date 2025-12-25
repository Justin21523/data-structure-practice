// 04 雙端佇列測試（JavaScript）/ Tests for deque (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const {  // Import code under test.  
    Deque,  // Deque implementation.  
} = require("./dequeDemo.js");  // Load module relative to this file.  

test("basic both-ends operations", () => {  // Deque should support operations at both ends.  
    const d = new Deque();  // Start with empty deque.  
    d.pushBack(3);  // Push 3 to back.  
    d.pushBack(7);  // Push 7 to back.  
    d.pushFront(1);  // Push 1 to front.  
    assert.deepEqual(d.toArray(), [1, 3, 7]);  // Validate ordering.  
    assert.equal(d.peekFront(), 1);  // Validate peekFront.  
    assert.equal(d.peekBack(), 7);  // Validate peekBack.  
    assert.equal(d.popFront().value, 1);  // Pop front returns 1.  
    assert.equal(d.popBack().value, 7);  // Pop back returns 7.  
    assert.equal(d.popBack().value, 3);  // Pop back returns 3.  
    assert.equal(d.isEmpty(), true);  // Deque should be empty at end.  
});  // End test.  

test("moved is always 0", () => {  // With circular buffer, operations should not shift elements.  
    const d = new Deque();  // Start with empty deque.  
    assert.equal(d.pushBack(10).moved, 0);  // pushBack should not shift.  
    assert.equal(d.pushFront(20).moved, 0);  // pushFront should not shift.  
    assert.equal(d.popFront().cost.moved, 0);  // popFront should not shift.  
    d.pushBack(30);  // Add one more element.  
    assert.equal(d.popBack().cost.moved, 0);  // popBack should not shift.  
});  // End test.  

test("resize copied counts with pushBack", () => {  // copied should count resize copies exactly.  
    const d = new Deque();  // Start with empty deque (capacity=1).  
    assert.equal(d.pushBack(10).copied, 0);  // First push fits without resize.  
    assert.equal(d.pushBack(20).copied, 1);  // Resize 1->2 copies 1 element.  
    assert.equal(d.pushBack(30).copied, 2);  // Resize 2->4 copies 2 elements.  
    assert.equal(d.pushBack(40).copied, 0);  // Fits in capacity 4.  
    assert.equal(d.pushBack(50).copied, 4);  // Resize 4->8 copies 4 elements.  
    assert.equal(d.size, 5);  // Validate final size.  
    assert.equal(d.capacity, 8);  // Validate final capacity.  
    assert.equal(d.totalCopies, 1 + 2 + 4);  // Total copies should sum resizes.  
    assert.deepEqual(d.toArray(), [10, 20, 30, 40, 50]);  // Ordering should be preserved.  
});  // End test.  

test("wrap-around with both ends", () => {  // pushFront/popBack should work when head wraps.  
    const d = new Deque();  // Start with empty deque.  
    for (const v of [0, 1, 2, 3]) {  // Fill to capacity 4 (with resizes).  
        d.pushBack(v);  // Push to back.  
    }  // Close loop.  
    assert.equal(d.popFront().value, 0);  // Pop front => 0.  
    assert.equal(d.popFront().value, 1);  // Pop front => 1.  
    d.pushBack(4);  // Push back (wrap-around).  
    d.pushBack(5);  // Push back again.  
    assert.deepEqual(d.toArray(), [2, 3, 4, 5]);  // Validate ordering.  
    d.pushFront(1);  // Push front (head wraps).  
    assert.deepEqual(d.toArray(), [1, 2, 3, 4, 5]);  // Validate ordering after pushFront.  
});  // End test.  

test("resize when head not 0 preserves order", () => {  // Resize should preserve order even when head is offset.  
    const d = new Deque();  // Start with empty deque.  
    for (const v of [0, 1, 2, 3]) {  // Fill to capacity 4.  
        d.pushBack(v);  // Push to back.  
    }  // Close loop.  
    d.popFront();  // Remove 0 (head moves).  
    d.popFront();  // Remove 1 (head moves).  
    d.pushBack(4);  // Push back (wrap-around).  
    d.pushBack(5);  // Push back (deque full again).  
    const cost = d.pushBack(6);  // Trigger resize and push 6.  
    assert.equal(cost.copied, 4);  // Resize should copy exactly size elements (4).  
    assert.equal(cost.moved, 0);  // moved stays 0.  
    assert.deepEqual(d.toArray(), [2, 3, 4, 5, 6]);  // Ordering should be preserved.  
});  // End test.  

test("empty operations throw", () => {  // pops/peeks should reject empty deque.  
    const d = new Deque();  // Create empty deque.  
    assert.throws(() => d.peekFront(), RangeError);  // Invalid peekFront.  
    assert.throws(() => d.peekBack(), RangeError);  // Invalid peekBack.  
    assert.throws(() => d.popFront(), RangeError);  // Invalid popFront.  
    assert.throws(() => d.popBack(), RangeError);  // Invalid popBack.  
});  // End test.  

