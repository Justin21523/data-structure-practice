/** Doc block start
 * 開放定址法雜湊表（Open Addressing Hash Table）- JavaScript 實作
 * 支援線性探測、二次探測與雙重雜湊 / 支援 linear probing 、 quadratic probing 與 double hashing
 *(blank line)
 * Open Addressing Hash Table implementation
 * Supporting linear probing, quadratic probing, and double hashing
 */  // End of block comment

/** Doc block start
 * 探測方法列舉 / Probe method enumeration
 */  // End of block comment
const ProbeMethod = {  // Assign or update a variable that represents the current algorithm state.
    LINEAR: 'LINEAR',           // 線性探測 - Linear probing
    QUADRATIC: 'QUADRATIC',     // 二次探測 - Quadratic probing
    DOUBLE_HASH: 'DOUBLE_HASH'  // 雙重雜湊 - Double hashing
};  // Execute this statement as part of the data structure implementation.

/** Doc block start
 * 已刪除標記（墓碑） / Deleted marker (tombstone)
 */  // End of block comment
const DELETED = Symbol('DELETED');  // Handle tombstones so deletions do not break the probing/search sequence.

class OpenAddressingHashTable {  // Declare the HashTable class that provides insert/search/delete operations.
    // ========== 常數 Constants ==========
    static DEFAULT_CAPACITY = 16;  // Assign or update a variable that represents the current algorithm state.
    static MAX_LOAD_FACTOR = 0.5;  // 開放定址法需要較低的負載因子

    /** Doc block start
     * 建構子：初始化開放定址雜湊表 / Constructor: Initialize open addressing hash table
     *(blank line)
     * @param {number} capacity - 表的大小（table size）
     * @param {string} probeMethod - 探測方法（LINEAR, QUADRATIC, DOUBLE_HASH）
     */  // End of block comment
    constructor(capacity = OpenAddressingHashTable.DEFAULT_CAPACITY,  // Assign or update a variable that represents the current algorithm state.
                probeMethod = ProbeMethod.LINEAR) {  // Advance or track the probing sequence used by open addressing.
        if (capacity <= 0) {  // Evaluate the condition and branch into the appropriate code path.
            throw new Error('容量必須為正整數 / Capacity must be positive');  // Throw an exception to signal an invalid argument or operation.
        }  // Close the current block scope.
        if (!Object.values(ProbeMethod).includes(probeMethod)) {  // Evaluate the condition and branch into the appropriate code path.
            throw new Error('無效的探測方法 / Invalid probe method');  // Throw an exception to signal an invalid argument or operation.
        }  // Close the current block scope.

        this._capacity = capacity;  // Assign or update a variable that represents the current algorithm state.
        this._size = 0;  // Assign or update a variable that represents the current algorithm state.
        this._probeMethod = probeMethod;  // Advance or track the probing sequence used by open addressing.

        // 使用陣列儲存鍵值對 - Use array to store key-value pairs
        // null 表示空位，DELETED 表示已刪除
        // null means empty, DELETED means deleted
        this._table = new Array(capacity).fill(null);  // Assign or update a variable that represents the current algorithm state.

        // 統計資訊 - Statistics
        this._totalProbes = 0;   // 總探測次數 - Total number of probes
        this._operations = 0;     // 總操作次數 - Total number of operations
    }  // Close the current block scope.

    // ========== 私有方法 Private Methods ==========

