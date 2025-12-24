// 05 環狀鏈結串列示範（C#）/ Circular linked list demo (C#).  // Bilingual file header.

using System;  // Provide exceptions and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for ToList conversions.  

namespace CircularLinkedListUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class CircularLinkedListDemo  // Host CircularLinkedList implementation and simulation helpers.  
    {  // Open class scope.  
        internal readonly struct OperationCost  // Record the traversal cost of one operation.  
        {  // Open struct scope.  
            public OperationCost(int hops)  // Construct an immutable cost record.  
            {  // Open constructor scope.  
                Hops = hops;  // Store hop count.  
            }  // Close constructor scope.  

            public int Hops { get; }  // Number of `next` pointer traversals performed (current = current.Next).  
        }  // Close struct scope.  

        internal readonly struct GetResult  // Return fetched value plus traversal cost.  
        {  // Open struct scope.  
            public GetResult(int value, OperationCost cost)  // Construct immutable get result.  
            {  // Open constructor scope.  
                Value = value;  // Store fetched value.  
                Cost = cost;  // Store cost record.  
            }  // Close constructor scope.  

            public int Value { get; }  // The fetched element value.  
            public OperationCost Cost { get; }  // Traversal cost record.  
        }  // Close struct scope.  

        internal readonly struct RemoveResult  // Return removed value plus traversal cost.  
        {  // Open struct scope.  
            public RemoveResult(int value, OperationCost cost)  // Construct immutable remove result.  
            {  // Open constructor scope.  
                Value = value;  // Store removed value.  
                Cost = cost;  // Store cost record.  
            }  // Close constructor scope.  

            public int Value { get; }  // The removed element value.  
            public OperationCost Cost { get; }  // Traversal cost record.  
        }  // Close struct scope.  

        internal readonly struct FindResult  // Return found index plus traversal cost.  
        {  // Open struct scope.  
            public FindResult(int index, OperationCost cost)  // Construct immutable find result.  
            {  // Open constructor scope.  
                Index = index;  // Store index.  
                Cost = cost;  // Store cost record.  
            }  // Close constructor scope.  

            public int Index { get; }  // Found index, or -1 if not found.  
            public OperationCost Cost { get; }  // Traversal cost record.  
        }  // Close struct scope.  

        private sealed class Node  // A singly circular linked list node (value + next pointer).  
        {  // Open class scope.  
            public Node(int value, Node? next)  // Construct node with value and next pointer.  
            {  // Open constructor scope.  
                Value = value;  // Store node value.  
                Next = next;  // Store next pointer (non-null in non-empty circular list).  
            }  // Close constructor scope.  

            public int Value { get; }  // Node value.  
            public Node? Next { get; set; }  // Next pointer (nullable only during construction).  
        }  // Close Node.  

        internal sealed class CircularLinkedList  // A singly circular linked list implemented with a tail pointer only.  
        {  // Open class scope.  
            private Node? _tail;  // Tail pointer (null means empty).  
            private int _size;  // Track size so we can traverse safely without null terminators.  

            public CircularLinkedList()  // Initialize an empty list.  
            {  // Open constructor scope.  
                _tail = null;  // Empty list has null tail.  
                _size = 0;  // Empty list has size 0.  
            }  // Close constructor scope.  

            public int Size => _size;  // Expose size for callers/tests.  
            public bool IsEmpty => _size == 0;  // Expose emptiness check.  

            private Node? HeadOrNull()  // Return head node (tail.Next) or null for empty list.  
            {  // Open method scope.  
                if (_tail == null)  // Empty list has no head.  
                {  // Open empty-case scope.  
                    return null;  // Signal no head.  
                }  // Close empty-case scope.  
                return _tail.Next;  // Head is tail.Next in a circular list.  
            }  // Close HeadOrNull.  

            public List<int> ToList()  // Convert list contents to a List<int> (bounded by size to avoid infinite loops).  
            {  // Open method scope.  
                var outList = new List<int>(_size);  // Pre-size list to reduce reallocation.  
                Node? head = HeadOrNull();  // Read head node.  
                if (head == null)  // Empty list converts to empty list.  
                {  // Open empty-case scope.  
                    return outList;  // Return empty list.  
                }  // Close empty-case scope.  
                Node current = head;  // Start at head.  
                for (int i = 0; i < _size; i++)  // Traverse exactly size nodes.  
                {  // Open loop scope.  
                    outList.Add(current.Value);  // Append one value.  
                    current = current.Next!;  // Move to next node (non-null in circular list).  
                }  // Close loop scope.  
                return outList;  // Return collected values.  
            }  // Close ToList.  

            private void RequireIndexInRange(int index)  // Validate index in [0, size-1].  
            {  // Open method scope.  
                if (index < 0 || index >= _size)  // Reject invalid indices.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("index out of range");  // Signal invalid index.  
                }  // Close validation scope.  
            }  // Close RequireIndexInRange.  

            public OperationCost PushBack(int value)  // Insert at tail (O(1), hops=0).  
            {  // Open method scope.  
                if (_tail == null)  // Empty list special-case.  
                {  // Open empty-case scope.  
                    var node = new Node(value, next: null);  // Create node with temporary next pointer.  
                    node.Next = node;  // Point to itself to form a 1-node circle.  
                    _tail = node;  // Tail becomes node.  
                    _size = 1;  // Size becomes 1.  
                    return new OperationCost(hops: 0);  // No traversal.  
                }  // Close empty-case scope.  

                Node head = _tail.Next!;  // Read current head (non-null in non-empty list).  
                var newNode = new Node(value, next: head);  // New node points to current head.  
                _tail.Next = newNode;  // Old tail points to new node.  
                _tail = newNode;  // New node becomes tail.  
                _size += 1;  // Update size.  
                return new OperationCost(hops: 0);  // No traversal.  
            }  // Close PushBack.  

            public OperationCost PushFront(int value)  // Insert at head (O(1), hops=0) by linking after tail.  
            {  // Open method scope.  
                if (_tail == null)  // Empty list special-case.  
                {  // Open empty-case scope.  
                    return PushBack(value);  // Inserting into empty list is the same as PushBack.  
                }  // Close empty-case scope.  

                Node head = _tail.Next!;  // Read current head.  
                var newNode = new Node(value, next: head);  // New node points to old head.  
                _tail.Next = newNode;  // Tail now points to new head.  
                _size += 1;  // Update size.  
                return new OperationCost(hops: 0);  // No traversal.  
            }  // Close PushFront.  

            public RemoveResult PopFront()  // Remove head (O(1), hops=0).  
            {  // Open method scope.  
                if (_tail == null)  // Reject popping from empty list.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("pop from empty list");  // Signal invalid operation.  
                }  // Close validation scope.  

                Node head = _tail.Next!;  // Read current head (non-null).  
                int removedValue = head.Value;  // Capture removed value.  
                if (_size == 1)  // Removing the only node empties the list.  
                {  // Open single-node scope.  
                    _tail = null;  // Clear tail.  
                    _size = 0;  // Reset size.  
                    return new RemoveResult(value: removedValue, cost: new OperationCost(hops: 0));  // No traversal.  
                }  // Close single-node scope.  

                Node newHead = head.Next!;  // Next node becomes the new head.  
                _tail.Next = newHead;  // Tail links to new head.  
                _size -= 1;  // Update size.  
                return new RemoveResult(value: removedValue, cost: new OperationCost(hops: 0));  // No traversal.  
            }  // Close PopFront.  

            public RemoveResult PopBack()  // Remove tail (O(n), hops=max(0, n-2)) in singly circular list.  
            {  // Open method scope.  
                if (_tail == null)  // Reject popping from empty list.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("pop from empty list");  // Signal invalid operation.  
                }  // Close validation scope.  

                int removedValue = _tail.Value;  // Capture removed value.  
                if (_size == 1)  // Removing the only node empties the list.  
                {  // Open single-node scope.  
                    _tail = null;  // Clear tail.  
                    _size = 0;  // Reset size.  
                    return new RemoveResult(value: removedValue, cost: new OperationCost(hops: 0));  // No traversal.  
                }  // Close single-node scope.  

                Node head = _tail.Next!;  // Read head node.  
                Node prev = head;  // Start at head to find the predecessor of tail.  
                int hops = 0;  // Count next traversals.  
                int steps = _size - 2;  // Number of `next` moves needed from head to reach tail's predecessor.  
                for (int i = 0; i < steps; i++)  // Walk steps times.  
                {  // Open loop scope.  
                    prev = prev.Next!;  // Traverse one next pointer.  
                    hops += 1;  // Count one hop.  
                }  // Close loop scope.  

                prev.Next = _tail.Next;  // Link predecessor to head (skipping old tail).  
                _tail = prev;  // Update tail pointer to predecessor.  
                _size -= 1;  // Update size.  
                return new RemoveResult(value: removedValue, cost: new OperationCost(hops: hops));  // Return value + traversal cost.  
            }  // Close PopBack.  

            public OperationCost Rotate(int steps)  // Rotate left by moving tail forward `steps` times (hops=steps%size).  
            {  // Open method scope.  
                if (steps < 0)  // Reject negative rotations for deterministic teaching semantics.  
                {  // Open validation scope.  
                    throw new ArgumentOutOfRangeException(nameof(steps), "steps must be >= 0");  // Signal invalid input.  
                }  // Close validation scope.  
                if (_tail == null || _size <= 1)  // Rotating empty/1-node list changes nothing.  
                {  // Open trivial-case scope.  
                    return new OperationCost(hops: 0);  // No traversal.  
                }  // Close trivial-case scope.  
                int stepsMod = steps % _size;  // Reduce steps to avoid unnecessary full cycles.  
                int hops = 0;  // Count traversals.  
                for (int i = 0; i < stepsMod; i++)  // Advance tail stepsMod times.  
                {  // Open loop scope.  
                    _tail = _tail!.Next!;  // Move tail forward one node (non-null in a non-empty circular list).  
                    hops += 1;  // Count one hop.  
                }  // Close loop scope.  
                return new OperationCost(hops: hops);  // Return traversal cost.  
            }  // Close Rotate.  

            public GetResult GetWithCost(int index)  // Get element at index (O(index), hops=index).  
            {  // Open method scope.  
                RequireIndexInRange(index);  // Validate index.  
                Node head = _tail!.Next!;  // Read head (non-null because size>0).  
                Node current = head;  // Start at head.  
                int hops = 0;  // Count traversals.  
                for (int i = 0; i < index; i++)  // Move forward index times.  
                {  // Open loop scope.  
                    current = current.Next!;  // Traverse one next pointer.  
                    hops += 1;  // Count one hop.  
                }  // Close loop scope.  
                return new GetResult(value: current.Value, cost: new OperationCost(hops: hops));  // Return value + cost.  
            }  // Close GetWithCost.  

            public FindResult IndexOfWithCost(int value)  // Find first index of value and return traversal cost.  
            {  // Open method scope.  
                if (_tail == null)  // Empty list cannot contain the value.  
                {  // Open empty-case scope.  
                    return new FindResult(index: -1, cost: new OperationCost(hops: 0));  // Not found with zero cost.  
                }  // Close empty-case scope.  
                Node current = _tail.Next!;  // Start at head.  
                int hops = 0;  // Count traversals.  
                for (int index = 0; index < _size; index++)  // Traverse exactly size nodes to avoid infinite loops.  
                {  // Open loop scope.  
                    if (current.Value == value)  // Check for match.  
                    {  // Open match scope.  
                        return new FindResult(index: index, cost: new OperationCost(hops: hops));  // Return index + cost.  
                    }  // Close match scope.  
                    current = current.Next!;  // Move to next node.  
                    hops += 1;  // Count one hop.  
                }  // Close loop scope.  
                return new FindResult(index: -1, cost: new OperationCost(hops: hops));  // Not found: hops equals size.  
            }  // Close IndexOfWithCost.  
        }  // Close CircularLinkedList.  

        public static CircularLinkedList BuildOrderedList(int n)  // Build a list containing [0, 1, ..., n-1].  
        {  // Open method scope.  
            if (n < 0)  // Reject invalid sizes.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(n), "n must be >= 0");  // Signal invalid input.  
            }  // Close validation scope.  
            var lst = new CircularLinkedList();  // Create empty list.  
            for (int v = 0; v < n; v++)  // Append in increasing order.  
            {  // Open loop scope.  
                lst.PushBack(v);  // pushBack is O(1) with tail pointer.  
            }  // Close loop scope.  
            return lst;  // Return built list.  
        }  // Close BuildOrderedList.  
    }  // Close class scope.  
}  // Close namespace scope.  
