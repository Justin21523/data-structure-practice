// 04 雙向鏈結串列示範（Java）/ Doubly linked list demo (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to store demo size lists (n values).  
import java.util.List;  // Use List for type clarity.  

public final class DoublyLinkedListDemo {  // Provide a demo + a doubly linked list implementation with explicit traversal costs.  
    private DoublyLinkedListDemo() {  // Prevent instantiation because this class is a container for static logic.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    public static final class OperationCost {  // Record the traversal cost of one operation.  
        public final int hops;  // Number of pointer traversals performed (next or prev).  

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

    private static final class Node {  // A doubly linked list node (value + prev/next pointers).  
        public final int value;  // Store node value.  
        public Node prev;  // Store previous pointer (mutable for linking).  
        public Node next;  // Store next pointer (mutable for linking).  

        private Node(int value, Node prev, Node next) {  // Construct a node with explicit neighbors.  
            this.value = value;  // Store value.  
            this.prev = prev;  // Store prev pointer.  
            this.next = next;  // Store next pointer.  
        }  // Close constructor.  
    }  // End Node.  

    private static final class NodeWithCost {  // Return a located node plus traversal cost.  
        public final Node node;  // Located node.  
        public final OperationCost cost;  // Traversal cost.  

        private NodeWithCost(Node node, OperationCost cost) {  // Construct immutable node+cost record.  
            this.node = node;  // Store node.  
            this.cost = cost;  // Store cost.  
        }  // Close constructor.  
    }  // End NodeWithCost.  

    public static final class DoublyLinkedList {  // A head+tail doubly linked list (teaching-oriented).  
        private Node head;  // Head pointer (null means empty).  
        private Node tail;  // Tail pointer (null means empty).  
        private int size;  // Track size in O(1).  

        public DoublyLinkedList() {  // Initialize an empty list.  
            this.head = null;  // Empty list has null head.  
            this.tail = null;  // Empty list has null tail.  
            this.size = 0;  // Empty list has size 0.  
        }  // Close constructor.  

        public int size() {  // Expose size for callers/tests.  
            return this.size;  // Return stored size.  
        }  // End size().  

        public boolean isEmpty() {  // Convenience check.  
            return this.size == 0;  // Empty iff size is zero.  
        }  // End isEmpty().  

        public int[] toArray() {  // Convert list contents to a plain int array for easy assertions.  
            int[] out = new int[this.size];  // Allocate array of exact size.  
            Node current = this.head;  // Start at head.  
            int i = 0;  // Track write index.  
            while (current != null) {  // Traverse until end.  
                out[i] = current.value;  // Store one value.  
                i += 1;  // Advance output index.  
                current = current.next;  // Move forward via next.  
            }  // Close traversal loop.  
            return out;  // Return copied array.  
        }  // End toArray().  

        private void requireIndexInRange(int index) {  // Validate index in [0, size-1].  
            if (index < 0 || index >= this.size) {  // Reject invalid indices.  
                throw new IndexOutOfBoundsException("index out of range");  // Signal invalid index.  
            }  // Close validation.  
        }  // End requireIndexInRange.  

        private void requireInsertIndexInRange(int index) {  // Validate insert index in [0, size].  
            if (index < 0 || index > this.size) {  // Reject invalid insertion indices.  
                throw new IndexOutOfBoundsException("index out of range for insert");  // Signal invalid index.  
            }  // Close validation.  
        }  // End requireInsertIndexInRange.  

        private NodeWithCost nodeAtWithCost(int index) {  // Locate node at index and report traversal hops.  
            requireIndexInRange(index);  // Validate index.  

            if (index < this.size / 2) {  // Traverse from head when index is in the first half.  
                int hops = 0;  // Count pointer traversals.  
                Node current = this.head;  // Start from head.  
                for (int i = 0; i < index; i++) {  // Move forward index times.  
                    current = current.next;  // Traverse one next pointer.  
                    hops += 1;  // Count one hop.  
                }  // Close loop.  
                return new NodeWithCost(current, new OperationCost(hops));  // Return node + cost.  
            }  // Close head-traversal branch.  

            int steps = this.size - 1 - index;  // How many prev steps needed to reach index from tail.  
            int hops = 0;  // Count pointer traversals.  
            Node current = this.tail;  // Start from tail.  
            for (int i = 0; i < steps; i++) {  // Move backward `steps` times.  
                current = current.prev;  // Traverse one prev pointer.  
                hops += 1;  // Count one hop.  
            }  // Close loop.  
            return new NodeWithCost(current, new OperationCost(hops));  // Return node + cost.  
        }  // End nodeAtWithCost.  

        public OperationCost pushFront(int value) {  // Insert at head (O(1), hops=0).  
            Node newHead = new Node(value, null, this.head);  // New node points forward to old head.  
            if (this.head == null) {  // Empty list case.  
                this.head = newHead;  // Head becomes new node.  
                this.tail = newHead;  // Tail becomes new node.  
                this.size = 1;  // Size becomes 1.  
                return new OperationCost(0);  // No traversal.  
            }  // Close empty case.  
            this.head.prev = newHead;  // Old head points back to new head.  
            this.head = newHead;  // Update head pointer.  
            this.size += 1;  // Update size.  
            return new OperationCost(0);  // No traversal.  
        }  // End pushFront.  

        public OperationCost pushBack(int value) {  // Insert at tail (O(1) with a tail pointer).  
            Node newTail = new Node(value, this.tail, null);  // New node points back to old tail.  
            if (this.tail == null) {  // Empty list case.  
                this.head = newTail;  // Head becomes new node.  
                this.tail = newTail;  // Tail becomes new node.  
                this.size = 1;  // Size becomes 1.  
                return new OperationCost(0);  // No traversal.  
            }  // Close empty case.  
            this.tail.next = newTail;  // Old tail points forward to new tail.  
            this.tail = newTail;  // Update tail pointer.  
            this.size += 1;  // Update size.  
            return new OperationCost(0);  // No traversal.  
        }  // End pushBack.  

        public RemoveResult popFront() {  // Remove from head (O(1), hops=0).  
            if (this.head == null) {  // Reject popping from an empty list.  
                throw new IndexOutOfBoundsException("pop from empty list");  // Signal invalid operation.  
            }  // Close empty check.  
            int removedValue = this.head.value;  // Capture removed value.  
            Node newHead = this.head.next;  // Candidate new head.  
            if (newHead == null) {  // Removing the only node empties the list.  
                this.head = null;  // Clear head.  
                this.tail = null;  // Clear tail.  
                this.size = 0;  // Reset size.  
                return new RemoveResult(removedValue, new OperationCost(0));  // No traversal.  
            }  // Close single-node case.  
            newHead.prev = null;  // New head has no previous node.  
            this.head = newHead;  // Update head.  
            this.size -= 1;  // Update size.  
            return new RemoveResult(removedValue, new OperationCost(0));  // No traversal.  
        }  // End popFront.  

        public RemoveResult popBack() {  // Remove from tail (O(1), hops=0).  
            if (this.tail == null) {  // Reject popping from an empty list.  
                throw new IndexOutOfBoundsException("pop from empty list");  // Signal invalid operation.  
            }  // Close empty check.  
            int removedValue = this.tail.value;  // Capture removed value.  
            Node newTail = this.tail.prev;  // Candidate new tail.  
            if (newTail == null) {  // Removing the only node empties the list.  
                this.head = null;  // Clear head.  
                this.tail = null;  // Clear tail.  
                this.size = 0;  // Reset size.  
                return new RemoveResult(removedValue, new OperationCost(0));  // No traversal.  
            }  // Close single-node case.  
            newTail.next = null;  // New tail has no next node.  
            this.tail = newTail;  // Update tail.  
            this.size -= 1;  // Update size.  
            return new RemoveResult(removedValue, new OperationCost(0));  // No traversal.  
        }  // End popBack.  

        public GetResult getWithCost(int index) {  // Get value at index with traversal cost.  
            NodeWithCost nwc = nodeAtWithCost(index);  // Locate node and cost.  
            return new GetResult(nwc.node.value, nwc.cost);  // Return value + cost.  
        }  // End getWithCost.  

        public OperationCost insertAt(int index, int value) {  // Insert at index and return traversal cost.  
            requireInsertIndexInRange(index);  // Validate insertion index.  
            if (index == 0) {  // Head insertion is O(1).  
                return pushFront(value);  // Delegate to pushFront.  
            }  // Close head case.  
            if (index == this.size) {  // Tail insertion is O(1).  
                return pushBack(value);  // Delegate to pushBack.  
            }  // Close tail case.  

            NodeWithCost target = nodeAtWithCost(index);  // Locate target node currently at index.  
            Node before = target.node.prev;  // Predecessor node.  
            Node newNode = new Node(value, before, target.node);  // New node sits before target.  
            before.next = newNode;  // Predecessor points forward to new node.  
            target.node.prev = newNode;  // Target points back to new node.  
            this.size += 1;  // Update size.  
            return target.cost;  // Traversal cost is the cost to find the insertion position.  
        }  // End insertAt.  

        public RemoveResult removeAt(int index) {  // Remove at index and return removed value + traversal cost.  
            requireIndexInRange(index);  // Validate index.  
            if (index == 0) {  // Head removal is O(1).  
                return popFront();  // Delegate to popFront.  
            }  // Close head case.  
            if (index == this.size - 1) {  // Tail removal is O(1).  
                return popBack();  // Delegate to popBack.  
            }  // Close tail case.  

            NodeWithCost target = nodeAtWithCost(index);  // Locate node to remove.  
            Node before = target.node.prev;  // Predecessor.  
            Node after = target.node.next;  // Successor.  
            before.next = after;  // Bypass target from the left.  
            after.prev = before;  // Bypass target from the right.  
            this.size -= 1;  // Update size.  
            return new RemoveResult(target.node.value, target.cost);  // Return removed value + traversal cost.  
        }  // End removeAt.  

        public FindResult indexOfWithCost(int value) {  // Find first index of value and return traversal cost.  
            Node current = this.head;  // Start from head.  
            int index = 0;  // Track index.  
            int hops = 0;  // Count next traversals.  
            while (current != null) {  // Traverse until end.  
                if (current.value == value) {  // Check for match.  
                    return new FindResult(index, new OperationCost(hops));  // Return index + cost.  
                }  // Close match branch.  
                current = current.next;  // Move to next node.  
                hops += 1;  // Count one hop.  
                index += 1;  // Advance index.  
            }  // Close traversal loop.  
            return new FindResult(-1, new OperationCost(hops));  // Not found sentinel with cost.  
        }  // End indexOfWithCost.  
    }  // End DoublyLinkedList.  

    public static DoublyLinkedList buildOrderedList(int n) {  // Build a list containing [0, 1, ..., n-1].  
        if (n < 0) {  // Reject invalid sizes.  
            throw new IllegalArgumentException("n must be >= 0");  // Signal invalid input.  
        }  // Close validation.  
        DoublyLinkedList lst = new DoublyLinkedList();  // Create empty list.  
        for (int v = 0; v < n; v++) {  // Append in increasing order (pushBack is O(1) with tail).  
            lst.pushBack(v);  // Append one value.  
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
            ns.add(16);  // Include n=16.  
            return ns;  // Return defaults.  
        }  // Close default branch.  
        for (String raw : args) {  // Parse each CLI token.  
            ns.add(Integer.parseInt(raw));  // Convert token to int.  
        }  // Close loop.  
        return ns;  // Return parsed list.  
    }  // End parseNs.  

    private static String formatPushPopTable(List<Integer> ns) {  // Format push/pop costs at both ends for multiple sizes.  
        String header = String.format("%6s | %6s | %6s | %6s | %6s", "n", "pushB", "popB", "pushF", "popF");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator line.  
        StringBuilder sb = new StringBuilder();  // Accumulate output.  
        sb.append(header).append('\n');  // Append header.  
        sb.append(separator).append('\n');  // Append separator.  
        for (int n : ns) {  // Render one row per n.  
            DoublyLinkedList a = buildOrderedList(n);  // Build list of size n.  
            int pushB = a.pushBack(999).hops;  // Measure pushBack hops (should be 0).  
            int popB = (a.size() > 0) ? a.popBack().cost.hops : 0;  // Measure popBack hops (0; handle n=0).  
            DoublyLinkedList b = buildOrderedList(n);  // Build list of size n.  
            int pushF = b.pushFront(999).hops;  // Measure pushFront hops (should be 0).  
            int popF = (b.size() > 0) ? b.popFront().cost.hops : 0;  // Measure popFront hops (0; handle n=0).  
            sb.append(String.format("%6d | %6d | %6d | %6d | %6d", n, pushB, popB, pushF, popF)).append('\n');  // Append row.  
        }  // Close loop.  
        return sb.toString();  // Return table string.  
    }  // End formatPushPopTable.  

    private static String formatGetHopsTable(int n, int[] indices) {  // Show get(i) hops on an ordered list of size n.  
        DoublyLinkedList lst = buildOrderedList(n);  // Build ordered list.  
        String header = String.format("%6s | %6s | %6s", "i", "value", "hops");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator line.  
        StringBuilder sb = new StringBuilder();  // Accumulate output.  
        sb.append(header).append('\n');  // Append header.  
        sb.append(separator).append('\n');  // Append separator.  
        for (int i : indices) {  // Render one row per index.  
            GetResult r = lst.getWithCost(i);  // Fetch value + cost.  
            sb.append(String.format("%6d | %6d | %6d", i, r.value, r.cost.hops)).append('\n');  // Append row.  
        }  // Close loop.  
        return sb.toString();  // Return table string.  
    }  // End formatGetHopsTable.  

    public static void main(String[] args) {  // CLI entry point for the demo.  
        List<Integer> ns = parseNs(args);  // Determine list sizes.  
        System.out.println("=== push/pop hops at both ends ===");  // Print section title.  
        System.out.print(formatPushPopTable(ns));  // Print push/pop table.  
        System.out.println();  // Blank line.  
        System.out.println("=== get(i) hops on list of size 16 ===");  // Print section title.  
        System.out.print(formatGetHopsTable(16, new int[] {0, 1, 2, 4, 8, 15}));  // Print get-cost table.  
    }  // End main.  
}  // End DoublyLinkedListDemo.  

