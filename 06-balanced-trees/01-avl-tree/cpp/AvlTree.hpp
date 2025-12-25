// 01 AVL Tree 示範（C++）/ AVL tree demo (C++).  // Bilingual header line for this unit.
#ifndef AVL_TREE_HPP  // Header guard to prevent multiple inclusion.
#define AVL_TREE_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max for height calculation.
#include <cstdlib>  // Provide std::abs for balance checks.
#include <memory>  // Provide std::unique_ptr for owning nodes safely.
#include <optional>  // Provide std::optional for min/max on empty trees.
#include <vector>  // Provide std::vector for inorder traversal output.

namespace avlunit {  // Use a small namespace to avoid polluting the global namespace.

struct TreeSummary {  // Summarize key properties and inorder traversal for printing/tests.
    int size;  // Number of nodes in the AVL tree.
    int height;  // Height measured in edges (empty=-1, leaf=0).
    std::optional<int> minValue;  // Minimum key (nullopt if empty).
    std::optional<int> maxValue;  // Maximum key (nullopt if empty).
    std::vector<int> inorder;  // Inorder traversal (sorted keys for a valid BST).
    bool isValid;  // Whether the structure satisfies AVL + BST invariants.
};  // End of TreeSummary.

class AvlTree {  // An AVL wrapper that owns nodes and exposes common operations.
public:
    struct Node {  // AVL node (key + left child + right child + cached height).
        int key;  // Node key (mutable so delete can replace with successor key).
        int height;  // Cached height in edges (leaf=0 by definition).
        std::unique_ptr<Node> left;  // Own left subtree.
        std::unique_ptr<Node> right;  // Own right subtree.

        explicit Node(int k) : key(k), height(0), left(nullptr), right(nullptr) {  // Initialize node with key and empty children.
        }  // Close constructor.
    };  // End Node.

    AvlTree() : root_(nullptr) {  // Initialize an empty AVL tree.
    }  // Close constructor.

    static AvlTree fromValues(const std::vector<int>& values) {  // Build an AVL tree by inserting values in order.
        AvlTree t;  // Start from an empty tree.
        for (int v : values) {  // Insert each value in the given order.
            t.insert(v);  // Insert one key (duplicates are rejected by policy).
        }  // Close loop.
        return t;  // Return built AVL tree.
    }  // End fromValues().

    bool isEmpty() const {  // Check whether the tree has no nodes.
        return root_ == nullptr;  // Empty iff root_ is nullptr.
    }  // End isEmpty().

    bool contains(int key) const {  // Return true if key exists in the tree.
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

    bool insert(int key) {  // Insert key; return false when key already exists.
        bool inserted = false;  // Track whether insertion happened.
        root_ = insertSubtree(std::move(root_), key, inserted);  // Insert into root subtree (rotations may change root).
        return inserted;  // Return whether insertion happened.
    }  // End insert().

    bool remove(int key) {  // Remove key; return true when a node was removed.
        bool removed = false;  // Track whether deletion happened.
        root_ = removeSubtree(std::move(root_), key, removed);  // Delete from root subtree (rotations may change root).
        return removed;  // Return whether deletion happened.
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

    int size() const {  // Return number of nodes in the tree (O(n)).
        return sizeOf(root_.get());  // Count nodes starting from root.
    }  // End size().

    int height() const {  // Return height measured in edges (empty=-1, leaf=0).
        return root_ == nullptr ? -1 : root_->height;  // Use cached root height (or -1 for empty).
    }  // End height().

    std::vector<int> inorder() const {  // Return inorder traversal (sorted for a valid BST).
        std::vector<int> out;  // Accumulate traversal result.
        out.reserve(static_cast<size_t>(size()));  // Reserve to avoid reallocations.
        inorderWalk(root_.get(), out);  // Walk from root.
        return out;  // Return traversal list.
    }  // End inorder().

    bool validate() const {  // Validate BST ordering + AVL balance + cached height correctness.
        int computedHeight = 0;  // Store computed height output from validation.
        return validateSubtree(root_.get(), std::nullopt, std::nullopt, computedHeight);  // Validate from root with open bounds.
    }  // End validate().

    TreeSummary summarize() const {  // Build a summary snapshot for demos/tests.
        return TreeSummary{  // Construct summary object.
            size(),  // Compute node count.
            height(),  // Read tree height (edges).
            min(),  // Compute minimum key (optional).
            max(),  // Compute maximum key (optional).
            inorder(),  // Compute inorder traversal.
            validate(),  // Validate AVL invariants.
        };  // Close summary.
    }  // End summarize().

private:
    std::unique_ptr<Node> root_;  // Own the root subtree.

    static int h(const Node* node) {  // Return cached height for node, or -1 for empty subtree.
        return node == nullptr ? -1 : node->height;  // Apply repo-wide height convention.
    }  // End h().

    static void updateHeight(Node* node) {  // Recompute node->height from children heights.
        node->height = 1 + std::max(h(node->left.get()), h(node->right.get()));  // Height in edges is 1 + max(child heights).
    }  // End updateHeight().

    static int balanceFactor(const Node* node) {  // Compute balance factor = height(left) - height(right).
        return h(node->left.get()) - h(node->right.get());  // Positive means left-heavy; negative means right-heavy.
    }  // End balanceFactor().

    static std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> y) {  // Right rotation to fix a left-heavy imbalance.
        std::unique_ptr<Node> x = std::move(y->left);  // Take left child as pivot (transfer ownership).
        std::unique_ptr<Node> t2 = std::move(x->right);  // Keep pivot's right subtree (will become y's left subtree).

        x->right = std::move(y);  // Put y as the right child of x (transfer ownership).
        x->right->left = std::move(t2);  // Attach T2 as y's left child.

        updateHeight(x->right.get());  // Update y first because its children changed.
        updateHeight(x.get());  // Update x after y because x now points to y.
        return x;  // Return new subtree root.
    }  // End rotateRight().

    static std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> x) {  // Left rotation to fix a right-heavy imbalance.
        std::unique_ptr<Node> y = std::move(x->right);  // Take right child as pivot (transfer ownership).
        std::unique_ptr<Node> t2 = std::move(y->left);  // Keep pivot's left subtree (will become x's right subtree).

        y->left = std::move(x);  // Put x as the left child of y (transfer ownership).
        y->left->right = std::move(t2);  // Attach T2 as x's right child.

        updateHeight(y->left.get());  // Update x first because its children changed.
        updateHeight(y.get());  // Update y after x because y now points to x.
        return y;  // Return new subtree root.
    }  // End rotateLeft().

