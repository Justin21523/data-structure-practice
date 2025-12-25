// 04 雙端佇列示範（C++）/ Deque demo (C++).  // Bilingual header line for this unit.
#ifndef DEQUE_HPP  // Header guard to prevent multiple inclusion.
#define DEQUE_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max for max-cost tracking.
#include <memory>  // Provide std::unique_ptr for owning the backing buffer safely.
#include <stdexcept>  // Provide exceptions for validation.
#include <vector>  // Provide std::vector for returning copies of used elements.

namespace dequeunit {  // Use a small namespace to avoid polluting the global namespace.

struct OperationCost {  // Record per-operation copy/shift counts.
    int copied;  // How many elements were copied due to resize (0 if no resize).
    int moved;  // How many elements were shifted due to an operation (should be 0 here).
};  // End of OperationCost.

struct PopResult {  // Return popped value plus its operation cost.
    int value;  // The popped element value.
    OperationCost cost;  // Copy/shift cost for this pop.
};  // End of PopResult.

struct PushSummary {  // Summarize a sequence of pushBack operations (growth behavior).
    int m;  // Number of pushes performed.
    int finalSize;  // Final size after pushes.
    int finalCapacity;  // Final capacity after growth.
    long long totalCopies;  // Total copied elements due to resizes.
    long long totalActualCost;  // Total cost if each push costs 1 write + copied.
    int maxCopiedInOneOp;  // Maximum copies in any single push.
};  // End of PushSummary.

class Deque {  // A circular-buffer deque with doubling growth (teaching-oriented).
public:
    Deque()  // Initialize an empty deque with capacity 1.
        : size_(0),  // Start with no stored elements.
          capacity_(1),  // Start with capacity 1 for deterministic doubling.
          head_(0),  // Head starts at index 0.
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

    std::vector<int> toVector() const {  // Return a copy of the used portion (front -> back).
        std::vector<int> out;  // Output container.
        out.reserve(static_cast<size_t>(size_));  // Reserve to avoid reallocations.
        for (int i = 0; i < size_; i++) {  // Copy elements in deque order.
            out.push_back(data_[static_cast<size_t>(indexAt(i))]);  // Append one element.
        }  // Close loop.
        return out;  // Return copy.
    }  // End toVector().

    OperationCost pushBack(int value) {  // Push at back (amortized O(1)).
        int copied = ensureCapacityForOneMore();  // Resize if needed.
        int tailIndex = indexAt(size_);  // Compute tail index where new element is written.
        data_[static_cast<size_t>(tailIndex)] = value;  // Store value at tail position.
        size_ += 1;  // Increase size.
        return OperationCost{copied, 0};  // moved stays 0 (no shifting).
    }  // End pushBack().

    OperationCost pushFront(int value) {  // Push at front (amortized O(1)).
        int copied = ensureCapacityForOneMore();  // Resize if needed.
        head_ = (head_ - 1 + capacity_) % capacity_;  // Move head left by one with wrap-around.
        data_[static_cast<size_t>(head_)] = value;  // Store value at new head position.
        size_ += 1;  // Increase size.
        return OperationCost{copied, 0};  // moved stays 0 (no shifting).
    }  // End pushFront().

    int peekFront() const {  // Return front value without removing it (O(1)).
        if (size_ == 0) {  // Reject peeking an empty deque.
            throw std::out_of_range("peek from empty deque");  // Signal invalid operation.
        }  // Close validation.
        return data_[static_cast<size_t>(head_)];  // Return front slot directly.
    }  // End peekFront().

    int peekBack() const {  // Return back value without removing it (O(1)).
        if (size_ == 0) {  // Reject peeking an empty deque.
            throw std::out_of_range("peek from empty deque");  // Signal invalid operation.
        }  // Close validation.
        int tailIndex = indexAt(size_ - 1);  // Compute index of last element.
        return data_[static_cast<size_t>(tailIndex)];  // Return back slot.
    }  // End peekBack().

    PopResult popFront() {  // Pop from front (O(1); no shifting).
        if (size_ == 0) {  // Reject popping an empty deque.
            throw std::out_of_range("pop from empty deque");  // Signal invalid operation.
        }  // Close validation.
        int removed = data_[static_cast<size_t>(head_)];  // Capture front value.
        head_ = (head_ + 1) % capacity_;  // Advance head by one with wrap-around.
        size_ -= 1;  // Decrease size.
        if (size_ == 0) {  // Normalize head for deterministic state when deque becomes empty.
            head_ = 0;  // Reset head (optional but stable).
        }  // Close normalize branch.
        return PopResult{removed, OperationCost{0, 0}};  // moved stays 0 (no shift).
    }  // End popFront().

