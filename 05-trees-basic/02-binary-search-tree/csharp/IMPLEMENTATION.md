# 實作說明（C#）— 二元搜尋樹（Binary Search Tree, BST）

## 目標

本單元用 C# 手刻 `BinarySearchTree`，練習 BST 的核心操作：

- `Insert(key)`：插入（本 repo **不允許重複 key**；重複插入回傳 `false`）
- `Contains(key)`：搜尋 key 是否存在
- `Delete(key)`：刪除（0 子 / 1 子 / 2 子；2 子使用 **inorder successor**）
- `Min()` / `Max()`：最小/最大值（空樹回傳 `null`）
- `Size()` / `Height()`：節點數與高度（`Height` 以邊數計：空樹 `-1`、葉 `0`）
- `Inorder()`：回傳「值序列」，用來驗證 BST 排序性質

## 檔案

- `BstDemo.cs`：`Node` / `BinarySearchTree` + `TreeSummary`
- `Program.cs`：demo + `--test` 測試模式（不依賴外部套件）
- `BinarySearchTree.csproj`：.NET 專案檔
- `NuGet.config` + `nuget-offline/`：離線友善設定

## 關鍵設計

### 1) delete：用 `DeleteResult` 回傳「新的子樹根」

刪除需要更新 parent 指標，所以用 helper 回傳：

```csharp
private readonly struct DeleteResult
{
    public Node? NewRoot { get; }
    public bool Removed { get; }
}
```

### 2) 2 子刪除：用 inorder successor（右子樹最小）

```csharp
int successorKey = MinKey(node.Right!);
node.Key = successorKey;
node.Right = DeleteSubtree(node.Right, successorKey).NewRoot;
```

## 如何執行

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

