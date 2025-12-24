// 04 雙向鏈結串列示範（JavaScript）/ Doubly linked list demo (JavaScript).  // Bilingual file header.
// We implement a head+tail doubly linked list and count traversal cost as `hops`.  // English overview.

"use strict";  // Enable strict mode to avoid accidental globals.  

class Node {  // A doubly linked list node (value + prev/next pointers).  
    constructor(value, prev, next) {  // Create a node with explicit neighbors.  
        this.value = value;  // Store node value.  
        this.prev = prev;  // Store previous pointer (or null).  
        this.next = next;  // Store next pointer (or null).  
    }  // Close constructor.  
}  // End Node.  

class DoublyLinkedList {  // A teaching-oriented doubly linked list (head + tail).  
    constructor() {  // Initialize an empty list.  
        this._head = null;  // Head pointer (null means empty).  
        this._tail = null;  // Tail pointer (null means empty).  
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
            current = current.next;  // Move forward via next.  
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

    _nodeAtWithCost(index) {  // Locate node at index and return { node, cost }.  
        this._requireIndexInRange(index);  // Validate index.  
        if (index < Math.floor(this._size / 2)) {  // Traverse from head when index is in the first half.  
            let hops = 0;  // Count pointer traversals.  
            let current = this._head;  // Start from head.  
            for (let i = 0; i < index; i += 1) {  // Move forward index times.  
                current = current.next;  // Traverse one next pointer.  
                hops += 1;  // Count one hop.  
            }  // Close loop.  
            return { node: current, cost: { hops: hops } };  // Return node + cost.  
        }  // Close head traversal branch.  

        const steps = this._size - 1 - index;  // How many prev steps needed from tail.  
        let hops = 0;  // Count pointer traversals.  
        let current = this._tail;  // Start from tail.  
        for (let i = 0; i < steps; i += 1) {  // Move backward `steps` times.  
            current = current.prev;  // Traverse one prev pointer.  
            hops += 1;  // Count one hop.  
        }  // Close loop.  
        return { node: current, cost: { hops: hops } };  // Return node + cost.  
    }  // End _nodeAtWithCost.  

    pushFront(value) {  // Insert at head (O(1), hops=0).  
        const newHead = new Node(value, null, this._head);  // New node points forward to old head.  
        if (this._head === null) {  // Empty list case.  
            this._head = newHead;  // Head becomes new node.  
            this._tail = newHead;  // Tail becomes new node.  
            this._size = 1;  // Size becomes 1.  
            return { hops: 0 };  // No traversal.  
        }  // Close empty case.  
        this._head.prev = newHead;  // Old head points back to new head.  
        this._head = newHead;  // Update head pointer.  
        this._size += 1;  // Update size.  
        return { hops: 0 };  // No traversal.  
    }  // End pushFront.  

    pushBack(value) {  // Insert at tail (O(1) with a tail pointer).  
        const newTail = new Node(value, this._tail, null);  // New node points back to old tail.  
        if (this._tail === null) {  // Empty list case.  
            this._head = newTail;  // Head becomes new node.  
            this._tail = newTail;  // Tail becomes new node.  
            this._size = 1;  // Size becomes 1.  
            return { hops: 0 };  // No traversal.  
        }  // Close empty case.  
        this._tail.next = newTail;  // Old tail points forward to new tail.  
        this._tail = newTail;  // Update tail pointer.  
        this._size += 1;  // Update size.  
        return { hops: 0 };  // No traversal.  
    }  // End pushBack.  

    popFront() {  // Remove from head (O(1), hops=0).  
        if (this._head === null) {  // Reject popping from an empty list.  
            throw new RangeError("pop from empty list");  // Signal invalid operation.  
        }  // Close empty check.  
        const removedValue = this._head.value;  // Capture removed value.  
        const newHead = this._head.next;  // Candidate new head.  
        if (newHead === null) {  // Removing the only node empties the list.  
            this._head = null;  // Clear head.  
            this._tail = null;  // Clear tail.  
            this._size = 0;  // Reset size.  
            return { value: removedValue, cost: { hops: 0 } };  // No traversal.  
        }  // Close single-node case.  
        newHead.prev = null;  // New head has no previous node.  
        this._head = newHead;  // Update head.  
        this._size -= 1;  // Update size.  
        return { value: removedValue, cost: { hops: 0 } };  // No traversal.  
    }  // End popFront.  

    popBack() {  // Remove from tail (O(1), hops=0).  
        if (this._tail === null) {  // Reject popping from an empty list.  
            throw new RangeError("pop from empty list");  // Signal invalid operation.  
        }  // Close empty check.  
        const removedValue = this._tail.value;  // Capture removed value.  
        const newTail = this._tail.prev;  // Candidate new tail.  
        if (newTail === null) {  // Removing the only node empties the list.  
            this._head = null;  // Clear head.  
            this._tail = null;  // Clear tail.  
            this._size = 0;  // Reset size.  
            return { value: removedValue, cost: { hops: 0 } };  // No traversal.  
        }  // Close single-node case.  
        newTail.next = null;  // New tail has no next node.  
        this._tail = newTail;  // Update tail.  
        this._size -= 1;  // Update size.  
        return { value: removedValue, cost: { hops: 0 } };  // No traversal.  
    }  // End popBack.  

    getWithCost(index) {  // Get value at index with traversal cost.  
        const result = this._nodeAtWithCost(index);  // Locate node + cost.  
        return { value: result.node.value, cost: result.cost };  // Return value + cost.  
    }  // End getWithCost.  

    insertAt(index, value) {  // Insert at index and return traversal cost.  
        this._requireInsertIndexInRange(index);  // Validate insertion index.  
        if (index === 0) {  // Head insertion is O(1).  
            return this.pushFront(value);  // Delegate to pushFront.  
        }  // Close head case.  
        if (index === this._size) {  // Tail insertion is O(1).  
            return this.pushBack(value);  // Delegate to pushBack.  
        }  // Close tail case.  

        const target = this._nodeAtWithCost(index);  // Locate target node currently at index.  
        const before = target.node.prev;  // Predecessor node.  
        const newNode = new Node(value, before, target.node);  // New node sits before target.  
        before.next = newNode;  // Predecessor points forward to new node.  
        target.node.prev = newNode;  // Target points back to new node.  
        this._size += 1;  // Update size.  
        return target.cost;  // Traversal cost is the cost to find the insertion position.  
    }  // End insertAt.  

    removeAt(index) {  // Remove at index and return removed value + traversal cost.  
        this._requireIndexInRange(index);  // Validate index.  
        if (index === 0) {  // Head removal is O(1).  
            return this.popFront();  // Delegate to popFront.  
        }  // Close head case.  
        if (index === this._size - 1) {  // Tail removal is O(1).  
            return this.popBack();  // Delegate to popBack.  
        }  // Close tail case.  

        const target = this._nodeAtWithCost(index);  // Locate node to remove.  
        const before = target.node.prev;  // Predecessor.  
        const after = target.node.next;  // Successor.  
        before.next = after;  // Bypass target from the left.  
        after.prev = before;  // Bypass target from the right.  
        this._size -= 1;  // Update size.  
        return { value: target.node.value, cost: target.cost };  // Return removed value + traversal cost.  
    }  // End removeAt.  

    indexOfWithCost(value) {  // Find first index of value and return traversal cost.  
        let current = this._head;  // Start from head.  
        let index = 0;  // Track index.  
        let hops = 0;  // Count next traversals.  
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
}  // End DoublyLinkedList.  

function buildOrderedList(n) {  // Build a list containing [0, 1, ..., n-1].  
    if (!Number.isInteger(n) || n < 0) {  // Validate n.  
        throw new RangeError("n must be an integer >= 0");  // Signal invalid input.  
    }  // Close validation.  
    const lst = new DoublyLinkedList();  // Create empty list.  
    for (let v = 0; v < n; v += 1) {  // Append in increasing order.  
        lst.pushBack(v);  // pushBack is O(1) with tail pointer.  
    }  // Close loop.  
    return lst;  // Return built list.  
}  // End buildOrderedList.  

function parseNs(argv) {  // Parse CLI args into n values.  
    if (argv.length === 0) {  // Use defaults when no args.  
        return [0, 1, 2, 4, 8, 16];  // Default sizes.  
    }  // Close default branch.  
    return argv.map((s) => Number.parseInt(s, 10));  // Parse each token.  
}  // End parseNs.  

function formatPushPopTable(ns) {  // Format push/pop costs at both ends for multiple sizes.  
    const header = `${"n".padStart(6)} | ${"pushB".padStart(6)} | ${"popB".padStart(6)} | ${"pushF".padStart(6)} | ${"popF".padStart(6)}`;  // Header line.  
    const separator = "-".repeat(header.length);  // Separator line.  
    const lines = [header, separator];  // Start with header + separator.  
    for (const n of ns) {  // Render one row per n.  
        const a = buildOrderedList(n);  // Build list of size n.  
        const pushB = a.pushBack(999).hops;  // pushBack hops (should be 0).  
        const popB = a.size > 0 ? a.popBack().cost.hops : 0;  // popBack hops (0; handle n=0).  
        const b = buildOrderedList(n);  // Build list of size n.  
        const pushF = b.pushFront(999).hops;  // pushFront hops (should be 0).  
        const popF = b.size > 0 ? b.popFront().cost.hops : 0;  // popFront hops (0; handle n=0).  
        lines.push(`${String(n).padStart(6)} | ${String(pushB).padStart(6)} | ${String(popB).padStart(6)} | ${String(pushF).padStart(6)} | ${String(popF).padStart(6)}`);  // Append row.  
    }  // Close loop.  
    return lines.join("\n");  // Join lines.  
}  // End formatPushPopTable.  

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
    console.log("=== push/pop hops at both ends ===");  // Print section title.  
    console.log(formatPushPopTable(ns));  // Print push/pop table.  
    console.log("");  // Blank line.  
    console.log("=== get(i) hops on list of size 16 ===");  // Print section title.  
    console.log(formatGetHopsTable(16, [0, 1, 2, 4, 8, 15]));  // Print get-cost table.  
    return 0;  // Return success code.  
}  // End main.  

module.exports = {  // Export API for tests.  
    Node: Node,  // Export Node (optional).  
    DoublyLinkedList: DoublyLinkedList,  // Export list class.  
    buildOrderedList: buildOrderedList,  // Export builder helper.  
};  // End exports.  

if (require.main === module) {  // Run CLI only when executed directly.  
    process.exitCode = main(process.argv.slice(2));  // Set exit code for shell integration.  
}  // Close main guard.  

