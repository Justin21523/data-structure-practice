# 05 環狀鏈結串列（Circular Linked List）

本單元介紹「**環狀鏈結串列（circular linked list）**」：最後一個節點的 `next` 會指回第一個節點，因此結構上**沒有 `null` 結尾**。

## 重點觀念

- 常見做法是只維護 `tail`：
  - `head` 可以用 `tail.next` 表示
  - `pushBack` / `pushFront` / `popFront` 都能做到 O(1)
- 但若是「單向」環狀串列，要 `popBack` 仍需要找到 tail 的前一個節點 → O(n)
- 旋轉（rotate）是環狀串列的典型操作：把 `tail` 往前走幾步，就等於把 head 往前移動（視覺上整串被旋轉）

本單元用可測試的成本來呈現：

- `hops`：走訪 `next` 的次數（每次 `current = current.next` 算 1 hop）

## 本單元會做什麼

我們會自己實作「單向環狀鏈結串列」`CircularLinkedList`（不使用內建容器），並提供：

- 示範程式：印出 push/pop/rotate 的 hops 與旋轉後的序列
- 測試程式：驗證資料正確性 + hops 計數公式
- 說明文件：`IMPLEMENTATION.md`

## 程式碼（各語言）

### Python

在 `02-arrays-and-linked-lists/05-circular-linked-list/python/`：

```bash
python3 circular_linked_list_demo.py
python3 -m unittest -v
```

### Java

在 `02-arrays-and-linked-lists/05-circular-linked-list/java/`：

```bash
javac CircularLinkedListDemo.java CircularLinkedListDemoTest.java
java CircularLinkedListDemo
java CircularLinkedListDemoTest
```

### JavaScript（Node.js）

在 `02-arrays-and-linked-lists/05-circular-linked-list/javascript/`：

```bash
node circularLinkedListDemo.js
node --test
```

### C

在 `02-arrays-and-linked-lists/05-circular-linked-list/c/`：

```bash
make
make run
make test
```

### C++

在 `02-arrays-and-linked-lists/05-circular-linked-list/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/circular_linked_list_demo
ctest --test-dir build
```

### C#

在 `02-arrays-and-linked-lists/05-circular-linked-list/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```

