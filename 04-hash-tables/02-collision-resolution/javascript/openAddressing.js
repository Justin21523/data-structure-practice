/**
 * 開放定址法雜湊表（Open Addressing Hash Table）- JavaScript 實作
 * 支援線性探測、二次探測與雙重雜湊
 *
 * Open Addressing Hash Table implementation
 * Supporting linear probing, quadratic probing, and double hashing
 */

/**
 * 探測方法列舉
 * Probe method enumeration
 */
const ProbeMethod = {
    LINEAR: 'LINEAR',           // 線性探測 - Linear probing
    QUADRATIC: 'QUADRATIC',     // 二次探測 - Quadratic probing
    DOUBLE_HASH: 'DOUBLE_HASH'  // 雙重雜湊 - Double hashing
};

/**
 * 已刪除標記（墓碑）
 * Deleted marker (tombstone)
 */
const DELETED = Symbol('DELETED');

class OpenAddressingHashTable {
    // ========== 常數 Constants ==========
    static DEFAULT_CAPACITY = 16;
    static MAX_LOAD_FACTOR = 0.5;  // 開放定址法需要較低的負載因子

    /**
     * 建構子：初始化開放定址雜湊表
     * Constructor: Initialize open addressing hash table
     *
     * @param {number} capacity - 表的大小（table size）
     * @param {string} probeMethod - 探測方法（LINEAR, QUADRATIC, DOUBLE_HASH）
     */
    constructor(capacity = OpenAddressingHashTable.DEFAULT_CAPACITY,
                probeMethod = ProbeMethod.LINEAR) {
        if (capacity <= 0) {
            throw new Error('容量必須為正整數 / Capacity must be positive');
        }
        if (!Object.values(ProbeMethod).includes(probeMethod)) {
            throw new Error('無效的探測方法 / Invalid probe method');
        }

        this._capacity = capacity;
        this._size = 0;
        this._probeMethod = probeMethod;

        // 使用陣列儲存鍵值對 - Use array to store key-value pairs
        // null 表示空位，DELETED 表示已刪除
        // null means empty, DELETED means deleted
        this._table = new Array(capacity).fill(null);

        // 統計資訊 - Statistics
        this._totalProbes = 0;   // 總探測次數 - Total number of probes
        this._operations = 0;     // 總操作次數 - Total number of operations
    }

    // ========== 私有方法 Private Methods ==========

