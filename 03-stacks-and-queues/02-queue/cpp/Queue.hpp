// 02 佇列示範（C++）/ Queue demo (C++).  // Bilingual header line for this unit.
#ifndef QUEUE_HPP  // Header guard to prevent multiple inclusion.
#define QUEUE_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max for max-cost tracking.
#include <memory>  // Provide std::unique_ptr for owning the backing buffer safely.
#include <stdexcept>  // Provide exceptions for validation.
#include <vector>  // Provide std::vector for returning copies of used elements.

namespace queueunit {  // Use a small namespace to avoid polluting the global namespace.

struct OperationCost {  // Record per-operation copy/shift counts.
    int copied;  // How many elements were copied due to resize (0 if no resize).
    int moved;  // How many elements were shifted left due to dequeue (0 for enqueue).
};  // End of OperationCost.

struct DequeueResult {  // Return dequeued value plus its operation cost.
    int value;  // The dequeued element value.
    OperationCost cost;  // Copy/shift cost for this dequeue.
};  // End of DequeueResult.

struct EnqueueSummary {  // Summarize a sequence of enqueues (growth behavior).
    int m;  // Number of enqueues performed.
    int finalSize;  // Final size after enqueues.
    int finalCapacity;  // Final capacity after growth.
    long long totalCopies;  // Total copied elements due to resizes.
    long long totalActualCost;  // Total cost if each enqueue costs 1 write + copied.
    int maxCopiedInOneOp;  // Maximum copies in any single enqueue.
};  // End of EnqueueSummary.

class ArrayQueue {  // A naive array-backed queue (dequeue shifts left; teaching-oriented).
public:
    ArrayQueue()  // Initialize an empty queue with capacity 1.
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

    std::vector<int> toVector() const {  // Return a copy of the used portion (front -> rear).
        std::vector<int> out;  // Output container.
        out.reserve(static_cast<size_t>(size_));  // Reserve to avoid reallocations.
        for (int i = 0; i < size_; i++) {  // Copy used portion.
            out.push_back(data_[static_cast<size_t>(i)]);  // Append one element.
        }  // Close loop.
        return out;  // Return copy.
    }  // End toVector().

    OperationCost enqueue(int value) {  // Add to rear (amortized O(1)).
        int copied = ensureCapacityForOneMore();  // Resize if needed.
        data_[static_cast<size_t>(size_)] = value;  // Write new value at rear slot.
        size_ += 1;  // Increase size.
        return OperationCost{copied, 0};  // Enqueue does not shift left.
    }  // End enqueue().

    int peek() const {  // Return front value without removing it (O(1)).
        if (size_ == 0) {  // Reject peeking an empty queue.
            throw std::out_of_range("peek from empty queue");  // Signal invalid operation.
        }  // Close validation.
        return data_[0];  // Return front slot.
    }  // End peek().

    DequeueResult dequeue() {  // Remove and return front value (O(n) due to shifting).
        if (size_ == 0) {  // Reject dequeuing an empty queue.
            throw std::out_of_range("dequeue from empty queue");  // Signal invalid operation.
        }  // Close validation.
        int removed = data_[0];  // Capture front value.
        int moved = size_ - 1;  // Shifting left moves (size - 1) elements.
        for (int i = 1; i < size_; i++) {  // Shift elements left by one.
            data_[static_cast<size_t>(i - 1)] = data_[static_cast<size_t>(i)];  // Move one element left.
        }  // Close shift loop.
        size_ -= 1;  // Decrease size after shifting.
        return DequeueResult{removed, OperationCost{0, moved}};  // Return value + shift cost.
    }  // End dequeue().

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

    int ensureCapacityForOneMore() {  // Ensure capacity for one more enqueue; return resize copies.
        if (size_ < capacity_) {  // Fast path: free space exists.
            return 0;  // No resize needed.
        }  // Close fast path.
        return resize(capacity_ * 2);  // Double capacity and return copies.
    }  // End ensureCapacityForOneMore().
};  // End ArrayQueue.

inline EnqueueSummary simulateEnqueues(int m) {  // Simulate m enqueues and summarize growth/copy costs.
    if (m < 0) {  // Reject invalid counts.
        throw std::invalid_argument("m must be >= 0");  // Signal invalid input.
    }  // Close validation.
    ArrayQueue q;  // Fresh queue for deterministic results.
    long long totalActualCost = 0;  // Accumulate total cost (1 write + copied).
    int maxCopied = 0;  // Track maximum copied elements in a single enqueue.
    for (int i = 0; i < m; i++) {  // Perform m enqueues.
        OperationCost cost = q.enqueue(i);  // Enqueue deterministic value and get per-op cost.
        totalActualCost += 1 + static_cast<long long>(cost.copied);  // Add write cost + resize copy cost.
        maxCopied = std::max(maxCopied, cost.copied);  // Update maximum copied.
    }  // Close loop.
    return EnqueueSummary{m, q.size(), q.capacity(), q.totalCopies(), totalActualCost, maxCopied};  // Return summary.
}  // End simulateEnqueues().

inline ArrayQueue buildOrderedQueue(int n) {  // Build a queue containing [0, 1, ..., n-1].
    if (n < 0) {  // Reject invalid sizes.
        throw std::invalid_argument("n must be >= 0");  // Signal invalid input.
    }  // Close validation.
    ArrayQueue q;  // Create empty queue.
    for (int v = 0; v < n; v++) {  // Enqueue in increasing order.
        q.enqueue(v);  // Enqueue one value.
    }  // Close loop.
    return q;  // Return built queue.
}  // End buildOrderedQueue().

inline OperationCost simulateDequeueCostAtSize(int n) {  // Build size n queue then dequeue once and return cost.
    ArrayQueue q = buildOrderedQueue(n);  // Build queue of size n.
    return q.dequeue().cost;  // Dequeue once and return cost.
}  // End simulateDequeueCostAtSize().

}  // namespace queueunit  // Close namespace.

#endif  // QUEUE_HPP  // End of header guard.

