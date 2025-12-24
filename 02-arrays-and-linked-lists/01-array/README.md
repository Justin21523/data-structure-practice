# 01 陣列（Array / Static Array）

本單元聚焦在「**靜態陣列（static array）**」：容量固定、記憶體連續（contiguous），因此 **隨機存取 O(1)**，但在中間插入/刪除需要搬移（shift）元素，通常是 **O(n)**。

## 本單元會做什麼

我們會自己實作一個「固定容量陣列（FixedArray）」來模擬真正的靜態陣列行為，並且用**可計數的搬移次數（moved elements）**來觀察成本：

- `get(i)` / `set(i, v)`：O(1)
- `append(v)`：O(1)（但若已滿會失敗）
- `insertAt(i, v)`：需要右移元素，搬移次數約為 `size - i` → O(n)
- `removeAt(i)`：需要左移元素，搬移次數約為 `size - i - 1` → O(n)
- `indexOf(v)`：線性搜尋 O(n)

每個語言資料夾皆提供：

- 示範程式（印出「插入/刪除位置」與搬移次數）
- 測試程式（驗證資料正確性 + 搬移次數）
- 說明文件：`IMPLEMENTATION.md`

## 程式碼（各語言）

### Python

在 `02-arrays-and-linked-lists/01-array/python/`：

```bash
python3 fixed_array_demo.py
python3 -m unittest -v
```

### Java

在 `02-arrays-and-linked-lists/01-array/java/`：

```bash
javac FixedArrayDemo.java FixedArrayDemoTest.java
java FixedArrayDemo
java FixedArrayDemoTest
```

### JavaScript（Node.js）

在 `02-arrays-and-linked-lists/01-array/javascript/`：

```bash
node fixedArrayDemo.js
node --test
```

### C

在 `02-arrays-and-linked-lists/01-array/c/`：

```bash
make
make run
make test
```

### C++

在 `02-arrays-and-linked-lists/01-array/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/fixed_array_demo
ctest --test-dir build
```

### C#

在 `02-arrays-and-linked-lists/01-array/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```

