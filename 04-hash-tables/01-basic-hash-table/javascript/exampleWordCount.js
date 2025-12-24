/** Doc block start
 * 雜湊表應用範例：單字計數（Word Count）
 * Hash Table Application Example: Word Counting
 */  // End of block comment

const HashTable = require('./hashTable');  // Compute a hash-based index so keys map into the table's storage.

/** Doc block start
 * 統計文字中每個單字出現的次數 / Count occurrences of each word in text
 *(blank line)
 * @param {string} text - 要分析的文字
 * @returns {HashTable} 單字計數的雜湊表
 */  // End of block comment
function wordCount(text) {  // Execute this statement as part of the data structure implementation.
    const counter = new HashTable();  // Assign or update a variable that represents the current algorithm state.

    // 將文字轉為小寫並分割成單字 / Convert to lowercase and split into words
    const words = text.toLowerCase().match(/[a-zA-Z]+/g) || [];  // Assign or update a variable that represents the current algorithm state.

    for (const word of words) {  // Iterate over a range/collection to process each item in sequence.
        const count = counter.search(word);  // Assign or update a variable that represents the current algorithm state.
        if (count === undefined) {  // Evaluate the condition and branch into the appropriate code path.
            counter.insert(word, 1);  // Execute this statement as part of the data structure implementation.
        } else {  // Handle the alternative branch when the condition is false.
            counter.insert(word, count + 1);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.

    return counter;  // Return the computed result to the caller.
}  // Close the current block scope.

/** Doc block start
 * 取得出現次數最多的前 n 個單字
 * Get top n most frequent words
 */  // End of block comment
function getTopWords(counter, n = 10) {  // Assign or update a variable that represents the current algorithm state.
    return counter.entries()  // Return the computed result to the caller.
        .sort((a, b) => b[1] - a[1])  // Assign or update a variable that represents the current algorithm state.
        .slice(0, n);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.

// ========== 範例執行 Example Execution ==========
if (require.main === module) {  // Evaluate the condition and branch into the appropriate code path.
    const sampleText = `  // Assign or update a variable that represents the current algorithm state.
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
    `;  // Execute this statement as part of the data structure implementation.

    console.log('=== 單字計數範例 Word Count Example ===\n');  // Execute this statement as part of the data structure implementation.

    const counter = wordCount(sampleText);  // Assign or update a variable that represents the current algorithm state.

    console.log(`總共有 ${counter.size} 個不同的單字`);  // Execute this statement as part of the data structure implementation.
    console.log(`負載因子: ${counter.loadFactor.toFixed(3)}\n`);  // Execute this statement as part of the data structure implementation.

    console.log('前 10 個最常出現的單字 Top 10 most frequent words:');  // Execute this statement as part of the data structure implementation.
    console.log('-'.repeat(30));  // Execute this statement as part of the data structure implementation.
    for (const [word, count] of getTopWords(counter, 10)) {  // Iterate over a range/collection to process each item in sequence.
        console.log(`  ${word.padEnd(15)} : ${count}`);  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.
    console.log();  // Execute this statement as part of the data structure implementation.

    console.log('查詢特定單字 Query specific words:');  // Execute this statement as part of the data structure implementation.
    const testWords = ['hash', 'data', 'python', 'structures'];  // Assign or update a variable that represents the current algorithm state.
    for (const word of testWords) {  // Iterate over a range/collection to process each item in sequence.
        const count = counter.search(word);  // Assign or update a variable that represents the current algorithm state.
        if (count !== undefined) {  // Evaluate the condition and branch into the appropriate code path.
            console.log(`  '${word}' 出現 ${count} 次`);  // Execute this statement as part of the data structure implementation.
        } else {  // Handle the alternative branch when the condition is false.
            console.log(`  '${word}' 未出現在文字中`);  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.
    }  // Close the current block scope.
}  // Close the current block scope.

module.exports = { wordCount, getTopWords };  // Assign or update a variable that represents the current algorithm state.
