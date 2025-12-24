/** Doc block start
 * 雜湊表（Hash Table）- JavaScript 實作
 * 使用鏈結法（chaining）處理碰撞
 *(blank line)
 * Hash Table implementation using chaining for collision resolution
 */  // End of block comment

class HashTable {  // Declare the HashTable class that provides insert/search/delete operations.
    // ========== 常數 Constants ==========
    static DEFAULT_CAPACITY = 16;  // Assign or update a variable that represents the current algorithm state.
    static MAX_LOAD_FACTOR = 0.75;  // Assign or update a variable that represents the current algorithm state.

    /** Doc block start
     * 建構子：初始化雜湊表 / Constructor: Initialize hash table
     *(blank line)
     * @param {number} capacity - 桶的數量（number of buckets）
     */  // End of block comment
    constructor(capacity = HashTable.DEFAULT_CAPACITY) {  // Assign or update a variable that represents the current algorithm state.
        if (capacity <= 0) {  // Evaluate the condition and branch into the appropriate code path.
            throw new Error('容量必須為正整數 / Capacity must be positive');  // Throw an exception to signal an invalid argument or operation.
        }  // Close the current block scope.
        this._capacity = capacity;  // Assign or update a variable that represents the current algorithm state.
        this._size = 0;  // Assign or update a variable that represents the current algorithm state.
        // 使用陣列的陣列來實作鏈結法 / Use array of arrays for chaining
        this._buckets = new Array(capacity).fill(null).map(() => []);  // Allocate the bucket array and initialize empty chains for each bucket.
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
            // 使用常見的雜湊算法 / Using common hash algorithm (djb2 variant)
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
                this.insert(key, value);  // Execute this statement as part of the data structure implementation.
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
     */  // End of block comment
    insert(key, value) {  // Execute this statement as part of the data structure implementation.
        const index = this._hash(key);  // Compute a hash-based index so keys map into the table's storage.
        const bucket = this._buckets[index];  // Access or update the bucket storage used to hold entries or chains.

        // 檢查 key 是否已存在 - Check if key exists
        for (let i = 0; i < bucket.length; i++) {  // Iterate over a range/collection to process each item in sequence.
            if (bucket[i][0] === key) {  // Evaluate the condition and branch into the appropriate code path.
                bucket[i][1] = value; // 更新 - Update existing
                return;  // Return the computed result to the caller.
            }  // Close the current block scope.
        }  // Close the current block scope.

        // 新增鍵值對 - Add new key-value pair
        bucket.push([key, value]);  // Access or update the bucket storage used to hold entries or chains.
        this._size++;  // Execute this statement as part of the data structure implementation.

        // 檢查是否需要擴容 - Check if rehashing needed
        if (this.loadFactor > HashTable.MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
            this._rehash();  // Rehash entries into a larger table to keep operations near O(1) on average.
        }  // Close the current block scope.
    }  // Close the current block scope.

    /** Doc block start
     * 搜尋給定 key 的 value
     * Search for value associated with key
     *(blank line)
     * @param {*} key - 要搜尋的鍵
     * @returns {*} 找到則回傳 value，否則回傳 undefined
     */  // End of block comment
    search(key) {  // Execute this statement as part of the data structure implementation.
        const index = this._hash(key);  // Compute a hash-based index so keys map into the table's storage.
        const bucket = this._buckets[index];  // Access or update the bucket storage used to hold entries or chains.

        for (const [k, v] of bucket) {  // Iterate over a range/collection to process each item in sequence.
            if (k === key) {  // Evaluate the condition and branch into the appropriate code path.
                return v;  // Return the computed result to the caller.
            }  // Close the current block scope.
        }  // Close the current block scope.
        return undefined;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 刪除指定的鍵值對 / Delete key-value pair
     *(blank line)
     * @param {*} key - 要刪除的鍵
     * @returns {boolean} 刪除成功回傳 true，key 不存在回傳 false
     */  // End of block comment
    delete(key) {  // Execute this statement as part of the data structure implementation.
        const index = this._hash(key);  // Compute a hash-based index so keys map into the table's storage.
        const bucket = this._buckets[index];  // Access or update the bucket storage used to hold entries or chains.

        for (let i = 0; i < bucket.length; i++) {  // Iterate over a range/collection to process each item in sequence.
            if (bucket[i][0] === key) {  // Evaluate the condition and branch into the appropriate code path.
                bucket.splice(i, 1);  // Access or update the bucket storage used to hold entries or chains.
                this._size--;  // Execute this statement as part of the data structure implementation.
                return true;  // Return the computed result to the caller.
            }  // Close the current block scope.
        }  // Close the current block scope.
        return false;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 檢查 key 是否存在
     * Check if key exists
     *(blank line)
     * @param {*} key - 要檢查的鍵
     * @returns {boolean}
     */  // End of block comment
    contains(key) {  // Execute this statement as part of the data structure implementation.
        return this.search(key) !== undefined;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 使用 get 方法（等同於 search）
     * Get method (alias for search)
     */  // End of block comment
    get(key) {  // Execute this statement as part of the data structure implementation.
        return this.search(key);  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 使用 set 方法（等同於 insert）
     * Set method (alias for insert)
     */  // End of block comment
    set(key, value) {  // Execute this statement as part of the data structure implementation.
        this.insert(key, value);  // Execute this statement as part of the data structure implementation.
        return this;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 使用 has 方法（等同於 contains）
     * Has method (alias for contains)
     */  // End of block comment
    has(key) {  // Execute this statement as part of the data structure implementation.
        return this.contains(key);  // Return the computed result to the caller.
    }  // Close the current block scope.

    // ========== 屬性 Properties ==========

    /** Doc block start
     * 計算負載因子 / Calculate load factor
     */  // End of block comment
    get loadFactor() {  // Execute this statement as part of the data structure implementation.
        return this._size / this._capacity;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 回傳元素數量 / Return number of elements
     */  // End of block comment
    get size() {  // Execute this statement as part of the data structure implementation.
        return this._size;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 回傳桶的數量 / Return number of buckets
     */  // End of block comment
    get capacity() {  // Execute this statement as part of the data structure implementation.
        return this._capacity;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 檢查是否為空 / Check if empty
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
    }  // Close the current block scope.

    // ========== 迭代器 Iterators ==========

    /** Doc block start
     * 回傳所有鍵的陣列 / Return array of all keys
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
     * 對每個鍵值對執行回呼函數 / Execute callback for each key-value pair
     */  // End of block comment
    forEach(callback) {  // Execute this statement as part of the data structure implementation.
        for (const bucket of this._buckets) {  // Iterate over a range/collection to process each item in sequence.
            for (const [key, value] of bucket) {  // Iterate over a range/collection to process each item in sequence.
                callback(value, key, this);  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.
    }  // Close the current block scope.

    /** Doc block start
     * 實作迭代器協定 / Implement iterator protocol
     */  // End of block comment
    *[Symbol.iterator]() {
        for (const bucket of this._buckets) {  // Iterate over a range/collection to process each item in sequence.
            for (const entry of bucket) {  // Iterate over a range/collection to process each item in sequence.
                yield entry;  // Execute this statement as part of the data structure implementation.
            }  // Close the current block scope.
        }  // Close the current block scope.
    }  // Close the current block scope.

    /** Doc block start
     * 字串表示 / String representation
     */  // End of block comment
    toString() {  // Execute this statement as part of the data structure implementation.
        const items = this.entries()  // Assign or update a variable that represents the current algorithm state.
            .map(([k, v]) => `${String(k)}: ${String(v)}`)  // Assign or update a variable that represents the current algorithm state.
            .join(', ');  // Execute this statement as part of the data structure implementation.
        return `HashTable{${items}}`;  // Return the computed result to the caller.
    }  // Close the current block scope.
}  // Close the current block scope.

// ========== 匯出 Export ==========
// CommonJS 匯出 - CommonJS export
if (typeof module !== 'undefined' && module.exports) {  // Evaluate the condition and branch into the appropriate code path.
    module.exports = HashTable;  // Assign or update a variable that represents the current algorithm state.
}  // Close the current block scope.

// ========== 範例執行 Example Usage ==========
if (typeof require !== 'undefined' && require.main === module) {  // Evaluate the condition and branch into the appropriate code path.
    console.log('=== 雜湊表基本操作示範 Hash Table Basic Operations Demo ===\n');  // Execute this statement as part of the data structure implementation.

    // 建立雜湊表 - Create hash table
    const ht = new HashTable();  // Assign or update a variable that represents the current algorithm state.

    // 插入操作 - Insert operations
    console.log('插入鍵值對 Inserting key-value pairs:');  // Execute this statement as part of the data structure implementation.
    ht.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
    ht.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
    ht.insert('cherry', 300);  // Execute this statement as part of the data structure implementation.
    ht.set('date', 400); // 使用 set 方法
    console.log(`  雜湊表: ${ht}`);  // Execute this statement as part of the data structure implementation.
    console.log(`  大小: ${ht.size}, 負載因子: ${ht.loadFactor.toFixed(3)}\n`);  // Execute this statement as part of the data structure implementation.

    // 搜尋操作 - Search operations
    console.log('搜尋操作 Search operations:');  // Execute this statement as part of the data structure implementation.
    console.log(`  search('apple') = ${ht.search('apple')}`);  // Assign or update a variable that represents the current algorithm state.
    console.log(`  get('banana') = ${ht.get('banana')}`);  // Assign or update a variable that represents the current algorithm state.
    console.log(`  search('grape') = ${ht.search('grape')}`);  // Assign or update a variable that represents the current algorithm state.
    console.log(`  has('cherry') = ${ht.has('cherry')}\n`);  // Assign or update a variable that represents the current algorithm state.

    // 更新操作 - Update operation
    console.log('更新操作 Update operation:');  // Execute this statement as part of the data structure implementation.
    ht.insert('apple', 150);  // Execute this statement as part of the data structure implementation.
    console.log('  更新 apple 的值為 150');  // Execute this statement as part of the data structure implementation.
    console.log(`  search('apple') = ${ht.search('apple')}\n`);  // Assign or update a variable that represents the current algorithm state.

    // 刪除操作 - Delete operation
    console.log('刪除操作 Delete operations:');  // Execute this statement as part of the data structure implementation.
    ht.delete('banana');  // Execute this statement as part of the data structure implementation.
    console.log(`  刪除 'banana' 後: ${ht}`);  // Execute this statement as part of the data structure implementation.
    console.log(`  大小: ${ht.size}\n`);  // Execute this statement as part of the data structure implementation.

    // 迭代操作 - Iteration
    console.log('迭代操作 Iteration:');  // Execute this statement as part of the data structure implementation.
    console.log(`  所有鍵 Keys: ${JSON.stringify(ht.keys())}`);  // Execute this statement as part of the data structure implementation.
    console.log(`  所有值 Values: ${JSON.stringify(ht.values())}`);  // Execute this statement as part of the data structure implementation.
    console.log(`  所有項目 Entries: ${JSON.stringify(ht.entries())}`);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.
