// 02 Red-Black Tree demo (JavaScript, LLRB) / 紅黑樹示範（JavaScript, LLRB）.  // Bilingual file header.

"use strict";  // Enable strict mode for clearer JavaScript semantics.  

const RED = true;  // Represent red nodes/links with true for readability.  
const BLACK = false;  // Represent black nodes/links with false for readability.  

class Node {  // LLRB node (key + left child + right child + color).  
    constructor(key, red) {  // Initialize node with a key, color, and empty children.  
        this.key = key;  // Store node key.  
        this.left = null;  // Store left child pointer (null means no child).  
        this.right = null;  // Store right child pointer (null means no child).  
        this.red = red;  // Store node color (true=RED, false=BLACK).  
    }  // Close constructor.  
}  // End Node.  

function isRed(node) {  // Return true when node is red; null is treated as black.  
    return node !== null && node.red === RED;  // Null links are black by definition.  
}  // End isRed().  

function rotateLeft(h) {  // Rotate left to eliminate a right-leaning red link.  
    const x = h.right;  // Take right child as pivot (must exist).  
    h.right = x.left;  // Move pivot's left subtree to h's right slot.  
    x.left = h;  // Put h as pivot's left child.  
    x.red = h.red;  // Preserve original color on the new root.  
    h.red = RED;  // Mark the moved-down node as red.  
    return x;  // Return new subtree root.  
}  // End rotateLeft().  

function rotateRight(h) {  // Rotate right to fix two consecutive left-leaning red links.  
    const x = h.left;  // Take left child as pivot (must exist).  
    h.left = x.right;  // Move pivot's right subtree to h's left slot.  
    x.right = h;  // Put h as pivot's right child.  
    x.red = h.red;  // Preserve original color on the new root.  
    h.red = RED;  // Mark the moved-down node as red.  
    return x;  // Return new subtree root.  
}  // End rotateRight().  

function flipColors(h) {  // Flip colors to split or merge 4-nodes (LLRB <-> 2-3 tree).  
    h.red = !h.red;  // Toggle parent color.  
    if (h.left !== null) {  // Guard for safety (children should exist in normal LLRB usage).  
        h.left.red = !h.left.red;  // Toggle left child color.  
    }  // Close left guard.  
    if (h.right !== null) {  // Guard for safety (children should exist in normal LLRB usage).  
        h.right.red = !h.right.red;  // Toggle right child color.  
    }  // Close right guard.  
}  // End flipColors().  

function fixUp(h) {  // Restore LLRB invariants on the way back up after insert/delete.  
    if (isRed(h.right) && !isRed(h.left)) {  // Fix right-leaning red link.  
        h = rotateLeft(h);  // Rotate left to make red link lean left.  
    }  // Close right-red fix.  
    if (isRed(h.left) && isRed(h.left.left)) {  // Fix two consecutive left reds.  
        h = rotateRight(h);  // Rotate right to balance the 4-node shape.  
    }  // Close left-left fix.  
    if (isRed(h.left) && isRed(h.right)) {  // Split 4-node when both children are red.  
        flipColors(h);  // Flip colors to push red up one level.  
    }  // Close split case.  
    return h;  // Return possibly rotated/flipped subtree root.  
}  // End fixUp().  

function moveRedLeft(h) {  // Ensure h.left is red when we need to delete from the left side.  
    flipColors(h);  // Push red down so we can borrow from the right side.  
    if (h.right !== null && isRed(h.right.left)) {  // Borrow from right-left red link when available.  
        h.right = rotateRight(h.right);  // Rotate right on right child to create a right-leaning red link.  
        h = rotateLeft(h);  // Rotate left on h to move red link to the left side.  
        flipColors(h);  // Re-flip to restore colors after rotations.  
    }  // Close borrow case.  
    return h;  // Return adjusted subtree root.  
}  // End moveRedLeft().  

function moveRedRight(h) {  // Ensure h.right is red when we need to delete from the right side.  
    flipColors(h);  // Push red down so we can borrow from the left side.  
    if (h.left !== null && isRed(h.left.left)) {  // Borrow from left-left red link when available.  
        h = rotateRight(h);  // Rotate right to move red link to the right side.  
        flipColors(h);  // Re-flip to restore colors after rotation.  
    }  // Close borrow case.  
    return h;  // Return adjusted subtree root.  
}  // End moveRedRight().  