    static std::unique_ptr<Node> rebalance(std::unique_ptr<Node> node) {  // Rebalance node if it violates AVL balance constraints.
        int balance = balanceFactor(node.get());  // Compute current node's balance factor.

        if (balance > 1) {  // Left-heavy subtree; need either LL or LR rotation(s).
            if (balanceFactor(node->left.get()) < 0) {  // LR case: left child is right-heavy.
                node->left = rotateLeft(std::move(node->left));  // LR case step 1: rotate left on left child.
            }  // Close LR pre-rotation.
            return rotateRight(std::move(node));  // LL/LR step 2: rotate right on current node.
        }  // Close left-heavy branch.

        if (balance < -1) {  // Right-heavy subtree; need either RR or RL rotation(s).
            if (balanceFactor(node->right.get()) > 0) {  // RL case: right child is left-heavy.
                node->right = rotateRight(std::move(node->right));  // RL case step 1: rotate right on right child.
            }  // Close RL pre-rotation.
            return rotateLeft(std::move(node));  // RR/RL step 2: rotate left on current node.
        }  // Close right-heavy branch.

        return node;  // Already balanced; return node unchanged.
    }  // End rebalance().

    static std::unique_ptr<Node> insertSubtree(std::unique_ptr<Node> node, int key, bool& inserted) {  // Insert into subtree and rebalance.
        if (node == nullptr) {  // Empty spot: create a new leaf node.
            inserted = true;  // Mark insertion.
            return std::make_unique<Node>(key);  // Return new node.
        }  // Close base case.

        if (key < node->key) {  // Insert into left subtree when key is smaller.
            node->left = insertSubtree(std::move(node->left), key, inserted);  // Recurse into left child slot.
        } else if (key > node->key) {  // Insert into right subtree when key is larger.
            node->right = insertSubtree(std::move(node->right), key, inserted);  // Recurse into right child slot.
        } else {  // key == node->key means duplicate key.
            inserted = false;  // Mark no insertion.
            return node;  // Reject duplicates by policy (no changes).
        }  // Close compare branch.

        updateHeight(node.get());  // Update cached height after insertion below.
        return rebalance(std::move(node));  // Rebalance on the way back up.
    }  // End insertSubtree().