    PopResult popBack() {  // Pop from back (O(1); no shifting).
        if (size_ == 0) {  // Reject popping an empty deque.
            throw std::out_of_range("pop from empty deque");  // Signal invalid operation.
        }  // Close validation.
        int tailIndex = indexAt(size_ - 1);  // Compute index of last element.
        int removed = data_[static_cast<size_t>(tailIndex)];  // Capture back value.
        size_ -= 1;  // Decrease size.
        if (size_ == 0) {  // Normalize head for deterministic state when deque becomes empty.
            head_ = 0;  // Reset head (optional but stable).
        }  // Close normalize branch.
        return PopResult{removed, OperationCost{0, 0}};  // moved stays 0 (no shift).
    }  // End popBack().

private:
    int size_;  // Number of stored elements.
    int capacity_;  // Allocated slots (always >= 1 in this unit).
    int head_;  // Head index (front position) within the buffer.
    std::unique_ptr<int[]> data_;  // Backing buffer.
    long long totalCopies_;  // Total copied elements due to resizes.

    int indexAt(int offset) const {  // Map logical offset [0..size) to physical buffer index.
        return (head_ + offset) % capacity_;  // Wrap around using modulo.
    }  // End indexAt().

    int resize(int newCapacity) {  // Resize buffer and return number of copied elements.
        if (newCapacity < size_) {  // Ensure new buffer can hold all existing elements.
            throw std::invalid_argument("newCapacity must be >= size");  // Signal invalid request.
        }  // Close validation.
        if (newCapacity < 1) {  // Keep capacity positive for simplicity.
            throw std::invalid_argument("newCapacity must be >= 1");  // Signal invalid request.
        }  // Close validation.

        std::unique_ptr<int[]> newData = std::make_unique<int[]>(static_cast<size_t>(newCapacity));  // Allocate new buffer.
        int copied = 0;  // Count copied elements deterministically.
        for (int i = 0; i < size_; i++) {  // Copy elements in deque order.
            newData[static_cast<size_t>(i)] = data_[static_cast<size_t>(indexAt(i))];  // Copy one element into compact layout.
            copied += 1;  // Count one copy.
        }  // Close copy loop.
        data_ = std::move(newData);  // Swap buffer.
        capacity_ = newCapacity;  // Update capacity.
        head_ = 0;  // Reset head so deque starts at index 0 after resize.
        totalCopies_ += static_cast<long long>(copied);  // Accumulate total copies.
        return copied;  // Return copies performed for this resize.
    }  // End resize().

    int ensureCapacityForOneMore() {  // Ensure capacity for one more push; return resize copies.
        if (size_ < capacity_) {  // Fast path: free space exists.
            return 0;  // No resize needed.
        }  // Close fast path.
        return resize(capacity_ * 2);  // Double capacity and return copies.
    }  // End ensureCapacityForOneMore().
};  // End Deque.

inline PushSummary simulatePushBacks(int m) {  // Simulate m pushBack operations and summarize growth/copy costs.
    if (m < 0) {  // Reject invalid counts.
        throw std::invalid_argument("m must be >= 0");  // Signal invalid input.
    }  // Close validation.
    Deque d;  // Fresh deque for deterministic results.
    long long totalActualCost = 0;  // Accumulate total cost (1 write + copied).
    int maxCopied = 0;  // Track maximum copied elements in a single push.
    for (int i = 0; i < m; i++) {  // Perform m pushes.
        OperationCost cost = d.pushBack(i);  // Push deterministic value and get per-op cost.
        totalActualCost += 1 + static_cast<long long>(cost.copied);  // Add write cost + resize copy cost.
        maxCopied = std::max(maxCopied, cost.copied);  // Update maximum copied.
    }  // Close loop.
    return PushSummary{m, d.size(), d.capacity(), d.totalCopies(), totalActualCost, maxCopied};  // Return summary.
}  // End simulatePushBacks().

inline Deque buildOrderedDeque(int n) {  // Build a deque containing [0, 1, ..., n-1] using pushBack.
    if (n < 0) {  // Reject invalid sizes.
        throw std::invalid_argument("n must be >= 0");  // Signal invalid input.
    }  // Close validation.
    Deque d;  // Create empty deque.
    for (int v = 0; v < n; v++) {  // Push values in increasing order.
        d.pushBack(v);  // Push one value.
    }  // Close loop.
    return d;  // Return built deque.
}  // End buildOrderedDeque().

}  // namespace dequeunit  // Close namespace.

#endif  // DEQUE_HPP  // End of header guard.

