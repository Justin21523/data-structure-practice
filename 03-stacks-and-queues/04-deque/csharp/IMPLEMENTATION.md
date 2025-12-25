# 實作說明（C#）— 雙端佇列（Deque）

## 目標

本單元在 `DequeDemo.cs` 內用「環狀陣列（circular buffer）」實作 `DequeDemo.Deque`，並用 `Program.cs` 提供 demo 與內建測試（不依賴任何外部套件）：

- `moved`：永遠為 `0`（不做 shift）
- `copied`：只在擴容（doubling）時出現，且一次 resize 會複製 `size` 個元素

## 檔案

- `DequeDemo.cs`：Deque + `SimulatePushBacks`（成長/複製成本統計）
- `Program.cs`：CLI demo + `--test` 內建測試
- `Deque.csproj`：`net8.0`、無外部套件
- `NuGet.config`：清空 sources，確保離線/受限環境可 restore

## 核心概念

### 1) indexAt：邏輯 offset → 實體索引

```csharp
private int IndexAt(int offset)
{
    return (_head + offset) % _capacity;
}
```

### 2) pushFront / pushBack 都只改索引（moved=0）

pushFront：

```csharp
_head = (_head - 1 + _capacity) % _capacity;
_data[_head] = value;
_size += 1;
```

pushBack：

```csharp
int tailIndex = IndexAt(_size);
_data[tailIndex] = value;
_size += 1;
```

### 3) resize：依 deque 順序複製並 head=0

```csharp
for (int i = 0; i < _size; i++)
{
    newData[i] = _data[IndexAt(i)];
}
_head = 0;
```

## 如何執行

在 `03-stacks-and-queues/04-deque/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上（把 CLI home 指到工作目錄）：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

