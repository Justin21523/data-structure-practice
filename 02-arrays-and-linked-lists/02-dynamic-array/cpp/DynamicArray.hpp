// 02 動態陣列示範（C++）/ Dynamic array demo (C++).  // Bilingual header line for this unit.
#ifndef DYNAMIC_ARRAY_HPP  // Header guard to prevent multiple inclusion.
#define DYNAMIC_ARRAY_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max for max-cost tracking.
#include <memory>  // Provide std::unique_ptr for owning the backing buffer safely.
#include <stdexcept>  // Provide exceptions for validation.
#include <vector>  // Provide std::vector for returning copies of used elements.

namespace dynamicarray {  // Use a small namespace to avoid polluting the global namespace.

struct OperationCost {  // Record per-operation copy/shift counts.
    int copied;  // How many elements were copied due to resize (0 if no resize).
    int moved;  // How many elements were shifted due to insert/remove (0 for append).
};  // End of OperationCost.

struct RemoveResult {  // Return removed value plus its operation cost.
    int value;  // The removed element value.
    OperationCost cost;  // Copy/shift cost for this removal.
};  // End of RemoveResult.

struct AppendSummary {  // Summarize a sequence of appends (growth behavior).
    int m;  // Number of appends performed.
    int finalSize;  // Final size after appends.
    int finalCapacity;  // Final capacity after growth.
    long long totalCopies;  // Total copied elements due to resizes.
    long long totalActualCost;  // Total cost if each append costs 1 write + copies.
    int maxCopiedInOneOp;  // Maximum copies in any single append.
};  // End of AppendSummary.

class DynamicArray {  // A simple dynamic array with doubling growth (teaching-oriented).
public:
    DynamicArray()  // Initialize empty array with capacity 1.
        : size_(0),  // Start with no stored elements.
          capacity_(1),  // Start with capacity 1 for deterministic doubling.
          data_(std::make_unique<int[]>(static_cast<size_t>(capacity_))),  // Allocate backing buffer.
          totalCopies_(0) {  // Start with zero total copies.
    }  // Close constructor.

    int size() const {  // Expose size for callers/tests.
        return size_;  // Return current size.
    }  // End size().

    int capacity() const {  // Expose capacity for callers/tests.
        return capacity_;  // Return current capacity.
    }  // End capacity().

    long long totalCopies() const {  // Expose total copies due to resizes.
        return totalCopies_;  // Return total copied elements.
    }  // End totalCopies().

    int get(int index) const {  // Return element at index (O(1)).
        requireIndexInRange(index);  // Validate index.
        return data_[static_cast<size_t>(index)];  // Read from backing buffer.
    }  // End get().

    void set(int index, int value) {  // Set element at index (O(1)).
        requireIndexInRange(index);  // Validate index.
        data_[static_cast<size_t>(index)] = value;  // Write to backing buffer.
    }  // End set().

    std::vector<int> toVector() const {  // Return a copy of the used portion as a vector.
        std::vector<int> result;  // Output container.
        result.reserve(static_cast<size_t>(size_));  // Reserve to avoid reallocations.
        for (int i = 0; i < size_; i++) {  // Copy used elements.
            result.push_back(data_[static_cast<size_t>(i)]);  // Append one element.
        }  // Close loop.
        return result;  // Return copy.
    }  // End toVector().

    int indexOf(int value) const {  // Return first index of value or -1 (O(n)).
        for (int i = 0; i < size_; i++) {  // Linear scan.
            if (data_[static_cast<size_t>(i)] == value) {  // Compare with target.
                return i;  // Return first match.
            }  // Close match check.
        }  // Close loop.
        return -1;  // Not found sentinel.
    }  // End indexOf().

    OperationCost append(int value) {  // Append at end and return resize cost (moved=0).
        int copied = ensureCapacityForOneMore();  // Resize if needed.
        data_[static_cast<size_t>(size_)] = value;  // Write new element.
        size_ += 1;  // Increase size.
        return OperationCost{copied, 0};  // Return cost record.
    }  // End append().

    OperationCost insertAt(int index, int value) {  // Insert at index and return cost (copies + shifts).
        requireInsertIndexInRange(index);  // Validate insertion index.
        int copied = ensureCapacityForOneMore();  // Resize if needed before shifting.
        int moved = size_ - index;  // Shifting right moves (size - index) elements.
        for (int i = size_; i > index; i--) {  // Shift right from tail toward index.
            data_[static_cast<size_t>(i)] = data_[static_cast<size_t>(i - 1)];  // Move one element right.
        }  // Close loop.
        data_[static_cast<size_t>(index)] = value;  // Write new value.
        size_ += 1;  // Increase size.
        return OperationCost{copied, moved};  // Return cost record.
    }  // End insertAt().

