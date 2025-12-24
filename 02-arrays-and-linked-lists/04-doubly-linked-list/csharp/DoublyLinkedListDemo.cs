// 04 雙向鏈結串列示範（C#）/ Doubly linked list demo (C#).  // Bilingual file header.

using System;  // Provide exceptions and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for ToList conversions.  

namespace DoublyLinkedListUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class DoublyLinkedListDemo  // Host DoublyLinkedList implementation and simulation helpers.  
    {  // Open class scope.  
        internal readonly struct OperationCost  // Record the traversal cost of one operation.  
        {  // Open struct scope.  
            public OperationCost(int hops)  // Construct an immutable cost record.  
            {  // Open constructor scope.  
                Hops = hops;  // Store hop count.  
            }  // Close constructor scope.  

            public int Hops { get; }  // Number of pointer traversals performed (next or prev).  
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

        private sealed class Node  // A doubly linked list node (value + prev/next pointers).  
        {  // Open class scope.  
            public Node(int value, Node? prev, Node? next)  // Construct node with explicit neighbors.  
            {  // Open constructor scope.  
                Value = value;  // Store node value.  
                Prev = prev;  // Store previous pointer.  
                Next = next;  // Store next pointer.  
            }  // Close constructor scope.  

            public int Value { get; }  // Node value.  
            public Node? Prev { get; set; }  // Previous pointer (nullable).  
            public Node? Next { get; set; }  // Next pointer (nullable).  
        }  // Close Node.  

        internal sealed class DoublyLinkedList  // A head+tail doubly linked list (teaching-oriented).  
        {  // Open class scope.  
            private Node? _head;  // Head pointer (null means empty).  
            private Node? _tail;  // Tail pointer (null means empty).  
            private int _size;  // Track size in O(1).  

            public DoublyLinkedList()  // Initialize an empty list.  
            {  // Open constructor scope.  
                _head = null;  // Empty list has null head.  
                _tail = null;  // Empty list has null tail.  
                _size = 0;  // Empty list has size 0.  
            }  // Close constructor scope.  

            public int Size => _size;  // Expose size for callers/tests.  
            public bool IsEmpty => _size == 0;  // Expose emptiness check.  

            public List<int> ToList()  // Convert list contents to a List<int> for easy assertions/printing.  
            {  // Open method scope.  
                var outList = new List<int>(_size);  // Pre-size list to reduce reallocation.  
                Node? current = _head;  // Start at head.  
                while (current != null)  // Traverse until end.  
                {  // Open loop scope.  
                    outList.Add(current.Value);  // Append one value.  
                    current = current.Next;  // Move forward via next.  
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

            private void RequireInsertIndexInRange(int index)  // Validate insert index in [0, size].  
            {  // Open method scope.  
                if (index < 0 || index > _size)  // Reject invalid insertion indices.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("index out of range for insert");  // Signal invalid index.  
                }  // Close validation scope.  
            }  // Close RequireInsertIndexInRange.  

            private (Node Node, OperationCost Cost) NodeAtWithCost(int index)  // Locate node at index and report traversal hops.  
            {  // Open method scope.  
                RequireIndexInRange(index);  // Validate index.  
                if (index < _size / 2)  // Traverse from head when index is in the first half.  
                {  // Open head-branch scope.  
                    int hops = 0;  // Count pointer traversals.  
                    Node current = _head!;  // Start from head (non-null after validation).  
                    for (int i = 0; i < index; i++)  // Move forward index times.  
                    {  // Open loop scope.  
                        current = current.Next!;  // Traverse one next pointer.  
                        hops += 1;  // Count one hop.  
                    }  // Close loop scope.  
                    return (current, new OperationCost(hops: hops));  // Return node + cost.  
                }  // Close head-branch scope.  

                int steps = _size - 1 - index;  // How many prev steps needed from tail.  
                int hopsTail = 0;  // Count pointer traversals.  
                Node currentTail = _tail!;  // Start from tail (non-null after validation).  
                for (int i = 0; i < steps; i++)  // Move backward `steps` times.  
                {  // Open loop scope.  
                    currentTail = currentTail.Prev!;  // Traverse one prev pointer.  
                    hopsTail += 1;  // Count one hop.  
                }  // Close loop scope.  
                return (currentTail, new OperationCost(hops: hopsTail));  // Return node + cost.  
            }  // Close NodeAtWithCost.  

            public OperationCost PushFront(int value)  // Insert at head (O(1), hops=0).  
            {  // Open method scope.  
                var newHead = new Node(value, prev: null, next: _head);  // New node points forward to old head.  
                if (_head == null)  // Empty list case.  
                {  // Open empty-case scope.  
                    _head = newHead;  // Head becomes new node.  
                    _tail = newHead;  // Tail becomes new node.  
                    _size = 1;  // Size becomes 1.  
                    return new OperationCost(hops: 0);  // No traversal.  
                }  // Close empty-case scope.  
                _head.Prev = newHead;  // Old head points back to new head.  
                _head = newHead;  // Update head pointer.  
                _size += 1;  // Update size.  
                return new OperationCost(hops: 0);  // No traversal.  
            }  // Close PushFront.  

            public OperationCost PushBack(int value)  // Insert at tail (O(1) with tail pointer).  
            {  // Open method scope.  
                var newTail = new Node(value, prev: _tail, next: null);  // New node points back to old tail.  
                if (_tail == null)  // Empty list case.  
                {  // Open empty-case scope.  
                    _head = newTail;  // Head becomes new node.  
                    _tail = newTail;  // Tail becomes new node.  
                    _size = 1;  // Size becomes 1.  
                    return new OperationCost(hops: 0);  // No traversal.  
                }  // Close empty-case scope.  
                _tail.Next = newTail;  // Old tail points forward to new tail.  
                _tail = newTail;  // Update tail pointer.  
                _size += 1;  // Update size.  
                return new OperationCost(hops: 0);  // No traversal.  
            }  // Close PushBack.  

            public RemoveResult PopFront()  // Remove from head (O(1), hops=0).  
            {  // Open method scope.  
                if (_head == null)  // Reject popping from an empty list.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("pop from empty list");  // Signal invalid operation.  
                }  // Close validation scope.  
                int removedValue = _head.Value;  // Capture removed value.  
                Node? newHead = _head.Next;  // Candidate new head.  
                if (newHead == null)  // Removing the only node empties the list.  
                {  // Open single-node scope.  
                    _head = null;  // Clear head.  
                    _tail = null;  // Clear tail.  
                    _size = 0;  // Reset size.  
                    return new RemoveResult(value: removedValue, cost: new OperationCost(hops: 0));  // No traversal.  
                }  // Close single-node scope.  
                newHead.Prev = null;  // New head has no previous node.  
                _head = newHead;  // Update head.  
                _size -= 1;  // Update size.  
                return new RemoveResult(value: removedValue, cost: new OperationCost(hops: 0));  // No traversal.  
            }  // Close PopFront.  

            public RemoveResult PopBack()  // Remove from tail (O(1), hops=0).  
            {  // Open method scope.  
                if (_tail == null)  // Reject popping from an empty list.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("pop from empty list");  // Signal invalid operation.  
                }  // Close validation scope.  
                int removedValue = _tail.Value;  // Capture removed value.  
                Node? newTail = _tail.Prev;  // Candidate new tail.  
                if (newTail == null)  // Removing the only node empties the list.  
                {  // Open single-node scope.  
                    _head = null;  // Clear head.  
                    _tail = null;  // Clear tail.  
                    _size = 0;  // Reset size.  
                    return new RemoveResult(value: removedValue, cost: new OperationCost(hops: 0));  // No traversal.  
                }  // Close single-node scope.  
                newTail.Next = null;  // New tail has no next node.  
                _tail = newTail;  // Update tail.  
                _size -= 1;  // Update size.  
                return new RemoveResult(value: removedValue, cost: new OperationCost(hops: 0));  // No traversal.  
            }  // Close PopBack.  

            public GetResult GetWithCost(int index)  // Get element at index with traversal cost.  
            {  // Open method scope.  
                (Node node, OperationCost cost) = NodeAtWithCost(index);  // Locate node + cost.  
                return new GetResult(value: node.Value, cost: cost);  // Return value + cost.  
            }  // Close GetWithCost.  

            public OperationCost InsertAt(int index, int value)  // Insert at index and return traversal cost.  
            {  // Open method scope.  
                RequireInsertIndexInRange(index);  // Validate insertion index.  
                if (index == 0)  // Head insertion is O(1).  
                {  // Open head-case scope.  
                    return PushFront(value);  // Delegate to PushFront.  
                }  // Close head-case scope.  
                if (index == _size)  // Tail insertion is O(1).  
                {  // Open tail-case scope.  
                    return PushBack(value);  // Delegate to PushBack.  
                }  // Close tail-case scope.  

                (Node target, OperationCost cost) = NodeAtWithCost(index);  // Locate target node currently at index.  
                Node before = target.Prev!;  // Predecessor node.  
                var newNode = new Node(value, prev: before, next: target);  // New node sits before target.  
                before.Next = newNode;  // Predecessor points forward to new node.  
                target.Prev = newNode;  // Target points back to new node.  
                _size += 1;  // Update size.  
                return cost;  // Traversal cost is the cost to find the insertion position.  
            }  // Close InsertAt.  

            public RemoveResult RemoveAt(int index)  // Remove at index and return removed value + traversal cost.  
            {  // Open method scope.  
                RequireIndexInRange(index);  // Validate index.  
                if (index == 0)  // Head removal is O(1).  
                {  // Open head-case scope.  
                    return PopFront();  // Delegate to PopFront.  
                }  // Close head-case scope.  
                if (index == _size - 1)  // Tail removal is O(1).  
                {  // Open tail-case scope.  
                    return PopBack();  // Delegate to PopBack.  
                }  // Close tail-case scope.  

                (Node target, OperationCost cost) = NodeAtWithCost(index);  // Locate node to remove.  
                Node before = target.Prev!;  // Predecessor.  
                Node after = target.Next!;  // Successor.  
                before.Next = after;  // Bypass target from the left.  
                after.Prev = before;  // Bypass target from the right.  
                _size -= 1;  // Update size.  
                return new RemoveResult(value: target.Value, cost: cost);  // Return removed value + traversal cost.  
            }  // Close RemoveAt.  

            public FindResult IndexOfWithCost(int value)  // Find first index of value and return traversal cost.  
            {  // Open method scope.  
                Node? current = _head;  // Start from head.  
                int index = 0;  // Track index.  
                int hops = 0;  // Count next traversals.  
                while (current != null)  // Traverse until end.  
                {  // Open loop scope.  
                    if (current.Value == value)  // Check for match.  
                    {  // Open match scope.  
                        return new FindResult(index: index, cost: new OperationCost(hops: hops));  // Return index + cost.  
                    }  // Close match scope.  
                    current = current.Next;  // Move to next node.  
                    hops += 1;  // Count one hop.  
                    index += 1;  // Advance index.  
                }  // Close traversal loop.  
                return new FindResult(index: -1, cost: new OperationCost(hops: hops));  // Not found sentinel with cost.  
            }  // Close IndexOfWithCost.  
        }  // Close DoublyLinkedList.  

        internal static DoublyLinkedList BuildOrderedList(int n)  // Build a list containing [0, 1, ..., n-1].  
        {  // Open method scope.  
            if (n < 0)  // Reject invalid sizes.  
            {  // Open validation scope.  
                throw new ArgumentException("n must be >= 0");  // Signal invalid input.  
            }  // Close validation scope.  
            var lst = new DoublyLinkedList();  // Create empty list.  
            for (int v = 0; v < n; v++)  // Append in increasing order.  
            {  // Open loop scope.  
                lst.PushBack(v);  // pushBack is O(1) with tail pointer.  
            }  // Close loop scope.  
            return lst;  // Return built list.  
        }  // Close BuildOrderedList.  

        internal static int ExpectedGetHops(int size, int index)  // Helper used by tests to compute expected hops.  
        {  // Open method scope.  
            return Math.Min(index, size - 1 - index);  // Nearest-end traversal cost formula.  
        }  // Close ExpectedGetHops.  
    }  // Close DoublyLinkedListDemo.  
}  // Close namespace scope.  