    /**
     * 主雜湊函數
     * Primary hash function
     *
     * @param {*} key - 要雜湊的鍵
     * @returns {number} 雜湊值
     */
    _hash1(key) {
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
     * 次要雜湊函數（用於雙重雜湊）
     * Secondary hash function (for double hashing)
     *
     * @param {*} key - 要雜湊的鍵
     * @returns {number} 雜湊值（1 到 capacity-1）
     */
    _hash2(key) {
        const strKey = String(key);
        let hash = 0;

        for (let i = 0; i < strKey.length; i++) {
            // 使用不同的雜湊算法
            // Using a different hash algorithm
            hash = ((hash << 4) + strKey.charCodeAt(i)) | 0;
        }

        // 確保回傳 1 到 capacity-1 之間的質數
        // Ensure return value between 1 and capacity-1 (preferably prime)
        const result = Math.abs(hash) % (this._capacity - 1);
        return result === 0 ? 1 : result;
    }

    /**
     * 計算探測位置
     * Calculate probe position
     *
     * @param {*} key - 鍵
     * @param {number} attempt - 探測次數（從 0 開始）
     * @returns {number} 探測位置索引
     */
    _probe(key, attempt) {
        const h1 = this._hash1(key);

        switch (this._probeMethod) {
            case ProbeMethod.LINEAR:
                // 線性探測：h(k, i) = (h1(k) + i) mod m
                // Linear probing: h(k, i) = (h1(k) + i) mod m
                return (h1 + attempt) % this._capacity;

            case ProbeMethod.QUADRATIC:
                // 二次探測：h(k, i) = (h1(k) + c1*i + c2*i²) mod m
                // Quadratic probing: h(k, i) = (h1(k) + c1*i + c2*i²) mod m
                // 使用 c1=1, c2=1
                const c1 = 1, c2 = 1;
                return (h1 + c1 * attempt + c2 * attempt * attempt) % this._capacity;

            case ProbeMethod.DOUBLE_HASH:
                // 雙重雜湊：h(k, i) = (h1(k) + i*h2(k)) mod m
                // Double hashing: h(k, i) = (h1(k) + i*h2(k)) mod m
                const h2 = this._hash2(key);
                return (h1 + attempt * h2) % this._capacity;

            default:
                return h1;
        }
    }

    /**
     * 尋找鍵的位置
     * Find position of key
     *
     * @param {*} key - 要尋找的鍵
     * @returns {{index: number, probes: number, found: boolean}} 位置資訊
     */
    _findPosition(key) {
        let probes = 0;
        let firstDeletedIndex = -1;

        for (let i = 0; i < this._capacity; i++) {
            probes++;
            const index = this._probe(key, i);
            const entry = this._table[index];

            if (entry === null) {
                // 找到空位 - Found empty slot
                // 如果之前遇到過 DELETED，返回第一個 DELETED 位置
                // If we encountered DELETED before, return first deleted position
                return {
                    index: firstDeletedIndex !== -1 ? firstDeletedIndex : index,
                    probes,
                    found: false
                };
            } else if (entry === DELETED) {
                // 記錄第一個 DELETED 位置 - Record first deleted position
                if (firstDeletedIndex === -1) {
                    firstDeletedIndex = index;
                }
                // 繼續探測 - Continue probing
            } else if (entry[0] === key) {
                // 找到鍵 - Found the key
                return { index, probes, found: true };
            }
        }

        // 表已滿 - Table is full
        return {
            index: firstDeletedIndex !== -1 ? firstDeletedIndex : -1,
            probes,
            found: false
        };
    }

    /**
     * 擴容並重新雜湊
     * Resize and rehash
     */
    _rehash() {
        const oldTable = this._table;
        const oldCapacity = this._capacity;

        this._capacity *= 2;
        this._table = new Array(this._capacity).fill(null);
        this._size = 0;

        // 重新插入所有元素 - Reinsert all elements
        for (let i = 0; i < oldCapacity; i++) {
            const entry = oldTable[i];
            if (entry !== null && entry !== DELETED) {
                // 直接插入，不計入統計 - Direct insert without stats
                const [key, value] = entry;
                const { index } = this._findPosition(key);
                if (index !== -1) {
                    this._table[index] = [key, value];
                    this._size++;
                }
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
        const { index, probes, found } = this._findPosition(key);

        if (index === -1) {
            throw new Error('雜湊表已滿 / Hash table is full');
        }

        this._table[index] = [key, value];
        if (!found) {
            this._size++;
        }

        this._totalProbes += probes;
        this._operations++;

        // 檢查是否需要擴容 - Check if rehashing needed
        if (this.loadFactor > OpenAddressingHashTable.MAX_LOAD_FACTOR) {
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
        const { index, probes, found } = this._findPosition(key);

        this._totalProbes += probes;
        this._operations++;

        if (found) {
            return { value: this._table[index][1], probes };
        }
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
        const { index, probes, found } = this._findPosition(key);

        this._totalProbes += probes;
        this._operations++;

        if (found) {
            // 使用墓碑標記 - Use tombstone marker
            this._table[index] = DELETED;
            this._size--;
            return { success: true, probes };
        }
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
     * @returns {number} 負載因子（元素數 / 表大小）
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
     * 回傳表的大小
     * Return table size
     *
     * @returns {number}
     */
    get capacity() {
        return this._capacity;
    }

    /**
     * 回傳探測方法
     * Return probe method
     *
     * @returns {string}
     */
    get probeMethod() {
        return this._probeMethod;
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
        this._table = new Array(this._capacity).fill(null);
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
     * 取得統計資訊
     * Get statistics
     *
     * @returns {Object} 統計資訊物件
     */
    getStatistics() {
        let deletedCount = 0;
        for (const entry of this._table) {
            if (entry === DELETED) {
                deletedCount++;
            }
        }

        return {
            size: this._size,
            capacity: this._capacity,
            loadFactor: this.loadFactor,
            probeMethod: this._probeMethod,
            totalProbes: this._totalProbes,
            operations: this._operations,
            averageProbes: this.getAverageProbes(),
            deletedSlots: deletedCount
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
        for (const entry of this._table) {
            if (entry !== null && entry !== DELETED) {
                result.push(entry[0]);
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
        for (const entry of this._table) {
            if (entry !== null && entry !== DELETED) {
                result.push(entry[1]);
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
        for (const entry of this._table) {
            if (entry !== null && entry !== DELETED) {
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
        return `OpenAddressingHashTable[${this._probeMethod}]{${items}}`;
    }
}

// ========== 匯出 Export ==========
// CommonJS 匯出 - CommonJS export
if (typeof module !== 'undefined' && module.exports) {
    module.exports = { OpenAddressingHashTable, ProbeMethod, DELETED };
}

// ========== 範例執行 Example Usage ==========
if (typeof require !== 'undefined' && require.main === module) {
    console.log('=== 開放定址法雜湊表示範 Open Addressing Hash Table Demo ===\n');

    // 測試線性探測 - Test linear probing
    console.log('--- 線性探測 Linear Probing ---');
    const linearHT = new OpenAddressingHashTable(8, ProbeMethod.LINEAR);

    console.log('插入操作 Insert operations:');
    console.log(`  插入 'apple': ${linearHT.insert('apple', 100)} 次探測`);
    console.log(`  插入 'banana': ${linearHT.insert('banana', 200)} 次探測`);
    console.log(`  插入 'cherry': ${linearHT.insert('cherry', 300)} 次探測`);
    console.log(`  ${linearHT}\n`);

    console.log('搜尋操作 Search operations:');
    const r1 = linearHT.search('apple');
    console.log(`  search('apple') = ${r1.value}, 探測: ${r1.probes}`);

    console.log('\n統計資訊 Statistics:');
    console.log(JSON.stringify(linearHT.getStatistics(), null, 2));

    // 測試二次探測 - Test quadratic probing
    console.log('\n--- 二次探測 Quadratic Probing ---');
    const quadraticHT = new OpenAddressingHashTable(8, ProbeMethod.QUADRATIC);
    quadraticHT.insert('apple', 100);
    quadraticHT.insert('banana', 200);
    quadraticHT.insert('cherry', 300);
    console.log(`  ${quadraticHT}\n`);

    // 測試雙重雜湊 - Test double hashing
    console.log('--- 雙重雜湊 Double Hashing ---');
    const doubleHT = new OpenAddressingHashTable(8, ProbeMethod.DOUBLE_HASH);
    doubleHT.insert('apple', 100);
    doubleHT.insert('banana', 200);
    doubleHT.insert('cherry', 300);
    console.log(`  ${doubleHT}\n`);
}
