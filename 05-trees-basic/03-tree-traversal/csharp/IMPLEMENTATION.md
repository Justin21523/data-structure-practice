# 實作說明（C#）— 樹走訪（Tree Traversal）

## 目標

本單元把 DFS 三種走訪寫出「遞迴」與「迭代」兩種版本，並用測試確認結果一致：

- `PreorderRecursive / PreorderIterative`
- `InorderRecursive / InorderIterative`
- `PostorderRecursive / PostorderIterative`
- `LevelOrder`（BFS）

## 檔案

- `TreeTraversalDemo.cs`：`Node` / `BinaryTree` + traversal 實作
- `Program.cs`：demo + `--test` 測試模式（不依賴外部套件）
- `TreeTraversal.csproj`：.NET 專案檔
- `NuGet.config` + `nuget-offline/`：離線友善設定

## 關鍵做法

### 1) inorder iterative：一路往左 push

用 `List<Node>` 當 stack，走到最左後 pop/visit，再切到 right。

### 2) postorder iterative：`Frame(node, visited)`

用 visited flag 確保 node 在左右子樹之後才輸出（每個 node 可能入棧兩次）。

### 3) BFS：queue + head index

queue 用 `List<Node>` 搭配 `head` 指標，避免 `RemoveAt(0)` 造成 O(n) 移動。

## 如何執行

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

