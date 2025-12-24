// 03 單向鏈結串列示範（JavaScript）/ Singly linked list demo (JavaScript).  // Bilingual file header.
// We implement a head-only singly linked list and count traversal cost as `hops`.  // English overview.

"use strict";  // Enable strict mode to avoid accidental globals.  

class Node {  // A singly linked list node (value + next pointer).  
    constructor(value, next) {  // Create a node with value and next reference.  
        this.value = value;  // Store node value.  
        this.next = next;  // Store next reference (or null).  
    }  // Close constructor.  
}  // End Node.  

class SinglyLinkedList {  // A teaching-oriented singly linked list (head-only, no tail).  
    constructor() {  // Initialize an empty list.  
        this._head = null;  // Head pointer (null means empty).  
        this._size = 0;  // Track size in O(1).  
    }  // Close constructor.  

    get size() {  // Expose size for callers/tests.  
        return this._size;  // Return stored size.  
    }  // End size getter.  

    isEmpty() {  // Convenience method to check emptiness.  
        return this._size === 0;  // Empty iff size is zero.  
    }  // End isEmpty.  

    toArray() {  // Convert list contents to a normal JS array.  
        const out = [];  // Accumulate values.  
        let current = this._head;  // Start from head.  
        while (current !== null) {  // Stop at end.  
            out.push(current.value);  // Append one value.  
            current = current.next;  // Move to next node.  
        }  // Close traversal loop.  
        return out;  // Return collected values.  
    }  // End toArray.  

    _requireIndexInRange(index) {  // Validate index in [0, size-1].  
        if (!Number.isInteger(index) || index < 0 || index >= this._size) {  // Reject invalid indices.  
            throw new RangeError("index out of range");  // Signal invalid index.  
        }  // Close validation.  
    }  // End _requireIndexInRange.  

    _requireInsertIndexInRange(index) {  // Validate insert index in [0, size].  
        if (!Number.isInteger(index) || index < 0 || index > this._size) {  // Reject invalid insertion indices.  
            throw new RangeError("index out of range for insert");  // Signal invalid index.  
        }  // Close validation.  
    }  // End _requireInsertIndexInRange.  

    pushFront(value) {  // Insert at head (O(1), hops=0).  
        this._head = new Node(value, this._head);  // Link new node before current head.  
        this._size += 1;  // Update size.  
        return { hops: 0 };  // Head insertion requires no traversal.  
    }  // End pushFront.  

    pushBack(value) {  // Insert at tail (O(n) without a tail pointer).  
        if (this._head === null) {  // Special-case empty list.  
            this._head = new Node(value, null);  // New node becomes head.  
            this._size = 1;  // Size becomes 1.  
            return { hops: 0 };  // No traversal needed.  
        }  // Close empty case.  

        let hops = 0;  // Count `next` traversals.  
        let current = this._head;  // Start from head.  
        while (current.next !== null) {  // Walk until last node.  
            current = current.next;  // Traverse one link.  
            hops += 1;  // Count one hop.  
        }  // Close traversal loop.  
        current.next = new Node(value, null);  // Link new node after last.  
        this._size += 1;  // Update size.  
        return { hops: hops };  // Return traversal cost.  
    }  // End pushBack.  

    getWithCost(index) {  // Get element at index (O(n) traversal).  
        this._requireIndexInRange(index);  // Validate index.  
        let hops = 0;  // Count `next` traversals.  
        let current = this._head;  // Start from head.  
        for (let i = 0; i < index; i += 1) {  // Move forward index times.  
            current = current.next;  // Traverse one link.  
            hops += 1;  // Count one hop.  
        }  // Close loop.  
        return { value: current.value, cost: { hops: hops } };  // Return value + cost.  
    }  // End getWithCost.  

    insertAt(index, value) {  // Insert at index (O(n) except at head).  
        this._requireInsertIndexInRange(index);  // Validate index.  
        if (index === 0) {  // Special-case head insertion.  
            return this.pushFront(value);  // Delegate to pushFront.  
        }  // Close head case.  

        let hops = 0;  // Count traversal hops.  
        let prev = this._head;  // Start from head (need predecessor).  
        for (let i = 0; i < index - 1; i += 1) {  // Move to predecessor.  
            prev = prev.next;  // Traverse one link.  
            hops += 1;  // Count one hop.  
        }  // Close loop.  
        prev.next = new Node(value, prev.next);  // Splice new node after predecessor.  
        this._size += 1;  // Update size.  
        return { hops: hops };  // Return traversal cost.  
    }  // End insertAt.  

