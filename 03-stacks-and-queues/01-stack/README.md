# 01 堆疊（Stack）

本單元介紹「**堆疊（stack）**」：一種 LIFO（Last In, First Out）的受限線性資料結構，只允許在同一端（top）進行操作。

## 重點觀念

- `push(x)`：把元素放到頂端（top）
- `pop()`：移除並回傳頂端元素
- `peek()` / `top()`：只看頂端、不移除
- 典型時間複雜度：
  - `push`：O(1) **攤銷**（若用動態陣列擴容）
  - `pop` / `peek`：O(1)

本單元會用「動態陣列（dynamic array）」作為底層儲存，並量化擴容成本：

- `copied`：擴容時複製了多少元素（resize copy count）

## 本單元會做什麼

我們會自己實作 `ArrayStack`（不使用語言內建 stack），並提供：

- 示範程式：印出 push/pop/peek 的行為與擴容時的 copied
- 測試程式：驗證 LIFO 正確性 + copied 計數
- 說明文件：`IMPLEMENTATION.md`

## 程式碼（各語言）

### Python

在 `03-stacks-and-queues/01-stack/python/`：

```bash
python3 stack_demo.py
python3 -m unittest -v
```

### Java

在 `03-stacks-and-queues/01-stack/java/`：

```bash
javac StackDemo.java StackDemoTest.java
java StackDemo
java StackDemoTest
```

### JavaScript（Node.js）

在 `03-stacks-and-queues/01-stack/javascript/`：

```bash
node stackDemo.js
node --test
```

### C

在 `03-stacks-and-queues/01-stack/c/`：

```bash
make
make run
make test
```

### C++

在 `03-stacks-and-queues/01-stack/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/stack_demo
ctest --test-dir build
```

### C#

在 `03-stacks-and-queues/01-stack/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```

