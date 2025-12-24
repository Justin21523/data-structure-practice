# 實作說明（C）— 動態陣列（Dynamic Array）

## 目標

本單元用 C 手刻一個 `dynamic_array`（底層是 `int*` 連續記憶體），並用明確的計數器把成本呈現出來：

- `copied`：擴容（resize）時，搬移/複製元素的次數
- `moved`：插入/刪除時 shift 元素的次數

容量策略採 **倍增（doubling）**：`size == capacity` 時把容量乘 2，讓 `append` 具備攤銷（amortized）O(1)。

## 檔案

- `dynamic_array.h` / `dynamic_array.c`：資料結構與操作 API
- `dynamic_array_main.c`：示範程式（印出表格）
- `test_dynamic_array.c`：最小化測試（用 `assert`）
- `Makefile`：`make run` / `make test`

## 核心概念：resize 與 shift

### resize（計 copied）

擴容的核心就是 `malloc` 新 buffer，並把舊資料複製過去：

```c
for (int i = 0; i < array->size; i++) {
    new_data[i] = array->data[i];
    copied += 1;
}
```

### insert/remove（計 moved）

- `insert_at(i, v)`：右移 `[i, size-1]`，`moved = size - i`
- `remove_at(i)`：左移 `[i+1, size-1]`，`moved = size - i - 1`

## 如何執行

在 `02-arrays-and-linked-lists/02-dynamic-array/c/`：

```bash
make
make run
make test
```

