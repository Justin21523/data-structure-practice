# 實作說明（C#）— 佇列（Queue）

## 目標

雖然 .NET 有 `Queue<T>`，本單元改用教學版 `ArrayQueue`（naive queue），把 `dequeue` 的 shift 成本量化：

- `Copied`：擴容時複製元素次數
- `Moved`：`Dequeue` 時左移元素次數（`Size-1`）

> 下一單元 `03-circular-queue` 會用環狀索引避免左移，讓 `Moved = 0`。

## 檔案

- `QueueDemo.cs`：`ArrayQueue` + 模擬函式（含 copied/moved）
- `Program.cs`：CLI 入口（支援 `--test`）
- `Queue.csproj`：專案檔（無外部套件）
- `NuGet.config`：清空套件來源（避免受限環境下嘗試連外）

## 核心概念

### Dequeue 的 Moved（左移）

```csharp
int moved = _size - 1;
for (int i = 1; i < _size; i++)
{
    _data[i - 1] = _data[i];
}
```

## 如何執行

在 `03-stacks-and-queues/02-queue/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可改用：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

