// 02 佇列示範（C#）/ Queue demo (C#).  // Bilingual file header.

using System;  // Provide exceptions and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for ToList conversions.  

namespace QueueUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class QueueDemo  // Host ArrayQueue implementation and simulation helpers.  
    {  // Open class scope.  
        internal readonly struct OperationCost  // Record per-operation copy/shift counts.  
        {  // Open struct scope.  
            public OperationCost(int copied, int moved)  // Construct an immutable cost record.  
            {  // Open constructor scope.  
                Copied = copied;  // Store copy count.  
                Moved = moved;  // Store move/shift count.  
            }  // Close constructor scope.  

            public int Copied { get; }  // How many elements were copied due to resize (0 if no resize).  
            public int Moved { get; }  // How many elements were shifted left due to dequeue (0 for enqueue).  
        }  // Close struct scope.  

        internal readonly struct DequeueResult  // Return dequeued value plus its operation cost.  
        {  // Open struct scope.  
            public DequeueResult(int value, OperationCost cost)  // Construct immutable dequeue result.  
            {  // Open constructor scope.  
                Value = value;  // Store dequeued value.  
                Cost = cost;  // Store cost record.  
            }  // Close constructor scope.  

            public int Value { get; }  // The dequeued element value.  
            public OperationCost Cost { get; }  // Copy/shift cost for this dequeue operation.  
        }  // Close struct scope.  

        internal readonly struct EnqueueSummary  // Summarize a sequence of enqueues (growth behavior).  
        {  // Open struct scope.  
            public EnqueueSummary(int m, int finalSize, int finalCapacity, long totalCopies, long totalActualCost, int maxCopiedInOneOp)  // Construct summary.  
            {  // Open constructor scope.  
                M = m;  // Store operation count.  
                FinalSize = finalSize;  // Store final size.  
                FinalCapacity = finalCapacity;  // Store final capacity.  
                TotalCopies = totalCopies;  // Store total copies.  
                TotalActualCost = totalActualCost;  // Store total actual cost.  
                MaxCopiedInOneOp = maxCopiedInOneOp;  // Store max per-op copied.  
            }  // Close constructor scope.  

            public int M { get; }  // Number of enqueues performed.  
            public int FinalSize { get; }  // Final size after enqueues.  
            public int FinalCapacity { get; }  // Final capacity after growth.  
            public long TotalCopies { get; }  // Total copied elements due to resizes.  
            public long TotalActualCost { get; }  // Total cost if each enqueue costs 1 write + copied.  
            public int MaxCopiedInOneOp { get; }  // Maximum copies in any single enqueue.  
        }  // Close struct scope.  

        internal sealed class ArrayQueue  // A naive array-backed queue (dequeue shifts left; teaching-oriented).  
        {  // Open class scope.  
            private int _size;  // Number of stored elements.  
            private int _capacity;  // Allocated slots (always >= 1 in this unit).  
            private int[] _data;  // Backing buffer.  
            private long _totalCopies;  // Total copied elements due to resizes.  

            public ArrayQueue()  // Initialize an empty queue with capacity 1.  
            {  // Open constructor scope.  
                _size = 0;  // Empty queue has size 0.  
                _capacity = 1;  // Start with capacity 1 for deterministic doubling.  
                _data = new int[_capacity];  // Allocate backing buffer.  
                _totalCopies = 0;  // Start with zero total copies.  
            }  // Close constructor scope.  

            public int Size => _size;  // Expose current size for callers/tests.  
            public int Capacity => _capacity;  // Expose current capacity for callers/tests.  
            public long TotalCopies => _totalCopies;  // Expose total resize copies for analysis-style checks.  
            public bool IsEmpty => _size == 0;  // Expose emptiness check.  

            public List<int> ToList()  // Return a copy of the used portion (front -> rear).  
            {  // Open method scope.  
                var outList = new List<int>(_size);  // Pre-size list to reduce reallocations.  
                for (int i = 0; i < _size; i++)  // Copy used portion.  
                {  // Open loop scope.  
                    outList.Add(_data[i]);  // Append one element.  
                }  // Close loop scope.  
                return outList;  // Return copy.  
            }  // Close ToList.  

            private int Resize(int newCapacity)  // Resize buffer and return number of copied elements.  
            {  // Open method scope.  
                if (newCapacity < _size)  // Ensure new buffer can hold all existing elements.  
                {  // Open validation scope.  
                    throw new ArgumentOutOfRangeException(nameof(newCapacity), "newCapacity must be >= size");  // Signal invalid request.  
                }  // Close validation scope.  
                if (newCapacity < 1)  // Keep capacity positive for simplicity.  
                {  // Open validation scope.  
                    throw new ArgumentOutOfRangeException(nameof(newCapacity), "newCapacity must be >= 1");  // Signal invalid request.  
                }  // Close validation scope.  

                var newData = new int[newCapacity];  // Allocate new buffer.  
                int copied = 0;  // Count copied elements deterministically.  
                for (int i = 0; i < _size; i++)  // Copy exactly the used portion.  
                {  // Open copy loop scope.  
                    newData[i] = _data[i];  // Copy one element.  
                    copied += 1;  // Count one copy.  
                }  // Close copy loop scope.  
                _data = newData;  // Swap buffer reference.  
                _capacity = newCapacity;  // Update capacity.  
                _totalCopies += copied;  // Accumulate total copies.  
                return copied;  // Return copies performed for this resize.  
            }  // Close Resize.  

            private int EnsureCapacityForOneMore()  // Ensure capacity for one more enqueue; return resize copies.  
            {  // Open method scope.  
                if (_size < _capacity)  // Fast path: free space exists.  
                {  // Open fast path scope.  
                    return 0;  // No resize needed.  
                }  // Close fast path scope.  
                return Resize(_capacity * 2);  // Double capacity and return copies.  
            }  // Close EnsureCapacityForOneMore.  

            public OperationCost Enqueue(int value)  // Add to rear (amortized O(1)).  
            {  // Open method scope.  
                int copied = EnsureCapacityForOneMore();  // Resize if needed.  
                _data[_size] = value;  // Write new value at rear slot.  
                _size += 1;  // Increase size.  
                return new OperationCost(copied: copied, moved: 0);  // Enqueue does not shift left.  
            }  // Close Enqueue.  

            public int Peek()  // Return front value without removing it (O(1)).  
            {  // Open method scope.  
                if (_size == 0)  // Reject peeking an empty queue.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("peek from empty queue");  // Signal invalid operation.  
                }  // Close validation scope.  
                return _data[0];  // Return front slot.  
            }  // Close Peek.  

            public DequeueResult Dequeue()  // Remove and return front value (O(n) due to shifting).  
            {  // Open method scope.  
                if (_size == 0)  // Reject dequeuing an empty queue.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("dequeue from empty queue");  // Signal invalid operation.  
                }  // Close validation scope.  

                int removed = _data[0];  // Capture front value.  
                int moved = _size - 1;  // Shifting left moves (size - 1) elements.  
                for (int i = 1; i < _size; i++)  // Shift elements left by one.  
                {  // Open shift loop scope.  
                    _data[i - 1] = _data[i];  // Move one element left.  
                }  // Close shift loop scope.  
                _size -= 1;  // Decrease size after shifting.  
                return new DequeueResult(value: removed, cost: new OperationCost(copied: 0, moved: moved));  // Return value + shift cost.  
            }  // Close Dequeue.  
        }  // Close ArrayQueue.  

        internal static EnqueueSummary SimulateEnqueues(int m)  // Simulate m enqueues and summarize growth/copy costs.  
        {  // Open method scope.  
            if (m < 0)  // Reject invalid counts.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(m), "m must be >= 0");  // Signal invalid input.  
            }  // Close validation scope.  
            var q = new ArrayQueue();  // Fresh queue for deterministic results.  
            long totalActualCost = 0;  // Accumulate total cost (1 write + copied).  
            int maxCopied = 0;  // Track maximum copied elements in a single enqueue.  
            for (int i = 0; i < m; i++)  // Perform m enqueues.  
            {  // Open loop scope.  
                OperationCost cost = q.Enqueue(i);  // Enqueue deterministic value and get per-op cost.  
                totalActualCost += 1 + cost.Copied;  // Add write cost + resize copy cost.  
                maxCopied = Math.Max(maxCopied, cost.Copied);  // Update maximum copied.  
            }  // Close loop scope.  
            return new EnqueueSummary(  // Return summary record.  
                m: m,  // Store operation count.  
                finalSize: q.Size,  // Store final size.  
                finalCapacity: q.Capacity,  // Store final capacity.  
                totalCopies: q.TotalCopies,  // Store total copies.  
                totalActualCost: totalActualCost,  // Store total actual cost.  
                maxCopiedInOneOp: maxCopied  // Store max per-op copied.  
            );  // Close summary construction.  
        }  // Close SimulateEnqueues.  

        internal static ArrayQueue BuildOrderedQueue(int n)  // Build a queue containing [0, 1, ..., n-1].  
        {  // Open method scope.  
            if (n < 0)  // Reject invalid sizes.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(n), "n must be >= 0");  // Signal invalid input.  
            }  // Close validation scope.  
            var q = new ArrayQueue();  // Create empty queue.  
            for (int v = 0; v < n; v++)  // Enqueue in increasing order.  
            {  // Open loop scope.  
                q.Enqueue(v);  // Enqueue one value.  
            }  // Close loop scope.  
            return q;  // Return built queue.  
        }  // Close BuildOrderedQueue.  

        internal static OperationCost SimulateDequeueCostAtSize(int n)  // Build size n queue then dequeue once and return cost.  
        {  // Open method scope.  
            ArrayQueue q = BuildOrderedQueue(n);  // Build queue of size n.  
            return q.Dequeue().Cost;  // Dequeue once and return cost.  
        }  // Close SimulateDequeueCostAtSize.  
    }  // Close class scope.  
}  // Close namespace scope.  

