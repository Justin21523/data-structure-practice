// 02 攤銷分析測試（JavaScript）/ Tests for amortized analysis (JavaScript).  // Bilingual test header.

const test = require("node:test");  // Use Node's built-in test runner (no external deps).  
const assert = require("node:assert/strict");  // Use strict assertions for clear failures.  

const demo = require("./amortizedDemo");  // Import the demo module under test.  

test("potential(0,1) should be 0", () => {  // Validate the chosen Φ offset.  
  assert.equal(demo.potential(0, 1), 0);  // Φ(0,1) == 0 by definition in this unit.  
});  // End test.  

test("capacity is power-of-two and >= size", () => {  // Validate capacity invariant.  
  for (const m of [0, 1, 2, 3, 4, 5, 6, 8, 16, 33, 100]) {  // Mix powers-of-two and non-powers-of-two.  
    const s = demo.simulateAppends(m);  // Run simulation.  
    assert.equal(s.finalSize, m);  // Size must match number of appends performed.  
    assert.equal(demo.isPowerOfTwo(s.finalCapacity), true);  // Capacity must be power of two.  
    assert.ok(s.finalCapacity >= Math.max(1, m));  // Capacity must be sufficient to hold m items.  
  }  // Close loop.  
});  // End test.  

test("total copies equals finalCapacity - 1", () => {  // Validate deterministic copy sum for doubling.  
  for (const m of [0, 1, 2, 3, 4, 5, 6, 8, 9, 16, 31, 32, 33]) {  // Cover boundaries around resizes.  
    const s = demo.simulateAppends(m);  // Run simulation.  
    assert.equal(s.totalCopies, s.finalCapacity - 1);  // Copies sum to 1+2+4+... = cap-1.  
  }  // Close loop.  
});  // End test.  

test("aggregate totalActualCost <= 3m", () => {  // Validate aggregate bound (amortized O(1)).  
  for (const m of [0, 1, 2, 3, 4, 5, 8, 16, 33, 100, 256]) {  // Test several sizes for robustness.  
    const s = demo.simulateAppends(m);  // Run simulation.  
    assert.ok(s.totalActualCost <= 3 * m);  // Total cost grows linearly with m.  
  }  // Close loop.  
});  // End test.  

test("accounting bank never negative", () => {  // Validate accounting method invariant.  
  const table = new demo.AmortizedDynamicArray();  // Create a fresh table.  
  for (let i = 0; i < 256; i += 1) {  // Run enough operations to hit multiple resizes.  
    const step = table.append(i);  // Append one value and capture step record.  
    assert.ok(step.bankAfter >= 0);  // Bank should never be negative.  
  }  // Close loop.  
});  // End test.  

test("potential method amortized cost is 3 each step", () => {  // Validate potential-method per-step amortized cost.  
  const table = new demo.AmortizedDynamicArray();  // Create a fresh table.  
  for (let i = 0; i < 256; i += 1) {  // Run enough operations to include many resizes.  
    const step = table.append(i);  // Append one value.  
    assert.equal(step.amortizedCost, 3);  // Amortized cost should be constant (3).  
  }  // Close loop.  
});  // End test.  

