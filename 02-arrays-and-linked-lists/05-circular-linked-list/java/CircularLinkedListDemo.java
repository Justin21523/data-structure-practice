// 05 環狀鏈結串列示範（Java）/ Circular linked list demo (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to store demo size lists (n values).  
import java.util.Arrays;  // Use Arrays.toString for printing list snapshots.  
import java.util.List;  // Use List for type clarity.  

public final class CircularLinkedListDemo {  // Provide a demo + a singly circular linked list implementation with explicit traversal costs.  
    private CircularLinkedListDemo() {  // Prevent instantiation because this class is a container for static logic.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    public static final class OperationCost {  // Record the traversal cost of one operation.  
        public final int hops;  // Number of `next` pointer traversals performed.  

        public OperationCost(int hops) {  // Construct an immutable cost record.  
            this.hops = hops;  // Store hop count.  
        }  // Close constructor.  
    }  // End OperationCost.  

    public static final class GetResult {  // Return fetched value plus traversal cost.  
        public final int value;  // The fetched element value.  
        public final OperationCost cost;  // Traversal cost record.  

        public GetResult(int value, OperationCost cost) {  // Construct immutable get result.  
            this.value = value;  // Store fetched value.  
            this.cost = cost;  // Store cost record.  
        }  // Close constructor.  
    }  // End GetResult.  

    public static final class RemoveResult {  // Return removed value plus traversal cost.  
        public final int value;  // The removed element value.  
        public final OperationCost cost;  // Traversal cost record.  

        public RemoveResult(int value, OperationCost cost) {  // Construct immutable remove result.  
            this.value = value;  // Store removed value.  
            this.cost = cost;  // Store cost record.  
        }  // Close constructor.  
    }  // End RemoveResult.  

    public static final class FindResult {  // Return found index plus traversal cost.  
        public final int index;  // Found index, or -1 if not found.  
        public final OperationCost cost;  // Traversal cost record.  

        public FindResult(int index, OperationCost cost) {  // Construct immutable find result.  
            this.index = index;  // Store index.  
            this.cost = cost;  // Store cost record.  
        }  // Close constructor.  
    }  // End FindResult.  

    private static final class Node {  // A singly circular linked list node (value + next).  
        public final int value;  // Store node value.  
        public Node next;  // Next pointer (never null in non-empty circular list).  

        private Node(int value, Node next) {  // Construct node with value and next pointer.  
            this.value = value;  // Store value.  
            this.next = next;  // Store next pointer.  
        }  // Close constructor.  
    }  // End Node.  

    public static final class CircularLinkedList {  // A singly circular linked list implemented with a tail pointer only.  
        private Node tail;  // Tail pointer (null means empty).  
        private int size;  // Track size so we can traverse safely without null terminators.  

        public CircularLinkedList() {  // Initialize an empty list.  
            this.tail = null;  // Empty list has null tail.  
            this.size = 0;  // Empty list has size 0.  
        }  // Close constructor.  

        public int size() {  // Expose size for callers/tests.  
            return this.size;  // Return stored size.  
        }  // End size().  

        public boolean isEmpty() {  // Convenience check.  
            return this.size == 0;  // Empty iff size is zero.  
        }  // End isEmpty().  

        private Node head() {  // Return head node (tail.next) for non-empty list.  
            if (this.tail == null) {  // Reject reading head of empty list.  
                return null;  // Signal empty.  
            }  // Close empty check.  
            return this.tail.next;  // Head is tail.next in a circular list.  
        }  // End head().  

        public int[] toArray() {  // Convert list contents to array (bounded by size to avoid infinite loops).  
            int[] out = new int[this.size];  // Allocate array of exact size.  
            if (this.tail == null) {  // Empty list returns empty array.  
                return out;  // Return empty.  
            }  // Close empty case.  
            Node current = head();  // Start at head.  
            for (int i = 0; i < this.size; i++) {  // Traverse exactly size nodes.  
                out[i] = current.value;  // Store one value.  
                current = current.next;  // Move to next node.  
            }  // Close loop.  
            return out;  // Return collected values.  
        }  // End toArray().  

        private void requireIndexInRange(int index) {  // Validate index in [0, size-1].  
            if (index < 0 || index >= this.size) {  // Reject invalid indices.  
                throw new IndexOutOfBoundsException("index out of range");  // Signal invalid index.  
            }  // Close validation.  
        }  // End requireIndexInRange.  

        public OperationCost pushBack(int value) {  // Insert at tail (O(1), hops=0).  
            if (this.tail == null) {  // Empty list case.  
                Node node = new Node(value, null);  // Create node with temporary next.  
                node.next = node;  // Point to itself to form a 1-node circle.  
                this.tail = node;  // Tail becomes node.  
                this.size = 1;  // Size becomes 1.  
                return new OperationCost(0);  // No traversal.  
            }  // Close empty case.  

            Node head = this.tail.next;  // Read current head.  
            Node node = new Node(value, head);  // New node points to head.  
            this.tail.next = node;  // Old tail points to new node.  
            this.tail = node;  // New node becomes tail.  
            this.size += 1;  // Update size.  
            return new OperationCost(0);  // No traversal.  
        }  // End pushBack.  

