# 實作說明（C#）— AVL Tree（自平衡二元搜尋樹）

## 目標

本單元用 C#（不依賴 NuGet 套件）實作 `AvlDemo.AvlTree`：

- 每個節點快取 `Height`（本 repo 以**邊數**定義：空樹 `-1`、葉 `0`）
- 插入/刪除回溯時用 `Rebalance()` 做 LL/RR/LR/RL 旋轉
- 提供 `Validate()`：檢查 BST ordering、height 正確、與 AVL 平衡條件

## 檔案

- `AvlDemo.cs`：資料結構與操作（`Insert/Delete/Contains/...`）
- `Program.cs`：demo + `--test` 內建測試
- `AvlTree.csproj` / `NuGet.config`：離線友善（無外部套件）

## 關鍵設計

### 1) 以 `Height` 快取高度

```csharp
node.Height = 1 + Math.Max(H(node.Left), H(node.Right));
```

其中 `H(null) = -1`，確保葉節點高度為 `0`。

### 2) 旋轉後更新高度

旋轉完成後要先更新被下移的節點，再更新新的根：

```csharp
UpdateHeight(y);
UpdateHeight(x);
```

### 3) insert/delete 走遞迴回傳新的子樹根

`InsertSubtree` / `DeleteSubtree` 回傳 `(newRoot, inserted/removed)`，避免漏接 parent 指標；回溯時再 `UpdateHeight` + `Rebalance`。

### 4) Validate()：把不變量寫成可測的函式

`ValidateSubtree(node, low, high)` 同時檢查：

- BST ordering（嚴格遞增、無重複）
- `node.Height` 是否等於由子樹計算出的高度
- `Math.Abs(leftHeight - rightHeight) <= 1`

## 如何執行

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

