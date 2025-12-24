// 05 環狀鏈結串列（C++）/ Circular linked list (C++).  // Bilingual header for this unit.
#ifndef CIRCULAR_LINKED_LIST_HPP  // Header guard to prevent multiple inclusion.
#define CIRCULAR_LINKED_LIST_HPP  // Header guard definition.

#include <stdexcept>  // Provide std::out_of_range / std::invalid_argument for validation.
#include <vector>  // Provide std::vector for toVector conversion in tests/demos.

struct OperationCost {  // Record the traversal cost of one operation.
    int hops;  // Number of `next` pointer traversals performed.
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

class CircularLinkedList {  // A singly circular linked list implemented with a tail pointer only.
public:  // Expose public API for demo/tests.
    CircularLinkedList() : tail_(nullptr), size_(0) {}  // Initialize an empty list.
    ~CircularLinkedList() { clear(); }  // Free nodes on destruction.

    CircularLinkedList(const CircularLinkedList&) = delete;  // Disable copy to avoid shallow-pointer bugs.
    CircularLinkedList& operator=(const CircularLinkedList&) = delete;  // Disable copy assignment.

    CircularLinkedList(CircularLinkedList&& other) noexcept : tail_(other.tail_), size_(other.size_) {  // Move-construct by stealing pointers.
        other.tail_ = nullptr;  // Leave source list empty.
        other.size_ = 0;  // Leave source size zero.
    }  // End move constructor.

    CircularLinkedList& operator=(CircularLinkedList&& other) noexcept {  // Move-assign by clearing then stealing pointers.
        if (this == &other) {  // Guard against self-move.
            return *this;  // No-op for self-assignment.
        }  // Close guard.
        clear();  // Free current nodes before taking ownership.
        tail_ = other.tail_;  // Steal tail pointer.
        size_ = other.size_;  // Steal size.
        other.tail_ = nullptr;  // Leave source list empty.
        other.size_ = 0;  // Leave source size zero.
        return *this;  // Return this for chaining.
    }  // End move assignment.

    int size() const {  // Expose current size.
        return size_;  // Return stored size.
    }  // End size().

    bool isEmpty() const {  // Convenience emptiness check.
        return size_ == 0;  // Empty iff size is zero.
    }  // End isEmpty().

    std::vector<int> toVector() const {  // Convert list contents to vector (bounded by size).
        std::vector<int> out;  // Accumulate values.
        out.reserve(static_cast<std::size_t>(size_));  // Reserve exact size for predictable output.
        if (tail_ == nullptr) {  // Empty list returns empty vector.
            return out;  // Return empty.
        }  // Close empty case.
        Node* current = tail_->next;  // Head is tail->next.
        for (int i = 0; i < size_; i += 1) {  // Traverse exactly size nodes.
            out.push_back(current->value);  // Append one value.
            current = current->next;  // Move to next node.
        }  // Close loop.
        return out;  // Return collected values.
    }  // End toVector().

    OperationCost pushBack(int value) {  // Insert at tail (O(1), hops=0).
        if (tail_ == nullptr) {  // Empty list case.
            Node* node = new Node(value, nullptr);  // Create node with temporary next.
            node->next = node;  // Point to itself to form a 1-node circle.
            tail_ = node;  // Tail becomes node.
            size_ = 1;  // Size becomes 1.
            return OperationCost{0};  // No traversal.
        }  // Close empty case.

        Node* head = tail_->next;  // Read current head.
        Node* node = new Node(value, head);  // New node points to head.
        tail_->next = node;  // Old tail points to new node.
        tail_ = node;  // New node becomes tail.
        size_ += 1;  // Update size.
        return OperationCost{0};  // No traversal.
    }  // End pushBack.

    OperationCost pushFront(int value) {  // Insert at head (O(1), hops=0).
        if (tail_ == nullptr) {  // Empty list case.
            return pushBack(value);  // Reuse pushBack logic.
        }  // Close empty case.

        Node* head = tail_->next;  // Read current head.
        Node* node = new Node(value, head);  // New node points to current head.
        tail_->next = node;  // Tail now points to new head.
        size_ += 1;  // Update size.
        return OperationCost{0};  // No traversal.
    }  // End pushFront.

    RemoveResult popFront() {  // Remove head (O(1), hops=0).
        if (tail_ == nullptr) {  // Reject popping from empty list.
            throw std::out_of_range("pop from empty list");  // Signal invalid operation.
        }  // Close empty check.
        Node* head = tail_->next;  // Head is tail->next.
        int removedValue = head->value;  // Capture removed value.
        if (size_ == 1) {  // Removing the only node empties the list.
            delete head;  // Free node.
            tail_ = nullptr;  // Clear tail.
            size_ = 0;  // Reset size.
            return RemoveResult{removedValue, OperationCost{0}};  // No traversal.
        }  // Close single-node case.
        tail_->next = head->next;  // Bypass head by linking tail to new head.
        delete head;  // Free removed node.
        size_ -= 1;  // Update size.
        return RemoveResult{removedValue, OperationCost{0}};  // No traversal.
    }  // End popFront.

