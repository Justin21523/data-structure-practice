# 實作說明（C#）— 漸進符號示範（Asymptotic Notation）

## 目標

用 **C#/.NET** 以「操作次數（operation count）」示範典型成長率，並且不依賴 xUnit/NUnit（避免 NuGet 套件下載需求），改用程式內建的測試模式。

## 檔案

- `AsymptoticNotation.csproj`：最小化 console 專案（無外部套件）。
- `NuGet.config`：關閉外部套件來源（避免 `dotnet restore` 在無網路/受限網路時失敗）。
- `AsymptoticDemo.cs`：核心計數函式（O(1)、O(log n)、O(n)、O(n log n)、O(n²)）。
- `Program.cs`：同時支援「印表格」與「跑測試」：
  - `dotnet run`：印出表格
  - `dotnet run -- --test`：執行內建測試

## 代表性片段

O(log n) 的 halving loop：

```csharp
while (current > 1)
{
    current /= 2;
    operations += 1;
}
```

## 如何執行

在 `01-introduction-and-complexity/01-asymptotic-notation/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上（將 CLI 家目錄導向可寫入路徑）：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```
