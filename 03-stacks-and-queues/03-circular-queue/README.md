# 03 環狀佇列（Circular Queue）

本單元介紹「**環狀佇列（circular queue）**」：用固定大小的陣列搭配 `head`（front index）與 `size` 來表示佇列，並用取餘數（mod）處理尾端繞回。

## 重點觀念

- `head` 指向前端元素的位置
- `tailIndex = (head + size) % capacity` 是下一個要 enqueue 的位置
- `enqueue` / `dequeue` 都能做到 O(1)：
  - `dequeue` 只移動 `head`，**不再左移整個陣列**
- 若容量不足仍使用倍增擴容（doubling），並把元素「依佇列順序」複製到新 buffer（讓 `head` 重設為 0）

本單元沿用上一單元的成本度量：

- `copied`：擴容時複製了多少元素
- `moved`：`dequeue` 時左移了多少元素（本單元應該永遠是 0）

## 本單元會做什麼

我們會自己實作 `CircularQueue`（不使用語言內建 queue），並提供：

- 示範程式：印出 enqueue 的 copied 表格 + dequeue moved（應為 0）表格 + FIFO 小例子
- 測試程式：驗證 FIFO 正確性、wrap-around 行為、以及 moved=0
- 說明文件：`IMPLEMENTATION.md`

## 程式碼（各語言）

### Python

在 `03-stacks-and-queues/03-circular-queue/python/`：

```bash
python3 circular_queue_demo.py
python3 -m unittest -v
```

### Java

在 `03-stacks-and-queues/03-circular-queue/java/`：

```bash
javac CircularQueueDemo.java CircularQueueDemoTest.java
java CircularQueueDemo
java CircularQueueDemoTest
```

### JavaScript（Node.js）

在 `03-stacks-and-queues/03-circular-queue/javascript/`：

```bash
node circularQueueDemo.js
node --test
```

### C

在 `03-stacks-and-queues/03-circular-queue/c/`：

```bash
make
make run
make test
```

### C++

在 `03-stacks-and-queues/03-circular-queue/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/circular_queue_demo
ctest --test-dir build
```

### C#

在 `03-stacks-and-queues/03-circular-queue/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```

