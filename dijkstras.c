/*
 * dijkstras.c
 * -----------
 *
 * Programmer: James Gardner
 * University of York
 * Date 24/01/2019
 *
 * This file contains functions for calculating the shortest
 * path between a source Vertex and all other vertices of a
 * Graph using a minHeap to increase efficiency. Dijkstra's
 * shortest path algorithm is used and INT_MAX is used in place
 * of the infinite initial distance to all other Vertices. This
 * is not perfect and could be improved by changing from integer
 * distances to floats as +infinity can be represented in floats
 * or by using a flag variable. This might be implemented in a
 * future update.
 */

#include <stdio.h>
#include <stdlib.h>
#include <dijkstras.h>
#include <limits.h>


#define STRING_SIZE 250 // Maximum length of string


/*
 * Function: dijkstras
 * -------------------
 * Takes a filled Graph structure, resets all distancesFromSource
 * previous and notVisited variables in case of a previous run
 * and generates a minHeap of distances from source Vertex to all
 * other Vertices. Then runs through Dijkstra's algorithms to find
 * shortest path from source Vertex to all other Vertices and updates
 * information within the Graph Vertices.
 */
void dijkstras(Graph* graph, MinHeap* minHeap, int source){

    // Resetting Vertex variables for Dijkstra's in case of previous run through
    vertexSetDistanceFromSource(graphGetVertex(graph, source), 0);

    int i;
    // For all Vertices in the graph
    for(i = 0; i < graphGetNumberOfCities(graph); i++){
        if(i != source){
            // Setting distance to INT_MAX to play the role of infinite
            // distance. If actual distances are over +2147483647 then
            // this will break.
            vertexSetDistanceFromSource(graphGetVertex(graph, i), INT_MAX);
        }
        vertexSetPrevious(graphGetVertex(graph, i), NULL);
        vertexSetIsNotVisited(graphGetVertex(graph, i));
        // Add newly set-up Vertex to minHeap.
        minHeapEnqueue(minHeap, graphGetVertex(graph, i));
    }

    while(!minHeapIsEmpty(minHeap)){

        Vertex* u;
        Vertex* v;
        int alternateRoute;
        Edge* edgeOfU;

        // Get the Vertex with shortest distance from source
        u = minHeapDequeue(minHeap);

        int j;
        // For all edges of the Vertex* u
        for (j = 0; j < vertexGetNumberOfEdges(u); j++){

            // Retrieve the Edge pointer
            edgeOfU = vertexGetEdge(u, j);

            // Retrieve the Vertex pointer the Edge connects with
            v = edgeGetEndVertex(edgeOfU);

            // If that Vertex's isVisted variable is set to zero
            if(!(vertexIsVisited(v))){

                alternateRoute = vertexGetDistanceFromSource(u) + edgeGetDistance(edgeOfU);

                // If new shorter path found
                if(alternateRoute < vertexGetDistanceFromSource(v)){

                    // Update Vertex
                    vertexSetDistanceFromSource(v, alternateRoute);
                    vertexSetPrevious(v, u);

                    // Update minHeap
                    minHeapDecreaseNodeValue(minHeap, v);
                }
            }
        }
        vertexSetIsVisited(u);
    }
}


/*
 * Function: dijkstrasPrintResult
 * ------------------------------
 * Prints out the complete shortest path information.
 * Distance from source Vertex to all other Vertices and
 * all Vertices previous Vertex.
 */
void dijkstrasPrintResult(Graph* graph){

    printf("%-10s%-20s%-20s%-20s\n", "Vertex", "CityName", "Distance", "Previous");
    printf("%41s\n\n", "From Source");

    Vertex* vertex;

    int i;
    for(i = 0; i < graphGetNumberOfCities(graph); i++){
        vertex = graphGetVertex(graph, i);

        // If source Vertex
        if(vertexGetDistanceFromSource(graphGetVertex(graph, i)) == 0){
            // Prints slightly different output as has no previous Vertex
            printf("%-10d%-20s%-10d%20s\n", i, vertexGetCityName(vertex),
                   vertexGetDistanceFromSource(vertex), "----------");

        } else {
            printf("%-10d%-20s%-20d%-20s\n", i, vertexGetCityName(vertex),
                   vertexGetDistanceFromSource(vertex), vertexGetPreviousCityName(vertex));
        }
    }
    printf("\n");
}


/*
 * Function: dijkstrasWriteToFile
 * ------------------------------
 * Writes the distance from source to destination Vertex
 * and the route taken to get there into a file. Because
 * the route taken is worked out backwards from destination
 * to source via previously pointers, initially it stores the
 * previous cityNames in a new Char** array and then runs through
 * that array backwards to print the journey in the correct
 * direction from Source to Destination.
 */
 void dijkstrasWriteToFile(Graph* graph, FILE* output, int sourceVertexNumber, int destinationVertexNumber){

    // Temp variables used to increase readability
    char* sourceName = vertexGetCityName(graphGetVertex(graph, sourceVertexNumber));
    char* destinationName = vertexGetCityName(graphGetVertex(graph, destinationVertexNumber));
    int distanceFromSource = vertexGetDistanceFromSource(graphGetVertex(graph, destinationVertexNumber));

    // Stores route in reverse
    char** routeCityNames = malloc(sizeof(char*));
    routeCityNames[0] = malloc(STRING_SIZE*sizeof(char));
    routeCityNames[0] = destinationName;


    fprintf(output, "%s to %s is %dkm\n\n", sourceName, destinationName, distanceFromSource);
    fprintf(output, "Route:\n");

    int i = destinationVertexNumber;
    int count = 1;

    // While previous Vertex* is not NULL e.g Not the Source Vertex
    while(vertexGetPrevious(graphGetVertex(graph, i)) != NULL){
        // Increase size of routeCityNames array
        routeCityNames = realloc(routeCityNames, (count + 1)*sizeof(char*));
        routeCityNames[count] = malloc(STRING_SIZE*sizeof(char));

        // Add name of previous city to the list
        routeCityNames[count] = vertexGetCityName(vertexGetPrevious(graphGetVertex(graph, i)));

        // Update i with the vertexNumber of the previous city
        i = vertexGetVertexNumber(vertexGetPrevious(graphGetVertex(graph, i)));
        count++;
    }

    // Loop through route from Source
    for(i = count - 1; i > 0 ; i--){
        fprintf(output, "%s ---> ", routeCityNames[i]);
    }

    fprintf(output, "%s\n\n", routeCityNames[0]);
    fprintf(output, "\n\n");

    for(i = 0; i > count ; i++){
        free(routeCityNames[i]);
    }
    free(routeCityNames);
}

