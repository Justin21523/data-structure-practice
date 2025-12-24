// 05 環狀鏈結串列示範（JavaScript）/ Circular linked list demo (JavaScript).  // Bilingual file header.
// We implement a singly circular linked list using a tail pointer and count traversal cost as `hops`.  // English overview.

"use strict";  // Enable strict mode to avoid accidental globals.  

class Node {  // A singly circular linked list node (value + next).  
    constructor(value, next) {  // Create a node with value and next reference.  
        this.value = value;  // Store node value.  
        this.next = next;  // Store next pointer (circular in non-empty list).  
    }  // Close constructor.  
}  // End Node.  

class CircularLinkedList {  // A singly circular linked list implemented with a tail pointer only.  
    constructor() {  // Initialize an empty list.  
        this._tail = null;  // Tail pointer (null means empty).  
        this._size = 0;  // Track size to bound traversals (no null terminators).  
    }  // Close constructor.  

    get size() {  // Expose size for callers/tests.  
        return this._size;  // Return stored size.  
    }  // End size getter.  

    isEmpty() {  // Convenience method to check emptiness.  
        return this._size === 0;  // Empty iff size is zero.  
    }  // End isEmpty.  

    _head() {  // Return head node (tail.next) or null for empty list.  
        if (this._tail === null) {  // Empty list has no head.  
            return null;  // Signal empty.  
        }  // Close empty check.  
        return this._tail.next;  // Head is tail.next in a circular list.  
    }  // End _head.  

    toArray() {  // Convert list contents to a normal JS array (bounded by size).  
        const out = [];  // Accumulate values.  
        const head = this._head();  // Read head node.  
        if (head === null) {  // Empty list converts to empty array.  
            return out;  // Return empty array.  
        }  // Close empty case.  
        let current = head;  // Start at head.  
        for (let i = 0; i < this._size; i += 1) {  // Traverse exactly size nodes.  
            out.push(current.value);  // Append one value.  
            current = current.next;  // Move to next node.  
        }  // Close loop.  
        return out;  // Return collected values.  
    }  // End toArray.  

    _requireIndexInRange(index) {  // Validate index in [0, size-1].  
        if (!Number.isInteger(index) || index < 0 || index >= this._size) {  // Reject invalid indices.  
            throw new RangeError("index out of range");  // Signal invalid index.  
        }  // Close validation.  
    }  // End _requireIndexInRange.  

    pushBack(value) {  // Insert at tail (O(1), hops=0).  
        if (this._tail === null) {  // Empty list case.  
            const node = new Node(value, null);  // Create node with temporary next.  
            node.next = node;  // Point to itself to form a 1-node circle.  
            this._tail = node;  // Tail becomes node.  
            this._size = 1;  // Size becomes 1.  
            return { hops: 0 };  // No traversal.  
        }  // Close empty case.  

        const head = this._tail.next;  // Read current head.  
        const node = new Node(value, head);  // New node points to head.  
        this._tail.next = node;  // Old tail points to new node.  
        this._tail = node;  // New node becomes tail.  
        this._size += 1;  // Update size.  
        return { hops: 0 };  // No traversal.  
    }  // End pushBack.  

    pushFront(value) {  // Insert at head (O(1), hops=0).  
        if (this._tail === null) {  // Empty list case.  
            const node = new Node(value, null);  // Create node with temporary next.  
            node.next = node;  // Point to itself to form a 1-node circle.  
            this._tail = node;  // Tail becomes node.  
            this._size = 1;  // Size becomes 1.  
            return { hops: 0 };  // No traversal.  
        }  // Close empty case.  

        const head = this._tail.next;  // Read current head.  
        const node = new Node(value, head);  // New node points to current head.  
        this._tail.next = node;  // Tail now points to new head.  
        this._size += 1;  // Update size.  
        return { hops: 0 };  // No traversal.  
    }  // End pushFront.  