function minNode(node) {  // Return the leftmost (minimum-key) node in a non-empty subtree.  
    let cur = node;  // Start at given node.  
    while (cur.left !== null) {  // Keep going left until we cannot.  
        cur = cur.left;  // Move to left child.  
    }  // Close loop.  
    return cur;  // Leftmost node has the minimum key.  
}  // End minNode().  

function insertSubtree(h, key) {  // Insert key into subtree and rebalance; return { node, inserted }.  
    if (h === null) {  // Empty spot: create a new red leaf node.  
        return { node: new Node(key, RED), inserted: true };  // Return new node and inserted=true.  
    }  // Close base case.  

    let inserted = false;  // Track insertion flag from recursion.  
    if (key < h.key) {  // Insert into left subtree when key is smaller.  
        const r = insertSubtree(h.left, key);  // Recurse into left child slot.  
        h.left = r.node;  // Re-attach possibly rebalanced left subtree.  
        inserted = r.inserted;  // Propagate inserted flag.  
    } else if (key > h.key) {  // Insert into right subtree when key is larger.  
        const r = insertSubtree(h.right, key);  // Recurse into right child slot.  
        h.right = r.node;  // Re-attach possibly rebalanced right subtree.  
        inserted = r.inserted;  // Propagate inserted flag.  
    } else {  // key === h.key means duplicate key.  
        return { node: h, inserted: false };  // Reject duplicates by policy (no changes).  
    }  // Close compare branch.  

    h = fixUp(h);  // Restore invariants after insertion below.  
    return { node: h, inserted: inserted };  // Return possibly rebalanced subtree root and insertion flag.  
}  // End insertSubtree().  

function deleteMin(h) {  // Delete the minimum node in this subtree and return new subtree root.  
    if (h.left === null) {  // Reached the minimum node (no left child).  
        return null;  // Delete this node by returning null.  
    }  // Close base case.  

    if (!isRed(h.left) && !isRed(h.left.left)) {  // Ensure we do not descend into a 2-node.  
        h = moveRedLeft(h);  // Move a red link to the left side.  
    }  // Close move case.  

    h.left = deleteMin(h.left);  // Recurse into left subtree.  
    return fixUp(h);  // Fix up invariants on the way back up.  
}  // End deleteMin().  

