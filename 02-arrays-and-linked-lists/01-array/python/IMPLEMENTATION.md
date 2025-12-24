# 實作說明（Python）— 靜態陣列（Fixed Capacity Array）

## 目標

Python 的 `list` 本質上是「動態陣列」，但本單元要學的是「**靜態陣列（static array）**」的特性：容量固定、插入/刪除需要搬移元素。  
因此我們用 `FixedArray` 手動維護 `capacity` 與 `size`，並用 **moved elements（搬移次數）** 讓 O(n) 成本變得可觀察、可測試。

## 檔案

- `fixed_array_demo.py`：`FixedArray` + CLI（印出 insert/remove 在不同位置的搬移次數）
- `test_fixed_array_demo.py`：`unittest` 測試（資料正確性 + 搬移次數公式）

## 核心概念（搬移次數）

### 插入 `insert_at(i, v)`

需要把區間 `[i, size-1]` 右移一格，因此：

- `moved = size - i`

對應程式片段：

```python
for i in range(self._size, index, -1):
    self._data[i] = self._data[i - 1]
```

### 刪除 `remove_at(i)`

需要把區間 `[i+1, size-1]` 左移一格，因此：

- `moved = size - i - 1`

## 如何執行

在 `02-arrays-and-linked-lists/01-array/python/`：

```bash
python3 fixed_array_demo.py
python3 -m unittest -v
```

