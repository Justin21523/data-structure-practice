// 02 二元搜尋樹示範（C++）/ Binary Search Tree (BST) demo (C++).  // Bilingual header line for this unit.
#ifndef BINARY_SEARCH_TREE_HPP  // Header guard to prevent multiple inclusion.
#define BINARY_SEARCH_TREE_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max for height calculation.
#include <memory>  // Provide std::unique_ptr for owning nodes safely.
#include <optional>  // Provide std::optional for min/max on empty trees.
#include <vector>  // Provide std::vector for inorder traversal output.

namespace bstunit {  // Use a small namespace to avoid polluting the global namespace.

struct TreeSummary {  // Summarize key properties and inorder traversal for printing/tests.
    int size;  // Number of nodes in the BST.
    int height;  // Height measured in edges (empty=-1, leaf=0).
    std::optional<int> minValue;  // Minimum key (nullopt if empty).
    std::optional<int> maxValue;  // Maximum key (nullopt if empty).
    std::vector<int> inorder;  // Inorder traversal (sorted keys for a valid BST).
};  // End of TreeSummary.

class BinarySearchTree {  // A BST wrapper that owns nodes and exposes common operations.
public:
    struct Node {  // BST node (key + left child + right child).
        int key;  // Node key (mutable so delete can replace with successor key).
        std::unique_ptr<Node> left;  // Own left subtree.
        std::unique_ptr<Node> right;  // Own right subtree.

        explicit Node(int k) : key(k), left(nullptr), right(nullptr) {  // Initialize node with key and empty children.
        }  // Close constructor.
    };  // End Node.

    BinarySearchTree() : root_(nullptr) {  // Initialize an empty BST.
    }  // Close constructor.

    static BinarySearchTree fromValues(const std::vector<int>& values) {  // Build a BST by inserting values in order.
        BinarySearchTree t;  // Start from an empty tree.
        for (int v : values) {  // Insert each value in the given order.
            t.insert(v);  // Insert one key (duplicates are ignored by policy).
        }  // Close loop.
        return t;  // Return built BST.
    }  // End fromValues().

    bool isEmpty() const {  // Check whether the BST has no nodes.
        return root_ == nullptr;  // Empty iff root is nullptr.
    }  // End isEmpty().

    bool contains(int key) const {  // Return true if key exists in the BST.
        const Node* node = root_.get();  // Start searching from the root.
        while (node != nullptr) {  // Walk down until we hit a missing child.
            if (key < node->key) {  // Go left when key is smaller.
                node = node->left.get();  // Move to left child.
            } else if (key > node->key) {  // Go right when key is larger.
                node = node->right.get();  // Move to right child.
            } else {  // key == node->key means we found it.
                return true;  // Report found.
            }  // Close compare branch.
        }  // Close loop.
        return false;  // Report not found when search falls off the tree.
    }  // End contains().

    bool insert(int key) {  // Insert key into BST; return false when key already exists.
        std::unique_ptr<Node>* link = &root_;  // Track the pointer-to-child we will update.
        while (link->get() != nullptr) {  // Walk down until we find an empty spot.
            Node* node = link->get();  // Read current node pointer.
            if (key < node->key) {  // Go left when key is smaller.
                link = &node->left;  // Move link to left child slot.
            } else if (key > node->key) {  // Go right when key is larger.
                link = &node->right;  // Move link to right child slot.
            } else {  // key == node->key means duplicate key.
                return false;  // Reject duplicates by policy.
            }  // Close compare branch.
        }  // Close loop.
        *link = std::make_unique<Node>(key);  // Attach new leaf node into the empty slot.
        return true;  // Report inserted.
    }  // End insert().

    bool remove(int key) {  // Remove key from BST; return true when a node was removed.
        return removeSubtree(root_, key);  // Delete from root (root_ may change when deleting root).
    }  // End remove().

    std::optional<int> min() const {  // Return minimum key, or nullopt if tree is empty.
        if (root_ == nullptr) {  // Empty tree has no minimum.
            return std::nullopt;  // Return nullopt for empty.
        }  // Close empty case.
        return minKey(root_.get());  // Return leftmost key.
    }  // End min().

    std::optional<int> max() const {  // Return maximum key, or nullopt if tree is empty.
        if (root_ == nullptr) {  // Empty tree has no maximum.
            return std::nullopt;  // Return nullopt for empty.
        }  // Close empty case.
        const Node* cur = root_.get();  // Start at root.
        while (cur->right != nullptr) {  // Keep going right until we cannot.
            cur = cur->right.get();  // Move to right child.
        }  // Close loop.
        return cur->key;  // Rightmost node has the maximum key.
    }  // End max().

