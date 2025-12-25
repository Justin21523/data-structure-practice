# 02 二元搜尋樹（Binary Search Tree, BST）

本單元介紹「**二元搜尋樹（BST）**」：在二元樹的基礎上，多了一個排序性質（ordering property），讓我們能用類似二分搜尋的方式做查找。

## BST 性質（重點）

對任意節點 `x`：

- 左子樹所有節點值 **< x.value**
- 右子樹所有節點值 **> x.value**

因此 **inorder（中序）走訪** 會得到「遞增排序」的序列。

## 本單元會做什麼

我們會手刻 `BinarySearchTree`（不使用語言內建 tree），並提供：

- `insert(key)`：插入（本 repo 採 **不允許重複 key**；重複插入回傳 `false` / `not inserted`）
- `contains(key)`：搜尋 key 是否存在
- `delete(key)`：刪除（涵蓋 0 子、1 子、2 子三種情況；2 子情況使用 **inorder successor**）
- `min()` / `max()`：取得最小/最大值（空樹回傳 `null/None`）
- `size()` / `height()`：節點數與高度  
  - 本單元 `height` 定義為「**邊數**」：空樹 `-1`、葉 `0`
- `inorder()`：回傳值序列（用來驗證 BST 的排序性質）

## 程式碼（各語言）

### Python

在 `05-trees-basic/02-binary-search-tree/python/`：

```bash
python3 bst_demo.py
python3 -m unittest -v
```

### Java

在 `05-trees-basic/02-binary-search-tree/java/`：

```bash
javac BstDemo.java BstDemoTest.java
java BstDemo
java BstDemoTest
```

### JavaScript（Node.js）

在 `05-trees-basic/02-binary-search-tree/javascript/`：

```bash
node bstDemo.js
node --test
```

### C

在 `05-trees-basic/02-binary-search-tree/c/`：

```bash
make run
make test
make clean
```

### C++

在 `05-trees-basic/02-binary-search-tree/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/bst_demo
ctest --test-dir build --output-on-failure
```

### C#

在 `05-trees-basic/02-binary-search-tree/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

