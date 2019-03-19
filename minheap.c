/*
 * minheap.c
 * ---------
 *
 * Programmer: James Gardner
 * University of York
 * Date 24/01/2019
 *
 * This file contains the functions of the minHeap Abstract
 * Data Type (ADT). The minHeap has a node at its zero index
 * so the equations for child and parent nodes are calculated
 * with that in mind. The Node of the Heap has a close relationship
 * with its corresponding Vertex in the Graph ADT and information
 * about the heap is updated in both structures.
 */

#include <stdio.h>
#include <stdlib.h>
#include <minheap.h>
#include <math.h>

typedef struct Node{

    int value;
    int position;
    int hasChildLeft;
    int hasChildRight;

    // Pointer to Vertex associated with this Node
    // Vertex Structure contains information
    // about the location of Node in heap.
    Vertex* vertex;

} Node;


typedef struct MinHeap{

    // Pointer to Node pointers creating array
    Node** heap;
    // size is the number of Nodes
    int size;

} MinHeap;


// Internal function declarations
Node* nodeConstructor(int position, Vertex* vertex);
void swapNode(MinHeap* minHeap, int currentPosition, int otherPosition);
void minHeapResizeUp(MinHeap* minHeap);
void minHeapResizeDown(MinHeap* minHeap);
void minHeapSiftDown(MinHeap* minHeap);
void nodeDestructor(Node* node);


/*
 * Function: nodeConstructor
 * -------------------------
 * Mallocs space for a Node and returns a pointer
 * to the new Node. Each new node has no child Nodes
 * and is assigned a pointer to it's corresponding
 * Vertex in the graph.
 */
Node* nodeConstructor(int position, Vertex* vertex){

    Node* newNode = malloc(sizeof(Node));

    // This will initially be INT_MAX for all Nodes except source Node
    newNode->value = vertexGetDistanceFromSource(vertex);

    // Position is the index of the node within the heap
    newNode->position = position;

    newNode->hasChildLeft = 0;
    newNode->hasChildRight = 0;
    newNode->vertex = vertex;
    return newNode;
}


/*
 * Function: nodeDestructor
 * ------------------------
 * Frees the memory allocated for a Node.
 */
void nodeDestructor(Node* node){
    free(node);
}


/*
 * Function: minHeapConstructor
 * ----------------------------
 * Constructs an empty minHeap and returns
 * a pointer to the new minHeap.
 */
MinHeap* minHeapConstructor(){

    MinHeap* newMinHeap = malloc(sizeof(MinHeap));
    newMinHeap->size = 0;
    // Has no Nodes at first
    newMinHeap->heap = NULL;
    return newMinHeap;
}


/*
 * Function: minHeapResizeUp
 * -------------------------
 * Increases the size of the heap array by one
 * for adding new Nodes to heap.
 */
void minHeapResizeUp(MinHeap* minHeap){

    minHeap->heap = realloc(minHeap->heap, (minHeap->size + 1)*sizeof(Node*));

    minHeap->heap[minHeap->size] = (Node*)malloc(sizeof(Node));
    minHeap->heap[minHeap->size] = NULL;
}


/*
 * Function: minHeapResizeDown
 * ---------------------------
 * When removing Nodes decreases heap array by one.
 */
void minHeapResizeDown(MinHeap* minHeap){

    nodeDestructor(minHeap->heap[(minHeap->size -1)]);
    minHeap->heap = realloc(minHeap->heap, (minHeap->size - 1)*sizeof(Node*));
}


/*
 * Function: minHeapDestructor
 * ---------------------------
 * Used to free memory used by heap. Frees all
 * Nodes in heap array first then frees minHeap
 * structure.
 */
void minHeapDestructor(MinHeap* minHeap){

    int i;
    for(i = 0; i < minHeap->size; i++){
        nodeDestructor(minHeap->heap[i]);
    }

    free(minHeap);
}


/*
 * Function: swapNode
 * ------------------
 * Swaps all the necessary information between two Nodes in the
 * Heap. Initially creates a temporary pointer to one of the
 * Nodes and two temporary integers to store the state of one
 * of the Nodes children.
 */
