// 03 環狀佇列示範（JavaScript）/ Circular queue demo (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for safer, more explicit JavaScript.

class CircularQueue {  // A circular array-backed queue (dequeue does not shift; teaching-oriented).
    constructor() {  // Initialize an empty queue with capacity 1.
        this._size = 0;  // Track number of stored elements.
        this._capacity = 1;  // Start with capacity 1 for deterministic doubling.
        this._head = 0;  // Head index (front position) within the buffer.
        this._data = new Array(this._capacity);  // Allocate backing buffer.
        this._totalCopies = 0;  // Accumulate total copied elements across all resizes.
    }  // Close constructor.

    get size() {  // Expose current size for callers/tests.
        return this._size;  // Return number of stored items.
    }  // End size getter.

    get capacity() {  // Expose current capacity for callers/tests.
        return this._capacity;  // Return allocated slots.
    }  // End capacity getter.

    get totalCopies() {  // Expose total resize copies for analysis-style checks.
        return this._totalCopies;  // Return total copied elements.
    }  // End totalCopies getter.

    isEmpty() {  // Convenience helper to check emptiness.
        return this._size === 0;  // Empty iff size is zero.
    }  // End isEmpty().

    _indexAt(offset) {  // Map logical offset [0..size) to physical buffer index.
        return (this._head + offset) % this._capacity;  // Wrap around using modulo.
    }  // End _indexAt().

    toArray() {  // Return a copy of the used portion (front -> rear).
        const out = [];  // Accumulate values in order.
        for (let i = 0; i < this._size; i += 1) {  // Copy exactly size elements in queue order.
            out.push(this._data[this._indexAt(i)]);  // Append one element.
        }  // Close loop.
        return out;  // Return copied array.
    }  // End toArray().

    _resize(newCapacity) {  // Resize buffer and return number of copied elements.
        if (newCapacity < this._size) {  // Ensure new buffer can hold all existing elements.
            throw new RangeError("newCapacity must be >= size");  // Signal invalid resize request.
        }  // Close validation.
        if (newCapacity < 1) {  // Keep capacity positive for simplicity.
            throw new RangeError("newCapacity must be >= 1");  // Signal invalid capacity.
        }  // Close validation.

        const newData = new Array(newCapacity);  // Allocate new buffer.
        let copied = 0;  // Count copied elements deterministically.
        for (let i = 0; i < this._size; i += 1) {  // Copy elements in queue order.
            newData[i] = this._data[this._indexAt(i)];  // Copy one element into compact layout.
            copied += 1;  // Count one copy.
        }  // Close copy loop.
        this._data = newData;  // Swap buffer reference.
        this._capacity = newCapacity;  // Update capacity.
        this._head = 0;  // Reset head so queue starts at index 0 after resize.
        this._totalCopies += copied;  // Accumulate total copies.
        return copied;  // Return copies performed for this resize.
    }  // End _resize().

    _ensureCapacityForOneMore() {  // Ensure capacity for one more enqueue; return resize copies.
        if (this._size < this._capacity) {  // Fast path: free space exists.
            return 0;  // No resize needed.
        }  // Close fast path.
        return this._resize(this._capacity * 2);  // Double capacity and return copies.
    }  // End _ensureCapacityForOneMore().

    enqueue(value) {  // Add to rear (amortized O(1)).
        const copied = this._ensureCapacityForOneMore();  // Resize if needed.
        const tailIndex = this._indexAt(this._size);  // Compute tail index where new element is written.
        this._data[tailIndex] = value;  // Store value at tail position.
        this._size += 1;  // Increase size.
        return { copied, moved: 0 };  // moved stays 0 (no shifting).
    }  // End enqueue().

    peek() {  // Return front value without removing it (O(1)).
        if (this._size === 0) {  // Reject peeking an empty queue.
            throw new RangeError("peek from empty queue");  // Signal invalid operation.
        }  // Close validation.
        return this._data[this._head];  // Return front slot directly.
    }  // End peek().

    dequeue() {  // Remove and return front value (O(1); no shifting).
        if (this._size === 0) {  // Reject dequeuing an empty queue.
            throw new RangeError("dequeue from empty queue");  // Signal invalid operation.
        }  // Close validation.
        const value = this._data[this._head];  // Capture front value.
        this._data[this._head] = undefined;  // Clear slot for debug friendliness.
        this._head = (this._head + 1) % this._capacity;  // Advance head by one with wrap-around.
        this._size -= 1;  // Decrease size.
        if (this._size === 0) {  // Normalize head for deterministic state when queue becomes empty.
            this._head = 0;  // Reset head (optional but stable).
        }  // Close normalize branch.
        return { value, cost: { copied: 0, moved: 0 } };  // moved stays 0 (no shift).
    }  // End dequeue().
}  // End CircularQueue.

function simulateEnqueues(m) {  // Simulate m enqueues and summarize growth/copy costs.
    if (m < 0) {  // Reject invalid counts.
        throw new RangeError("m must be >= 0");  // Signal invalid input.
    }  // Close validation.
    const q = new CircularQueue();  // Fresh queue for deterministic results.
    let totalActualCost = 0;  // Accumulate total cost (1 write + copied).
    let maxCopied = 0;  // Track maximum copied elements in a single enqueue.
    for (let i = 0; i < m; i += 1) {  // Perform m enqueues.
        const cost = q.enqueue(i);  // Enqueue deterministic value and get per-op cost.
        totalActualCost += 1 + cost.copied;  // Add write cost + resize copy cost.
        maxCopied = Math.max(maxCopied, cost.copied);  // Update maximum copied.
    }  // Close loop.
    return {  // Return a compact summary object.
        m,  // Store operation count.
        finalSize: q.size,  // Store final size.
        finalCapacity: q.capacity,  // Store final capacity.
        totalCopies: q.totalCopies,  // Store total copies.
        totalActualCost,  // Store total actual cost.
        maxCopiedInOneOp: maxCopied,  // Store max per-op copied.
    };  // Close summary.
}  // End simulateEnqueues().

