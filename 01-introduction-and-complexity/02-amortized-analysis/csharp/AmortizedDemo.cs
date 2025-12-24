// 02 攤銷分析示範（C#）/ Amortized analysis demo (C#).  // Bilingual file header.

using System;  // Provide core runtime types and exceptions.  
using System.Collections.Generic;  // Provide List<T> for step logs and CLI parsing.  

namespace AmortizedAnalysis  // Keep this unit isolated within its own namespace.  
{  // Open namespace scope.  
    internal static class AmortizedDemo  // Host potential function and dynamic array implementation.  
    {  // Open class scope.  
        public static int Potential(int size, int capacity)  // Compute Φ for the potential method.  
        {  // Open method scope.  
            if (capacity < 1)  // Reject invalid capacities so invariants remain well-defined.  
            {  // Open validation scope.  
                throw new ArgumentException("capacity must be >= 1");  // Fail fast with a clear message.  
            }  // Close validation scope.  
            return (2 * size) - capacity + 1;  // Use Φ = 2·size - capacity + 1 so Φ(0,1)=0.  
        }  // Close Potential.  

        internal readonly struct AppendStep  // Record one append operation's costs and amortized-analysis state.  
        {  // Open struct scope.  
            public AppendStep(  // Construct an immutable step record.  
                int index,  // 1-based operation index.  
                int sizeBefore,  // Size before append.  
                int capacityBefore,  // Capacity before append.  
                int copied,  // Copied elements during resize.  
                int actualCost,  // Actual cost for this append.  
                int phiBefore,  // Φ before append.  
                int phiAfter,  // Φ after append.  
                int amortizedCost,  // Amortized cost via potential method.  
                long bankAfter  // Bank after accounting charge/payment.  
            )  // Close constructor signature.  
            {  // Open constructor body.  
                Index = index;  // Store index.  
                SizeBefore = sizeBefore;  // Store sizeBefore.  
                CapacityBefore = capacityBefore;  // Store capacityBefore.  
                Copied = copied;  // Store copied.  
                ActualCost = actualCost;  // Store actualCost.  
                PhiBefore = phiBefore;  // Store phiBefore.  
                PhiAfter = phiAfter;  // Store phiAfter.  
                AmortizedCost = amortizedCost;  // Store amortizedCost.  
                BankAfter = bankAfter;  // Store bankAfter.  
            }  // Close constructor body.  

            public int Index { get; }  // 1-based operation index.  
            public int SizeBefore { get; }  // Size before operation.  
            public int CapacityBefore { get; }  // Capacity before operation.  
            public int Copied { get; }  // Copy count in this operation.  
            public int ActualCost { get; }  // Actual cost in this operation.  
            public int PhiBefore { get; }  // Φ before.  
            public int PhiAfter { get; }  // Φ after.  
            public int AmortizedCost { get; }  // Amortized cost (should be 3).  
            public long BankAfter { get; }  // Bank after operation.  
        }  // Close struct scope.  

        internal readonly struct SimulationSummary  // Summarize costs for m appends.  
        {  // Open struct scope.  
            public SimulationSummary(  // Construct an immutable summary record.  
                int m,  // Number of operations.  
                int finalSize,  // Final size.  
                int finalCapacity,  // Final capacity.  
                long totalActualCost,  // Total actual cost.  
                long totalCopies,  // Total copies.  
                long maxActualCost,  // Max per-op cost.  
                long finalBank  // Final bank balance.  
            )  // Close constructor signature.  
            {  // Open constructor body.  
                M = m;  // Store m.  
                FinalSize = finalSize;  // Store finalSize.  
                FinalCapacity = finalCapacity;  // Store finalCapacity.  
                TotalActualCost = totalActualCost;  // Store totalActualCost.  
                TotalCopies = totalCopies;  // Store totalCopies.  
                MaxActualCost = maxActualCost;  // Store maxActualCost.  
                FinalBank = finalBank;  // Store finalBank.  
            }  // Close constructor body.  

            public int M { get; }  // Operation count.  
            public int FinalSize { get; }  // Final size.  
            public int FinalCapacity { get; }  // Final capacity.  
            public long TotalActualCost { get; }  // Total actual cost.  
            public long TotalCopies { get; }  // Total copies.  
            public long MaxActualCost { get; }  // Max cost.  
            public long FinalBank { get; }  // Final bank.  
        }  // Close struct scope.  

        internal sealed class AmortizedDynamicArray  // Minimal dynamic array instrumented for amortized analysis.  
        {  // Open class scope.  
            private int _size;  // Track logical size (stored elements).  
            private int _capacity;  // Track allocated capacity (slots).  
            private int[] _data;  // Backing fixed-size buffer (manually resized).  

            private long _bank;  // Accounting-method credit balance.  
            private long _totalActualCost;  // Aggregate total actual cost.  
            private long _totalCopies;  // Aggregate total copy count.  
            private long _maxActualCost;  // Track maximum per-operation cost.  
            private readonly List<AppendStep> _steps;  // Keep per-step log for tests.  

            public AmortizedDynamicArray()  // Initialize an empty table with capacity 1.  
            {  // Open constructor scope.  
                _size = 0;  // Start with no elements.  
                _capacity = 1;  // Start with one slot for a simple deterministic model.  
                _data = new int[_capacity];  // Allocate backing buffer.  

                _bank = 0;  // Start with zero credit.  
                _totalActualCost = 0;  // Start totals at zero.  
                _totalCopies = 0;  // Start totals at zero.  
                _maxActualCost = 0;  // Start max at zero.  
                _steps = new List<AppendStep>();  // Allocate step log list.  
            }  // Close constructor scope.  

