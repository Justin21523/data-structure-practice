// 03 樹走訪示範（C++）/ Tree traversal demo (C++).  // Bilingual header line for this unit.
#ifndef TREE_TRAVERSAL_HPP  // Header guard to prevent multiple inclusion.
#define TREE_TRAVERSAL_HPP  // Header guard definition.

#include <memory>  // Provide std::unique_ptr and std::make_unique for owning nodes safely.
#include <optional>  // Provide std::optional for null values in level-order input.
#include <utility>  // Provide std::pair for (node, visited) stack frames.
#include <vector>  // Provide std::vector for traversal outputs and queue/stack storage.

namespace traversalunit {  // Use a small namespace to avoid polluting the global namespace.

struct TraversalSummary {  // Summarize recursive/iterative traversal outputs for printing/tests.
    std::vector<int> preorderRecursive;  // Preorder traversal via recursion.
    std::vector<int> preorderIterative;  // Preorder traversal via explicit stack.
    std::vector<int> inorderRecursive;  // Inorder traversal via recursion.
    std::vector<int> inorderIterative;  // Inorder traversal via explicit stack.
    std::vector<int> postorderRecursive;  // Postorder traversal via recursion.
    std::vector<int> postorderIterative;  // Postorder traversal via explicit stack.
    std::vector<int> levelOrder;  // Level-order traversal via BFS queue.
};  // End of TraversalSummary.

class BinaryTree {  // A binary tree wrapper that owns nodes and provides traversal algorithms.
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
            t.nodes_.push_back(std::make_unique<Node>(*values[i]));  // Allocate node and store ownership in the tree.
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

    Node* root() const {  // Expose root pointer for demos/tests (read-only by convention).
        return root_;  // Return root pointer.
    }  // End root().

    bool isEmpty() const {  // Check whether the tree has no nodes.
        return root_ == nullptr;  // Empty iff root is nullptr.
    }  // End isEmpty().

    std::vector<int> preorderRecursive() const {  // Return preorder traversal via recursion (root, left, right).
        std::vector<int> out;  // Accumulate traversal result.
        preorderWalk(root_, out);  // Walk from root.
        return out;  // Return traversal list.
    }  // End preorderRecursive().

    std::vector<int> inorderRecursive() const {  // Return inorder traversal via recursion (left, root, right).
        std::vector<int> out;  // Accumulate traversal result.
        inorderWalk(root_, out);  // Walk from root.
        return out;  // Return traversal list.
    }  // End inorderRecursive().

    std::vector<int> postorderRecursive() const {  // Return postorder traversal via recursion (left, right, root).
        std::vector<int> out;  // Accumulate traversal result.
        postorderWalk(root_, out);  // Walk from root.
        return out;  // Return traversal list.
    }  // End postorderRecursive().

    std::vector<int> preorderIterative() const {  // Return preorder traversal via explicit stack (root, left, right).
        if (root_ == nullptr) {  // Empty tree has empty traversal.
            return {};  // Return empty vector.
        }  // Close empty case.

        std::vector<int> out;  // Accumulate traversal result.
        std::vector<const Node*> stack;  // Use vector as stack storage.
        stack.push_back(root_);  // Push root onto stack.

        while (!stack.empty()) {  // Process until stack is empty.
            const Node* node = stack.back();  // Read top node.
            stack.pop_back();  // Pop top node (LIFO).
            out.push_back(node->value);  // Visit node when popped (preorder).
            if (node->right != nullptr) {  // Push right child first so left is processed first.
                stack.push_back(node->right);  // Push right child.
            }  // Close right push.
            if (node->left != nullptr) {  // Push left child last.
                stack.push_back(node->left);  // Push left child.
            }  // Close left push.
        }  // Close loop.

        return out;  // Return traversal list.
    }  // End preorderIterative().

    std::vector<int> inorderIterative() const {  // Return inorder traversal via explicit stack (left, root, right).
        std::vector<int> out;  // Accumulate traversal result.
        std::vector<const Node*> stack;  // Use vector as stack storage.
        const Node* node = root_;  // Start from root.

        while (node != nullptr || !stack.empty()) {  // Continue while there is a node to visit or stack is non-empty.
            while (node != nullptr) {  // Walk left as far as possible.
                stack.push_back(node);  // Push current node so we can come back later.
                node = node->left;  // Move to left child.
            }  // Close left-walk loop.
            node = stack.back();  // Read the next node whose left subtree is done.
            stack.pop_back();  // Pop it from the stack.
            out.push_back(node->value);  // Visit node (inorder).
            node = node->right;  // Move to right subtree next.
        }  // Close main loop.

        return out;  // Return traversal list.
    }  // End inorderIterative().

