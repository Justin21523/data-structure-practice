// 04 通用雜湊（JavaScript）/ Universal hashing (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for safer JavaScript semantics.

// ============================================================  // Section banner: prime helpers.
// Prime Helpers  // Section title.
// ============================================================  // Section banner end.

function isPrime(n) {  // Check whether n is prime (trial division).
    if (n < 2) {  // Handle 0, 1, and negatives.
        return false;  // Not prime.
    }  // Close base-case branch.
    if (n === 2) {  // Handle the only even prime.
        return true;  // 2 is prime.
    }  // Close base-case branch.
    if (n % 2 === 0) {  // Reject other even numbers quickly.
        return false;  // Not prime.
    }  // Close even branch.
    const limit = Math.floor(Math.sqrt(n));  // Compute integer sqrt limit for trial division.
    for (let i = 3; i <= limit; i += 2) {  // Try odd divisors only.
        if (n % i === 0) {  // Detect a divisor.
            return false;  // Not prime.
        }  // Close divisor branch.
    }  // Close loop.
    return true;  // No divisors found => prime.
}  // End isPrime().

function nextPrime(n) {  // Find the smallest prime >= n.
    let x = Math.max(2, Math.floor(n));  // Ensure we start at least from 2 and keep integer.
    while (!isPrime(x)) {  // Repeat until a prime is found.
        x += 1;  // Move to next candidate.
    }  // Close loop.
    return x;  // Return the prime candidate.
}  // End nextPrime().

// ============================================================  // Section banner: deterministic RNG for tests.
// Deterministic RNG (LCG)  // Section title.
// ============================================================  // Section banner end.

class LcgRng {  // Provide a tiny deterministic RNG so tests can be stable without external deps.
    constructor(seed) {  // Initialize RNG state from a 32-bit seed.
        this._state = (seed >>> 0);  // Store state as uint32.
    }  // Close constructor.

    nextUint32() {  // Generate next uint32 value using a classic LCG step.
        this._state = (Math.imul(1664525, this._state) + 1013904223) >>> 0;  // Update state (mod 2^32).
        return this._state;  // Return new state.
    }  // End nextUint32().

    nextInt(minInclusive, maxInclusive) {  // Generate an integer uniformly in [minInclusive, maxInclusive] (mod bias acceptable for tests).
        const range = (maxInclusive - minInclusive + 1);  // Compute range size.
        const v = this.nextUint32() % range;  // Reduce uint32 to range (small bias is fine for this unit).
        return minInclusive + v;  // Shift into desired range.
    }  // End nextInt().
}  // End LcgRng.

// ============================================================  // Section banner: universal hash families.
// Universal Hash Families  // Section title.
// ============================================================  // Section banner end.

class UniversalHashFamily {  // Represent h_{a,b}(k) = ((a*k + b) mod p) mod m.
    constructor(m, p = null, seed = null) {  // Create a family with bucket count m and optional prime p and seed.
        if (m <= 0) {  // Validate bucket count.
            throw new RangeError("m must be >= 1");  // Signal invalid input.
        }  // Close validation.
        this.m = m;  // Store bucket count.
        this.p = (p !== null && p !== undefined) ? p : nextPrime(Math.max(m * 10, 10007));  // Pick prime modulus similar to Python reference.
        const s = (seed !== null && seed !== undefined) ? seed : (Date.now() >>> 0);  // Choose default seed when none is provided.
        this._rng = new LcgRng(s);  // Create deterministic RNG instance.
        this.regenerate();  // Choose initial parameters.
    }  // Close constructor.

    regenerate() {  // Choose new random parameters (a, b).
        this.a = this._rng.nextInt(1, this.p - 1);  // Pick a in [1, p-1].
        this.b = this._rng.nextInt(0, this.p - 1);  // Pick b in [0, p-1].
    }  // End regenerate().

    hash(key) {  // Compute hash value in [0, m).
        const linear = (this.a * key + this.b);  // Compute a*k + b as Number (safe for small demo keys).
        const modP = ((linear % this.p) + this.p) % this.p;  // Reduce mod p and keep non-negative.
        return modP % this.m;  // Reduce mod m for bucket index.
    }  // End hash().
}  // End UniversalHashFamily.

class UniversalStringHashFamily {  // Represent a universal-ish polynomial hash family for strings.
    constructor(m, p = null, seed = null) {  // Create a family with bucket count m and optional prime p and seed.
        if (m <= 0) {  // Validate bucket count.
            throw new RangeError("m must be >= 1");  // Signal invalid input.
        }  // Close validation.
        this.m = m;  // Store bucket count.
        this.p = (p !== null && p !== undefined) ? p : nextPrime(Math.max(m * 10, 10007));  // Pick prime modulus similar to Python reference.
        const s = (seed !== null && seed !== undefined) ? seed : (Date.now() >>> 0);  // Choose default seed when none is provided.
        this._rng = new LcgRng(s);  // Create deterministic RNG instance.
        this.regenerate();  // Choose initial base parameter.
    }  // Close constructor.

    regenerate() {  // Choose a new random base a.
        this.a = this._rng.nextInt(1, this.p - 1);  // Pick a in [1, p-1].
    }  // End regenerate().

    hash(s) {  // Compute string hash value in [0, m).
        let h = 0;  // Initialize polynomial accumulator.
        for (let i = 0; i < s.length; i += 1) {  // Iterate UTF-16 code units.
            h = (h * this.a + s.charCodeAt(i)) % this.p;  // Horner step mod p.
        }  // Close loop.
        return h % this.m;  // Reduce mod m for bucket index.
    }  // End hash().
}  // End UniversalStringHashFamily.

