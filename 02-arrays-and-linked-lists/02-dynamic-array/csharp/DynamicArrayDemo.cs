// 02 動態陣列示範（C#）/ Dynamic array demo (C#).  // Bilingual file header.

using System;  // Provide exceptions and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for returning copies of used elements.  

namespace DynamicArrayUnit  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class DynamicArrayDemo  // Host DynamicArray implementation and simulation helpers.  
    {  // Open class scope.  
        internal readonly struct OperationCost  // Record per-operation copy/shift counts.  
        {  // Open struct scope.  
            public OperationCost(int copied, int moved)  // Construct an immutable cost record.  
            {  // Open constructor scope.  
                Copied = copied;  // Store copied count.  
                Moved = moved;  // Store moved count.  
            }  // Close constructor scope.  

            public int Copied { get; }  // How many elements were copied due to resize.  
            public int Moved { get; }  // How many elements were shifted due to insert/remove.  
        }  // Close struct scope.  

        internal readonly struct RemoveResult  // Return removed value plus its operation cost.  
        {  // Open struct scope.  
            public RemoveResult(int value, OperationCost cost)  // Construct immutable remove result.  
            {  // Open constructor scope.  
                Value = value;  // Store removed value.  
                Cost = cost;  // Store cost.  
            }  // Close constructor scope.  

            public int Value { get; }  // Removed element value.  
            public OperationCost Cost { get; }  // Copy/shift cost for this removal.  
        }  // Close struct scope.  

        internal readonly struct AppendSummary  // Summarize a sequence of appends (growth behavior).  
        {  // Open struct scope.  
            public AppendSummary(int m, int finalSize, int finalCapacity, long totalCopies, long totalActualCost, int maxCopiedInOneOp)  // Construct immutable summary.  
            {  // Open constructor scope.  
                M = m;  // Store m.  
                FinalSize = finalSize;  // Store size.  
                FinalCapacity = finalCapacity;  // Store capacity.  
                TotalCopies = totalCopies;  // Store copies.  
                TotalActualCost = totalActualCost;  // Store total cost.  
                MaxCopiedInOneOp = maxCopiedInOneOp;  // Store max copied.  
            }  // Close constructor scope.  

            public int M { get; }  // Number of operations.  
            public int FinalSize { get; }  // Final size.  
            public int FinalCapacity { get; }  // Final capacity.  
            public long TotalCopies { get; }  // Total copies due to resizes.  
            public long TotalActualCost { get; }  // Total cost (writes + copies).  
            public int MaxCopiedInOneOp { get; }  // Max copies in one append.  
        }  // Close struct scope.  

        internal sealed class DynamicArray  // A simple dynamic array with doubling growth (teaching-oriented).  
        {  // Open class scope.  
            private int _size;  // Track number of stored elements.  
            private int _capacity;  // Track allocated capacity.  
            private int[] _data;  // Backing buffer.  
            private long _totalCopies;  // Total elements copied due to resizes.  

            public DynamicArray()  // Initialize empty array with capacity 1.  
            {  // Open constructor scope.  
                _size = 0;  // Start empty.  
                _capacity = 1;  // Start capacity at 1 for deterministic doubling.  
                _data = new int[_capacity];  // Allocate backing buffer.  
                _totalCopies = 0;  // Start with zero total copies.  
            }  // Close constructor scope.  

            public int Size => _size;  // Expose size for callers/tests.  
            public int Capacity => _capacity;  // Expose capacity for callers/tests.  
            public long TotalCopies => _totalCopies;  // Expose total copies for analysis/tests.  

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

            private int Resize(int newCapacity)  // Resize backing buffer and return copied elements count.  
            {  // Open method scope.  
                if (newCapacity < _size)  // Ensure new capacity can hold existing elements.  
                {  // Open validation scope.  
                    throw new ArgumentException("newCapacity must be >= size");  // Signal invalid request.  
                }  // Close validation scope.  
                if (newCapacity < 1)  // Keep capacity positive.  
                {  // Open validation scope.  
                    throw new ArgumentException("newCapacity must be >= 1");  // Signal invalid request.  
                }  // Close validation scope.  

                int[] newData = new int[newCapacity];  // Allocate new buffer.  
                int copied = 0;  // Count copied elements deterministically.  
                for (int i = 0; i < _size; i++)  // Copy used portion.  
                {  // Open loop scope.  
                    newData[i] = _data[i];  // Copy one element.  
                    copied += 1;  // Count one copy.  
                }  // Close loop scope.  
                _data = newData;  // Swap buffer.  
                _capacity = newCapacity;  // Update capacity.  
                _totalCopies += copied;  // Accumulate total copies.  
                return copied;  // Return copies performed.  
            }  // Close Resize.  

            private int EnsureCapacityForOneMore()  // Ensure capacity for one more element; return resize copies.  
            {  // Open method scope.  
                if (_size < _capacity)  // Fast path: free space exists.  
                {  // Open fast path.  
                    return 0;  // No resize needed.  
                }  // Close fast path.  
                return Resize(_capacity * 2);  // Double capacity and return copies count.  
            }  // Close EnsureCapacityForOneMore.  

            public int Get(int index)  // Return element at index (O(1)).  
            {  // Open method scope.  
                RequireIndexInRange(index);  // Validate index.  
                return _data[index];  // Read from backing buffer.  
            }  // Close Get.  

            public void Set(int index, int value)  // Set element at index (O(1)).  
            {  // Open method scope.  
                RequireIndexInRange(index);  // Validate index.  
                _data[index] = value;  // Write to backing buffer.  
            }  // Close Set.  

            public List<int> ToList()  // Return a copy of used portion as a List<int>.  
            {  // Open method scope.  
                var result = new List<int>(_size);  // Pre-size list for efficiency.  
                for (int i = 0; i < _size; i++)  // Copy used elements.  
                {  // Open loop scope.  
                    result.Add(_data[i]);  // Append one element.  
                }  // Close loop scope.  
                return result;  // Return copy.  
            }  // Close ToList.  

            public int IndexOf(int value)  // Return first index of value or -1 (O(n)).  
            {  // Open method scope.  
                for (int i = 0; i < _size; i++)  // Linear scan.  
                {  // Open loop scope.  
                    if (_data[i] == value)  // Compare with target.  
                    {  // Open match scope.  
                        return i;  // Return first match.  
                    }  // Close match scope.  
                }  // Close loop scope.  
                return -1;  // Not found sentinel.  
            }  // Close IndexOf.  

            public OperationCost Append(int value)  // Append at end and return resize cost (moved=0).  
            {  // Open method scope.  
                int copied = EnsureCapacityForOneMore();  // Resize if needed.  
                _data[_size] = value;  // Write new element.  
                _size += 1;  // Increase size.  
                return new OperationCost(copied, 0);  // Return cost record.  
            }  // Close Append.  

            public OperationCost InsertAt(int index, int value)  // Insert at index and return cost (copies + shifts).  
            {  // Open method scope.  
                RequireInsertIndexInRange(index);  // Validate insertion index.  
                int copied = EnsureCapacityForOneMore();  // Resize if needed.  

                int moved = _size - index;  // Shifting right moves (size - index) elements.  
                for (int i = _size; i > index; i--)  // Shift right from tail toward index.  
                {  // Open loop scope.  
                    _data[i] = _data[i - 1];  // Move one element right.  
                }  // Close loop scope.  
                _data[index] = value;  // Write new value.  
                _size += 1;  // Increase size.  
                return new OperationCost(copied, moved);  // Return cost record.  
            }  // Close InsertAt.  

            public RemoveResult RemoveAt(int index)  // Remove at index and return removed value + shift cost.  
            {  // Open method scope.  
                RequireIndexInRange(index);  // Validate index.  
                int removedValue = _data[index];  // Capture removed value.  
                int moved = _size - index - 1;  // Shifting left moves (size - index - 1) elements.  
                for (int i = index; i < _size - 1; i++)  // Shift left to fill gap.  
                {  // Open loop scope.  
                    _data[i] = _data[i + 1];  // Move one element left.  
                }  // Close loop scope.  
                _size -= 1;  // Decrease size.  
                return new RemoveResult(removedValue, new OperationCost(0, moved));  // Return removed value and cost.  
            }  // Close RemoveAt.  
        }  // Close DynamicArray.  

        internal static bool IsPowerOfTwo(int x)  // Helper for capacity invariant.  
        {  // Open method scope.  
            return x > 0 && (x & (x - 1)) == 0;  // Standard bit trick.  
        }  // Close IsPowerOfTwo.  

        internal static AppendSummary SimulateAppends(int m)  // Simulate m appends and summarize growth/copy costs.  
        {  // Open method scope.  
            if (m < 0)  // Reject invalid counts.  
            {  // Open validation scope.  
                throw new ArgumentException("m must be >= 0");  // Signal invalid input.  
            }  // Close validation scope.  

            var a = new DynamicArray();  // Fresh array for deterministic results.  
            long totalActualCost = 0;  // Accumulate total cost (1 write + copied).  
            int maxCopied = 0;  // Track max copied in one append.  
            for (int i = 0; i < m; i++)  // Perform m appends.  
            {  // Open loop scope.  
                OperationCost cost = a.Append(i);  // Append deterministic value.  
                totalActualCost += 1 + cost.Copied;  // Add write + copy cost.  
                maxCopied = Math.Max(maxCopied, cost.Copied);  // Update max copied.  
            }  // Close loop scope.  

            return new AppendSummary(  // Return summary record.  
                m: m,  // Store m.  
                finalSize: a.Size,  // Store final size.  
                finalCapacity: a.Capacity,  // Store final capacity.  
                totalCopies: a.TotalCopies,  // Store total copies.  
                totalActualCost: totalActualCost,  // Store total cost.  
                maxCopiedInOneOp: maxCopied  // Store max copied.  
            );  // Close summary construction.  
        }  // Close SimulateAppends.  

        internal static DynamicArray BuildFilledArray(int n)  // Build a dynamic array filled with 0..n-1.  
        {  // Open method scope.  
            if (n < 0)  // Reject invalid sizes.  
            {  // Open validation scope.  
                throw new ArgumentException("n must be >= 0");  // Signal invalid input.  
            }  // Close validation scope.  
            var a = new DynamicArray();  // Start empty.  
            for (int i = 0; i < n; i++)  // Fill with deterministic values.  
            {  // Open loop scope.  
                a.Append(i);  // Append one value.  
            }  // Close loop scope.  
            return a;  // Return filled array.  
        }  // Close BuildFilledArray.  

        internal static OperationCost SimulateAppendCostAtSize(int n)  // Build size n then append once and return cost.  
        {  // Open method scope.  
            DynamicArray a = BuildFilledArray(n);  // Build array.  
            return a.Append(999);  // Append sentinel.  
        }  // Close SimulateAppendCostAtSize.  

        internal static OperationCost SimulateInsert0CostAtSize(int n)  // Build size n then insert at head and return cost.  
        {  // Open method scope.  
            DynamicArray a = BuildFilledArray(n);  // Build array.  
            return a.InsertAt(0, 999);  // Insert sentinel at head.  
        }  // Close SimulateInsert0CostAtSize.  
    }  // Close DynamicArrayDemo.  
}  // Close namespace scope.  

