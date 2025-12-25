# 實作說明（Python）— 二元搜尋樹（Binary Search Tree, BST）

## 目標

本單元用 Python 手刻 `BinarySearchTree`，練習 BST 的核心操作與刪除三種情況：

- `insert(key)`：插入（本 repo **不允許重複 key**；重複插入回傳 `False`）
- `contains(key)`：查找 key 是否存在
- `delete(key)`：刪除（0 子 / 1 子 / 2 子；2 子使用 **inorder successor**）
- `inorder()`：中序走訪（BST 會得到遞增排序序列）
- `size()` / `height()` / `min()` / `max()`
  - `height` 以「邊數」定義：空樹 `-1`、葉 `0`

## 檔案

- `bst_demo.py`：資料結構 + demo 輸出
- `test_bst_demo.py`：使用 `unittest` 的測試（離線可跑）

## 關鍵設計

### 1) insert：走到空指標再接上去（拒絕重複）

用 while 往下找位置，遇到相等就拒絕：

```python
while node is not None:
    if key < node.key:
        node = node.left
    elif key > node.key:
        node = node.right
    else:
        return False
```

### 2) delete：用遞迴回傳「新的子樹根」

刪除時最容易踩雷的是「要把 parent 的指標改掉」，所以用遞迴回傳 `(new_root, removed)`：

```python
node.left, removed = self._delete_subtree(node.left, key)
return node, removed
```

### 3) 2 子刪除：用 inorder successor（右子樹最小）

當節點有兩個子樹時，做法是：
1) 找右子樹的最小值 `successor_key`
2) 把當前 node.key 改成 successor
3) 再到右子樹刪掉 successor 那個節點

```python
successor_key = self._min_key(node.right)
node.key = successor_key
node.right, _ = self._delete_subtree(node.right, successor_key)
```

## 如何執行

在 `05-trees-basic/02-binary-search-tree/python/`：

```bash
python3 bst_demo.py
python3 -m unittest -v
```

