# 實作說明（C#）— 靜態陣列（Fixed Capacity Array）

## 目標

用 C# 的 `int[]` 模擬靜態陣列：容量固定、插入/刪除需要 shift。  
本單元用 **moved elements（搬移次數）** 讓 O(n) 的來源具體可見，並用內建測試模式驗證。

## 檔案

- `StaticArray.csproj`：最小化 console 專案（無外部套件）
- `NuGet.config`：關閉外部套件來源（無網路也可 build/run）
- `FixedArrayDemo.cs`：`FixedArray` + `SimulateInsertMoves/SimulateRemoveMoves`
- `Program.cs`：預設印表格；`--test` 跑內建測試

## 搬移次數（公式）

- 插入 `InsertAt(i)`：右移 `[i, size-1]` → `moved = size - i`
- 刪除 `RemoveAt(i)`：左移 `[i+1, size-1]` → `moved = size - i - 1`

## 如何執行

在 `02-arrays-and-linked-lists/01-array/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```

