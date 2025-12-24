# 實作說明（C#）— 環狀鏈結串列（Circular Linked List）

## 目標

本單元用教學版 `CircularLinkedList`（單向環狀鏈結串列）示範 tail-only 的技巧，並用 `Hops` 量化走訪成本：

- `Hops`：每做一次 `current = current.Next` 就 +1

## 檔案

- `CircularLinkedListDemo.cs`：`CircularLinkedList` 實作（含 hops 計數）
- `Program.cs`：CLI 入口（支援 `--test`）
- `CircularLinkedList.csproj`：專案檔（無外部套件）
- `NuGet.config`：清空套件來源（避免受限環境下嘗試連外）

## 核心概念

### 1) 只存 tail：`head = tail.Next`

```csharp
private Node? HeadOrNull()
{
    if (_tail == null) return null;
    return _tail.Next;
}
```

### 2) 走訪一定要 bounded（用 size）

因為沒有 `null` 結尾，`ToList()` 會走 `for (i < _size)` 次，避免無限迴圈。

### 3) 成本公式（Hops）

- `PushFront / PushBack / PopFront`：`Hops = 0`
- `PopBack`：找 predecessor → `Hops = max(0, n-2)`
- `Rotate(k)`：`Hops = k % size`，且 `k < 0` 會丟 `ArgumentOutOfRangeException`

## 如何執行

在 `02-arrays-and-linked-lists/05-circular-linked-list/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可改用：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