        public OperationCost pushFront(int value) {  // Insert at head (O(1), hops=0).  
            if (this.tail == null) {  // Empty list case.  
                Node node = new Node(value, null);  // Create node with temporary next.  
                node.next = node;  // Point to itself to form a 1-node circle.  
                this.tail = node;  // Tail becomes node.  
                this.size = 1;  // Size becomes 1.  
                return new OperationCost(0);  // No traversal.  
            }  // Close empty case.  

            Node head = this.tail.next;  // Read current head.  
            Node node = new Node(value, head);  // New node points to current head.  
            this.tail.next = node;  // Tail now points to new head.  
            this.size += 1;  // Update size.  
            return new OperationCost(0);  // No traversal.  
        }  // End pushFront.  

        public RemoveResult popFront() {  // Remove head (O(1), hops=0).  
            if (this.tail == null) {  // Reject popping from empty list.  
                throw new IndexOutOfBoundsException("pop from empty list");  // Signal invalid operation.  
            }  // Close empty check.  
            Node head = this.tail.next;  // Head is tail.next.  
            int removedValue = head.value;  // Capture removed value.  
            if (this.size == 1) {  // Removing the only node empties the list.  
                this.tail = null;  // Clear tail.  
                this.size = 0;  // Reset size.  
                return new RemoveResult(removedValue, new OperationCost(0));  // No traversal.  
            }  // Close single-node case.  
            this.tail.next = head.next;  // Bypass head by linking tail to new head.  
            this.size -= 1;  // Update size.  
            return new RemoveResult(removedValue, new OperationCost(0));  // No traversal.  
        }  // End popFront.  

        public RemoveResult popBack() {  // Remove tail (O(n) in singly circular list), return removed value + hops.  
            if (this.tail == null) {  // Reject popping from empty list.  
                throw new IndexOutOfBoundsException("pop from empty list");  // Signal invalid operation.  
            }  // Close empty check.  
            int removedValue = this.tail.value;  // Capture removed value.  
            if (this.size == 1) {  // Removing the only node empties the list.  
                this.tail = null;  // Clear tail.  
                this.size = 0;  // Reset size.  
                return new RemoveResult(removedValue, new OperationCost(0));  // No traversal.  
            }  // Close single-node case.  

            Node head = this.tail.next;  // Read head.  
            Node current = head;  // Start from head to find predecessor.  
            int hops = 0;  // Count next traversals.  
            while (current.next != this.tail) {  // Stop when current is predecessor of tail.  
                current = current.next;  // Traverse one next pointer.  
                hops += 1;  // Count one hop.  
            }  // Close loop.  
            current.next = this.tail.next;  // Predecessor now points to head.  
            this.tail = current;  // Predecessor becomes new tail.  
            this.size -= 1;  // Update size.  
            return new RemoveResult(removedValue, new OperationCost(hops));  // Return removed value + traversal cost.  
        }  // End popBack.  

        public OperationCost rotate(int steps) {  // Rotate by moving tail forward `steps` times (left rotation).  
            if (steps < 0) {  // Reject negative rotations for simplicity.  
                throw new IllegalArgumentException("steps must be >= 0");  // Signal invalid input.  
            }  // Close validation.  
            if (this.tail == null || this.size <= 1) {  // Rotating empty/1-node list changes nothing.  
                return new OperationCost(0);  // No traversal.  
            }  // Close trivial case.  
            int stepsMod = steps % this.size;  // Reduce steps to avoid unnecessary cycles.  
            int hops = 0;  // Count traversals.  
            for (int i = 0; i < stepsMod; i++) {  // Advance tail stepsMod times.  
                this.tail = this.tail.next;  // Move tail forward one node.  
                hops += 1;  // Count one hop.  
            }  // Close loop.  
            return new OperationCost(hops);  // Return traversal cost.  
        }  // End rotate.  

        public GetResult getWithCost(int index) {  // Get element at index (O(index) traversal).  
            requireIndexInRange(index);  // Validate index.  
            Node current = head();  // Start from head.  
            int hops = 0;  // Count next traversals.  
            for (int i = 0; i < index; i++) {  // Move forward index times.  
                current = current.next;  // Traverse one next pointer.  
                hops += 1;  // Count one hop.  
            }  // Close loop.  
            return new GetResult(current.value, new OperationCost(hops));  // Return value + cost.  
        }  // End getWithCost.  

        public FindResult indexOfWithCost(int value) {  // Find first index of value and return traversal cost.  
            if (this.tail == null) {  // Empty list cannot contain the value.  
                return new FindResult(-1, new OperationCost(0));  // Return not found with zero cost.  
            }  // Close empty case.  
            Node current = this.tail.next;  // Start at head.  
            int hops = 0;  // Count next traversals.  
            for (int index = 0; index < this.size; index++) {  // Traverse exactly size nodes.  
                if (current.value == value) {  // Check for match.  
                    return new FindResult(index, new OperationCost(hops));  // Return index + cost.  
                }  // Close match branch.  
                current = current.next;  // Move to next node.  
                hops += 1;  // Count one hop.  
            }  // Close loop.  
            return new FindResult(-1, new OperationCost(hops));  // Not found: hops equals size.  
        }  // End indexOfWithCost.  
    }  // End CircularLinkedList.  

