// 03 單向鏈結串列示範（C#）/ Singly linked list demo (C#).  // Bilingual file header.

using System;  // Provide exceptions and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for ToList conversions.  

namespace SinglyLinkedListUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class SinglyLinkedListDemo  // Host SinglyLinkedList implementation and simulation helpers.  
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

        private sealed class Node  // A singly linked list node (value + next pointer).  
        {  // Open class scope.  
            public Node(int value, Node? next)  // Construct node with value and next pointer.  
            {  // Open constructor scope.  
                Value = value;  // Store node value.  
                Next = next;  // Store next pointer.  
            }  // Close constructor scope.  

            public int Value { get; }  // Node value.  
            public Node? Next { get; set; }  // Next pointer (nullable).  
        }  // Close Node.  

        internal sealed class SinglyLinkedList  // A head-only singly linked list (no tail pointer).  
        {  // Open class scope.  
            private Node? _head;  // Head pointer (null means empty).  
            private int _size;  // Track size in O(1).  

            public SinglyLinkedList()  // Initialize an empty list.  
            {  // Open constructor scope.  
                _head = null;  // Empty list has null head.  
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
                    current = current.Next;  // Move to next node.  
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

            public OperationCost PushFront(int value)  // Insert at head (O(1), hops=0).  
            {  // Open method scope.  
                _head = new Node(value, _head);  // Link new node before current head.  
                _size += 1;  // Update size.  
                return new OperationCost(hops: 0);  // Head insertion does not traverse `next`.  
            }  // Close PushFront.  

            public OperationCost PushBack(int value)  // Insert at tail (O(n) without a tail pointer).  
            {  // Open method scope.  
                if (_head == null)  // Special-case empty list.  
                {  // Open empty-case scope.  
                    _head = new Node(value, next: null);  // New node becomes head.  
                    _size = 1;  // Size becomes 1.  
                    return new OperationCost(hops: 0);  // No traversal needed.  
                }  // Close empty-case scope.  

                int hops = 0;  // Count `next` traversals.  
                Node current = _head;  // Start from head.  
                while (current.Next != null)  // Walk until last node.  
                {  // Open loop scope.  
                    current = current.Next;  // Traverse one link.  
                    hops += 1;  // Count one hop.  
                }  // Close loop scope.  
                current.Next = new Node(value, next: null);  // Link new node after last.  
                _size += 1;  // Update size.  
                return new OperationCost(hops: hops);  // Return traversal cost.  
            }  // Close PushBack.  

            public GetResult GetWithCost(int index)  // Get element at index (O(n) traversal).  
            {  // Open method scope.  
                RequireIndexInRange(index);  // Validate index.  
                int hops = 0;  // Count `next` traversals.  
                Node current = _head!;  // Start from head (non-null after validation).  
                for (int i = 0; i < index; i++)  // Move forward index times.  
                {  // Open loop scope.  
                    current = current.Next!;  // Traverse one link (non-null before reaching index).  
                    hops += 1;  // Count one hop.  
                }  // Close loop scope.  
                return new GetResult(value: current.Value, cost: new OperationCost(hops: hops));  // Return value + cost.  
            }  // Close GetWithCost.  

            public OperationCost InsertAt(int index, int value)  // Insert at index (O(n) except at head).  
            {  // Open method scope.  
                RequireInsertIndexInRange(index);  // Validate index.  
                if (index == 0)  // Special-case head insertion.  
                {  // Open head-case scope.  
                    return PushFront(value);  // Delegate to PushFront.  
                }  // Close head-case scope.  

                int hops = 0;  // Count traversal hops.  
                Node prev = _head!;  // Start from head (non-null because index>0).  
                for (int i = 0; i < index - 1; i++)  // Move to predecessor.  
                {  // Open loop scope.  
                    prev = prev.Next!;  // Traverse one link.  
                    hops += 1;  // Count one hop.  
                }  // Close loop scope.  
                prev.Next = new Node(value, next: prev.Next);  // Splice new node after predecessor.  
                _size += 1;  // Update size.  
                return new OperationCost(hops: hops);  // Return traversal cost.  
            }  // Close InsertAt.  

            public RemoveResult RemoveAt(int index)  // Remove at index and return removed value + cost.  
            {  // Open method scope.  
                RequireIndexInRange(index);  // Validate index.  
                if (index == 0)  // Special-case head removal.  
                {  // Open head-case scope.  
                    Node removed = _head!;  // Capture head node.  
                    _head = removed.Next;  // Bypass old head.  
                    _size -= 1;  // Update size.  
                    return new RemoveResult(value: removed.Value, cost: new OperationCost(hops: 0));  // No traversal needed.  
                }  // Close head-case scope.  

                int hops = 0;  // Count traversal hops.  
                Node prev = _head!;  // Start from head (need predecessor).  
                for (int i = 0; i < index - 1; i++)  // Move to predecessor.  
                {  // Open loop scope.  
                    prev = prev.Next!;  // Traverse one link.  
                    hops += 1;  // Count one hop.  
                }  // Close loop scope.  
                Node removedNode = prev.Next!;  // Node to remove (non-null for valid index).  
                prev.Next = removedNode.Next;  // Bypass removed node.  
                _size -= 1;  // Update size.  
                return new RemoveResult(value: removedNode.Value, cost: new OperationCost(hops: hops));  // Return value + cost.  
            }  // Close RemoveAt.  

            public FindResult IndexOfWithCost(int value)  // Find first index of value (O(n)).  
            {  // Open method scope.  
                Node? current = _head;  // Start from head.  
                int index = 0;  // Track current index.  
                int hops = 0;  // Count `next` traversals.  
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
        }  // Close SinglyLinkedList.  

        internal static SinglyLinkedList BuildOrderedList(int n)  // Build a list containing [0, 1, ..., n-1].  
        {  // Open method scope.  
            if (n < 0)  // Reject invalid sizes.  
            {  // Open validation scope.  
                throw new ArgumentException("n must be >= 0");  // Signal invalid input.  
            }  // Close validation scope.  
            var lst = new SinglyLinkedList();  // Create empty list.  
            for (int v = n - 1; v >= 0; v--)  // Push front in reverse to get ascending order.  
            {  // Open loop scope.  
                lst.PushFront(v);  // O(1) head insertion.  
            }  // Close loop scope.  
            return lst;  // Return built list.  
        }  // Close BuildOrderedList.  
    }  // Close SinglyLinkedListDemo.  
}  // Close namespace scope.  

