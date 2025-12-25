// 03 樹走訪示範（JavaScript）/ Tree traversal demo (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for safer, more explicit JavaScript.  

class Node {  // Binary tree node (value + left child + right child).  
    constructor(value) {  // Initialize a node with a value and empty children.  
        this.value = value;  // Store node value.  
        this.left = null;  // Store left child pointer (null means no child).  
        this.right = null;  // Store right child pointer (null means no child).  
    }  // Close constructor.  
}  // End Node.  

class BinaryTree {  // A binary tree wrapper that holds a root node.  
    constructor(root) {  // Initialize tree with an optional root.  
        this._root = root;  // Store root reference (null means empty tree).  
    }  // Close constructor.  

    get root() {  // Expose root for demos/tests (read-only semantics by convention).  
        return this._root;  // Return root reference.  
    }  // End root getter.  

    static fromLevelOrder(values) {  // Build a tree from level-order array representation (null means missing node).  
        if (!Array.isArray(values) || values.length === 0) {  // Treat non-array/empty as an empty tree.  
            return new BinaryTree(null);  // Return empty tree.  
        }  // Close validation.  

        const nodes = new Array(values.length);  // Allocate parallel node array (null for missing).  
        for (let i = 0; i < values.length; i += 1) {  // First pass: create nodes for non-null values.  
            const v = values[i];  // Read value at index i.  
            nodes[i] = (v === null || v === undefined) ? null : new Node(Number(v));  // Create node or keep null.  
        }  // Close first pass.  

        for (let i = 0; i < values.length; i += 1) {  // Second pass: link children using array indices.  
            const node = nodes[i];  // Read node at index i.  
            if (node === null) {  // Skip missing nodes.  
                continue;  // Move to next index.  
            }  // Close skip branch.  
            const leftIndex = 2 * i + 1;  // Compute left child index.  
            const rightIndex = 2 * i + 2;  // Compute right child index.  
            if (leftIndex < nodes.length) {  // Link left child when index is in range.  
                node.left = nodes[leftIndex];  // Assign left child (may be null).  
            }  // Close left link.  
            if (rightIndex < nodes.length) {  // Link right child when index is in range.  
                node.right = nodes[rightIndex];  // Assign right child (may be null).  
            }  // Close right link.  
        }  // Close second pass.  

        return new BinaryTree(nodes[0]);  // Root is the node at index 0 (may be null).  
    }  // End fromLevelOrder().  

    // =========================  // Visual separator for traversal APIs.  
    // Recursive DFS traversals  // Section title.  
    // =========================  // End separator.  

