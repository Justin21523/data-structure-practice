// 02 佇列測試（JavaScript）/ Tests for queue (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for tests as well.  

const test = require("node:test");  // Use Node's built-in test runner.  
const assert = require("node:assert/strict");  // Use strict assertions.  

const {  // Import code under test.  
    ArrayQueue,  // Naive array-backed queue implementation.  
    buildOrderedQueue,  // Helper that builds [0..n-1].  
} = require("./queueDemo.js");  // Load module relative to this file.  

test("FIFO enqueue/dequeue and peek", () => {  // Queue should obey FIFO semantics.  
    const q = new ArrayQueue();  // Start with empty queue.  
    q.enqueue(10);  // Enqueue 10.  
    q.enqueue(20);  // Enqueue 20.  
    q.enqueue(30);  // Enqueue 30.  
    assert.equal(q.peek(), 10);  // Peek should return first enqueued value.  
    assert.equal(q.dequeue().value, 10);  // First dequeue returns 10.  
    assert.equal(q.dequeue().value, 20);  // Second dequeue returns 20.  
    assert.equal(q.dequeue().value, 30);  // Third dequeue returns 30.  
    assert.equal(q.isEmpty(), true);  // Queue should be empty after removing all.  
});  // End test.  

test("enqueue resize copied counts", () => {  // copied should count resize copies exactly.  
    const q = new ArrayQueue();  // Start with empty queue (capacity=1).  
    assert.equal(q.enqueue(10).copied, 0);  // First enqueue fits without resize.  
    assert.equal(q.enqueue(20).copied, 1);  // Resize 1->2 copies 1 element.  
    assert.equal(q.enqueue(30).copied, 2);  // Resize 2->4 copies 2 elements.  
    assert.equal(q.enqueue(40).copied, 0);  // Fits in capacity 4.  
    assert.equal(q.enqueue(50).copied, 4);  // Resize 4->8 copies 4 elements.  
    assert.equal(q.size, 5);  // Validate final size.  
    assert.equal(q.capacity, 8);  // Validate final capacity.  
    assert.equal(q.totalCopies, 1 + 2 + 4);  // Total copies should sum resizes.  
    assert.deepEqual(q.toArray(), [10, 20, 30, 40, 50]);  // Ordering should remain FIFO.  
});  // End test.  

test("dequeue moved cost is max(0, n-1)", () => {  // Dequeue should shift left size-1 elements in naive array queue.  
    for (const n of [1, 2, 3, 5, 10]) {  // Multiple sizes.  
        const q = buildOrderedQueue(n);  // Build [0..n-1].  
        const r = q.dequeue();  // Dequeue once.  
        assert.equal(r.value, 0);  // Dequeued value should be 0.  
        const expectedMoved = n <= 1 ? 0 : n - 1;  // Shifting left moves n-1 elements.  
        assert.equal(r.cost.moved, expectedMoved);  // Validate moved count.  
        assert.deepEqual(q.toArray(), Array.from({ length: n - 1 }, (_, i) => i + 1));  // Remaining queue should be [1..n-1].  
    }  // Close loop.  
});  // End test.  

test("empty operations throw", () => {  // dequeue/peek should reject empty queue.  
    const q = new ArrayQueue();  // Create empty queue.  
    assert.throws(() => q.peek(), RangeError);  // Invalid peek.  
    assert.throws(() => q.dequeue(), RangeError);  // Invalid dequeue.  
});  // End test.  

