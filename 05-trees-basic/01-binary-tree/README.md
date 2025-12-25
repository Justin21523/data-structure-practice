# 01 二元樹（Binary Tree）

本單元介紹「**二元樹（binary tree）**」：每個節點最多有兩個子節點（left / right），但**不**包含排序性質（排序性質會在下一單元 BST 討論）。

## 重點觀念

- **節點（node）**：`value` + `left` + `right`
- **根（root）**：最上層節點
- **葉（leaf）**：沒有子節點的節點
- **高度（height）**：本單元 `height` 定義為「**從節點到最深葉節點的邊數**」  
  - 空樹 `height = -1`、單一節點 `height = 0`

## 本單元會做什麼

我們會手刻 `BinaryTree`（不使用語言內建 tree），並提供：

- `size()`：節點數
- `height()`：高度（邊數定義）
- `countLeaves()`：葉節點數
- DFS 走訪：`preorder / inorder / postorder`
- `fromLevelOrder(...)`：用 level-order 陣列（含 `null/None`）建樹，方便測試與出題

## 程式碼（各語言）

### Python

在 `05-trees-basic/01-binary-tree/python/`：

```bash
python3 binary_tree_demo.py
python3 -m unittest -v
```

### Java

在 `05-trees-basic/01-binary-tree/java/`：

```bash
javac BinaryTreeDemo.java BinaryTreeDemoTest.java
java BinaryTreeDemo
java BinaryTreeDemoTest
```

### JavaScript（Node.js）

在 `05-trees-basic/01-binary-tree/javascript/`：

```bash
node binaryTreeDemo.js
node --test
```

### C

在 `05-trees-basic/01-binary-tree/c/`：

```bash
make run
make test
make clean
```

### C++

在 `05-trees-basic/01-binary-tree/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/binary_tree_demo
ctest --test-dir build --output-on-failure
```

### C#

在 `05-trees-basic/01-binary-tree/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

