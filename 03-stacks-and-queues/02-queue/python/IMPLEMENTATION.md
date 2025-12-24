# 實作說明（Python）— 佇列（Queue）

## 目標

本單元用「教學版」`ArrayQueue` 先做出一個 **naive queue**：`dequeue` 會把資料全部左移，刻意讓你看到為什麼下一單元需要環狀佇列（circular queue）。

我們把成本量化成可測試的數字：

- `copied`：擴容時複製了多少元素（doubling resize copies）
- `moved`：`dequeue` 後左移了多少元素（shift-left moves）

## 檔案

- `queue_demo.py`：`ArrayQueue` + CLI（enqueue 成長表 + dequeue moved 表 + FIFO 小例子）
- `test_queue_demo.py`：`unittest`（驗證 FIFO 正確性 + `copied/moved` 公式）

## 核心概念

### 1) enqueue 的 copied（倍增擴容）

當 `size == capacity` 時配置新 buffer，複製 `0..size-1`：

```python
for i in range(self._size):
    new_data[i] = self._data[i]
    copied += 1
```

### 2) dequeue 的 moved（左移）

移除 `data[0]` 後把剩下元素左移一格，因此：

- `moved = size - 1`

```python
for i in range(1, self._size):
    self._data[i - 1] = self._data[i]
```

> 下一單元 `03-circular-queue` 會把 moved 降到 0（用環狀索引避免 shift）。

## 如何執行

在 `03-stacks-and-queues/02-queue/python/`：

```bash
python3 queue_demo.py
python3 -m unittest -v
```

