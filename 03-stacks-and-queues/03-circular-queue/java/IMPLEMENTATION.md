# 實作說明（Java）— 環狀佇列（Circular Queue）

## 目標

本單元用 `int[]` 手刻 `CircularQueue`，用環狀索引把 `dequeue` 左移成本消除：

- `moved`：永遠是 0（不做 shift）
- `copied`：只在倍增擴容（doubling resize）時出現

## 檔案

- `CircularQueueDemo.java`：資料結構 + CLI（enqueue 成長表 + dequeue moved 表 + FIFO 小例子）
- `CircularQueueDemoTest.java`：最小測試器（不使用 JUnit）

## 核心概念

### 1) tailIndex 計算

```java
int tailIndex = (head + size) % capacity;
```

### 2) dequeue 不左移（moved=0）

```java
int removed = data[head];
head = (head + 1) % capacity;
size -= 1;
```

### 3) resize 複製要按「佇列順序」

```java
newData[i] = data[(head + i) % capacity];
```

## 如何執行

在 `03-stacks-and-queues/03-circular-queue/java/`：

```bash
javac CircularQueueDemo.java CircularQueueDemoTest.java
java CircularQueueDemo
java CircularQueueDemoTest
```

