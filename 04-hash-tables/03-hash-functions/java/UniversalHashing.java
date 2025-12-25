// 04 通用雜湊（Java）/ Universal hashing (Java).  // Bilingual file header.

import java.util.ArrayList;  // Use ArrayList to store buckets and chains.  
import java.util.List;  // Use List for clear collection types.  
import java.util.Random;  // Use Random as a simple RNG for choosing (a, b) parameters.  

public final class UniversalHashing {  // Provide universal hash families and a small hash table that can rehash on attacks.  
    private UniversalHashing() {  // Prevent instantiation because this is a utility container.  
        // No instances.  // Clarify intent.  
    }  // Close constructor.  

    public static boolean isPrime(int n) {  // Check whether n is prime (trial division).  
        if (n < 2) {  // Handle 0, 1, and negatives.  
            return false;  // Not prime.  
        }  // Close base-case branch.  
        if (n == 2) {  // Handle the only even prime.  
            return true;  // 2 is prime.  
        }  // Close base-case branch.  
        if (n % 2 == 0) {  // Reject other even numbers quickly.  
            return false;  // Not prime.  
        }  // Close even branch.  
        int limit = (int)Math.floor(Math.sqrt((double)n));  // Compute integer sqrt limit for trial division.  
        for (int i = 3; i <= limit; i += 2) {  // Try odd divisors only.  
            if (n % i == 0) {  // Detect a divisor.  
                return false;  // Not prime.  
            }  // Close divisor branch.  
        }  // Close loop.  
        return true;  // No divisors found => prime.  
    }  // End isPrime().  

    public static int nextPrime(int n) {  // Find the smallest prime >= n.  
        int x = Math.max(n, 2);  // Ensure we start at least from 2.  
        while (!isPrime(x)) {  // Repeat until a prime is found.  
            x += 1;  // Move to next candidate.  
        }  // Close loop.  
        return x;  // Return the prime candidate.  
    }  // End nextPrime().  

    public static final class UniversalHashFamily {  // Represent h_{a,b}(k) = ((a*k + b) mod p) mod m.  
        private final int m;  // Bucket count (table size).  
        private final int p;  // Prime modulus (p should be > universe size).  
        private final Random rng;  // RNG used to choose parameters (a, b).  
        private int a;  // Random multiplier in [1, p-1].  
        private int b;  // Random offset in [0, p-1].  

        public UniversalHashFamily(int m) {  // Construct with default prime selection and non-deterministic RNG.  
            this(m, null, new Random());  // Delegate to the internal constructor.  
        }  // Close constructor.  

        public UniversalHashFamily(int m, long seed) {  // Construct with a deterministic RNG seed (useful for tests).  
            this(m, null, new Random(seed));  // Delegate to the internal constructor.  
        }  // Close constructor.  

        public UniversalHashFamily(int m, Integer p, long seed) {  // Construct with explicit prime p and deterministic RNG seed.  
            this(m, p, new Random(seed));  // Delegate to the internal constructor.  
        }  // Close constructor.  

        private UniversalHashFamily(int m, Integer p, Random rng) {  // Shared constructor logic.  
            if (m <= 0) {  // Validate bucket count.  
                throw new IllegalArgumentException("m must be >= 1");  // Signal invalid input.  
            }  // Close validation.  
            this.m = m;  // Store bucket count.  
            int chosenP = (p != null) ? p.intValue() : nextPrime(Math.max(m * 10, 10007));  // Pick a prime similar to the Python reference.  
            this.p = chosenP;  // Store prime modulus.  
            this.rng = rng;  // Store RNG for parameter generation.  
            regenerate();  // Choose initial (a, b) parameters.  
        }  // Close constructor.  

        public void regenerate() {  // Choose new random parameters (a, b) from their allowed ranges.  
            this.a = 1 + this.rng.nextInt(this.p - 1);  // Pick a in [1, p-1].  
            this.b = this.rng.nextInt(this.p);  // Pick b in [0, p-1].  
        }  // End regenerate().  

        public int hash(int key) {  // Compute hash value in [0, m).  
            long linear = ((long)this.a) * ((long)key) + ((long)this.b);  // Compute a*k + b in 64-bit.  
            int modP = Math.floorMod(linear, this.p);  // Reduce mod p (and keep non-negative).  
            return modP % this.m;  // Reduce mod m to get the bucket index.  
        }  // End hash().  

        public int call(int key) {  // Provide a method that mimics Python's __call__.  
            return hash(key);  // Delegate to hash().  
        }  // End call().  

        public int getM() {  // Expose m for inspection and debugging.  
            return this.m;  // Return bucket count.  
        }  // End getM().  

        public int getP() {  // Expose p for inspection and debugging.  
            return this.p;  // Return prime modulus.  
        }  // End getP().  

        public int getA() {  // Expose a for inspection and debugging.  
            return this.a;  // Return multiplier.  
        }  // End getA().  

        public int getB() {  // Expose b for inspection and debugging.  
            return this.b;  // Return offset.  
        }  // End getB().  
    }  // End UniversalHashFamily.  

