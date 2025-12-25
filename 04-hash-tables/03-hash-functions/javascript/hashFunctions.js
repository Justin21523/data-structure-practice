// 04 雜湊函數（JavaScript）/ Hash functions (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for safer JavaScript semantics.

// ============================================================  // Section banner: integer hash functions.
// Integer Hash Functions  // Section title.
// ============================================================  // Section banner end.

function divisionHash(key, m) {  // Compute division-method hash: h(k) = k mod m.
    if (m <= 0) {  // Validate bucket count to avoid invalid modulo.
        throw new RangeError("m must be >= 1");  // Signal invalid input.
    }  // Close validation.
    return ((key % m) + m) % m;  // Use a positive-mod trick so result is always in [0, m).
}  // End divisionHash().

function multiplicationHash(key, m, A = null) {  // Compute multiplication-method hash: floor(m * frac(k*A)).
    if (m <= 0) {  // Validate bucket count so we return a valid index.
        throw new RangeError("m must be >= 1");  // Signal invalid input.
    }  // Close validation.
    const aConst = (A !== null && A !== undefined) ? A : ((Math.sqrt(5) - 1) / 2);  // Use golden ratio conjugate by default.
    const product = key * aConst;  // Multiply key by A in floating-point.
    const fractional = product - Math.floor(product);  // Extract fractional part in [0, 1) (works for negative too).
    const index = Math.floor(m * fractional);  // Scale by m and take floor to get an index.
    return Math.min(index, m - 1);  // Clamp for rare rounding edge cases (fractional could be ~1.0).
}  // End multiplicationHash().

function midSquareHash(key, r) {  // Compute mid-square hash by squaring and extracting middle r digits.
    if (r <= 0) {  // Validate digit count so substring extraction is meaningful.
        throw new RangeError("r must be >= 1");  // Signal invalid input.
    }  // Close validation.
    const squared = Math.abs(key) * Math.abs(key);  // Square the key (Number is fine for small demo inputs).
    const s = String(squared);  // Convert to decimal string for digit extraction.
    if (s.length < r) {  // If there are not enough digits, return the squared value directly (Python reference behavior).
        return squared;  // Return fallback result.
    }  // Close short-length branch.
    const mid = Math.floor(s.length / 2);  // Find the middle position.
    const start = mid - Math.floor(r / 2);  // Choose start so we take r digits around the middle.
    const end = start + r;  // Compute exclusive end index.
    const slice = s.slice(start, end);  // Extract the middle digits.
    return Number.parseInt(slice, 10);  // Convert to number and return.
}  // End midSquareHash().

function foldingHash(key, chunkSize, m) {  // Compute folding hash by chunking digits and summing.
    if (chunkSize <= 0) {  // Validate chunk size to avoid infinite loops.
        throw new RangeError("chunkSize must be >= 1");  // Signal invalid input.
    }  // Close validation.
    if (m <= 0) {  // Validate bucket count so we return a valid range.
        throw new RangeError("m must be >= 1");  // Signal invalid input.
    }  // Close validation.
    const digits = String(Math.abs(key));  // Convert abs(key) to digits string.
    let total = 0;  // Accumulate chunk sums.
    for (let i = 0; i < digits.length; i += chunkSize) {  // Walk the string in chunkSize steps.
        const chunk = digits.slice(i, i + chunkSize);  // Extract one digit chunk.
        total += Number.parseInt(chunk, 10);  // Add chunk value to total.
    }  // Close loop.
    return total % m;  // Reduce mod m to get bucket index.
}  // End foldingHash().

// ============================================================  // Section banner: string hash functions.
// String Hash Functions  // Section title.
// ============================================================  // Section banner end.

function simpleSumHash(s, m) {  // Compute simple sum-of-chars hash (anagrams collide).
    if (m <= 0) {  // Validate bucket count so we return a valid range.
        throw new RangeError("m must be >= 1");  // Signal invalid input.
    }  // Close validation.
    let sum = 0;  // Accumulate code units.
    for (let i = 0; i < s.length; i += 1) {  // Iterate UTF-16 code units.
        sum += s.charCodeAt(i);  // Add one code unit value.
    }  // Close loop.
    return sum % m;  // Reduce mod m to get bucket index.
}  // End simpleSumHash().

function polynomialHash(s, a = 31, m = null) {  // Compute polynomial rolling hash (Horner's method).
    let h = 0;  // Track rolling hash accumulator.
    for (let i = 0; i < s.length; i += 1) {  // Iterate UTF-16 code units.
        h = h * a + s.charCodeAt(i);  // Apply Horner step: h = h*a + c.
        if (m !== null && m !== undefined) {  // Apply modulo when m is provided.
            h %= m;  // Reduce to keep values small and stable.
        }  // Close modulo branch.
    }  // Close loop.
    return h;  // Return hash value (may be not reduced when m is null).
}  // End polynomialHash().

