# 04 雙向鏈結串列（Doubly Linked List）

本單元介紹「**雙向鏈結串列（doubly linked list）**」：每個節點（node）同時持有 `prev` 與 `next` 指標，因此可以**往前/往後**走訪。

## 重點觀念

- 維護 `head` 與 `tail`：
  - `pushFront` / `popFront`：O(1)
  - `pushBack` / `popBack`：O(1)（相較單向串列若沒有 tail，尾端操作可能是 O(n)）
- `get(i)` / `insertAt(i)` / `removeAt(i)`：仍然需要走訪 → O(n)
- 但雙向串列能「**從 head 或 tail 擇一較近端開始走**」，走訪步數可用 `min(i, size-1-i)` 直覺理解

本單元同樣用可測試的成本來呈現：

- `hops`：走訪指標的次數（每次 `current = current.next` 或 `current = current.prev` 都算 1 hop）

## 本單元會做什麼

我們會自己實作 `DoublyLinkedList`（不使用語言內建 linked list），並提供：

- 示範程式：印出 `push/pop` 與 `get(i)` 的 hops 表格
- 測試程式：驗證資料正確性 + hops 計數公式
- 說明文件：`IMPLEMENTATION.md`

## 程式碼（各語言）

### Python

在 `02-arrays-and-linked-lists/04-doubly-linked-list/python/`：

```bash
python3 doubly_linked_list_demo.py
python3 -m unittest -v
```

### Java

在 `02-arrays-and-linked-lists/04-doubly-linked-list/java/`：

```bash
javac DoublyLinkedListDemo.java DoublyLinkedListDemoTest.java
java DoublyLinkedListDemo
java DoublyLinkedListDemoTest
```

### JavaScript（Node.js）

在 `02-arrays-and-linked-lists/04-doubly-linked-list/javascript/`：

```bash
node doublyLinkedListDemo.js
node --test
```

### C

在 `02-arrays-and-linked-lists/04-doubly-linked-list/c/`：

```bash
make
make run
make test
```

### C++

在 `02-arrays-and-linked-lists/04-doubly-linked-list/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/doubly_linked_list_demo
ctest --test-dir build
```

### C#

在 `02-arrays-and-linked-lists/04-doubly-linked-list/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```

