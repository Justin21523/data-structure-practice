# 實作說明（Python）— 環狀鏈結串列（Circular Linked List）

## 目標

本單元實作「單向環狀鏈結串列」`CircularLinkedList`，並用可測試的成本來呈現操作複雜度：

- `hops`：每做一次 `current = current.next` 就 +1

## 檔案

- `circular_linked_list_demo.py`：資料結構 + CLI 示範（印出基本操作與 rotate 的 hops）
- `test_circular_linked_list_demo.py`：`unittest`（驗證資料正確性 + hops 公式）

## 核心概念

### 1) 只維護 tail：`head = tail.next`

我們不額外存 `head`，而是用 `tail.next` 表示：

```python
def _head(self) -> Optional[_Node]:
    if self._tail is None:
        return None
    return self._tail.next
```

### 2) 走訪一定要用 size 限制

因為是「環」，沒有 `null` 結尾；`to_list()` 與搜尋都用 `range(self._size)`，避免無限迴圈。

### 3) 典型成本公式

- `push_front / push_back / pop_front`：不走訪 → `hops = 0`
- `pop_back`：單向需要找 tail 前一個節點 → `hops = max(0, n-2)`
- `rotate(k)`：把 tail 往前走 `k % size` 步 → `hops = k % size`（且 `k < 0` 會丟 `ValueError`）

## 如何執行

在 `02-arrays-and-linked-lists/05-circular-linked-list/python/`：

```bash
python3 circular_linked_list_demo.py
python3 -m unittest -v
```

