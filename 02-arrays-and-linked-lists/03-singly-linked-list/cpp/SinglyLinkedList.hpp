// 03 單向鏈結串列（C++）/ Singly linked list (C++).  // Bilingual header for this unit.
#ifndef SINGLY_LINKED_LIST_HPP  // Header guard to prevent multiple inclusion.
#define SINGLY_LINKED_LIST_HPP  // Header guard definition.

#include <stdexcept>  // Provide std::out_of_range for index validation.
#include <vector>  // Provide std::vector for toVector conversion in tests/demos.

struct OperationCost {  // Record the traversal cost of one operation.
    int hops;  // Number of `next` pointer traversals performed (current = current->next).
};  // End OperationCost.

struct GetResult {  // Return fetched value plus traversal cost.
    int value;  // The fetched element value.
    OperationCost cost;  // Traversal cost record.
};  // End GetResult.

struct RemoveResult {  // Return removed value plus traversal cost.
    int value;  // The removed element value.
    OperationCost cost;  // Traversal cost record.
};  // End RemoveResult.

struct FindResult {  // Return found index plus traversal cost.
    int index;  // Found index, or -1 if not found.
    OperationCost cost;  // Traversal cost record.
};  // End FindResult.

class SinglyLinkedList {  // A head-only singly linked list (no tail pointer).
public:  // Expose public API for demo/tests.
    SinglyLinkedList() : head_(nullptr), size_(0) {}  // Initialize an empty list.
    ~SinglyLinkedList() { clear(); }  // Free nodes on destruction.

    SinglyLinkedList(const SinglyLinkedList&) = delete;  // Disable copy to avoid shallow-pointer bugs.
    SinglyLinkedList& operator=(const SinglyLinkedList&) = delete;  // Disable copy assignment.

    SinglyLinkedList(SinglyLinkedList&& other) noexcept : head_(other.head_), size_(other.size_) {  // Move-construct by stealing pointers.
        other.head_ = nullptr;  // Leave source list empty.
        other.size_ = 0;  // Leave source size zero.
    }  // End move constructor.

    SinglyLinkedList& operator=(SinglyLinkedList&& other) noexcept {  // Move-assign by clearing then stealing pointers.
        if (this == &other) {  // Guard against self-move.
            return *this;  // No-op for self-assignment.
        }  // Close guard.
        clear();  // Free current nodes before taking ownership.
        head_ = other.head_;  // Steal head pointer.
        size_ = other.size_;  // Steal size.
        other.head_ = nullptr;  // Leave source list empty.
        other.size_ = 0;  // Leave source size zero.
        return *this;  // Return this for chaining.
    }  // End move assignment.

    int size() const {  // Expose current size.
        return size_;  // Return stored size.
    }  // End size().

    bool isEmpty() const {  // Convenience emptiness check.
        return size_ == 0;  // Empty iff size is zero.
    }  // End isEmpty().

    std::vector<int> toVector() const {  // Convert list contents to vector for assertions/printing.
        std::vector<int> out;  // Accumulate values.
        out.reserve(static_cast<std::size_t>(size_));  // Reserve to avoid reallocations in demo/tests.
        Node* current = head_;  // Start at head.
        while (current != nullptr) {  // Traverse until end.
            out.push_back(current->value);  // Append one value.
            current = current->next;  // Move to next node.
        }  // Close traversal loop.
        return out;  // Return collected values.
    }  // End toVector().

    OperationCost pushFront(int value) {  // Insert at head (O(1), hops=0).
        head_ = new Node(value, head_);  // Link new node before current head.
        size_ += 1;  // Update size.
        return OperationCost{0};  // Head insertion does not traverse `next`.
    }  // End pushFront.

    OperationCost pushBack(int value) {  // Insert at tail (O(n) without a tail pointer).
        if (head_ == nullptr) {  // Special-case empty list.
            head_ = new Node(value, nullptr);  // New node becomes head.
            size_ = 1;  // Size becomes 1.
            return OperationCost{0};  // No traversal needed.
        }  // Close empty case.

        int hops = 0;  // Count `next` traversals.
        Node* current = head_;  // Start from head.
        while (current->next != nullptr) {  // Walk until last node.
            current = current->next;  // Traverse one link.
            hops += 1;  // Count one hop.
        }  // Close traversal loop.
        current->next = new Node(value, nullptr);  // Link new node after last.
        size_ += 1;  // Update size.
        return OperationCost{hops};  // Return traversal cost.
    }  // End pushBack.

    GetResult getWithCost(int index) const {  // Get element at index (O(n) traversal).
        requireIndexInRange(index);  // Validate index.
        int hops = 0;  // Count traversals.
        Node* current = head_;  // Start from head.
        for (int i = 0; i < index; i += 1) {  // Move forward index times.
            current = current->next;  // Traverse one link.
            hops += 1;  // Count one hop.
        }  // Close loop.
        return GetResult{current->value, OperationCost{hops}};  // Return value + cost.
    }  // End getWithCost.

