# 實作說明（C#）— Red-Black Tree（LLRB 版本）

## 目標

本單元用 C#（不依賴 NuGet 套件）實作紅黑樹的等價變體 **Left-Leaning Red-Black Tree（LLRB）**：

- LLRB 仍滿足紅黑樹性質，但額外要求「紅連結向左」，讓插入/刪除程式更一致
- 本 repo **不允許重複 key**：重複插入回傳 `false`
- `height` 以「邊數」定義：空樹 `-1`、葉 `0`（此單元高度用遞迴計算）

## 檔案

- `RbtDemo.cs`：資料結構與操作（LLRB）
- `Program.cs`：demo + `--test` 內建測試
- `RedBlackTree.csproj` / `NuGet.config`：離線友善（無外部套件）

## 關鍵設計

### 1) `IsRed(null) == false`

空指標視為黑色：

```csharp
return node is not null && node.Red;
```

### 2) insert：BST 插入 + `FixUp()`

`FixUp()` 依序做：

1) 右紅就左旋（RotateLeft）  
2) 連續左紅就右旋（RotateRight）  
3) 左右都紅就 FlipColors（分裂 4-node）

### 3) delete：top-down 下降時先「借紅」，回溯再 `FixUp()`

刪除用 `MoveRedLeft/MoveRedRight` 避免下去後無法修復；回溯時再 `FixUp()` 恢復 LLRB 形狀。

為了避免 key 不存在時破壞樹，本單元 `Delete(key)` 會先 `Contains(key)`，不存在直接回傳 `false`。

### 4) Validate()：BST ordering + 紅黑樹不變量（含 LLRB）

`Validate()` 檢查：

- root 必須是黑  
- 無連續紅  
- 黑高一致  
- LLRB 規則：不允許右紅  
- BST ordering（嚴格遞增、無重複）

## 如何執行

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

