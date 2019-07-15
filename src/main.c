#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "graph.c"

int indexOfChar(char line[], char character) {
    char *substring = strchr(line, character);
    if (substring == NULL) {
        return -1;
    }
    return (substring - line);
}

int main() {
    printf("Choose input type:\n");
    printf("1) Keyboard - Press 1\n");
    printf("2) File - Press 2\n");
    int inputType = 0;
    while (!(scanf("%i", &inputType) && (inputType == 1 || inputType == 2))) {
        printf("Incorrect input type! Please, try again...\n");
        while ((getchar()) != '\n');
    }
    FILE * stream  = NULL;
    if (inputType == 1) {
        printf("Keyboard input type has been chosen!\n");
        printf("In the first line input Graph size.\n");
        printf("In each line after type 2 digits, separated by space to represent a line.\n");
        printf("End typing by any non-digit symbol.\n");
        while ((getchar()) != '\n');
        stream = stdin;
    } else {
        printf("File input type has been chosen!\n");
        printf("Input file name:\n");
        while ((getchar()) != '\n');
        char fileName[30];
        while (stream == NULL) {
            scanf("%s", fileName);
            stream = fopen(fileName, "r");
            if (stream == NULL) {
                printf("Cannot access file with such name! Please, try again:\n");
            }
            while ((getchar()) != '\n');
        }
    }
    Graph *graph = NULL;
    char line[10];
    while (fgets(line, 10, stream)) {
        if (line[0] < '0' || line[0] > '9') {
            break;
        }
        if (graph == NULL) {
            graph = createGraph(strtoul(line, NULL, 10));
            if (graph == NULL) {
                printf("Failed to create a graph!");
                return -1;
            }
            continue;
        }
        int spaceIndex = indexOfChar(line, ' ');
        char sourceIndexLine[10] = {0};
        strncpy(sourceIndexLine, line, spaceIndex);
        size_t sourceIndex = strtoul(sourceIndexLine, NULL, 10);
        char destinationIndexLine[10] = {0};
        strncpy(destinationIndexLine, line + spaceIndex, strlen(line));
        size_t destinationIndex = strtoul(destinationIndexLine, NULL, 10);
        addLine(graph, sourceIndex, destinationIndex);
    }
    fclose(stream);
    if (isGraphConnected(graph)) {
        printf("Graph is connected!\n");
    } else {
        printf("Graph is not connected!\n");
    }
    Stack *longestGraphCycle = getLongestGraphCycle(graph);
    if (longestGraphCycle != NULL) {
        printf("Longest graph cycle is:\n");
        printStack(longestGraphCycle);
        printf("\n");
    } else {
        printf("Graph has no cycles!\n");
    }
    freeStack(longestGraphCycle);
    freeGraph(graph);
    return 0;
}
