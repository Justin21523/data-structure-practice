# 實作說明（C）— 二元搜尋樹（Binary Search Tree, BST）

## 目標

本單元用 C 手刻 `BST`，重點練習：

- `bst_insert`：插入（本 repo **不允許重複 key**；重複插入回傳 `false`）
- `bst_contains`：搜尋 key 是否存在
- `bst_delete`：刪除（0 子 / 1 子 / 2 子；2 子使用 **inorder successor**）
- `bst_min` / `bst_max`：最小/最大值（空樹回傳 `false`）
- `bst_size` / `bst_height`：節點數與高度（`height` 以邊數計：空樹 `-1`、葉 `0`）
- `bst_inorder`：回傳「值序列」（BST 會得到遞增排序）

## 檔案

- `bst.h/.c`：資料結構與操作
- `bst_main.c`：demo（印出 summary 與 inorder）
- `test_bst.c`：測試（不使用外部測試框架）
- `Makefile`：`make run` / `make test`

## 關鍵設計

### 1) delete：用 struct 回傳「新的子樹根」

因為刪除需要更新 parent 指標，所以用：

```c
typedef struct bst_delete_result {
    bst_node* new_root;
    bool removed;
} bst_delete_result;
```

### 2) 2 子刪除：用 inorder successor（右子樹最小）

```c
int successor_key = min_key(node->right);
node->key = successor_key;
node->right = delete_subtree(node->right, successor_key).new_root;
```

### 3) inorder：回傳排序序列

`bst_inorder` 會先用 `bst_size` 配置陣列，再用遞迴填入。

## 如何執行

在 `05-trees-basic/02-binary-search-tree/c/`：

```bash
make run
make test
make clean
```

