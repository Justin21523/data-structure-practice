/**
 * 鏈結法雜湊表（Chained Hash Table）- JavaScript 實作
 * 使用鏈結法（chaining）處理碰撞，並追蹤探測次數
 *
 * Chained Hash Table implementation with collision resolution
 * Using chaining method and tracking probe statistics
 */

class ChainedHashTable {
    // ========== 常數 Constants ==========
    static DEFAULT_CAPACITY = 16;
    static MAX_LOAD_FACTOR = 0.75;

    /**
     * 建構子：初始化鏈結雜湊表
     * Constructor: Initialize chained hash table
     *
     * @param {number} capacity - 桶的數量（number of buckets）
     */
    constructor(capacity = ChainedHashTable.DEFAULT_CAPACITY) {
        if (capacity <= 0) {
            throw new Error('容量必須為正整數 / Capacity must be positive');
        }
        this._capacity = capacity;
        this._size = 0;
        // 使用陣列的陣列來實作鏈結法
        // Use array of arrays for chaining
        this._buckets = new Array(capacity).fill(null).map(() => []);

        // 統計資訊 - Statistics
        this._totalProbes = 0;  // 總探測次數 - Total number of probes
        this._operations = 0;    // 總操作次數 - Total number of operations
    }

    // ========== 私有方法 Private Methods ==========

    /**
     * 計算雜湊值
     * Compute hash value for the given key
     *
     * @param {*} key - 要雜湊的鍵
     * @returns {number} 桶的索引（bucket index）
     */
    _hash(key) {
        // 將 key 轉為字串後計算雜湊值
        // Convert key to string and compute hash
        const strKey = String(key);
        let hash = 0;

        for (let i = 0; i < strKey.length; i++) {
            // 使用常見的雜湊算法（djb2 變體）
            // Using common hash algorithm (djb2 variant)
            hash = ((hash << 5) - hash + strKey.charCodeAt(i)) | 0;
        }

        // 確保回傳正數索引 - Ensure positive index
        return Math.abs(hash) % this._capacity;
    }

    /**
     * 擴容並重新雜湊
     * Resize and rehash
     */
    _rehash() {
        const oldBuckets = this._buckets;
        this._capacity *= 2;
        this._buckets = new Array(this._capacity).fill(null).map(() => []);
        this._size = 0;

        // 重新插入所有元素 - Reinsert all elements
        for (const bucket of oldBuckets) {
            for (const [key, value] of bucket) {
                // 直接插入，不計入統計 - Direct insert without stats
                const index = this._hash(key);
                this._buckets[index].push([key, value]);
                this._size++;
            }
        }
    }

    // ========== 公開方法 Public Methods ==========

    /**
     * 插入鍵值對（若 key 已存在則更新）
     * Insert key-value pair (update if key exists)
     *
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *
     * @param {*} key - 鍵
     * @param {*} value - 值
     * @returns {number} 探測次數 - Number of probes
     */
    insert(key, value) {
        const index = this._hash(key);
        const bucket = this._buckets[index];
        let probes = 0;

        // 檢查 key 是否已存在 - Check if key exists
        for (let i = 0; i < bucket.length; i++) {
            probes++;
            if (bucket[i][0] === key) {
                bucket[i][1] = value; // 更新 - Update existing
                this._totalProbes += probes;
                this._operations++;
                return probes;
            }
        }

        // 新增鍵值對 - Add new key-value pair
        probes++; // 新增位置也算一次探測 - Adding also counts as a probe
        bucket.push([key, value]);
        this._size++;
        this._totalProbes += probes;
        this._operations++;

        // 檢查是否需要擴容 - Check if rehashing needed
        if (this.loadFactor > ChainedHashTable.MAX_LOAD_FACTOR) {
            this._rehash();
        }

        return probes;
    }

    /**
     * 搜尋給定 key 的 value
     * Search for value associated with key
     *
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *
     * @param {*} key - 要搜尋的鍵
     * @returns {{value: *, probes: number}|null} 找到則回傳物件，否則回傳 null
     */
    search(key) {
        const index = this._hash(key);
        const bucket = this._buckets[index];
        let probes = 0;

        for (const [k, v] of bucket) {
            probes++;
            if (k === key) {
                this._totalProbes += probes;
                this._operations++;
                return { value: v, probes };
            }
        }

        // 未找到 - Not found
        this._totalProbes += probes;
        this._operations++;
        return null;
    }

    /**
     * 刪除指定的鍵值對
     * Delete key-value pair
     *
     * 時間複雜度 Time Complexity: 平均 O(1), 最差 O(n)
     *
     * @param {*} key - 要刪除的鍵
     * @returns {{success: boolean, probes: number}} 刪除結果與探測次數
     */
    delete(key) {
        const index = this._hash(key);
        const bucket = this._buckets[index];
        let probes = 0;

        for (let i = 0; i < bucket.length; i++) {
            probes++;
            if (bucket[i][0] === key) {
                bucket.splice(i, 1);
                this._size--;
                this._totalProbes += probes;
                this._operations++;
                return { success: true, probes };
            }
        }

        // 未找到 - Not found
        this._totalProbes += probes;
        this._operations++;
        return { success: false, probes };
    }

    /**
     * 檢查 key 是否存在
     * Check if key exists
     *
     * @param {*} key - 要檢查的鍵
     * @returns {boolean}
     */
    contains(key) {
        return this.search(key) !== null;
    }

