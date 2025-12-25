# 實作說明（Python）— AVL Tree（自平衡二元搜尋樹）

## 目標

本單元用 Python 手刻 `AvlTree`，在 BST 的插入/刪除基礎上，加入「高度快取 + 旋轉」來維持平衡：

- **AVL 平衡條件**：對任意節點 `x`，`balanceFactor(x) = height(left) - height(right)` 必須落在 `-1..1`
- 本 repo 的 `height` 定義為「**邊數**」：空樹 `-1`、葉 `0`
- 本 repo **不允許重複 key**：重複插入回傳 `False`

## 檔案

- `avl_tree_demo.py`：資料結構 + demo 輸出
- `test_avl_tree_demo.py`：使用 `unittest` 的測試（離線可跑）

## 關鍵設計

### 1) 用節點欄位快取 height（避免每次都 O(n) 算高度）

我們在每個 `Node` 上維護 `height`，並定義空子樹高度為 `-1`：

```python
def _h(node):
    return -1 if node is None else node.height
```

插入/刪除回溯時用 `update_height` 更新：

```python
node.height = 1 + max(_h(node.left), _h(node.right))
```

### 2) 旋轉（rotation）是 re-balance 的核心

以 **右旋**（Right Rotation）為例（LL / LR 的其中一步）：

```python
x = y.left
t2 = x.right
x.right = y
y.left = t2
```

旋轉後要依序更新 height（先更新被下移的節點，再更新新的根）：

```python
_update_height(y)
_update_height(x)
```

### 3) insert/delete 都用遞迴回傳「新的子樹根」

插入/刪除最容易踩雷的是「父節點要接回新的子樹根」，所以都用遞迴回傳 `(newRoot, inserted/removed)`：

- insert：先做 BST 插入，回溯時 `update_height` + `_rebalance`
- delete：先做 BST 刪除（含 0/1/2 子），回溯時同樣 `update_height` + `_rebalance`

### 4) validate()：把不變量寫成可測的函式

`validate()` 同時檢查：

- BST ordering（用 `(low, high)` 開區間界線，確保嚴格遞增、沒有重複）
- `node.height` 是否等於由子樹計算出的高度
- `abs(height(left) - height(right)) <= 1`

## 如何執行

在 `06-balanced-trees/01-avl-tree/python/`：

```bash
python3 avl_tree_demo.py
python3 -m unittest -v
```

