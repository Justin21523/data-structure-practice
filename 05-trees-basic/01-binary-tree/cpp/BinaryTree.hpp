// 01 二元樹示範（C++）/ Binary tree demo (C++).  // Bilingual header line for this unit.
#ifndef BINARY_TREE_HPP  // Header guard to prevent multiple inclusion.
#define BINARY_TREE_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max for height calculation.
#include <memory>  // Provide std::unique_ptr for owning nodes safely.
#include <optional>  // Provide std::optional for null values in level-order input.
#include <vector>  // Provide std::vector for returning traversal sequences.

namespace binarytreeunit {  // Use a small namespace to avoid polluting the global namespace.

struct TreeSummary {  // Summarize key properties and traversal sequences.
    int size;  // Number of nodes in the tree.
    int height;  // Height measured in edges (empty=-1, leaf=0).
    int leaves;  // Number of leaf nodes.
    std::vector<int> preorder;  // Preorder traversal (root, left, right).
    std::vector<int> inorder;  // Inorder traversal (left, root, right).
    std::vector<int> postorder;  // Postorder traversal (left, right, root).
    std::vector<int> levelOrder;  // Level-order traversal (BFS).
};  // End of TreeSummary.

class BinaryTree {  // A binary tree wrapper that owns all nodes and exposes traversal utilities.
public:
    struct Node {  // Binary tree node (value + left + right).
        int value;  // Node value.
        Node* left;  // Left child pointer (nullptr means no child).
        Node* right;  // Right child pointer (nullptr means no child).

        explicit Node(int v) : value(v), left(nullptr), right(nullptr) {  // Initialize node with value and empty children.
        }  // Close constructor.
    };  // End Node.

    BinaryTree() : root_(nullptr), nodes_() {  // Initialize an empty tree.
    }  // Close constructor.

    static BinaryTree fromLevelOrder(const std::vector<std::optional<int>>& values) {  // Build a tree from level-order array representation with nulls.
        BinaryTree t;  // Create a tree instance that will own nodes.
        if (values.empty()) {  // Empty list builds an empty tree.
            return t;  // Return empty tree.
        }  // Close empty branch.

        std::vector<Node*> refs(values.size(), nullptr);  // Map input indices -> raw node pointers (nullptr for missing).
        t.nodes_.reserve(values.size());  // Reserve to keep pointers stable (avoid vector reallocation).

        for (size_t i = 0; i < values.size(); i++) {  // First pass: create nodes for non-null values.
            if (!values[i].has_value()) {  // Skip null entries.
                continue;  // Move to next index.
            }  // Close skip branch.
            t.nodes_.push_back(std::make_unique<Node>(*values[i]));  // Allocate a node and store ownership in the tree.
            refs[i] = t.nodes_.back().get();  // Store raw pointer for linking.
        }  // Close allocation pass.

        for (size_t i = 0; i < refs.size(); i++) {  // Second pass: link children using array indices.
            Node* node = refs[i];  // Read node pointer at index i.
            if (node == nullptr) {  // Skip missing nodes.
                continue;  // Move to next index.
            }  // Close skip branch.
            size_t leftIndex = 2 * i + 1;  // Compute left child index.
            size_t rightIndex = 2 * i + 2;  // Compute right child index.
            if (leftIndex < refs.size()) {  // Link left child when in range.
                node->left = refs[leftIndex];  // Assign left child (may be nullptr).
            }  // Close left link.
            if (rightIndex < refs.size()) {  // Link right child when in range.
                node->right = refs[rightIndex];  // Assign right child (may be nullptr).
            }  // Close right link.
        }  // Close linking pass.

        t.root_ = refs[0];  // Root is index 0 (may be nullptr).
        return t;  // Return built tree (moves ownership).
    }  // End fromLevelOrder().

    Node* root() const {  // Expose the root pointer for demos/tests (read-only by convention).
        return root_;  // Return root pointer.
    }  // End root().

    bool isEmpty() const {  // Check whether the tree has no nodes.
        return root_ == nullptr;  // Empty iff root is nullptr.
    }  // End isEmpty().

    int size() const {  // Return number of nodes in the tree (O(n)).
        return sizeOf(root_);  // Count nodes starting from root.
    }  // End size().

    int height() const {  // Return height measured in edges (empty=-1, leaf=0).
        return heightOf(root_);  // Compute height from root.
    }  // End height().

    int countLeaves() const {  // Return number of leaf nodes (O(n)).
        return leavesOf(root_);  // Count leaves from root.
    }  // End countLeaves().

    std::vector<int> preorder() const {  // Return preorder traversal as a vector of values.
        std::vector<int> out;  // Accumulate traversal result.
        out.reserve(static_cast<size_t>(size()));  // Reserve for fewer allocations.
        preorderWalk(root_, out);  // Walk from root.
        return out;  // Return traversal list.
    }  // End preorder().

    std::vector<int> inorder() const {  // Return inorder traversal as a vector of values.
        std::vector<int> out;  // Accumulate traversal result.
        out.reserve(static_cast<size_t>(size()));  // Reserve for fewer allocations.
        inorderWalk(root_, out);  // Walk from root.
        return out;  // Return traversal list.
    }  // End inorder().

