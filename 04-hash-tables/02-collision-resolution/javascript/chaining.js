/** Doc block start
 * 鏈結法雜湊表（Chained Hash Table）- JavaScript 實作
 * 使用鏈結法（chaining）處理碰撞，並追蹤探測次數
 *(blank line)
 * Chained Hash Table implementation with collision resolution
 * Using chaining method and tracking probe statistics
 */  // End of block comment

class ChainedHashTable {  // Declare the HashTable class that provides insert/search/delete operations.
    // ========== 常數 Constants ==========
    static DEFAULT_CAPACITY = 16;  // Assign or update a variable that represents the current algorithm state.
    static MAX_LOAD_FACTOR = 0.75;  // Assign or update a variable that represents the current algorithm state.

    /** Doc block start
     * 建構子：初始化鏈結雜湊表 / Constructor: Initialize chained hash table
     *(blank line)
     * @param {number} capacity - 桶的數量（number of buckets）
     */  // End of block comment
    constructor(capacity = ChainedHashTable.DEFAULT_CAPACITY) {  // Assign or update a variable that represents the current algorithm state.
        if (capacity <= 0) {  // Evaluate the condition and branch into the appropriate code path.
            throw new Error('容量必須為正整數 / Capacity must be positive');  // Throw an exception to signal an invalid argument or operation.
        }  // Close the current block scope.
        this._capacity = capacity;  // Assign or update a variable that represents the current algorithm state.
        this._size = 0;  // Assign or update a variable that represents the current algorithm state.
        // 使用陣列的陣列來實作鏈結法 / Use array of arrays for chaining
        this._buckets = new Array(capacity).fill(null).map(() => []);  // Allocate the bucket array and initialize empty chains for each bucket.

        // 統計資訊 - Statistics
        this._totalProbes = 0;  // 總探測次數 - Total number of probes
        this._operations = 0;    // 總操作次數 - Total number of operations
    }  // Close the current block scope.

    // ========== 私有方法 Private Methods ==========

