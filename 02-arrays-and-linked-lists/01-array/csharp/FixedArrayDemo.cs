// 01 靜態陣列示範（C#）/ Static array demo (C#).  // Bilingual file header.

using System;  // Provide exceptions and basic runtime types.  
using System.Collections.Generic;  // Provide List<T> for returning copies of used elements.  

namespace StaticArray  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class FixedArrayDemo  // Host FixedArray implementation and simulation helpers.  
    {  // Open class scope.  
        internal readonly struct RemoveResult  // Return both removed value and moved elements count.  
        {  // Open struct scope.  
            public RemoveResult(int value, int moved)  // Construct an immutable remove result record.  
            {  // Open constructor scope.  
                Value = value;  // Store removed value.  
                Moved = moved;  // Store moved count.  
            }  // Close constructor scope.  

            public int Value { get; }  // Removed element value.  
            public int Moved { get; }  // How many elements were shifted left.  
        }  // Close struct scope.  

        internal sealed class FixedArray  // A fixed-capacity array with manual shifting (teaching-oriented).  
        {  // Open class scope.  
            private readonly int _capacity;  // Store fixed capacity for overflow checks.  
            private int _size;  // Track current number of stored elements.  
            private readonly int[] _data;  // Backing storage for contiguous-array behavior.  

            public FixedArray(int capacity)  // Create an empty fixed array with a given capacity.  
            {  // Open constructor scope.  
                if (capacity < 0)  // Reject negative capacities.  
                {  // Open validation scope.  
                    throw new ArgumentException("capacity must be >= 0");  // Signal invalid input.  
                }  // Close validation scope.  
                _capacity = capacity;  // Store capacity.  
                _size = 0;  // Start empty.  
                _data = new int[capacity];  // Allocate backing buffer.  
            }  // Close constructor scope.  

            public int Capacity => _capacity;  // Expose capacity for callers/tests.  
            public int Size => _size;  // Expose size for callers/tests.  

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

            private void RequireNotFull()  // Ensure there is at least one free slot.  
            {  // Open method scope.  
                if (_size >= _capacity)  // Detect overflow when array is full.  
                {  // Open validation scope.  
                    throw new InvalidOperationException("fixed array is full");  // Signal overflow.  
                }  // Close validation scope.  
            }  // Close RequireNotFull.  

            public int Get(int index)  // Return element at index (O(1)).  
            {  // Open method scope.  
                RequireIndexInRange(index);  // Validate index.  
                return _data[index];  // Read from backing storage.  
            }  // Close Get.  

            public void Set(int index, int value)  // Set element at index (O(1)).  
            {  // Open method scope.  
                RequireIndexInRange(index);  // Validate index.  
                _data[index] = value;  // Write to backing storage.  
            }  // Close Set.  

            public List<int> ToList()  // Return a copy of the used portion as a List<int>.  
            {  // Open method scope.  
                var result = new List<int>(_size);  // Pre-size list to avoid re-allocations.  
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
                return -1;  // Return not found sentinel.  
            }  // Close IndexOf.  

            public int Append(int value)  // Append at end and return moved count (always 0 when not full).  
            {  // Open method scope.  
                return InsertAt(_size, value);  // Delegate to InsertAt for consistent behavior.  
            }  // Close Append.  

            public int InsertAt(int index, int value)  // Insert value at index and return moved elements count.  
            {  // Open method scope.  
                RequireNotFull();  // Ensure free space exists.  
                RequireInsertIndexInRange(index);  // Validate insertion index.  

                int moved = _size - index;  // Shifting right moves exactly (size - index) elements.  
                for (int i = _size; i > index; i--)  // Shift right from tail toward index.  
                {  // Open loop scope.  
                    _data[i] = _data[i - 1];  // Move one element one step to the right.  
                }  // Close loop scope.  
                _data[index] = value;  // Write new value into the gap.  
                _size += 1;  // Increase size after insertion.  
                return moved;  // Return moved count for visibility.  
            }  // Close InsertAt.  

            public RemoveResult RemoveAt(int index)  // Remove element at index and return removed value + moved count.  
            {  // Open method scope.  
                RequireIndexInRange(index);  // Validate index.  

                int removedValue = _data[index];  // Capture removed value.  
                int moved = _size - index - 1;  // Shifting left moves exactly (size - index - 1) elements.  
                for (int i = index; i < _size - 1; i++)  // Shift left to fill removed slot.  
                {  // Open loop scope.  
                    _data[i] = _data[i + 1];  // Move one element one step to the left.  
                }  // Close loop scope.  
                _size -= 1;  // Decrease size after removal.  
                return new RemoveResult(removedValue, moved);  // Return removed value and move count.  
            }  // Close RemoveAt.  
        }  // Close FixedArray.  

        internal static FixedArray BuildFilledArray(int n, int extraCapacity)  // Build array filled with 0..n-1.  
        {  // Open method scope.  
            if (n < 0 || extraCapacity < 0)  // Validate parameters.  
            {  // Open validation scope.  
                throw new ArgumentException("n and extraCapacity must be >= 0");  // Signal invalid input.  
            }  // Close validation scope.  
            var a = new FixedArray(n + extraCapacity);  // Allocate capacity to allow insert when needed.  
            for (int i = 0; i < n; i++)  // Fill with deterministic values.  
            {  // Open loop scope.  
                a.Append(i);  // Append one value.  
            }  // Close loop scope.  
            return a;  // Return filled array.  
        }  // Close BuildFilledArray.  

        internal static int SimulateInsertMoves(int n, int index)  // Simulate insert and return moved count.  
        {  // Open method scope.  
            FixedArray a = BuildFilledArray(n, 1);  // Build size n with one extra slot.  
            return a.InsertAt(index, 999);  // Insert sentinel and return moved count.  
        }  // Close SimulateInsertMoves.  

        internal static int SimulateRemoveMoves(int n, int index)  // Simulate remove and return moved count.  
        {  // Open method scope.  
            FixedArray a = BuildFilledArray(n, 0);  // Build size n exactly.  
            return a.RemoveAt(index).Moved;  // Remove and return moved count.  
        }  // Close SimulateRemoveMoves.  
    }  // Close FixedArrayDemo.  
}  // Close namespace scope.  

