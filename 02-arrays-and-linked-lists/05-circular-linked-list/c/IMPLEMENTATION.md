# 實作說明（C）— 環狀鏈結串列（Circular Linked List）

## 目標

本單元用 C 實作「單向環狀鏈結串列」，並以 `hops`（走訪 `next` 次數）量化成本：

- `hops`：每做一次 `current = current->next` 就 +1

## 檔案

- `circular_linked_list.h/.c`：`CircularLinkedList` + 操作函式（含 hops）
- `circular_linked_list_main.c`：示範程式
- `test_circular_linked_list.c`：最小測試器（assert）
- `Makefile`：`make run` / `make test`

## 核心概念

### 1) tail-only：`head = tail->next`

只存 `tail`；非空時 `tail->next` 就是 head。

### 2) 走訪一定要 bounded（用 size）

環狀串列沒有 `NULL` 結尾；`to_array`、搜尋、destroy 都以 `size` 迴圈控制，避免無限迴圈與 double-free。

### 3) 成本公式（hops）

- `push_front / push_back / pop_front`：`hops = 0`
- `pop_back`：需要找 predecessor → `hops = max(0, n-2)`
- `rotate(k)`：`hops = k % size`，且 `k < 0` 會回傳 `false`（視為輸入錯誤）

## 如何執行

在 `02-arrays-and-linked-lists/05-circular-linked-list/c/`：

```bash
make
make run
make test
```

