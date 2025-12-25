// 01 AVL Tree demo (JavaScript) / AVL Tree 示範（JavaScript）.  // Bilingual file header.

"use strict";  // Enable strict mode for clearer JavaScript semantics.  

class Node {  // AVL node (key + left child + right child + cached height).  
    constructor(key) {  // Initialize node with a key and empty children.  
        this.key = key;  // Store node key.  
        this.left = null;  // Store left child pointer (null means no child).  
        this.right = null;  // Store right child pointer (null means no child).  
        this.height = 0;  // Store cached height in edges (leaf=0 by definition).  
    }  // Close constructor.  
}  // End Node.  

function h(node) {  // Return cached height for node, or -1 for empty subtree.  
    return node === null ? -1 : node.height;  // Apply repo-wide height convention.  
}  // End h().  

function updateHeight(node) {  // Recompute node.height from children heights.  
    node.height = 1 + Math.max(h(node.left), h(node.right));  // Height in edges is 1 + max(child heights).  
}  // End updateHeight().  

function balanceFactor(node) {  // Compute balance factor = height(left) - height(right).  
    return h(node.left) - h(node.right);  // Positive means left-heavy; negative means right-heavy.  
}  // End balanceFactor().  

function rotateRight(y) {  // Right rotation to fix a left-heavy imbalance.  
    const x = y.left;  // Take left child as pivot.  
    const t2 = x.right;  // Keep pivot's right subtree (will become y's left subtree).  

    x.right = y;  // Put y as the right child of x.  
    y.left = t2;  // Attach T2 as y's left child.  

    updateHeight(y);  // Update y first because its children changed.  
    updateHeight(x);  // Update x after y because x now points to y.  
    return x;  // Return new subtree root.  
}  // End rotateRight().  

function rotateLeft(x) {  // Left rotation to fix a right-heavy imbalance.  
    const y = x.right;  // Take right child as pivot.  
    const t2 = y.left;  // Keep pivot's left subtree (will become x's right subtree).  

    y.left = x;  // Put x as the left child of y.  
    x.right = t2;  // Attach T2 as x's right child.  

    updateHeight(x);  // Update x first because its children changed.  
    updateHeight(y);  // Update y after x because y now points to x.  
    return y;  // Return new subtree root.  
}  // End rotateLeft().  

function rebalance(node) {  // Rebalance node if it violates AVL balance constraints.  
    const balance = balanceFactor(node);  // Compute current node's balance factor.  

    if (balance > 1) {  // Left-heavy subtree; need either LL or LR rotation(s).  
        if (balanceFactor(node.left) >= 0) {  // LL case: left child is not right-heavy.  
            return rotateRight(node);  // Single right rotation fixes LL.  
        }  // Close LL branch.  
        node.left = rotateLeft(node.left);  // LR case step 1: rotate left on left child.  
        return rotateRight(node);  // LR case step 2: rotate right on current node.  
    }  // Close left-heavy branch.  

    if (balance < -1) {  // Right-heavy subtree; need either RR or RL rotation(s).  
        if (balanceFactor(node.right) <= 0) {  // RR case: right child is not left-heavy.  
            return rotateLeft(node);  // Single left rotation fixes RR.  
        }  // Close RR branch.  
        node.right = rotateRight(node.right);  // RL case step 1: rotate right on right child.  
        return rotateLeft(node);  // RL case step 2: rotate left on current node.  
    }  // Close right-heavy branch.  

    return node;  // Already balanced; return node unchanged.  
}  // End rebalance().  

function minNode(node) {  // Return the leftmost (minimum-key) node in a non-empty subtree.  
    let cur = node;  // Start at given node.  
    while (cur.left !== null) {  // Keep going left until we cannot.  
        cur = cur.left;  // Move to left child.  
    }  // Close loop.  
    return cur;  // Leftmost node has the minimum key.  
}  // End minNode().  

function insertSubtree(node, key) {  // Insert into subtree and rebalance; return { node, inserted }.  
    if (node === null) {  // Empty spot: create a new leaf node.  
        return { node: new Node(key), inserted: true };  // Return new node and inserted=true.  
    }  // Close base case.  

    let inserted = false;  // Track insertion flag from recursion.  
    if (key < node.key) {  // Insert into left subtree when key is smaller.  
        const r = insertSubtree(node.left, key);  // Recurse into left child slot.  
        node.left = r.node;  // Re-attach possibly-rotated left subtree.  
        inserted = r.inserted;  // Propagate inserted flag.  
    } else if (key > node.key) {  // Insert into right subtree when key is larger.  
        const r = insertSubtree(node.right, key);  // Recurse into right child slot.  
        node.right = r.node;  // Re-attach possibly-rotated right subtree.  
        inserted = r.inserted;  // Propagate inserted flag.  
    } else {  // key === node.key means duplicate key.  
        return { node: node, inserted: false };  // Reject duplicates by policy (no changes).  
    }  // Close compare branch.  

    updateHeight(node);  // Update cached height after insertion below.  
    const newRoot = rebalance(node);  // Rebalance on the way back up.  
    return { node: newRoot, inserted: inserted };  // Return possibly new root and insertion flag.  
}  // End insertSubtree().  

