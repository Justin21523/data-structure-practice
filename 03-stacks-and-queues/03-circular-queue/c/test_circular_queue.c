// 03 環狀佇列測試（C）/ Tests for circular queue (C).  // Bilingual file header.

#include "circular_queue.h"  // Include the API under test.

#include <stdio.h>  // Provide printf for status output.
#include <stdlib.h>  // Provide exit for early termination on failure.

static void assert_true(bool condition, const char* message) {  // Minimal assertion helper.
    if (!condition) {  // Fail when condition is false.
        printf("FAIL: %s\n", message);  // Print message.
        exit(1);  // Exit with failure.
    }  // Close failure branch.
    printf("PASS: %s\n", message);  // Print pass status.
}  // Close assert_true().

static void test_fifo_enqueue_dequeue_and_peek(void) {  // Queue should obey FIFO semantics.
    circular_queue q;  // Create queue struct.
    assert_true(circular_queue_init(&q), "init should succeed");  // Initialize queue.

    queue_cost c;  // Cost record for enqueues.
    assert_true(circular_queue_enqueue(&q, 10, &c), "enqueue(10) should succeed");  // Enqueue 10.
    assert_true(circular_queue_enqueue(&q, 20, &c), "enqueue(20) should succeed");  // Enqueue 20.
    assert_true(circular_queue_enqueue(&q, 30, &c), "enqueue(30) should succeed");  // Enqueue 30.

    int front = 0;  // Store peeked value.
    assert_true(circular_queue_peek(&q, &front), "peek should succeed");  // Peek front.
    assert_true(front == 10, "peek should return first enqueued value");  // Validate peek value.

    queue_dequeue_result r;  // Dequeue result.
    assert_true(circular_queue_dequeue(&q, &r), "dequeue should succeed");  // Dequeue first.
    assert_true(r.value == 10, "first dequeue should return 10");  // Validate first dequeue.
    assert_true(r.cost.moved == 0, "dequeue moved should be 0");  // Validate moved=0.
    assert_true(circular_queue_dequeue(&q, &r), "dequeue should succeed");  // Dequeue second.
    assert_true(r.value == 20, "second dequeue should return 20");  // Validate second dequeue.
    assert_true(circular_queue_dequeue(&q, &r), "dequeue should succeed");  // Dequeue third.
    assert_true(r.value == 30, "third dequeue should return 30");  // Validate third dequeue.
    assert_true(q.size == 0, "queue should be empty after removing all");  // Validate emptiness.

    circular_queue_destroy(&q);  // Free resources.
}  // Close test_fifo_enqueue_dequeue_and_peek().

static void test_enqueue_resize_copied_counts(void) {  // copied should count resize copies exactly.
    circular_queue q;  // Create queue struct.
    assert_true(circular_queue_init(&q), "init should succeed");  // Initialize queue.

    queue_cost c;  // Cost record.
    assert_true(circular_queue_enqueue(&q, 10, &c), "enqueue should succeed");  // Enqueue 10.
    assert_true(c.copied == 0, "first enqueue fits without resize");  // Validate copied=0.
    assert_true(circular_queue_enqueue(&q, 20, &c), "enqueue should succeed");  // Enqueue 20.
    assert_true(c.copied == 1, "resize 1->2 copies 1 element");  // Validate copied=1.
    assert_true(circular_queue_enqueue(&q, 30, &c), "enqueue should succeed");  // Enqueue 30.
    assert_true(c.copied == 2, "resize 2->4 copies 2 elements");  // Validate copied=2.
    assert_true(circular_queue_enqueue(&q, 40, &c), "enqueue should succeed");  // Enqueue 40.
    assert_true(c.copied == 0, "enqueue fits in capacity 4");  // Validate copied=0.
    assert_true(circular_queue_enqueue(&q, 50, &c), "enqueue should succeed");  // Enqueue 50.
    assert_true(c.copied == 4, "resize 4->8 copies 4 elements");  // Validate copied=4.

    assert_true(q.size == 5, "final size should be 5");  // Validate final size.
    assert_true(q.capacity == 8, "final capacity should be 8");  // Validate final capacity.
    assert_true(q.total_copies == 1 + 2 + 4, "total copies should sum resizes");  // Validate total copies.

    int out[8];  // Output buffer for snapshot.
    assert_true(circular_queue_to_array(&q, out, 8), "to_array should succeed");  // Copy snapshot.
    assert_true(out[0] == 10 && out[4] == 50, "snapshot should preserve FIFO order");  // Validate snapshot endpoints.

    circular_queue_destroy(&q);  // Free resources.
}  // Close test_enqueue_resize_copied_counts().

static void test_dequeue_moved_cost_is_zero(void) {  // Dequeue should not shift elements in circular queue.
    int ns[] = {1, 2, 3, 5, 10};  // Multiple sizes.
    int ns_len = (int)(sizeof(ns) / sizeof(ns[0]));  // Length of ns.
    for (int k = 0; k < ns_len; k++) {  // Iterate cases.
        int n = ns[k];  // Read size.
        circular_queue q;  // Create queue struct.
        assert_true(circular_queue_init(&q), "init should succeed");  // Initialize queue.
        queue_cost c;  // Reusable cost record for enqueues.
        for (int i = 0; i < n; i++) {  // Enqueue 0..n-1.
            assert_true(circular_queue_enqueue(&q, i, &c), "enqueue should succeed");  // Enqueue one value.
        }  // Close enqueue loop.

        queue_dequeue_result r;  // Dequeue result.
        assert_true(circular_queue_dequeue(&q, &r), "dequeue should succeed");  // Dequeue once.
        assert_true(r.value == 0, "dequeued value should be 0");  // Validate removed value.
        assert_true(r.cost.moved == 0, "dequeue moved should be 0");  // Validate moved=0.

        int out[16];  // Output buffer for snapshot (n <= 10 here).
        assert_true(circular_queue_to_array(&q, out, 16), "to_array should succeed");  // Copy snapshot.
        if (n > 1) {  // Validate remaining order when elements remain.
            assert_true(out[0] == 1 && out[n - 2] == n - 1, "after dequeue, queue should be 1..n-1");  // Validate endpoints.
        }  // Close remaining-elements branch.

        circular_queue_destroy(&q);  // Free resources.
    }  // Close loop.
}  // Close test_dequeue_moved_cost_is_zero().

