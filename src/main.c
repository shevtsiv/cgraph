#include <stdlib.h>
#include <stdio.h>
#include "graph.c"

int main() {
    Graph *graph = createGraph(5);
    addNode(graph, 1);
    addNode(graph, 5);
    addNode(graph, 10);
    addNode(graph, 11);
    addNode(graph, 6);
    addLine(graph, 1, 5);
    addLine(graph, 1, 6);
    addLine(graph, 1, 10);

    addLine(graph, 5, 10);
    addLine(graph, 5, 11);

    addLine(graph, 6, 5);

    addNode(graph, 9);
    addLine(graph, 1, 9);
    
    addNode(graph, 54);
    addLine(graph, 9, 54);

    printGraph(graph);
    freeGraph(graph);
    return 0;
}
