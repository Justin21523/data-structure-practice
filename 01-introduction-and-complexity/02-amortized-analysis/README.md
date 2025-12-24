# 02 攤銷分析（Amortized Analysis）

攤銷分析（amortized analysis）用來回答這類問題：

> 單次操作「偶爾很貴」，但一長串操作的**平均成本**其實很便宜嗎？

最常見的例子是「動態陣列（dynamic array）」擴容：當容量不夠時，需要一次搬移大量元素（看起來很貴），但把這個成本平均分攤到每次 `append/push` 後，平均仍可達到 **O(1) 攤銷時間**。

## 三種常用方法

1. **聚合法（Aggregate Method）**：直接算一段操作序列的總成本，再除以操作次數。
2. **記帳法（Accounting Method）**：為操作「多收一點」作為信用（credit），用來支付未來昂貴操作。
3. **位能法（Potential Method）**：定義位能函數 Φ，將「預先儲存的能量」形式化。

## 本單元程式碼示範

本 repo 會用「可計數的成本」示範攤銷分析（避免實際計時受到環境影響）：

- **實際成本（actual cost）**：每次 `append` 計 `1`（寫入新元素）+ `copies`（擴容時搬移元素次數）
- **記帳法**：每次 `append` 固定收 `3`，用 `bank`（信用）支付擴容搬移
- **位能法**：使用位能函數 `Φ = 2·size - capacity + 1`，並計算 `amortized = actual + ΔΦ`

你會看到：即使某次 `append` 很貴（例如搬移 2^k 個元素），但整串操作的平均成本仍是常數。

## 程式碼（各語言）

每個語言資料夾皆提供：

- 示範程式（印出表格）
- 測試程式（驗證：總成本界線、bank 不為負、位能法每步攤銷成本）
- 說明文件：`IMPLEMENTATION.md`

### Python

在 `01-introduction-and-complexity/02-amortized-analysis/python/`：

```bash
python3 amortized_demo.py
python3 -m unittest -v
```

### Java

在 `01-introduction-and-complexity/02-amortized-analysis/java/`：

```bash
javac AmortizedDemo.java AmortizedDemoTest.java
java AmortizedDemo
java AmortizedDemoTest
```

### JavaScript（Node.js）

在 `01-introduction-and-complexity/02-amortized-analysis/javascript/`：

```bash
node amortizedDemo.js
node --test
```

### C

在 `01-introduction-and-complexity/02-amortized-analysis/c/`：

```bash
make
make run
make test
```

### C++

在 `01-introduction-and-complexity/02-amortized-analysis/cpp/`：

```bash
cmake -S . -B build
cmake --build build
./build/amortized_demo
ctest --test-dir build
```

### C#

在 `01-introduction-and-complexity/02-amortized-analysis/csharp/`：

```bash
dotnet run
dotnet run -- --test
```

若你在受限環境無法連線 nuget.org，請使用本資料夾提供的 `NuGet.config`（已預設關閉外部來源）。

如果你在受限環境（例如 sandbox/某些 CI）無法寫入家目錄的 `~/.dotnet`，請改用：

```bash
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run
DOTNET_CLI_HOME=out DOTNET_SKIP_FIRST_TIME_EXPERIENCE=1 dotnet run -- --test
```
