# 實作說明（C#）— 環狀佇列（Circular Queue）

## 目標

雖然 .NET 有 `Queue<T>`，本單元改用教學版 `CircularQueue`（head + size + capacity）來消除 dequeue 左移成本：

- `Moved`：永遠是 0（不 shift）
- `Copied`：只在倍增擴容時出現

## 檔案

- `CircularQueueDemo.cs`：`CircularQueue` + 模擬函式（含 copied/moved）
- `Program.cs`：CLI 入口（支援 `--test`）
- `CircularQueue.csproj`：專案檔（無外部套件）
- `NuGet.config`：清空套件來源（避免受限環境下嘗試連外）

## 核心概念

### tailIndex 計算

```csharp
int tailIndex = (_head + _size) % _capacity;
```

### Dequeue 不左移（Moved=0）

```csharp
_head = (_head + 1) % _capacity;
_size -= 1;
```

## 如何執行

在 `03-stacks-and-queues/03-circular-queue/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可改用：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