            public int Size => _size;  // Expose size for callers/tests.  
            public int Capacity => _capacity;  // Expose capacity for callers/tests.  
            public long Bank => _bank;  // Expose bank for callers/tests.  
            public long TotalActualCost => _totalActualCost;  // Expose total actual cost.  
            public long TotalCopies => _totalCopies;  // Expose total copies.  
            public long MaxActualCost => _maxActualCost;  // Expose max actual cost.  
            public IReadOnlyList<AppendStep> Steps => _steps;  // Expose steps as read-only list.  

            private int Resize(int newCapacity)  // Resize backing buffer and return number of copies performed.  
            {  // Open method scope.  
                if (newCapacity < _size)  // Ensure we can hold existing elements.  
                {  // Open validation scope.  
                    throw new ArgumentException("newCapacity must be >= size");  // Reject invalid requests.  
                }  // Close validation scope.  
                if (newCapacity < 1)  // Keep capacity positive.  
                {  // Open validation scope.  
                    throw new ArgumentException("newCapacity must be >= 1");  // Reject invalid requests.  
                }  // Close validation scope.  

                int[] newData = new int[newCapacity];  // Allocate new fixed-size array.  
                int copied = 0;  // Count how many elements we copy.  
                for (int i = 0; i < _size; i++)  // Copy exactly the stored elements.  
                {  // Open copy loop scope.  
                    newData[i] = _data[i];  // Copy one element (one "move" in our cost model).  
                    copied += 1;  // Increment copy counter.  
                }  // Close copy loop scope.  
                _data = newData;  // Swap backing buffer.  
                _capacity = newCapacity;  // Update capacity to match the new buffer.  
                return copied;  // Return copy count as resize cost.  
            }  // Close Resize.  

            public AppendStep Append(int value)  // Append one value and return a step record.  
            {  // Open method scope.  
                int phiBefore = Potential(_size, _capacity);  // Capture Φ before mutation.  
                int sizeBefore = _size;  // Capture size before insertion.  
                int capacityBefore = _capacity;  // Capture capacity before possible resize.  

                int copied = 0;  // Default: no resize means no copies.  
                int actualCost = 1;  // Always pay 1 for writing the new element.  
                if (_size == _capacity)  // Resize exactly when full (doubling rule).  
                {  // Open resize branch.  
                    copied = Resize(_capacity * 2);  // Resize and count copies.  
                    actualCost += copied;  // Add copy cost to actual cost.  
                }  // Close resize branch.  

                _data[_size] = value;  // Write new element into next free slot.  
                _size += 1;  // Increase logical size after write.  

                int phiAfter = Potential(_size, _capacity);  // Capture Φ after mutation.  
                int amortizedCost = actualCost + (phiAfter - phiBefore);  // Compute amortized cost via potential method.  

                _bank += 3 - actualCost;  // Accounting method: charge 3 and pay actual cost.  
                if (_bank < 0)  // Credit should never be negative for this classic analysis.  
                {  // Open invariant scope.  
                    throw new InvalidOperationException("bank went negative (accounting invariant violated)");  // Fail fast.  
                }  // Close invariant scope.  

                _totalActualCost += actualCost;  // Accumulate total actual cost.  
                _totalCopies += copied;  // Accumulate total copies.  
                _maxActualCost = Math.Max(_maxActualCost, actualCost);  // Track maximum per-op cost.  

                var step = new AppendStep(  // Construct step record for tests/inspection.  
                    index: _steps.Count + 1,  // 1-based index.  
                    sizeBefore: sizeBefore,  // Size before.  
                    capacityBefore: capacityBefore,  // Capacity before.  
                    copied: copied,  // Copies.  
                    actualCost: actualCost,  // Actual cost.  
                    phiBefore: phiBefore,  // Φ before.  
                    phiAfter: phiAfter,  // Φ after.  
                    amortizedCost: amortizedCost,  // Amortized cost.  
                    bankAfter: _bank  // Bank after.  
                );  // Close step construction.  
                _steps.Add(step);  // Append to step log.  
                return step;  // Return record to caller.  
            }  // Close Append.  
        }  // Close class scope.  

        internal static SimulationSummary SimulateAppends(int m)  // Run m appends and return a summary.  
        {  // Open method scope.  
            if (m < 0)  // Reject invalid negative counts.  
            {  // Open validation scope.  
                throw new ArgumentException("m must be >= 0");  // Signal invalid input.  
            }  // Close validation scope.  

            var table = new AmortizedDynamicArray();  // Use a fresh table for deterministic results.  
            for (int i = 0; i < m; i++)  // Perform m appends.  
            {  // Open loop scope.  
                table.Append(i);  // Append deterministic value.  
            }  // Close loop scope.  

            return new SimulationSummary(  // Return a compact summary record.  
                m: m,  // Operation count.  
                finalSize: table.Size,  // Final size.  
                finalCapacity: table.Capacity,  // Final capacity.  
                totalActualCost: table.TotalActualCost,  // Total actual cost.  
                totalCopies: table.TotalCopies,  // Total copies.  
                maxActualCost: table.MaxActualCost,  // Max actual cost.  
                finalBank: table.Bank  // Final bank.  
            );  // Close summary construction.  
        }  // Close SimulateAppends.  

        internal static bool IsPowerOfTwo(int x)  // Check whether x is a power of two.  
        {  // Open method scope.  
            return x > 0 && (x & (x - 1)) == 0;  // Standard bit trick for powers of two.  
        }  // Close IsPowerOfTwo.  
    }  // Close class scope.  
}  // Close namespace scope.  