    public static final class UniversalStringHashFamily {  // Represent a universal-ish polynomial hash family for strings.  
        private final int m;  // Bucket count.  
        private final int p;  // Prime modulus.  
        private final Random rng;  // RNG used to choose parameter a.  
        private int a;  // Random base in [1, p-1].  

        public UniversalStringHashFamily(int m) {  // Construct with default prime selection and non-deterministic RNG.  
            this(m, null, new Random());  // Delegate to shared constructor.  
        }  // Close constructor.  

        public UniversalStringHashFamily(int m, long seed) {  // Construct with deterministic RNG seed (useful for tests).  
            this(m, null, new Random(seed));  // Delegate to shared constructor.  
        }  // Close constructor.  

        public UniversalStringHashFamily(int m, Integer p, long seed) {  // Construct with explicit prime p and deterministic RNG seed.  
            this(m, p, new Random(seed));  // Delegate to shared constructor.  
        }  // Close constructor.  

        private UniversalStringHashFamily(int m, Integer p, Random rng) {  // Shared constructor logic.  
            if (m <= 0) {  // Validate bucket count.  
                throw new IllegalArgumentException("m must be >= 1");  // Signal invalid input.  
            }  // Close validation.  
            this.m = m;  // Store bucket count.  
            int chosenP = (p != null) ? p.intValue() : nextPrime(Math.max(m * 10, 10007));  // Pick a prime similar to the Python reference.  
            this.p = chosenP;  // Store prime modulus.  
            this.rng = rng;  // Store RNG for parameter generation.  
            regenerate();  // Choose initial base a.  
        }  // Close constructor.  

        public void regenerate() {  // Choose a new random base a in [1, p-1].  
            this.a = 1 + this.rng.nextInt(this.p - 1);  // Pick base parameter.  
        }  // End regenerate().  

        public int hash(String s) {  // Compute string hash value in [0, m).  
            long h = 0L;  // Initialize polynomial accumulator.  
            for (int i = 0; i < s.length(); i++) {  // Iterate code units for consistency with Python ord().  
                h = (h * (long)this.a + (long)((int)s.charAt(i))) % (long)this.p;  // Apply Horner step mod p.  
            }  // Close loop.  
            return (int)(h % (long)this.m);  // Reduce mod m for bucket index.  
        }  // End hash().  

        public int call(String s) {  // Provide a method that mimics Python's __call__.  
            return hash(s);  // Delegate to hash().  
        }  // End call().  
    }  // End UniversalStringHashFamily.  

    public static final class UniversalHashTable<V> {  // A simple chained hash table for int keys using universal hashing.  
        public static final int DEFAULT_CAPACITY = 16;  // Default number of buckets.  
        public static final double MAX_LOAD_FACTOR = 0.75;  // Resize threshold.  
        public static final int MAX_CHAIN_LENGTH = 10;  // Chain length threshold to trigger rehash defense.  

        private int capacity;  // Current bucket array size.  
        private int size;  // Current number of key-value pairs.  
        private List<List<Entry<V>>> buckets;  // Bucket array: each bucket is a chain (list) of entries.  
        private UniversalHashFamily hashFamily;  // Hash family used to compute bucket indices.  
        private int rehashCount;  // How many times we regenerated hash parameters due to long chains.  

        public UniversalHashTable() {  // Construct with default capacity.  
            this(DEFAULT_CAPACITY);  // Delegate to capacity constructor.  
        }  // Close constructor.  

        public UniversalHashTable(int capacity) {  // Construct with a specified capacity.  
            if (capacity <= 0) {  // Validate capacity so we can allocate buckets.  
                throw new IllegalArgumentException("capacity must be >= 1");  // Signal invalid input.  
            }  // Close validation.  
            this.capacity = capacity;  // Store capacity.  
            this.size = 0;  // Start empty.  
            this.buckets = allocateBuckets(capacity);  // Allocate empty bucket chains.  
            this.hashFamily = new UniversalHashFamily(capacity);  // Choose a hash family for this capacity.  
            this.rehashCount = 0;  // Start with no rehashes.  
        }  // Close constructor.  

        private static <V> List<List<Entry<V>>> allocateBuckets(int capacity) {  // Allocate bucket chains for a given capacity.  
            List<List<Entry<V>>> out = new ArrayList<>();  // Create top-level bucket list.  
            for (int i = 0; i < capacity; i++) {  // Create one empty chain per bucket.  
                out.add(new ArrayList<>());  // Add a new empty bucket chain.  
            }  // Close loop.  
            return out;  // Return allocated buckets.  
        }  // End allocateBuckets().  

        private int hash(int key) {  // Compute bucket index for a key.  
            return this.hashFamily.hash(key);  // Delegate to the current hash family.  
        }  // End hash().  

