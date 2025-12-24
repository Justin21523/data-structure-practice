# 實作說明（C#）— 堆疊（Stack）

## 目標

雖然 .NET 有 `Stack<T>`，本單元改用教學版 `ArrayStack`（自己管理 `size/capacity`），並用 `Copied` 量化擴容成本：

- `Copied`：擴容時複製既有元素次數（resize copy count）

`top` 位於陣列尾端（`Size-1`），所以 `Push/Pop/Peek` 都是 O(1)；`Push` 在擴容當下會複製 O(n) 個元素，但整體是攤銷 O(1)（doubling）。

## 檔案

- `StackDemo.cs`：`ArrayStack` + `SimulatePushes`（含 copied）
- `Program.cs`：CLI 入口（支援 `--test`）
- `Stack.csproj`：專案檔（無外部套件）
- `NuGet.config`：清空套件來源（避免受限環境下嘗試連外）

## 核心概念

### 倍增擴容與 Copied

```csharp
for (int i = 0; i < _size; i++)
{
    newData[i] = _data[i];
    copied += 1;
}
```

## 如何執行

在 `03-stacks-and-queues/01-stack/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可改用：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