    /**
     * 計算負載因子
     * Calculate load factor
     *
     * @returns {number} 負載因子（元素數 / 桶數）
     */
    get loadFactor() {
        return this._size / this._capacity;
    }

    /**
     * 回傳元素數量
     * Return number of elements
     *
     * @returns {number}
     */
    get size() {
        return this._size;
    }

    /**
     * 回傳桶的數量
     * Return number of buckets
     *
     * @returns {number}
     */
    get capacity() {
        return this._capacity;
    }

    /**
     * 檢查是否為空
     * Check if empty
     *
     * @returns {boolean}
     */
    isEmpty() {
        return this._size === 0;
    }

    /**
     * 清空雜湊表
     * Clear all elements
     */
    clear() {
        this._buckets = new Array(this._capacity).fill(null).map(() => []);
        this._size = 0;
        this._totalProbes = 0;
        this._operations = 0;
    }

    /**
     * 取得平均探測次數
     * Get average number of probes per operation
     *
     * @returns {number} 平均探測次數
     */
    getAverageProbes() {
        return this._operations === 0 ? 0 : this._totalProbes / this._operations;
    }

    /**
     * 取得最長鏈結長度
     * Get maximum chain length
     *
     * @returns {number} 最長鏈結的長度
     */
    getMaxChainLength() {
        let maxLength = 0;
        for (const bucket of this._buckets) {
            if (bucket.length > maxLength) {
                maxLength = bucket.length;
            }
        }
        return maxLength;
    }

    /**
     * 取得統計資訊
     * Get statistics
     *
     * @returns {Object} 統計資訊物件
     */
    getStatistics() {
        return {
            size: this._size,
            capacity: this._capacity,
            loadFactor: this.loadFactor,
            totalProbes: this._totalProbes,
            operations: this._operations,
            averageProbes: this.getAverageProbes(),
            maxChainLength: this.getMaxChainLength()
        };
    }

    // ========== 迭代器 Iterators ==========

    /**
     * 回傳所有鍵的陣列
     * Return array of all keys
     *
     * @returns {Array}
     */
    keys() {
        const result = [];
        for (const bucket of this._buckets) {
            for (const [key] of bucket) {
                result.push(key);
            }
        }
        return result;
    }

    /**
     * 回傳所有值的陣列
     * Return array of all values
     *
     * @returns {Array}
     */
    values() {
        const result = [];
        for (const bucket of this._buckets) {
            for (const [, value] of bucket) {
                result.push(value);
            }
        }
        return result;
    }

    /**
     * 回傳所有鍵值對的陣列
     * Return array of all key-value pairs
     *
     * @returns {Array}
     */
    entries() {
        const result = [];
        for (const bucket of this._buckets) {
            for (const entry of bucket) {
                result.push([...entry]);
            }
        }
        return result;
    }

    /**
     * 字串表示
     * String representation
     *
     * @returns {string}
     */
    toString() {
        const items = this.entries()
            .map(([k, v]) => `${String(k)}: ${String(v)}`)
            .join(', ');
        return `ChainedHashTable{${items}}`;
    }
}

// ========== 匯出 Export ==========
// CommonJS 匯出 - CommonJS export
if (typeof module !== 'undefined' && module.exports) {
    module.exports = ChainedHashTable;
}

// ========== 範例執行 Example Usage ==========
if (typeof require !== 'undefined' && require.main === module) {
    console.log('=== 鏈結法雜湊表示範 Chained Hash Table Demo ===\n');

    // 建立雜湊表 - Create hash table
    const ht = new ChainedHashTable(8);

    // 插入操作 - Insert operations
    console.log('插入鍵值對 Inserting key-value pairs:');
    const probes1 = ht.insert('apple', 100);
    console.log(`  插入 'apple': ${probes1} 次探測`);
    const probes2 = ht.insert('banana', 200);
    console.log(`  插入 'banana': ${probes2} 次探測`);
    const probes3 = ht.insert('cherry', 300);
    console.log(`  插入 'cherry': ${probes3} 次探測`);
    console.log(`  雜湊表: ${ht}\n`);

    // 搜尋操作 - Search operations
    console.log('搜尋操作 Search operations:');
    const result1 = ht.search('apple');
    console.log(`  search('apple') = ${result1.value}, 探測: ${result1.probes}`);
    const result2 = ht.search('banana');
    console.log(`  search('banana') = ${result2.value}, 探測: ${result2.probes}`);
    const result3 = ht.search('grape');
    console.log(`  search('grape') = ${result3}, 未找到\n`);

    // 刪除操作 - Delete operation
    console.log('刪除操作 Delete operation:');
    const delResult = ht.delete('banana');
    console.log(`  刪除 'banana': 成功=${delResult.success}, 探測=${delResult.probes}\n`);

    // 統計資訊 - Statistics
    console.log('統計資訊 Statistics:');
    const stats = ht.getStatistics();
    console.log(`  大小 Size: ${stats.size}`);
    console.log(`  容量 Capacity: ${stats.capacity}`);
    console.log(`  負載因子 Load Factor: ${stats.loadFactor.toFixed(3)}`);
    console.log(`  總探測次數 Total Probes: ${stats.totalProbes}`);
    console.log(`  總操作次數 Operations: ${stats.operations}`);
    console.log(`  平均探測次數 Average Probes: ${stats.averageProbes.toFixed(3)}`);
    console.log(`  最長鏈結 Max Chain: ${stats.maxChainLength}`);
}
