// 01 堆疊示範（C++）/ Stack demo (C++).  // Bilingual header line for this unit.
#ifndef STACK_HPP  // Header guard to prevent multiple inclusion.
#define STACK_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max for max-cost tracking.
#include <memory>  // Provide std::unique_ptr for owning the backing buffer safely.
#include <stdexcept>  // Provide exceptions for validation.
#include <vector>  // Provide std::vector for returning copies of used elements.

namespace stackunit {  // Use a small namespace to avoid polluting the global namespace.

struct OperationCost {  // Record per-operation resize copy counts.
    int copied;  // How many elements were copied due to resize (0 if no resize).
};  // End of OperationCost.

struct PopResult {  // Return popped value plus its operation cost.
    int value;  // The popped element value.
    OperationCost cost;  // Resize-copy cost for this pop (always 0 in this unit).
};  // End of PopResult.

struct PushSummary {  // Summarize a sequence of pushes (growth behavior).
    int m;  // Number of pushes performed.
    int finalSize;  // Final size after pushes.
    int finalCapacity;  // Final capacity after growth.
    long long totalCopies;  // Total copied elements due to resizes.
    long long totalActualCost;  // Total cost if each push costs 1 write + copied.
    int maxCopiedInOneOp;  // Maximum copies in any single push.
};  // End of PushSummary.

class ArrayStack {  // An array-backed stack with doubling growth (teaching-oriented).
public:
    ArrayStack()  // Initialize an empty stack with capacity 1.
        : size_(0),  // Start with no stored elements.
          capacity_(1),  // Start with capacity 1 for deterministic doubling.
          data_(std::make_unique<int[]>(static_cast<size_t>(capacity_))),  // Allocate backing buffer.
          totalCopies_(0) {  // Start with zero total copies.
    }  // Close constructor.

    int size() const {  // Expose current size for callers/tests.
        return size_;  // Return number of stored items.
    }  // End size().

    int capacity() const {  // Expose current capacity for callers/tests.
        return capacity_;  // Return allocated slots.
    }  // End capacity().

    long long totalCopies() const {  // Expose total resize copies for analysis-style checks.
        return totalCopies_;  // Return total copied elements.
    }  // End totalCopies().

    bool isEmpty() const {  // Convenience helper to check emptiness.
        return size_ == 0;  // Empty iff size is zero.
    }  // End isEmpty().

    std::vector<int> toVector() const {  // Return a copy of the used portion (bottom -> top).
        std::vector<int> out;  // Output container.
        out.reserve(static_cast<size_t>(size_));  // Reserve to avoid reallocations.
        for (int i = 0; i < size_; i++) {  // Copy used portion.
            out.push_back(data_[static_cast<size_t>(i)]);  // Append one element.
        }  // Close loop.
        return out;  // Return copy.
    }  // End toVector().

    OperationCost push(int value) {  // Push to top (amortized O(1)).
        int copied = ensureCapacityForOneMore();  // Resize if needed.
        data_[static_cast<size_t>(size_)] = value;  // Write new value at the top slot.
        size_ += 1;  // Increase size.
        return OperationCost{copied};  // Return deterministic resize-copy cost.
    }  // End push().

    int peek() const {  // Return top value without removing it (O(1)).
        if (size_ == 0) {  // Reject peeking an empty stack.
            throw std::out_of_range("peek from empty stack");  // Signal invalid operation.
        }  // Close validation.
        return data_[static_cast<size_t>(size_ - 1)];  // Return top slot.
    }  // End peek().

    PopResult pop() {  // Pop top value (O(1) in this unit; no shrinking).
        if (size_ == 0) {  // Reject popping an empty stack.
            throw std::out_of_range("pop from empty stack");  // Signal invalid operation.
        }  // Close validation.
        size_ -= 1;  // Decrease size first so top index becomes size.
        int v = data_[static_cast<size_t>(size_)];  // Read removed slot.
        return PopResult{v, OperationCost{0}};  // Pop does not resize/copy in this unit.
    }  // End pop().

private:
    int size_;  // Number of stored elements.
    int capacity_;  // Allocated slots (always >= 1 in this unit).
    std::unique_ptr<int[]> data_;  // Backing buffer.
    long long totalCopies_;  // Total copied elements due to resizes.

    int resize(int newCapacity) {  // Resize buffer and return number of copied elements.
        if (newCapacity < size_) {  // Ensure new buffer can hold all existing elements.
            throw std::invalid_argument("newCapacity must be >= size");  // Signal invalid request.
        }  // Close validation.
        if (newCapacity < 1) {  // Keep capacity positive for simplicity.
            throw std::invalid_argument("newCapacity must be >= 1");  // Signal invalid request.
        }  // Close validation.

        std::unique_ptr<int[]> newData = std::make_unique<int[]>(static_cast<size_t>(newCapacity));  // Allocate new buffer.
        int copied = 0;  // Count copied elements deterministically.
        for (int i = 0; i < size_; i++) {  // Copy exactly the used portion.
            newData[static_cast<size_t>(i)] = data_[static_cast<size_t>(i)];  // Copy one element.
            copied += 1;  // Count one copy.
        }  // Close copy loop.
        data_ = std::move(newData);  // Swap buffer.
        capacity_ = newCapacity;  // Update capacity.
        totalCopies_ += static_cast<long long>(copied);  // Accumulate total copies.
        return copied;  // Return copies performed for this resize.
    }  // End resize().

    int ensureCapacityForOneMore() {  // Ensure capacity for one more push; return resize copies.
        if (size_ < capacity_) {  // Fast path: free space exists.
            return 0;  // No resize needed.
        }  // Close fast path.
        return resize(capacity_ * 2);  // Double capacity and return copies.
    }  // End ensureCapacityForOneMore().
};  // End ArrayStack.

inline PushSummary simulatePushes(int m) {  // Simulate m pushes and summarize growth/copy costs.
    if (m < 0) {  // Reject invalid counts.
        throw std::invalid_argument("m must be >= 0");  // Signal invalid input.
    }  // Close validation.
    ArrayStack s;  // Fresh stack for deterministic results.
    long long totalActualCost = 0;  // Accumulate total cost (1 write + copied).
    int maxCopied = 0;  // Track maximum copied elements in a single push.
    for (int i = 0; i < m; i++) {  // Perform m pushes.
        OperationCost cost = s.push(i);  // Push deterministic value and get per-op cost.
        totalActualCost += 1 + static_cast<long long>(cost.copied);  // Add write cost + resize copy cost.
        maxCopied = std::max(maxCopied, cost.copied);  // Update maximum copied.
    }  // Close loop.
    return PushSummary{m, s.size(), s.capacity(), s.totalCopies(), totalActualCost, maxCopied};  // Return summary.
}  // End simulatePushes().

}  // namespace stackunit  // Close namespace.

#endif  // STACK_HPP  // End of header guard.