void swapNode(MinHeap* minHeap, int currentPosition, int otherPosition){

    Node* tempNode;
    tempNode = minHeap->heap[currentPosition];

    // This is needed as information would otherwise
    // be lost when swapping state of children
    int tempHasChildLeft, tempHasChildRight;
    tempHasChildLeft = tempNode->hasChildLeft;
    tempHasChildRight = tempNode->hasChildRight;

    // Swapping state of child Nodes
    minHeap->heap[currentPosition]->hasChildLeft = minHeap->heap[otherPosition]->hasChildLeft;
    minHeap->heap[currentPosition]->hasChildRight = minHeap->heap[otherPosition]->hasChildRight;
    minHeap->heap[otherPosition]->hasChildLeft = tempHasChildLeft;
    minHeap->heap[otherPosition]->hasChildRight = tempHasChildRight;

    // Swapping actual Node pointers
    minHeap->heap[currentPosition] = minHeap->heap[otherPosition];
    minHeap->heap[otherPosition] = tempNode;

    // Swapping position indices
    minHeap->heap[otherPosition]->position = otherPosition;
    // Updating information of location in Heap for the Vertex in the Graph
    vertexSetPositionInHeap(minHeap->heap[otherPosition]->vertex, otherPosition);

    minHeap->heap[currentPosition]->position = currentPosition;
    vertexSetPositionInHeap(minHeap->heap[currentPosition]->vertex, currentPosition);
}


/*
 * Function: minHeapEnqueue
 * ------------------------
 * Add a new Node to the Heap. Need to send in
 * a Vertex pointer as information associated with
 * the Heap is stored in each Vertex. Increases size
 * of the Heap array. Adds a new Node to end of heap
 * array and then sifts up the Node to the correct
 * position in the heap.
 */
void minHeapEnqueue(MinHeap* minHeap, Vertex* vertex){

    minHeapResizeUp(minHeap);
    // Furthest right point in array gets the new Node
    minHeap->heap[minHeap->size] = nodeConstructor(minHeap->size, vertex);
    // Update the associated Vertex with position in Heap
    vertexSetPositionInHeap(vertex, minHeap->size);
    minHeap->size++;

    int currentPosition;
    int parentPosition;

    currentPosition = minHeap->size - 1;
    // MinHeap has Node at index zero so parent position is floor((childPosition - 1)/2)
    parentPosition = (int)floor(((currentPosition - 1)/2));

    // If new Node is not the top Node
    if(currentPosition > 0){
        // If new Node index is even
        if( currentPosition % 2 == 0){
            // The parent Node now has a child on it's right
            minHeap->heap[parentPosition]->hasChildRight = 1;
        } else {
            // The parent Node now has a child on it's Left
            minHeap->heap[parentPosition]->hasChildLeft = 1;
        }
    }

    // While the new Nodes value is smaller than its parents value
    // swap them around. Sifting up.
    while((minHeap->heap[currentPosition]->value < minHeap->heap[parentPosition]->value)){
        swapNode(minHeap, currentPosition, parentPosition);
        currentPosition = parentPosition;
        parentPosition = (int)floor(((currentPosition - 1)/2));
    }
}


/*
 * Function: minHeapSiftDown
 * -------------------------
 * Checks if top Node is greater in value than it's children
 * and if so swaps with the smallest of it's children. This is
 * repeated until Heap order is satisfied.
 */
void minHeapSiftDown(MinHeap* minHeap){

    int currentPosition;
    int childPositionLeft, childPositionRight;
    int leftValue, rightValue;

    // Current position is top of heap
    currentPosition = 0;

    // Heap has Node at index zero so child positions are:
    childPositionLeft = 2*currentPosition + 1;
    childPositionRight = 2*currentPosition + 2;

    // While current Node has a child on either the left or right
    while(minHeap->heap[currentPosition]->hasChildLeft || minHeap->heap[currentPosition]->hasChildRight){

        // If it has children on both sides
        if(minHeap->heap[currentPosition]->hasChildLeft && minHeap->heap[currentPosition]->hasChildRight){
            // Assign values of children to variables for readability
            leftValue = minHeap->heap[childPositionLeft]->value;
            rightValue = minHeap->heap[childPositionRight]->value;

            // If current Node is greater than or equal in value than either of its children
            if(minHeap->heap[currentPosition]->value >= leftValue
                || minHeap->heap[currentPosition]->value >= rightValue){

                // Checking which of children is smaller
                if((rightValue - leftValue) <= 0) {
                    // Right value is smaller or both equal
                    swapNode(minHeap, currentPosition, childPositionRight);
                    // Update current position variable
                    currentPosition = childPositionRight;

                } else{
                    // Left value is smaller
                    swapNode(minHeap, currentPosition, childPositionLeft);
                    currentPosition = childPositionLeft;
                }
            } else {
                break; // In correct position
            }

        // Only has a child on its left side
        } else if(minHeap->heap[currentPosition]->hasChildLeft){
            // Assign value of child to variable for readability
            leftValue = minHeap->heap[childPositionLeft]->value;

            // If current Node is greater than or equal in value than its child
            if(minHeap->heap[currentPosition]->value >= leftValue){
                swapNode(minHeap, currentPosition, childPositionLeft);
                currentPosition = childPositionLeft;
            } else {
                break; // In correct position
            }

        // Only has a child on its right side
        } else {
            // Assign value of child to variable for readability
            rightValue = minHeap->heap[childPositionRight]->value;

            // If current Node is greater than or equal in value than its child
            if(minHeap->heap[currentPosition]->value >= rightValue){
                swapNode(minHeap, currentPosition, childPositionRight);
                currentPosition = childPositionRight;
            } else {
                break; // In correct position
            }
        }

        // New children positions in heap
        childPositionLeft = 2*currentPosition + 1;
        childPositionRight = 2*currentPosition + 2;
    }
}


