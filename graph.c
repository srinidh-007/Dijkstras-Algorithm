/*
 * graph.c
 * -------
 *
 * Programmer: James Gardner
 * University of York
 * Date 26/01/2019
 *
 * This file contains the functions for the Graph
 * abstract data type. Here the Graph structure contains
 * a list of pointers to it's Vertices and each Vertex structure
 * contains a list of pointers to its Edges and each Edge has
 * pointers to it's start and Vertices. This forms an Adjacency
 * List that stores the information of the Graph. The Vertex
 * structure also contains some information needed to speed up
 * Dijkstra's algorithms such as it's location within the minHeap
 * ADT. A lot of the functions within this file are getters
 * and setters necessary to access the information contained within
 * the Structs in different files.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <graph.h>

#define STRING_SIZE 250 // Maximum length of a string

typedef struct Vertex{

    // All Vertices have a vertexNumber as
    // easier to compare and navigate than using
    // char* cityName
    int vertexNumber;
    int numberOfEdges;
    char* cityName;

    // Pointer to Edge pointers
    // Forms an adjacency list of this Vertex
    Edge** edges;


    // Following variables needed for
    // improving efficiency of Dijkstra's
    // algorithm
    Vertex* previous; // Previous city on route to destination
    int distanceFromSource;
    int positionInHeap;
    int visited; // Has vertex been visited by Dijkstra's algorithm

} Vertex;


typedef struct Edge{

    Vertex* start;
    Vertex* end;
    int distance;

} Edge;


typedef struct Graph{

    int numberOfCities;
    Vertex** adjLists; // Pointer to Vertex pointers

} Graph;


// Internal function declarations
void addEdge(Graph* graph, int start, int end, int distance);
void checkStringsKnown(Graph* graph, char* stringA, char* stringB, int* vertexNumberA, int* vertexNumberB);
void vertexEdgesResize(Vertex* vertex);
void vertexDestructor(Vertex* vertex);
void graphVertexResize(Graph* graph, int i, char* string);
Edge* edgeConstructor(Graph* graph, int distance, int start, int end);
void edgeDestructor();


/*
 * Function: vertexConstructor
 * ---------------------------
 * Constructs a Vertex using a provided number
 * and city name. Initially assumes Vertex has no
 * edges and these are added later. Distance from
 * source is initially set to INT_MAX as this is
 * needed in Dijkstra's algorithm. Malloced memory
 * is freed in vertexDestructor().
 */
Vertex* vertexConstructor(int vertexNumber, char* cityName){

    Vertex* newVertex = malloc(sizeof(Vertex));

    newVertex->vertexNumber = vertexNumber;

    // Needs the +1 for the null-terminator
    newVertex->cityName = malloc(strlen(cityName)+1);
    strcpy(newVertex->cityName, cityName);

    newVertex->numberOfEdges = 0;
    // Edges not sized initially so just set to NULL.
    // Will be resized in vertexEdgesResize().
    newVertex->edges = NULL;

    // Position in heap defaults to -1 as not yet
    // being used in a heap.
    newVertex->positionInHeap = -1;
    newVertex->previous = NULL;
    // INT_MAX Requires limits.h, sets an
    // integer to its maximum possible value.
    newVertex->distanceFromSource = INT_MAX;
    newVertex->visited = 0; // Not visited

    return newVertex;
}


/*
 * Function: vertexEdgesResize
 * ---------------------------
 * Used to resize the edges variable in a Vertex
 * as new edges are added.
 */
void vertexEdgesResize(Vertex* vertex){
    // Increase the size of edges by one.
    vertex->edges = realloc(vertex->edges, (vertex->numberOfEdges + 1)*sizeof(Edge*));

    // Each edge needs memory allocated. Pointer returned and stores in
    // Furthest right index, (numberOfEdges), of edges array.
    vertex->edges[vertex->numberOfEdges] = malloc(sizeof(Edge));
    vertex->edges[vertex->numberOfEdges] = NULL;
}


/*
 * Function: vertexDestructor
 * --------------------------
 * Frees the memory used for a Vertex.
 */
void vertexDestructor(Vertex* vertex){

    int i;

    // Each edge in edges needs freeing first
    for (i = 0; i < vertex->numberOfEdges; i++){
        edgeDestructor(vertex->edges[i]);
    }

    free(vertex->cityName);
    free(vertex);
}


