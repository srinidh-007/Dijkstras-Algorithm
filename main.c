/*
 * main.c
 * ------
 *
 * Programmer: James Gardner
 * University of York
 * Date 25/01/2019
 *
 * This is the main function of the assignment for
 * Algorithms and Numerical Methods. First it imports two
 * files. One, "ukcities.txt", containing a pair of city names
 * with distances between them. The other file, "citypairs.txt",
 * contains a pair of city names. The program then generates
 * a graph structure of the cities. Then uses Dijkstra's
 * shortest path algorithm alongside a minHeap data structure
 * to calculate the shortest path. It outputs the
 * shortest distance between the two cites in "citypairs.txt"
 * and stores the result in a file called "output.txt".
 * "output.txt" includes the list of the cities on the route and
 * the overall distance.
 */

#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <minheap.h>
#include <dijkstras.h>

#define STRING_SIZE 250 // Maximum length of a string

void fastestRoute(FILE* citypairs, FILE* output, Graph* graph, MinHeap* minHeap);

/*
 * Function: main
 * --------------
 * Opens necessary files, makes calls to other functions
 * setting up graph and minheap structures needed for
 * Dijkstra's algorithm. Runs Dijkstra's until all routes
 * in the citypairs.txt file are complete and then frees
 * all dynamically allocated memory.
 */
int main(){

    // Importing text files and checking if they opened correctly
    // This is the file of cities and distances
    FILE* ukcities = NULL;

    if ((ukcities = fopen("ukcities.txt", "r")) == NULL){
        fprintf(stderr, "error: file open failed 'ukcities.txt'.\n");
        return -1;
    }

    printf("FILE ukcities.txt opened successfully.\n\n");

    // This is the cities of which the shortest distance between
    // is to be calculated
    FILE* citypairs = NULL;

    if ((citypairs = fopen("citypairs.txt", "r")) == NULL){
        fprintf(stderr, "error: file open failed 'citypairs.txt'.\n");
        return -1;
    }

    printf("FILE citypairs.txt opened successfully.\n\n");

    // This is the output file that the results will be written too
    FILE* output = NULL;

    // Opens file "output.txt" or creates one if not there
    output = fopen("output.txt", "w");

    printf("FILE output.txt opened successfully.\n\n");

    // Create an empty graph structure
    Graph* graph = graphConstructor();

    // Fill the graph with cities and their connections
    graphPopulateGraph(graph, ukcities);


    // Create an empty minimum heap structure
    MinHeap* minHeap = minHeapConstructor();

    printf("Calculating fastest routes...\n\n");

    fastestRoute(citypairs, output, graph, minHeap);

    printf("Fastest routes have been saved into output.txt.\n\n");

    printf("Program terminating...\n\n");

    // Freeing all dynamically allocated memory
    graphDestructor(graph);
    minHeapDestructor(minHeap);

    // Closing files
    fclose(ukcities);
    fclose(citypairs);
    fclose(output);

    printf("Press 'Enter' to close.\n");
    getchar();
    return 0;
}


/*
 * Function: fastestRoute
 * ----------------------
 * Reading routes to calculate from citypairs file
 * calculating their associated vertexNumber and
 * running Dijkstra's algorithm using that number.
 */
void fastestRoute(FILE* citypairs, FILE* output, Graph* graph, MinHeap* minHeap){

    char* start = (char*)malloc(STRING_SIZE*sizeof(char));
    char* end = (char*)malloc(STRING_SIZE*sizeof(char));
    int startVertexNumber;
    int endVertexNumber;

    // Counting the number of lines
    // Used for error checking
    char character;
    int lineNumber = 0;
    while(!feof(citypairs)){
        character = fgetc(citypairs);
        if(character == '\n'){
            lineNumber++;
        }
    }

    // Sets the file position to the beginning of the file
    rewind(citypairs);

    //  %[^\t] - read any character that is not a Taking a worst-case scenario, the highest order of BigO of the entire program is:TAB
    //  \t the TAB
    //  \r the return carriage character
    //  \n the new line character
    // While fscanf has read three things, two strings and an integer on a line

    int i = 0;
    while(2 == fscanf(citypairs, "%[^\t]\t%[^\t]\r\n", start, end)){
        // Getting associated Vertex number of cities
        // or error if city is not known
        i++;
        startVertexNumber = graphGetVertexNumber(graph, start);
        endVertexNumber = graphGetVertexNumber(graph, end);
        dijkstras(graph, minHeap, startVertexNumber);
        dijkstrasWriteToFile(graph, output, startVertexNumber, endVertexNumber);
    }
    // If the number of lines read by the
    // file when importing data isn't equal
    // to the number of lines in the file
    // report an error.
    if(i != lineNumber){
        printf("\n######################################\n");
        printf("There was an error reading 'citypairs.txt' line: %d\n\n", i);
        printf("Please check each line of the file contains two,\n");
        printf("tab-delimited, strings.\n");
        printf("######################################\n\n");
        exit(-1);
    }

    free(start);
    free(end);
}
