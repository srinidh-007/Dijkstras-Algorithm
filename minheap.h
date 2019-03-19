/*
 * minheap.h
 * ---------
 *
 * Programmer: James Gardner
 * University of York
 * Date 24/01/2019
 *
 * The header file for minheap.c, contains some
 * information on how to use each function.
 */

#ifndef MINHEAP_H
#define MINHEAP_H

#include <graph.h>

typedef struct Node Node;

typedef struct MinHeap MinHeap;

/*
 * Function: minHeapConstructor
 * ----------------------------
 * Constructs an empty minHeap and returns
 * a pointer to the new minHeap.
 */
MinHeap* minHeapConstructor();


/*
 * Function: minHeapDestructor
 * ---------------------------
 * Used to free memory used by heap.
 */
void minHeapDestructor(MinHeap* minHeap);


/*
 * Function: minHeapEnqueue
 * ------------------------
 * Add a new Node to the Heap. Need to send in
 * a associated Vertex pointer.
 */
void minHeapEnqueue(MinHeap* minHeap, Vertex* vertex);


/*
 * Function: printHeap
 * -------------------
 * Runs through the heap array printing all values.
 */
void printHeap(MinHeap* minHeap);


/*
 * Function: minHeapGetSize
 * ------------------------
 * Returns the size of the heap.
 */
int minHeapGetSize(MinHeap* minHeap);


/*
 * Function: minHeapIsEmpty
 * ------------------------
 * If size of heap is zero returns true.
 */
int minHeapIsEmpty(MinHeap* minHeap);


/*
 * Function: minHeapDequeue
 * ------------------------
 * Returns pointer to the Vertex associated with the
 * top Node of the Heap. Then removes that Node from
 * the heap.
 */
Vertex* minHeapDequeue(MinHeap* minHeap);


/*
 * Function: minHeapDecreaseNodeValue
 * ----------------------------------
 * Reduces a specific Nodes value and then sifts up
 * till the Heap is correctly sorted.
 */
void minHeapDecreaseNodeValue(MinHeap* minHeap, Vertex* vertex);

#endif
