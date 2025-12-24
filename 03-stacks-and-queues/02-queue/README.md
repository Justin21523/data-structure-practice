# 02 佇列（Queue）

本單元介紹「**佇列（queue）**」：一種 FIFO（First In, First Out）的受限線性資料結構，元素從尾端加入（enqueue），從前端移除（dequeue）。

## 重點觀念

- `enqueue(x)`：把元素加到尾端（rear）
- `dequeue()`：移除並回傳前端（front）元素
- `peek()` / `front()`：只看前端、不移除

本單元會先用「**陣列**」做一個教學版 `ArrayQueue`，刻意讓你看到「為什麼需要環狀佇列」：

- `enqueue`：尾端新增，容量不足時倍增擴容 → 攤銷 O(1)
- `dequeue`：從 index 0 移除後，需要把剩下元素全部左移 → O(n)

我們把成本量化成可測試的數字：

- `copied`：擴容時複製了多少元素
- `moved`：`dequeue` 時左移了多少元素（通常是 `size-1`）

> 下一單元 `03-circular-queue` 會用環狀索引把 `moved` 降到 0，讓 `dequeue` 回到 O(1)。

## 本單元會做什麼

我們會自己實作 `ArrayQueue`（不使用語言內建 queue），並提供：

- 示範程式：印出 enqueue/dequeue 的 copied/moved
- 測試程式：驗證 FIFO 正確性 + 成本公式
- 說明文件：`IMPLEMENTATION.md`

## 程式碼（各語言）

### Python

在 `03-stacks-and-queues/02-queue/python/`：

```bash
python3 queue_demo.py
python3 -m unittest -v
```

### Java

在 `03-stacks-and-queues/02-queue/java/`：

```bash
javac QueueDemo.java QueueDemoTest.java
java QueueDemo
java QueueDemoTest
```

### JavaScript（Node.js）

在 `03-stacks-and-queues/02-queue/javascript/`：

```bash
node queueDemo.js
node --test
```

### C

在 `03-stacks-and-queues/02-queue/c/`：

```bash
make
make run
make test
```

### C++

在 `03-stacks-and-queues/02-queue/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/queue_demo
ctest --test-dir build
```

### C#

在 `03-stacks-and-queues/02-queue/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```

