/**
 *                        Min-Heaps
 *
 * Please make sure you read the blurb in `minheap.h` to ensure you understand
 * how we are implementing min-heaps here and what assumptions / requirements
 * are being made.
 *
 * (c) Mustafa Quraish, 2021
 */

#include "minheap.h"

/**
 * Allocate a new min heap of the given size.
 *
 * TODO:
 *  Allocate memory for the `MinHeap` object, and the 2 arrays inside it.
 *  `numItems` should initially be set to 0, and all the indices should be
 *   set to -1 to start with (since we don't have anything in the heap).
 */
MinHeap *newMinHeap(int size) {
    // allocate heap
    MinHeap *heap = malloc(sizeof(MinHeap));

    heap->maxSize = size;
    heap->numItems = 0;
    // allocate space for the arrays
    heap->indices = calloc(size, sizeof(int));
    heap->arr = calloc(size, sizeof(HeapElement));

    // set indices to -1
    for(int i = 0; i < size; i++) heap->indices[i] = -1;

    return heap;
}

/**
 * Swaps elements at indices `a` and `b` in the heap, and also updates their
 * indices. Assumes that `a` and `b` are valid.
 *
 * NOTE: This is already implemented for you, no need to change anything.
 */
void swap(MinHeap *heap, int a, int b) {
    // Swap the elements
    HeapElement temp = heap->arr[a];
    heap->arr[a] = heap->arr[b];
    heap->arr[b] = temp;

    // Refresh their indices
    heap->indices[heap->arr[a].val] = a;
    heap->indices[heap->arr[b].val] = b;
}

/**
 * @brief Get the min child of the HeapElement parent, NULL if does not exist
 *
 * @param heap
 * @param parent
 * @return HeapElement*
 */
HeapElement *get_min(MinHeap *heap, HeapElement *parent) {
    int index = heap->indices[parent->val];

    // if a left child exists
    if (index*2+1 < heap->numItems) {
        HeapElement child1 = heap->arr[index*2+1];

        // if a right child exists return smaller one
        if (index*2+2 < heap->numItems) {
            HeapElement child2 = heap->arr[index*2+2];
            return child1.priority < child2.priority ? heap->arr+index*2+1 : heap->arr+index*2+2;
        }

        return heap->arr+index*2+1;
    }

    // no child
    return NULL;
}

/**
 * @brief Perform heapify on the heap
 *
 * @param heap
 */
void heapify(MinHeap *heap) {
    int smaller_child = 1;
    int index = 0;

    do {
        HeapElement parent = heap->arr[index];
        // get smallest child of parent
        HeapElement *smallest_child = get_min(heap, &parent);

        // if child exists and is smaller in priority swap them and update index
        if (smallest_child != NULL && parent.priority > smallest_child->priority) {
            int val = parent.val;
            swap(heap, heap->indices[val], heap->indices[smallest_child->val]);
            index = heap->indices[val];
        // if child is bigger, element is in right spot so end loop
        } else {
            smaller_child = 0;
        }
    } while(smaller_child && index < heap->numItems);

    return;
}

/**
 * @brief Perform percolate starting at the HeapElement at index
 *
 * @param heap
 * @param index
 */
void percolate(MinHeap *heap, int index) {
    if (index < 1) return;

    int smaller_parent = 1;

    do {
        HeapElement child = heap->arr[index];
        // since index is 0 based need to -1 on even indices after dividing to get parent
        int parent_index = (index % 2 == 0) ? index/2-1 : index/2;
        HeapElement parent = heap->arr[parent_index];

        // if parent is larger priority then swap and update index
        if (parent.priority > child.priority) {
            swap(heap, index, parent_index);
            index = parent_index;
        // if parent is smaller element is in right spot so stop loop
        } else {
            smaller_parent = 0;
        }
    } while(smaller_parent && index > 0);

    return;
}

/**
 * Add a value with the given priority into the heap.
 *
 * TODO: Complete this function, and make sure all the relevant data is updated
 *      correctly, including the `indices` array if you move elements around.
 *      Make sure the heap property is not violated.
 *
 * You may assume the value does not already exist in the heap, and there is
 * enough space in the heap for it.
 */
void heapPush(MinHeap *heap, int val, double priority) {
    // add element
    heap->arr[heap->numItems].val = val;
    heap->arr[heap->numItems].priority = priority;
    heap->indices[val] = heap->numItems;
    heap->numItems += 1;

    // percolate to place in correct spot
    percolate(heap, heap->numItems-1);
    return;
}

/**
 * Extract and return the value from the heap with the minimum priority. Store
 *  the priority for this value in `*priority`.
 *
 * For example, if `10` was the value with the lowest priority of `1.0`, then
 *  you would have something that is equivalent to:
 *
 *        *priority = 1.0;
 *        return 10;
 *
 * TODO: Complete this function, and make sure all the relevant data is updated
 *      correctly, including the `indices` array if you move elements around.
 *      Make sure the heap property is not violated.
 *
 * You may assume there is at least 1 value in the heap.
 */
int heapExtractMin(MinHeap *heap, double *priority) {
    // return priority and get val before discarding
    *priority = heap->arr[0].priority;
    int val = heap->arr[0].val;

    // swap the last element in heap and current min
    swap(heap, 0, heap->numItems-1);
    // decrease numItems to 'discard' min
    heap->numItems -= 1;

    // heapify to fix min heap structure
    heapify(heap);

    // return val
    return val;
}

/**
 * Decrease the priority of the given value (already in the heap) with the
 * new priority.
 *
 * NOTE: You will find it helpful here to first get the index of the value
 *       in the heap from the `indices` array.
 *
 * TODO: Complete this function, and make sure all the relevant data is updated
 *      correctly, including the `indices` array if you move elements around.
 *      Make sure the heap property is not violated.
 *
 * You may assume the value is already in the heap, and the new priority is
 *  smaller than the old one (caller is responsible for ensuring this).
 */
void heapDecreasePriority(MinHeap *heap, int val, double priority) {
    // get index and update priority
    int index = heap->indices[val];
    heap->arr[index].priority = priority;

    // percolate starting at index to fix heap structure
    percolate(heap, index);

    return;
}

/**
 * Free the data for the heap. This won't be marked, but it is always good
 * practice to free up after yourself when using a language like C.
 */
void freeHeap(MinHeap *heap) {
    // free arrays and heap
    free(heap->arr);
    free(heap->indices);
    free(heap);
    return;
}
