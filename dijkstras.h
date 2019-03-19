/*
 * dijkstras.h
 * -----------
 *
 * Programmer: James Gardner
 * University of York
 * Date 24/01/2019
 *
 * The header file for dijkstras.c, contains some
 * information on how to use each function.
 */

#ifndef DIJKSTRAS_H
#define DIJKSTRAS_H

#include <minheap.h>
#include <graph.h>

/*
 * Function: dijkstras
 * -------------------
 * Calculates shortest path from source to all other Vertices
 * in the Graph.
 */
void dijkstras(Graph* graph, MinHeap* minHeap, int source);


/*
 * Function: dijkstrasPrintResult
 * ------------------------------
 * Prints out all distances and previous city names.
 */
void dijkstrasPrintResult(Graph* graph);


/*
 * Function: dijkstrasWriteToFile
 * ------------------------------
 * Writes a specific shortest path from source to a
 * destination city and route taken to get there to
 * a file.
 */
void dijkstrasWriteToFile(Graph* graph, FILE* output, int sourceVertexNumber, int destinationVertexNumber);

#endif