    preorderRecursive() {  // Return preorder traversal via recursion (root, left, right).  
        const out = [];  // Accumulate traversal result.  
        const walk = (node) => {  // Recursive helper for preorder traversal.  
            if (node === null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            out.push(node.value);  // Visit root first.  
            walk(node.left);  // Visit left subtree.  
            walk(node.right);  // Visit right subtree.  
        };  // Close helper.  
        walk(this._root);  // Walk from root.  
        return out;  // Return traversal list.  
    }  // End preorderRecursive().  

    inorderRecursive() {  // Return inorder traversal via recursion (left, root, right).  
        const out = [];  // Accumulate traversal result.  
        const walk = (node) => {  // Recursive helper for inorder traversal.  
            if (node === null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            walk(node.left);  // Visit left subtree first.  
            out.push(node.value);  // Visit root.  
            walk(node.right);  // Visit right subtree.  
        };  // Close helper.  
        walk(this._root);  // Walk from root.  
        return out;  // Return traversal list.  
    }  // End inorderRecursive().  

    postorderRecursive() {  // Return postorder traversal via recursion (left, right, root).  
        const out = [];  // Accumulate traversal result.  
        const walk = (node) => {  // Recursive helper for postorder traversal.  
            if (node === null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            walk(node.left);  // Visit left subtree first.  
            walk(node.right);  // Visit right subtree next.  
            out.push(node.value);  // Visit root last.  
        };  // Close helper.  
        walk(this._root);  // Walk from root.  
        return out;  // Return traversal list.  
    }  // End postorderRecursive().  

    // =========================  // Visual separator for traversal APIs.  
    // Iterative DFS traversals  // Section title.  
    // =========================  // End separator.  

    preorderIterative() {  // Return preorder traversal via explicit stack (root, left, right).  
        if (this._root === null) {  // Empty tree has empty traversal.  
            return [];  // Return empty list.  
        }  // Close empty case.  

        const out = [];  // Accumulate traversal result.  
        const stack = [this._root];  // Use array as stack; start with root.  
        while (stack.length > 0) {  // Process until stack is empty.  
            const node = stack.pop();  // Pop the top node (LIFO).  
            out.push(node.value);  // Visit node when popped (preorder).  
            if (node.right !== null) {  // Push right child first so left is processed first.  
                stack.push(node.right);  // Push right child.  
            }  // Close right push.  
            if (node.left !== null) {  // Push left child last.  
                stack.push(node.left);  // Push left child.  
            }  // Close left push.  
        }  // Close loop.  
        return out;  // Return traversal list.  
    }  // End preorderIterative().  

    inorderIterative() {  // Return inorder traversal via explicit stack (left, root, right).  
        const out = [];  // Accumulate traversal result.  
        const stack = [];  // Use array as stack to store the path to current node.  
        let node = this._root;  // Start from root.  

        while (node !== null || stack.length > 0) {  // Continue while there is a node to visit or stack is non-empty.  
            while (node !== null) {  // Walk left as far as possible.  
                stack.push(node);  // Push current node so we can come back later.  
                node = node.left;  // Move to left child.  
            }  // Close left-walk loop.  
            node = stack.pop();  // Pop the next node whose left subtree is done.  
            out.push(node.value);  // Visit node (inorder).  
            node = node.right;  // Move to right subtree next.  
        }  // Close main loop.  

        return out;  // Return traversal list.  
    }  // End inorderIterative().  

    postorderIterative() {  // Return postorder traversal via explicit stack (left, right, root).  
        if (this._root === null) {  // Empty tree has empty traversal.  
            return [];  // Return empty list.  
        }  // Close empty case.  

        const out = [];  // Accumulate traversal result.  
        const stack = [[this._root, false]];  // Store [node, visited] so we can output after children.  
        while (stack.length > 0) {  // Process until stack is empty.  
            const [node, visited] = stack.pop();  // Pop one frame.  
            if (visited) {  // When visited==true, children were already processed.  
                out.push(node.value);  // Visit node last (postorder).  
                continue;  // Move to next frame.  
            }  // Close visited branch.  
            stack.push([node, true]);  // Re-push node marked as visited (to output after children).  
            if (node.right !== null) {  // Push right child first so left is processed first.  
                stack.push([node.right, false]);  // Process right subtree.  
            }  // Close right push.  
            if (node.left !== null) {  // Push left child last.  
                stack.push([node.left, false]);  // Process left subtree first.  
            }  // Close left push.  
        }  // Close loop.  
        return out;  // Return traversal list.  
    }  // End postorderIterative().  

    // =========================  // Visual separator for BFS traversal.  
    // BFS traversal (queue)     // Section title.  
    // =========================  // End separator.  

    levelOrder() {  // Return level-order traversal (BFS) as an array of values.  
        if (this._root === null) {  // Empty tree has empty traversal.  
            return [];  // Return empty array.  
        }  // Close empty case.  

        const queue = [this._root];  // Use an array as a queue storage.  
        let head = 0;  // Maintain head index so dequeue is O(1) without shifting.  
        const out = [];  // Accumulate traversal result.  

        while (head < queue.length) {  // Process queue until head reaches the end.  
            const node = queue[head];  // Read next node to process.  
            head += 1;  // Advance head (logical dequeue).  
            out.push(node.value);  // Visit node in BFS order.  
            if (node.left !== null) {  // Enqueue left child when present.  
                queue.push(node.left);  // Append to queue tail.  
            }  // Close left enqueue.  
            if (node.right !== null) {  // Enqueue right child when present.  
                queue.push(node.right);  // Append to queue tail.  
            }  // Close right enqueue.  
        }  // Close BFS loop.  

        return out;  // Return traversal list.  
    }  // End levelOrder().  

    summarize() {  // Build a snapshot summary for demos/tests.  
        return {  // Return a compact summary object.  
            preorderRecursive: this.preorderRecursive(),  // Compute recursive preorder.  
            preorderIterative: this.preorderIterative(),  // Compute iterative preorder.  
            inorderRecursive: this.inorderRecursive(),  // Compute recursive inorder.  
            inorderIterative: this.inorderIterative(),  // Compute iterative inorder.  
            postorderRecursive: this.postorderRecursive(),  // Compute recursive postorder.  
            postorderIterative: this.postorderIterative(),  // Compute iterative postorder.  
            levelOrder: this.levelOrder(),  // Compute level-order BFS.  
        };  // Close summary object.  
    }  // End summarize().  
}  // End BinaryTree.  

function formatDemo() {  // Format a small demo output using a sample tree.  
    const values = [1, 2, 3, 4, 5];  // Sample tree used across this repo for traversal examples.  
    const tree = BinaryTree.fromLevelOrder(values);  // Build tree from level-order list.  
    const s = tree.summarize();  // Summarize all traversal variants.  
    const lines = [];  // Accumulate printable lines.  
    lines.push("=== Tree Traversal Demo (JavaScript) ===");  // Print header.  
    lines.push(`level_order input: ${JSON.stringify(values)}`);  // Print input list.  
    lines.push(`preorder_recursive=${JSON.stringify(s.preorderRecursive)}`);  // Print recursive preorder.  
    lines.push(`preorder_iterative=${JSON.stringify(s.preorderIterative)}`);  // Print iterative preorder.  
    lines.push(`inorder_recursive=${JSON.stringify(s.inorderRecursive)}`);  // Print recursive inorder.  
    lines.push(`inorder_iterative=${JSON.stringify(s.inorderIterative)}`);  // Print iterative inorder.  
    lines.push(`postorder_recursive=${JSON.stringify(s.postorderRecursive)}`);  // Print recursive postorder.  
    lines.push(`postorder_iterative=${JSON.stringify(s.postorderIterative)}`);  // Print iterative postorder.  
    lines.push(`level_order=${JSON.stringify(s.levelOrder)}`);  // Print level-order BFS.  
    return lines.join("\n");  // Join lines into one printable string.  
}  // End formatDemo().  

function main() {  // CLI entry point for the demo.  
    console.log(formatDemo());  // Print demo output.  
}  // End main().  

if (require.main === module) {  // Run demo only when executed as a script.  
    main();  // Execute main.  
}  // Close entry-point guard.  

module.exports = {  // Export public API for tests and reuse.  
    Node,  // Export Node class.  
    BinaryTree,  // Export BinaryTree class.  
    formatDemo,  // Export demo formatter.  
};  // End exports.  

