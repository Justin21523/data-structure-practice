"""AVL Tree（自平衡二元搜尋樹）示範 / AVL tree demo.  # Bilingual module header.

本單元實作 AVL Tree 的核心操作（以 int key 為例）：  # Traditional Chinese overview.
- insert / contains / delete（刪除含 0 子、1 子、2 子）  # List supported operations.
- 透過旋轉（LL / RR / LR / RL）維持 balance factor 在 -1..1  # Mention balancing rule.
- inorder 走訪會得到排序結果（用來驗證 BST ordering）  # Explain inorder property.

We keep the code explicit and heavily commented so it reads like study notes.  # English overview.
"""  # End of module docstring.

from __future__ import annotations  # Allow forward references in type hints.  

from dataclasses import dataclass  # Use dataclass for small immutable demo records.  
from typing import List, Optional, Sequence, Tuple  # Import common typing helpers.  


@dataclass(frozen=True)  # Make demo snapshots immutable and easy to print/tests.  
class AvlSummary:  # Summarize key properties and inorder traversal for printing/tests.  
    size: int  # Number of nodes in the AVL tree.  
    height: int  # Height measured in edges (empty=-1, leaf=0).  
    min_value: Optional[int]  # Minimum key (None if empty).  
    max_value: Optional[int]  # Maximum key (None if empty).  
    inorder: List[int]  # Inorder traversal (sorted keys for a valid BST).  
    is_valid: bool  # Whether the structure satisfies AVL + BST invariants.  


class Node:  # AVL node (key + left child + right child + cached height).  
    __slots__ = ("key", "left", "right", "height")  # Keep the node minimal and explicit.  

    def __init__(self, key: int) -> None:  # Initialize node with a key and empty children.  
        self.key = key  # Store node key.  
        self.left: Optional[Node] = None  # Store left child pointer (None means no child).  
        self.right: Optional[Node] = None  # Store right child pointer (None means no child).  
        self.height = 0  # Store cached height in edges (leaf=0 by definition).  