    RemoveResult popBack() {  // Remove tail (O(n)), return removed value + hops.
        if (tail_ == nullptr) {  // Reject popping from empty list.
            throw std::out_of_range("pop from empty list");  // Signal invalid operation.
        }  // Close empty check.
        int removedValue = tail_->value;  // Capture removed value.
        if (size_ == 1) {  // Removing the only node empties the list.
            delete tail_;  // Free node.
            tail_ = nullptr;  // Clear tail.
            size_ = 0;  // Reset size.
            return RemoveResult{removedValue, OperationCost{0}};  // No traversal.
        }  // Close single-node case.

        Node* oldTail = tail_;  // Capture old tail pointer.
        Node* head = oldTail->next;  // Head is tail->next.
        Node* current = head;  // Start from head to find predecessor.
        int hops = 0;  // Count next traversals.
        while (current->next != oldTail) {  // Stop when current is predecessor of tail.
            current = current->next;  // Traverse one next pointer.
            hops += 1;  // Count one hop.
        }  // Close loop.
        current->next = oldTail->next;  // Predecessor now points to head.
        tail_ = current;  // Predecessor becomes new tail.
        delete oldTail;  // Free removed node.
        size_ -= 1;  // Update size.
        return RemoveResult{removedValue, OperationCost{hops}};  // Return removed value + traversal cost.
    }  // End popBack.

    OperationCost rotate(int steps) {  // Rotate by moving tail forward `steps` times (left rotation).
        if (steps < 0) {  // Reject negative rotations for simplicity.
            throw std::invalid_argument("steps must be >= 0");  // Signal invalid input.
        }  // Close validation.
        if (tail_ == nullptr || size_ <= 1) {  // Rotating empty/1-node list changes nothing.
            return OperationCost{0};  // No traversal.
        }  // Close trivial case.
        int stepsMod = steps % size_;  // Reduce steps to avoid unnecessary cycles.
        int hops = 0;  // Count traversals.
        for (int i = 0; i < stepsMod; i += 1) {  // Advance tail stepsMod times.
            tail_ = tail_->next;  // Move tail forward one node.
            hops += 1;  // Count one hop.
        }  // Close loop.
        return OperationCost{hops};  // Return traversal cost.
    }  // End rotate.

    GetResult getWithCost(int index) const {  // Get element at index (O(index) traversal).
        requireIndexInRange(index);  // Validate index.
        Node* current = tail_->next;  // Start from head.
        int hops = 0;  // Count next traversals.
        for (int i = 0; i < index; i += 1) {  // Move forward index times.
            current = current->next;  // Traverse one next pointer.
            hops += 1;  // Count one hop.
        }  // Close loop.
        return GetResult{current->value, OperationCost{hops}};  // Return value + cost.
    }  // End getWithCost.

    FindResult indexOfWithCost(int value) const {  // Find first index of value and return traversal cost.
        if (tail_ == nullptr) {  // Empty list cannot contain the value.
            return FindResult{-1, OperationCost{0}};  // Return not found with zero cost.
        }  // Close empty case.
        Node* current = tail_->next;  // Start at head.
        int hops = 0;  // Count next traversals.
        for (int index = 0; index < size_; index += 1) {  // Traverse exactly size nodes.
            if (current->value == value) {  // Check for match.
                return FindResult{index, OperationCost{hops}};  // Return index + cost.
            }  // Close match branch.
            current = current->next;  // Move to next node.
            hops += 1;  // Count one hop.
        }  // Close loop.
        return FindResult{-1, OperationCost{hops}};  // Not found: hops equals size.
    }  // End indexOfWithCost.

    void clear() {  // Delete all nodes and reset list to empty state.
        if (tail_ == nullptr) {  // Empty list has nothing to free.
            size_ = 0;  // Ensure size is consistent.
            return;  // Done.
        }  // Close empty case.
        Node* head = tail_->next;  // Head is tail->next.
        Node* current = head;  // Start from head.
        for (int i = 0; i < size_; i += 1) {  // Delete exactly size nodes.
            Node* next = current->next;  // Save next pointer before delete.
            delete current;  // Delete current node.
            current = next;  // Move to next node.
        }  // Close loop.
        tail_ = nullptr;  // Reset tail.
        size_ = 0;  // Reset size.
    }  // End clear.

private:  // Hide implementation details.
    struct Node {  // Internal node type.
        int value;  // Store node value.
        Node* next;  // Store next pointer.
        Node(int v, Node* n) : value(v), next(n) {}  // Initialize node with value and next pointer.
    };  // End Node.

    Node* tail_;  // Tail pointer.
    int size_;  // Stored size.

    void requireIndexInRange(int index) const {  // Validate index in [0, size-1].
        if (index < 0 || index >= size_) {  // Reject invalid indices.
            throw std::out_of_range("index out of range");  // Signal invalid index.
        }  // Close validation.
    }  // End requireIndexInRange.
};  // End CircularLinkedList.

inline CircularLinkedList buildOrderedList(int n) {  // Build a list containing [0, 1, ..., n-1].
    if (n < 0) {  // Reject invalid sizes.
        throw std::invalid_argument("n must be >= 0");  // Signal invalid input.
    }  // Close validation.
    CircularLinkedList lst;  // Create empty list.
    for (int v = 0; v < n; v += 1) {  // Append in increasing order.
        lst.pushBack(v);  // pushBack is O(1) with tail pointer.
    }  // Close loop.
    return lst;  // Return built list (move).
}  // End buildOrderedList.

#endif  // CIRCULAR_LINKED_LIST_HPP  // End of header guard.

