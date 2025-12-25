// 02 Red-Black Tree 示範（C++，LLRB）/ Red-black tree demo (C++, LLRB).  // Bilingual header line for this unit.
#ifndef RED_BLACK_TREE_HPP  // Header guard to prevent multiple inclusion.
#define RED_BLACK_TREE_HPP  // Header guard definition.

#include <algorithm>  // Provide std::max for height calculation.
#include <memory>  // Provide std::unique_ptr for owning nodes safely.
#include <optional>  // Provide std::optional for min/max on empty trees.
#include <vector>  // Provide std::vector for inorder traversal output.

namespace rbtunit {  // Use a small namespace to avoid polluting the global namespace.

struct TreeSummary {  // Summarize key properties and inorder traversal for printing/tests.
    int size;  // Number of nodes in the tree.
    int height;  // Height measured in edges (empty=-1, leaf=0).
    std::optional<int> minValue;  // Minimum key (nullopt if empty).
    std::optional<int> maxValue;  // Maximum key (nullopt if empty).
    std::vector<int> inorder;  // Inorder traversal (sorted keys for a valid BST).
    bool isValid;  // Whether the structure satisfies RB + BST invariants.
};  // End of TreeSummary.

class RedBlackTree {  // Red-black tree wrapper (implemented as Left-Leaning Red-Black, LLRB).
public:
    static constexpr bool RED = true;  // Represent red nodes/links with true for readability.
    static constexpr bool BLACK = false;  // Represent black nodes/links with false for readability.

    struct Node {  // LLRB node (key + left child + right child + color).
        int key;  // Node key (mutable so delete can replace with successor key).
        bool red;  // Node color (true=RED, false=BLACK).
        std::unique_ptr<Node> left;  // Own left subtree.
        std::unique_ptr<Node> right;  // Own right subtree.

        Node(int k, bool isRed) : key(k), red(isRed), left(nullptr), right(nullptr) {  // Initialize node with key, color, and empty children.
        }  // Close constructor.
    };  // End Node.

    RedBlackTree() : root_(nullptr) {  // Initialize an empty tree.
    }  // Close constructor.

    static RedBlackTree fromValues(const std::vector<int>& values) {  // Build a tree by inserting values in order.
        RedBlackTree t;  // Start from an empty tree.
        for (int v : values) {  // Insert each value in the given order.
            t.insert(v);  // Insert one key (duplicates are rejected by policy).
        }  // Close loop.
        return t;  // Return built tree.
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
        root_ = insertSubtree(std::move(root_), key, inserted);  // Insert into root subtree.
        if (root_ != nullptr) {  // Guard for empty tree.
            root_->red = BLACK;  // Root must always be black.
        }  // Close root-color guard.
        return inserted;  // Return whether insertion happened.
    }  // End insert().

    bool remove(int key) {  // Remove key; return true when a node was removed.
        if (root_ == nullptr) {  // Empty tree cannot delete anything.
            return false;  // Report not removed.
        }  // Close empty case.
        if (!contains(key)) {  // Avoid running LLRB delete when key is missing.
            return false;  // Report not removed.
        }  // Close missing-key guard.

        if (!isRed(root_->left.get()) && !isRed(root_->right.get())) {  // Detect 2-node root situation.
            root_->red = RED;  // Temporarily color root red to simplify deletion logic.
        }  // Close pre-step.

        root_ = deleteSubtree(std::move(root_), key);  // Delete key from root subtree (guaranteed to exist).
        if (root_ != nullptr) {  // Restore invariant when tree is non-empty.
            root_->red = BLACK;  // Root must be black.
        }  // Close root-color guard.
        return true;  // Report removed.
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
        return heightOf(root_.get());  // Compute height from root.
    }  // End height().

    std::vector<int> inorder() const {  // Return inorder traversal (sorted for a valid BST).
        std::vector<int> out;  // Accumulate traversal result.
        out.reserve(static_cast<size_t>(size()));  // Reserve to avoid reallocations.
        inorderWalk(root_.get(), out);  // Walk from root.
        return out;  // Return traversal list.
    }  // End inorder().