    removeAt(index) {  // Remove node at index and return removed value + cost.  
        this._requireIndexInRange(index);  // Validate index.  
        if (index === 0) {  // Special-case head removal.  
            const removedValue = this._head.value;  // Capture removed head value.  
            this._head = this._head.next;  // Bypass old head.  
            this._size -= 1;  // Update size.  
            return { value: removedValue, cost: { hops: 0 } };  // No traversal needed.  
        }  // Close head case.  

        let hops = 0;  // Count traversal hops.  
        let prev = this._head;  // Start from head (need predecessor).  
        for (let i = 0; i < index - 1; i += 1) {  // Move to predecessor.  
            prev = prev.next;  // Traverse one link.  
            hops += 1;  // Count one hop.  
        }  // Close loop.  
        const removedValue = prev.next.value;  // Capture removed value.  
        prev.next = prev.next.next;  // Bypass removed node.  
        this._size -= 1;  // Update size.  
        return { value: removedValue, cost: { hops: hops } };  // Return value + cost.  
    }  // End removeAt.  

    indexOfWithCost(value) {  // Find first index of value (O(n)).  
        let current = this._head;  // Start from head.  
        let index = 0;  // Track current index.  
        let hops = 0;  // Count `next` traversals.  
        while (current !== null) {  // Traverse until end.  
            if (current.value === value) {  // Check for match.  
                return { index: index, cost: { hops: hops } };  // Return index + cost.  
            }  // Close match branch.  
            current = current.next;  // Move to next node.  
            hops += 1;  // Count one hop.  
            index += 1;  // Advance index.  
        }  // Close traversal loop.  
        return { index: -1, cost: { hops: hops } };  // Not found sentinel with cost.  
    }  // End indexOfWithCost.  
}  // End SinglyLinkedList.  

function buildOrderedList(n) {  // Build a list containing [0, 1, ..., n-1].  
    if (!Number.isInteger(n) || n < 0) {  // Validate n.  
        throw new RangeError("n must be an integer >= 0");  // Signal invalid input.  
    }  // Close validation.  
    const lst = new SinglyLinkedList();  // Create empty list.  
    for (let v = n - 1; v >= 0; v -= 1) {  // Push front in reverse to get ascending order.  
        lst.pushFront(v);  // O(1) head insertion.  
    }  // Close loop.  
    return lst;  // Return built list.  
}  // End buildOrderedList.  

function parseNs(argv) {  // Parse CLI args into n values.  
    if (argv.length === 0) {  // Use defaults when no args.  
        return [0, 1, 2, 4, 8, 16];  // Default sizes.  
    }  // Close default branch.  
    return argv.map((s) => Number.parseInt(s, 10));  // Parse each token.  
}  // End parseNs.  

function formatPushFrontVsBackTable(ns) {  // Format pushFront vs pushBack hops table.  
    const header = `${"n".padStart(6)} | ${"frontH".padStart(6)} | ${"backH".padStart(6)}`;  // Header line.  
    const separator = "-".repeat(header.length);  // Separator line.  
    const lines = [header, separator];  // Start with header + separator.  
    for (const n of ns) {  // Render one row per n.  
        const a = buildOrderedList(n);  // Build list of size n.  
        const front = a.pushFront(999);  // Measure head insertion.  
        const b = buildOrderedList(n);  // Build another list of size n.  
        const back = b.pushBack(999);  // Measure tail insertion.  
        lines.push(`${String(n).padStart(6)} | ${String(front.hops).padStart(6)} | ${String(back.hops).padStart(6)}`);  // Append row.  
    }  // Close loop.  
    return lines.join("\n");  // Join lines.  
}  // End formatPushFrontVsBackTable.  

function formatGetHopsTable(n, indices) {  // Show get(i) hops on an ordered list of size n.  
    const lst = buildOrderedList(n);  // Build ordered list.  
    const header = `${"i".padStart(6)} | ${"value".padStart(6)} | ${"hops".padStart(6)}`;  // Header line.  
    const separator = "-".repeat(header.length);  // Separator line.  
    const lines = [header, separator];  // Start with header + separator.  
    for (const i of indices) {  // Render one row per index.  
        const r = lst.getWithCost(i);  // Fetch value + cost.  
        lines.push(`${String(i).padStart(6)} | ${String(r.value).padStart(6)} | ${String(r.cost.hops).padStart(6)}`);  // Append row.  
    }  // Close loop.  
    return lines.join("\n");  // Join lines.  
}  // End formatGetHopsTable.  

function main(argv) {  // CLI entry point.  
    const ns = parseNs(argv);  // Determine list sizes.  
    console.log("=== pushFront vs pushBack hops at size n ===");  // Print section title.  
    console.log(formatPushFrontVsBackTable(ns));  // Print comparison table.  
    console.log("");  // Blank line.  
    console.log("=== get(i) hops on list of size 16 ===");  // Print section title.  
    console.log(formatGetHopsTable(16, [0, 1, 2, 4, 8, 15]));  // Print get-cost table.  
    return 0;  // Return success code.  
}  // End main.  

module.exports = {  // Export API for tests.  
    Node: Node,  // Export Node (optional, but keeps tests flexible).  
    SinglyLinkedList: SinglyLinkedList,  // Export list class.  
    buildOrderedList: buildOrderedList,  // Export builder helper.  
};  // End exports.  

if (require.main === module) {  // Run CLI only when executed directly.  
    process.exitCode = main(process.argv.slice(2));  // Set exit code for shell integration.  
}  // Close main guard.  