    /** Doc block start
     * 主雜湊函數 / Primary hash function
     *(blank line)
     * @param {*} key - 要雜湊的鍵
     * @returns {number} 雜湊值
     */  // End of block comment
    _hash1(key) {  // Compute a hash-based index so keys map into the table's storage.
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
     * 次要雜湊函數（用於雙重雜湊） / Secondary hash function (for double hashing)
     *(blank line)
     * @param {*} key - 要雜湊的鍵
     * @returns {number} 雜湊值（1 到 capacity-1）
     */  // End of block comment
    _hash2(key) {  // Compute a hash-based index so keys map into the table's storage.
        const strKey = String(key);  // Assign or update a variable that represents the current algorithm state.
        let hash = 0;  // Assign or update a variable that represents the current algorithm state.

        for (let i = 0; i < strKey.length; i++) {  // Iterate over a range/collection to process each item in sequence.
            // 使用不同的雜湊算法 / Using a different hash algorithm
            hash = ((hash << 4) + strKey.charCodeAt(i)) | 0;  // Compute a hash-based index so keys map into the table's storage.
        }  // Close the current block scope.

        // 確保回傳 1 到 capacity-1 之間的質數
        // Ensure return value between 1 and capacity-1 (preferably prime)
        const result = Math.abs(hash) % (this._capacity - 1);  // Normalize the hash to a non-negative bucket index within the table range.
        return result === 0 ? 1 : result;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 計算探測位置 / Calculate probe position
     *(blank line)
     * @param {*} key - 鍵
     * @param {number} attempt - 探測次數（從 0 開始）
     * @returns {number} 探測位置索引
     */  // End of block comment
    _probe(key, attempt) {  // Advance or track the probing sequence used by open addressing.
        const h1 = this._hash1(key);  // Compute a hash-based index so keys map into the table's storage.

        switch (this._probeMethod) {  // Advance or track the probing sequence used by open addressing.
            case ProbeMethod.LINEAR:  // Execute this statement as part of the data structure implementation.
                // 線性探測：h(k, i) = (h1(k) + i) mod m
                // Linear probing: h(k, i) = (h1(k) + i) mod m
                return (h1 + attempt) % this._capacity;  // Return the computed result to the caller.

            case ProbeMethod.QUADRATIC:  // Execute this statement as part of the data structure implementation.
                // 二次探測：h(k, i) = (h1(k) + c1*i + c2*i²) mod m
                // Quadratic probing: h(k, i) = (h1(k) + c1*i + c2*i²) mod m
                // 使用 c1=1, c2=1
                const c1 = 1, c2 = 1;  // Assign or update a variable that represents the current algorithm state.
                return (h1 + c1 * attempt + c2 * attempt * attempt) % this._capacity;  // Return the computed result to the caller.

            case ProbeMethod.DOUBLE_HASH:  // Execute this statement as part of the data structure implementation.
                // 雙重雜湊：h(k, i) = (h1(k) + i*h2(k)) mod m
                // Double hashing: h(k, i) = (h1(k) + i*h2(k)) mod m
                const h2 = this._hash2(key);  // Compute a hash-based index so keys map into the table's storage.
                return (h1 + attempt * h2) % this._capacity;  // Return the computed result to the caller.

            default:  // Execute this statement as part of the data structure implementation.
                return h1;  // Return the computed result to the caller.
        }  // Close the current block scope.
    }  // Close the current block scope.

    /** Doc block start
     * 尋找鍵的位置 / Find position of key
     *(blank line)
     * @param {*} key - 要尋找的鍵
     * @returns {{index: number, probes: number, found: boolean}} 位置資訊
     */  // End of block comment
    _findPosition(key) {  // Execute this statement as part of the data structure implementation.
        let probes = 0;  // Advance or track the probing sequence used by open addressing.
        let firstDeletedIndex = -1;  // Assign or update a variable that represents the current algorithm state.

        for (let i = 0; i < this._capacity; i++) {  // Iterate over a range/collection to process each item in sequence.
            probes++;  // Advance or track the probing sequence used by open addressing.
            const index = this._probe(key, i);  // Advance or track the probing sequence used by open addressing.
            const entry = this._table[index];  // Assign or update a variable that represents the current algorithm state.

            if (entry === null) {  // Evaluate the condition and branch into the appropriate code path.
                // 找到空位 - Found empty slot
                // 如果之前遇到過 DELETED，返回第一個 DELETED 位置
                // If we encountered DELETED before, return first deleted position
                return {  // Return the computed result to the caller.
                    index: firstDeletedIndex !== -1 ? firstDeletedIndex : index,  // Execute this statement as part of the data structure implementation.
                    probes,  // Advance or track the probing sequence used by open addressing.
                    found: false  // Execute this statement as part of the data structure implementation.
                };  // Execute this statement as part of the data structure implementation.
            } else if (entry === DELETED) {  // Handle tombstones so deletions do not break the probing/search sequence.
                // 記錄第一個 DELETED 位置 - Record first deleted position
                if (firstDeletedIndex === -1) {  // Evaluate the condition and branch into the appropriate code path.
                    firstDeletedIndex = index;  // Assign or update a variable that represents the current algorithm state.
                }  // Close the current block scope.
                // 繼續探測 - Continue probing
            } else if (entry[0] === key) {  // Execute this statement as part of the data structure implementation.
                // 找到鍵 - Found the key
                return { index, probes, found: true };  // Return the computed result to the caller.
            }  // Close the current block scope.
        }  // Close the current block scope.

        // 表已滿 - Table is full
        return {  // Return the computed result to the caller.
            index: firstDeletedIndex !== -1 ? firstDeletedIndex : -1,  // Execute this statement as part of the data structure implementation.
            probes,  // Advance or track the probing sequence used by open addressing.
            found: false  // Execute this statement as part of the data structure implementation.
        };  // Execute this statement as part of the data structure implementation.
    }  // Close the current block scope.

    /** Doc block start
     * 擴容並重新雜湊 / Resize and rehash
     */  // End of block comment
    _rehash() {  // Rehash entries into a larger table to keep operations near O(1) on average.
        const oldTable = this._table;  // Assign or update a variable that represents the current algorithm state.
        const oldCapacity = this._capacity;  // Assign or update a variable that represents the current algorithm state.

        this._capacity *= 2;  // Grow the table (often doubling) to reduce the load factor and collisions.
        this._table = new Array(this._capacity).fill(null);  // Assign or update a variable that represents the current algorithm state.
        this._size = 0;  // Assign or update a variable that represents the current algorithm state.

        // 重新插入所有元素 - Reinsert all elements
        for (let i = 0; i < oldCapacity; i++) {  // Iterate over a range/collection to process each item in sequence.
            const entry = oldTable[i];  // Assign or update a variable that represents the current algorithm state.
            if (entry !== null && entry !== DELETED) {  // Evaluate the condition and branch into the appropriate code path.
                // 直接插入，不計入統計 - Direct insert without stats
                const [key, value] = entry;  // Assign or update a variable that represents the current algorithm state.
                const { index } = this._findPosition(key);  // Assign or update a variable that represents the current algorithm state.
                if (index !== -1) {  // Evaluate the condition and branch into the appropriate code path.
                    this._table[index] = [key, value];  // Assign or update a variable that represents the current algorithm state.
                    this._size++;  // Execute this statement as part of the data structure implementation.
                }  // Close the current block scope.
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
        const { index, probes, found } = this._findPosition(key);  // Advance or track the probing sequence used by open addressing.

        if (index === -1) {  // Evaluate the condition and branch into the appropriate code path.
            throw new Error('雜湊表已滿 / Hash table is full');  // Throw an exception to signal an invalid argument or operation.
        }  // Close the current block scope.

        this._table[index] = [key, value];  // Assign or update a variable that represents the current algorithm state.
        if (!found) {  // Evaluate the condition and branch into the appropriate code path.
            this._size++;  // Execute this statement as part of the data structure implementation.
        }  // Close the current block scope.

        this._totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
        this._operations++;  // Execute this statement as part of the data structure implementation.

        // 檢查是否需要擴容 - Check if rehashing needed
        if (this.loadFactor > OpenAddressingHashTable.MAX_LOAD_FACTOR) {  // Evaluate the condition and branch into the appropriate code path.
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
        const { index, probes, found } = this._findPosition(key);  // Advance or track the probing sequence used by open addressing.

        this._totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
        this._operations++;  // Execute this statement as part of the data structure implementation.

        if (found) {  // Evaluate the condition and branch into the appropriate code path.
            return { value: this._table[index][1], probes };  // Return the computed result to the caller.
        }  // Close the current block scope.
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
        const { index, probes, found } = this._findPosition(key);  // Advance or track the probing sequence used by open addressing.

        this._totalProbes += probes;  // Advance or track the probing sequence used by open addressing.
        this._operations++;  // Execute this statement as part of the data structure implementation.

        if (found) {  // Evaluate the condition and branch into the appropriate code path.
            // 使用墓碑標記 - Use tombstone marker
            this._table[index] = DELETED;  // Handle tombstones so deletions do not break the probing/search sequence.
            this._size--;  // Execute this statement as part of the data structure implementation.
            return { success: true, probes };  // Return the computed result to the caller.
        }  // Close the current block scope.
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
     * @returns {number} 負載因子（元素數 / 表大小）
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
     * 回傳表的大小 / Return table size
     *(blank line)
     * @returns {number}
     */  // End of block comment
    get capacity() {  // Execute this statement as part of the data structure implementation.
        return this._capacity;  // Return the computed result to the caller.
    }  // Close the current block scope.

    /** Doc block start
     * 回傳探測方法 / Return probe method
     *(blank line)
     * @returns {string}
     */  // End of block comment
    get probeMethod() {  // Advance or track the probing sequence used by open addressing.
        return this._probeMethod;  // Return the computed result to the caller.
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
        this._table = new Array(this._capacity).fill(null);  // Assign or update a variable that represents the current algorithm state.
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
     * 取得統計資訊 / Get statistics
     *(blank line)
     * @returns {Object} 統計資訊物件
     */  // End of block comment
    getStatistics() {  // Execute this statement as part of the data structure implementation.
        let deletedCount = 0;  // Handle tombstones so deletions do not break the probing/search sequence.
        for (const entry of this._table) {  // Iterate over a range/collection to process each item in sequence.
            if (entry === DELETED) {  // Evaluate the condition and branch into the appropriate code path.
                deletedCount++;  // Handle tombstones so deletions do not break the probing/search sequence.
            }  // Close the current block scope.
        }  // Close the current block scope.

        return {  // Return the computed result to the caller.
            size: this._size,  // Execute this statement as part of the data structure implementation.
            capacity: this._capacity,  // Execute this statement as part of the data structure implementation.
            loadFactor: this.loadFactor,  // Execute this statement as part of the data structure implementation.
            probeMethod: this._probeMethod,  // Advance or track the probing sequence used by open addressing.
            totalProbes: this._totalProbes,  // Execute this statement as part of the data structure implementation.
            operations: this._operations,  // Execute this statement as part of the data structure implementation.
            averageProbes: this.getAverageProbes(),  // Execute this statement as part of the data structure implementation.
            deletedSlots: deletedCount  // Handle tombstones so deletions do not break the probing/search sequence.
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
        for (const entry of this._table) {  // Iterate over a range/collection to process each item in sequence.
            if (entry !== null && entry !== DELETED) {  // Evaluate the condition and branch into the appropriate code path.
                result.push(entry[0]);  // Execute this statement as part of the data structure implementation.
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
        for (const entry of this._table) {  // Iterate over a range/collection to process each item in sequence.
            if (entry !== null && entry !== DELETED) {  // Evaluate the condition and branch into the appropriate code path.
                result.push(entry[1]);  // Execute this statement as part of the data structure implementation.
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
        for (const entry of this._table) {  // Iterate over a range/collection to process each item in sequence.
            if (entry !== null && entry !== DELETED) {  // Evaluate the condition and branch into the appropriate code path.
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
        return `OpenAddressingHashTable[${this._probeMethod}]{${items}}`;  // Return the computed result to the caller.
    }  // Close the current block scope.
}  // Close the current block scope.

// ========== 匯出 Export ==========
// CommonJS 匯出 - CommonJS export
if (typeof module !== 'undefined' && module.exports) {  // Evaluate the condition and branch into the appropriate code path.
    module.exports = { OpenAddressingHashTable, ProbeMethod, DELETED };  // Handle tombstones so deletions do not break the probing/search sequence.
}  // Close the current block scope.

// ========== 範例執行 Example Usage ==========
if (typeof require !== 'undefined' && require.main === module) {  // Evaluate the condition and branch into the appropriate code path.
    console.log('=== 開放定址法雜湊表示範 Open Addressing Hash Table Demo ===\n');  // Execute this statement as part of the data structure implementation.

    // 測試線性探測 - Test linear probing
    console.log('--- 線性探測 Linear Probing ---');  // Execute this statement as part of the data structure implementation.
    const linearHT = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);  // Assign or update a variable that represents the current algorithm state.

    console.log('插入操作 Insert operations:');  // Execute this statement as part of the data structure implementation.
    console.log(`  插入 'apple': ${linearHT.insert('apple', 100)} 次探測`);  // Execute this statement as part of the data structure implementation.
    console.log(`  插入 'banana': ${linearHT.insert('banana', 200)} 次探測`);  // Execute this statement as part of the data structure implementation.
    console.log(`  插入 'cherry': ${linearHT.insert('cherry', 300)} 次探測`);  // Execute this statement as part of the data structure implementation.
    console.log(`  ${linearHT}\n`);  // Execute this statement as part of the data structure implementation.

    console.log('搜尋操作 Search operations:');  // Execute this statement as part of the data structure implementation.
    const r1 = linearHT.search('apple');  // Assign or update a variable that represents the current algorithm state.
    console.log(`  search('apple') = ${r1.value}, 探測: ${r1.probes}`);  // Advance or track the probing sequence used by open addressing.

    console.log('\n統計資訊 Statistics:');  // Execute this statement as part of the data structure implementation.
    console.log(JSON.stringify(linearHT.getStatistics(), null, 2));  // Execute this statement as part of the data structure implementation.

    // 測試二次探測 - Test quadratic probing
    console.log('\n--- 二次探測 Quadratic Probing ---');  // Execute this statement as part of the data structure implementation.
    const quadraticHT = new OpenAddressingHashTable(8, ProbeMethod.QUADRATIC);  // Assign or update a variable that represents the current algorithm state.
    quadraticHT.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
    quadraticHT.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
    quadraticHT.insert('cherry', 300);  // Execute this statement as part of the data structure implementation.
    console.log(`  ${quadraticHT}\n`);  // Execute this statement as part of the data structure implementation.

    // 測試雙重雜湊 - Test double hashing
    console.log('--- 雙重雜湊 Double Hashing ---');  // Execute this statement as part of the data structure implementation.
    const doubleHT = new OpenAddressingHashTable(8, ProbeMethod.DOUBLE_HASH);  // Assign or update a variable that represents the current algorithm state.
    doubleHT.insert('apple', 100);  // Execute this statement as part of the data structure implementation.
    doubleHT.insert('banana', 200);  // Execute this statement as part of the data structure implementation.
    doubleHT.insert('cherry', 300);  // Execute this statement as part of the data structure implementation.
    console.log(`  ${doubleHT}\n`);  // Execute this statement as part of the data structure implementation.
}  // Close the current block scope.
