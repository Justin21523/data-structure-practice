// 01 靜態陣列示範（C++）/ Static array demo (C++).  // Bilingual header line for this unit.
#ifndef FIXED_ARRAY_HPP  // Header guard to prevent multiple inclusion.
#define FIXED_ARRAY_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max for bounds and max-cost tracking.
#include <memory>  // Provide std::unique_ptr for owning the backing buffer safely.
#include <stdexcept>  // Provide std::invalid_argument/std::out_of_range for validation.
#include <vector>  // Provide std::vector for returning copies of used elements.

namespace fixedarray {  // Use a small namespace to avoid polluting the global namespace.

struct RemoveResult {  // Return both removed value and moved elements count.
    int value;  // The removed element value.
    int moved;  // How many elements were shifted left after removal.
};  // End of RemoveResult.

class FixedArray {  // A fixed-capacity array with manual shifting (teaching-oriented).
public:
    explicit FixedArray(int capacity)  // Create an empty fixed array with a given capacity.
        : capacity_(capacity),  // Store capacity for overflow checks.
          size_(0),  // Start empty.
          data_(capacity_ > 0 ? std::make_unique<int[]>(static_cast<size_t>(capacity_)) : nullptr) {  // Allocate backing buffer if needed.
        if (capacity < 0) {  // Reject negative capacities.
            throw std::invalid_argument("capacity must be >= 0");  // Signal invalid input.
        }  // Close validation block.
    }  // End constructor.

    int capacity() const {  // Expose capacity for callers/tests.
        return capacity_;  // Return fixed capacity.
    }  // End capacity().

    int size() const {  // Expose size for callers/tests.
        return size_;  // Return current size.
    }  // End size().

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
        result.reserve(static_cast<size_t>(size_));  // Reserve to avoid re-allocations.
        for (int i = 0; i < size_; i++) {  // Copy used elements.
            result.push_back(data_[static_cast<size_t>(i)]);  // Append one element.
        }  // Close loop.
        return result;  // Return copy.
    }  // End toVector().

    int indexOf(int value) const {  // Return first index of value or -1 (O(n)).
        for (int i = 0; i < size_; i++) {  // Linear scan.
            if (data_[static_cast<size_t>(i)] == value) {  // Compare element with target.
                return i;  // Return first match.
            }  // Close match check.
        }  // Close loop.
        return -1;  // Return not found sentinel.
    }  // End indexOf().

    int append(int value) {  // Append at end and return moved count (always 0 when not full).
        return insertAt(size_, value);  // Delegate to insertAt to keep logic consistent.
    }  // End append().

    int insertAt(int index, int value) {  // Insert value at index and return moved elements count.
        requireNotFull();  // Ensure free space exists.
        requireInsertIndexInRange(index);  // Validate insertion index.

        int moved = size_ - index;  // Shifting right moves exactly (size - index) elements.
        for (int i = size_; i > index; i--) {  // Shift right from tail toward index.
            data_[static_cast<size_t>(i)] = data_[static_cast<size_t>(i - 1)];  // Move one element right.
        }  // Close shift loop.
        data_[static_cast<size_t>(index)] = value;  // Write new value.
        size_ += 1;  // Increase size after insertion.
        return moved;  // Return moved count.
    }  // End insertAt().

    RemoveResult removeAt(int index) {  // Remove element at index and return removed value + moved count.
        requireIndexInRange(index);  // Validate index.

        int removedValue = data_[static_cast<size_t>(index)];  // Capture removed value.
        int moved = size_ - index - 1;  // Shifting left moves exactly (size - index - 1) elements.
        for (int i = index; i < size_ - 1; i++) {  // Shift left to fill gap.
            data_[static_cast<size_t>(i)] = data_[static_cast<size_t>(i + 1)];  // Move one element left.
        }  // Close shift loop.
        size_ -= 1;  // Decrease size after removal.
        return RemoveResult{removedValue, moved};  // Return both removed value and move count.
    }  // End removeAt().

private:
    int capacity_;  // Fixed capacity.
    int size_;  // Current size.
    std::unique_ptr<int[]> data_;  // Backing buffer.

    void requireIndexInRange(int index) const {  // Validate index in [0, size-1].
        if (index < 0 || index >= size_) {  // Reject invalid indices.
            throw std::out_of_range("index out of range");  // Signal invalid index.
        }  // Close validation.
    }  // End requireIndexInRange().

    void requireInsertIndexInRange(int index) const {  // Validate index in [0, size] for insertion.
        if (index < 0 || index > size_) {  // Reject invalid insertion indices.
            throw std::out_of_range("index out of range for insert");  // Signal invalid index.
        }  // Close validation.
    }  // End requireInsertIndexInRange().

    void requireNotFull() const {  // Ensure there is free capacity.
        if (size_ >= capacity_) {  // Detect overflow.
            throw std::runtime_error("fixed array is full");  // Signal overflow.
        }  // Close overflow check.
    }  // End requireNotFull().
};  // End of FixedArray.

inline FixedArray buildFilledArray(int n, int extraCapacity) {  // Build array filled with 0..n-1.
    if (n < 0 || extraCapacity < 0) {  // Validate parameters.
        throw std::invalid_argument("n and extraCapacity must be >= 0");  // Signal invalid input.
    }  // Close validation.
    FixedArray a(n + extraCapacity);  // Allocate enough capacity to allow insertion if needed.
    for (int i = 0; i < n; i++) {  // Fill with deterministic values.
        a.append(i);  // Append one value.
    }  // Close loop.
    return a;  // Return filled array.
}  // End buildFilledArray.

inline int simulateInsertMoves(int n, int index) {  // Simulate insert and return moved count.
    FixedArray a = buildFilledArray(n, 1);  // Build size n with one extra slot.
    return a.insertAt(index, 999);  // Insert sentinel and return moved count.
}  // End simulateInsertMoves.

inline int simulateRemoveMoves(int n, int index) {  // Simulate remove and return moved count.
    FixedArray a = buildFilledArray(n, 0);  // Build size n exactly.
    return a.removeAt(index).moved;  // Remove and return moved count.
}  // End simulateRemoveMoves.

inline bool isPowerOfTwo(int x) {  // Utility predicate (not required here but sometimes useful in later units).
    return x > 0 && ((x & (x - 1)) == 0);  // Standard power-of-two check.
}  // End isPowerOfTwo.

}  // namespace fixedarray  // Close namespace.

#endif  // FIXED_ARRAY_HPP  // End of header guard.