function deleteSubtree(node, key) {  // Delete from subtree and rebalance; return { node, removed }.  
    if (node === null) {  // Key not found in an empty subtree.  
        return { node: null, removed: false };  // Nothing removed.  
    }  // Close base case.  

    let removed = false;  // Track whether a node was removed in this subtree.  
    if (key < node.key) {  // Delete from left subtree when key is smaller.  
        const r = deleteSubtree(node.left, key);  // Recurse into left child.  
        node.left = r.node;  // Re-attach possibly-rotated left subtree.  
        removed = r.removed;  // Propagate removed flag.  
    } else if (key > node.key) {  // Delete from right subtree when key is larger.  
        const r = deleteSubtree(node.right, key);  // Recurse into right child.  
        node.right = r.node;  // Re-attach possibly-rotated right subtree.  
        removed = r.removed;  // Propagate removed flag.  
    } else {  // key === node.key means we found the node to delete.  
        removed = true;  // Mark that we are removing a node.  

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
        const successor = minNode(node.right);  // Find successor node from right subtree.  
        node.key = successor.key;  // Copy successor key into current node (keeps tree shape).  
        const rr = deleteSubtree(node.right, successor.key);  // Delete successor node from right subtree.  
        node.right = rr.node;  // Re-attach right subtree after successor removal.  
    }  // Close match branch.  

    updateHeight(node);  // Update cached height after deletion below.  
    const newRoot = rebalance(node);  // Rebalance on the way back up.  
    return { node: newRoot, removed: removed };  // Return possibly new root and removed flag.  
}  // End deleteSubtree().  

function inorderWalk(node, out) {  // Recursive inorder traversal (left, root, right).  
    if (node === null) {  // Empty subtree contributes nothing.  
        return;  // Stop recursion.  
    }  // Close base case.  
    inorderWalk(node.left, out);  // Visit left subtree first.  
    out.push(node.key);  // Visit root.  
    inorderWalk(node.right, out);  // Visit right subtree.  
}  // End inorderWalk().  

function sizeOf(node) {  // Recursive helper that counts nodes in a subtree.  
    if (node === null) {  // Empty subtree contributes 0 nodes.  
        return 0;  // Return 0 for empty.  
    }  // Close base case.  
    return 1 + sizeOf(node.left) + sizeOf(node.right);  // Count root + left + right.  
}  // End sizeOf().  

function validateSubtree(node, low, high) {  // Validate subtree; return { ok, computedHeight }.  
    if (node === null) {  // Empty subtree is valid and has height -1 by definition.  
        return { ok: true, computedHeight: -1 };  // Return valid with height -1.  
    }  // Close base case.  

    if (low !== null && node.key <= low) {  // Enforce strict lower bound (no duplicates).  
        return { ok: false, computedHeight: 0 };  // Report invalid (height value is irrelevant on failure).  
    }  // Close lower-bound check.  
    if (high !== null && node.key >= high) {  // Enforce strict upper bound (no duplicates).  
        return { ok: false, computedHeight: 0 };  // Report invalid (height value is irrelevant on failure).  
    }  // Close upper-bound check.  

    const left = validateSubtree(node.left, low, node.key);  // Validate left subtree with updated upper bound.  
    if (!left.ok) {  // Early return on left failure to keep logic simple.  
        return { ok: false, computedHeight: 0 };  // Propagate failure.  
    }  // Close left-failure branch.  
    const right = validateSubtree(node.right, node.key, high);  // Validate right subtree with updated lower bound.  
    if (!right.ok) {  // Early return on right failure to keep logic simple.  
        return { ok: false, computedHeight: 0 };  // Propagate failure.  
    }  // Close right-failure branch.  

    const computed = 1 + Math.max(left.computedHeight, right.computedHeight);  // Compute expected height from children heights.  
    if (node.height !== computed) {  // Cached height must match computed height.  
        return { ok: false, computedHeight: computed };  // Report height mismatch.  
    }  // Close height check.  

    if (Math.abs(left.computedHeight - right.computedHeight) > 1) {  // AVL balance constraint requires |bf| <= 1.  
        return { ok: false, computedHeight: computed };  // Report imbalance.  
    }  // Close balance check.  

    return { ok: true, computedHeight: computed };  // Report valid subtree with computed height.  
}  // End validateSubtree().  