    popFront() {  // Remove head (O(1), hops=0).  
        if (this._tail === null) {  // Reject popping from empty list.  
            throw new RangeError("pop from empty list");  // Signal invalid operation.  
        }  // Close empty check.  
        const head = this._tail.next;  // Head is tail.next.  
        const removedValue = head.value;  // Capture removed value.  
        if (this._size === 1) {  // Removing the only node empties the list.  
            this._tail = null;  // Clear tail.  
            this._size = 0;  // Reset size.  
            return { value: removedValue, cost: { hops: 0 } };  // No traversal.  
        }  // Close single-node case.  
        this._tail.next = head.next;  // Bypass head by linking tail to new head.  
        this._size -= 1;  // Update size.  
        return { value: removedValue, cost: { hops: 0 } };  // No traversal.  
    }  // End popFront.  

    popBack() {  // Remove tail (O(n) in singly circular list), return removed value + hops.  
        if (this._tail === null) {  // Reject popping from empty list.  
            throw new RangeError("pop from empty list");  // Signal invalid operation.  
        }  // Close empty check.  
        const removedValue = this._tail.value;  // Capture removed value.  
        if (this._size === 1) {  // Removing the only node empties the list.  
            this._tail = null;  // Clear tail.  
            this._size = 0;  // Reset size.  
            return { value: removedValue, cost: { hops: 0 } };  // No traversal.  
        }  // Close single-node case.  

        const head = this._tail.next;  // Read head.  
        let current = head;  // Start from head to find predecessor of tail.  
        let hops = 0;  // Count next traversals.  
        while (current.next !== this._tail) {  // Stop when current is predecessor.  
            current = current.next;  // Traverse one next pointer.  
            hops += 1;  // Count one hop.  
        }  // Close loop.  
        current.next = this._tail.next;  // Predecessor now points to head.  
        this._tail = current;  // Predecessor becomes new tail.  
        this._size -= 1;  // Update size.  
        return { value: removedValue, cost: { hops: hops } };  // Return removed value + traversal cost.  
    }  // End popBack.  

    rotate(steps) {  // Rotate by moving tail forward `steps` times (left rotation).  
        if (!Number.isInteger(steps) || steps < 0) {  // Reject invalid steps for simplicity.  
            throw new RangeError("steps must be an integer >= 0");  // Signal invalid input.  
        }  // Close validation.  
        if (this._tail === null || this._size <= 1) {  // Rotating empty/1-node list changes nothing.  
            return { hops: 0 };  // No traversal.  
        }  // Close trivial case.  
        const stepsMod = steps % this._size;  // Reduce steps to avoid unnecessary cycles.  
        let hops = 0;  // Count traversals.  
        for (let i = 0; i < stepsMod; i += 1) {  // Advance tail stepsMod times.  
            this._tail = this._tail.next;  // Move tail forward one node.  
            hops += 1;  // Count one hop.  
        }  // Close loop.  
        return { hops: hops };  // Return traversal cost.  
    }  // End rotate.  

    getWithCost(index) {  // Get element at index (O(index) traversal).  
        this._requireIndexInRange(index);  // Validate index.  
        const head = this._head();  // Read head.  
        let current = head;  // Start from head.  
        let hops = 0;  // Count next traversals.  
        for (let i = 0; i < index; i += 1) {  // Move forward index times.  
            current = current.next;  // Traverse one next pointer.  
            hops += 1;  // Count one hop.  
        }  // Close loop.  
        return { value: current.value, cost: { hops: hops } };  // Return value + cost.  
    }  // End getWithCost.  

    indexOfWithCost(value) {  // Find first index of value and return traversal cost.  
        if (this._tail === null) {  // Empty list cannot contain the value.  
            return { index: -1, cost: { hops: 0 } };  // Return not found with zero cost.  
        }  // Close empty case.  
        let current = this._tail.next;  // Start at head.  
        let hops = 0;  // Count next traversals.  
        for (let index = 0; index < this._size; index += 1) {  // Traverse exactly size nodes.  
            if (current.value === value) {  // Check for match.  
                return { index: index, cost: { hops: hops } };  // Return index + cost.  
            }  // Close match branch.  
            current = current.next;  // Move to next node.  
            hops += 1;  // Count one hop.  
        }  // Close loop.  
        return { index: -1, cost: { hops: hops } };  // Not found: hops equals size.  
    }  // End indexOfWithCost.  
}  // End CircularLinkedList.  

