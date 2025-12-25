# 實作說明（Python）— Red-Black Tree（LLRB 版本）

## 目標

本單元用 Python 手刻 `RedBlackTree`，實作紅黑樹的等價變體 **Left-Leaning Red-Black Tree（LLRB）**：

- LLRB 仍滿足紅黑樹性質，但額外要求「紅連結向左」，因此程式可以用較一致的遞迴流程完成插入/刪除
- 本 repo **不允許重複 key**：重複插入回傳 `False`
- `height` 以「邊數」定義：空樹 `-1`、葉 `0`（此單元的高度用遞迴計算）

## 檔案

- `red_black_tree_demo.py`：資料結構 + demo 輸出
- `test_red_black_tree_demo.py`：使用 `unittest` 的測試（離線可跑）

## 關鍵設計

### 1) 用布林值表示顏色，`None` 視為黑

```python
def _is_red(node):
    return node is not None and node.red
```

### 2) insert：BST 插入 + `fix_up()` 修正 LLRB 形狀

插入後用三步修正：

1) 消除右紅（rotateLeft）  
2) 消除連續左紅（rotateRight）  
3) 兩側都紅時 flipColors（分裂 4-node）

### 3) delete：top-down 下降時先「借紅」，回溯再 `fix_up()`

刪除用 `move_red_left/move_red_right`，在往下走之前先確保要走的那一側不是 2-node（避免下去後無法修復）。

為了避免 key 不存在時破壞樹，本單元 `delete(key)` 會先 `contains(key)`，不存在直接回傳 `False`。

### 4) validate()：BST ordering + 紅黑樹不變量

`validate()` 會檢查：

- root 必須是黑  
- 無連續紅（red node 的 children 不能是 red）  
- 黑高一致（所有 root->leaf 路徑黑節點數相同）  
- LLRB 額外規則：不允許右紅（red right link）  
- BST ordering（嚴格遞增、無重複）

## 如何執行

在 `06-balanced-trees/02-red-black-tree/python/`：

```bash
python3 red_black_tree_demo.py
python3 -m unittest -v
```

