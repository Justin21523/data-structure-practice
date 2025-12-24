// 01 堆疊示範（C#）/ Stack demo (C#).  // Bilingual file header.

using System;  // Provide exceptions and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for ToList conversions.  

namespace StackUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class StackDemo  // Host ArrayStack implementation and simulation helpers.  
    {  // Open class scope.  
        internal readonly struct OperationCost  // Record per-operation resize copy counts.  
        {  // Open struct scope.  
            public OperationCost(int copied)  // Construct an immutable cost record.  
            {  // Open constructor scope.  
                Copied = copied;  // Store copy count.  
            }  // Close constructor scope.  

            public int Copied { get; }  // How many elements were copied due to resize (0 if no resize).  
        }  // Close struct scope.  

        internal readonly struct PopResult  // Return popped value plus its operation cost.  
        {  // Open struct scope.  
            public PopResult(int value, OperationCost cost)  // Construct immutable pop result.  
            {  // Open constructor scope.  
                Value = value;  // Store popped value.  
                Cost = cost;  // Store cost record.  
            }  // Close constructor scope.  

            public int Value { get; }  // The popped element value.  
            public OperationCost Cost { get; }  // Resize-copy cost for this pop (always 0 in this unit).  
        }  // Close struct scope.  

        internal readonly struct PushSummary  // Summarize a sequence of pushes (growth behavior).  
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

        internal sealed class ArrayStack  // An array-backed stack with doubling growth (teaching-oriented).  
        {  // Open class scope.  
            private int _size;  // Number of stored elements.  
            private int _capacity;  // Allocated slots (always >= 1 in this unit).  
            private int[] _data;  // Backing buffer.  
            private long _totalCopies;  // Total copied elements due to resizes.  

            public ArrayStack()  // Initialize an empty stack with capacity 1.  
            {  // Open constructor scope.  
                _size = 0;  // Empty stack has size 0.  
                _capacity = 1;  // Start with capacity 1 for deterministic doubling.  
                _data = new int[_capacity];  // Allocate backing buffer.  
                _totalCopies = 0;  // Start with zero total copies.  
            }  // Close constructor scope.  

            public int Size => _size;  // Expose current size for callers/tests.  
            public int Capacity => _capacity;  // Expose current capacity for callers/tests.  
            public long TotalCopies => _totalCopies;  // Expose total resize copies for analysis-style checks.  
            public bool IsEmpty => _size == 0;  // Expose emptiness check.  

            public List<int> ToList()  // Return a copy of the used portion (bottom -> top).  
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

            private int EnsureCapacityForOneMore()  // Ensure capacity for one more push; return resize copies.  
            {  // Open method scope.  
                if (_size < _capacity)  // Fast path: free space exists.  
                {  // Open fast path scope.  
                    return 0;  // No resize needed.  
                }  // Close fast path scope.  
                return Resize(_capacity * 2);  // Double capacity and return copies.  
            }  // Close EnsureCapacityForOneMore.  

            public OperationCost Push(int value)  // Push to top (amortized O(1)).  
            {  // Open method scope.  
                int copied = EnsureCapacityForOneMore();  // Resize if needed.  
                _data[_size] = value;  // Write new value at top slot.  
                _size += 1;  // Increase size.  
                return new OperationCost(copied: copied);  // Return deterministic resize-copy cost.  
            }  // Close Push.  

            public int Peek()  // Return top value without removing it (O(1)).  
            {  // Open method scope.  
                if (_size == 0)  // Reject peeking an empty stack.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("peek from empty stack");  // Signal invalid operation.  
                }  // Close validation scope.  
                return _data[_size - 1];  // Return top slot.  
            }  // Close Peek.  

            public PopResult Pop()  // Pop top value (O(1) in this unit; no shrinking).  
            {  // Open method scope.  
                if (_size == 0)  // Reject popping an empty stack.  
                {  // Open validation scope.  
                    throw new IndexOutOfRangeException("pop from empty stack");  // Signal invalid operation.  
                }  // Close validation scope.  
                _size -= 1;  // Decrease size first so top index becomes size.  
                int v = _data[_size];  // Read removed slot.  
                return new PopResult(value: v, cost: new OperationCost(copied: 0));  // Pop does not resize/copy in this unit.  
            }  // Close Pop.  
        }  // Close ArrayStack.  

        internal static PushSummary SimulatePushes(int m)  // Simulate m pushes and summarize growth/copy costs.  
        {  // Open method scope.  
            if (m < 0)  // Reject invalid counts.  
            {  // Open validation scope.  
                throw new ArgumentOutOfRangeException(nameof(m), "m must be >= 0");  // Signal invalid input.  
            }  // Close validation scope.  
            var s = new ArrayStack();  // Fresh stack for deterministic results.  
            long totalActualCost = 0;  // Accumulate total cost (1 write + copied).  
            int maxCopied = 0;  // Track maximum copied elements in a single push.  
            for (int i = 0; i < m; i++)  // Perform m pushes.  
            {  // Open loop scope.  
                OperationCost cost = s.Push(i);  // Push deterministic value and get per-op cost.  
                totalActualCost += 1 + cost.Copied;  // Add write cost + resize copy cost.  
                maxCopied = Math.Max(maxCopied, cost.Copied);  // Update maximum copied.  
            }  // Close loop scope.  
            return new PushSummary(  // Return summary record.  
                m: m,  // Store operation count.  
                finalSize: s.Size,  // Store final size.  
                finalCapacity: s.Capacity,  // Store final capacity.  
                totalCopies: s.TotalCopies,  // Store total copies.  
                totalActualCost: totalActualCost,  // Store total actual cost.  
                maxCopiedInOneOp: maxCopied  // Store max per-op copied.  
            );  // Close summary construction.  
        }  // Close SimulatePushes.  
    }  // Close class scope.  
}  // Close namespace scope.  