    OperationCost insertAt(int index, int value) {  // Insert at index (O(n) except at head).
        requireInsertIndexInRange(index);  // Validate index.
        if (index == 0) {  // Special-case head insertion.
            return pushFront(value);  // Delegate to pushFront.
        }  // Close head case.

        int hops = 0;  // Count traversal hops.
        Node* prev = head_;  // Start from head (need predecessor).
        for (int i = 0; i < index - 1; i += 1) {  // Move to predecessor.
            prev = prev->next;  // Traverse one link.
            hops += 1;  // Count one hop.
        }  // Close loop.
        prev->next = new Node(value, prev->next);  // Splice new node after predecessor.
        size_ += 1;  // Update size.
        return OperationCost{hops};  // Return traversal cost.
    }  // End insertAt.

    RemoveResult removeAt(int index) {  // Remove node at index and return removed value + cost.
        requireIndexInRange(index);  // Validate index.
        if (index == 0) {  // Special-case head removal.
            Node* removed = head_;  // Capture node to remove.
            int removedValue = removed->value;  // Capture removed value.
            head_ = removed->next;  // Bypass old head.
            delete removed;  // Free removed node.
            size_ -= 1;  // Update size.
            return RemoveResult{removedValue, OperationCost{0}};  // No traversal needed.
        }  // Close head case.

        int hops = 0;  // Count traversal hops.
        Node* prev = head_;  // Start from head (need predecessor).
        for (int i = 0; i < index - 1; i += 1) {  // Move to predecessor.
            prev = prev->next;  // Traverse one link.
            hops += 1;  // Count one hop.
        }  // Close loop.
        Node* removed = prev->next;  // Node to remove.
        int removedValue = removed->value;  // Capture removed value.
        prev->next = removed->next;  // Bypass removed node.
        delete removed;  // Free removed node.
        size_ -= 1;  // Update size.
        return RemoveResult{removedValue, OperationCost{hops}};  // Return value + cost.
    }  // End removeAt.

    FindResult indexOfWithCost(int value) const {  // Find first index of value (O(n)).
        Node* current = head_;  // Start from head.
        int index = 0;  // Track current index.
        int hops = 0;  // Count `next` traversals.
        while (current != nullptr) {  // Traverse until end.
            if (current->value == value) {  // Check for match.
                return FindResult{index, OperationCost{hops}};  // Return index + cost.
            }  // Close match branch.
            current = current->next;  // Move to next node.
            hops += 1;  // Count one hop.
            index += 1;  // Advance index.
        }  // Close traversal loop.
        return FindResult{-1, OperationCost{hops}};  // Not found sentinel with cost.
    }  // End indexOfWithCost.

    void clear() {  // Delete all nodes and reset list to empty state.
        Node* current = head_;  // Start at head.
        while (current != nullptr) {  // Traverse until end.
            Node* next = current->next;  // Save next pointer before delete.
            delete current;  // Delete current node.
            current = next;  // Move to next node.
        }  // Close traversal loop.
        head_ = nullptr;  // Reset head.
        size_ = 0;  // Reset size.
    }  // End clear.

private:  // Hide implementation details.
    struct Node {  // Internal node type.
        int value;  // Store node value.
        Node* next;  // Store next pointer.
        Node(int v, Node* n) : value(v), next(n) {}  // Initialize node with value and next pointer.
    };  // End Node.

    Node* head_;  // Head pointer.
    int size_;  // Stored size.

    void requireIndexInRange(int index) const {  // Validate index in [0, size-1].
        if (index < 0 || index >= size_) {  // Reject invalid indices.
            throw std::out_of_range("index out of range");  // Signal invalid index.
        }  // Close validation.
    }  // End requireIndexInRange.

    void requireInsertIndexInRange(int index) const {  // Validate insert index in [0, size].
        if (index < 0 || index > size_) {  // Reject invalid insertion indices.
            throw std::out_of_range("index out of range for insert");  // Signal invalid index.
        }  // Close validation.
    }  // End requireInsertIndexInRange.
};  // End SinglyLinkedList.

inline SinglyLinkedList buildOrderedList(int n) {  // Build a list containing [0, 1, ..., n-1].
    if (n < 0) {  // Reject invalid sizes.
        throw std::invalid_argument("n must be >= 0");  // Signal invalid input.
    }  // Close validation.
    SinglyLinkedList lst;  // Create empty list.
    for (int v = n - 1; v >= 0; v -= 1) {  // Push front in reverse to get ascending order.
        lst.pushFront(v);  // O(1) head insertion.
    }  // Close loop.
    return lst;  // Return built list.
}  // End buildOrderedList.

#endif  // SINGLY_LINKED_LIST_HPP  // End of header guard.
