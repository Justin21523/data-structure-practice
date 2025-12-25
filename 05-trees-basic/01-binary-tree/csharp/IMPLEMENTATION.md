# 實作說明（C#）— 二元樹（Binary Tree）

## 目標

本單元用 C# 手刻 `BinaryTree`（不使用內建 tree 結構），練習：

- 遞迴性質：`Size()` / `Height()` / `CountLeaves()`
- DFS 走訪：`Preorder / Inorder / Postorder`
- BFS 走訪：`LevelOrder()`（用 `head` 指標避免移動資料）
- `FromLevelOrder(int?[] values)`：用含 `null` 的 level-order 陣列快速建樹

> 注意：`Height()` 以「邊數」定義：空樹 `-1`、葉 `0`。

## 檔案

- `BinaryTreeDemo.cs`：`Node` / `BinaryTree` + `TreeSummary`
- `Program.cs`：demo + `--test` 測試模式（不依賴外部套件）
- `BinaryTree.csproj`：.NET 專案檔
- `NuGet.config` + `nuget-offline/`：離線友善設定

## 關鍵設計

### 1) level-order 建樹（支援 `null` 缺洞）

索引規則：`left=2*i+1`、`right=2*i+2`：

```csharp
int leftIndex = 2 * i + 1;
int rightIndex = 2 * i + 2;
node.Left = leftIndex < nodes.Length ? nodes[leftIndex] : null;
node.Right = rightIndex < nodes.Length ? nodes[rightIndex] : null;
```

### 2) BFS 不用 `RemoveAt(0)`（O(1) dequeue）

```csharp
int head = 0;
while (head < queue.Count)
{
    Node node = queue[head];
    head += 1;
}
```

## 如何執行

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