    bool validate() const {  // Validate BST ordering + red-black properties (LLRB variant).
        if (root_ == nullptr) {  // Empty tree is valid.
            return true;  // Vacuously valid.
        }  // Close empty case.
        if (root_->red) {  // Root must be black.
            return false;  // Report invalid root color.
        }  // Close root check.
        int blackHeight = 0;  // Store computed black-height output.
        return validateSubtree(root_.get(), std::nullopt, std::nullopt, blackHeight);  // Validate from root with open bounds.
    }  // End validate().

    TreeSummary summarize() const {  // Build a summary snapshot for demos/tests.
        return TreeSummary{  // Construct summary object.
            size(),  // Compute node count.
            height(),  // Compute height (edges).
            min(),  // Compute minimum key (optional).
            max(),  // Compute maximum key (optional).
            inorder(),  // Compute inorder traversal.
            validate(),  // Validate red-black invariants.
        };  // Close summary.
    }  // End summarize().

private:
    std::unique_ptr<Node> root_;  // Own the root subtree.

    static bool isRed(const Node* node) {  // Return true when node is red; nullptr is treated as black.
        return node != nullptr && node->red == RED;  // Null links are black by definition.
    }  // End isRed().

    static std::unique_ptr<Node> rotateLeft(std::unique_ptr<Node> h) {  // Rotate left to eliminate a right-leaning red link.
        std::unique_ptr<Node> x = std::move(h->right);  // Take right child as pivot (transfer ownership).
        std::unique_ptr<Node> t2 = std::move(x->left);  // Keep pivot's left subtree (will become h's right subtree).

        x->left = std::move(h);  // Put h as pivot's left child (transfer ownership).
        x->left->right = std::move(t2);  // Attach T2 as h's right child.

        x->red = x->left->red;  // Preserve original color on the new root (x gets old h color).
        x->left->red = RED;  // Mark the moved-down node as red.
        return x;  // Return new subtree root.
    }  // End rotateLeft().

    static std::unique_ptr<Node> rotateRight(std::unique_ptr<Node> h) {  // Rotate right to fix two consecutive left-leaning red links.
        std::unique_ptr<Node> x = std::move(h->left);  // Take left child as pivot (transfer ownership).
        std::unique_ptr<Node> t2 = std::move(x->right);  // Keep pivot's right subtree (will become h's left subtree).

        x->right = std::move(h);  // Put h as pivot's right child (transfer ownership).
        x->right->left = std::move(t2);  // Attach T2 as h's left child.

        x->red = x->right->red;  // Preserve original color on the new root (x gets old h color).
        x->right->red = RED;  // Mark the moved-down node as red.
        return x;  // Return new subtree root.
    }  // End rotateRight().

    static void flipColors(Node* h) {  // Flip colors to split or merge 4-nodes (LLRB <-> 2-3 tree).
        h->red = !h->red;  // Toggle parent color.
        if (h->left != nullptr) {  // Guard for safety (children should exist in normal LLRB usage).
            h->left->red = !h->left->red;  // Toggle left child color.
        }  // Close left guard.
        if (h->right != nullptr) {  // Guard for safety (children should exist in normal LLRB usage).
            h->right->red = !h->right->red;  // Toggle right child color.
        }  // Close right guard.
    }  // End flipColors().

    static std::unique_ptr<Node> fixUp(std::unique_ptr<Node> h) {  // Restore LLRB invariants on the way back up after insert/delete.
        if (isRed(h->right.get()) && !isRed(h->left.get())) {  // Fix right-leaning red link.
            h = rotateLeft(std::move(h));  // Rotate left to make red link lean left.
        }  // Close right-red fix.
        if (isRed(h->left.get()) && isRed(h->left->left.get())) {  // Fix two consecutive left reds.
            h = rotateRight(std::move(h));  // Rotate right to balance the 4-node shape.
        }  // Close left-left fix.
        if (isRed(h->left.get()) && isRed(h->right.get())) {  // Split 4-node when both children are red.
            flipColors(h.get());  // Flip colors to push red up one level.
        }  // Close split case.
        return h;  // Return possibly rotated/flipped subtree root.
    }  // End fixUp().

