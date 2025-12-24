# Makefile - 雜湊表 C 語言專案建置設定
# Hash Table C Project Build Configuration

# 編譯器設定 - Compiler Settings
CC = gcc
CFLAGS = -Wall -Wextra -Wpedantic -std=c11 -O2
DEBUGFLAGS = -g -DDEBUG

# 目標檔案 - Target Files
LIB_SRC = hash_table.c
LIB_OBJ = hash_table.o
MAIN_SRC = hash_table.c
EXAMPLE_SRC = example_word_count.c
TEST_SRC = test_hash_table.c

# 執行檔 - Executables
MAIN_EXE = hash_table_demo
EXAMPLE_EXE = example_word_count
TEST_EXE = test_hash_table

# 預設目標 - Default target
.PHONY: all
all: $(MAIN_EXE) $(EXAMPLE_EXE) $(TEST_EXE)

# 編譯函式庫物件檔 - Compile library object file
$(LIB_OBJ): $(LIB_SRC) hash_table.h
	$(CC) $(CFLAGS) -c $(LIB_SRC) -o $(LIB_OBJ)

# 編譯主程式（含範例）- Compile main program with example
$(MAIN_EXE): $(LIB_SRC) hash_table.h
	$(CC) $(CFLAGS) -DHASH_TABLE_MAIN $(LIB_SRC) -o $(MAIN_EXE)

# 編譯單字計數範例 - Compile word count example
$(EXAMPLE_EXE): $(LIB_OBJ) $(EXAMPLE_SRC)
	$(CC) $(CFLAGS) $(EXAMPLE_SRC) $(LIB_OBJ) -o $(EXAMPLE_EXE)

# 編譯測試程式 - Compile test program
$(TEST_EXE): $(LIB_OBJ) $(TEST_SRC)
	$(CC) $(CFLAGS) $(TEST_SRC) $(LIB_OBJ) -o $(TEST_EXE)

# 除錯版本 - Debug build
.PHONY: debug
debug: CFLAGS += $(DEBUGFLAGS)
debug: clean all

# 執行主程式 - Run main program
.PHONY: run
run: $(MAIN_EXE)
	./$(MAIN_EXE)

# 執行範例 - Run example
.PHONY: example
example: $(EXAMPLE_EXE)
	./$(EXAMPLE_EXE)

# 執行測試 - Run tests
.PHONY: test
test: $(TEST_EXE)
	./$(TEST_EXE)

# 清理 - Clean
.PHONY: clean
clean:
	rm -f $(LIB_OBJ) $(MAIN_EXE) $(EXAMPLE_EXE) $(TEST_EXE)
	rm -rf *.dSYM

# 顯示說明 - Help
.PHONY: help
help:
	@echo "可用目標 Available targets:"
	@echo "  all      - 編譯所有程式 Build all programs"
	@echo "  debug    - 編譯除錯版本 Build debug version"
	@echo "  run      - 執行主程式 Run main program"
	@echo "  example  - 執行單字計數範例 Run word count example"
	@echo "  test     - 執行單元測試 Run unit tests"
	@echo "  clean    - 清理建置檔案 Clean build files"
	@echo "  help     - 顯示此說明 Show this help"