function buildOrderedList(n) {  // Build a list containing [0, 1, ..., n-1].  
    if (!Number.isInteger(n) || n < 0) {  // Validate n.  
        throw new RangeError("n must be an integer >= 0");  // Signal invalid input.  
    }  // Close validation.  
    const lst = new CircularLinkedList();  // Create empty list.  
    for (let v = 0; v < n; v += 1) {  // Append in increasing order.  
        lst.pushBack(v);  // pushBack is O(1) with tail pointer.  
    }  // Close loop.  
    return lst;  // Return built list.  
}  // End buildOrderedList.  

function parseNs(argv) {  // Parse CLI args into n values.  
    if (argv.length === 0) {  // Use defaults when no args.  
        return [0, 1, 2, 4, 8];  // Default sizes.  
    }  // Close default branch.  
    return argv.map((s) => Number.parseInt(s, 10));  // Parse each token.  
}  // End parseNs.  

function formatBasicCostTable(ns) {  // Show push/pop costs at multiple sizes.  
    const header = `${"n".padStart(6)} | ${"pushB".padStart(6)} | ${"pushF".padStart(6)} | ${"popF".padStart(6)} | ${"popB".padStart(6)}`;  // Header line.  
    const separator = "-".repeat(header.length);  // Separator line.  
    const lines = [header, separator];  // Start with header + separator.  
    for (const n of ns) {  // Render one row per n.  
        const a = buildOrderedList(n);  // Build list of size n.  
        const pushB = a.pushBack(999).hops;  // pushBack cost (0).  
        const b = buildOrderedList(n);  // Build list of size n.  
        const pushF = b.pushFront(999).hops;  // pushFront cost (0).  
        const c = buildOrderedList(n);  // Build list of size n.  
        const popF = c.size > 0 ? c.popFront().cost.hops : 0;  // popFront cost (0; handle n=0).  
        const d = buildOrderedList(n);  // Build list of size n.  
        const popB = d.size > 0 ? d.popBack().cost.hops : 0;  // popBack cost (n-2; handle n=0).  
        lines.push(`${String(n).padStart(6)} | ${String(pushB).padStart(6)} | ${String(pushF).padStart(6)} | ${String(popF).padStart(6)} | ${String(popB).padStart(6)}`);  // Append row.  
    }  // Close loop.  
    return lines.join("\n");  // Join lines.  
}  // End formatBasicCostTable.  

function formatRotateTable(n, stepsList) {  // Show rotate cost and resulting head value.  
    const lst = buildOrderedList(n);  // Build base list.  
    const header = `${"steps".padStart(6)} | ${"hops".padStart(6)} | ${"head".padStart(6)} | ${"list".padStart(20)}`;  // Header line.  
    const separator = "-".repeat(header.length);  // Separator line.  
    const lines = [header, separator];  // Start with header + separator.  
    for (const steps of stepsList) {  // Render one row per rotation (cumulative).  
        const cost = lst.rotate(steps);  // Rotate list.  
        const arr = lst.toArray();  // Snapshot list.  
        const headValue = arr.length > 0 ? arr[0] : -1;  // Read head value (or -1 for empty).  
        lines.push(`${String(steps).padStart(6)} | ${String(cost.hops).padStart(6)} | ${String(headValue).padStart(6)} | ${String(JSON.stringify(arr)).padStart(20)}`);  // Append row.  
    }  // Close loop.  
    return lines.join("\n");  // Join lines.  
}  // End formatRotateTable.  

function main(argv) {  // CLI entry point.  
    const ns = parseNs(argv);  // Determine list sizes.  
    console.log("=== basic costs (push/pop) ===");  // Print section title.  
    console.log(formatBasicCostTable(ns));  // Print cost table.  
    console.log("");  // Blank line.  
    console.log("=== rotate demo on n=5 ===");  // Print section title.  
    console.log(formatRotateTable(5, [0, 1, 2, 5, 7]));  // Print rotation table.  
    return 0;  // Return success code.  
}  // End main.  

module.exports = {  // Export API for tests.  
    Node: Node,  // Export Node (optional).  
    CircularLinkedList: CircularLinkedList,  // Export list class.  
    buildOrderedList: buildOrderedList,  // Export builder helper.  
};  // End exports.  

if (require.main === module) {  // Run CLI only when executed directly.  
    process.exitCode = main(process.argv.slice(2));  // Set exit code for shell integration.  
}  // Close main guard.  