function deleteSubtree(h, key) {  // Delete key from subtree (assumes key exists).  
    if (key < h.key) {  // Key is in the left subtree.  
        if (h.left !== null) {  // Only recurse when left subtree exists.  
            if (!isRed(h.left) && !isRed(h.left.left)) {  // Avoid descending into a 2-node.  
                h = moveRedLeft(h);  // Move a red link to the left side.  
            }  // Close move case.  
            h.left = deleteSubtree(h.left, key);  // Recurse into left subtree.  
        }  // Close left guard.  
    } else {  // Key is in the current node or right subtree.  
        if (isRed(h.left)) {  // Rotate right to maintain delete preconditions.  
            h = rotateRight(h);  // Rotate right to make sure we can move red right when needed.  
        }  // Close rotate-right pre-step.  

        if (key === h.key && h.right === null) {  // Case: match and no right child => can delete this node.  
            return null;  // Delete node by returning null.  
        }  // Close leaf-right-null case.  

        if (h.right !== null) {  // Only work on right side when it exists.  
            if (!isRed(h.right) && !isRed(h.right.left)) {  // Avoid descending into a 2-node.  
                h = moveRedRight(h);  // Move a red link to the right side.  
            }  // Close move case.  
            if (key === h.key) {  // Match case with a right subtree => replace with successor.  
                const successor = minNode(h.right);  // Find successor node from right subtree.  
                h.key = successor.key;  // Copy successor key into current node.  
                h.right = deleteMin(h.right);  // Delete successor node from right subtree.  
            } else {  // Otherwise delete in the right subtree.  
                h.right = deleteSubtree(h.right, key);  // Recurse into right subtree.  
            }  // Close match/descend branch.  
        }  // Close right guard.  
    }  // Close key-side branch.  

    return fixUp(h);  // Fix up invariants on the way back up.  
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

function heightOf(node) {  // Recursive helper that computes height in edges.  
    if (node === null) {  // Empty subtree has height -1.  
        return -1;  // Return -1 for empty.  
    }  // Close base case.  
    return 1 + Math.max(heightOf(node.left), heightOf(node.right));  // Height is 1 + max(child heights).  
}  // End heightOf().  

function validateSubtree(node, low, high) {  // Validate subtree; return { ok, blackHeight }.  
    if (node === null) {  // Null links are black and contribute a base black-height of 1.  
        return { ok: true, blackHeight: 1 };  // Treat null as a black leaf sentinel.  
    }  // Close base case.  

    if (low !== null && node.key <= low) {  // Enforce strict lower bound (no duplicates).  
        return { ok: false, blackHeight: 0 };  // Report invalid ordering.  
    }  // Close lower-bound check.  
    if (high !== null && node.key >= high) {  // Enforce strict upper bound (no duplicates).  
        return { ok: false, blackHeight: 0 };  // Report invalid ordering.  
    }  // Close upper-bound check.  

    if (isRed(node.right)) {  // LLRB rule: red links must lean left (no red right child).  
        return { ok: false, blackHeight: 0 };  // Report invalid right-leaning red link.  
    }  // Close right-red check.  

    if (isRed(node) && (isRed(node.left) || isRed(node.right))) {  // RB rule: no two consecutive reds.  
        return { ok: false, blackHeight: 0 };  // Report invalid consecutive red links.  
    }  // Close consecutive-red check.  

    const left = validateSubtree(node.left, low, node.key);  // Validate left subtree with updated upper bound.  
    if (!left.ok) {  // Early return on left failure to keep logic simple.  
        return { ok: false, blackHeight: 0 };  // Propagate failure.  
    }  // Close left-failure branch.  
    const right = validateSubtree(node.right, node.key, high);  // Validate right subtree with updated lower bound.  
    if (!right.ok) {  // Early return on right failure to keep logic simple.  
        return { ok: false, blackHeight: 0 };  // Propagate failure.  
    }  // Close right-failure branch.  

    if (left.blackHeight !== right.blackHeight) {  // RB rule: all root-to-leaf paths have the same number of black nodes.  
        return { ok: false, blackHeight: 0 };  // Report black-height mismatch.  
    }  // Close black-height check.  

    const blackHere = isRed(node) ? 0 : 1;  // Count black nodes; red nodes do not add to black-height.  
    return { ok: true, blackHeight: left.blackHeight + blackHere };  // Return ok with computed black-height.  
}  // End validateSubtree().  

class RedBlackTree {  // Red-black tree wrapper (implemented as Left-Leaning Red-Black, LLRB).  
    constructor(root = null) {  // Initialize tree with an optional root.  
        this._root = root;  // Store root reference (null means empty tree).  
    }  // Close constructor.  

    get root() {  // Expose root for demos/tests (read-only semantics by convention).  
        return this._root;  // Return root reference.  
    }  // Close getter.  

    static fromValues(values) {  // Build a tree by inserting values in order.  
        const t = new RedBlackTree(null);  // Start from an empty tree.  
        for (const v of values) {  // Insert each value in the given order.  
            t.insert(Number(v));  // Insert one key (duplicates are rejected by policy).  
        }  // Close loop.  
        return t;  // Return built tree.  
    }  // End fromValues().  

    isEmpty() {  // Check whether the tree has no nodes.  
        return this._root === null;  // Empty iff root is null.  
    }  // End isEmpty().  

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
        this._root = r.node;  // Update root pointer.  
        if (this._root !== null) {  // Guard for empty tree.  
            this._root.red = BLACK;  // Root must always be black.  
        }  // Close root-color guard.  
        return r.inserted;  // Return whether insertion happened.  
    }  // End insert().  

    delete(key) {  // Delete key; return true when a node was removed.  
        if (this._root === null) {  // Empty tree cannot delete anything.  
            return false;  // Report not removed.  
        }  // Close empty case.  
        if (!this.contains(key)) {  // Avoid running LLRB delete when key is missing.  
            return false;  // Report not removed.  
        }  // Close missing-key guard.  

        if (!isRed(this._root.left) && !isRed(this._root.right)) {  // Detect 2-node root situation.  
            this._root.red = RED;  // Temporarily color root red to simplify deletion logic.  
        }  // Close pre-step.  

        this._root = deleteSubtree(this._root, key);  // Delete key from root subtree (guaranteed to exist).  
        if (this._root !== null) {  // Restore invariant when tree is non-empty.  
            this._root.red = BLACK;  // Root must be black.  
        }  // Close root-color guard.  
        return true;  // Report removed.  
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

    size() {  // Return number of nodes in the tree (O(n)).  
        return sizeOf(this._root);  // Count nodes starting from root.  
    }  // End size().  

    height() {  // Return height measured in edges (empty=-1, leaf=0).  
        return heightOf(this._root);  // Compute height from root.  
    }  // End height().  

    inorder() {  // Return inorder traversal (sorted for a valid BST).  
        const out = [];  // Accumulate traversal result.  
        inorderWalk(this._root, out);  // Walk from root.  
        return out;  // Return traversal array.  
    }  // End inorder().  

    validate() {  // Validate BST ordering + red-black properties (LLRB variant).  
        if (this._root === null) {  // Empty tree is valid.  
            return true;  // Vacuously valid.  
        }  // Close empty case.  
        if (this._root.red) {  // Root must be black.  
            return false;  // Report invalid root color.  
        }  // Close root check.  
        return validateSubtree(this._root, null, null).ok;  // Validate from root with open bounds.  
    }  // End validate().  

    summarize() {  // Build a snapshot summary for demos/tests.  
        return {  // Return a plain object for easy printing.  
            size: this.size(),  // Compute node count.  
            height: this.height(),  // Compute height (edges).  
            minValue: this.min(),  // Compute minimum key (nullable).  
            maxValue: this.max(),  // Compute maximum key (nullable).  
            inorder: this.inorder(),  // Compute inorder traversal.  
            isValid: this.validate(),  // Validate red-black invariants.  
        };  // Close summary object.  
    }  // End summarize().  
}  // End RedBlackTree.  

function formatDemo() {  // Format a small demo output using a classic insert sequence.  
    const values = [8, 3, 10, 1, 6, 14, 4, 7, 13];  // Classic textbook insert order.  
    const t = RedBlackTree.fromValues(values);  // Build tree by inserting in order.  
    const s = t.summarize();  // Summarize properties and inorder traversal.  

    const lines = [];  // Accumulate printable lines.  
    lines.push("=== Red-Black Tree Demo (JavaScript, LLRB) ===");  // Print header.  
    lines.push("insert order: " + JSON.stringify(values));  // Print insertion order.  
    lines.push(`size=${s.size}, height=${s.height}, min=${s.minValue}, max=${s.maxValue}, valid=${s.isValid}`);  // Print key properties.  
    lines.push("inorder(sorted)=" + JSON.stringify(s.inorder));  // Print inorder traversal (should be sorted).  
    lines.push(`contains(6)=${t.contains(6)}, contains(99)=${t.contains(99)}`);  // Print search examples.  

    t.delete(7);  // Delete a leaf (7).  
    t.delete(14);  // Delete a one-child node (14 has left child 13 in this data set).  
    t.delete(3);  // Delete a two-child node (3) and rebalance.  
    lines.push("after deletes (7,14,3) inorder=" + JSON.stringify(t.inorder()) + ", valid=" + t.validate());  // Show results after deletions.  
    return lines.join("\n");  // Join lines into one printable string.  
}  // End formatDemo().  

if (require.main === module) {  // Run demo only when executed as a script.  
    console.log(formatDemo());  // Print demo output.  
}  // Close main guard.  

module.exports = {  // Export public API for tests.  
    RedBlackTree,  // Export RedBlackTree wrapper.  
};  // End exports.  