class AvlTree:  # AVL tree wrapper that holds a root node.  
    def __init__(self, root: Optional[Node] = None) -> None:  # Initialize AVL tree with an optional root.  
        self._root = root  # Store root reference (None means empty tree).  

    @property
    def root(self) -> Optional[Node]:  # Expose root for demos/tests (read-only semantics by convention).  
        return self._root  # Return root reference.  

    def is_empty(self) -> bool:  # Check whether the tree has no nodes.  
        return self._root is None  # Empty iff root is None.  

    @staticmethod
    def from_values(values: Sequence[int]) -> AvlTree:  # Build an AVL tree by inserting values in order.  
        t = AvlTree(root=None)  # Start from an empty tree.  
        for v in values:  # Insert each value in the given order.  
            t.insert(int(v))  # Insert one key (duplicates are rejected by policy).  
        return t  # Return built AVL tree.  

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
        return inserted  # Return whether insertion happened.  

    def delete(self, key: int) -> bool:  # Delete key; return True when a node was removed.  
        self._root, removed = self._delete_subtree(self._root, key)  # Delete from root subtree.  
        return removed  # Return whether deletion happened.  

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

    def height(self) -> int:  # Return tree height measured in edges (empty=-1, leaf=0).  
        return -1 if self._root is None else self._root.height  # Use cached root height (or -1 for empty).  

    def size(self) -> int:  # Return number of nodes in the tree (O(n)).  
        def _size(node: Optional[Node]) -> int:  # Recursive helper that counts nodes in a subtree.  
            if node is None:  # Empty subtree contributes 0 nodes.  
                return 0  # Return 0 for empty.  
            return 1 + _size(node.left) + _size(node.right)  # Count root + left + right.  

        return _size(self._root)  # Count nodes starting from root.  

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

    def validate(self) -> bool:  # Validate BST ordering + AVL balance + cached height correctness.  
        ok, _ = self._validate_subtree(self._root, low=None, high=None)  # Validate from root with open bounds.  
        return ok  # Return validation result.  

    def summarize(self) -> AvlSummary:  # Build a snapshot summary for demos/tests.  
        return AvlSummary(  # Construct a summary record.  
            size=self.size(),  # Compute node count.  
            height=self.height(),  # Read tree height (edges).  
            min_value=self.min(),  # Compute minimum key (or None).  
            max_value=self.max(),  # Compute maximum key (or None).  
            inorder=self.inorder(),  # Compute inorder traversal (sorted for a valid BST).  
            is_valid=self.validate(),  # Validate AVL invariants.  
        )  # Close summary construction.  

    # -------------------------  # Visual separator for teaching clarity.  
    # Internal helpers (height / balance / rotations)  # Explain next section purpose.  
    # -------------------------  # Visual separator for teaching clarity.  

    @staticmethod
    def _h(node: Optional[Node]) -> int:  # Return cached height for node, or -1 for empty subtree.  
        return -1 if node is None else node.height  # Apply repo-wide height convention.  

    @classmethod
    def _update_height(cls, node: Node) -> None:  # Recompute node.height from children heights.  
        node.height = 1 + max(cls._h(node.left), cls._h(node.right))  # Height in edges is 1 + max(child heights).  

    @classmethod
    def _balance_factor(cls, node: Node) -> int:  # Compute balance factor = height(left) - height(right).  
        return cls._h(node.left) - cls._h(node.right)  # Positive means left-heavy; negative means right-heavy.  

    @classmethod
    def _rotate_right(cls, y: Node) -> Node:  # Right rotation to fix a left-heavy imbalance (LL or LR step).  
        x = y.left  # Take left child as pivot.  
        assert x is not None  # Rotation precondition: left child must exist.  
        t2 = x.right  # Keep pivot's right subtree (will become y's left subtree).  

        x.right = y  # Put y as the right child of x.  
        y.left = t2  # Attach T2 as y's left child.  

        cls._update_height(y)  # Update y first because its children changed.  
        cls._update_height(x)  # Update x after y because x now points to y.  
        return x  # Return new subtree root.  

    @classmethod
    def _rotate_left(cls, x: Node) -> Node:  # Left rotation to fix a right-heavy imbalance (RR or RL step).  
        y = x.right  # Take right child as pivot.  
        assert y is not None  # Rotation precondition: right child must exist.  
        t2 = y.left  # Keep pivot's left subtree (will become x's right subtree).  

        y.left = x  # Put x as the left child of y.  
        x.right = t2  # Attach T2 as x's right child.  

        cls._update_height(x)  # Update x first because its children changed.  
        cls._update_height(y)  # Update y after x because y now points to x.  
        return y  # Return new subtree root.  

    @classmethod
    def _rebalance(cls, node: Node) -> Node:  # Rebalance node if it violates AVL balance constraints.  
        balance = cls._balance_factor(node)  # Compute current node's balance factor.  

        if balance > 1:  # Left-heavy subtree; need either LL or LR rotation(s).  
            assert node.left is not None  # Left child must exist when balance > 1.  
            if cls._balance_factor(node.left) >= 0:  # LL case: left child is not right-heavy.  
                return cls._rotate_right(node)  # Single right rotation fixes LL.  
            node.left = cls._rotate_left(node.left)  # LR case step 1: rotate left on left child.  
            return cls._rotate_right(node)  # LR case step 2: rotate right on current node.  

        if balance < -1:  # Right-heavy subtree; need either RR or RL rotation(s).  
            assert node.right is not None  # Right child must exist when balance < -1.  
            if cls._balance_factor(node.right) <= 0:  # RR case: right child is not left-heavy.  
                return cls._rotate_left(node)  # Single left rotation fixes RR.  
            node.right = cls._rotate_right(node.right)  # RL case step 1: rotate right on right child.  
            return cls._rotate_left(node)  # RL case step 2: rotate left on current node.  

        return node  # Already balanced; return node unchanged.  

    # -------------------------  # Visual separator for teaching clarity.  
    # Internal helpers (insert / delete)  # Explain next section purpose.  
    # -------------------------  # Visual separator for teaching clarity.  

    @classmethod
    def _insert_subtree(cls, node: Optional[Node], key: int) -> Tuple[Optional[Node], bool]:  # Insert into subtree and rebalance.  
        if node is None:  # Empty spot: create a new leaf node.  
            return Node(key), True  # Return new node and inserted=True.  

        if key < node.key:  # Insert into left subtree when key is smaller.  
            node.left, inserted = cls._insert_subtree(node.left, key)  # Recurse into left child slot.  
        elif key > node.key:  # Insert into right subtree when key is larger.  
            node.right, inserted = cls._insert_subtree(node.right, key)  # Recurse into right child slot.  
        else:  # key == node.key means duplicate key.  
            return node, False  # Reject duplicates by policy (no structural changes).  

        cls._update_height(node)  # Update cached height after insertion below.  
        node = cls._rebalance(node)  # Rebalance on the way back up.  
        return node, inserted  # Return possibly new root and insertion flag.  

    @classmethod
    def _delete_subtree(cls, node: Optional[Node], key: int) -> Tuple[Optional[Node], bool]:  # Delete from subtree and rebalance.  
        if node is None:  # Key not found in an empty subtree.  
            return None, False  # Nothing removed.  

        removed = False  # Track whether a node was removed in this subtree.  
        if key < node.key:  # Delete from left subtree when key is smaller.  
            node.left, removed = cls._delete_subtree(node.left, key)  # Recurse into left child.  
        elif key > node.key:  # Delete from right subtree when key is larger.  
            node.right, removed = cls._delete_subtree(node.right, key)  # Recurse into right child.  
        else:  # key == node.key means we found the node to delete.  
            removed = True  # Mark that we are removing a node.  

            if node.left is None and node.right is None:  # Case 1: leaf node (0 children).  
                return None, True  # Remove leaf by returning None.  
            if node.left is None:  # Case 2a: only right child exists.  
                return node.right, True  # Replace node by its right child.  
            if node.right is None:  # Case 2b: only left child exists.  
                return node.left, True  # Replace node by its left child.  

            # Case 3: two children => replace with inorder successor (min of right subtree).  # Explain 2-child strategy.  
            successor = cls._min_node(node.right)  # Find successor node from right subtree.  
            node.key = successor.key  # Copy successor key into current node (keeps tree shape).  
            node.right, _ = cls._delete_subtree(node.right, successor.key)  # Delete successor node from right subtree.  

        cls._update_height(node)  # Update cached height after deletion below.  
        node = cls._rebalance(node)  # Rebalance on the way back up.  
        return node, removed  # Return possibly new root and removed flag.  

    @staticmethod
    def _min_node(node: Node) -> Node:  # Return the leftmost (minimum-key) node in a non-empty subtree.  
        cur = node  # Start at given node.  
        while cur.left is not None:  # Keep going left until we cannot.  
            cur = cur.left  # Move to left child.  
        return cur  # Leftmost node has the minimum key.  

    # -------------------------  # Visual separator for teaching clarity.  
    # Validation helper  # Explain next section purpose.  
    # -------------------------  # Visual separator for teaching clarity.  

    @classmethod
    def _validate_subtree(  # Validate subtree and return (ok, computedHeight).  
        cls, node: Optional[Node], low: Optional[int], high: Optional[int]  # Carry open interval bounds for BST ordering.  
    ) -> Tuple[bool, int]:  # Return (isValid, computedHeight).  
        if node is None:  # Empty subtree is valid and has height -1 by definition.  
            return True, -1  # Return valid with height -1.  

        if low is not None and node.key <= low:  # Enforce strict lower bound (no duplicates).  
            return False, 0  # Report invalid; height value is irrelevant on failure.  
        if high is not None and node.key >= high:  # Enforce strict upper bound (no duplicates).  
            return False, 0  # Report invalid; height value is irrelevant on failure.  

        left_ok, left_h = cls._validate_subtree(node.left, low, node.key)  # Validate left subtree with updated upper bound.  
        if not left_ok:  # Early return on left failure to keep logic simple.  
            return False, 0  # Propagate failure.  
        right_ok, right_h = cls._validate_subtree(node.right, node.key, high)  # Validate right subtree with updated lower bound.  
        if not right_ok:  # Early return on right failure to keep logic simple.  
            return False, 0  # Propagate failure.  

        computed_h = 1 + max(left_h, right_h)  # Compute expected height from children heights.  
        if node.height != computed_h:  # Cached height must match computed height.  
            return False, computed_h  # Report height mismatch (include computed height for debugging).  

        if abs(left_h - right_h) > 1:  # AVL balance constraint requires |bf| <= 1.  
            return False, computed_h  # Report imbalance.  

        return True, computed_h  # Report valid subtree with computed height.  