        public void insert(int key, V value) {  // Insert or update a key-value pair.  
            int index = hash(key);  // Compute bucket index.  
            List<Entry<V>> bucket = this.buckets.get(index);  // Get the chain at that bucket.  

            for (int i = 0; i < bucket.size(); i++) {  // Search for existing key in this chain.  
                Entry<V> e = bucket.get(i);  // Read one entry.  
                if (e.key == key) {  // Update existing key.  
                    bucket.set(i, new Entry<>(key, value));  // Replace entry with updated value.  
                    return;  // Size does not change for updates.  
                }  // Close match branch.  
            }  // Close search loop.  

            bucket.add(new Entry<>(key, value));  // Append new entry to the chain.  
            this.size += 1;  // Increase size for a new key.  

            if (loadFactor() > MAX_LOAD_FACTOR) {  // Resize when load factor is too high.  
                resize();  // Rehash into a larger table.  
            } else if (bucket.size() > MAX_CHAIN_LENGTH) {  // Regenerate hash family when a chain becomes suspiciously long.  
                regenerateHash();  // Rehash with new (a, b) parameters.  
            }  // Close defense branch.  
        }  // End insert().  

        public V search(int key) {  // Search for a key and return its value (or null when missing).  
            int index = hash(key);  // Compute bucket index.  
            for (Entry<V> e : this.buckets.get(index)) {  // Scan chain linearly.  
                if (e.key == key) {  // Return value on match.  
                    return e.value;  // Found.  
                }  // Close match branch.  
            }  // Close scan loop.  
            return null;  // Not found.  
        }  // End search().  

        public boolean delete(int key) {  // Delete a key-value pair; return true when removed.  
            int index = hash(key);  // Compute bucket index.  
            List<Entry<V>> bucket = this.buckets.get(index);  // Get the chain at that bucket.  
            for (int i = 0; i < bucket.size(); i++) {  // Scan chain for key.  
                if (bucket.get(i).key == key) {  // Match found.  
                    bucket.remove(i);  // Remove entry from chain.  
                    this.size -= 1;  // Decrease size.  
                    return true;  // Report success.  
                }  // Close match branch.  
            }  // Close scan loop.  
            return false;  // Not found.  
        }  // End delete().  

        private void resize() {  // Double capacity and reinsert all entries (rehash).  
            List<List<Entry<V>>> oldBuckets = this.buckets;  // Save old bucket array.  
            this.capacity *= 2;  // Double capacity.  
            this.buckets = allocateBuckets(this.capacity);  // Allocate new empty buckets.  
            this.hashFamily = new UniversalHashFamily(this.capacity);  // Choose a new hash family for the new size.  
            this.size = 0;  // Reset size and reinsert to recompute bucket indices.  

            for (List<Entry<V>> bucket : oldBuckets) {  // Iterate old buckets.  
                for (Entry<V> e : bucket) {  // Reinsert each entry into the new table.  
                    insert(e.key, e.value);  // Reuse insert to maintain invariants.  
                }  // Close entry loop.  
            }  // Close bucket loop.  
        }  // End resize().  

        private void regenerateHash() {  // Regenerate (a, b) and redistribute without changing capacity.  
            this.rehashCount += 1;  // Count this defensive rehash.  
            List<List<Entry<V>>> oldBuckets = this.buckets;  // Save old buckets.  
            this.buckets = allocateBuckets(this.capacity);  // Allocate fresh empty buckets.  
            this.hashFamily.regenerate();  // Choose new random (a, b) parameters.  
            this.size = 0;  // Reset size and reinsert entries.  

            for (List<Entry<V>> bucket : oldBuckets) {  // Iterate old buckets.  
                for (Entry<V> e : bucket) {  // Reinsert each entry with the new hash function.  
                    insert(e.key, e.value);  // Reuse insert to maintain invariants.  
                }  // Close entry loop.  
            }  // Close bucket loop.  
        }  // End regenerateHash().  

        public double loadFactor() {  // Compute current load factor alpha = n / m.  
            return ((double)this.size) / ((double)this.capacity);  // Return load factor as double.  
        }  // End loadFactor().  

        public int size() {  // Expose current size (number of pairs).  
            return this.size;  // Return size.  
        }  // End size().  

        public int capacity() {  // Expose current bucket array length.  
            return this.capacity;  // Return capacity.  
        }  // End capacity().  

        public int rehashCount() {  // Expose number of defensive rehashes.  
            return this.rehashCount;  // Return rehash count.  
        }  // End rehashCount().  

        public int getMaxChainLength() {  // Compute the maximum chain length across all buckets.  
            int max = 0;  // Track maximum chain length.  
            for (List<Entry<V>> bucket : this.buckets) {  // Scan all buckets.  
                if (bucket.size() > max) {  // Update max when needed.  
                    max = bucket.size();  // Store new maximum.  
                }  // Close max update.  
            }  // Close scan loop.  
            return max;  // Return maximum chain length.  
        }  // End getMaxChainLength().  

        private static final class Entry<V> {  // Represent one key-value pair stored in a chain.  
            private final int key;  // Stored key (int).  
            private final V value;  // Stored value.  

            private Entry(int key, V value) {  // Construct a key-value entry.  
                this.key = key;  // Store key.  
                this.value = value;  // Store value.  
            }  // Close constructor.  
        }  // End Entry.  
    }  // End UniversalHashTable.  
}  // End UniversalHashing.  

