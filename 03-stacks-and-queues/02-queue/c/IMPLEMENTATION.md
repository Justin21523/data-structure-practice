# 實作說明（C）— 佇列（Queue）

## 目標

本單元用 C 實作 `array_queue`（naive queue），並回傳可測試的成本：

- `copied`：擴容時複製元素次數
- `moved`：`dequeue` 時左移元素次數（`size-1`）

## 檔案

- `queue.h/.c`：`array_queue` + 操作函式（含 copied/moved）
- `queue_main.c`：示範程式
- `test_queue.c`：最小測試器（assert）
- `Makefile`：`make run` / `make test`

## 核心概念

### dequeue 的 moved（左移）

```c
int moved = queue->size - 1;
for (int i = 1; i < queue->size; i++) {
    queue->data[i - 1] = queue->data[i];
}
```

> 下一單元 `03-circular-queue` 會用環狀索引避免 shift，讓 `dequeue` 不再是 O(n)。

## 如何執行

在 `03-stacks-and-queues/02-queue/c/`：

```bash
make
make run
make test
```

