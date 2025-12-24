# 實作說明（Java）— 佇列（Queue）

## 目標

本單元用 `int[]` 手刻 `ArrayQueue`（naive queue），並回傳可測試的成本：

- `copied`：擴容（doubling）時複製既有元素次數
- `moved`：`dequeue` 時左移元素次數（通常 `size-1`）

## 檔案

- `QueueDemo.java`：`ArrayQueue` + CLI（enqueue 成長表 + dequeue moved 表 + FIFO 小例子）
- `QueueDemoTest.java`：最小測試器（不使用 JUnit）

## 核心概念

### dequeue 的 moved（左移）

```java
int moved = this.size - 1;
for (int i = 1; i < this.size; i++) {
    this.data[i - 1] = this.data[i];
}
```

> 下一單元 `03-circular-queue` 會用環狀索引避免這個左移，讓 `dequeue` 回到 O(1)。

## 如何執行

在 `03-stacks-and-queues/02-queue/java/`：

```bash
javac QueueDemo.java QueueDemoTest.java
java QueueDemo
java QueueDemoTest
```

