// 02 二元搜尋樹示範（JavaScript）/ Binary Search Tree (BST) demo (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for safer, more explicit JavaScript.  

class Node {  // BST node (key + left child + right child).  
    constructor(key) {  // Initialize node with a key and empty children.  
        this.key = key;  // Store node key (mutable so delete can replace with successor key).  
        this.left = null;  // Store left child pointer (null means no child).  
        this.right = null;  // Store right child pointer (null means no child).  
    }  // Close constructor.  
}  // End Node.  

class BinarySearchTree {  // A BST wrapper that holds a root node.  
    constructor(root) {  // Initialize BST with an optional root.  
        this._root = root;  // Store root reference (null means empty tree).  
    }  // Close constructor.  

    get root() {  // Expose root for demos/tests (read-only semantics by convention).  
        return this._root;  // Return root reference.  
    }  // End root getter.  

    isEmpty() {  // Check whether the BST has no nodes.  
        return this._root === null;  // Empty iff root is null.  
    }  // End isEmpty().  

    static fromValues(values) {  // Build a BST by inserting values in order.  
        const t = new BinarySearchTree(null);  // Start from an empty tree.  
        for (const v of values) {  // Insert each value in the given order.  
            t.insert(Number(v));  // Insert one key (duplicates are ignored by policy).  
        }  // Close loop.  
        return t;  // Return built BST.  
    }  // End fromValues().  

    contains(key) {  // Return true if key exists in the BST.  
        let node = this._root;  // Start searching from the root.  
        while (node !== null) {  // Walk down until we hit a missing child.  
            if (key < node.key) {  // Go left when key is smaller.  
                node = node.left;  // Move to left child.  
            } else if (key > node.key) {  // Go right when key is larger.  
                node = node.right;  // Move to right child.  
            } else {  // key === node.key means we found it.  
                return true;  // Report found.  
            }  // Close compare branch.  
        }  // Close loop.  
        return false;  // Report not found when search falls off the tree.  
    }  // End contains().  

    insert(key) {  // Insert key into BST; return false when key already exists.  
        if (this._root === null) {  // Empty tree case: inserted node becomes the root.  
            this._root = new Node(key);  // Create root node.  
            return true;  // Report inserted.  
        }  // Close empty case.  

        let parent = null;  // Track parent so we can attach the new node.  
        let node = this._root;  // Start from root.  
        while (node !== null) {  // Walk down until we find an empty spot.  
            parent = node;  // Update parent before moving.  
            if (key < node.key) {  // Go left when key is smaller.  
                node = node.left;  // Move to left child.  
            } else if (key > node.key) {  // Go right when key is larger.  
                node = node.right;  // Move to right child.  
            } else {  // key === node.key means duplicate key.  
                return false;  // Reject duplicates by policy.  
            }  // Close compare branch.  
        }  // Close loop.  

        const newNode = new Node(key);  // Create the new leaf node.  
        if (key < parent.key) {  // Attach to the left side when key is smaller.  
            parent.left = newNode;  // Link as left child.  
        } else {  // Otherwise attach to the right side.  
            parent.right = newNode;  // Link as right child.  
        }  // Close attach branch.  
        return true;  // Report inserted.  
    }  // End insert().  

    delete(key) {  // Delete key from BST; return true when a node was removed.  
        const r = this._deleteSubtree(this._root, key);  // Delete from root.  
        this._root = r.node;  // Update root pointer (may change when deleting root).  
        return r.removed;  // Return whether deletion occurred.  
    }  // End delete().  

    _deleteSubtree(node, key) {  // Recursive delete that returns { node: newRoot, removed }.  
        if (node === null) {  // Key not found in an empty subtree.  
            return { node: null, removed: false };  // Nothing removed.  
        }  // Close base case.  

        if (key < node.key) {  // Delete from left subtree when key is smaller.  
            const r = this._deleteSubtree(node.left, key);  // Recurse into left child.  
            node.left = r.node;  // Update left child pointer.  
            return { node, removed: r.removed };  // Return updated subtree root.  
        }  // Close left branch.  
        if (key > node.key) {  // Delete from right subtree when key is larger.  
            const r = this._deleteSubtree(node.right, key);  // Recurse into right child.  
            node.right = r.node;  // Update right child pointer.  
            return { node, removed: r.removed };  // Return updated subtree root.  
        }  // Close right branch.  

        // Now key === node.key, so we found the node to delete.  // Explain match branch.  
        if (node.left === null && node.right === null) {  // Case 1: leaf node (0 children).  
            return { node: null, removed: true };  // Remove leaf by returning null.  
        }  // Close leaf case.  
        if (node.left === null) {  // Case 2a: only right child exists.  
            return { node: node.right, removed: true };  // Replace node by its right child.  
        }  // Close right-only case.  
        if (node.right === null) {  // Case 2b: only left child exists.  
            return { node: node.left, removed: true };  // Replace node by its left child.  
        }  // Close left-only case.  

        // Case 3: two children => replace with inorder successor (min of right subtree).  // Explain 2-child strategy.  
        const successorKey = this._minKey(node.right);  // Find successor key from right subtree.  
        node.key = successorKey;  // Copy successor key into current node.  
        const r = this._deleteSubtree(node.right, successorKey);  // Delete successor node from right subtree.  
        node.right = r.node;  // Update right child pointer after successor removal.  
        return { node, removed: true };  // Report successful deletion.  
    }  // End _deleteSubtree().  

