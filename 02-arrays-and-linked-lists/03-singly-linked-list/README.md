# 03 單向鏈結串列（Singly Linked List）

本單元介紹「**單向鏈結串列（singly linked list）**」：每個節點（node）存放資料與 `next` 指標，節點在記憶體中**不需要連續**。

## 重點觀念

- `pushFront`（頭插）/ `popFront`（頭刪）：O(1)
- `pushBack`（尾插）：若**沒有 tail 指標**，需要走到尾端 → O(n)
- `get(i)` / `insertAt(i)` / `removeAt(i)`：需要從 head 開始走訪 → O(n)
- 走訪成本可用「**next 指標跳躍次數（hops）**」量化：每做一次 `current = current.next` 計 1 hop

> 本單元刻意只維護 `head`（不維護 `tail`），用來凸顯「為何鏈結串列不適合隨機存取、尾端插入為何可能是 O(n)」。

## 本單元會做什麼

我們會自己實作 `SinglyLinkedList`（不使用語言內建的 linked list），並讓每次操作回傳可測試的成本：

- `hops`：操作中走訪 `next` 的次數（0 代表完全不需要走訪，例如頭插）

每個語言資料夾皆提供：

- 示範程式（印出不同操作的 hops）
- 測試程式（驗證資料正確性 + hops 計數公式）
- 說明文件：`IMPLEMENTATION.md`

## 程式碼（各語言）

### Python

在 `02-arrays-and-linked-lists/03-singly-linked-list/python/`：

```bash
python3 singly_linked_list_demo.py
python3 -m unittest -v
```

### Java

在 `02-arrays-and-linked-lists/03-singly-linked-list/java/`：

```bash
javac SinglyLinkedListDemo.java SinglyLinkedListDemoTest.java
java SinglyLinkedListDemo
java SinglyLinkedListDemoTest
```

### JavaScript（Node.js）

在 `02-arrays-and-linked-lists/03-singly-linked-list/javascript/`：

```bash
node singlyLinkedListDemo.js
node --test
```

### C

在 `02-arrays-and-linked-lists/03-singly-linked-list/c/`：

```bash
make
make run
make test
```

### C++

在 `02-arrays-and-linked-lists/03-singly-linked-list/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/singly_linked_list_demo
ctest --test-dir build
```

### C#

在 `02-arrays-and-linked-lists/03-singly-linked-list/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```