/*
 * Function: vertexGetDistanceFromSource
 * -------------------------------------
 * Returns the value of distanceFromSource.
 */
int vertexGetDistanceFromSource(Vertex* vertex){
    return vertex->distanceFromSource;
}


/*
 * Function: vertexGetVertexNumber
 * -------------------------------
 * Returns the associated vertex number.
 */
int vertexGetVertexNumber(Vertex* vertex){
    return vertex->vertexNumber;
}


/*
 * Function: vertexGetCityName
 * ---------------------------
 * Returns the char* cityName variable.
 */
char* vertexGetCityName(Vertex* vertex){
    return vertex->cityName;
}


/*
 * Function: vertexGetPreviousCityName
 * -----------------------------------
 * Points through to previous Vertex and
 * returns it's cityName variable.
 */
char* vertexGetPreviousCityName(Vertex* vertex){
    return vertex->previous->cityName;
}


/*
 * Function: vertexGetPrevious
 * ---------------------------
 * Returns a pointer to the previous Vertex
 */
Vertex* vertexGetPrevious(Vertex* vertex){
    return vertex->previous;
}


/*
 * Function: vertexIsVisited
 * -------------------------
 * Returns the value of visited, used
 * like a boolean true or false.
 */
int vertexIsVisited(Vertex* vertex){
    return vertex->visited;
}


/*
 * Function: vertexSetIsVisited
 * ----------------------------
 * Used to set a vertex as visited.
 */
void vertexSetIsVisited(Vertex* vertex){
    vertex->visited = 1;
}


/*
 * Function: vertexSetIsNotVisited
 * -------------------------------
 * For resetting the Vertex to not visited. This
 * is used at the start of each Dijkstra's run through.
 */
void vertexSetIsNotVisited(Vertex* vertex){
    vertex->visited = 0;
}



/*
 * Function: vertexSetPositionInHeap
 * ---------------------------------
 * Setting the associated node in heap index. This is needed
 * to speed up Dijkstra's algorithm as no need to search the heap.
 */
void vertexSetPositionInHeap(Vertex* vertex, int position){
    vertex->positionInHeap = position;
}


/*
 * Function: vertexGetPositionInHeap
 * ---------------------------------
 * Returns index of associated node in heap.
 */
int vertexGetPositionInHeap(Vertex* vertex){
    return vertex->positionInHeap;
}


/*
 * Function: vertexSetDistanceFromSource
 * -------------------------------------
 * Distance from the chosen source Vertex.
 */
void vertexSetDistanceFromSource(Vertex* vertex, int distance){
    vertex->distanceFromSource = distance;
}


/*
 * Function: vertexSetPrevious
 * ---------------------------
 * Setting the pointer to the previous Vertex. This
 * is used to plot a route from destination to source
 * vertex to find the shortest path.
 */
void vertexSetPrevious(Vertex* currentVertex, Vertex* previousVertex){
    currentVertex->previous = previousVertex;
}


/*
 * Function: vertexGetNumberOfEdges
 * --------------------------------
 * Returns the number of edges on a Vertex.
 */
int vertexGetNumberOfEdges(Vertex* vertex){
    return vertex->numberOfEdges;
}


/*
 * Function: vertexGetEdge
 * -----------------------
 * Returns a pointer to a specific Edges within
 * a Vertex's edges list.
 */
Edge* vertexGetEdge(Vertex* vertex, int edgeNumber){
    return vertex->edges[edgeNumber];
}


/*
 * Function: edgeConstructor
 * -------------------------
 * Constructs and mallocs an Edge and returns a pointer to it.
 * The start and end integers are the Vertex's the edge will point too.
 */
Edge* edgeConstructor(Graph* graph, int distance, int start, int end){

    Edge* newEdge = malloc(sizeof(Edge));

    newEdge->distance = distance;
    // Pointers to Vertices in the overall graph structure.
    newEdge->start = graph->adjLists[start];
    newEdge->end = graph->adjLists[end];

    return newEdge;
}


/*
 * Function: edgeDestructor
 * ------------------------
 * Frees the dynamically allocated memory.
 */
void edgeDestructor(Edge* edge){

    free(edge);
}


/*
 * Function: edgeGetEndVertex
 * --------------------------
 * Returns a pointer to the end Vertex of an Edge.
 */
