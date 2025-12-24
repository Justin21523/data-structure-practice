# 實作說明（C#）— 動態陣列（Dynamic Array）

## 目標

.NET 的 `List<T>` 就是動態陣列；本單元改用教學版 `DynamicArray` 來把成本「算得出來」：

- `Copied`：擴容（resize）時複製既有元素次數
- `Moved`：`InsertAt/RemoveAt` shift 的搬移次數

容量策略採 **倍增（doubling）**：當容量不足時 `capacity *= 2`，使 `Append` 呈現攤銷（amortized）O(1)。

## 檔案

- `DynamicArrayDemo.cs`：`DynamicArray` + 表格輸出（同時提供 `--test`）
- `Program.cs`：CLI 入口（轉呼叫 demo/test）
- `DynamicArray.csproj`：專案檔
- `NuGet.config`：清空套件來源（避免受限環境下嘗試連外）

## 核心概念：resize 與 shift

擴容會配置新陣列並複製舊資料，因此 `Copied == Size`：

```csharp
for (var i = 0; i < _size; i += 1) {
    newData[i] = _data[i];
    copied += 1;
}
```

插入/刪除會造成 shift：

- `InsertAt(i, v)`：右移 `[i, size-1]`，`Moved = size - i`
- `RemoveAt(i)`：左移 `[i+1, size-1]`，`Moved = size - i - 1`

## 如何執行

在 `02-arrays-and-linked-lists/02-dynamic-array/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可改用：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