    static std::unique_ptr<Node> moveRedLeft(std::unique_ptr<Node> h) {  // Ensure h->left is red when we need to delete from the left side.
        flipColors(h.get());  // Push red down so we can borrow from the right side.
        if (h->right != nullptr && isRed(h->right->left.get())) {  // Borrow from right-left red link when available.
            h->right = rotateRight(std::move(h->right));  // Rotate right on right child to create a right-leaning red link.
            h = rotateLeft(std::move(h));  // Rotate left on h to move red link to the left side.
            flipColors(h.get());  // Re-flip to restore colors after rotations.
        }  // Close borrow case.
        return h;  // Return adjusted subtree root.
    }  // End moveRedLeft().

    static std::unique_ptr<Node> moveRedRight(std::unique_ptr<Node> h) {  // Ensure h->right is red when we need to delete from the right side.
        flipColors(h.get());  // Push red down so we can borrow from the left side.
        if (h->left != nullptr && isRed(h->left->left.get())) {  // Borrow from left-left red link when available.
            h = rotateRight(std::move(h));  // Rotate right to move red link to the right side.
            flipColors(h.get());  // Re-flip to restore colors after rotation.
        }  // Close borrow case.
        return h;  // Return adjusted subtree root.
    }  // End moveRedRight().

    static std::unique_ptr<Node> insertSubtree(std::unique_ptr<Node> h, int key, bool& inserted) {  // Insert key into subtree and rebalance.
        if (h == nullptr) {  // Empty spot: create a new red leaf node.
            inserted = true;  // Mark insertion.
            return std::make_unique<Node>(key, RED);  // Return new node.
        }  // Close base case.

        if (key < h->key) {  // Insert into left subtree when key is smaller.
            h->left = insertSubtree(std::move(h->left), key, inserted);  // Recurse into left child slot.
        } else if (key > h->key) {  // Insert into right subtree when key is larger.
            h->right = insertSubtree(std::move(h->right), key, inserted);  // Recurse into right child slot.
        } else {  // key == h->key means duplicate key.
            inserted = false;  // Mark no insertion.
            return h;  // Reject duplicates by policy (no changes).
        }  // Close compare branch.

        return fixUp(std::move(h));  // Restore invariants after insertion below.
    }  // End insertSubtree().

    static int minKey(const Node* node) {  // Return the minimum key in a non-empty subtree.
        const Node* cur = node;  // Start at given node.
        while (cur->left != nullptr) {  // Keep going left until we cannot.
            cur = cur->left.get();  // Move to left child.
        }  // Close loop.
        return cur->key;  // Leftmost node has the minimum key.
    }  // End minKey().

    static std::unique_ptr<Node> deleteMin(std::unique_ptr<Node> h) {  // Delete the minimum node in this subtree and return new subtree root.
        if (h->left == nullptr) {  // Reached the minimum node (no left child).
            return nullptr;  // Delete this node by returning nullptr.
        }  // Close base case.

        if (!isRed(h->left.get()) && !isRed(h->left->left.get())) {  // Ensure we do not descend into a 2-node.
            h = moveRedLeft(std::move(h));  // Move a red link to the left side.
        }  // Close move case.

        h->left = deleteMin(std::move(h->left));  // Recurse into left subtree.
        return fixUp(std::move(h));  // Fix up invariants on the way back up.
    }  // End deleteMin().

