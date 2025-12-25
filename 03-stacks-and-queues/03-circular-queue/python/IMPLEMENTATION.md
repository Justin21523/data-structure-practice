# 實作說明（Python）— 環狀佇列（Circular Queue）

## 目標

本單元用 `CircularQueue` 實作「環狀佇列」，把上一單元 `ArrayQueue` 的 `dequeue` 左移成本消除掉：

- `moved`：本單元應該永遠是 0（因為 dequeue 不做 shift）
- `copied`：只有在擴容（doubling）時才會出現

## 檔案

- `circular_queue_demo.py`：資料結構 + CLI（enqueue 成長表 + dequeue moved 表 + FIFO 小例子）
- `test_circular_queue_demo.py`：`unittest`（驗證 FIFO、wrap-around、resize 時順序保持、moved=0）

## 核心概念

### 1) head + size 表示法

- `head`：front 的實體索引
- `tailIndex = (head + size) % capacity`：下一個 enqueue 寫入的位置

### 2) dequeue 是 O(1) 且 moved=0

```python
removed = self._data[self._head]
self._head = (self._head + 1) % self._capacity
self._size -= 1
```

### 3) resize 要「依佇列順序」複製

即使 head 不在 0，也要按照 `i=0..size-1` 的邏輯順序搬到新 buffer：

```python
old_idx = (self._head + i) % self._capacity
new_data[i] = self._data[old_idx]
```

搬完後把 `head` 重設為 0，讓資料在新 buffer 內是連續排列。

## 如何執行

在 `03-stacks-and-queues/03-circular-queue/python/`：

```bash
python3 circular_queue_demo.py
python3 -m unittest -v
```

