/* 01 漸進符號示範測試（Node.js）/ Tests for asymptotic demo (Node.js). */  // Bilingual test file header.
'use strict';  // Enable strict mode for consistent runtime behavior.

const test = require('node:test');  // Use Node's built-in test runner (no external dependencies).  
const assert = require('node:assert/strict');  // Use strict assertions for clearer failures.  

const {  // Import the demo functions under test from the local module.
  countConstantOps,  // O(1) counter under test.
  countLog2Ops,  // O(log n) counter under test.
  countLinearOps,  // O(n) counter under test.
  countNLog2NOps,  // O(n log n) counter under test.
  countQuadraticOps,  // O(n^2) counter under test.
} = require('./asymptoticDemo');  // Load the demo module from the same folder.

test('countConstantOps should be constant', () => {  // Verify the O(1) demo ignores input size.
  assert.equal(countConstantOps(0), countConstantOps(10));  // Same output for different inputs.
  assert.equal(countConstantOps(1), 3);  // This demo uses exactly 3 simulated operations.
});  // End test case.

test('countLog2Ops should match halving count', () => {  // Verify the O(log n) demo returns floor(log2(n)).
  assert.equal(countLog2Ops(1), 0);  // No halving needed when n == 1.
  assert.equal(countLog2Ops(2), 1);  // 2 -> 1 uses one halving.
  assert.equal(countLog2Ops(8), 3);  // 8 -> 4 -> 2 -> 1 uses three halvings.
});  // End test case.

test('countLog2Ops should reject non-positive', () => {  // Verify invalid inputs throw for log-based demo.
  assert.throws(() => countLog2Ops(0), /n must be >= 1/);  // log2(0) is not accepted in this discrete demo.
  assert.throws(() => countLog2Ops(-1), /n must be >= 1/);  // Negative sizes are invalid.
});  // End test case.

test('countLinearOps should equal n', () => {  // Verify the O(n) demo returns exactly n operations.
  assert.equal(countLinearOps(0), 0);  // Zero-sized input performs zero operations.
  assert.equal(countLinearOps(5), 5);  // One operation per iteration for n iterations.
});  // End test case.

test('countQuadraticOps should equal n^2', () => {  // Verify the O(n^2) demo returns n squared operations.
  assert.equal(countQuadraticOps(0), 0);  // 0 * 0 iterations.
  assert.equal(countQuadraticOps(4), 16);  // 4 * 4 iterations.
});  // End test case.

test('countNLog2NOps should equal n * floor(log2(n)) for powers of two', () => {  // Verify the intended n log n growth model.
  assert.equal(countNLog2NOps(0), 0);  // Define 0 * log(0) as 0 work for this demo.
  assert.equal(countNLog2NOps(8), 24);  // 8 * log2(8) = 8 * 3 = 24 operations.
});  // End test case.

