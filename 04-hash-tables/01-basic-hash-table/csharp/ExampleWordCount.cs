/** Doc block start
 * 雜湊表應用範例：單字計數（Word Count）
 * Hash Table Application Example: Word Counting
 */  // End of block comment

using System;  // Execute this statement as part of the data structure implementation.
using System.Collections.Generic;  // Execute this statement as part of the data structure implementation.
using System.Linq;  // Execute this statement as part of the data structure implementation.
using System.Text.RegularExpressions;  // Execute this statement as part of the data structure implementation.

namespace DataStructures  // Execute this statement as part of the data structure implementation.
{  // Open a new block scope.
    public class ExampleWordCount  // Execute this statement as part of the data structure implementation.
    {  // Open a new block scope.
        /// <summary>
        /// 統計文字中每個單字出現的次數 / Count occurrences of each word in text
        /// </summary>
        public static HashTable<string, int> WordCount(string text)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            var counter = new HashTable<string, int>();  // Assign or update a variable that represents the current algorithm state.

            // 使用正則表達式分割單字 / Use regex to split words
            var words = Regex.Matches(text.ToLower(), @"[a-zA-Z]+")  // Assign or update a variable that represents the current algorithm state.
                            .Cast<Match>()  // Execute this statement as part of the data structure implementation.
                            .Select(m => m.Value);  // Assign or update a variable that represents the current algorithm state.

            foreach (var word in words)  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                if (counter.TryGetValue(word, out int count))  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    counter[word] = count + 1;  // Assign or update a variable that represents the current algorithm state.
                }  // Close the current block scope.
                else  // Handle the alternative branch when the condition is false.
                {  // Open a new block scope.
                    counter.Insert(word, 1);  // Execute this statement as part of the data structure implementation.
                }  // Close the current block scope.
            }  // Close the current block scope.

            return counter;  // Return the computed result to the caller.
        }  // Close the current block scope.

        /// <summary>
        /// 取得出現次數最多的前 n 個單字
        /// Get top n most frequent words
        /// </summary>
        public static List<KeyValuePair<string, int>> GetTopWords(  // Execute this statement as part of the data structure implementation.
            HashTable<string, int> counter, int n = 10)  // Assign or update a variable that represents the current algorithm state.
        {  // Open a new block scope.
            return counter  // Return the computed result to the caller.
                .OrderByDescending(kvp => kvp.Value)  // Assign or update a variable that represents the current algorithm state.
                .Take(n)  // Execute this statement as part of the data structure implementation.
                .ToList();  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        public static void Main(string[] args)  // Execute this statement as part of the data structure implementation.
        {  // Open a new block scope.
            string sampleText = @"  // Assign or update a variable that represents the current algorithm state.
                Data structures are fundamental building blocks in computer science.  // Execute this statement as part of the data structure implementation.
                A hash table is a data structure that implements an associative array,  // Execute this statement as part of the data structure implementation.
                also called a dictionary. A dictionary is a collection of key-value pairs.  // Execute this statement as part of the data structure implementation.
                Hash tables use a hash function to compute an index into an array of buckets.  // Access or update the bucket storage used to hold entries or chains.
                The hash function takes a key as input and returns a hash code.  // Execute this statement as part of the data structure implementation.
                Hash tables provide constant time average case for search, insert, and delete.  // Execute this statement as part of the data structure implementation.
                This makes hash tables very efficient for many applications.  // Execute this statement as part of the data structure implementation.
                Common applications include database indexing, caching, and symbol tables.  // Execute this statement as part of the data structure implementation.
                Hash tables are one of the most important data structures in programming.  // Execute this statement as part of the data structure implementation.
                Many programming languages provide built-in hash table implementations.  // Execute this statement as part of the data structure implementation.
            ";  // Execute this statement as part of the data structure implementation.

            Console.WriteLine("=== 單字計數範例 Word Count Example ===\n");  // Execute this statement as part of the data structure implementation.

            // 執行單字計數 - Perform word counting
            var counter = WordCount(sampleText);  // Assign or update a variable that represents the current algorithm state.

            Console.WriteLine($"總共有 {counter.Count} 個不同的單字");  // Execute this statement as part of the data structure implementation.
            Console.WriteLine($"負載因子: {counter.LoadFactor:F3}\n");  // Execute this statement as part of the data structure implementation.

            // 顯示前 10 個最常出現的單字 - Show top 10 words
            Console.WriteLine("前 10 個最常出現的單字 Top 10 most frequent words:");  // Execute this statement as part of the data structure implementation.
            Console.WriteLine(new string('-', 30));  // Execute this statement as part of the data structure implementation.

            foreach (var kvp in GetTopWords(counter, 10))  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                Console.WriteLine($"  {kvp.Key,-15} : {kvp.Value}");  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
            Console.WriteLine();  // Execute this statement as part of the data structure implementation.

            // 查詢特定單字 - Query specific words
            Console.WriteLine("查詢特定單字 Query specific words:");  // Execute this statement as part of the data structure implementation.
            string[] testWords = { "hash", "data", "python", "structures" };  // Assign or update a variable that represents the current algorithm state.

            foreach (var word in testWords)  // Execute this statement as part of the data structure implementation.
            {  // Open a new block scope.
                if (counter.TryGetValue(word, out int count))  // Evaluate the condition and branch into the appropriate code path.
                {  // Open a new block scope.
                    Console.WriteLine($"  '{word}' 出現 {count} 次");  // Execute this statement as part of the data structure implementation.
                }  // Close the current block scope.
                else  // Handle the alternative branch when the condition is false.
                {  // Open a new block scope.
                    Console.WriteLine($"  '{word}' 未出現在文字中");  // Execute this statement as part of the data structure implementation.
                }  // Close the current block scope.
            }  // Close the current block scope.
        }  // Close the current block scope.
    }  // Close the current block scope.
}  // Close the current block scope.
