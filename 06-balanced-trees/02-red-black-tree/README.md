# 02 Red-Black Tree（紅黑樹）

本單元介紹 **Red-Black Tree（紅黑樹）**：一種「自平衡（self-balancing）」的二元搜尋樹（Binary Search Tree, BST），透過節點顏色（Red/Black）與旋轉維持樹高在 `O(log n)`。

為了讓程式更容易閱讀、也更適合手刻練習，本 repo 在各語言實作會採用 **Left-Leaning Red-Black Tree（LLRB）** 這個等價變體：  
它仍滿足紅黑樹的性質，但用「紅連結向左」的規則，讓插入/刪除可以用較一致的遞迴寫法完成。

## 本單元約定（與前面章節一致）

- Key 型別：整數（int）。  
- **不允許重複 key**：重複插入回傳 `false`。  
- `height` 定義為「**邊數**」：空樹 `-1`、葉節點 `0`。  
- 提供 `validate()`：檢查 BST ordering + 紅黑樹（含 LLRB 變體）不變量。

## 會實作的操作

- `insert(key)` / `delete(key)` / `contains(key)`
- `min()` / `max()` / `size()` / `height()` / `inorder()`
- `validate()`：用來在測試中快速驗證「仍然是紅黑樹」

## 程式碼（各語言）

### Python

在 `06-balanced-trees/02-red-black-tree/python/`：

```bash
python3 red_black_tree_demo.py
python3 -m unittest -v
```

### Java

在 `06-balanced-trees/02-red-black-tree/java/`：

```bash
javac RedBlackTreeDemo.java RedBlackTreeDemoTest.java
java RedBlackTreeDemo
java RedBlackTreeDemoTest
```

### JavaScript（Node.js）

在 `06-balanced-trees/02-red-black-tree/javascript/`：

```bash
node redBlackTreeDemo.js
node --test
```

### C

在 `06-balanced-trees/02-red-black-tree/c/`：

```bash
make run
make test
make clean
```

### C++

在 `06-balanced-trees/02-red-black-tree/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/rbt_demo
ctest --test-dir build --output-on-failure
```

### C#

在 `06-balanced-trees/02-red-black-tree/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

