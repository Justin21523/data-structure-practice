"""紅黑樹（Red-Black Tree）示範 / Red-black tree demo.  # Bilingual module header.

本單元實作紅黑樹的「等價變體」：Left-Leaning Red-Black Tree（LLRB）。  # Traditional Chinese overview.
LLRB 仍然是紅黑樹（滿足 RB 的性質），但用「紅連結向左」的規則讓程式更一致：  # Explain why LLRB.
- insert / contains / delete（含 0 子、1 子、2 子）  # List supported operations.
- inorder 走訪會得到排序結果（用來驗證 BST ordering）  # Explain inorder property.
- validate() 會檢查 BST ordering + 紅黑樹/LLRB 不變量  # Mention validation.

We keep the code explicit and heavily commented so it reads like study notes.  # English overview.
"""  # End of module docstring.

from __future__ import annotations  # Allow forward references in type hints.  

from dataclasses import dataclass  # Use dataclass for small immutable demo records.  
from typing import List, Optional, Sequence, Tuple  # Import common typing helpers.  


RED = True  # Represent red links/nodes with True for readability.  
BLACK = False  # Represent black links/nodes with False for readability.  


@dataclass(frozen=True)  # Make demo snapshots immutable and easy to print/tests.  
class RbtSummary:  # Summarize key properties and inorder traversal for printing/tests.  
    size: int  # Number of nodes in the tree.  
    height: int  # Height measured in edges (empty=-1, leaf=0).  
    min_value: Optional[int]  # Minimum key (None if empty).  
    max_value: Optional[int]  # Maximum key (None if empty).  
    inorder: List[int]  # Inorder traversal (sorted keys for a valid BST).  
    is_valid: bool  # Whether the structure satisfies RB + BST invariants.  


class Node:  # LLRB node (key + left child + right child + color).  
    __slots__ = ("key", "left", "right", "red")  # Keep the node minimal and explicit.  

    def __init__(self, key: int, red: bool) -> None:  # Initialize node with a key, color, and empty children.  
        self.key = key  # Store node key.  
        self.left: Optional[Node] = None  # Store left child pointer (None means no child).  
        self.right: Optional[Node] = None  # Store right child pointer (None means no child).  
        self.red = red  # Store node color (True=RED, False=BLACK).  


