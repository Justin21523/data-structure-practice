# 實作說明（Python）— 堆疊（Stack）

## 目標

本單元用教學版 `ArrayStack` 來實作堆疊（stack），並把「擴容成本」量化成可測試的數字：

- `copied`：當底層陣列滿了需要擴容時，複製了多少元素（resize copy count）

堆疊採用 **陣列尾端當作 top**（`top_index = size - 1`），因此：

- `push / pop / peek` 都是 O(1)
- `push` 在容量不足時會擴容，單次最壞會複製 O(n) 個元素，但整體是 **攤銷 O(1)**（doubling）

## 檔案

- `stack_demo.py`：`ArrayStack` + CLI（印出 push 成長表格與一個 LIFO 小例子）
- `test_stack_demo.py`：`unittest`（驗證 LIFO 正確性 + `copied` 計數）

## 核心概念

### 1) 倍增擴容與 copied

當 `size == capacity` 時，配置更大的 buffer 並把 `0..size-1` 複製過去：

```python
for i in range(self._size):
    new_data[i] = self._data[i]
    copied += 1
```

### 2) 典型 copied 序列（容量從 1 開始）

依序 push `10,20,30,40,50`，`copied` 會是：

- `[0, 1, 2, 0, 4]`（因為容量走 `1 → 2 → 4 → 8`）

## 如何執行

在 `03-stacks-and-queues/01-stack/python/`：

```bash
python3 stack_demo.py
python3 -m unittest -v
```

