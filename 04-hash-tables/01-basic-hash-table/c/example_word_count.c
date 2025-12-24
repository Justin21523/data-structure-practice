/** Doc block start
 * 雜湊表應用範例：單字計數（Word Count）
 * Hash Table Application Example: Word Counting
 */  // End of block comment

#include "hash_table.h"  // Execute this statement as part of the data structure implementation.
#include <stdio.h>  // Execute this statement as part of the data structure implementation.
#include <stdlib.h>  // Execute this statement as part of the data structure implementation.
#include <string.h>  // Execute this statement as part of the data structure implementation.
#include <ctype.h>  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 將字元轉為小寫 / Convert character to lowercase
 */  // End of block comment
static char to_lower(char c) {  // Execute this statement as part of the data structure implementation.
    return (c >= 'A' && c <= 'Z') ? c + 32 : c;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 統計文字中每個單字出現的次數 / Count occurrences of each word in text
 */  // End of block comment
HashTable* word_count(const char* text) {  // Execute this statement as part of the data structure implementation.
    HashTable* counter = ht_create(0);  // Assign or update a variable that represents the current algorithm state.
    if (!counter) return NULL;  // Evaluate the condition and branch into the appropriate code path.

    char word[256];  // Execute this statement as part of the data structure implementation.
    int word_len = 0;  // Assign or update a variable that represents the current algorithm state.

    for (const char* p = text; ; p++) {  // Iterate over a range/collection to process each item in sequence.
        // 是否為字母 - Is alphabetic?
        if (isalpha((unsigned char)*p)) {  // Evaluate the condition and branch into the appropriate code path.
            if (word_len < 255) {  // Evaluate the condition and branch into the appropriate code path.
                word[word_len++] = to_lower(*p);  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.
        } else {  // Handle the alternative branch when the condition is false.
            // 遇到非字母，處理目前的單字 - Non-alpha, process current word
            if (word_len > 0) {  // Evaluate the condition and branch into the appropriate code path.
                word[word_len] = '\0';  // Assign or update a variable that represents the current algorithm state.

                // 查找並更新計數 - Find and update count
                int* count = (int*)ht_search(counter, word);  // Assign or update a variable that represents the current algorithm state.
                if (count) {  // Evaluate the condition and branch into the appropriate code path.
                    (*count)++;  // Execute this statement as part of the data structure implementation.
                } else {  // Handle the alternative branch when the condition is false.
                    // 建立新計數 - Create new count
                    int* new_count = malloc(sizeof(int));  // Allocate heap memory for a new struct/object instance.
                    if (new_count) {  // Evaluate the condition and branch into the appropriate code path.
                        *new_count = 1;
                        ht_insert(counter, word, new_count);  // Execute this statement as part of the data structure implementation.
                    }  // Close the current block scope.
                }  // Close the current block scope.

                word_len = 0;  // Assign or update a variable that represents the current algorithm state.
            }  // Close the current block scope.

            if (*p == '\0') break;  // Evaluate the condition and branch into the appropriate code path.
        }  // Close the current block scope.
    }  // Close the current block scope.

    return counter;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 釋放單字計數表（包含所有計數值） / Free word count table including all count values
 */  // End of block comment
void free_word_count(HashTable* counter) {  // Execute this statement as part of the data structure implementation.
    if (!counter) return;  // Evaluate the condition and branch into the appropriate code path.

    // 需要遍歷並釋放所有 value（計數指標）
    // Need to traverse and free all values (count pointers)
    for (size_t i = 0; i < ht_capacity(counter); i++) {  // Iterate over a range/collection to process each item in sequence.
        HashTableEntry* entry = counter->buckets[i];  // Access or update the bucket storage used to hold entries or chains.
        while (entry) {  // Repeat while the loop condition remains true.
            free(entry->value);  // 釋放計數 - Free count
            entry = entry->next;  // Assign or update a variable that represents the current algorithm state.
        }  // Close the current block scope.
    }  // Close the current block scope.

    ht_free(counter);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

/** Doc block start
 * 查詢特定單字的計數 / Query count for specific word
 */  // End of block comment
int get_count(HashTable* counter, const char* word) {  // Execute this statement as part of the data structure implementation.
    int* count = (int*)ht_search(counter, word);  // Assign or update a variable that represents the current algorithm state.
    return count ? *count : 0;  // Return the computed result to the caller.
}  // Close the current block scope.

int main(void) {  // Execute this statement as part of the data structure implementation.
    const char* sample_text =  // Assign or update a variable that represents the current algorithm state.
        "Data structures are fundamental building blocks in computer science. "  // Execute this statement as part of the data structure implementation.
        "A hash table is a data structure that implements an associative array, "  // Execute this statement as part of the data structure implementation.
        "also called a dictionary. A dictionary is a collection of key-value pairs. "  // Execute this statement as part of the data structure implementation.
        "Hash tables use a hash function to compute an index into an array of buckets. "  // Access or update the bucket storage used to hold entries or chains.
        "The hash function takes a key as input and returns a hash code. "  // Execute this statement as part of the data structure implementation.
        "Hash tables provide constant time average case for search, insert, and delete. "  // Execute this statement as part of the data structure implementation.
        "This makes hash tables very efficient for many applications. "  // Execute this statement as part of the data structure implementation.
        "Common applications include database indexing, caching, and symbol tables. "  // Execute this statement as part of the data structure implementation.
        "Hash tables are one of the most important data structures in programming. "  // Execute this statement as part of the data structure implementation.
        "Many programming languages provide built-in hash table implementations.";  // Execute this statement as part of the data structure implementation.

    printf("=== 單字計數範例 Word Count Example ===\n\n");  // Execute this statement as part of the data structure implementation.

    // 執行單字計數 - Perform word counting
    HashTable* counter = word_count(sample_text);  // Assign or update a variable that represents the current algorithm state.
    if (!counter) {  // Evaluate the condition and branch into the appropriate code path.
        fprintf(stderr, "無法建立計數表 / Failed to create counter\n");  // Execute this statement as part of the data structure implementation.
        return 1;  // Return the computed result to the caller.
    }  // Close the current block scope.

    printf("總共有 %zu 個不同的單字\n", ht_size(counter));  // Execute this statement as part of the data structure implementation.
    printf("負載因子: %.3f\n\n", ht_load_factor(counter));  // Execute this statement as part of the data structure implementation.

    // 查詢特定單字 - Query specific words
    printf("查詢特定單字 Query specific words:\n");  // Execute this statement as part of the data structure implementation.
    const char* test_words[] = {"hash", "data", "python", "structures"};  // Assign or update a variable that represents the current algorithm state.
    size_t num_words = sizeof(test_words) / sizeof(test_words[0]);  // Assign or update a variable that represents the current algorithm state.

    for (size_t i = 0; i < num_words; i++) {  // Iterate over a range/collection to process each item in sequence.
        int count = get_count(counter, test_words[i]);  // Assign or update a variable that represents the current algorithm state.
        if (count > 0) {  // Evaluate the condition and branch into the appropriate code path.
            printf("  '%s' 出現 %d 次\n", test_words[i], count);  // Execute this statement as part of the data structure implementation.
        } else {  // Handle the alternative branch when the condition is false.
            printf("  '%s' 未出現在文字中\n", test_words[i]);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // 釋放記憶體 - Free memory
    free_word_count(counter);  // Execute this statement as part of the data structure implementation.

    return 0;  // Return the computed result to the caller.
}  // Close the current block scope.
