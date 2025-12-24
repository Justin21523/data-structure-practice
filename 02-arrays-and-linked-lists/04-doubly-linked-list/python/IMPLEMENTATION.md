# 實作說明（Python）— 雙向鏈結串列（Doubly Linked List）

## 目標

本單元用教學版 `DoublyLinkedList` 實作雙向鏈結串列，並把「走訪成本」量化為可測試的數字：

- `hops`：**每走一次指標**就 +1（`current = current.next` 或 `current = current.prev`）

我們刻意同時維護 `head` 與 `tail`，讓你清楚看到：

- `push_front / push_back / pop_front / pop_back` 都是 O(1)（`hops = 0`）
- `get/insert_at/remove_at` 仍要走訪，但可從 **較近端**（head 或 tail）開始走，讓 `hops = min(i, size-1-i)`

## 檔案

- `doubly_linked_list_demo.py`：`DoublyLinkedList` + CLI（印出 push/pop 與 get(i) 的 hops 表格）
- `test_doubly_linked_list_demo.py`：`unittest`（驗證資料正確性 + hops 計數）

## 核心概念

### 1) 兩端操作為 O(1)

有 `tail` 時，尾端插入/刪除不需要走訪：

- `push_back`：直接用 `tail.next = newTail` 串上去
- `pop_back`：直接把 `tail = tail.prev`，並更新 `tail.next = None`

因此本單元規定其成本 `hops = 0`。

### 2) get/insert/remove 的 hops（從較近端開始）

定位 index `i` 時：

- 若 `i < size/2`：從 head 往右走 `i` 步
- 否則：從 tail 往左走 `size-1-i` 步

對應程式片段：

```python
if index < self._size // 2:
    for _ in range(index):
        current = current.next
        hops += 1
else:
    steps = self._size - 1 - index
    for _ in range(steps):
        current = current.prev
        hops += 1
```

因此 `hops = min(i, size-1-i)`。

## 如何執行

在 `02-arrays-and-linked-lists/04-doubly-linked-list/python/`：

```bash
python3 doubly_linked_list_demo.py
python3 -m unittest -v
```

