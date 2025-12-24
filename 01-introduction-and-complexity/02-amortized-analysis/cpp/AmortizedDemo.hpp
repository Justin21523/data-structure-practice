// 02 攤銷分析示範（C++）/ Amortized analysis demo (C++).  // Bilingual header line for this unit.
#ifndef AMORTIZED_DEMO_HPP  // Header guard to prevent multiple inclusion.
#define AMORTIZED_DEMO_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max for max-cost tracking.
#include <memory>  // Provide std::unique_ptr for owning the backing buffer safely.
#include <stdexcept>  // Provide std::invalid_argument/std::runtime_error for validation and invariants.
#include <vector>  // Provide std::vector for step logs.

namespace amortized {  // Use a small namespace to avoid polluting the global namespace.

inline int potential(int size, int capacity) {  // Compute Φ for the potential method.
    if (capacity < 1) {  // Reject invalid capacities so invariants remain well-defined.
        throw std::invalid_argument("capacity must be >= 1");  // Signal invalid input to the caller.
    }  // Close validation block.
    return (2 * size) - capacity + 1;  // Use Φ = 2·size - capacity + 1 so Φ(0,1)=0.
}  // End of potential.

struct AppendStep {  // Record one append operation's costs and amortized-analysis state.
    int index;  // 1-based operation index.
    int sizeBefore;  // Size before append.
    int capacityBefore;  // Capacity before append.
    int copied;  // Number of copied elements during resize.
    int actualCost;  // Actual cost: 1 write + copied moves.
    int phiBefore;  // Φ before operation.
    int phiAfter;  // Φ after operation.
    int amortizedCost;  // Amortized cost: actual + ΔΦ (should be 3).
    long long bankAfter;  // Accounting credit after charging 3 and paying actual cost.
};  // End of AppendStep.

struct SimulationSummary {  // Summarize costs for a sequence of m appends.
    int m;  // Number of operations.
    int finalSize;  // Final size.
    int finalCapacity;  // Final capacity (power of two >= m).
    long long totalActualCost;  // Total actual cost.
    long long totalCopies;  // Total copies.
    long long maxActualCost;  // Maximum per-operation actual cost.
    long long finalBank;  // Final credit balance.
};  // End of SimulationSummary.

class AmortizedDynamicArray {  // Minimal dynamic array instrumented for amortized analysis.
public:
    AmortizedDynamicArray()  // Initialize an empty table with capacity 1.
        : size_(0),  // Start with no stored elements.
          capacity_(1),  // Start with one slot to keep the model simple.
          data_(std::make_unique<int[]>(static_cast<size_t>(capacity_))),  // Allocate backing buffer.
          bank_(0),  // Start with zero credit.
          totalActualCost_(0),  // Start with zero total actual cost.
          totalCopies_(0),  // Start with zero total copies.
          maxActualCost_(0),  // Start with max cost = 0.
          steps_() {  // Start with an empty step log.
    }  // Close constructor.

    int size() const {  // Expose size for callers/tests.
        return size_;  // Return current size.
    }  // End of size().

    int capacity() const {  // Expose capacity for callers/tests.
        return capacity_;  // Return current capacity.
    }  // End of capacity().

    long long bank() const {  // Expose bank for callers/tests.
        return bank_;  // Return current credit balance.
    }  // End of bank().

    long long totalActualCost() const {  // Expose total actual cost.
        return totalActualCost_;  // Return total actual cost.
    }  // End of totalActualCost().

    long long totalCopies() const {  // Expose total copy count.
        return totalCopies_;  // Return total copies.
    }  // End of totalCopies().

    long long maxActualCost() const {  // Expose maximum per-operation actual cost.
        return maxActualCost_;  // Return maximum actual cost.
    }  // End of maxActualCost().

    const std::vector<AppendStep>& steps() const {  // Expose step log for tests.
        return steps_;  // Return a const reference for read-only access.
    }  // End of steps().

