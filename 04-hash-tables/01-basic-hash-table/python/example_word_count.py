"""Docstring start
雜湊表應用範例：單字計數（Word Count）
Hash Table Application Example: Word Counting

展示如何使用雜湊表實作高效的單字頻率統計 / Demonstrates using hash table for efficient word frequency counting
"""  # End of docstring

import re  # Import modules and symbols needed by this implementation.
from hash_table import HashTable  # Import modules and symbols needed by this implementation.


def word_count(text: str) -> HashTable:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    統計文字中每個單字出現的次數 / Count occurrences of each word in text

    使用雜湊表達成 O(n) 時間複雜度
    Uses hash table to achieve O(n) time complexity

    Args:
        text: 要分析的文字

    Returns:
        HashTable，其中 key 是單字，value 是出現次數
    """  # End of docstring
    # 建立雜湊表 - Create hash table
    counter = HashTable()  # Create a new hash table instance to store key->value mappings.

    # 將文字轉為小寫並分割成單字 / Convert to lowercase and split into words
    # 使用正則表達式只保留字母和數字 / Use regular expression keep only letters 和 digits
    words = re.findall(r'\b[a-zA-Z]+\b', text.lower())  # Use a regular expression to extract normalized word tokens from the input.

    # 統計每個單字 - Count each word
    for word in words:  # Iterate over a range/collection to process each item in sequence.
        current_count = counter.search(word)  # Look up the current value for the given key (None if the key is missing).
        if current_count is None:  # Evaluate the condition and branch into the appropriate code path.
            counter.insert(word, 1)  # Insert or update the given key with the provided value.
        else:  # Handle the alternative branch when the condition is false.
            counter.insert(word, current_count + 1)  # Insert or update the given key with the provided value.

    return counter  # Return the computed result to the caller.


def get_top_words(counter: HashTable, n: int = 10) -> list:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    取得出現次數最多的前 n 個單字
    Get top n most frequent words

    Args:
        counter: 單字計數的雜湊表
        n: 要回傳的單字數量

    Returns:
        [(word, count), ...] 的串列，按次數降序排列
    """  # End of docstring
    # 收集所有單字與計數 - Collect all words and counts
    word_counts = list(counter.items())  # Materialize all (key, value) pairs into a list so we can sort/slice them.

    # 按次數排序（降序） - Sort by count (descending)
    word_counts.sort(key=lambda x: x[1], reverse=True)  # Assign or update a variable that represents the current algorithm state.

    return word_counts[:n]  # Return the computed result to the caller.


def find_unique_words(counter: HashTable) -> list:  # Define a function/method that implements one operation of this unit.
    """Docstring start
    找出只出現一次的單字 / Find words that appear only once

    Args:
        counter: 單字計數的雜湊表

    Returns:
        只出現一次的單字串列 / Appear only once 的單字 list
    """  # End of docstring
    return [word for word, count in counter.items() if count == 1]  # Return the computed result to the caller.


# ========== 範例執行 Example Execution ==========

if __name__ == "__main__":  # Evaluate the condition and branch into the appropriate code path.
    # 範例文字 - Sample text
    sample_text = """
    Data structures are fundamental building blocks in computer science.
    A hash table is a data structure that implements an associative array,
    also called a dictionary. A dictionary is a collection of key-value pairs.
    Hash tables use a hash function to compute an index into an array of buckets.
    The hash function takes a key as input and returns a hash code.
    Hash tables provide constant time average case for search, insert, and delete.
    This makes hash tables very efficient for many applications.
    Common applications include database indexing, caching, and symbol tables.
    Hash tables are one of the most important data structures in programming.
    Many programming languages provide built-in hash table implementations.
    """  # End of docstring

    print("=== 單字計數範例 Word Count Example ===\n")  # Print a formatted message for the interactive example output.

    # 執行單字計數 - Perform word counting
    counter = word_count(sample_text)  # Assign or update a variable that represents the current algorithm state.

    print(f"總共有 {counter.size} 個不同的單字\n")  # Print a formatted message for the interactive example output.
    print(f"負載因子: {counter.load_factor:.3f}\n")  # Compute/check the load factor (size/capacity) to decide about resizing.

    # 顯示前 10 個最常出現的單字 - Show top 10 words
    print("前 10 個最常出現的單字 Top 10 most frequent words:")  # Print a formatted message for the interactive example output.
    print("-" * 30)  # Print a formatted message for the interactive example output.
    for word, count in get_top_words(counter, 10):  # Iterate over a range/collection to process each item in sequence.
        print(f"  {word:15} : {count}")  # Print a formatted message for the interactive example output.
    print()  # Print a formatted message for the interactive example output.

    # 查詢特定單字 - Query specific words
    print("查詢特定單字 Query specific words:")  # Print a formatted message for the interactive example output.
    test_words = ["hash", "data", "python", "structures"]  # Assign or update a variable that represents the current algorithm state.
    for word in test_words:  # Iterate over a range/collection to process each item in sequence.
        count = counter.search(word)  # Look up the current value for the given key (None if the key is missing).
        if count:  # Evaluate the condition and branch into the appropriate code path.
            print(f"  '{word}' 出現 {count} 次")  # Print a formatted message for the interactive example output.
        else:  # Handle the alternative branch when the condition is false.
            print(f"  '{word}' 未出現在文字中")  # Print a formatted message for the interactive example output.
    print()  # Print a formatted message for the interactive example output.

    # 找出只出現一次的單字 - Find unique words
    unique = find_unique_words(counter)  # Assign or update a variable that represents the current algorithm state.
    print(f"只出現一次的單字 (共 {len(unique)} 個):")  # Print a formatted message for the interactive example output.
    print(f"  {', '.join(sorted(unique)[:10])}{'...' if len(unique) > 10 else ''}")  # Print a formatted message for the interactive example output.
