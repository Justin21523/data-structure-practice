# 02 動態陣列（Dynamic Array）

本單元介紹「**動態陣列（dynamic array）**」：底層仍是連續記憶體，但容量不足時會**擴容（resize）**，常見策略是 **容量倍增（doubling）**。

## 重點觀念

- `get/set`：O(1)
- `append/push`：平均/攤銷（amortized）O(1)，但擴容那一次可能是 O(n)
- `insertAt/removeAt`：除了可能擴容外，還需要 shift，因此通常是 O(n)

> 若你想看更完整的「攤銷分析」推導，請搭配第一章的 `01-introduction-and-complexity/02-amortized-analysis/`。

## 本單元會做什麼

我們會自己實作 `DynamicArray`（不使用語言內建的動態容器），並用可計數的成本來呈現：

- `copies`：擴容時搬移（copy）既有元素次數
- `moved`：插入/刪除時 shift 的搬移次數

每個語言資料夾皆提供：

- 示範程式（印出 capacity 變化與 copies/moved）
- 測試程式（驗證資料正確性 + 擴容/搬移計數）
- 說明文件：`IMPLEMENTATION.md`

## 程式碼（各語言）

### Python

在 `02-arrays-and-linked-lists/02-dynamic-array/python/`：

```bash
python3 dynamic_array_demo.py
python3 -m unittest -v
```

### Java

在 `02-arrays-and-linked-lists/02-dynamic-array/java/`：

```bash
javac DynamicArrayDemo.java DynamicArrayDemoTest.java
java DynamicArrayDemo
java DynamicArrayDemoTest
```

### JavaScript（Node.js）

在 `02-arrays-and-linked-lists/02-dynamic-array/javascript/`：

```bash
node dynamicArrayDemo.js
node --test
```

### C

在 `02-arrays-and-linked-lists/02-dynamic-array/c/`：

```bash
make
make run
make test
```

### C++

在 `02-arrays-and-linked-lists/02-dynamic-array/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/dynamic_array_demo
ctest --test-dir build
```

### C#

在 `02-arrays-and-linked-lists/02-dynamic-array/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```