function djb2Hash(s) {  // Compute DJB2 hash (unsigned 32-bit output).
    let h = 5381 >>> 0;  // Initialize with DJB2 offset basis (force uint32).
    for (let i = 0; i < s.length; i += 1) {  // Iterate UTF-16 code units.
        h = (((h << 5) + h) + s.charCodeAt(i)) >>> 0;  // Update: h = h*33 + c (keep uint32).
    }  // Close loop.
    return h >>> 0;  // Return uint32 as a number.
}  // End djb2Hash().

function fnv1aHash(s) {  // Compute FNV-1a 32-bit hash (unsigned 32-bit output).
    let h = 2166136261 >>> 0;  // Initialize with FNV offset basis (uint32).
    for (let i = 0; i < s.length; i += 1) {  // Iterate UTF-16 code units.
        h ^= s.charCodeAt(i);  // XOR in the next code unit.
        h = Math.imul(h, 16777619) >>> 0;  // Multiply in 32-bit space and keep uint32.
    }  // Close loop.
    return h >>> 0;  // Return uint32.
}  // End fnv1aHash().

function jenkinsOneAtATime(s) {  // Compute Jenkins one-at-a-time hash (unsigned 32-bit output).
    let h = 0 >>> 0;  // Initialize accumulator (uint32).
    for (let i = 0; i < s.length; i += 1) {  // Iterate UTF-16 code units.
        h = (h + s.charCodeAt(i)) >>> 0;  // Add character.
        h = (h + (h << 10)) >>> 0;  // Mix with left shift.
        h = (h ^ (h >>> 6)) >>> 0;  // Mix with right shift.
    }  // Close loop.
    h = (h + (h << 3)) >>> 0;  // Final avalanche step 1.
    h = (h ^ (h >>> 11)) >>> 0;  // Final avalanche step 2.
    h = (h + (h << 15)) >>> 0;  // Final avalanche step 3.
    return h >>> 0;  // Return uint32 result.
}  // End jenkinsOneAtATime().

// ============================================================  // Section banner: analysis helpers.
// Hash Function Analysis Tools  // Section title.
// ============================================================  // Section banner end.

function analyzeDistribution(hashFunc, keys, m) {  // Analyze bucket distribution for a given hash function.
    if (m <= 0) {  // Validate bucket count so arrays have a valid size.
        throw new RangeError("m must be >= 1");  // Signal invalid input.
    }  // Close validation.
    const buckets = new Array(m).fill(0);  // Allocate bucket counters.
    for (const key of keys) {  // Hash each key into a bucket.
        const h = hashFunc(key, m);  // Compute bucket index for this key.
        buckets[h] += 1;  // Increment bucket occupancy.
    }  // Close loop.

    let nonEmpty = 0;  // Count non-empty buckets.
    let maxCount = 0;  // Track maximum bucket size.
    let minCount = buckets[0];  // Track minimum bucket size.
    for (const b of buckets) {  // Scan bucket counts to compute summary stats.
        if (b > 0) {  // Identify non-empty buckets.
            nonEmpty += 1;  // Increment non-empty counter.
        }  // Close non-empty branch.
        if (b > maxCount) {  // Update max when needed.
            maxCount = b;  // Store new maximum.
        }  // Close max update.
        if (b < minCount) {  // Update min when needed.
            minCount = b;  // Store new minimum.
        }  // Close min update.
    }  // Close scan loop.

    const avg = keys.length / m;  // Compute average bucket size (n/m).
    let variance = 0.0;  // Accumulate variance over buckets.
    for (const b of buckets) {  // Compute squared deviations from the mean.
        const diff = b - avg;  // Compute deviation.
        variance += diff * diff;  // Accumulate squared deviation.
    }  // Close variance loop.
    variance /= m;  // Convert sum to average.
    const stdDev = Math.sqrt(variance);  // Compute standard deviation.

    return {  // Return a report object similar to the Python reference.
        totalKeys: keys.length,  // Store total key count.
        buckets: m,  // Store bucket count.
        nonEmptyBuckets: nonEmpty,  // Store non-empty bucket count.
        maxBucketSize: maxCount,  // Store max bucket size.
        minBucketSize: minCount,  // Store min bucket size.
        avgBucketSize: avg,  // Store average bucket size.
        stdDeviation: stdDev,  // Store std deviation.
        distribution: buckets,  // Store raw distribution array.
    };  // Close report.
}  // End analyzeDistribution().

module.exports = {  // Export public API for tests and reuse.
    divisionHash,  // Export division hash.
    multiplicationHash,  // Export multiplication hash.
    midSquareHash,  // Export mid-square hash.
    foldingHash,  // Export folding hash.
    simpleSumHash,  // Export simple sum hash.
    polynomialHash,  // Export polynomial hash.
    djb2Hash,  // Export DJB2 hash.
    fnv1aHash,  // Export FNV-1a hash.
    jenkinsOneAtATime,  // Export Jenkins hash.
    analyzeDistribution,  // Export distribution analyzer.
};  // Close exports.