/*
 * Function: printHeap
 * -------------------
 * Runs through the heap array printing all values.
 */
void printHeap(MinHeap* minHeap){

    int i;
    for(i = 0; i < minHeap->size; i++){
        printf("%d\n", minHeap->heap[i]->value);
    }
    printf("\n");
}


/*
 * Function: minHeapGetSize
 * ------------------------
 * Returns the size of the heap.
 */
int minHeapGetSize(MinHeap* minHeap){
    return minHeap->size;
}


/*
 * Function: minHeapIsEmpty
 * ------------------------
 * If size of heap is zero returns true.
 */
int minHeapIsEmpty(MinHeap* minHeap){
    if(minHeap->size == 0){
        return 1;
    } else {
        return 0;
    }
}


/*
 * Function: minHeapDequeue
 * ------------------------
 * Returns pointer to the Vertex associated with the
 * top Node of the Heap. Then removes that Node from
 * the heap and moves the furthest right Node to the
 * top. Then sifts down to re-establish heap structure.
 */
Vertex* minHeapDequeue(MinHeap* minHeap){

    Vertex* minVertex;
    Node* tempNode;
    // Top Node on Heap
    tempNode = minHeap->heap[0];
    // Associated Vertex
    minVertex = tempNode->vertex;

    // If there is more than one Node
    if(minHeap->size > 1){

        // Moving most right value to top
        minHeap->heap[0] = minHeap->heap[(minHeap->size - 1)];

        // Changing its position variable value to 0;
        minHeap->heap[0]->position = 0;
        vertexSetPositionInHeap(minHeap->heap[0]->vertex, 0);

        // Updating state of children with previous top Nodes values
        minHeap->heap[0]->hasChildLeft = tempNode->hasChildLeft;
        minHeap->heap[0]->hasChildRight = tempNode->hasChildRight;

        int parentNode;

        // Parent of most right Node
        parentNode = (int)floor(((minHeap->size - 2)/2));

        // Updating state of its children as it has
        // just lost one to the top of the heap
        // If size is even
        if( minHeap->size % 2 == 0){
            // It lost a child on its left
            minHeap->heap[parentNode]->hasChildLeft = 0;
        } else {
            // It lost a child on its right
            minHeap->heap[parentNode]->hasChildRight = 0;
        }


        // Setting most right Node of heap as null pointer
        minHeap->heap[(minHeap->size - 1)] = NULL;
        // Resizing heap
        minHeapResizeDown(minHeap);
        minHeap->size--;
        minHeapSiftDown(minHeap);
        nodeDestructor(tempNode);

    // Only one Node Remaining
    } else {
        minHeap->heap[(minHeap->size - 1)] = NULL;
        minHeapResizeDown(minHeap);
        minHeap->size--;
        nodeDestructor(tempNode);
    }
    return minVertex;
}


/*
 * Function: minHeapDecreaseNodeValue
 * ----------------------------------
 * Reduces a specific Nodes value and then sifts up
 * till the Heap is correctly sorted.
 */
void minHeapDecreaseNodeValue(MinHeap* minHeap, Vertex* vertex){

    int currentPosition;
    int parentPosition;

    currentPosition = vertexGetPositionInHeap(vertex);
    // MinHeap has Node at index zero so parent position is floor((childPosition - 1)/2)
    parentPosition = (int)floor(((currentPosition - 1)/2));

    // Reducing Node value with new distance of associated Vertex
    minHeap->heap[currentPosition]->value = vertexGetDistanceFromSource(vertex);

    // While the new Nodes value is smaller than its parents value
    // swap them around. Sifting up.
    while((minHeap->heap[currentPosition]->value < minHeap->heap[parentPosition]->value)){
        swapNode(minHeap, currentPosition, parentPosition);
        currentPosition = parentPosition;
        parentPosition = (int)floor(((currentPosition - 1)/2));
    }
}