    RemoveResult removeAt(int index) {  // Remove at index and return removed value + shift cost.
        requireIndexInRange(index);  // Validate index.
        int removedValue = data_[static_cast<size_t>(index)];  // Capture removed value.
        int moved = size_ - index - 1;  // Shifting left moves (size - index - 1) elements.
        for (int i = index; i < size_ - 1; i++) {  // Shift left to fill gap.
            data_[static_cast<size_t>(i)] = data_[static_cast<size_t>(i + 1)];  // Move one element left.
        }  // Close loop.
        size_ -= 1;  // Decrease size.
        return RemoveResult{removedValue, OperationCost{0, moved}};  // Return removed value and cost.
    }  // End removeAt().

private:
    int size_;  // Number of stored elements.
    int capacity_;  // Allocated slots.
    std::unique_ptr<int[]> data_;  // Backing buffer.
    long long totalCopies_;  // Total copies due to resizes.

    void requireIndexInRange(int index) const {  // Validate index in [0, size-1].
        if (index < 0 || index >= size_) {  // Reject invalid indices.
            throw std::out_of_range("index out of range");  // Signal invalid index.
        }  // Close validation.
    }  // End requireIndexInRange().

    void requireInsertIndexInRange(int index) const {  // Validate insert index in [0, size].
        if (index < 0 || index > size_) {  // Reject invalid insertion indices.
            throw std::out_of_range("index out of range for insert");  // Signal invalid index.
        }  // Close validation.
    }  // End requireInsertIndexInRange().

    int resize(int newCapacity) {  // Resize buffer and return number of copied elements.
        if (newCapacity < size_) {  // Ensure new buffer can hold all elements.
            throw std::invalid_argument("newCapacity must be >= size");  // Signal invalid request.
        }  // Close validation.
        if (newCapacity < 1) {  // Keep capacity positive.
            throw std::invalid_argument("newCapacity must be >= 1");  // Signal invalid request.
        }  // Close validation.

        std::unique_ptr<int[]> newData = std::make_unique<int[]>(static_cast<size_t>(newCapacity));  // Allocate new buffer.
        int copied = 0;  // Count copied elements.
        for (int i = 0; i < size_; i++) {  // Copy used portion.
            newData[static_cast<size_t>(i)] = data_[static_cast<size_t>(i)];  // Copy one element.
            copied += 1;  // Count one copy.
        }  // Close copy loop.
        data_ = std::move(newData);  // Swap buffer.
        capacity_ = newCapacity;  // Update capacity.
        totalCopies_ += static_cast<long long>(copied);  // Accumulate total copies.
        return copied;  // Return copies performed.
    }  // End resize().

    int ensureCapacityForOneMore() {  // Ensure capacity for one more element; return resize copies.
        if (size_ < capacity_) {  // Fast path: free space exists.
            return 0;  // No resize needed.
        }  // Close fast path.
        return resize(capacity_ * 2);  // Double capacity and return copies.
    }  // End ensureCapacityForOneMore().
};  // End DynamicArray.

inline bool isPowerOfTwo(int x) {  // Helper for verifying doubling-capacity behavior.
    return x > 0 && ((x & (x - 1)) == 0);  // Standard power-of-two check.
}  // End isPowerOfTwo.

inline AppendSummary simulateAppends(int m) {  // Simulate m appends and summarize growth/copy costs.
    if (m < 0) {  // Reject invalid counts.
        throw std::invalid_argument("m must be >= 0");  // Signal invalid input.
    }  // Close validation.
    DynamicArray a;  // Fresh array for deterministic results.
    long long totalActualCost = 0;  // Accumulate total cost (1 write + copied).
    int maxCopied = 0;  // Track max copied in one append.
    for (int i = 0; i < m; i++) {  // Perform m appends.
        OperationCost cost = a.append(i);  // Append deterministic value.
        totalActualCost += 1 + static_cast<long long>(cost.copied);  // Add write + copy cost.
        maxCopied = std::max(maxCopied, cost.copied);  // Update max copied.
    }  // Close loop.
    return AppendSummary{m, a.size(), a.capacity(), a.totalCopies(), totalActualCost, maxCopied};  // Return summary.
}  // End simulateAppends.

inline DynamicArray buildFilledArray(int n) {  // Build dynamic array filled with 0..n-1.
    if (n < 0) {  // Reject invalid sizes.
        throw std::invalid_argument("n must be >= 0");  // Signal invalid input.
    }  // Close validation.
    DynamicArray a;  // Start empty.
    for (int i = 0; i < n; i++) {  // Fill with deterministic values.
        a.append(i);  // Append one value.
    }  // Close loop.
    return a;  // Return filled array.
}  // End buildFilledArray.

inline OperationCost simulateAppendCostAtSize(int n) {  // Build size n then append once and return cost.
    DynamicArray a = buildFilledArray(n);  // Build array.
    return a.append(999);  // Append sentinel.
}  // End simulateAppendCostAtSize.

inline OperationCost simulateInsert0CostAtSize(int n) {  // Build size n then insert at head and return cost.
    DynamicArray a = buildFilledArray(n);  // Build array.
    return a.insertAt(0, 999);  // Insert sentinel at head.
}  // End simulateInsert0CostAtSize.

}  // namespace dynamicarray  // Close namespace.

#endif  // DYNAMIC_ARRAY_HPP  // End of header guard.