def format_demo() -> str:  # Format a small demo output using a classic insert sequence.  
    values = [8, 3, 10, 1, 6, 14, 4, 7, 13]  # Classic textbook insert order.  
    tree = AvlTree.from_values(values)  # Build AVL tree by inserting in order.  
    s = tree.summarize()  # Summarize properties and inorder traversal.  

    lines: List[str] = []  # Accumulate printable lines.  
    lines.append("=== AVL Tree Demo (Python) ===")  # Print header.  
    lines.append(f"insert order: {values}")  # Print insertion order.  
    lines.append(f"size={s.size}, height={s.height}, min={s.min_value}, max={s.max_value}, valid={s.is_valid}")  # Print key properties.  
    lines.append(f"inorder(sorted)={s.inorder}")  # Print inorder traversal (should be sorted).  
    lines.append(f"contains(6)={tree.contains(6)}, contains(99)={tree.contains(99)}")  # Print search examples.  

    tree.delete(7)  # Delete a leaf (7) to show delete on AVL.  
    tree.delete(14)  # Delete a one-child node (14 has left child 13).  
    tree.delete(3)  # Delete a two-child node (3) and rebalance.  
    lines.append(f"after deletes (7,14,3) inorder={tree.inorder()}, valid={tree.validate()}")  # Show results after deletions.  
    return "\n".join(lines)  # Join lines into one printable string.  


if __name__ == "__main__":  # Run demo only when executed as a script.  
    print(format_demo())  # Print demo output.  

