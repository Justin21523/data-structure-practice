# 01 AVL Tree（自平衡二元搜尋樹）

本單元介紹 **AVL Tree**：一種「自平衡（self-balancing）」的二元搜尋樹（Binary Search Tree, BST）。  
AVL 的核心規則是：對任意節點，左右子樹高度差（balance factor）必須落在 `-1..1`，因此在插入/刪除後需要透過 **旋轉（rotation）** 重新平衡。

## 本單元約定（與前面章節一致）

- Key 型別：整數（int）。  
- **不允許重複 key**：重複插入回傳 `false`。  
- `height` 定義為「**邊數**」：空樹 `-1`、葉節點 `0`。  
- 提供 `validate()`：檢查 BST ordering、每個節點 height 正確、以及 AVL 平衡條件成立。

## 會實作的操作

- `insert(key)` / `delete(key)` / `contains(key)`
- `min()` / `max()` / `size()` / `height()` / `inorder()`
- `validate()`：用來在測試中快速驗證「仍然是 AVL」

## 程式碼（各語言）

### Python

在 `06-balanced-trees/01-avl-tree/python/`：

```bash
python3 avl_tree_demo.py
python3 -m unittest -v
```

### Java

在 `06-balanced-trees/01-avl-tree/java/`：

```bash
javac AvlTreeDemo.java AvlTreeDemoTest.java
java AvlTreeDemo
java AvlTreeDemoTest
```

### JavaScript（Node.js）

在 `06-balanced-trees/01-avl-tree/javascript/`：

```bash
node avlTreeDemo.js
node --test
```

### C

在 `06-balanced-trees/01-avl-tree/c/`：

```bash
make run
make test
make clean
```

### C++

在 `06-balanced-trees/01-avl-tree/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/avl_demo
ctest --test-dir build --output-on-failure
```

### C#

在 `06-balanced-trees/01-avl-tree/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

