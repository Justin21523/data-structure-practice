# 實作說明（C）— 樹走訪（Tree Traversal）

## 目標

本單元重點是把 DFS 三種走訪寫出「遞迴」與「迭代」兩種版本，並用測試確認結果一致：

- `tt_preorder_*`：root→left→right
- `tt_inorder_*`：left→root→right
- `tt_postorder_*`：left→right→root
- `tt_level_order`：BFS（queue）

## 檔案

- `tree_traversal.h/.c`：建樹（from level-order）+ traversal 實作
- `tree_traversal_main.c`：demo（印出各種走訪序列）
- `test_tree_traversal.c`：測試（不使用外部測試框架）
- `Makefile`：`make run` / `make test`

## 關鍵做法

### 1) postorder iterative：stack + visited flag（每個 node 可能入棧兩次）

```c
typedef struct tt_frame { tt_node* node; bool visited; } tt_frame;
```

當 `visited=false` 時先把自己以 `visited=true` 放回去，再把右/左 child 推進去。

### 2) inorder iterative：一路往左 push，回來再往右

用 `tt_node* stack[]` 存 path，走到最左邊後再 pop/visit。

### 3) BFS：queue + head/tail index

queue 用固定長度 `n` 的陣列（`n = tree_size`），避免做 shift。

## 如何執行

在 `05-trees-basic/03-tree-traversal/c/`：

```bash
make run
make test
make clean
```

