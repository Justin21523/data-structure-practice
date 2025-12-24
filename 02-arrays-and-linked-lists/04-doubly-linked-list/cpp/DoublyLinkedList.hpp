// 04 雙向鏈結串列（C++）/ Doubly linked list (C++).  // Bilingual header for this unit.
#ifndef DOUBLY_LINKED_LIST_HPP  // Header guard to prevent multiple inclusion.
#define DOUBLY_LINKED_LIST_HPP  // Header guard definition.

#include <stdexcept>  // Provide std::out_of_range for index validation.
#include <vector>  // Provide std::vector for toVector conversion in tests/demos.

struct OperationCost {  // Record the traversal cost of one operation.
    int hops;  // Number of pointer traversals performed (next or prev).
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

class DoublyLinkedList {  // A head+tail doubly linked list (teaching-oriented).
public:  // Expose public API for demo/tests.
    DoublyLinkedList() : head_(nullptr), tail_(nullptr), size_(0) {}  // Initialize an empty list.
    ~DoublyLinkedList() { clear(); }  // Free nodes on destruction.

    DoublyLinkedList(const DoublyLinkedList&) = delete;  // Disable copy to avoid shallow-pointer bugs.
    DoublyLinkedList& operator=(const DoublyLinkedList&) = delete;  // Disable copy assignment.

    DoublyLinkedList(DoublyLinkedList&& other) noexcept : head_(other.head_), tail_(other.tail_), size_(other.size_) {  // Move-construct by stealing pointers.
        other.head_ = nullptr;  // Leave source list empty.
        other.tail_ = nullptr;  // Leave source list empty.
        other.size_ = 0;  // Leave source size zero.
    }  // End move constructor.

