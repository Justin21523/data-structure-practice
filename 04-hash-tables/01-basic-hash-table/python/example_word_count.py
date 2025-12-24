"""
雜湊表應用範例：單字計數（Word Count）
Hash Table Application Example: Word Counting

展示如何使用雜湊表實作高效的單字頻率統計
Demonstrates using hash table for efficient word frequency counting
"""

import re
from hash_table import HashTable


def word_count(text: str) -> HashTable:
    """
    統計文字中每個單字出現的次數
    Count occurrences of each word in text

    使用雜湊表達成 O(n) 時間複雜度
    Uses hash table to achieve O(n) time complexity

    Args:
        text: 要分析的文字

    Returns:
        HashTable，其中 key 是單字，value 是出現次數
    """
    # 建立雜湊表 - Create hash table
    counter = HashTable()

    # 將文字轉為小寫並分割成單字
    # Convert to lowercase and split into words
    # 使用正則表達式只保留字母和數字
    words = re.findall(r'\b[a-zA-Z]+\b', text.lower())

    # 統計每個單字 - Count each word
    for word in words:
        current_count = counter.search(word)
        if current_count is None:
            counter.insert(word, 1)
        else:
            counter.insert(word, current_count + 1)

    return counter


def get_top_words(counter: HashTable, n: int = 10) -> list:
    """
    取得出現次數最多的前 n 個單字
    Get top n most frequent words

    Args:
        counter: 單字計數的雜湊表
        n: 要回傳的單字數量

    Returns:
        [(word, count), ...] 的串列，按次數降序排列
    """
    # 收集所有單字與計數 - Collect all words and counts
    word_counts = list(counter.items())

    # 按次數排序（降序） - Sort by count (descending)
    word_counts.sort(key=lambda x: x[1], reverse=True)

    return word_counts[:n]


def find_unique_words(counter: HashTable) -> list:
    """
    找出只出現一次的單字
    Find words that appear only once

    Args:
        counter: 單字計數的雜湊表

    Returns:
        只出現一次的單字串列
    """
    return [word for word, count in counter.items() if count == 1]


# ========== 範例執行 Example Execution ==========

if __name__ == "__main__":
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
    """

    print("=== 單字計數範例 Word Count Example ===\n")

    # 執行單字計數 - Perform word counting
    counter = word_count(sample_text)

    print(f"總共有 {counter.size} 個不同的單字\n")
    print(f"負載因子: {counter.load_factor:.3f}\n")

    # 顯示前 10 個最常出現的單字 - Show top 10 words
    print("前 10 個最常出現的單字 Top 10 most frequent words:")
    print("-" * 30)
    for word, count in get_top_words(counter, 10):
        print(f"  {word:15} : {count}")
    print()

    # 查詢特定單字 - Query specific words
    print("查詢特定單字 Query specific words:")
    test_words = ["hash", "data", "python", "structures"]
    for word in test_words:
        count = counter.search(word)
        if count:
            print(f"  '{word}' 出現 {count} 次")
        else:
            print(f"  '{word}' 未出現在文字中")
    print()

    # 找出只出現一次的單字 - Find unique words
    unique = find_unique_words(counter)
    print(f"只出現一次的單字 (共 {len(unique)} 個):")
    print(f"  {', '.join(sorted(unique)[:10])}{'...' if len(unique) > 10 else ''}")
