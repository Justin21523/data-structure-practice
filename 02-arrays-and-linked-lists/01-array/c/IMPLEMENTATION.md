# 實作說明（C）— 靜態陣列（Fixed Capacity Array）

## 目標

用 C 手動配置一段固定大小的連續記憶體（`malloc(capacity * sizeof(int))`），模擬靜態陣列的行為：  
容量固定、隨機存取 O(1)，但插入/刪除會因為 shift 而是 O(n)。

本單元用 **moved elements（搬移次數）** 量化 shift 成本，並寫進測試驗證公式。

## 檔案

- `fixed_array.h` / `fixed_array.c`：`fixed_array` API（append/insert/remove/get/set/index_of）
- `fixed_array_main.c`：CLI 印出 insert/remove 在不同位置的搬移次數
- `test_fixed_array.c`：最小化測試 runner（PASS/FAIL）
- `Makefile`：`make run` / `make test`

## 搬移次數公式

- 插入 `insert_at(i)`：右移 `[i, size-1]`  
  `moved = size - i`
- 刪除 `remove_at(i)`：左移 `[i+1, size-1]`  
  `moved = size - i - 1`

## 如何執行

在 `02-arrays-and-linked-lists/01-array/c/`：

```bash
make
make run
make test
```

