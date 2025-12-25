# 04 雙端佇列（Deque）

本單元介紹「**雙端佇列（deque, double-ended queue）**」：允許在**兩端**都能插入與刪除的受限線性資料結構。

## 重點觀念

- `pushFront(x)` / `popFront()`：在前端操作
- `pushBack(x)` / `popBack()`：在尾端操作
- `peekFront()` / `peekBack()`：只看兩端、不移除

若用「環狀陣列（circular buffer）」實作，只要維護：

- `head`：前端位置
- `size`：目前元素數量
- `capacity`：buffer 大小

就能做到兩端操作都是 O(1)，且不需要像 naive array 版本那樣搬移元素。

本單元沿用成本度量（方便與前兩單元比較）：

- `copied`：擴容時複製了多少元素（doubling resize copies）
- `moved`：操作中額外搬移元素的次數（本單元應該永遠是 0）

## 本單元會做什麼

我們會自己實作 `Deque`（不使用語言內建 deque），並提供：

- 示範程式：印出 push 成長表格（copied）+ 一個雙端操作範例
- 測試程式：驗證正確性、wrap-around、resize 保序、以及 moved=0
- 說明文件：`IMPLEMENTATION.md`

## 程式碼（各語言）

### Python

在 `03-stacks-and-queues/04-deque/python/`：

```bash
python3 deque_demo.py
python3 -m unittest -v
```

### Java

在 `03-stacks-and-queues/04-deque/java/`：

```bash
javac DequeDemo.java DequeDemoTest.java
java DequeDemo
java DequeDemoTest
```

### JavaScript（Node.js）

在 `03-stacks-and-queues/04-deque/javascript/`：

```bash
node dequeDemo.js
node --test
```

### C

在 `03-stacks-and-queues/04-deque/c/`：

```bash
make
make run
make test
```

### C++

在 `03-stacks-and-queues/04-deque/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/deque_demo
ctest --test-dir build
```

### C#

在 `03-stacks-and-queues/04-deque/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```

