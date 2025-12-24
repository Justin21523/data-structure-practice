# 實作說明（Python）— 動態陣列（Dynamic Array）

## 目標

Python 的 `list` 本質上就是動態陣列，但本單元刻意「自己實作」一個 `DynamicArray`，用來觀察兩件事：

- **擴容（resize）時的搬移成本**：用 `copied` 計數「擴容時複製了多少元素」。
- **插入/刪除（shift）時的搬移成本**：用 `moved` 計數「為了騰出/填補位置而移動了多少元素」。

容量策略採用 **倍增（doubling）**：當 `size == capacity` 時把 `capacity *= 2`，讓 `append` 呈現攤銷（amortized）O(1)。

## 檔案

- `dynamic_array_demo.py`：`DynamicArray` + CLI（印出 capacity 成長與 copied/moved 表格）
- `test_dynamic_array_demo.py`：`unittest`（驗證資料正確性 + 擴容/搬移計數）

## 核心概念

### 1) 擴容與 copied（為何 append 是攤銷 O(1)）

擴容會配置新 buffer，並把既有元素搬過去，因此 `copied == size`：

```python
for i in range(self._size):
    new_data[i] = self._data[i]
    copied += 1
```

在倍增策略下，做完 m 次 `append` 的「總 copied」會是 `final_capacity - 1`（1+2+4+... 的和），因此總成本是線性的。

### 2) insert_at / remove_at 與 moved（為何通常是 O(n)）

- `insert_at(index, v)`：右移 `[index, size-1]`，`moved = size - index`
- `remove_at(index)`：左移 `[index+1, size-1]`，`moved = size - index - 1`

## 如何執行

在 `02-arrays-and-linked-lists/02-dynamic-array/python/`：

```bash
python3 dynamic_array_demo.py
python3 -m unittest -v
```