    std::vector<int> postorderIterative() const {  // Return postorder traversal via explicit stack (left, right, root).
        if (root_ == nullptr) {  // Empty tree has empty traversal.
            return {};  // Return empty vector.
        }  // Close empty case.

        std::vector<int> out;  // Accumulate traversal result.
        std::vector<std::pair<const Node*, bool>> stack;  // Store (node, visited) frames.
        stack.push_back(std::make_pair(root_, false));  // Push root frame (not visited).

        while (!stack.empty()) {  // Process until stack is empty.
            std::pair<const Node*, bool> f = stack.back();  // Read top frame.
            stack.pop_back();  // Pop top frame.
            const Node* node = f.first;  // Extract node pointer.
            bool visited = f.second;  // Extract visited flag.
            if (visited) {  // When visited==true, children were already processed.
                out.push_back(node->value);  // Visit node last (postorder).
                continue;  // Move to next frame.
            }  // Close visited branch.
            stack.push_back(std::make_pair(node, true));  // Re-push node marked as visited (to output after children).
            if (node->right != nullptr) {  // Push right child first so left is processed first.
                stack.push_back(std::make_pair(node->right, false));  // Process right subtree.
            }  // Close right push.
            if (node->left != nullptr) {  // Push left child last.
                stack.push_back(std::make_pair(node->left, false));  // Process left subtree first.
            }  // Close left push.
        }  // Close loop.

        return out;  // Return traversal list.
    }  // End postorderIterative().

    std::vector<int> levelOrder() const {  // Return level-order traversal via BFS queue.
        if (root_ == nullptr) {  // Empty tree has empty traversal.
            return {};  // Return empty vector.
        }  // Close empty case.

        std::vector<const Node*> queue;  // Use vector as queue storage.
        size_t head = 0;  // Maintain head index so dequeue is O(1) without shifting.
        std::vector<int> out;  // Accumulate traversal result.

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

    TraversalSummary summarize() const {  // Build a summary snapshot for demos/tests.
        return TraversalSummary{  // Construct summary object.
            preorderRecursive(),  // Compute recursive preorder.
            preorderIterative(),  // Compute iterative preorder.
            inorderRecursive(),  // Compute recursive inorder.
            inorderIterative(),  // Compute iterative inorder.
            postorderRecursive(),  // Compute recursive postorder.
            postorderIterative(),  // Compute iterative postorder.
            levelOrder(),  // Compute BFS traversal.
        };  // Close summary.
    }  // End summarize().

private:
    Node* root_;  // Root pointer (non-owning; owned by nodes_).
    std::vector<std::unique_ptr<Node>> nodes_;  // Own all nodes so raw pointers stay valid.

    static void preorderWalk(const Node* node, std::vector<int>& out) {  // Recursive preorder helper.
        if (node == nullptr) {  // Empty subtree contributes nothing.
            return;  // Stop recursion.
        }  // Close base case.
        out.push_back(node->value);  // Visit root first.
        preorderWalk(node->left, out);  // Visit left subtree.
        preorderWalk(node->right, out);  // Visit right subtree.
    }  // End preorderWalk().

    static void inorderWalk(const Node* node, std::vector<int>& out) {  // Recursive inorder helper.
        if (node == nullptr) {  // Empty subtree contributes nothing.
            return;  // Stop recursion.
        }  // Close base case.
        inorderWalk(node->left, out);  // Visit left subtree first.
        out.push_back(node->value);  // Visit root.
        inorderWalk(node->right, out);  // Visit right subtree.
    }  // End inorderWalk().

    static void postorderWalk(const Node* node, std::vector<int>& out) {  // Recursive postorder helper.
        if (node == nullptr) {  // Empty subtree contributes nothing.
            return;  // Stop recursion.
        }  // Close base case.
        postorderWalk(node->left, out);  // Visit left subtree first.
        postorderWalk(node->right, out);  // Visit right subtree next.
        out.push_back(node->value);  // Visit root last.
    }  // End postorderWalk().
};  // End BinaryTree.

}  // namespace traversalunit  // Close namespace.

#endif  // TREE_TRAVERSAL_HPP  // End of header guard.
