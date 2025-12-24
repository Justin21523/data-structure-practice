// 01 堆疊測試（JavaScript）/ Tests for stack (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const {  // Import code under test.  
    ArrayStack,  // Array-backed stack implementation.  
} = require("./stackDemo.js");  // Load module relative to this file.  

test("LIFO push/pop and peek", () => {  // Stack should obey LIFO semantics.  
    const s = new ArrayStack();  // Start with empty stack.  
    s.push(10);  // Push 10.  
    s.push(20);  // Push 20.  
    s.push(30);  // Push 30.  
    assert.equal(s.peek(), 30);  // Peek should return last pushed value.  
    assert.equal(s.pop().value, 30);  // First pop returns 30.  
    assert.equal(s.pop().value, 20);  // Second pop returns 20.  
    assert.equal(s.pop().value, 10);  // Third pop returns 10.  
    assert.equal(s.isEmpty(), true);  // Stack should be empty after popping all.  
});  // End test.  

test("push resize copied counts", () => {  // copied should count resize copies exactly.  
    const s = new ArrayStack();  // Start with empty stack (capacity=1).  
    assert.equal(s.push(10).copied, 0);  // First push fits without resize.  
    assert.equal(s.push(20).copied, 1);  // Resize 1->2 copies 1 element.  
    assert.equal(s.push(30).copied, 2);  // Resize 2->4 copies 2 elements.  
    assert.equal(s.push(40).copied, 0);  // Fits in capacity 4.  
    assert.equal(s.push(50).copied, 4);  // Resize 4->8 copies 4 elements.  
    assert.equal(s.size, 5);  // Validate final size.  
    assert.equal(s.capacity, 8);  // Validate final capacity.  
    assert.equal(s.totalCopies, 1 + 2 + 4);  // Total copies should sum resizes.  
    assert.deepEqual(s.toArray(), [10, 20, 30, 40, 50]);  // Snapshot should preserve bottom->top order.  
});  // End test.  

test("empty operations throw", () => {  // pop/peek should reject empty stack.  
    const s = new ArrayStack();  // Create empty stack.  
    assert.throws(() => s.peek(), RangeError);  // Invalid peek.  
    assert.throws(() => s.pop(), RangeError);  // Invalid pop.  
});  // End test.  

