# 實作說明（Python）— 雙端佇列（Deque）

## 目標

本單元用「環狀陣列（circular buffer）」手刻 `Deque`，讓兩端操作都能維持 **O(1)**（攤銷），並且把「搬移成本」降到 0：

- `moved`：永遠為 `0`（不做任何 shift）
- `copied`：只在擴容（doubling resize）時出現，且一次擴容會複製 `size` 個元素

## 檔案

- `deque_demo.py`：資料結構 + CLI（pushBack 成長表 + 雙端操作小例子）
- `test_deque_demo.py`：`unittest`（驗證正確性、wrap-around、head≠0 時 resize 保序、以及 moved=0）

## 核心概念

### 1) 用 head + size 表示整個 deque

- `head`：front 的實體索引（buffer 中哪個格子是最前面）
- `size`：目前元素數量
- `capacity`：buffer 大小

把「邏輯位置（第 i 個）」映射到「實體索引」：

```python
def _index_at(self, offset: int) -> int:
    return (self._head + offset) % self._capacity
```

### 2) pushBack / pushFront 都只改索引（moved=0）

pushBack 寫在尾端（tailIndex = head + size）：

```python
tail_idx = self._index_at(self._size)
self._data[tail_idx] = value
self._size += 1
```

pushFront 先把 head 往左「繞回去」，再寫入：

```python
self._head = (self._head - 1 + self._capacity) % self._capacity
self._data[self._head] = value
self._size += 1
```

### 3) popFront / popBack 都是 O(1)

popFront 只把 head 往右移：

```python
removed = self._data[self._head]
self._head = (self._head + 1) % self._capacity
self._size -= 1
```

popBack 只算出最後一格並縮小 size：

```python
tail_idx = self._index_at(self._size - 1)
removed = self._data[tail_idx]
self._size -= 1
```

### 4) resize 必須依「deque 順序」複製，並把 head 重設為 0

即使 head 不在 0，也要用 `i=0..size-1` 的邏輯順序搬到新陣列：

```python
for i in range(self._size):
    new_data[i] = self._data[self._index_at(i)]
self._head = 0
```

## 如何執行

在 `03-stacks-and-queues/04-deque/python/`：

```bash
python3 deque_demo.py
python3 -m unittest -v
```

