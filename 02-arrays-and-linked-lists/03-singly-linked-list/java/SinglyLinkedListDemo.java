// 03 單向鏈結串列示範（Java）/ Singly linked list demo (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to store demo size lists (n values).  
import java.util.List;  // Use List for type clarity.  

public final class SinglyLinkedListDemo {  // Provide a demo + a singly linked list implementation with explicit traversal costs.  
    private SinglyLinkedListDemo() {  // Prevent instantiation because this class is a container for static logic.  
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

    private static final class Node {  // A singly linked list node (value + next pointer).  
        public final int value;  // Store node value.  
        public Node next;  // Store next pointer (mutable for linking).  

        private Node(int value, Node next) {  // Construct a node with value and next pointer.  
            this.value = value;  // Store value.  
            this.next = next;  // Store next pointer.  
        }  // Close constructor.  
    }  // End Node.  

    public static final class SinglyLinkedList {  // A head-only singly linked list (no tail pointer).  
        private Node head;  // Head pointer (null means empty).  
        private int size;  // Track size in O(1).  

        public SinglyLinkedList() {  // Initialize an empty list.  
            this.head = null;  // Empty list has null head.  
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
            while (current != null) {  // Stop when end is reached.  
                out[i] = current.value;  // Store one value.  
                i += 1;  // Advance output index.  
                current = current.next;  // Move to next node.  
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

        public OperationCost pushFront(int value) {  // Insert at head (O(1), hops=0).  
            this.head = new Node(value, this.head);  // Link new node before current head.  
            this.size += 1;  // Update size.  
            return new OperationCost(0);  // Head insertion requires no traversal.  
        }  // End pushFront.  

        public OperationCost pushBack(int value) {  // Insert at tail (O(n) without a tail pointer).  
            if (this.head == null) {  // Special-case empty list.  
                this.head = new Node(value, null);  // New node becomes head.  
                this.size = 1;  // Size becomes 1.  
                return new OperationCost(0);  // No traversal needed.  
            }  // Close empty case.  

            int hops = 0;  // Count `next` traversals.  
            Node current = this.head;  // Start from head.  
            while (current.next != null) {  // Walk until last node.  
                current = current.next;  // Traverse one link.  
                hops += 1;  // Count one hop.  
            }  // Close traversal loop.  
            current.next = new Node(value, null);  // Link new node after last.  
            this.size += 1;  // Update size.  
            return new OperationCost(hops);  // Return traversal cost.  
        }  // End pushBack.  

        public GetResult getWithCost(int index) {  // Get element at index (O(n) traversal).  
            requireIndexInRange(index);  // Validate index.  
            int hops = 0;  // Count `next` traversals.  
            Node current = this.head;  // Start from head.  
            for (int i = 0; i < index; i++) {  // Move forward index times.  
                current = current.next;  // Traverse one link.  
                hops += 1;  // Count one hop.  
            }  // Close loop.  
            return new GetResult(current.value, new OperationCost(hops));  // Return value + cost.  
        }  // End getWithCost.  

        public OperationCost insertAt(int index, int value) {  // Insert at index (O(n) except at head).  
            requireInsertIndexInRange(index);  // Validate index.  
            if (index == 0) {  // Special-case head insertion.  
                return pushFront(value);  // Delegate to pushFront.  
            }  // Close head case.  

            int hops = 0;  // Count traversal hops.  
            Node prev = this.head;  // Start from head (we need predecessor).  
            for (int i = 0; i < index - 1; i++) {  // Move to (index-1)th node.  
                prev = prev.next;  // Traverse one link.  
                hops += 1;  // Count one hop.  
            }  // Close loop.  
            prev.next = new Node(value, prev.next);  // Splice new node after predecessor.  
            this.size += 1;  // Update size.  
            return new OperationCost(hops);  // Return traversal cost.  
        }  // End insertAt.  

        public RemoveResult removeAt(int index) {  // Remove node at index and return removed value + cost.  
            requireIndexInRange(index);  // Validate index.  
            if (index == 0) {  // Special-case head removal.  
                int removedValue = this.head.value;  // Capture head value.  
                this.head = this.head.next;  // Bypass old head.  
                this.size -= 1;  // Update size.  
                return new RemoveResult(removedValue, new OperationCost(0));  // No traversal needed.  
            }  // Close head case.  

            int hops = 0;  // Count traversal hops.  
            Node prev = this.head;  // Start from head (need predecessor).  
            for (int i = 0; i < index - 1; i++) {  // Move to predecessor.  
                prev = prev.next;  // Traverse one link.  
                hops += 1;  // Count one hop.  
            }  // Close loop.  
            int removedValue = prev.next.value;  // Capture removed value.  
            prev.next = prev.next.next;  // Bypass removed node.  
            this.size -= 1;  // Update size.  
            return new RemoveResult(removedValue, new OperationCost(hops));  // Return value + cost.  
        }  // End removeAt.  

        public FindResult indexOfWithCost(int value) {  // Find first index of value (O(n)).  
            Node current = this.head;  // Start from head.  
            int index = 0;  // Track current index.  
            int hops = 0;  // Count `next` traversals.  
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
    }  // End SinglyLinkedList.  

    public static SinglyLinkedList buildOrderedList(int n) {  // Build a list containing [0, 1, ..., n-1].  
        if (n < 0) {  // Reject invalid sizes.  
            throw new IllegalArgumentException("n must be >= 0");  // Signal invalid input.  
        }  // Close validation.  
        SinglyLinkedList lst = new SinglyLinkedList();  // Create empty list.  
        for (int v = n - 1; v >= 0; v--) {  // Push front in reverse to get ascending order.  
            lst.pushFront(v);  // O(1) head insertion.  
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

    private static String formatPushFrontVsBackTable(List<Integer> ns) {  // Format pushFront vs pushBack hops table.  
        String header = String.format("%6s | %6s | %6s", "n", "frontH", "backH");  // Header line.  
        String separator = "-".repeat(header.length());  // Separator line.  
        StringBuilder sb = new StringBuilder();  // Accumulate output.  
        sb.append(header).append('\n');  // Append header.  
        sb.append(separator).append('\n');  // Append separator.  
        for (int n : ns) {  // Render one row per n.  
            SinglyLinkedList a = buildOrderedList(n);  // Build list of size n.  
            OperationCost front = a.pushFront(999);  // Measure head insertion.  
            SinglyLinkedList b = buildOrderedList(n);  // Build another list of size n.  
            OperationCost back = b.pushBack(999);  // Measure tail insertion (no tail pointer).  
            sb.append(String.format("%6d | %6d | %6d", n, front.hops, back.hops)).append('\n');  // Append row.  
        }  // Close loop.  
        return sb.toString();  // Return table string.  
    }  // End formatPushFrontVsBackTable.  

    private static String formatGetHopsTable(int n, int[] indices) {  // Show get(i) hops on an ordered list of size n.  
        SinglyLinkedList lst = buildOrderedList(n);  // Build ordered list.  
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
        System.out.println("=== pushFront vs pushBack hops at size n ===");  // Print section title.  
        System.out.print(formatPushFrontVsBackTable(ns));  // Print comparison table.  
        System.out.println();  // Blank line.  
        System.out.println("=== get(i) hops on list of size 16 ===");  // Print section title.  
        System.out.print(formatGetHopsTable(16, new int[] {0, 1, 2, 4, 8, 15}));  // Print get-cost table.  
    }  // End main.  
}  // End SinglyLinkedListDemo.  

