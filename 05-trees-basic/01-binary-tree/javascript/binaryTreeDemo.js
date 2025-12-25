// 01 二元樹示範（JavaScript）/ Binary tree demo (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for safer, more explicit JavaScript.  

class Node {  // Binary tree node (value + left child + right child).  
    constructor(value) {  // Initialize node with a value and empty children.  
        this.value = value;  // Store node value.  
        this.left = null;  // Store left child pointer (null means no child).  
        this.right = null;  // Store right child pointer (null means no child).  
    }  // Close constructor.  
}  // End Node.  

class BinaryTree {  // A binary tree wrapper that holds a root node.  
    constructor(root) {  // Initialize tree with an optional root.  
        this._root = root;  // Store root reference (null means empty tree).  
    }  // Close constructor.  

    get root() {  // Expose the root for demos/tests (read-only semantics by convention).  
        return this._root;  // Return root reference.  
    }  // End root getter.  

    isEmpty() {  // Check whether the tree has no nodes.  
        return this._root === null;  // Empty iff root is null.  
    }  // End isEmpty().  

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
            if (node === null) {  // Skip missing nodes because they have no children.  
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

    size() {  // Return number of nodes in the tree (O(n)).  
        const sizeOf = (node) => {  // Recursive helper that counts nodes in a subtree.  
            if (node === null) {  // Empty subtree contributes 0 nodes.  
                return 0;  // Return 0 for empty.  
            }  // Close base case.  
            return 1 + sizeOf(node.left) + sizeOf(node.right);  // Count root + left + right.  
        };  // Close helper.  
        return sizeOf(this._root);  // Count nodes starting from root.  
    }  // End size().  

    height() {  // Return tree height measured in edges (empty=-1, leaf=0).  
        const heightOf = (node) => {  // Recursive helper that computes height in edges.  
            if (node === null) {  // Empty subtree has height -1 (so leaf has height 0).  
                return -1;  // Return -1 for empty.  
            }  // Close base case.  
            return 1 + Math.max(heightOf(node.left), heightOf(node.right));  // Height is 1 + max(child heights).  
        };  // Close helper.  
        return heightOf(this._root);  // Compute height from root.  
    }  // End height().  

    countLeaves() {  // Return number of leaf nodes (O(n)).  
        const leavesOf = (node) => {  // Recursive helper that counts leaf nodes in a subtree.  
            if (node === null) {  // Empty subtree contributes 0 leaves.  
                return 0;  // Return 0 for empty.  
            }  // Close base case.  
            if (node.left === null && node.right === null) {  // A node with no children is a leaf.  
                return 1;  // Count this leaf.  
            }  // Close leaf case.  
            return leavesOf(node.left) + leavesOf(node.right);  // Sum leaves from both subtrees.  
        };  // Close helper.  
        return leavesOf(this._root);  // Count leaves from root.  
    }  // End countLeaves().  

    preorder() {  // Return preorder traversal as an array of values.  
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
    }  // End preorder().  

    inorder() {  // Return inorder traversal as an array of values.  
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
    }  // End inorder().  

    postorder() {  // Return postorder traversal as an array of values.  
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
    }  // End postorder().  

    levelOrder() {  // Return level-order traversal (BFS) as an array of values.  
        if (this._root === null) {  // Empty tree has empty traversal.  
            return [];  // Return empty array.  
        }  // Close empty case.  

        const queue = [this._root];  // Use an array as a queue storage.  
        let head = 0;  // Maintain a head index so dequeue is O(1) without shifting.  
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
            size: this.size(),  // Compute node count.  
            height: this.height(),  // Compute height (edges).  
            leaves: this.countLeaves(),  // Compute leaf count.  
            preorder: this.preorder(),  // Compute preorder traversal.  
            inorder: this.inorder(),  // Compute inorder traversal.  
            postorder: this.postorder(),  // Compute postorder traversal.  
            levelOrder: this.levelOrder(),  // Compute level-order traversal.  
        };  // Close summary object.  
    }  // End summarize().  
}  // End BinaryTree.  

function formatDemo() {  // Format a small demo output using a sample tree.  
    const values = [1, 2, 3, 4, 5];  // Build sample tree: 1 with children 2 and 3; 2 has 4 and 5.  
    const tree = BinaryTree.fromLevelOrder(values);  // Build tree from level-order list.  
    const s = tree.summarize();  // Summarize properties and traversals.  
    const lines = [];  // Accumulate printable lines.  
    lines.push("=== Binary Tree Demo (JavaScript) ===");  // Print header.  
    lines.push(`level_order input: ${JSON.stringify(values)}`);  // Print input array.  
    lines.push(`size=${s.size}, height=${s.height}, leaves=${s.leaves}`);  // Print key properties.  
    lines.push(`preorder=${JSON.stringify(s.preorder)}`);  // Print preorder list.  
    lines.push(`inorder=${JSON.stringify(s.inorder)}`);  // Print inorder list.  
    lines.push(`postorder=${JSON.stringify(s.postorder)}`);  // Print postorder list.  
    lines.push(`level_order=${JSON.stringify(s.levelOrder)}`);  // Print BFS list.  
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