function buildOrderedQueue(n) {  // Build a queue containing [0, 1, ..., n-1].
    if (n < 0) {  // Reject invalid sizes.
        throw new RangeError("n must be >= 0");  // Signal invalid input.
    }  // Close validation.
    const q = new CircularQueue();  // Create empty queue.
    for (let v = 0; v < n; v += 1) {  // Enqueue in increasing order.
        q.enqueue(v);  // Enqueue one value.
    }  // Close loop.
    return q;  // Return built queue.
}  // End buildOrderedQueue().

function simulateDequeueCostAtSize(n) {  // Build size n queue then dequeue once and return cost.
    const q = buildOrderedQueue(n);  // Build queue of size n.
    return q.dequeue().cost;  // Dequeue once and return cost.
}  // End simulateDequeueCostAtSize().

function parseMsOrDefault(argv) {  // Parse CLI args into m values or use defaults.
    if (argv.length === 0) {  // Use defaults when no args are provided.
        return [0, 1, 2, 4, 8, 16, 32];  // Default m values.
    }  // Close default branch.
    return argv.map((x) => Number.parseInt(x, 10));  // Parse each token to int.
}  // End parseMsOrDefault().

function formatEnqueueSummaryTable(ms) {  // Format enqueue-growth summaries for multiple m values.
    const header = ["m", "cap", "copies", "avg", "maxCp"].map((s) => s.padStart(6)).join(" | ");  // Header line.
    const separator = "-".repeat(header.length);  // Separator line.
    const lines = [header, separator];  // Start with header + separator.
    for (const m of ms) {  // Render one row per m.
        const s = simulateEnqueues(m);  // Simulate and summarize.
        const avg = s.m > 0 ? (s.totalActualCost / s.m) : 0.0;  // Average actual cost per op.
        lines.push(  // Append formatted row.
            String(s.m).padStart(6) + " | " +  // Column: m.
            String(s.finalCapacity).padStart(6) + " | " +  // Column: capacity.
            String(s.totalCopies).padStart(6) + " | " +  // Column: total copies.
            avg.toFixed(2).padStart(6) + " | " +  // Column: avg.
            String(s.maxCopiedInOneOp).padStart(6)  // Column: max copied.
        );  // Close push.
    }  // Close loop.
    return lines.join("\n");  // Join lines into printable table.
}  // End formatEnqueueSummaryTable().

function formatDequeueCostTable(ns) {  // Show dequeue moved costs at multiple sizes (should be 0).
    const header = ["n", "moved"].map((s) => s.padStart(6)).join(" | ");  // Header line.
    const separator = "-".repeat(header.length);  // Separator line.
    const lines = [header, separator];  // Start with header + separator.
    for (const n of ns) {  // Render one row per n.
        const moved = n === 0 ? 0 : simulateDequeueCostAtSize(n).moved;  // Skip invalid dequeue on empty queue.
        lines.push(String(n).padStart(6) + " | " + String(moved).padStart(6));  // Append row.
    }  // Close loop.
    return lines.join("\n");  // Join lines.
}  // End formatDequeueCostTable().

function formatFifoDemo() {  // Show a small FIFO example with peek/dequeue.
    const q = new CircularQueue();  // Create an empty queue.
    q.enqueue(3);  // Enqueue 3.
    q.enqueue(7);  // Enqueue 7.
    q.enqueue(1);  // Enqueue 1.
    const front = q.peek();  // Peek current front.
    const d1 = q.dequeue().value;  // Dequeue first (should be 3).
    const d2 = q.dequeue().value;  // Dequeue second (should be 7).
    const d3 = q.dequeue().value;  // Dequeue third (should be 1).
    return `enqueue [3,7,1] => peek=${front}, dequeues=[${d1},${d2},${d3}]`;  // Return formatted demo line.
}  // End formatFifoDemo().

function main(argv) {  // CLI entry point for the demo.
    const ms = parseMsOrDefault(argv);  // Parse m values or use defaults.
    const ns = [0, 1, 2, 4, 8, 16];  // Fixed n list for dequeue costs.
    console.log("=== Enqueue Growth (m enqueues) ===");  // Print section title.
    console.log(formatEnqueueSummaryTable(ms));  // Print enqueue growth table.
    console.log("");  // Blank line.
    console.log("=== Dequeue Shift Cost (should be 0) ===");  // Print section title.
    console.log(formatDequeueCostTable(ns));  // Print dequeue moved table.
    console.log("");  // Blank line.
    console.log("=== FIFO demo ===");  // Print section title.
    console.log(formatFifoDemo());  // Print FIFO demo line.
}  // End main().

module.exports = {  // Export public API for tests and reuse.
    CircularQueue,  // Export CircularQueue class.
    buildOrderedQueue,  // Export helper for building [0..n-1].
};  // Close exports.

if (require.main === module) {  // Execute demo only when run directly.
    main(process.argv.slice(2));  // Run main with CLI args.
}  // Close direct-run guard.