    std::vector<int> postorder() const {  // Return postorder traversal as a vector of values.
        std::vector<int> out;  // Accumulate traversal result.
        out.reserve(static_cast<size_t>(size()));  // Reserve for fewer allocations.
        postorderWalk(root_, out);  // Walk from root.
        return out;  // Return traversal list.
    }  // End postorder().

    std::vector<int> levelOrder() const {  // Return level-order traversal (BFS) as a vector of values.
        if (root_ == nullptr) {  // Empty tree has empty traversal.
            return {};  // Return empty vector.
        }  // Close empty branch.

        std::vector<const Node*> queue;  // Use vector as queue storage.
        queue.reserve(static_cast<size_t>(size()));  // Reserve to avoid reallocations.
        size_t head = 0;  // Maintain a head index so dequeue is O(1) without shifting.
        std::vector<int> out;  // Accumulate traversal result.
        out.reserve(static_cast<size_t>(size()));  // Reserve output size.

        queue.push_back(root_);  // Enqueue root node.
        while (head < queue.size()) {  // Process queue until head reaches the end.
            const Node* node = queue[head];  // Read next node to process.
            head += 1;  // Advance head (logical dequeue).
            out.push_back(node->value);  // Visit node in BFS order.
            if (node->left != nullptr) {  // Enqueue left child when present.
                queue.push_back(node->left);  // Append to queue tail.
            }  // Close left enqueue.
            if (node->right != nullptr) {  // Enqueue right child when present.
                queue.push_back(node->right);  // Append to queue tail.
            }  // Close right enqueue.
        }  // Close BFS loop.

        return out;  // Return traversal list.
    }  // End levelOrder().

    TreeSummary summarize() const {  // Build a summary snapshot for demos/tests.
        return TreeSummary{  // Construct summary object.
            size(),  // Compute node count.
            height(),  // Compute height (edges).
            countLeaves(),  // Compute leaf count.
            preorder(),  // Compute preorder traversal.
            inorder(),  // Compute inorder traversal.
            postorder(),  // Compute postorder traversal.
            levelOrder(),  // Compute level-order traversal.
        };  // Close summary.
    }  // End summarize().

private:
    Node* root_;  // Root pointer (non-owning; owned by nodes_).
    std::vector<std::unique_ptr<Node>> nodes_;  // Own all nodes so raw pointers stay valid.

    static int sizeOf(const Node* node) {  // Recursive helper that counts nodes in a subtree.
        if (node == nullptr) {  // Empty subtree contributes 0 nodes.
            return 0;  // Return 0 for empty.
        }  // Close base case.
        return 1 + sizeOf(node->left) + sizeOf(node->right);  // Count root + left + right.
    }  // End sizeOf().

    static int heightOf(const Node* node) {  // Recursive helper that computes height in edges.
        if (node == nullptr) {  // Empty subtree has height -1 (so leaf has height 0).
            return -1;  // Return -1 for empty.
        }  // Close base case.
        return 1 + std::max(heightOf(node->left), heightOf(node->right));  // Height is 1 + max(child heights).
    }  // End heightOf().

    static int leavesOf(const Node* node) {  // Recursive helper that counts leaf nodes.
        if (node == nullptr) {  // Empty subtree contributes 0 leaves.
            return 0;  // Return 0 for empty.
        }  // Close base case.
        if (node->left == nullptr && node->right == nullptr) {  // Node with no children is a leaf.
            return 1;  // Count this leaf.
        }  // Close leaf case.
        return leavesOf(node->left) + leavesOf(node->right);  // Sum leaves in both subtrees.
    }  // End leavesOf().

    static void preorderWalk(const Node* node, std::vector<int>& out) {  // Recursive preorder (root, left, right).
        if (node == nullptr) {  // Empty subtree contributes nothing.
            return;  // Stop recursion.
        }  // Close base case.
        out.push_back(node->value);  // Visit root first.
        preorderWalk(node->left, out);  // Visit left subtree.
        preorderWalk(node->right, out);  // Visit right subtree.
    }  // End preorderWalk().

    static void inorderWalk(const Node* node, std::vector<int>& out) {  // Recursive inorder (left, root, right).
        if (node == nullptr) {  // Empty subtree contributes nothing.
            return;  // Stop recursion.
        }  // Close base case.
        inorderWalk(node->left, out);  // Visit left subtree first.
        out.push_back(node->value);  // Visit root.
        inorderWalk(node->right, out);  // Visit right subtree.
    }  // End inorderWalk().

    static void postorderWalk(const Node* node, std::vector<int>& out) {  // Recursive postorder (left, right, root).
        if (node == nullptr) {  // Empty subtree contributes nothing.
            return;  // Stop recursion.
        }  // Close base case.
        postorderWalk(node->left, out);  // Visit left subtree first.
        postorderWalk(node->right, out);  // Visit right subtree next.
        out.push_back(node->value);  // Visit root last.
    }  // End postorderWalk().
};  // End BinaryTree.

}  // namespace binarytreeunit  // Close namespace.

#endif  // BINARY_TREE_HPP  // End of header guard.

