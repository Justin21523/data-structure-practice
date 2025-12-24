/**
 * 雜湊表應用範例：單字計數（Word Count）
 * Hash Table Application Example: Word Counting
 */

#include "hash_table.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

/**
 * 將字元轉為小寫
 * Convert character to lowercase
 */
static char to_lower(char c) {
    return (c >= 'A' && c <= 'Z') ? c + 32 : c;
}

/**
 * 統計文字中每個單字出現的次數
 * Count occurrences of each word in text
 */
HashTable* word_count(const char* text) {
    HashTable* counter = ht_create(0);
    if (!counter) return NULL;

    char word[256];
    int word_len = 0;

    for (const char* p = text; ; p++) {
        // 是否為字母 - Is alphabetic?
        if (isalpha((unsigned char)*p)) {
            if (word_len < 255) {
                word[word_len++] = to_lower(*p);
            }
        } else {
            // 遇到非字母，處理目前的單字 - Non-alpha, process current word
            if (word_len > 0) {
                word[word_len] = '\0';

                // 查找並更新計數 - Find and update count
                int* count = (int*)ht_search(counter, word);
                if (count) {
                    (*count)++;
                } else {
                    // 建立新計數 - Create new count
                    int* new_count = malloc(sizeof(int));
                    if (new_count) {
                        *new_count = 1;
                        ht_insert(counter, word, new_count);
                    }
                }

                word_len = 0;
            }

            if (*p == '\0') break;
        }
    }

    return counter;
}

/**
 * 釋放單字計數表（包含所有計數值）
 * Free word count table including all count values
 */
void free_word_count(HashTable* counter) {
    if (!counter) return;

    // 需要遍歷並釋放所有 value（計數指標）
    // Need to traverse and free all values (count pointers)
    for (size_t i = 0; i < ht_capacity(counter); i++) {
        HashTableEntry* entry = counter->buckets[i];
        while (entry) {
            free(entry->value);  // 釋放計數 - Free count
            entry = entry->next;
        }
    }

    ht_free(counter);
}

/**
 * 查詢特定單字的計數
 * Query count for specific word
 */
int get_count(HashTable* counter, const char* word) {
    int* count = (int*)ht_search(counter, word);
    return count ? *count : 0;
}

int main(void) {
    const char* sample_text =
        "Data structures are fundamental building blocks in computer science. "
        "A hash table is a data structure that implements an associative array, "
        "also called a dictionary. A dictionary is a collection of key-value pairs. "
        "Hash tables use a hash function to compute an index into an array of buckets. "
        "The hash function takes a key as input and returns a hash code. "
        "Hash tables provide constant time average case for search, insert, and delete. "
        "This makes hash tables very efficient for many applications. "
        "Common applications include database indexing, caching, and symbol tables. "
        "Hash tables are one of the most important data structures in programming. "
        "Many programming languages provide built-in hash table implementations.";

    printf("=== 單字計數範例 Word Count Example ===\n\n");

    // 執行單字計數 - Perform word counting
    HashTable* counter = word_count(sample_text);
    if (!counter) {
        fprintf(stderr, "無法建立計數表 / Failed to create counter\n");
        return 1;
    }

    printf("總共有 %zu 個不同的單字\n", ht_size(counter));
    printf("負載因子: %.3f\n\n", ht_load_factor(counter));

    // 查詢特定單字 - Query specific words
    printf("查詢特定單字 Query specific words:\n");
    const char* test_words[] = {"hash", "data", "python", "structures"};
    size_t num_words = sizeof(test_words) / sizeof(test_words[0]);

    for (size_t i = 0; i < num_words; i++) {
        int count = get_count(counter, test_words[i]);
        if (count > 0) {
            printf("  '%s' 出現 %d 次\n", test_words[i], count);
        } else {
            printf("  '%s' 未出現在文字中\n", test_words[i]);
        }
    }

    // 釋放記憶體 - Free memory
    free_word_count(counter);

    return 0;
}