    static std::unique_ptr<Node> removeSubtree(std::unique_ptr<Node> node, int key, bool& removed) {  // Delete from subtree and rebalance.
        if (node == nullptr) {  // Key not found in an empty subtree.
            removed = false;  // Mark that nothing was removed.
            return nullptr;  // Return empty subtree.
        }  // Close base case.

        if (key < node->key) {  // Delete from left subtree when key is smaller.
            node->left = removeSubtree(std::move(node->left), key, removed);  // Recurse into left child.
        } else if (key > node->key) {  // Delete from right subtree when key is larger.
            node->right = removeSubtree(std::move(node->right), key, removed);  // Recurse into right child.
        } else {  // key == node->key means we found the node to delete.
            removed = true;  // Mark that we are removing a node.

            if (node->left == nullptr && node->right == nullptr) {  // Case 1: leaf node (0 children).
                return nullptr;  // Remove leaf by returning nullptr.
            }  // Close leaf case.
            if (node->left == nullptr) {  // Case 2a: only right child exists.
                return std::move(node->right);  // Replace node by its right child.
            }  // Close right-only case.
            if (node->right == nullptr) {  // Case 2b: only left child exists.
                return std::move(node->left);  // Replace node by its left child.
            }  // Close left-only case.

            // Case 3: two children => replace with inorder successor (min of right subtree).  // Explain 2-child strategy.
            int successorKey = minKey(node->right.get());  // Find successor key from right subtree.
            node->key = successorKey;  // Copy successor key into current node (keeps tree shape).
            bool dummy = false;  // Track removal of successor node (must be true, but not needed here).
            node->right = removeSubtree(std::move(node->right), successorKey, dummy);  // Delete successor node from right subtree.
        }  // Close match branch.

        updateHeight(node.get());  // Update cached height after deletion below.
        return rebalance(std::move(node));  // Rebalance on the way back up.
    }  // End removeSubtree().

    static int minKey(const Node* node) {  // Return the minimum key in a non-empty subtree.
        const Node* cur = node;  // Start at given node.
        while (cur->left != nullptr) {  // Keep going left until we cannot.
            cur = cur->left.get();  // Move to left child.
        }  // Close loop.
        return cur->key;  // Leftmost node has the minimum key.
    }  // End minKey().

    static int sizeOf(const Node* node) {  // Recursive helper that counts nodes in a subtree.
        if (node == nullptr) {  // Empty subtree contributes 0 nodes.
            return 0;  // Return 0 for empty.
        }  // Close base case.
        return 1 + sizeOf(node->left.get()) + sizeOf(node->right.get());  // Count root + left + right.
    }  // End sizeOf().

    static void inorderWalk(const Node* node, std::vector<int>& out) {  // Recursive inorder (left, root, right).
        if (node == nullptr) {  // Empty subtree contributes nothing.
            return;  // Stop recursion.
        }  // Close base case.
        inorderWalk(node->left.get(), out);  // Visit left subtree first.
        out.push_back(node->key);  // Visit root.
        inorderWalk(node->right.get(), out);  // Visit right subtree.
    }  // End inorderWalk().

    static bool validateSubtree(const Node* node, const std::optional<int>& low, const std::optional<int>& high, int& outHeight) {  // Validate subtree and output computed height.
        if (node == nullptr) {  // Empty subtree is valid and has height -1 by definition.
            outHeight = -1;  // Write computed height for empty subtree.
            return true;  // Report valid.
        }  // Close base case.

        if (low.has_value() && node->key <= *low) {  // Enforce strict lower bound (no duplicates).
            return false;  // Report invalid ordering.
        }  // Close lower-bound check.
        if (high.has_value() && node->key >= *high) {  // Enforce strict upper bound (no duplicates).
            return false;  // Report invalid ordering.
        }  // Close upper-bound check.

        int leftH = 0;  // Store computed left subtree height.
        int rightH = 0;  // Store computed right subtree height.
        if (!validateSubtree(node->left.get(), low, std::optional<int>(node->key), leftH)) {  // Validate left subtree with updated upper bound.
            return false;  // Propagate failure.
        }  // Close left validation.
        if (!validateSubtree(node->right.get(), std::optional<int>(node->key), high, rightH)) {  // Validate right subtree with updated lower bound.
            return false;  // Propagate failure.
        }  // Close right validation.

        int computed = 1 + std::max(leftH, rightH);  // Compute expected height from children heights.
        if (node->height != computed) {  // Cached height must match computed height.
            return false;  // Report height mismatch.
        }  // Close height check.

        if (std::abs(leftH - rightH) > 1) {  // AVL balance constraint requires |bf| <= 1.
            return false;  // Report imbalance.
        }  // Close balance check.

        outHeight = computed;  // Write computed height for this subtree.
        return true;  // Report valid subtree.
    }  // End validateSubtree().
};  // End AvlTree.

}  // namespace avlunit  // Close namespace.

#endif  // AVL_TREE_HPP  // End of header guard.