Vertex* edgeGetEndVertex(Edge* edge){
    return edge->end;
}


/*
 * Function: edgeGetDistance
 * -------------------------
 * Returns the distance value associated with an Edge.
 */
int edgeGetDistance(Edge* edge){
    return edge->distance;
}


/*
 * Function: graphConstructor
 * --------------------------
 * Creates a Graph structure and returns a pointer
 * to that structure. Builds a graph using a StringArray
 * structure. Creates an adjacency list of Vertex pointers
 * big enough to fit the number of stings in the StringArray.
 * Then calls to vertexConstructor mapping each vertex number
 * and  city name with it's corresponding index in the StringArray.
 */
Graph* graphConstructor(){

    Graph* newGraph = malloc(sizeof(Graph));
    newGraph->numberOfCities = 0;
    newGraph->adjLists = NULL;
    return newGraph;
}


/*
 * Function: graphVertexResize
 * ---------------------------
 * Reallocs the size of the adjLists to
 * enable the adding of a single new Vertex. Then
 * constructs a vertex using an index and a
 * cityName string.
 */
void graphVertexResize(Graph* graph, int i, char* string){

    // Increase size of adjLists
    graph->adjLists = realloc(graph->adjLists, (graph->numberOfCities + 1)*sizeof(Vertex*));
    graph->adjLists[graph->numberOfCities] = malloc(sizeof(Vertex*));

    // Construct new Vertex and increase the number of cities
    graph->adjLists[graph->numberOfCities] = vertexConstructor(i, string);
    graph->numberOfCities++;
}


/*
 * Function: graphDestructor
 * -------------------------
 * Frees dynamically allocated memory of Graph.
 * Frees all Vertices first and then the Graph.
 */
void graphDestructor(Graph* graph){

    int i;
    for (i = 0; i < graph->numberOfCities; i++){
        vertexDestructor(graph->adjLists[i]);
    }

    free(graph);
}


/*
 * Function: addEdge
 * -----------------
 * Creates an Edge from start index to end index and adds
 * it to the Vertex with vertexNumber start. Then creates an
 * Edge from end index to start index and adds it to the
 * Vertex with vertexNumber end.
 */
void addEdge(Graph* graph, int start, int end, int distance){

    // Edge from start to end
    Edge* edge = edgeConstructor(graph, distance, start, end);

    // Resize the size of the Edges array in the start Vertex
    vertexEdgesResize(graph->adjLists[start]);

    // The Vertex at graph->adjList[start], gets the new Edge added to the end-of (number-of-edges) it's list of Edges
    graph->adjLists[start]->edges[graph->adjLists[start]->numberOfEdges] = edge;
    // That Vertex now has one more Edge
    graph->adjLists[start]->numberOfEdges++;


    // Create the reverse Edge
    edge = edgeConstructor(graph, distance, end, start);

    // Resize Edges array in end Vertex
    vertexEdgesResize(graph->adjLists[end]);
    // Add Edge to edges array
    graph->adjLists[end]->edges[graph->adjLists[end]->numberOfEdges] = edge;
    // Increase numberOfEdges
    graph->adjLists[end]->numberOfEdges++;
}


/*
 * Function: checkStringsKnown
 * ---------------------------
 * Loops through the graph vertices to check for
 * a new, unknown string. If the string is
 * unknown the adjLists is realloced and
 * the new string added. The vertex number of
 * the city is returned via int pointers
 */
void checkStringsKnown(Graph* graph, char* stringA, char* stringB, int* vertexNumberA, int* vertexNumberB){

    // Used as bools check if city has been found
    int stringAKnown = 0;
    int stringBKnown = 0;

    int k = 0;
    for (k = 0; k < graph->numberOfCities; k++){
        // loop through array and compare known with stringA
        if(!strcmp(graph->adjLists[k]->cityName, stringA)) {
            *vertexNumberA = k;
            stringAKnown = 1;
        }
        // loop through array and compare known with stringB
        if(!strcmp(graph->adjLists[k]->cityName, stringB)) {
            *vertexNumberB = k;
            stringBKnown = 1;
        }
    }
    if (!(stringAKnown && stringBKnown)){
         // If string not known add it to array
         if (!stringAKnown){
                *vertexNumberA = graph->numberOfCities;
                graphVertexResize(graph, graph->numberOfCities, stringA);
         }
         if(!stringBKnown){
                *vertexNumberB = graph->numberOfCities;
                graphVertexResize(graph, graph->numberOfCities, stringB);
         }
    }
}


