// 03 雜湊函數示範程式（JavaScript）/ Hash functions demo program (JavaScript).  // Bilingual file header.

"use strict";  // Enable strict mode for safer JavaScript semantics.

const {  // Import hash functions and analysis helper.  
    divisionHash,  // Division method hash.  
    multiplicationHash,  // Multiplication method hash.  
    midSquareHash,  // Mid-square hash.  
    foldingHash,  // Folding hash.  
    polynomialHash,  // Polynomial rolling hash.  
    djb2Hash,  // DJB2 hash.  
    fnv1aHash,  // FNV-1a hash.  
    analyzeDistribution,  // Distribution analyzer.  
} = require("./hashFunctions.js");  // Load module relative to this file.  

const {  // Import universal hash family for a tiny example.  
    UniversalHashFamily,  // Universal hash family for ints.  
} = require("./universalHashing.js");  // Load module relative to this file.  

function main() {  // CLI entry point for the demo.
    const testKey = 123456;  // Pick a deterministic integer key for demo output.
    const mPrime = 97;  // Use a prime bucket count for division hashing.
    console.log("=== Hash Functions Demo (JavaScript) ===");  // Print header.
    console.log(`Key: ${testKey}, m: ${mPrime}`);  // Print key summary.
    console.log(`  Division:       ${divisionHash(testKey, mPrime)}`);  // Print division hash.
    console.log(`  Multiplication: ${multiplicationHash(testKey, mPrime)}`);  // Print multiplication hash.
    console.log(`  Mid-Square (4): ${midSquareHash(testKey, 4)}`);  // Print mid-square hash.
    console.log(`  Folding (3):    ${foldingHash(testKey, 3, mPrime)}`);  // Print folding hash.

    console.log("");  // Blank line.
    console.log("--- String hashes ---");  // Print section title.
    for (const s of ["hello", "world", "hash", "table", "js"]) {  // Print a few hash values for each string.
        const djb2 = djb2Hash(s);  // Compute DJB2 hash.
        const fnv = fnv1aHash(s);  // Compute FNV-1a hash.
        const poly = polynomialHash(s, 31, 1000003);  // Compute polynomial hash mod a prime.
        console.log(`${s} => djb2=${djb2}, fnv1a=${fnv}, poly=${poly}`);  // Print one line.
    }  // Close loop.

    const keys = [];  // Deterministic keys list for distribution demo.
    for (let i = 0; i < 1000; i += 1) {  // Create 1000 keys.
        keys.push(`key_${i}`);  // Append one key string.
    }  // Close loop.
    const report = analyzeDistribution((k, m) => (djb2Hash(k) % m), keys, 100);  // Analyze DJB2 reduced mod 100.
    console.log("");  // Blank line.
    console.log("--- Distribution (DJB2, 1000 keys, m=100) ---");  // Print section title.
    console.log(`non_empty=${report.nonEmptyBuckets}, max=${report.maxBucketSize}, std_dev=${report.stdDeviation.toFixed(3)}`);  // Print summary.

    const uh = new UniversalHashFamily(10, null, 123);  // Universal hash family with deterministic seed.
    console.log("");  // Blank line.
    console.log("--- UniversalHashFamily (m=10) ---");  // Print section title.
    console.log(`hash(42)=${uh.hash(42)}, hash(137)=${uh.hash(137)}`);  // Print example values.
}  // End main().

if (require.main === module) {  // Execute demo only when run directly.
    main();  // Run main.
}  // Close direct-run guard.

