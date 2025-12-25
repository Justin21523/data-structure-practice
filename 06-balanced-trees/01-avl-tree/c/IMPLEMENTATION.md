# 實作說明（C）— AVL Tree（自平衡二元搜尋樹）

## 目標

本單元用 C 手刻 `avl_tree`，把 BST 的插入/刪除改成「會自動平衡」：

- 節點結構多一個 `height` 欄位做快取（本 repo 以**邊數**定義：空樹 `-1`、葉 `0`）
- 插入/刪除回溯時：`update_height()` + `rebalance()`（LL/RR/LR/RL 旋轉）
- 另外提供 `avl_validate()`：同時檢查 BST ordering、height 正確、與平衡條件

## 檔案

- `avl.h` / `avl.c`：資料結構與操作 API
- `avl_main.c`：demo 輸出
- `test_avl.c`：自製測試（離線可跑）
- `Makefile`：`make run` / `make test`

## 關鍵設計

### 1) 以 `h(NULL)=-1` 對齊 repo 的 height 定義

```c
static int h(const avl_node* node) { return (node == NULL) ? -1 : node->height; }
```

### 2) 旋轉後要更新 height

右旋（right rotation）後先更新被下移的 `y`，再更新新的根 `x`：

```c
update_height(y);
update_height(x);
```

### 3) insert/delete 用遞迴回傳新的子樹根

`insert_subtree` / `delete_subtree` 都回傳 `(new_root, inserted/removed)`，避免漏接 parent 指標；回溯時再做 `update_height()` + `rebalance()`。

### 4) validate：把不變量寫成可測的函式

`avl_validate()` 會檢查：

- BST ordering（用 `(low, high)` 開區間界線、嚴格遞增、無重複）
- `node->height` 是否等於由子樹計算出的高度
- `abs(height(left) - height(right)) <= 1`

## 如何執行

在 `06-balanced-trees/01-avl-tree/c/`：

```bash
make run
make test
make clean
```