    int size() const {  // Return number of nodes in the BST (O(n)).
        return sizeOf(root_.get());  // Count nodes starting from root.
    }  // End size().

    int height() const {  // Return height measured in edges (empty=-1, leaf=0).
        return heightOf(root_.get());  // Compute height from root.
    }  // End height().

    std::vector<int> inorder() const {  // Return inorder traversal (sorted for a valid BST).
        std::vector<int> out;  // Accumulate traversal result.
        out.reserve(static_cast<size_t>(size()));  // Reserve to avoid reallocations.
        inorderWalk(root_.get(), out);  // Walk from root.
        return out;  // Return traversal list.
    }  // End inorder().

    TreeSummary summarize() const {  // Build a summary snapshot for demos/tests.
        return TreeSummary{  // Construct summary object.
            size(),  // Compute node count.
            height(),  // Compute height (edges).
            min(),  // Compute minimum key (optional).
            max(),  // Compute maximum key (optional).
            inorder(),  // Compute inorder traversal.
        };  // Close summary.
    }  // End summarize().

private:
    std::unique_ptr<Node> root_;  // Own the root subtree.

    static int minKey(const Node* node) {  // Return the minimum key in a non-empty subtree.
        const Node* cur = node;  // Start at given node.
        while (cur->left != nullptr) {  // Keep going left until we cannot.
            cur = cur->left.get();  // Move to left child.
        }  // Close loop.
        return cur->key;  // Leftmost node has the minimum key.
    }  // End minKey().

    static bool removeSubtree(std::unique_ptr<Node>& node, int key) {  // Recursive delete that mutates a unique_ptr link.
        if (node == nullptr) {  // Key not found in an empty subtree.
            return false;  // Nothing removed.
        }  // Close base case.

        if (key < node->key) {  // Delete from left subtree when key is smaller.
            return removeSubtree(node->left, key);  // Recurse into left child.
        }  // Close left branch.
        if (key > node->key) {  // Delete from right subtree when key is larger.
            return removeSubtree(node->right, key);  // Recurse into right child.
        }  // Close right branch.

        // Now key == node->key, so we found the node to delete.  // Explain match branch.
        if (node->left == nullptr && node->right == nullptr) {  // Case 1: leaf node (0 children).
            node.reset();  // Remove leaf by resetting unique_ptr.
            return true;  // Report successful deletion.
        }  // Close leaf case.
        if (node->left == nullptr) {  // Case 2a: only right child exists.
            node = std::move(node->right);  // Replace node by its right child.
            return true;  // Report successful deletion.
        }  // Close right-only case.
        if (node->right == nullptr) {  // Case 2b: only left child exists.
            node = std::move(node->left);  // Replace node by its left child.
            return true;  // Report successful deletion.
        }  // Close left-only case.

        // Case 3: two children => replace with inorder successor (min of right subtree).  // Explain 2-child strategy.
        int successorKey = minKey(node->right.get());  // Find successor key from right subtree.
        node->key = successorKey;  // Copy successor key into current node.
        (void)removeSubtree(node->right, successorKey);  // Delete successor node from right subtree (must exist).
        return true;  // Report successful deletion.
    }  // End removeSubtree().

    static int sizeOf(const Node* node) {  // Recursive helper that counts nodes in a subtree.
        if (node == nullptr) {  // Empty subtree contributes 0 nodes.
            return 0;  // Return 0 for empty.
        }  // Close base case.
        return 1 + sizeOf(node->left.get()) + sizeOf(node->right.get());  // Count root + left + right.
    }  // End sizeOf().

    static int heightOf(const Node* node) {  // Recursive helper that computes height in edges.
        if (node == nullptr) {  // Empty subtree has height -1 (so leaf has height 0).
            return -1;  // Return -1 for empty.
        }  // Close base case.
        return 1 + std::max(heightOf(node->left.get()), heightOf(node->right.get()));  // Height is 1 + max(child heights).
    }  // End heightOf().

    static void inorderWalk(const Node* node, std::vector<int>& out) {  // Recursive inorder (left, root, right).
        if (node == nullptr) {  // Empty subtree contributes nothing.
            return;  // Stop recursion.
        }  // Close base case.
        inorderWalk(node->left.get(), out);  // Visit left subtree first.
        out.push_back(node->key);  // Visit root.
        inorderWalk(node->right.get(), out);  // Visit right subtree.
    }  // End inorderWalk().
};  // End BinarySearchTree.

}  // namespace bstunit  // Close namespace.

#endif  // BINARY_SEARCH_TREE_HPP  // End of header guard.

