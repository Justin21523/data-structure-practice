# 實作說明（C#）— 攤銷分析（Amortized Analysis）

## 目標

用 C#/.NET 做一個最小化的 Console 專案，示範「動態陣列倍增擴容」的攤銷分析，並避免依賴 xUnit/NUnit（不需要 NuGet 套件下載）。

## 成本模型

- 寫入新元素：`+1`
- 擴容時搬移：copy 1 個元素算 `+1`
- **actual cost** = `1 + copied`

## 三種方法（程式對應）

### 聚合法（Aggregate）

容量倍增後，最後容量 `finalCapacity` 是 2 的冪，且：

`totalCopies = finalCapacity - 1`

因此 `totalActualCost <= 3m`（測試驗證）。

### 記帳法（Accounting）

每次操作固定收 3，信用放在 `Bank`：

```csharp
_bank += 3 - actualCost;
```

測試會逐步確認 `BankAfter >= 0`。

### 位能法（Potential）

位能函數：

`Φ = 2·size - capacity + 1`

攤銷成本：

`amortized = actual + (Φ_after - Φ_before)`

測試驗證每一步 `AmortizedCost == 3`。

## 檔案

- `AmortizedAnalysis.csproj`：最小化 console 專案（無外部套件）
- `NuGet.config`：關閉外部套件來源（無網路也可 `dotnet restore/run`）
- `AmortizedDemo.cs`：核心實作（Potential + DynamicArray + 模擬）
- `Program.cs`：`dotnet run` 印表格；`--test` 跑內建測試

## 如何執行

在 `01-introduction-and-complexity/02-amortized-analysis/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上（將 CLI 家目錄導向可寫入路徑）：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```

