# 實作說明（C#）— 雜湊函數（Hash Functions）

## 檔案

- `HashFunctions.cs`：整數/字串雜湊函數 + `AnalyzeDistribution`。
- `UniversalHashing.cs`：通用雜湊（universal hashing）函數族 + `UniversalHashTable`（chaining）。
- `Program.cs`：demo + `--test` 內建測試（無外部依賴）。
- `HashFunctions.csproj`：`net8.0`、無外部套件
- `NuGet.config`：清空 sources，確保離線/受限環境可 restore

## 核心概念

### 1) 32-bit unsigned hash

字串 hash（DJB2 / FNV-1a / Jenkins）使用 `uint`，自然具備「模 2^32」的 overflow 行為。

### 2) `AnalyzeDistribution`

把 keys 打到 `m` 個桶後，回傳：

- `NonEmptyBuckets`：有元素的桶數
- `StdDeviation`：桶大小標準差

### 3) 通用雜湊（Universal Hashing）

`UniversalHashFamily` 使用：

```
h_{a,b}(k) = ((a*k + b) mod p) mod m
```

並允許 `seed`，讓測試可重現（每次 trial 用 `seed=t` 模擬隨機挑函數）。

### 4) `UniversalHashTable`

chaining 結構：`List<List<Entry>>`，插入後：

- load factor `> 0.75` → resize（capacity * 2）
- chain 長度 `> 10` → regenerate 並重新分配（防禦性 rehash）

## 如何執行

在 `04-hash-tables/03-hash-functions/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法寫入 `~/.dotnet`，可加上：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 DOTNET_NOLOGO=1 dotnet run -- --test
```