/*
 * Function: graphPopulateGraph
 * ----------------------------
 * Runs through the ukcities file assigning information
 * to the temporary variables. Checking if the city is known
 * and if not creating a new vertex to store the city.
 * Makes calls to addEdge to connect all the Vertices. This
 * completes the Graph structure.
 */
void graphPopulateGraph(Graph* graph, FILE* ukcities){

    // Temporary variables for cities and distances imported
    char* start = malloc(STRING_SIZE*sizeof(char));
    char* end = malloc(STRING_SIZE*sizeof(char));
    int distance;
    int vertexNumberStart = 0;
    int vertexNumberEnd = 0;

    // Counting the number of lines
    // Used for error checking
    char character;
    int lineNumber = 0;
    while(!feof(ukcities)){
        character = fgetc(ukcities);
        if(character == '\n'){
            lineNumber++;
        }
    }

    // Sets the file position to the beginning of the file
    rewind(ukcities);


    //  %[^\t] - Read any character that is not a TAB
    //  \t - The TAB character
    //  %d - The integer
    //  \r - The return carriage character
    //  \n - The new line character
    // Each line of the file is terminated with both a \r and a \n character
    // While fscanf has read three things, two strings and an integer on a line
    int i = 0;
    while(3 == fscanf(ukcities, "%[^\t]\t%[^\t]\t%d\r\n", start, end, &distance)){

        checkStringsKnown(graph, start, end, &vertexNumberStart, &vertexNumberEnd);

        if(distance <= 0){
            printf("#################################################\n");
            printf("Distance is equal too or less than zero\n");
            printf("Please check distance between %s and %s\n", start, end);
            printf("#################################################\n\n");
            exit(-1);
        }

        addEdge(graph, vertexNumberStart, vertexNumberEnd, distance);

        // i is counting the number of lines read
        // used for error checking
        i++;
    }


    // If the number of lines read by the
    // file when importing data isn't equal
    // to the number of lines in the file
    // report an error.
    if(i != lineNumber){
        printf("\n######################################\n");
        printf("There was an error reading 'ukcities.txt' on line: %d\n\n", i);
        printf("Please check each line of the file contains two,\n");
        printf("tab-delimited, strings followed by an integer\n");
        printf("and try again.\n");
        printf("######################################\n\n");
        exit(-1);
    }

    free(start);
    free(end);
}


/*
 * Function: printAdjList
 * ----------------------
 * Runs through each Vertex in the Graph, then
 * through each Edge in the Vertex and prints
 * it's end Vertex's cityName. Shows all the
 * connections that form the Adjacency List.
 */
void printAdjList(Graph* graph){

    int i;
    int j;
    for (i = 0; i < graph->numberOfCities; i++){
        // Print Vertex i
        printf("\n%s ->", graph->adjLists[i]->cityName);
        for (j = 0; j < graph->adjLists[i]->numberOfEdges; j++){
            // Print vertices connected to Vertex i
            printf(" %d %s ->", graph->adjLists[i]->edges[j]->distance, graph->adjLists[i]->edges[j]->end->cityName);
        }
    }
    printf("\n");
}


/*
 * Function: graphGetNumberOfCities
 * --------------------------------
 * Returns the number of unique cities or Vertices.
 */
int graphGetNumberOfCities(Graph* graph){
    return graph->numberOfCities;
}


/*
 * Function: graphGetVertex
 * ------------------------
 * Returns a pointer to a Vertex of a specific
 * vertexNumber.
 */
Vertex* graphGetVertex(Graph* graph, int vertexNumber){
    return graph->adjLists[vertexNumber];
}


/*
 * Function: graphGetVertexNumber
 * ------------------------------
 * Linear searches through the vertices
 * comparing the char* string variable with
 * ones in the array. Returns it's index if known
 * and a -1 if not found.
 */
int graphGetVertexNumber(Graph* graph, char* string){

    int i;
    for(i = 0; i < graph->numberOfCities; i++){
        if(!strcmp(graph->adjLists[i]->cityName, string)){
            return i;
        }
    }
    printf("\n#####################################\n");
    printf("Error, city name %s is not known.\n", string);
    printf("#####################################\n\n");
    exit(-1);
}
