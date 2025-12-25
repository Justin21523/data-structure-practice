# 實作說明（Java）— 雙端佇列（Deque）

## 目標

本單元用「環狀陣列（circular buffer）」手刻 `Deque`，支援前後端操作並維持 **O(1)**（攤銷）：

- `moved`：永遠為 `0`（不做 shift）
- `copied`：只有擴容（doubling）才會出現，且每次 resize 會複製 `size` 個元素

## 檔案

- `DequeDemo.java`：資料結構 + CLI（pushBack 成長表 + 雙端操作小例子）
- `DequeDemoTest.java`：測試（驗證正確性、wrap-around、head≠0 resize 保序、moved=0）

## 核心概念

### 1) head + size 的表示法

- `head`：front 的實體索引
- `size`：元素數量
- `capacity`：陣列容量

邏輯位置 `offset` 對應到實體索引：

```java
private int indexAt(int offset) {
    return (this.head + offset) % this.capacity;
}
```

### 2) 兩端 push 都只更新索引（moved=0）

pushBack 寫到 tail：

```java
int tailIndex = indexAt(this.size);
this.data[tailIndex] = value;
this.size += 1;
```

pushFront 先把 head 往左移一格：

```java
this.head = (this.head - 1 + this.capacity) % this.capacity;
this.data[this.head] = value;
this.size += 1;
```

### 3) resize：依 deque 順序複製並 head=0

當 `size == capacity` 時擴成 `capacity * 2`，並把元素依 `i=0..size-1` 搬到新陣列的 `0..size-1`：

```java
for (int i = 0; i < this.size; i++) {
    newData[i] = this.data[indexAt(i)];
}
this.head = 0;
```

## 如何執行

在 `03-stacks-and-queues/04-deque/java/`：

```bash
javac DequeDemo.java DequeDemoTest.java
java DequeDemo
java DequeDemoTest
```

