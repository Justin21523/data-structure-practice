# 實作說明（Python）— 單向鏈結串列（Singly Linked List）

## 目標

本單元用教學版 `SinglyLinkedList` 實作單向鏈結串列，並把「走訪成本」量化為可測試的數字：

- `hops`：**做了幾次 `current = current.next`**（也就是沿著 `next` 指標走了幾步）

我們刻意 **不維護 tail 指標**，讓你清楚看到：

- `push_front` 為何是 O(1)
- `push_back` 為何可能是 O(n)
- `get/insert_at/remove_at` 為何都需要走訪（O(n)）

## 檔案

- `singly_linked_list_demo.py`：`SinglyLinkedList` + CLI（印出 hops 表格）
- `test_singly_linked_list_demo.py`：`unittest`（驗證資料正確性 + hops 計數）

## 核心概念（hops）

### 1) 尾端插入 `push_back`

因為沒有 tail，我們必須走到最後一個節點：

```python
while current.next is not None:
    current = current.next
    hops += 1
```

若串列目前長度為 `n`，則：

- `n == 0`：`hops = 0`
- `n > 0`：`hops = n - 1`

### 2) 隨機存取 `get_with_cost(i)`

必須從 head 走 `i` 步才能到 index `i`：

- `hops = i`

## 如何執行

在 `02-arrays-and-linked-lists/03-singly-linked-list/python/`：

```bash
python3 singly_linked_list_demo.py
python3 -m unittest -v
```