    /** Doc block start
     * 計算雜湊值 / Compute hash value for the given key
     *(blank line)
     * @param {*} key - 要雜湊的鍵
     * @returns {number} 桶的索引（bucket index）
     */  // End of block comment
    _hash(key) {  // Compute a hash-based index so keys map into the table's storage.
        // 將 key 轉為字串後計算雜湊值
        // Convert key to string and compute hash
        const strKey = String(key);  // Assign or update a variable that represents the current algorithm state.
        let hash = 0;  // Assign or update a variable that represents the current algorithm state.

        for (let i = 0; i < strKey.length; i++) {  // Iterate over a range/collection to process each item in sequence.
            // 使用常見的雜湊算法（djb2 變體）
            // Using common hash algorithm (djb2 variant)
            hash = ((hash << 5) - hash + strKey.charCodeAt(i)) | 0;  // Compute a hash-based index so keys map into the table's storage.
        }  // Close the current block scope.

        // 確保回傳正數索引 - Ensure positive index
        return Math.abs(hash) % this._capacity;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 擴容並重新雜湊 / Resize and rehash
     */  // End of block comment
    _rehash() {  // Rehash entries into a larger table to keep operations near O(1) on average.
        const oldBuckets = this._buckets;  // Access or update the bucket storage used to hold entries or chains.
        this._capacity *= 2;  // Grow the table (often doubling) to reduce the load factor and collisions.
        this._buckets = new Array(this._capacity).fill(null).map(() => []);  // Allocate the bucket array and initialize empty chains for each bucket.
        this._size = 0;  // Assign or update a variable that represents the current algorithm state.

        // 重新插入所有元素 - Reinsert all elements
        for (const bucket of oldBuckets) {  // Iterate over a range/collection to process each item in sequence.
            for (const [key, value] of bucket) {  // Iterate over a range/collection to process each item in sequence.
                // 直接插入，不計入統計 - Direct insert without stats
                const index = this._hash(key);  // Compute a hash-based index so keys map into the table's storage.
                this._buckets[index].push([key, value]);  // Access or update the bucket storage used to hold entries or chains.
                this._size++;  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.
    }  // Close the current block scope.

    // ========== 公開方法 Public Methods ==========

    /** Doc block start
     * 插入鍵值對（若 key 已存在則更新）
     * Insert key-value pair (update if key exists)
     *(blank line)
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *(blank line)
     * @param {*} key - 鍵
     * @param {*} value - 值
     * @returns {number} 探測次數 - Number of probes
     */  // End of block comment
    insert(key, value) {  // Execute this statement as part of the data structure implementation.
        const index = this._hash(key);  // Compute a hash-based index so keys map into the table's storage.
        const bucket = this._buckets[index];  // Access or update the bucket storage used to hold entries or chains.
        let probes = 0;  // Advance or track the probing sequence used by open addressing.

        // 檢查 key 是否已存在 - Check if key exists
        for (let i = 0; i < bucket.length; i++) {  // Iterate over a range/collection to process each item in sequence.
            probes++;  // Advance or track the probing sequence used by open addressing.
            if (bucket[i][0] === key) {  // Evaluate the condition and branch into the appropriate code path.
                bucket[i][1] = value; // 更新 - Update existing
                this._totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                this._operations++;  // Execute this statement as part of the data structure implementation.
                return probes;  // Return the computed result to the caller.
            }  // Close the current block scope.
        }  // Close the current block scope.

        // 新增鍵值對 - Add new key-value pair
        probes++; // 新增位置也算一次探測 - Adding also counts as a probe
        bucket.push([key, value]);  // Access or update the bucket storage used to hold entries or chains.
        this._size++;  // Execute this statement as part of the data structure implementation.
        this._totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
        this._operations++;  // Execute this statement as part of the data structure implementation.

        // 檢查是否需要擴容 - Check if rehashing needed
        if (this.loadFactor > ChainedHashTable.MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
            this._rehash();  // Rehash entries into a larger table to keep operations near O(1) on average.
        }  // Close the current block scope.

        return probes;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 搜尋給定 key 的 value
     * Search for value associated with key
     *(blank line)
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *(blank line)
     * @param {*} key - 要搜尋的鍵
     * @returns {{value: *, probes: number}|null} 找到則回傳物件，否則回傳 null
     */  // End of block comment
    search(key) {  // Execute this statement as part of the data structure implementation.
        const index = this._hash(key);  // Compute a hash-based index so keys map into the table's storage.
        const bucket = this._buckets[index];  // Access or update the bucket storage used to hold entries or chains.
        let probes = 0;  // Advance or track the probing sequence used by open addressing.

        for (const [k, v] of bucket) {  // Iterate over a range/collection to process each item in sequence.
            probes++;  // Advance or track the probing sequence used by open addressing.
            if (k === key) {  // Evaluate the condition and branch into the appropriate code path.
                this._totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                this._operations++;  // Execute this statement as part of the data structure implementation.
                return { value: v, probes };  // Return the computed result to the caller.
            }  // Close the current block scope.
        }  // Close the current block scope.

        // 未找到 - Not found
        this._totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
        this._operations++;  // Execute this statement as part of the data structure implementation.
        return null;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 刪除指定的鍵值對 / Delete key-value pair
     *(blank line)
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *(blank line)
     * @param {*} key - 要刪除的鍵
     * @returns {{success: boolean, probes: number}} 刪除結果與探測次數
     */  // End of block comment
    delete(key) {  // Execute this statement as part of the data structure implementation.
        const index = this._hash(key);  // Compute a hash-based index so keys map into the table's storage.
        const bucket = this._buckets[index];  // Access or update the bucket storage used to hold entries or chains.
        let probes = 0;  // Advance or track the probing sequence used by open addressing.

        for (let i = 0; i < bucket.length; i++) {  // Iterate over a range/collection to process each item in sequence.
            probes++;  // Advance or track the probing sequence used by open addressing.
            if (bucket[i][0] === key) {  // Evaluate the condition and branch into the appropriate code path.
                bucket.splice(i, 1);  // Access or update the bucket storage used to hold entries or chains.
                this._size--;  // Execute this statement as part of the data structure implementation.
                this._totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
                this._operations++;  // Execute this statement as part of the data structure implementation.
                return { success: true, probes };  // Return the computed result to the caller.
            }  // Close the current block scope.
        }  // Close the current block scope.

        // 未找到 - Not found
        this._totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
        this._operations++;  // Execute this statement as part of the data structure implementation.
        return { success: false, probes };  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 檢查 key 是否存在
     * Check if key exists
     *(blank line)
     * @param {*} key - 要檢查的鍵
     * @returns {boolean}
     */  // End of block comment
    contains(key) {  // Execute this statement as part of the data structure implementation.
        return this.search(key) !== null;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 計算負載因子 / Calculate load factor
     *(blank line)
     * @returns {number} 負載因子（元素數 / 桶數）
     */  // End of block comment
    get loadFactor() {  // Execute this statement as part of the data structure implementation.
        return this._size / this._capacity;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 回傳元素數量 / Return number of elements
     *(blank line)
     * @returns {number}
     */  // End of block comment
    get size() {  // Execute this statement as part of the data structure implementation.
        return this._size;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 回傳桶的數量 / Return number of buckets
     *(blank line)
     * @returns {number}
     */  // End of block comment
    get capacity() {  // Execute this statement as part of the data structure implementation.
        return this._capacity;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 檢查是否為空 / Check if empty
     *(blank line)
     * @returns {boolean}
     */  // End of block comment
    isEmpty() {  // Execute this statement as part of the data structure implementation.
        return this._size === 0;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 清空雜湊表 / Clear all elements
     */  // End of block comment
    clear() {  // Execute this statement as part of the data structure implementation.
        this._buckets = new Array(this._capacity).fill(null).map(() => []);  // Allocate the bucket array and initialize empty chains for each bucket.
        this._size = 0;  // Assign or update a variable that represents the current algorithm state.
        this._totalProbes = 0;  // Assign or update a variable that represents the current algorithm state.
        this._operations = 0;  // Assign or update a variable that represents the current algorithm state.
    }  // Close the current block scope.

    /** Doc block start
     * 取得平均探測次數 / Get average number of probes per operation
     *(blank line)
     * @returns {number} 平均探測次數
     */  // End of block comment
    getAverageProbes() {  // Execute this statement as part of the data structure implementation.
        return this._operations === 0 ? 0 : this._totalProbes / this._operations;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 取得最長鏈結長度 / Get maximum chain length
     *(blank line)
     * @returns {number} 最長鏈結的長度
     */  // End of block comment
    getMaxChainLength() {  // Execute this statement as part of the data structure implementation.
        let maxLength = 0;  // Assign or update a variable that represents the current algorithm state.
        for (const bucket of this._buckets) {  // Iterate over a range/collection to process each item in sequence.
            if (bucket.length > maxLength) {  // Evaluate the condition and branch into the appropriate code path.
                maxLength = bucket.length;  // Access or update the bucket storage used to hold entries or chains.
            }  // Close the current block scope.
        }  // Close the current block scope.
        return maxLength;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 取得統計資訊 / Get statistics
     *(blank line)
     * @returns {Object} 統計資訊物件
     */  // End of block comment
    getStatistics() {  // Execute this statement as part of the data structure implementation.
        return {  // Return the computed result to the caller.
            size: this._size,  // Execute this statement as part of the data structure implementation.
            capacity: this._capacity,  // Execute this statement as part of the data structure implementation.
            loadFactor: this.loadFactor,  // Execute this statement as part of the data structure implementation.
            totalProbes: this._totalProbes,  // Execute this statement as part of the data structure implementation.
            operations: this._operations,  // Execute this statement as part of the data structure implementation.
            averageProbes: this.getAverageProbes(),  // Execute this statement as part of the data structure implementation.
            maxChainLength: this.getMaxChainLength()  // Execute this statement as part of the data structure implementation.
        };  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    // ========== 迭代器 Iterators ==========

    /** Doc block start
     * 回傳所有鍵的陣列 / Return array of all keys
     *(blank line)
     * @returns {Array}
     */  // End of block comment
    keys() {  // Execute this statement as part of the data structure implementation.
        const result = [];  // Assign or update a variable that represents the current algorithm state.
        for (const bucket of this._buckets) {  // Iterate over a range/collection to process each item in sequence.
            for (const [key] of bucket) {  // Iterate over a range/collection to process each item in sequence.
                result.push(key);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.
        return result;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 回傳所有值的陣列 / Return array of all values
     *(blank line)
     * @returns {Array}
     */  // End of block comment
    values() {  // Execute this statement as part of the data structure implementation.
        const result = [];  // Assign or update a variable that represents the current algorithm state.
        for (const bucket of this._buckets) {  // Iterate over a range/collection to process each item in sequence.
            for (const [, value] of bucket) {  // Iterate over a range/collection to process each item in sequence.
                result.push(value);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.
        return result;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 回傳所有鍵值對的陣列 / Return array of all key-value pairs
     *(blank line)
     * @returns {Array}
     */  // End of block comment
    entries() {  // Execute this statement as part of the data structure implementation.
        const result = [];  // Assign or update a variable that represents the current algorithm state.
        for (const bucket of this._buckets) {  // Iterate over a range/collection to process each item in sequence.
            for (const entry of bucket) {  // Iterate over a range/collection to process each item in sequence.
                result.push([...entry]);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.
        return result;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 字串表示 / String representation
     *(blank line)
     * @returns {string}
     */  // End of block comment
    toString() {  // Execute this statement as part of the data structure implementation.
        const items = this.entries()  // Assign or update a variable that represents the current algorithm state.
            .map(([k, v]) => `${String(k)}: ${String(v)}`)  // Assign or update a variable that represents the current algorithm state.
            .join(', ');  // Execute this statement as part of the data structure implementation.
        return `ChainedHashTable{${items}}`;  // Return the computed result to the caller.
    }  // Close the current block scope.
}  // Close the current block scope.

// ========== 匯出 Export ==========
// CommonJS 匯出 - CommonJS export
if (typeof module !== 'undefined' && module.exports) {  // Evaluate the condition and branch into the appropriate code path.
    module.exports = ChainedHashTable;  // Assign or update a variable that represents the current algorithm state.
}  // Close the current block scope.

// ========== 範例執行 Example Usage ==========
if (typeof require !== 'undefined' && require.main === module) {  // Evaluate the condition and branch into the appropriate code path.
    console.log('=== 鏈結法雜湊表示範 Chained Hash Table Demo ===\n');  // Execute this statement as part of the data structure implementation.

    // 建立雜湊表 - Create hash table
    const ht = new ChainedHashTable(8);  // Assign or update a variable that represents the current algorithm state.

    // 插入操作 - Insert operations
    console.log('插入鍵值對 Inserting key-value pairs:');  // Execute this statement as part of the data structure implementation.
    const probes1 = ht.insert('apple', 100);  // Advance or track the probing sequence used by open addressing.
    console.log(`  插入 'apple': ${probes1} 次探測`);  // Advance or track the probing sequence used by open addressing.
    const probes2 = ht.insert('banana', 200);  // Advance or track the probing sequence used by open addressing.
    console.log(`  插入 'banana': ${probes2} 次探測`);  // Advance or track the probing sequence used by open addressing.
    const probes3 = ht.insert('cherry', 300);  // Advance or track the probing sequence used by open addressing.
    console.log(`  插入 'cherry': ${probes3} 次探測`);  // Advance or track the probing sequence used by open addressing.
    console.log(`  雜湊表: ${ht}\n`);  // Execute this statement as part of the data structure implementation.

    // 搜尋操作 - Search operations
    console.log('搜尋操作 Search operations:');  // Execute this statement as part of the data structure implementation.
    const result1 = ht.search('apple');  // Assign or update a variable that represents the current algorithm state.
    console.log(`  search('apple') = ${result1.value}, 探測: ${result1.probes}`);  // Advance or track the probing sequence used by open addressing.
    const result2 = ht.search('banana');  // Assign or update a variable that represents the current algorithm state.
    console.log(`  search('banana') = ${result2.value}, 探測: ${result2.probes}`);  // Advance or track the probing sequence used by open addressing.
    const result3 = ht.search('grape');  // Assign or update a variable that represents the current algorithm state.
    console.log(`  search('grape') = ${result3}, 未找到\n`);  // Assign or update a variable that represents the current algorithm state.

    // 刪除操作 - Delete operation
    console.log('刪除操作 Delete operation:');  // Execute this statement as part of the data structure implementation.
    const delResult = ht.delete('banana');  // Assign or update a variable that represents the current algorithm state.
    console.log(`  刪除 'banana': 成功=${delResult.success}, 探測=${delResult.probes}\n`);  // Advance or track the probing sequence used by open addressing.

    // 統計資訊 - Statistics
    console.log('統計資訊 Statistics:');  // Execute this statement as part of the data structure implementation.
    const stats = ht.getStatistics();  // Assign or update a variable that represents the current algorithm state.
    console.log(`  大小 Size: ${stats.size}`);  // Execute this statement as part of the data structure implementation.
    console.log(`  容量 Capacity: ${stats.capacity}`);  // Execute this statement as part of the data structure implementation.
    console.log(`  負載因子 Load Factor: ${stats.loadFactor.toFixed(3)}`);  // Execute this statement as part of the data structure implementation.
    console.log(`  總探測次數 Total Probes: ${stats.totalProbes}`);  // Execute this statement as part of the data structure implementation.
    console.log(`  總操作次數 Operations: ${stats.operations}`);  // Execute this statement as part of the data structure implementation.
    console.log(`  平均探測次數 Average Probes: ${stats.averageProbes.toFixed(3)}`);  // Execute this statement as part of the data structure implementation.
    console.log(`  最長鏈結 Max Chain: ${stats.maxChainLength}`);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.
