# 實作說明（C）— 環狀佇列（Circular Queue）

## 目標

本單元用 C 實作 `circular_queue`（head + size + capacity），並量化成本：

- `moved`：永遠是 0（dequeue 不做 shift）
- `copied`：只在倍增擴容時出現

## 檔案

- `circular_queue.h/.c`：`circular_queue_*` API（含 copied/moved）
- `circular_queue_main.c`：示範程式
- `test_circular_queue.c`：最小測試器（assert）
- `Makefile`：`make run` / `make test`

## 核心概念

### 1) tailIndex 計算

```c
int tail_index = (queue->head + queue->size) % queue->capacity;
```

### 2) dequeue 不左移（moved=0）

```c
queue->head = (queue->head + 1) % queue->capacity;
queue->size -= 1;
```

### 3) resize 依佇列順序複製

```c
new_data[i] = old_data[(head + i) % capacity];
```

## 如何執行

在 `03-stacks-and-queues/03-circular-queue/c/`：

```bash
make
make run
make test
```

