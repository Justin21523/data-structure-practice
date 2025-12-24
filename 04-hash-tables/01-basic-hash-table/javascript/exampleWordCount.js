/**
 * 雜湊表應用範例：單字計數（Word Count）
 * Hash Table Application Example: Word Counting
 */

const HashTable = require('./hashTable');

/**
 * 統計文字中每個單字出現的次數
 * Count occurrences of each word in text
 *
 * @param {string} text - 要分析的文字
 * @returns {HashTable} 單字計數的雜湊表
 */
function wordCount(text) {
    const counter = new HashTable();

    // 將文字轉為小寫並分割成單字
    // Convert to lowercase and split into words
    const words = text.toLowerCase().match(/[a-zA-Z]+/g) || [];

    for (const word of words) {
        const count = counter.search(word);
        if (count === undefined) {
            counter.insert(word, 1);
        } else {
            counter.insert(word, count + 1);
        }
    }

    return counter;
}

/**
 * 取得出現次數最多的前 n 個單字
 * Get top n most frequent words
 */
function getTopWords(counter, n = 10) {
    return counter.entries()
        .sort((a, b) => b[1] - a[1])
        .slice(0, n);
}

// ========== 範例執行 Example Execution ==========
if (require.main === module) {
    const sampleText = `
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
    `;

    console.log('=== 單字計數範例 Word Count Example ===\n');

    const counter = wordCount(sampleText);

    console.log(`總共有 ${counter.size} 個不同的單字`);
    console.log(`負載因子: ${counter.loadFactor.toFixed(3)}\n`);

    console.log('前 10 個最常出現的單字 Top 10 most frequent words:');
    console.log('-'.repeat(30));
    for (const [word, count] of getTopWords(counter, 10)) {
        console.log(`  ${word.padEnd(15)} : ${count}`);
    }
    console.log();

    console.log('查詢特定單字 Query specific words:');
    const testWords = ['hash', 'data', 'python', 'structures'];
    for (const word of testWords) {
        const count = counter.search(word);
        if (count !== undefined) {
            console.log(`  '${word}' 出現 ${count} 次`);
        } else {
            console.log(`  '${word}' 未出現在文字中`);
        }
    }
}

module.exports = { wordCount, getTopWords };