class RedBlackTree:  # Red-Black Tree wrapper (implemented as Left-Leaning Red-Black, LLRB).  
    def __init__(self, root: Optional[Node] = None) -> None:  # Initialize tree with an optional root.  
        self._root = root  # Store root reference (None means empty tree).  

    @property
    def root(self) -> Optional[Node]:  # Expose root for demos/tests (read-only semantics by convention).  
        return self._root  # Return root reference.  

    def is_empty(self) -> bool:  # Check whether the tree has no nodes.  
        return self._root is None  # Empty iff root is None.  

    @staticmethod
    def from_values(values: Sequence[int]) -> RedBlackTree:  # Build a tree by inserting values in order.  
        t = RedBlackTree(root=None)  # Start from an empty tree.  
        for v in values:  # Insert each value in the given order.  
            t.insert(int(v))  # Insert one key (duplicates are rejected by policy).  
        return t  # Return built tree.  

    def contains(self, key: int) -> bool:  # Return True if key exists in the tree.  
        node = self._root  # Start searching from the root.  
        while node is not None:  # Walk down until we hit a missing child.  
            if key < node.key:  # Go left when key is smaller.  
                node = node.left  # Move to left child.  
            elif key > node.key:  # Go right when key is larger.  
                node = node.right  # Move to right child.  
            else:  # key == node.key means we found it.  
                return True  # Report found.  
        return False  # Report not found when search falls off the tree.  

    def insert(self, key: int) -> bool:  # Insert key; return False when key already exists.  
        self._root, inserted = self._insert_subtree(self._root, key)  # Insert into root subtree.  
        if self._root is not None:  # Ensure root exists before coloring.  
            self._root.red = BLACK  # Root must always be black in a red-black tree.  
        return inserted  # Return whether insertion happened.  

    def delete(self, key: int) -> bool:  # Delete key; return True when a node was removed.  
        if self._root is None:  # Empty tree cannot delete anything.  
            return False  # Report not removed.  
        if not self.contains(key):  # Avoid running LLRB delete when key is missing.  
            return False  # Report not removed.  

        # If both children of root are black, set root red to enable moves during deletion.  # Explain standard LLRB pre-step.  
        if (not self._is_red(self._root.left)) and (not self._is_red(self._root.right)):  # Detect 2-node root situation.  
            self._root.red = RED  # Temporarily color root red to simplify rebalancing logic.  

        self._root = self._delete_subtree(self._root, key)  # Delete key from root subtree (guaranteed to exist).  
        if self._root is not None:  # Restore invariant when tree is non-empty.  
            self._root.red = BLACK  # Root must be black.  
        return True  # Report removed.  

    def min(self) -> Optional[int]:  # Return minimum key, or None if tree is empty.  
        if self._root is None:  # Empty tree has no minimum.  
            return None  # Return None for empty.  
        return self._min_node(self._root).key  # Return leftmost key.  

    def max(self) -> Optional[int]:  # Return maximum key, or None if tree is empty.  
        if self._root is None:  # Empty tree has no maximum.  
            return None  # Return None for empty.  
        node = self._root  # Start at root.  
        while node.right is not None:  # Keep going right until we cannot.  
            node = node.right  # Move to right child.  
        return node.key  # Rightmost node has the maximum key.  

    def size(self) -> int:  # Return number of nodes in the tree (O(n)).  
        def _size(node: Optional[Node]) -> int:  # Recursive helper that counts nodes in a subtree.  
            if node is None:  # Empty subtree contributes 0 nodes.  
                return 0  # Return 0 for empty.  
            return 1 + _size(node.left) + _size(node.right)  # Count root + left + right.  

        return _size(self._root)  # Count nodes starting from root.  

    def height(self) -> int:  # Return height measured in edges (empty=-1, leaf=0).  
        def _height(node: Optional[Node]) -> int:  # Recursive helper that computes height in edges.  
            if node is None:  # Empty subtree has height -1.  
                return -1  # Return -1 for empty.  
            return 1 + max(_height(node.left), _height(node.right))  # Height is 1 + max(child heights).  

        return _height(self._root)  # Compute height from root.  

    def inorder(self) -> List[int]:  # Return inorder traversal (sorted for a valid BST).  
        out: List[int] = []  # Accumulate traversal result.  

        def _walk(node: Optional[Node]) -> None:  # Recursive inorder (left, root, right).  
            if node is None:  # Empty subtree contributes nothing.  
                return  # Stop recursion.  
            _walk(node.left)  # Visit left subtree first.  
            out.append(node.key)  # Visit root.  
            _walk(node.right)  # Visit right subtree.  

        _walk(self._root)  # Walk from root.  
        return out  # Return traversal list.  

    def validate(self) -> bool:  # Validate BST ordering + red-black properties (LLRB variant).  
        if self._root is None:  # Empty tree is valid.  
            return True  # Vacuously valid.  
        if self._root.red:  # Root must be black.  
            return False  # Report invalid root color.  
        ok, _ = self._validate_subtree(self._root, low=None, high=None)  # Validate from root with open bounds.  
        return ok  # Return validation result.  

    def summarize(self) -> RbtSummary:  # Build a snapshot summary for demos/tests.  
        return RbtSummary(  # Construct a summary record.  
            size=self.size(),  # Compute node count.  
            height=self.height(),  # Compute height (edges).  
            min_value=self.min(),  # Compute minimum key (or None).  
            max_value=self.max(),  # Compute maximum key (or None).  
            inorder=self.inorder(),  # Compute inorder traversal (sorted for a valid BST).  
            is_valid=self.validate(),  # Validate red-black invariants.  
        )  # Close summary construction.  

    # -------------------------  # Visual separator for teaching clarity.  
    # LLRB helpers (color / rotations / flips)  # Explain next section purpose.  
    # -------------------------  # Visual separator for teaching clarity.  

    @staticmethod
    def _is_red(node: Optional[Node]) -> bool:  # Return True when node is red; None is treated as black.  
        return node is not None and node.red == RED  # Null links are black by definition.  

    @classmethod
    def _rotate_left(cls, h: Node) -> Node:  # Rotate left to eliminate a right-leaning red link.  
        x = h.right  # Take right child as pivot.  
        assert x is not None  # Rotation precondition: right child must exist.  

        h.right = x.left  # Move pivot's left subtree to h's right slot.  
        x.left = h  # Put h as pivot's left child.  

        x.red = h.red  # Preserve original color on the new root.  
        h.red = RED  # Mark the moved-down node as red (standard LLRB rotation rule).  
        return x  # Return new subtree root.  

    @classmethod
    def _rotate_right(cls, h: Node) -> Node:  # Rotate right to fix two consecutive left-leaning red links.  
        x = h.left  # Take left child as pivot.  
        assert x is not None  # Rotation precondition: left child must exist.  

        h.left = x.right  # Move pivot's right subtree to h's left slot.  
        x.right = h  # Put h as pivot's right child.  

        x.red = h.red  # Preserve original color on the new root.  
        h.red = RED  # Mark the moved-down node as red (standard LLRB rotation rule).  
        return x  # Return new subtree root.  

    @staticmethod
    def _flip_colors(h: Node) -> None:  # Flip colors to split or merge 4-nodes (LLRB <-> 2-3 tree).  
        h.red = not h.red  # Toggle parent color.  
        if h.left is not None:  # Guard for safety (children should exist in normal LLRB usage).  
            h.left.red = not h.left.red  # Toggle left child color.  
        if h.right is not None:  # Guard for safety (children should exist in normal LLRB usage).  
            h.right.red = not h.right.red  # Toggle right child color.  

    @classmethod
    def _fix_up(cls, h: Node) -> Node:  # Restore LLRB invariants on the way back up after insert/delete.  
        if cls._is_red(h.right) and not cls._is_red(h.left):  # Fix right-leaning red link.  
            h = cls._rotate_left(h)  # Rotate left to make red link lean left.  
        if cls._is_red(h.left) and cls._is_red(h.left.left if h.left else None):  # Fix two consecutive left reds.  
            h = cls._rotate_right(h)  # Rotate right to balance the 4-node shape.  
        if cls._is_red(h.left) and cls._is_red(h.right):  # Split 4-node when both children are red.  
            cls._flip_colors(h)  # Flip colors to push red up one level.  
        return h  # Return possibly rotated/flipped subtree root.  

    @classmethod
    def _move_red_left(cls, h: Node) -> Node:  # Ensure h.left is red when we need to delete from the left side.  
        cls._flip_colors(h)  # Push red down so we can borrow from the right side.  
        if h.right is not None and cls._is_red(h.right.left):  # Borrow from right-left red link when available.  
            h.right = cls._rotate_right(h.right)  # Rotate right on right child to create a right-leaning red link.  
            h = cls._rotate_left(h)  # Rotate left on h to move red link to the left side.  
            cls._flip_colors(h)  # Re-flip to restore colors after rotations.  
        return h  # Return adjusted subtree root.  

    @classmethod
    def _move_red_right(cls, h: Node) -> Node:  # Ensure h.right is red when we need to delete from the right side.  
        cls._flip_colors(h)  # Push red down so we can borrow from the left side.  
        if h.left is not None and cls._is_red(h.left.left):  # Borrow from left-left red link when available.  
            h = cls._rotate_right(h)  # Rotate right to move red link to the right side.  
            cls._flip_colors(h)  # Re-flip to restore colors after rotation.  
        return h  # Return adjusted subtree root.  

    # -------------------------  # Visual separator for teaching clarity.  
    # Insert/Delete (LLRB)  # Explain next section purpose.  
    # -------------------------  # Visual separator for teaching clarity.  

    @classmethod
    def _insert_subtree(cls, h: Optional[Node], key: int) -> Tuple[Optional[Node], bool]:  # Insert key into subtree and rebalance.  
        if h is None:  # Empty spot: create a new red leaf node.  
            return Node(key=key, red=RED), True  # Return new node and inserted=True.  

        inserted = False  # Track insertion flag from recursion.  
        if key < h.key:  # Insert into left subtree when key is smaller.  
            h.left, inserted = cls._insert_subtree(h.left, key)  # Recurse into left child slot.  
        elif key > h.key:  # Insert into right subtree when key is larger.  
            h.right, inserted = cls._insert_subtree(h.right, key)  # Recurse into right child slot.  
        else:  # key == h.key means duplicate key.  
            return h, False  # Reject duplicates by policy (no changes).  

        h = cls._fix_up(h)  # Restore invariants after insertion below.  
        return h, inserted  # Return possibly rebalanced subtree root and insertion flag.  

    @classmethod
    def _delete_min(cls, h: Node) -> Optional[Node]:  # Delete the minimum node in this subtree and return new subtree root.  
        if h.left is None:  # Reached the minimum node (no left child).  
            return None  # Delete this node by returning None.  

        if (not cls._is_red(h.left)) and (not cls._is_red(h.left.left if h.left else None)):  # Ensure we do not descend into a 2-node.  
            h = cls._move_red_left(h)  # Move a red link to the left side.  

        h.left = cls._delete_min(h.left)  # Recurse into left subtree.  
        return cls._fix_up(h)  # Fix up invariants on the way back up.  

    @classmethod
    def _delete_subtree(cls, h: Node, key: int) -> Optional[Node]:  # Delete key from subtree (assumes key exists).  
        if key < h.key:  # Key is in the left subtree.  
            if h.left is not None:  # Only recurse when left subtree exists.  
                if (not cls._is_red(h.left)) and (not cls._is_red(h.left.left if h.left else None)):  # Avoid descending into a 2-node.  
                    h = cls._move_red_left(h)  # Move a red link to the left side.  
                h.left = cls._delete_subtree(h.left, key)  # Recurse into left subtree.  
        else:  # Key is in the current node or right subtree.  
            if cls._is_red(h.left):  # Rotate right to ensure we have a red link on the right path when needed.  
                h = cls._rotate_right(h)  # Rotate right to maintain LLRB delete preconditions.  

            if key == h.key and h.right is None:  # Case: match and no right child => can delete this node.  
                return None  # Delete node by returning None.  

            if h.right is not None:  # Only work on right side when it exists.  
                if (not cls._is_red(h.right)) and (not cls._is_red(h.right.left)):  # Avoid descending into a 2-node.  
                    h = cls._move_red_right(h)  # Move a red link to the right side.  
                if key == h.key:  # Match case with a right subtree => replace with successor.  
                    successor = cls._min_node(h.right)  # Find successor node from right subtree.  
                    h.key = successor.key  # Copy successor key into current node (keeps shape).  
                    h.right = cls._delete_min(h.right)  # Delete successor node from right subtree.  
                else:  # Otherwise delete in the right subtree.  
                    h.right = cls._delete_subtree(h.right, key)  # Recurse into right subtree.  

        return cls._fix_up(h)  # Fix up invariants on the way back up.  

    @staticmethod
    def _min_node(node: Node) -> Node:  # Return the leftmost (minimum-key) node in a non-empty subtree.  
        cur = node  # Start at given node.  
        while cur.left is not None:  # Keep going left until we cannot.  
            cur = cur.left  # Move to left child.  
        return cur  # Leftmost node has the minimum key.  

    # -------------------------  # Visual separator for teaching clarity.  
    # Validation (RB + LLRB)  # Explain next section purpose.  
    # -------------------------  # Visual separator for teaching clarity.  

    @classmethod
    def _validate_subtree(cls, node: Optional[Node], low: Optional[int], high: Optional[int]) -> Tuple[bool, int]:  # Return (ok, blackHeight).  
        if node is None:  # Null links are black and contribute a base black-height of 1.  
            return True, 1  # Treat None as a black leaf sentinel.  

        if low is not None and node.key <= low:  # Enforce strict lower bound (no duplicates).  
            return False, 0  # Report invalid ordering.  
        if high is not None and node.key >= high:  # Enforce strict upper bound (no duplicates).  
            return False, 0  # Report invalid ordering.  

        if cls._is_red(node.right):  # LLRB rule: red links must lean left (no red right child).  
            return False, 0  # Report invalid right-leaning red link.  

        if cls._is_red(node) and (cls._is_red(node.left) or cls._is_red(node.right)):  # RB rule: no two consecutive reds.  
            return False, 0  # Report invalid consecutive red links.  

        left_ok, left_bh = cls._validate_subtree(node.left, low, node.key)  # Validate left subtree with updated upper bound.  
        if not left_ok:  # Early return on left failure to keep logic simple.  
            return False, 0  # Propagate failure.  
        right_ok, right_bh = cls._validate_subtree(node.right, node.key, high)  # Validate right subtree with updated lower bound.  
        if not right_ok:  # Early return on right failure to keep logic simple.  
            return False, 0  # Propagate failure.  

        if left_bh != right_bh:  # RB rule: all root-to-leaf paths have the same number of black nodes.  
            return False, 0  # Report black-height mismatch.  

        black_here = 0 if cls._is_red(node) else 1  # Count black nodes; red nodes do not add to black-height.  
        return True, left_bh + black_here  # Return ok with computed black-height.  


