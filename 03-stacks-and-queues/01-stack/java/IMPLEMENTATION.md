# 實作說明（Java）— 堆疊（Stack）

## 目標

本單元用 `int[]` 手刻 `ArrayStack`，並回傳可測試的擴容成本：

- `copied`：擴容時複製既有元素次數（resize copy count）

`top` 位於陣列尾端（index = `size-1`），所以 `push/pop/peek` 都是 O(1)；只有 `push` 在擴容當下會出現 O(n) 複製，但整體是攤銷 O(1)（doubling）。

## 檔案

- `StackDemo.java`：`ArrayStack` + CLI（印出 push 成長表格與 LIFO 小例子）
- `StackDemoTest.java`：最小測試器（不使用 JUnit）

## 核心概念

### 倍增擴容與 copied

```java
int[] newData = new int[newCapacity];
for (int i = 0; i < this.size; i++) {
    newData[i] = this.data[i];
    copied += 1;
}
```

## 如何執行

在 `03-stacks-and-queues/01-stack/java/`：

```bash
javac StackDemo.java StackDemoTest.java
java StackDemo
java StackDemoTest
```