    DoublyLinkedList& operator=(DoublyLinkedList&& other) noexcept {  // Move-assign by clearing then stealing pointers.
        if (this == &other) {  // Guard against self-move.
            return *this;  // No-op for self-assignment.
        }  // Close guard.
        clear();  // Free current nodes before taking ownership.
        head_ = other.head_;  // Steal head pointer.
        tail_ = other.tail_;  // Steal tail pointer.
        size_ = other.size_;  // Steal size.
        other.head_ = nullptr;  // Leave source list empty.
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

    std::vector<int> toVector() const {  // Convert list contents to vector for assertions/printing.
        std::vector<int> out;  // Accumulate values.
        out.reserve(static_cast<std::size_t>(size_));  // Reserve to avoid reallocations in demo/tests.
        Node* current = head_;  // Start at head.
        while (current != nullptr) {  // Traverse until end.
            out.push_back(current->value);  // Append one value.
            current = current->next;  // Move forward via next.
        }  // Close traversal loop.
        return out;  // Return collected values.
    }  // End toVector().

    OperationCost pushFront(int value) {  // Insert at head (O(1), hops=0).
        Node* node = new Node(value, nullptr, head_);  // New node points forward to old head.
        if (head_ == nullptr) {  // Empty list case.
            head_ = node;  // Head becomes new node.
            tail_ = node;  // Tail becomes new node.
            size_ = 1;  // Size becomes 1.
            return OperationCost{0};  // No traversal.
        }  // Close empty case.
        head_->prev = node;  // Old head points back to new head.
        head_ = node;  // Update head pointer.
        size_ += 1;  // Update size.
        return OperationCost{0};  // No traversal.
    }  // End pushFront.

    OperationCost pushBack(int value) {  // Insert at tail (O(1) with tail pointer).
        Node* node = new Node(value, tail_, nullptr);  // New node points back to old tail.
        if (tail_ == nullptr) {  // Empty list case.
            head_ = node;  // Head becomes new node.
            tail_ = node;  // Tail becomes new node.
            size_ = 1;  // Size becomes 1.
            return OperationCost{0};  // No traversal.
        }  // Close empty case.
        tail_->next = node;  // Old tail points forward to new tail.
        tail_ = node;  // Update tail pointer.
        size_ += 1;  // Update size.
        return OperationCost{0};  // No traversal.
    }  // End pushBack.

    RemoveResult popFront() {  // Remove from head (O(1), hops=0).
        if (head_ == nullptr) {  // Reject popping from empty list.
            throw std::out_of_range("pop from empty list");  // Signal invalid operation.
        }  // Close empty check.
        Node* removed = head_;  // Capture node to remove.
        int removedValue = removed->value;  // Capture removed value.
        Node* newHead = removed->next;  // Candidate new head.
        if (newHead == nullptr) {  // Removing the only node empties the list.
            head_ = nullptr;  // Clear head.
            tail_ = nullptr;  // Clear tail.
            size_ = 0;  // Reset size.
            delete removed;  // Free removed node.
            return RemoveResult{removedValue, OperationCost{0}};  // No traversal.
        }  // Close single-node case.
        newHead->prev = nullptr;  // New head has no previous node.
        head_ = newHead;  // Update head.
        size_ -= 1;  // Update size.
        delete removed;  // Free removed node.
        return RemoveResult{removedValue, OperationCost{0}};  // No traversal.
    }  // End popFront.

    RemoveResult popBack() {  // Remove from tail (O(1), hops=0).
        if (tail_ == nullptr) {  // Reject popping from empty list.
            throw std::out_of_range("pop from empty list");  // Signal invalid operation.
        }  // Close empty check.
        Node* removed = tail_;  // Capture node to remove.
        int removedValue = removed->value;  // Capture removed value.
        Node* newTail = removed->prev;  // Candidate new tail.
        if (newTail == nullptr) {  // Removing the only node empties the list.
            head_ = nullptr;  // Clear head.
            tail_ = nullptr;  // Clear tail.
            size_ = 0;  // Reset size.
            delete removed;  // Free removed node.
            return RemoveResult{removedValue, OperationCost{0}};  // No traversal.
        }  // Close single-node case.
        newTail->next = nullptr;  // New tail has no next node.
        tail_ = newTail;  // Update tail.
        size_ -= 1;  // Update size.
        delete removed;  // Free removed node.
        return RemoveResult{removedValue, OperationCost{0}};  // No traversal.
    }  // End popBack.

    GetResult getWithCost(int index) const {  // Get element at index with traversal cost.
        NodeWithCost nwc = nodeAtWithCost(index);  // Locate node + cost.
        return GetResult{nwc.node->value, nwc.cost};  // Return value + cost.
    }  // End getWithCost.

    OperationCost insertAt(int index, int value) {  // Insert at index and return traversal cost.
        requireInsertIndexInRange(index);  // Validate insert index.
        if (index == 0) {  // Head insertion is O(1).
            return pushFront(value);  // Delegate to pushFront.
        }  // Close head case.
        if (index == size_) {  // Tail insertion is O(1).
            return pushBack(value);  // Delegate to pushBack.
        }  // Close tail case.

        NodeWithCost target = nodeAtWithCost(index);  // Locate target node currently at index.
        Node* before = target.node->prev;  // Predecessor node.
        Node* node = new Node(value, before, target.node);  // New node sits before target.
        before->next = node;  // Predecessor points forward to new node.
        target.node->prev = node;  // Target points back to new node.
        size_ += 1;  // Update size.
        return target.cost;  // Traversal cost is the cost to find the insertion position.
    }  // End insertAt.

    RemoveResult removeAt(int index) {  // Remove at index and return removed value + traversal cost.
        requireIndexInRange(index);  // Validate index.
        if (index == 0) {  // Head removal is O(1).
            return popFront();  // Delegate to popFront.
        }  // Close head case.
        if (index == size_ - 1) {  // Tail removal is O(1).
            return popBack();  // Delegate to popBack.
        }  // Close tail case.

        NodeWithCost target = nodeAtWithCost(index);  // Locate node to remove.
        Node* before = target.node->prev;  // Predecessor.
        Node* after = target.node->next;  // Successor.
        before->next = after;  // Bypass target from the left.
        after->prev = before;  // Bypass target from the right.
        int removedValue = target.node->value;  // Capture removed value.
        delete target.node;  // Free removed node.
        size_ -= 1;  // Update size.
        return RemoveResult{removedValue, target.cost};  // Return removed value + traversal cost.
    }  // End removeAt.

    FindResult indexOfWithCost(int value) const {  // Find first index of value and return traversal cost.
        Node* current = head_;  // Start from head.
        int index = 0;  // Track index.
        int hops = 0;  // Count next traversals.
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
        tail_ = nullptr;  // Reset tail.
        size_ = 0;  // Reset size.
    }  // End clear.

private:  // Hide implementation details.
    struct Node {  // Internal node type.
        int value;  // Store node value.
        Node* prev;  // Store prev pointer.
        Node* next;  // Store next pointer.
        Node(int v, Node* p, Node* n) : value(v), prev(p), next(n) {}  // Initialize node with neighbors.
    };  // End Node.

    struct NodeWithCost {  // Internal helper record for node lookup.
        Node* node;  // Located node pointer.
        OperationCost cost;  // Traversal cost.
    };  // End NodeWithCost.

    Node* head_;  // Head pointer.
    Node* tail_;  // Tail pointer.
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

    NodeWithCost nodeAtWithCost(int index) const {  // Locate node at index and report traversal hops.
        requireIndexInRange(index);  // Validate index.
        if (index < size_ / 2) {  // Traverse from head when index is in the first half.
            int hops = 0;  // Count pointer traversals.
            Node* current = head_;  // Start from head.
            for (int i = 0; i < index; i += 1) {  // Move forward index times.
                current = current->next;  // Traverse one next pointer.
                hops += 1;  // Count one hop.
            }  // Close loop.
            return NodeWithCost{current, OperationCost{hops}};  // Return node + cost.
        }  // Close head traversal branch.

        int steps = size_ - 1 - index;  // How many prev steps needed from tail.
        int hops = 0;  // Count pointer traversals.
        Node* current = tail_;  // Start from tail.
        for (int i = 0; i < steps; i += 1) {  // Move backward `steps` times.
            current = current->prev;  // Traverse one prev pointer.
            hops += 1;  // Count one hop.
        }  // Close loop.
        return NodeWithCost{current, OperationCost{hops}};  // Return node + cost.
    }  // End nodeAtWithCost.
};  // End DoublyLinkedList.

inline DoublyLinkedList buildOrderedList(int n) {  // Build a list containing [0, 1, ..., n-1].
    if (n < 0) {  // Reject invalid sizes.
        throw std::invalid_argument("n must be >= 0");  // Signal invalid input.
    }  // Close validation.
    DoublyLinkedList lst;  // Create empty list.
    for (int v = 0; v < n; v += 1) {  // Append in increasing order.
        lst.pushBack(v);  // pushBack is O(1) with tail pointer.
    }  // Close loop.
    return lst;  // Return built list (move).
}  // End buildOrderedList.

#endif  // DOUBLY_LINKED_LIST_HPP  // End of header guard.