    min() {  // Return minimum key, or null if tree is empty.  
        if (this._root === null) {  // Empty tree has no minimum.  
            return null;  // Return null for empty.  
        }  // Close empty case.  
        return this._minKey(this._root);  // Return leftmost key.  
    }  // End min().  

    _minKey(node) {  // Return the minimum key in a non-empty subtree.  
        let cur = node;  // Start at given node.  
        while (cur.left !== null) {  // Keep going left until we cannot.  
            cur = cur.left;  // Move to left child.  
        }  // Close loop.  
        return cur.key;  // Leftmost node has the minimum key.  
    }  // End _minKey().  

    max() {  // Return maximum key, or null if tree is empty.  
        if (this._root === null) {  // Empty tree has no maximum.  
            return null;  // Return null for empty.  
        }  // Close empty case.  
        let cur = this._root;  // Start at root.  
        while (cur.right !== null) {  // Keep going right until we cannot.  
            cur = cur.right;  // Move to right child.  
        }  // Close loop.  
        return cur.key;  // Rightmost node has the maximum key.  
    }  // End max().  

    size() {  // Return number of nodes in the BST (O(n)).  
        const sizeOf = (n) => {  // Recursive helper that counts nodes in a subtree.  
            if (n === null) {  // Empty subtree contributes 0 nodes.  
                return 0;  // Return 0 for empty.  
            }  // Close base case.  
            return 1 + sizeOf(n.left) + sizeOf(n.right);  // Count root + left + right.  
        };  // Close helper.  
        return sizeOf(this._root);  // Count nodes starting from root.  
    }  // End size().  

    height() {  // Return height measured in edges (empty=-1, leaf=0).  
        const heightOf = (n) => {  // Recursive helper that computes height in edges.  
            if (n === null) {  // Empty subtree has height -1.  
                return -1;  // Return -1 for empty.  
            }  // Close base case.  
            return 1 + Math.max(heightOf(n.left), heightOf(n.right));  // Height is 1 + max(child heights).  
        };  // Close helper.  
        return heightOf(this._root);  // Compute height from root.  
    }  // End height().  

    inorder() {  // Return inorder traversal (sorted for a valid BST).  
        const out = [];  // Accumulate traversal result.  
        const walk = (n) => {  // Recursive helper for inorder traversal.  
            if (n === null) {  // Empty subtree contributes nothing.  
                return;  // Stop recursion.  
            }  // Close base case.  
            walk(n.left);  // Visit left subtree first.  
            out.push(n.key);  // Visit root.  
            walk(n.right);  // Visit right subtree.  
        };  // Close helper.  
        walk(this._root);  // Walk from root.  
        return out;  // Return traversal list.  
    }  // End inorder().  

    summarize() {  // Build a snapshot summary for demos/tests.  
        return {  // Return a compact summary object.  
            size: this.size(),  // Compute node count.  
            height: this.height(),  // Compute height (edges).  
            minValue: this.min(),  // Compute minimum key (or null).  
            maxValue: this.max(),  // Compute maximum key (or null).  
            inorder: this.inorder(),  // Compute inorder traversal.  
        };  // Close summary object.  
    }  // End summarize().  
}  // End BinarySearchTree.  

function formatDemo() {  // Format a small demo output using a classic BST example.  
    const values = [8, 3, 10, 1, 6, 14, 4, 7, 13];  // Classic BST insert order used in many textbooks.  
    const tree = BinarySearchTree.fromValues(values);  // Build BST by inserting in order.  
    const s = tree.summarize();  // Summarize properties and inorder traversal.  
    const lines = [];  // Accumulate printable lines.  
    lines.push("=== Binary Search Tree Demo (JavaScript) ===");  // Print header.  
    lines.push(`insert order: ${JSON.stringify(values)}`);  // Print insertion order.  
    lines.push(`size=${s.size}, height=${s.height}, min=${s.minValue}, max=${s.maxValue}`);  // Print key properties.  
    lines.push(`inorder(sorted)=${JSON.stringify(s.inorder)}`);  // Print inorder traversal.  
    lines.push(`contains(6)=${tree.contains(6)}, contains(99)=${tree.contains(99)}`);  // Print search examples.  
    tree.delete(7);  // Delete a leaf (7).  
    tree.delete(14);  // Delete a one-child node (14 has left child 13).  
    tree.delete(3);  // Delete a two-child node (3) using successor replacement.  
    lines.push(`after deletes (7,14,3) inorder=${JSON.stringify(tree.inorder())}`);  // Show inorder after deletions.  
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
    BinarySearchTree,  // Export BinarySearchTree class.  
    formatDemo,  // Export demo formatter.  
};  // End exports.  