class UniversalHashTable {  // A simple chained hash table for int keys using universal hashing.
    constructor(capacity = 16, seed = null) {  // Initialize empty table with capacity and optional seed.
        if (capacity <= 0) {  // Validate capacity.
            throw new RangeError("capacity must be >= 1");  // Signal invalid input.
        }  // Close validation.
        this._capacity = capacity;  // Store capacity.
        this._size = 0;  // Start empty.
        this._buckets = Array.from({ length: capacity }, () => []);  // Allocate empty bucket chains.
        this._rehashCount = 0;  // Track how many defensive rehashes happened.
        this._seed = (seed !== null && seed !== undefined) ? (seed >>> 0) : (Date.now() >>> 0);  // Store base seed for reproducible rehashes when desired.
        this._hashFamily = new UniversalHashFamily(capacity, null, this._seed);  // Create hash family for current capacity.
    }  // Close constructor.

    get size() {  // Expose current number of key-value pairs.
        return this._size;  // Return size.
    }  // End size getter.

    get loadFactor() {  // Compute current load factor alpha = n / m.
        return this._size / this._capacity;  // Return load factor.
    }  // End loadFactor getter.

    get rehashCount() {  // Expose number of defensive rehashes.
        return this._rehashCount;  // Return rehash count.
    }  // End rehashCount getter.

    _hash(key) {  // Compute bucket index for a key.
        return this._hashFamily.hash(key);  // Delegate to current hash family.
    }  // End _hash().

    insert(key, value) {  // Insert or update a key-value pair.
        const index = this._hash(key);  // Compute bucket index.
        const bucket = this._buckets[index];  // Get chain at bucket.

        for (let i = 0; i < bucket.length; i += 1) {  // Search for existing key.
            const [k] = bucket[i];  // Read existing entry key.
            if (k === key) {  // Update existing key.
                bucket[i] = [key, value];  // Overwrite value.
                return;  // Size does not change for updates.
            }  // Close match branch.
        }  // Close search loop.

        bucket.push([key, value]);  // Append new entry to chain.
        this._size += 1;  // Increase size for new key.

        if (this.loadFactor > 0.75) {  // Resize when load factor is too high.
            this._resize();  // Grow and rehash.
        } else if (bucket.length > 10) {  // Regenerate hash when a chain becomes suspiciously long.
            this._regenerateHash();  // Defensive rehash.
        }  // Close defense branch.
    }  // End insert().

    search(key) {  // Search for a key and return its value (or null when missing).
        const index = this._hash(key);  // Compute bucket index.
        for (const [k, v] of this._buckets[index]) {  // Scan chain linearly.
            if (k === key) {  // Match found.
                return v;  // Return stored value.
            }  // Close match branch.
        }  // Close scan loop.
        return null;  // Not found.
    }  // End search().

    delete(key) {  // Delete a key-value pair; return true when removed.
        const index = this._hash(key);  // Compute bucket index.
        const bucket = this._buckets[index];  // Get chain at bucket.
        for (let i = 0; i < bucket.length; i += 1) {  // Scan chain for key.
            if (bucket[i][0] === key) {  // Match found.
                bucket.splice(i, 1);  // Remove entry from chain.
                this._size -= 1;  // Decrease size.
                return true;  // Report success.
            }  // Close match branch.
        }  // Close scan loop.
        return false;  // Not found.
    }  // End delete().

    _resize() {  // Double capacity and reinsert all entries (rehash).
        const oldBuckets = this._buckets;  // Save old buckets.
        this._capacity *= 2;  // Double capacity.
        this._buckets = Array.from({ length: this._capacity }, () => []);  // Allocate new empty buckets.
        this._hashFamily = new UniversalHashFamily(this._capacity, null, (this._seed + 1) >>> 0);  // Pick new hash family for new capacity.
        this._size = 0;  // Reset size and reinsert entries.

        for (const bucket of oldBuckets) {  // Iterate old buckets.
            for (const [k, v] of bucket) {  // Reinsert each entry.
                this.insert(k, v);  // Reuse insert to maintain invariants.
            }  // Close entry loop.
        }  // Close bucket loop.
    }  // End _resize().

    _regenerateHash() {  // Regenerate (a, b) and redistribute without changing capacity.
        this._rehashCount += 1;  // Count this defensive rehash.
        const oldBuckets = this._buckets;  // Save old buckets.
        this._buckets = Array.from({ length: this._capacity }, () => []);  // Allocate fresh empty buckets.
        this._hashFamily.regenerate();  // Choose new random (a, b) parameters.
        this._size = 0;  // Reset size and reinsert entries.

        for (const bucket of oldBuckets) {  // Iterate old buckets.
            for (const [k, v] of bucket) {  // Reinsert each entry.
                this.insert(k, v);  // Reuse insert to maintain invariants.
            }  // Close entry loop.
        }  // Close bucket loop.
    }  // End _regenerateHash().

    getMaxChainLength() {  // Compute maximum chain length across buckets.
        let max = 0;  // Track maximum chain length.
        for (const bucket of this._buckets) {  // Scan all buckets.
            if (bucket.length > max) {  // Update max when needed.
                max = bucket.length;  // Store new maximum.
            }  // Close max update.
        }  // Close scan loop.
        return max;  // Return maximum chain length.
    }  // End getMaxChainLength().
}  // End UniversalHashTable.

module.exports = {  // Export public API for tests and reuse.
    isPrime,  // Export prime check.
    nextPrime,  // Export next-prime helper.
    LcgRng,  // Export deterministic RNG (useful for tests/demos).
    UniversalHashFamily,  // Export universal hash family for ints.
    UniversalStringHashFamily,  // Export universal string hash family.
    UniversalHashTable,  // Export universal-hashing-backed table.
};  // Close exports.