    public static CircularLinkedList buildOrderedList(int n) {  // Build a list containing [0, 1, ..., n-1].  
        if (n < 0) {  // Reject invalid sizes.  
            throw new IllegalArgumentException("n must be >= 0");  // Signal invalid input.  
        }  // Close validation.  
        CircularLinkedList lst = new CircularLinkedList();  // Create empty list.  
        for (int v = 0; v < n; v++) {  // Append in increasing order.  
            lst.pushBack(v);  // pushBack is O(1) with tail pointer.  
        }  // Close loop.  
        return lst;  // Return built list.  
    }  // End buildOrderedList.  

    private static List<Integer> parseNs(String[] args) {  // Parse CLI args into a list of n values.  
        List<Integer> ns = new ArrayList<>();  // Accumulate n values.  
        if (args.length == 0) {  // Use defaults when no args are provided.  
            ns.add(0);  // Include n=0.  
            ns.add(1);  // Include n=1.  
            ns.add(2);  // Include n=2.  
            ns.add(4);  // Include n=4.  
            ns.add(8);  // Include n=8.  
            return ns;  // Return defaults.  
        }  // Close default branch.  
        for (String raw : args) {  // Parse each CLI token.  
            ns.add(Integer.parseInt(raw));  // Convert token to int.  
        }  // Close loop.  
        return ns;  // Return parsed list.  
    }  // End parseNs.  

    private static String formatBasicCostTable(List<Integer> ns) {  // Show push/pop costs at multiple sizes.  
        String header = String.format("%6s | %6s | %6s | %6s | %6s", "n", "pushB", "pushF", "popF", "popB");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator line.  
        StringBuilder sb = new StringBuilder();  // Accumulate output.  
        sb.append(header).append('\n');  // Append header.  
        sb.append(separator).append('\n');  // Append separator.  
        for (int n : ns) {  // Render one row per n.  
            CircularLinkedList a = buildOrderedList(n);  // Build list of size n.  
            int pushB = a.pushBack(999).hops;  // pushBack cost (0).  
            CircularLinkedList b = buildOrderedList(n);  // Build list of size n.  
            int pushF = b.pushFront(999).hops;  // pushFront cost (0).  
            CircularLinkedList c = buildOrderedList(n);  // Build list of size n.  
            int popF = (c.size() > 0) ? c.popFront().cost.hops : 0;  // popFront cost (0; handle n=0).  
            CircularLinkedList d = buildOrderedList(n);  // Build list of size n.  
            int popB = (d.size() > 0) ? d.popBack().cost.hops : 0;  // popBack cost (n-2; handle n=0).  
            sb.append(String.format("%6d | %6d | %6d | %6d | %6d", n, pushB, pushF, popF, popB)).append('\n');  // Append row.  
        }  // Close loop.  
        return sb.toString();  // Return table string.  
    }  // End formatBasicCostTable.  

    private static String formatRotateTable(int n, int[] stepsList) {  // Show rotate cost and resulting head value.  
        CircularLinkedList lst = buildOrderedList(n);  // Build base list.  
        String header = String.format("%6s | %6s | %6s | %20s", "steps", "hops", "head", "list");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator line.  
        StringBuilder sb = new StringBuilder();  // Accumulate output.  
        sb.append(header).append('\n');  // Append header.  
        sb.append(separator).append('\n');  // Append separator.  
        for (int steps : stepsList) {  // Render one row per rotation (cumulative).  
            OperationCost cost = lst.rotate(steps);  // Rotate list.  
            int headValue = (lst.size() > 0) ? lst.toArray()[0] : -1;  // Read head value (or -1 for empty).  
            String snapshot = Arrays.toString(lst.toArray());  // Snapshot list for demo readability.  
            sb.append(String.format("%6d | %6d | %6d | %20s", steps, cost.hops, headValue, snapshot)).append('\n');  // Append row.  
        }  // Close loop.  
        return sb.toString();  // Return table string.  
    }  // End formatRotateTable.  

    public static void main(String[] args) {  // CLI entry point for the demo.  
        List<Integer> ns = parseNs(args);  // Determine list sizes.  
        System.out.println("=== basic costs (push/pop) ===");  // Print section title.  
        System.out.print(formatBasicCostTable(ns));  // Print cost table.  
        System.out.println();  // Blank line.  
        System.out.println("=== rotate demo on n=5 ===");  // Print section title.  
        System.out.print(formatRotateTable(5, new int[] {0, 1, 2, 5, 7}));  // Print rotation table.  
    }  // End main.  
}  // End CircularLinkedListDemo.  