class AvlTree {  // AVL tree wrapper that holds a root node.  
    constructor(root = null) {  // Initialize AVL tree with an optional root.  
        this._root = root;  // Store root reference (null means empty tree).  
    }  // Close constructor.  

    get root() {  // Expose root for demos/tests (read-only semantics by convention).  
        return this._root;  // Return root reference.  
    }  // Close getter.  

    static fromValues(values) {  // Build an AVL tree by inserting values in order.  
        const t = new AvlTree(null);  // Start from an empty tree.  
        for (const v of values) {  // Insert each value in the given order.  
            t.insert(Number(v));  // Insert one key (duplicates are rejected by policy).  
        }  // Close loop.  
        return t;  // Return built AVL tree.  
    }  // End fromValues().  

    isEmpty() {  // Check whether the tree has no nodes.  
        return this._root === null;  // Empty iff root is null.  
    }  // End isEmpty().  

    height() {  // Return tree height measured in edges (empty=-1, leaf=0).  
        return this._root === null ? -1 : this._root.height;  // Use cached root height (or -1 for empty).  
    }  // End height().  

    size() {  // Return number of nodes in the tree (O(n)).  
        return sizeOf(this._root);  // Count nodes starting from root.  
    }  // End size().  

    contains(key) {  // Return true if key exists in the tree.  
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

    insert(key) {  // Insert key; return false when key already exists.  
        const r = insertSubtree(this._root, key);  // Insert into root subtree.  
        this._root = r.node;  // Update root pointer (rotations may change root).  
        return r.inserted;  // Return whether insertion happened.  
    }  // End insert().  

    delete(key) {  // Delete key; return true when a node was removed.  
        const r = deleteSubtree(this._root, key);  // Delete from root subtree.  
        this._root = r.node;  // Update root pointer (rotations may change root).  
        return r.removed;  // Return whether deletion happened.  
    }  // End delete().  

    min() {  // Return minimum key, or null if tree is empty.  
        if (this._root === null) {  // Empty tree has no minimum.  
            return null;  // Return null for empty.  
        }  // Close empty case.  
        return minNode(this._root).key;  // Return leftmost key.  
    }  // End min().  

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

    inorder() {  // Return inorder traversal (sorted for a valid BST).  
        const out = [];  // Accumulate traversal result.  
        inorderWalk(this._root, out);  // Walk from root.  
        return out;  // Return traversal array.  
    }  // End inorder().  

    validate() {  // Validate BST ordering + AVL balance + cached height correctness.  
        return validateSubtree(this._root, null, null).ok;  // Validate from root with open bounds.  
    }  // End validate().  

    summarize() {  // Build a snapshot summary for demos/tests.  
        return {  // Return a plain object for easy printing.  
            size: this.size(),  // Compute node count.  
            height: this.height(),  // Read tree height (edges).  
            minValue: this.min(),  // Compute minimum key (nullable).  
            maxValue: this.max(),  // Compute maximum key (nullable).  
            inorder: this.inorder(),  // Compute inorder traversal.  
            isValid: this.validate(),  // Validate AVL invariants.  
        };  // Close summary object.  
    }  // End summarize().  
}  // End AvlTree.  

function formatDemo() {  // Format a small demo output using a classic insert sequence.  
    const values = [8, 3, 10, 1, 6, 14, 4, 7, 13];  // Classic textbook insert order.  
    const t = AvlTree.fromValues(values);  // Build AVL tree by inserting in order.  
    const s = t.summarize();  // Summarize properties and inorder traversal.  

    const lines = [];  // Accumulate printable lines.  
    lines.push("=== AVL Tree Demo (JavaScript) ===");  // Print header.  
    lines.push("insert order: " + JSON.stringify(values));  // Print insertion order.  
    lines.push(`size=${s.size}, height=${s.height}, min=${s.minValue}, max=${s.maxValue}, valid=${s.isValid}`);  // Print key properties.  
    lines.push("inorder(sorted)=" + JSON.stringify(s.inorder));  // Print inorder traversal (should be sorted).  
    lines.push(`contains(6)=${t.contains(6)}, contains(99)=${t.contains(99)}`);  // Print search examples.  

    t.delete(7);  // Delete a leaf (7) to show delete on AVL.  
    t.delete(14);  // Delete a one-child node (14 has left child 13).  
    t.delete(3);  // Delete a two-child node (3) and rebalance.  
    lines.push("after deletes (7,14,3) inorder=" + JSON.stringify(t.inorder()) + ", valid=" + t.validate());  // Show results after deletions.  
    return lines.join("\n");  // Join lines into one printable string.  
}  // End formatDemo().  

if (require.main === module) {  // Run demo only when executed as a script.  
    console.log(formatDemo());  // Print demo output.  
}  // Close main guard.  

module.exports = {  // Export public API for tests.  
    AvlTree,  // Export AvlTree wrapper.  
};  // End exports.  

