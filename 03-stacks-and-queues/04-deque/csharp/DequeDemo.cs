// 04 雙端佇列示範（C#）/ Deque demo (C#).  // Bilingual file header.

using System;  // Provide exceptions and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for ToList conversions.  

namespace DequeUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class DequeDemo  // Host Deque implementation and simulation helpers.  
    {  // Open class scope.  
        internal readonly struct OperationCost  // Record per-operation copy/shift counts.  
        {  // Open struct scope.  
            public OperationCost(int copied, int moved)  // Construct an immutable cost record.  
            {  // Open constructor scope.  
                Copied = copied;  // Store copy count.  
                Moved = moved;  // Store move/shift count.  
            }  // Close constructor scope.  

            public int Copied { get; }  // How many elements were copied due to resize (0 if no resize).  
            public int Moved { get; }  // How many elements were shifted due to an operation (should be 0 here).  
        }  // Close struct scope.  

        internal readonly struct PopResult  // Return popped value plus its operation cost.  
        {  // Open struct scope.  
            public PopResult(int value, OperationCost cost)  // Construct immutable pop result.  
            {  // Open constructor scope.  
                Value = value;  // Store popped value.  
                Cost = cost;  // Store cost record.  
            }  // Close constructor scope.  

            public int Value { get; }  // The popped element value.  
            public OperationCost Cost { get; }  // Copy/shift cost for this pop operation.  
        }  // Close struct scope.  

        internal readonly struct PushSummary  // Summarize a sequence of pushBack operations (growth behavior).  
        {  // Open struct scope.  
            public PushSummary(int m, int finalSize, int finalCapacity, long totalCopies, long totalActualCost, int maxCopiedInOneOp)  // Construct summary.  
            {  // Open constructor scope.  
                M = m;  // Store operation count.  
                FinalSize = finalSize;  // Store final size.  
                FinalCapacity = finalCapacity;  // Store final capacity.  
                TotalCopies = totalCopies;  // Store total copies.  
                TotalActualCost = totalActualCost;  // Store total actual cost.  
                MaxCopiedInOneOp = maxCopiedInOneOp;  // Store max per-op copied.  
            }  // Close constructor scope.  

            public int M { get; }  // Number of pushes performed.  
            public int FinalSize { get; }  // Final size after pushes.  
            public int FinalCapacity { get; }  // Final capacity after growth.  
            public long TotalCopies { get; }  // Total copied elements due to resizes.  
            public long TotalActualCost { get; }  // Total cost if each push costs 1 write + copied.  
            public int MaxCopiedInOneOp { get; }  // Maximum copies in any single push.  
        }  // Close struct scope.  

        internal sealed class Deque  // A circular-buffer deque with doubling growth (teaching-oriented).  
        {  // Open class scope.  
            private int _size;  // Number of stored elements.  
            private int _capacity;  // Allocated slots (always >= 1 in this unit).  
            private int _head;  // Head index (front position) within the buffer.  
            private int[] _data;  // Backing buffer.  
            private long _totalCopies;  // Total copied elements due to resizes.  

            public Deque()  // Initialize an empty deque with capacity 1.  
            {  // Open constructor scope.  
                _size = 0;  // Empty deque has size 0.  
                _capacity = 1;  // Start with capacity 1 for deterministic doubling.  
                _head = 0;  // Head starts at index 0.  
                _data = new int[_capacity];  // Allocate backing buffer.  
                _totalCopies = 0;  // Start with zero total copies.  
            }  // Close constructor scope.  

            public int Size => _size;  // Expose current size for callers/tests.  
            public int Capacity => _capacity;  // Expose current capacity for callers/tests.  
            public long TotalCopies => _totalCopies;  // Expose total resize copies for analysis-style checks.  
            public bool IsEmpty => _size == 0;  // Expose emptiness check.  

            private int IndexAt(int offset)  // Map logical offset [0..size) to physical buffer index.  
            {  // Open method scope.  
                return (_head + offset) % _capacity;  // Wrap around using modulo.  
            }  // Close IndexAt.  

            public List<int> ToList()  // Return a copy of the used portion (front -> back).  
            {  // Open method scope.  
                var outList = new List<int>(capacity: _size);  // Allocate list of exact size for a stable snapshot.  
                for (int i = 0; i < _size; i++)  // Copy elements in deque order.  
                {  // Open loop scope.  
                    outList.Add(_data[IndexAt(i)]);  // Append one element in logical order.  
                }  // Close loop scope.  
                return outList;  // Return copied list.  
            }  // Close ToList.  

            private int Resize(int newCapacity)  // Resize buffer and return number of copied elements.  
            {  // Open method scope.  
                if (newCapacity < _size)  // Ensure new buffer can hold all existing elements.  
                {  // Open validation scope.  
                    throw new ArgumentOutOfRangeException(nameof(newCapacity), "newCapacity must be >= size");  // Signal invalid resize request.  
                }  // Close validation scope.  
                if (newCapacity < 1)  // Keep capacity positive for simplicity.  
                {  // Open validation scope.  
                    throw new ArgumentOutOfRangeException(nameof(newCapacity), "newCapacity must be >= 1");  // Signal invalid capacity.  
                }  // Close validation scope.  

                int[] newData = new int[newCapacity];  // Allocate new buffer.  
                int copied = 0;  // Count copied elements deterministically.  
                for (int i = 0; i < _size; i++)  // Copy elements in deque order.  
                {  // Open loop scope.  
                    newData[i] = _data[IndexAt(i)];  // Copy one element into compact layout.  
                    copied += 1;  // Count one copy.  
                }  // Close loop scope.  
                _data = newData;  // Swap buffer.  
                _capacity = newCapacity;  // Update capacity.  
                _head = 0;  // Reset head so deque starts at index 0 after resize.  
                _totalCopies += copied;  // Accumulate total copies.  
                return copied;  // Return copies performed for this resize.  
            }  // Close Resize.  

            private int EnsureCapacityForOneMore()  // Ensure capacity for one more push; return resize copies.  
            {  // Open method scope.  
                if (_size < _capacity)  // Fast path: free space exists.  
                {  // Open fast-path scope.  
                    return 0;  // No resize needed.  
                }  // Close fast-path scope.  
                return Resize(_capacity * 2);  // Double capacity and return copies.  
            }  // Close EnsureCapacityForOneMore.  

            public OperationCost PushBack(int value)  // Push at back (amortized O(1)).  
            {  // Open method scope.  
                int copied = EnsureCapacityForOneMore();  // Resize if needed.  
                int tailIndex = IndexAt(_size);  // Compute tail index where new element is written.  
                _data[tailIndex] = value;  // Store value at tail position.  
                _size += 1;  // Increase size.  
                return new OperationCost(copied: copied, moved: 0);  // moved stays 0 (no shifting).  
            }  // Close PushBack.  

            public OperationCost PushFront(int value)  // Push at front (amortized O(1)).  
            {  // Open method scope.  
                int copied = EnsureCapacityForOneMore();  // Resize if needed.  
                _head = (_head - 1 + _capacity) % _capacity;  // Move head left by one with wrap-around.  
                _data[_head] = value;  // Store value at new head position.  
                _size += 1;  // Increase size.  
                return new OperationCost(copied: copied, moved: 0);  // moved stays 0 (no shifting).  
            }  // Close PushFront.  

            public int PeekFront()  // Return front value without removing it (O(1)).  
            {  // Open method scope.  
                if (_size == 0)  // Reject peeking an empty deque.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("peek from empty deque");  // Signal invalid operation.  
                }  // Close validation scope.  
                return _data[_head];  // Return front slot directly.  
            }  // Close PeekFront.  

            public int PeekBack()  // Return back value without removing it (O(1)).  
            {  // Open method scope.  
                if (_size == 0)  // Reject peeking an empty deque.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("peek from empty deque");  // Signal invalid operation.  
                }  // Close validation scope.  
                int tailIndex = IndexAt(_size - 1);  // Compute index of last element.  
                return _data[tailIndex];  // Return back slot.  
            }  // Close PeekBack.  

            public PopResult PopFront()  // Pop from front (O(1); no shifting).  
            {  // Open method scope.  
                if (_size == 0)  // Reject popping an empty deque.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("pop from empty deque");  // Signal invalid operation.  
                }  // Close validation scope.  

                int removed = _data[_head];  // Capture front value.  
                _head = (_head + 1) % _capacity;  // Advance head by one with wrap-around.  
                _size -= 1;  // Decrease size.  
                if (_size == 0)  // Normalize head for deterministic state when deque becomes empty.  
                {  // Open normalize scope.  
                    _head = 0;  // Reset head (optional but stable).  
                }  // Close normalize scope.  
                return new PopResult(value: removed, cost: new OperationCost(copied: 0, moved: 0));  // moved stays 0 (no shift).  
            }  // Close PopFront.  

            public PopResult PopBack()  // Pop from back (O(1); no shifting).  
            {  // Open method scope.  
                if (_size == 0)  // Reject popping an empty deque.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("pop from empty deque");  // Signal invalid operation.  
                }  // Close validation scope.  

                int tailIndex = IndexAt(_size - 1);  // Compute index of last element.  
                int removed = _data[tailIndex];  // Capture back value.  
                _size -= 1;  // Decrease size.  
                if (_size == 0)  // Normalize head for deterministic state when deque becomes empty.  
                {  // Open normalize scope.  
                    _head = 0;  // Reset head (optional but stable).  
                }  // Close normalize scope.  
                return new PopResult(value: removed, cost: new OperationCost(copied: 0, moved: 0));  // moved stays 0 (no shift).  
            }  // Close PopBack.  
        }  // Close Deque.  

        internal static PushSummary SimulatePushBacks(int m)  // Simulate m pushBack operations and summarize growth/copy costs.  
        {  // Open method scope.  
            if (m < 0)  // Reject invalid counts.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(m), "m must be >= 0");  // Signal invalid input.  
            }  // Close validation scope.  

            var d = new Deque();  // Fresh deque for deterministic results.  
            long totalActualCost = 0;  // Accumulate total cost (1 write + copied).  
            int maxCopied = 0;  // Track maximum copied elements in a single push.  
            for (int i = 0; i < m; i++)  // Perform m pushes.  
            {  // Open loop scope.  
                OperationCost cost = d.PushBack(i);  // Push deterministic value and get per-op cost.  
                totalActualCost += 1 + cost.Copied;  // Add write cost + resize copy cost.  
                maxCopied = Math.Max(maxCopied, cost.Copied);  // Update maximum copied.  
            }  // Close loop scope.  

            return new PushSummary(  // Return summary record.  
                m: m,  // Store operation count.  
                finalSize: d.Size,  // Store final size.  
                finalCapacity: d.Capacity,  // Store final capacity.  
                totalCopies: d.TotalCopies,  // Store total copies.  
                totalActualCost: totalActualCost,  // Store total actual cost.  
                maxCopiedInOneOp: maxCopied  // Store max per-op copied.  
            );  // Close summary construction.  
        }  // Close SimulatePushBacks.  
    }  // Close class scope.  
}  // Close namespace scope.  