def format_demo() -> str:  # Format a small demo output using a classic insert sequence.  
    values = [8, 3, 10, 1, 6, 14, 4, 7, 13]  # Classic textbook insert order.  
    tree = RedBlackTree.from_values(values)  # Build tree by inserting in order.  
    s = tree.summarize()  # Summarize properties and inorder traversal.  

    lines: List[str] = []  # Accumulate printable lines.  
    lines.append("=== Red-Black Tree Demo (Python, LLRB) ===")  # Print header.  
    lines.append(f"insert order: {values}")  # Print insertion order.  
    lines.append(f"size={s.size}, height={s.height}, min={s.min_value}, max={s.max_value}, valid={s.is_valid}")  # Print key properties.  
    lines.append(f"inorder(sorted)={s.inorder}")  # Print inorder traversal (should be sorted).  
    lines.append(f"contains(6)={tree.contains(6)}, contains(99)={tree.contains(99)}")  # Print search examples.  

    tree.delete(7)  # Delete a leaf (7).  
    tree.delete(14)  # Delete a one-child node (14 has left child 13 in this data set).  
    tree.delete(3)  # Delete a two-child node (3) and rebalance.  
    lines.append(f"after deletes (7,14,3) inorder={tree.inorder()}, valid={tree.validate()}")  # Show results after deletions.  
    return "\n".join(lines)  # Join lines into one printable string.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    print(format_demo())  # Print demo output.  