    static std::unique_ptr<Node> deleteSubtree(std::unique_ptr<Node> h, int key) {  // Delete key from subtree (assumes key exists).
        if (key < h->key) {  // Key is in the left subtree.
            if (h->left != nullptr) {  // Only recurse when left subtree exists.
                if (!isRed(h->left.get()) && !isRed(h->left->left.get())) {  // Avoid descending into a 2-node.
                    h = moveRedLeft(std::move(h));  // Move a red link to the left side.
                }  // Close move case.
                h->left = deleteSubtree(std::move(h->left), key);  // Recurse into left subtree.
            }  // Close left guard.
        } else {  // Key is in the current node or right subtree.
            if (isRed(h->left.get())) {  // Rotate right to maintain delete preconditions.
                h = rotateRight(std::move(h));  // Rotate right to make sure we can move red right when needed.
            }  // Close rotate-right pre-step.

            if (key == h->key && h->right == nullptr) {  // Case: match and no right child => can delete this node.
                return nullptr;  // Delete node by returning nullptr.
            }  // Close leaf-right-null case.

            if (h->right != nullptr) {  // Only work on right side when it exists.
                if (!isRed(h->right.get()) && !isRed(h->right->left.get())) {  // Avoid descending into a 2-node.
                    h = moveRedRight(std::move(h));  // Move a red link to the right side.
                }  // Close move case.
                if (key == h->key) {  // Match case with a right subtree => replace with successor.
                    int successorKey = minKey(h->right.get());  // Find successor key from right subtree.
                    h->key = successorKey;  // Copy successor key into current node.
                    h->right = deleteMin(std::move(h->right));  // Delete successor node from right subtree.
                } else {  // Otherwise delete in the right subtree.
                    h->right = deleteSubtree(std::move(h->right), key);  // Recurse into right subtree.
                }  // Close match/descend branch.
            }  // Close right guard.
        }  // Close key-side branch.

        return fixUp(std::move(h));  // Fix up invariants on the way back up.
    }  // End deleteSubtree().

    static int sizeOf(const Node* node) {  // Recursive helper that counts nodes in a subtree.
        if (node == nullptr) {  // Empty subtree contributes 0 nodes.
            return 0;  // Return 0 for empty.
        }  // Close base case.
        return 1 + sizeOf(node->left.get()) + sizeOf(node->right.get());  // Count root + left + right.
    }  // End sizeOf().

    static int heightOf(const Node* node) {  // Recursive helper that computes height in edges.
        if (node == nullptr) {  // Empty subtree has height -1.
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

    static bool validateSubtree(const Node* node, const std::optional<int>& low, const std::optional<int>& high, int& outBlackHeight) {  // Validate subtree and output black-height.
        if (node == nullptr) {  // Null links are black and contribute a base black-height of 1.
            outBlackHeight = 1;  // Treat nullptr as a black leaf sentinel.
            return true;  // Report valid.
        }  // Close base case.

        if (low.has_value() && node->key <= *low) {  // Enforce strict lower bound (no duplicates).
            return false;  // Report invalid ordering.
        }  // Close lower-bound check.
        if (high.has_value() && node->key >= *high) {  // Enforce strict upper bound (no duplicates).
            return false;  // Report invalid ordering.
        }  // Close upper-bound check.

        if (isRed(node->right.get())) {  // LLRB rule: red links must lean left (no red right child).
            return false;  // Report invalid right-leaning red link.
        }  // Close right-red check.

        if (isRed(node) && (isRed(node->left.get()) || isRed(node->right.get()))) {  // RB rule: no two consecutive reds.
            return false;  // Report invalid consecutive red links.
        }  // Close consecutive-red check.

        int leftBH = 0;  // Store computed left subtree black-height.
        int rightBH = 0;  // Store computed right subtree black-height.
        if (!validateSubtree(node->left.get(), low, std::optional<int>(node->key), leftBH)) {  // Validate left subtree with updated upper bound.
            return false;  // Propagate failure.
        }  // Close left validation.
        if (!validateSubtree(node->right.get(), std::optional<int>(node->key), high, rightBH)) {  // Validate right subtree with updated lower bound.
            return false;  // Propagate failure.
        }  // Close right validation.

        if (leftBH != rightBH) {  // RB rule: all root-to-leaf paths have same number of black nodes.
            return false;  // Report black-height mismatch.
        }  // Close black-height check.

        int blackHere = node->red ? 0 : 1;  // Count black nodes; red nodes do not add to black-height.
        outBlackHeight = leftBH + blackHere;  // Write computed black-height for this subtree.
        return true;  // Report valid subtree.
    }  // End validateSubtree().
};  // End RedBlackTree.

}  // namespace rbtunit  // Close namespace.

#endif  // RED_BLACK_TREE_HPP  // End of header guard.

