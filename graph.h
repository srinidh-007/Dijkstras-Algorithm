/*
 * graph.h
 * -------
 *
 * Programmer: James Gardner
 * University of York
 * Date 24/01/2019
 *
 * The header file for graph.c, contains some
 * information on how to use each function.
 */

#ifndef GRAPH_H
#define GRAPH_H

typedef struct Vertex Vertex;

typedef struct Edge Edge;

typedef struct Graph Graph;

/*
 * Function: vertexConstructor
 * ---------------------------
 * Constructs a Vertex using a provided number
 * and city name and vertexNumber. Returns a pointer.
 */
Vertex* vertexConstructor(int vertexNumber, char* cityName);


/*
 * Function: vertexGetDistanceFromSource
 * -------------------------------------
 * Returns the value of distanceFromSource.
 */
int vertexGetDistanceFromSource(Vertex* vertex);


/*
 * Function: vertexGetVertexNumber
 * -------------------------------
 * Returns the associated vertex number.
 */
int vertexGetVertexNumber(Vertex* vertex);


/*
 * Function: vertexGetCityName
 * ---------------------------
 * Returns the char* cityName variable.
 */
char* vertexGetCityName(Vertex* vertex);


/*
 * Function: vertexGetPreviousCityName
 * -----------------------------------
 * Returns a Vertex's previous cityName
 */
char* vertexGetPreviousCityName(Vertex* vertex);


/*
 * Function: vertexGetPrevious
 * ---------------------------
 * Returns a pointer to the previous Vertex
 */
Vertex* vertexGetPrevious(Vertex* vertex);


/*
 * Function: vertexIsVisited
 * -------------------------
 * Returns the value of visited, used
 * like a boolean true or false.
 */
int vertexIsVisited(Vertex* vertex);


/*
 * Function: vertexSetIsVisited
 * ----------------------------
 * Used to set a vertex as visited.
 */
void vertexSetIsVisited(Vertex* vertex);


/*
 * Function: vertexSetIsNotVisited
 * -------------------------------
 * Used to set a vertex as not visited.
 */
void vertexSetIsNotVisited(Vertex* vertex);


/*
 * Function: vertexSetPositionInHeap
 * ---------------------------------
 * Send a integer value to assign position in heap.
 */
void vertexSetPositionInHeap(Vertex* vertex, int position);


/*
 * Function: vertexGetPositionInHeap
 * ---------------------------------
 * Returns index of associated node in heap.
 */
int vertexGetPositionInHeap(Vertex* vertex);


/*
 * Function: vertexSetDistanceFromSource
 * -------------------------------------
 * Distance from the chosen source Vertex.
 */
void vertexSetDistanceFromSource(Vertex* vertex, int distance);


/*
 * Function: vertexSetPrevious
 * ---------------------------
 * Setting the pointer to the previous Vertex.
 * Must send to Vertex Pointers into function.
 */
void vertexSetPrevious(Vertex* currentVertex, Vertex* previousVertex);


/*
 * Function: vertexGetNumberOfEdges
 * --------------------------------
 * Returns the number of edges on a Vertex.
 */
int vertexGetNumberOfEdges(Vertex* vertex);


/*
 * Function: vertexGetEdge
 * -----------------------
 * Returns a pointer to a specific Edges within
 * a Vertex's edges list.
 */
Edge* vertexGetEdge(Vertex* vertex, int edgeNumber);


/*
 * Function: edgeGetEndVertex
 * --------------------------
 * Returns a pointer to the end Vertex of an Edge.
 */
Vertex* edgeGetEndVertex(Edge* edge);


/*
 * Function: edgeGetDistance
 * -------------------------
 * Returns the distance value associated with an Edge.
 */
int edgeGetDistance(Edge* edge);


/*
 * Function: graphConstructor
 * --------------------------
 * Creates an empty Graph structure and returns a pointer to it.
 */
Graph* graphConstructor();


/*
 * Function: graphDestructor
 * -------------------------
 * Frees dynamically allocated memory of Graph.
 */
void graphDestructor(Graph* graph);


/*
 * Function: graphPopulateGraph
 * ----------------------------
 * Must send in a tab-delimited file that contains
 * two tab-delimited strings followed by a tab-delimited
 * integer and each line terminating in \r then \n.
 * Will fill graph with vertices and edges.
 */
void graphPopulateGraph(Graph* graph, FILE* ukcities);


/*
 * Function: printAdjList
 * ----------------------
 * Prints adjList of graph.
 */
void printAdjList(Graph* graph);


/*
 * Function: graphGetNumberOfCities
 * --------------------------------
 * Returns the number of unique cities or Vertices.
 */
int graphGetNumberOfCities(Graph* graph);


/*
 * Function: graphGetVertex
 * ------------------------
 * Returns a pointer to a Vertex of a specific
 * vertexNumber.
 */
Vertex* graphGetVertex(Graph* graph, int vertexNumber);


/*
 * Function: graphGetVertexNumber
 * ------------------------------
 * Returns it's index if known
 * and a -1 if not found.
 */
int graphGetVertexNumber(Graph* graph, char* string);

#endif