    AppendStep append(int value) {  // Append one value and return a detailed step record.
        int phiBefore = potential(size_, capacity_);  // Capture Φ before mutation.
        int sizeBefore = size_;  // Capture size before insertion.
        int capacityBefore = capacity_;  // Capture capacity before possible resize.

        int copied = 0;  // Default: no resize means zero copies.
        int actualCost = 1;  // Always pay 1 for writing the new element.
        if (size_ == capacity_) {  // Resize exactly when full (doubling rule).
            copied = resize(capacity_ * 2);  // Resize and count copies.
            actualCost += copied;  // Add copy cost to the actual cost.
        }  // Close resize branch.

        data_[static_cast<size_t>(size_)] = value;  // Write the new element to the next free slot.
        size_ += 1;  // Increase logical size after successful write.

        int phiAfter = potential(size_, capacity_);  // Capture Φ after mutation.
        int amortizedCost = actualCost + (phiAfter - phiBefore);  // Compute amortized cost via potential method.

        bank_ += 3 - static_cast<long long>(actualCost);  // Accounting method: charge 3 and pay actual cost.
        if (bank_ < 0) {  // Credit should never be negative for this classic amortized analysis.
            throw std::runtime_error("bank went negative (accounting invariant violated)");  // Fail fast.
        }  // Close invariant check.

        totalActualCost_ += static_cast<long long>(actualCost);  // Accumulate total actual cost.
        totalCopies_ += static_cast<long long>(copied);  // Accumulate total copies.
        maxActualCost_ = std::max(maxActualCost_, static_cast<long long>(actualCost));  // Track maximum cost observed.

        AppendStep step{  // Build a step record for inspection/tests.
            static_cast<int>(steps_.size() + 1),  // 1-based index.
            sizeBefore,  // Size before.
            capacityBefore,  // Capacity before.
            copied,  // Copies.
            actualCost,  // Actual cost.
            phiBefore,  // Φ before.
            phiAfter,  // Φ after.
            amortizedCost,  // Amortized cost.
            bank_,  // Bank after.
        };  // Close step initialization.
        steps_.push_back(step);  // Append to step log.
        return step;  // Return the record to the caller.
    }  // End of append().

private:
    int size_;  // Number of stored elements.
    int capacity_;  // Allocated capacity.
    std::unique_ptr<int[]> data_;  // Backing buffer for deterministic copy counting.

    long long bank_;  // Accounting-method credit balance.
    long long totalActualCost_;  // Total actual cost.
    long long totalCopies_;  // Total copies.
    long long maxActualCost_;  // Max per-op cost.
    std::vector<AppendStep> steps_;  // Per-operation log.

    int resize(int newCapacity) {  // Resize backing buffer and return number of copies performed.
        if (newCapacity < size_) {  // Ensure capacity can hold existing elements.
            throw std::invalid_argument("newCapacity must be >= size");  // Reject invalid resize request.
        }  // Close validation block.
        if (newCapacity < 1) {  // Keep capacity positive.
            throw std::invalid_argument("newCapacity must be >= 1");  // Reject invalid capacity.
        }  // Close validation block.

        std::unique_ptr<int[]> newData = std::make_unique<int[]>(static_cast<size_t>(newCapacity));  // Allocate new buffer.
        int copied = 0;  // Count how many elements are moved.
        for (int i = 0; i < size_; i++) {  // Copy existing elements.
            newData[static_cast<size_t>(i)] = data_[static_cast<size_t>(i)];  // Copy one element.
            copied += 1;  // Increment copy count.
        }  // Close copy loop.
        data_ = std::move(newData);  // Install new buffer.
        capacity_ = newCapacity;  // Update capacity.
        return copied;  // Return copy count.
    }  // End of resize().
};  // End of AmortizedDynamicArray.

inline bool isPowerOfTwo(int x) {  // Check whether x is a power of two (capacity invariant).
    return x > 0 && ((x & (x - 1)) == 0);  // Standard bit trick for powers of two.
}  // End of isPowerOfTwo.

inline SimulationSummary simulateAppends(int m) {  // Run m appends using a fresh table and return a summary.
    if (m < 0) {  // Reject negative counts because they do not make sense.
        throw std::invalid_argument("m must be >= 0");  // Signal invalid input.
    }  // Close validation block.

    AmortizedDynamicArray table;  // Create a fresh table for deterministic results.
    for (int i = 0; i < m; i++) {  // Perform m appends.
        table.append(i);  // Append deterministic value.
    }  // Close loop.

    return SimulationSummary{  // Return compact summary.
        m,  // m operations.
        table.size(),  // Final size.
        table.capacity(),  // Final capacity.
        table.totalActualCost(),  // Total cost.
        table.totalCopies(),  // Total copies.
        table.maxActualCost(),  // Max cost.
        table.bank(),  // Final bank.
    };  // Close summary initialization.
}  // End of simulateAppends.

}  // namespace amortized  // Close namespace.

#endif  // AMORTIZED_DEMO_HPP  // End of header guard.

