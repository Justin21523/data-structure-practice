# 03 樹走訪（Tree Traversal）

本單元專注在「**樹走訪（traversal）**」：把樹的節點依特定順序列出。你會同時看到 **遞迴（recursive）** 與 **迭代（iterative, 用 stack/queue）** 兩種寫法，並用測試確認它們產生的序列一致。

## 本單元會做什麼

我們會手刻一棵「一般二元樹」的 `Node`（value/left/right），並提供：

- DFS（三種順序）
  - `preorder`：root → left → right
  - `inorder`：left → root → right
  - `postorder`：left → right → root
- BFS
  - `level-order`：逐層由左到右

每種 DFS 會做兩個版本：

- `*_recursive`：最直觀（但深樹可能造成呼叫堆疊很深）
- `*_iterative`：用 **stack** 模擬遞迴（面試/考試常考）

## 方便測試的建樹方式

為了讓測試好寫，本單元提供 `fromLevelOrder([...])`：用 level-order 陣列（含 `null/None` 缺洞）建樹，索引規則：

- `left = 2*i + 1`
- `right = 2*i + 2`

## 程式碼（各語言）

### Python

在 `05-trees-basic/03-tree-traversal/python/`：

```bash
python3 tree_traversal_demo.py
python3 -m unittest -v
```

### Java

在 `05-trees-basic/03-tree-traversal/java/`：

```bash
javac TreeTraversalDemo.java TreeTraversalDemoTest.java
java TreeTraversalDemo
java TreeTraversalDemoTest
```

### JavaScript（Node.js）

在 `05-trees-basic/03-tree-traversal/javascript/`：

```bash
node treeTraversalDemo.js
node --test
```

### C

在 `05-trees-basic/03-tree-traversal/c/`：

```bash
make run
make test
make clean
```

### C++

在 `05-trees-basic/03-tree-traversal/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/tree_traversal_demo
ctest --test-dir build --output-on-failure
```

### C#

在 `05-trees-basic/03-tree-traversal/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