static void test_wrap_around_works(void) {  // Enqueue should wrap around when head is not 0.
    circular_queue q;  // Create queue struct.
    assert_true(circular_queue_init(&q), "init should succeed");  // Initialize queue.
    queue_cost c;  // Reusable cost record for enqueues.
    for (int v = 0; v < 4; v++) {  // Enqueue 0..3 (capacity should become 4).
        assert_true(circular_queue_enqueue(&q, v, &c), "enqueue should succeed");  // Enqueue one value.
    }  // Close loop.
    queue_dequeue_result r;  // Dequeue result.
    assert_true(circular_queue_dequeue(&q, &r), "dequeue should succeed");  // Dequeue 0.
    assert_true(r.value == 0, "dequeue should return 0");  // Validate.
    assert_true(circular_queue_dequeue(&q, &r), "dequeue should succeed");  // Dequeue 1.
    assert_true(r.value == 1, "dequeue should return 1");  // Validate.
    assert_true(circular_queue_enqueue(&q, 4, &c), "enqueue should succeed");  // Enqueue 4 (wrap-around).
    assert_true(circular_queue_enqueue(&q, 5, &c), "enqueue should succeed");  // Enqueue 5 (wrap-around).
    int out[8];  // Output buffer for snapshot.
    assert_true(circular_queue_to_array(&q, out, 8), "to_array should succeed");  // Snapshot queue.
    assert_true(out[0] == 2 && out[3] == 5, "wrap-around ordering should be [2,3,4,5]");  // Validate endpoints.
    circular_queue_destroy(&q);  // Free resources.
}  // Close test_wrap_around_works().

static void test_resize_when_head_not_zero_preserves_order(void) {  // Resize should preserve order even when head is offset.
    circular_queue q;  // Create queue struct.
    assert_true(circular_queue_init(&q), "init should succeed");  // Initialize queue.
    queue_cost c;  // Reusable cost record for enqueues.
    for (int v = 0; v < 4; v++) {  // Enqueue 0..3 (capacity should become 4).
        assert_true(circular_queue_enqueue(&q, v, &c), "enqueue should succeed");  // Enqueue one value.
    }  // Close loop.
    queue_dequeue_result r;  // Dequeue result.
    circular_queue_dequeue(&q, &r);  // Dequeue 0.
    circular_queue_dequeue(&q, &r);  // Dequeue 1 (head offset).
    assert_true(circular_queue_enqueue(&q, 4, &c), "enqueue should succeed");  // Enqueue 4.
    assert_true(circular_queue_enqueue(&q, 5, &c), "enqueue should succeed");  // Enqueue 5 (queue full).
    assert_true(circular_queue_enqueue(&q, 6, &c), "enqueue should succeed");  // Enqueue 6 (triggers resize).
    assert_true(c.copied == 4, "resize should copy exactly size elements (4)");  // Validate copied count.
    int out[16];  // Output buffer for snapshot.
    assert_true(circular_queue_to_array(&q, out, 16), "to_array should succeed");  // Snapshot queue.
    assert_true(out[0] == 2 && out[4] == 6, "order should be preserved after resize");  // Validate endpoints.
    circular_queue_destroy(&q);  // Free resources.
}  // Close test_resize_when_head_not_zero_preserves_order().

static void test_empty_operations_fail(void) {  // dequeue/peek should reject empty queue.
    circular_queue q;  // Create queue struct.
    assert_true(circular_queue_init(&q), "init should succeed");  // Initialize queue.

    int front = 0;  // Placeholder for peek value.
    assert_true(!circular_queue_peek(&q, &front), "peek should fail on empty");  // Invalid peek.
    queue_dequeue_result r;  // Placeholder for dequeue result.
    assert_true(!circular_queue_dequeue(&q, &r), "dequeue should fail on empty");  // Invalid dequeue.

    circular_queue_destroy(&q);  // Free resources.
}  // Close test_empty_operations_fail().

int main(void) {  // Run all tests and print status.
    printf("=== CircularQueue Tests (C) ===\n");  // Print header.
    test_fifo_enqueue_dequeue_and_peek();  // Run FIFO tests.
    test_enqueue_resize_copied_counts();  // Run resize copy tests.
    test_dequeue_moved_cost_is_zero();  // Run moved=0 tests.
    test_wrap_around_works();  // Run wrap-around test.
    test_resize_when_head_not_zero_preserves_order();  // Run resize-with-offset-head test.
    test_empty_operations_fail();  // Run empty-operation tests.
    printf("================================\n");  // Print footer separator.
    printf("All tests PASSED.\n");  // Print success message.
    return 0;  // Exit success.
}  // Close main().

