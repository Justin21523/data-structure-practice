# 實作說明（C）— Red-Black Tree（LLRB 版本）

## 目標

本單元用 C 手刻紅黑樹的等價變體 **Left-Leaning Red-Black Tree（LLRB）**：

- LLRB 仍滿足紅黑樹性質，但額外要求「紅連結向左」，讓插入/刪除的程式結構更一致
- 本 repo **不允許重複 key**：重複插入回傳 `false`
- `height` 以「邊數」定義：空樹 `-1`、葉 `0`（此單元高度用遞迴計算）

## 檔案

- `rbt.h` / `rbt.c`：資料結構與操作 API（LLRB）
- `rbt_main.c`：demo 輸出
- `test_rbt.c`：自製測試（離線可跑）
- `Makefile`：`make run` / `make test`

## 關鍵設計

### 1) `is_red(NULL) == false`

空指標視為黑色，讓判斷更乾淨：

```c
return node != NULL && node->red;
```

### 2) insert：BST 插入 + `fix_up()`

`fix_up()` 依序做：

1) 右紅就左旋（rotate_left）  
2) 連續左紅就右旋（rotate_right）  
3) 左右都紅就 flip_colors（分裂 4-node）

### 3) delete：top-down 下降時先「借紅」，回溯再 `fix_up()`

刪除用 `move_red_left/move_red_right`，在往下走之前先確保要走的那一側不是 2-node，避免走到底後無法修復。

為了避免 key 不存在時破壞樹，本單元 `rbt_delete()` 會先 `rbt_contains()`，不存在直接回傳 `false`。

### 4) validate：BST ordering + 紅黑樹不變量（含 LLRB）

`rbt_validate()` 檢查：

- root 必須是黑  
- 無連續紅  
- 黑高一致（所有 root->leaf 路徑黑節點數相同）  
- LLRB 規則：不允許右紅（red right link）  
- BST ordering（嚴格遞增、無重複）

## 如何執行

在 `06-balanced-trees/02-red-black-tree/c/`：

```bash
make run
make test
make clean
```

